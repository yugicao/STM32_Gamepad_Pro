#include "stdio.h"
#include "menu_handle.h"
#include "ssd1306.h"
#include "pong.h"
#include "saved_configs.h"

extern GamepadConfig_t config;

static uint8_t menu_row_index = 1;
MenuPosition_t menu_position = MAIN_MENU;

void pong_game_run(void);

void menu_reset(void)
{
	menu_position = MAIN_MENU;
	menu_row_index = 1;
	display_main_menu(0, 1);
}

void menu_init()
{
	SSD1306_Init ();
	menu_position = MAIN_MENU;
	menu_row_index = 1;
	display_main_menu(0, 1);
}


void menu_click_up(void)
{
	click_handle(0, 0, 0, 1);
}
void menu_click_down(void)
{
	click_handle(0, 0, 0, 0);
}
void menu_click_select(void)
{
	click_handle(1, 0, 1, 0);
}
void menu_click_back(void)
{
	click_handle(0, 1, 1, 0);
}

__weak void connect_mode_callback(void)
{

}

void click_handle(uint8_t isSelect, uint8_t isBack, uint8_t isReset, uint8_t isUp)
{
	if(isSelect)
	{
		switch(menu_position)
		{
			case MAIN_MENU:
				menu_position = menu_row_index;
				if(menu_row_index == 1)
				{
					connect_mode_callback();
				}
				break;
			case CONNECTING:
				break;
			case LOAD_CONFIG_MENU:{
				read_config(menu_row_index, &config);
				if(config.mode == 0x00)
				{
					config.joystickUpID = W_USAGE_ID;
					config.joystickDownID = S_USAGE_ID;
					config.joystickRightID = D_USAGE_ID;
					config.joystickLeftID = A_USAGE_ID;
				}
				else if(config.mode == 0x01)
				{
					config.joystickUpID = ARROW_UP_USAGE_ID;
					config.joystickDownID = ARROW_DOWN_USAGE_ID;
					config.joystickRightID = ARROW_RIGHT_USAGE_ID;
					config.joystickLeftID = ARROW_LEFT_USAGE_ID;
				}
				menu_position = MAIN_MENU;
				break;
			}
			case OFFLINE_GAME_MENU:
				SSD1306_Fill(0);
				SSD1306_Stopscroll();
				pong_game_run();
				break;
			default: break;
		}
	}

	if(isBack)
	{
		switch(menu_position)
		{
			case MAIN_MENU:
				break;
			case CONNECTING:
				break;
			case LOAD_CONFIG_MENU:
				menu_position = MAIN_MENU;
				break;
			case OFFLINE_GAME_MENU:
				menu_position = MAIN_MENU;
				break;
			default: break;
		}
	}

	switch(menu_position)
	{
		case MAIN_MENU:
			display_main_menu(isUp, isReset);
			break;
		case CONNECTING:
			break;
		case LOAD_CONFIG_MENU:
			display_load_config_menu(isUp, isReset);
			break;
		case OFFLINE_GAME_MENU:
			display_game_offline_menu(isUp, isReset);
			break;
		default: break;
	}
}

void display_updated_wifi(void)
{
//	SSD1306_GotoXY (0, SSID_INTERFACE_LINE_Y);
//
//	char buffer[64];
//    sprintf(buffer, "SSID: %s", SSID);
//
//    SSD1306_Puts (buffer, &Font_7x10, 1);
//	SSD1306_UpdateScreen();
	switch(menu_position)
	{
		case MAIN_MENU:
			display_main_menu(0, 1);
			break;
		case CONNECTING:
			break;
		case LOAD_CONFIG_MENU:
			display_load_config_menu(0, 1);
			break;
		case OFFLINE_GAME_MENU:
			display_game_offline_menu(0, 1);
			break;
		default: break;
	}
}

void display_main_menu(uint8_t isUp, uint8_t isReset)
{
	SSD1306_Fill(0);
	SSD1306_Stopscroll();
	SSD1306_GotoXY (0, SSID_INTERFACE_LINE_Y);

	char buffer[64];
    sprintf(buffer, "SSID: %s", SSID);
	SSD1306_Puts (buffer, &Font_7x10, 1);

	SSD1306_GotoXY (0, LINE_1_Y);
	SSD1306_Puts ("     Connect", &Font_7x10, 1);
	SSD1306_GotoXY (0, LINE_2_Y);
	SSD1306_Puts ("    Load Config", &Font_7x10, 1);
	SSD1306_GotoXY (0, LINE_3_Y);
	SSD1306_Puts ("   Offline Game", &Font_7x10, 1);
	SSD1306_UpdateScreen();
	if(isReset)
	{
		menu_row_index = 1;
		SSD1306_ScrollRight(1, 2);
		return;
	}
	if(isUp)
	{
		if((--menu_row_index) <= 0)
			menu_row_index = MAIN_MENU_ITEM_COUNT;
	}
	else
	{
		if((++menu_row_index) > MAIN_MENU_ITEM_COUNT)
			menu_row_index = 1;
	}
	switch(menu_row_index)
	{
		case 1:
			SSD1306_ScrollRight(1, 2);
			break;
		case 2:
			SSD1306_ScrollRight(3, 4);
			break;
		case 3:
			SSD1306_ScrollRight(5, 6);
			break;
		default: break;
	}
}



