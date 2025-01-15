/*
 * keyboard.c
 *
 *  Created on: Oct 30, 2024
 *      Author: Dell
 */
#include "keyboard.h"
#include "usbd_hid.h"
#include "usb_device.h"

extern USBD_HandleTypeDef hUsbDeviceFS;

static uint8_t buffer[REPORT_SIZE] = {1, 0, 0, 0, 0, 0, 0, 0};

void pressKey(uint8_t key)
{
//    if (key == 0xE0)
//    {
//        buffer[1] |= (1 << 0);
//    }
//    else if (key == 0xE1)
//    {
//        buffer[1] |= (1 << 1);
//    }
    if (key == 0xE0 || key == 0xE1 || key == 0xE2 || key == 0xE3 ||
        key == 0xE4 || key == 0xE5 || key == 0xE6 || key == 0xE7)
    {
        buffer[1] |= (1 << (key - 0xE0));
    }
    else
    {
        for (int i = 3; i < REPORT_SIZE; i++)
        {
            if (buffer[i] == 0)
            {
                buffer[i] = key;
                break;
            }
        }
    }
    USBD_HID_SendReport(&hUsbDeviceFS, buffer, REPORT_SIZE);
}

void releaseKey(uint8_t key)
{
//    if (key == 0xE0)
//    {
//        buffer[1] &= ~(1 << 0);
//    }
//    else if (key == 0xE1)
//    {
//        buffer[1] &= ~(1 << 1);
//    }
    if (key == 0xE0 || key == 0xE1 || key == 0xE2 || key == 0xE3 ||
        key == 0xE4 || key == 0xE5 || key == 0xE6 || key == 0xE7)
    {
        buffer[1] &= ~(1 << (key - 0xE0));
    }
    else
    {
        for (int i = 3; i < REPORT_SIZE; i++)
        {
            if (buffer[i] == key)
            {
                buffer[i] = 0;
                break;
            }
        }
    }
    USBD_HID_SendReport(&hUsbDeviceFS, buffer, REPORT_SIZE);
}

void releaseAllKeys(void)
{
	buffer[0] = 1;
    for (int i = 1; i < REPORT_SIZE; i++)
    {
        buffer[i] = 0;
    }

    USBD_HID_SendReport(&hUsbDeviceFS, buffer, REPORT_SIZE);
}

