/*
 * Copyright 2020-2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @brief rk024hh298 display driver init through spi interface.
 */

#include "sln_ili9341.h"

void FT9341_Init(ili9341_send_byte_t _writeData, ili9341_send_byte_t _writeCommand)
{
    _writeCommand(ILI9341_CMD_PGAMMA);
    _writeData(0x84);
    _writeData(0x00);
    _writeData(0x7a);

    _writeCommand(ILI9341_CMD_PWRB);
    _writeData(0x00);
    _writeData(0x81);
    _writeData(0X30);

    _writeCommand(ILI9341_CMD_DTCB);
    _writeData(0x00);
    _writeData(0x00);

    _writeCommand(ILI9341_CMD_PWR1);
    _writeData(0x21);

    _writeCommand(ILI9341_CMD_PWR2);
    _writeData(0x11);

    _writeCommand(ILI9341_CMD_VCOM1);
    _writeData(0x41);
    _writeData(0x3c);

    _writeCommand(ILI9341_CMD_VCOM2);
    _writeData(0xac);

    _writeCommand(ILI9341_CMD_MAC);
    _writeData(0x48);

    _writeCommand(ILI9341_CMD_PIXELFORMAT);
    _writeData(0x55);

    _writeCommand(ILI9341_CMD_3GAMMAEN);
    _writeData(0x00);

    _writeCommand(ILI9341_CMD_GAMMA);
    _writeData(0x01);

    _writeCommand(ILI9341_CMD_PGAMMA);
    _writeData(0x0F);
    _writeData(0x38);
    _writeData(0x32);
    _writeData(0x0C);
    _writeData(0x0e);
    _writeData(0x07);
    _writeData(0x4f);
    _writeData(0X98);
    _writeData(0x39);
    _writeData(0x07);
    _writeData(0x11);
    _writeData(0x04);
    _writeData(0x13);
    _writeData(0x11);
    _writeData(0x00);

    _writeCommand(ILI9341_CMD_NGAMMA);
    _writeData(0x00);
    _writeData(0x0d);
    _writeData(0x0e);
    _writeData(0x04);
    _writeData(0x12);
    _writeData(0x08);
    _writeData(0x31);
    _writeData(0x78);
    _writeData(0x46);
    _writeData(0x08);
    _writeData(0x10);
    _writeData(0x0d);
    _writeData(0x2b);
    _writeData(0x33);
    _writeData(0x0F);

    _writeCommand(ILI9341_CMD_RGBINTERFACE);
    _writeData(0xc2);

    _writeCommand(ILI9341_CMD_INTERFACE);
    _writeData(0x00);
    _writeData(0x00);
    _writeData(0x06);

    _writeCommand(ILI9341_CMD_SLEEPOUT);
    _writeCommand(ILI9341_CMD_DISPLAYON);
}
