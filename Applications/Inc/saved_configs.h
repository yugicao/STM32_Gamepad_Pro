#ifndef __SAVED_CONFIGS_H
#define __SAVED_CONFIGS_H

#include "main.h"
#define CONFIG_DATA_LEN sizeof(GamepadConfig_t)

#define CONFIG1_ADDRESS 0x0801F000
#define CONFIG2_ADDRESS 0x0801F400
#define CONFIG3_ADDRESS 0x0801F800

//#define CONFIG1_ADDRESS 0x0800E800  // Page 58
//#define CONFIG2_ADDRESS 0x0800F000  // Page 60
//#define CONFIG3_ADDRESS 0x0800F800  // Page 62


#define W_USAGE_ID 0x1A
#define A_USAGE_ID 0x04
#define S_USAGE_ID 0x16
#define D_USAGE_ID 0x07

#define ARROW_UP_USAGE_ID 0x48
#define ARROW_DOWN_USAGE_ID 0x50
#define ARROW_RIGHT_USAGE_ID 0x4D
#define ARROW_LEFT_USAGE_ID 0x4B

typedef struct {
    uint8_t configSelect;
    uint8_t mode;
    uint8_t joystickButtonID;
    uint8_t button1ID;
    uint8_t button2ID;
    uint8_t button3ID;
    uint8_t button4ID;
    uint8_t joystickUpID;
    uint8_t joystickDownID;
    uint8_t joystickLeftID;
    uint8_t joystickRightID;
}__attribute__((__packed__)) GamepadConfig_t;


//void write_config(uint8_t configID, GamepadConfig_t data);

void write_config(GamepadConfig_t* data);

void read_config(uint8_t configID, GamepadConfig_t* data);


#endif
