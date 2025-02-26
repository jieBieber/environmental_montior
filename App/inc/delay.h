#ifndef __DELAY_H
#define __DELAY_H 	

#include "stm32f10x.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//使用SysTick的普通计数模式对延迟进行管理（适合STM32F10x系列）
//包括delay_us,delay_ms
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2010/1/1
//版本：V1.8
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
////////////////////////////////////////////////////////////////////////////////// 
	 
	 
extern uint16_t time2Count;

// 裸机使用的延时函数
//void delay_init(void);
//void delay_ms(u16 nms);
//void delay_us(u32 nus);

//// 使用定时器2完成的延时函数
//void Tim2Delay_us(__IO uint32_t nTime);

uint32_t DWT_Dalay_Init(void);
//延时函数,参数是n微秒
void DWT_Delay_us(volatile uint32_t nus);

#endif





























