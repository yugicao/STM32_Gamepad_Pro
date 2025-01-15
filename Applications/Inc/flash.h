#ifndef __FLASH_H
#define __FLASH_H
#include "main.h"

void flash_unlock(void);

void flash_lock(void);

void flash_erase(uint32_t addr);

void flash_write_arr(uint32_t addr, uint8_t *data, uint16_t len);

void flash_read_arr(uint32_t addr, uint8_t *data, uint16_t len);

#endif

