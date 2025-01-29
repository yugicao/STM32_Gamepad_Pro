#ifndef MENU_HANDLE_H
#define MENU_HANDLE_H

#include "main.h"

#define SSID_BUFFER_MAX_LEN        13
#define SSID_INTERFACE_LINE_Y      0
#define LINE_1_Y                   15
#define LINE_2_Y                   30
#define LINE_3_Y                   45

#define MAIN_MENU_ITEM_COUNT       3
#define GAME_MENU_ITEM_COUNT       1
#define CONFIGS_ITEM_COUNT         3

#define SCROLL_LINE_1              1, 2
#define SCROLL_LINE_2              3, 4
#define SCROLL_LINE_3              5, 6

typedef enum {
    MAIN_MENU = 0,
    CONNECTING,
    LOAD_CONFIG_MENU,
    OFFLINE_GAME_MENU
} MenuPosition_t;

void display_main_menu(uint8_t isUp, uint8_t isReset);
void display_game_offline_menu(uint8_t isUp, uint8_t isReset);
void display_load_config_menu(uint8_t isUp, uint8_t isReset);
void display_connect_state(uint8_t connect_state);
void display_updated_wifi(void);

void click_handle(uint8_t isSelect, uint8_t isBack, uint8_t isReset, uint8_t isUp);
void menu_click_up(void);
void menu_click_down(void);
void menu_click_select(void);
void menu_click_back(void);
void menu_init(void);
void menu_reset(void);

__weak void connect_mode_callback(void);

#endif
