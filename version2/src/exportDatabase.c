#include "../main.h"

int exportDatabase(MYSQL *conn, const char *dbName) {

    const char *filename = "exported_database.sql";

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing\n");
        return 1;
    }

    MYSQL_RES *tablesResult;
    char tablesQuery[512];
    snprintf(tablesQuery, sizeof(tablesQuery), "SHOW TABLES FROM %s", dbName);
    if (mysql_query(conn, tablesQuery) != 0) {
        fprintf(stderr, "Error fetching tables information: %s\n", mysql_error(conn));
        fclose(file);
        return 1;
    }

    tablesResult = mysql_store_result(conn);

    MYSQL_ROW tableRow;
    while ((tableRow = mysql_fetch_row(tablesResult)) != NULL) {
        char *tableName = tableRow[0];

        fprintf(file, "CREATE TABLE `%s` (\n", tableName);

        MYSQL_RES *columnsResult;
        char columnsQuery[512];
        snprintf(columnsQuery, sizeof(columnsQuery), "SHOW COLUMNS FROM `%s` IN %s", tableName, dbName);
        if (mysql_query(conn, columnsQuery) != 0) {
            fprintf(stderr, "Error fetching columns information: %s\n", mysql_error(conn));
            fclose(file);
            mysql_free_result(tablesResult);
            return 1;
        }
        columnsResult = mysql_store_result(conn);

        MYSQL_ROW columnRow;
        int firstColumn = 1;
        
        while ((columnRow = mysql_fetch_row(columnsResult)) != NULL) {
            char *columnName = columnRow[0];
            char *columnType = columnRow[1];

            if (!firstColumn) {
                fprintf(file, ",\n");
            }
            fprintf(file, "  `%s` %s", columnName, columnType);

            firstColumn = 0;
        }

        mysql_free_result(columnsResult);

        fprintf(file, "\n);\n\n");
    }

    mysql_free_result(tablesResult);
    fclose(file);
    return 0;
}