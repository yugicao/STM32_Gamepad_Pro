#include "saved_configs.h"
#include "flash.h"

const uint32_t Config_Address[] = {CONFIG1_ADDRESS, CONFIG2_ADDRESS, CONFIG3_ADDRESS};

//void write_config(uint8_t configID, GamepadConfig_t data)
//{
//	flash_unlock();
//	flash_erase(Config_Address[configID + 1]);
//	flash_write_arr(Config_Address[configID + 1], (uint8_t *)&data, CONFIG_DATA_LEN);
//	flash_lock();
//}

void write_config(GamepadConfig_t* data)
{
	flash_unlock();
	flash_erase(Config_Address[data->configSelect - 1]);
	flash_write_arr(Config_Address[data->configSelect - 1], (uint8_t *)data, CONFIG_DATA_LEN);
	flash_lock();
}

void read_config(uint8_t configID, GamepadConfig_t* data)
{
    if (configID < 1 || configID > 3) return;

	flash_read_arr(Config_Address[configID - 1], (uint8_t *)data, CONFIG_DATA_LEN);
}