void display_game_offline_menu(uint8_t isUp, uint8_t isReset)
{
	SSD1306_Fill(0);
	SSD1306_Stopscroll();
	SSD1306_GotoXY (0, SSID_INTERFACE_LINE_Y);

	char buffer[64];
    sprintf(buffer, "SSID: %s", SSID);
	SSD1306_Puts (buffer, &Font_7x10, 1);

	SSD1306_GotoXY (0, LINE_1_Y);
	SSD1306_Puts ("    PONG GAME", &Font_7x10, 1);

	SSD1306_UpdateScreen();
	if(isReset)
	{
		menu_row_index = 1;
		SSD1306_ScrollRight(1, 2);
		return;
	}
	if(isUp)
	{
		if((--menu_row_index) <= 0)
			menu_row_index = GAME_MENU_ITEM_COUNT;
	}
	else
	{
		if((++menu_row_index) > GAME_MENU_ITEM_COUNT)
			menu_row_index = 1;
	}
	switch(menu_row_index)
	{
		case 1:
			SSD1306_ScrollRight(1, 2);
			break;
		case 2:
			SSD1306_ScrollRight(3, 4);
			break;
		case 3:
			SSD1306_ScrollRight(5, 6);
			break;
		default: break;
	}
}

void pong_game_run(void)
{
	  static bool game_over = FALSE;
//	  static ballPos ball_x_local = 0;
//	  static ballPos ball_y_local = 0;
	  reset_game();

	  game_over = FALSE;

	  pong_start_screen();

	  pong_wait_for_button_press();

	  while(!game_over)
	  {
		  pong_move_paddle();

		  if(pong_ball_at_paddle()){
			  if(ball_in_paddle()){
				  bounce_off_paddle();
			  } else {
				  pong_game_over();
				  pong_wait_for_button_press();
				  game_over = TRUE;
			  }
		  } else {
			  pong_update_ball_pos();
		  }


//		  ball_x_local = get_ball_x();
//		  ball_y_local = get_ball_y();

		  if(!game_over)
		  {
			  pong_display_frame();
		  }
	  }
	  return;
}


void display_load_config_menu(uint8_t isUp, uint8_t isReset)
{
	SSD1306_Fill(0);
	SSD1306_Stopscroll();
	SSD1306_GotoXY (0, SSID_INTERFACE_LINE_Y);

	char buffer[64];
    sprintf(buffer, "SSID: %s", SSID);
	SSD1306_Puts (buffer, &Font_7x10, 1);
//	SSD1306_Puts ("SSID:", &Font_7x10, 1);

	SSD1306_GotoXY (0, LINE_1_Y);
	SSD1306_Puts ("     CONFIG 1", &Font_7x10, 1);
	SSD1306_GotoXY (0, LINE_2_Y);
	SSD1306_Puts ("     CONFIG 2", &Font_7x10, 1);
	SSD1306_GotoXY (0, LINE_3_Y);
	SSD1306_Puts ("     CONFIG 3", &Font_7x10, 1);

	SSD1306_UpdateScreen();
	if(isReset)
	{
		menu_row_index = 1;
		SSD1306_ScrollRight(1, 2);
		return;
	}
	if(isUp)
	{
		if((--menu_row_index) <= 0)
			menu_row_index = CONFIGS_ITEM_COUNT;
	}
	else
	{
		if((++menu_row_index) > CONFIGS_ITEM_COUNT)
			menu_row_index = 1;
	}
	switch(menu_row_index)
	{
		case 1:
			SSD1306_ScrollRight(1, 2);
			break;
		case 2:
			SSD1306_ScrollRight(3, 4);
			break;
		case 3:
			SSD1306_ScrollRight(5, 6);
			break;
		default: break;
	}
}

void display_connect_state(uint8_t connect_state)
{
	SSD1306_Fill(0);
	SSD1306_Stopscroll();
	SSD1306_GotoXY (0, SSID_INTERFACE_LINE_Y);

	char buffer[64];
    sprintf(buffer, "SSID: %s", SSID);
	SSD1306_Puts (buffer, &Font_7x10, 1);
	SSD1306_GotoXY (0, LINE_2_Y);

	if(connect_state == 0x03)
	{
		SSD1306_Puts ("    CONNECTED", &Font_7x10, 1);
	}
	else
	{
		SSD1306_Puts ("  NOT CONNECTED", &Font_7x10, 1);
	}
	SSD1306_UpdateScreen();

}
