#ifndef MAIN_MENU_H
#define MAIN_MENU_H

enum ProgramState {
    STATE_MAIN_MENU,
    STATE_CREATE_ACCOUNT,
    STATE_CONNECT_ACCOUNT,
    // ... autres états
};

extern enum ProgramState currentState;

#endif
