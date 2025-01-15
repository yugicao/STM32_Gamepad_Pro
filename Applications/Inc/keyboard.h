/*
 * keyboard.h
 *
 *  Created on: Oct 30, 2024
 *      Author: Dell
 */

#ifndef INC_KEYBOARD_H_
#define INC_KEYBOARD_H_
#include "main.h"

#define REPORT_SIZE 8

#define KEY_A         0x04
#define KEY_B         0x05
#define KEY_C         0x06
#define KEY_D         0x07
#define KEY_E         0x08
#define KEY_F         0x09
#define KEY_G         0x0A
#define KEY_H         0x0B
#define KEY_I         0x0C
#define KEY_J         0x0D
#define KEY_K         0x0E
#define KEY_L         0x0F
#define KEY_M         0x10
#define KEY_N         0x11
#define KEY_O         0x12
#define KEY_P         0x13
#define KEY_Q         0x14
#define KEY_R         0x15
#define KEY_S         0x16
#define KEY_T         0x17
#define KEY_U         0x18
#define KEY_V         0x19
#define KEY_W         0x1A
#define KEY_X         0x1B
#define KEY_Y         0x1C
#define KEY_Z         0x1D
#define KEY_0         0x27
#define KEY_1         0x1E
#define KEY_2         0x1F
#define KEY_3         0x20
#define KEY_4         0x21
#define KEY_5         0x22
#define KEY_6         0x23
#define KEY_7         0x24
#define KEY_8         0x25
#define KEY_9         0x26
#define KEY_ENTER     0x28
#define KEY_ESCAPE    0x29
#define KEY_BACKSPACE 0x2A
#define KEY_TAB       0x2B
#define KEY_SPACE     0x2C
#define KEY_SHIFT     0xE1
#define KEY_CONTROL   0xE0
#define KEY_ALT       0xE2
#define KEY_ARROW_UP  0x52
#define KEY_ARROW_DOWN 0x51
#define KEY_ARROW_LEFT 0x50
#define KEY_ARROW_RIGHT 0x4F

void pressKey(uint8_t key);

void releaseKey(uint8_t key);

void releaseAllKeys(void);


#endif /* INC_KEYBOARD_H_ */
