#ifndef _FLASH_H__
#define _FLASH_H__

#include "Qitas.h"
#include "stm32f10x_flash.h"
#include <stdio.h>

/*
 *      -------------------- 0x8020000
 *      |                  |
 *      |      LOG区       |
 *      |                  |
 *      -------------------- 0x8010000
 *      |    全局配置区    |
 *      -------------------- 0x800FC00
 *      |  上报数据保存区  |
 *      -------------------- 0x800E000
 *      |                  |
 *      |      代码区      |
 *      |                  |
 *      -------------------- 0x8000000
 */

#define LOG_WRITE_ADDR    0x8010200
#define CONFIG_BASE_ADDR  0x800FE00
#define REPORT_BASE_ADDR  0x800E200
#define FLASH_BUTT_ADDR   0x8020200

int STORE_Flash(uint32_t ADDR,uint32_t value);
uint8_t READ_Flash(uint32_t ADR,uint32_t* buff);
void Flash_Write(uint32_t addr,uint32_t *buf,uint32_t num);
uint8_t Flash_Read(uint32_t addr,uint32_t *buf,uint32_t num);

#endif
