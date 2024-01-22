#include "../main.h"

size_t write_callback(void *contents, size_t size, size_t nmemb, FILE *userp) {
    return fwrite(contents, size, nmemb, userp);
}

void download_image(const char *url, const char *output_file) {
    CURL *curl;
    CURLcode res;

    FILE *fp = fopen(output_file, "wb");
    if (!fp) {
        fprintf(stderr, "Erreur d'ouverture du fichier pour écriture.\n");
        return;
    }

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if (curl) {
        // Définir l'URL de l'image
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Définir la fonction de rappel pour gérer les données reçues
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        // Enregistrer les données téléchargées dans le fichier
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        // Effectuer la requête
        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
            fprintf(stderr, "Erreur de curl_easy_perform : %s\n", curl_easy_strerror(res));

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    fclose(fp);
}

int downloadImg(SDL_Renderer *renderer){
    const char *image_url = "https://scontent-cdg4-2.xx.fbcdn.net/v/t39.30808-6/386081292_18018561505825160_4212432463964703339_n.jpg?_nc_cat=101&ccb=1-7&_nc_sid=3635dc&_nc_ohc=noZ1vPI315IAX8JKBbP&_nc_ht=scontent-cdg4-2.xx&oh=00_AfA7uAD1FBqTqMcJGLW6Gr_4yq5eVUnVQ5FkRtNfdheyHg&oe=65B293AA";
    const char *output_file = "download_image.jpg";
    download_image(image_url, output_file);

    // l'afficher
    SDL_RenderClear(renderer);

    SDL_Texture *option1Texture = IMG_LoadTexture(renderer, "download_image.jpg");
    SDL_Texture *backgroundTexture = IMG_LoadTexture(renderer, "img/banniere.png");

    SDL_Rect option1Rect = {600, 260, 720, 720};

    SDL_Event event;

    int quit = 0;
    
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    SDL_RenderCopy(renderer, option1Texture, NULL, &option1Rect);
    SDL_RenderPresent(renderer);

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
                quit = 1;
            }
        }
        SDL_Delay(10);
    }

    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(option1Texture);

    return 0;
}