#ifndef PONG_H
#define PONG_H

#include "stm32f1xx_hal.h"
#include "ssd1306.h"

#define BUTTON_LEFT HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12)
#define BUTTON_RIGHT HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13)
#define BUTTON_UP HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14)
#define BUTTON_DOWN HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15)

#define TRUE 1
#define FALSE 0

typedef int16_t ballPos;
typedef uint8_t bool;

void pong_move_paddle(void);

void pong_start_screen(void);

void pong_wait_for_button_press(void);

void pong_display_frame(void);

void reset_game(void);

bool pong_ball_at_paddle(void);

bool ball_in_paddle(void);

void pong_update_ball_pos(void);

void pong_game_over(void);

ballPos get_ball_x(void);

ballPos get_ball_y(void);

void bounce_off_paddle(void);


#endif
