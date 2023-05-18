#include "Oled.h"
#include "Oledfont.h"
#include "delay.h"

#define dealy_ms( ms )   DWT_Delay_us(ms*1000)

/*
	* @函数作用	向SSD1106写入一个字节
	* @参数
	*				@dat: 待写入的数据
	*				@cmd: 数据/命令标志 0-order,1-date
	*	@返回值 无
*/
void OLED_WR_Byte(u8 dat,u8 cmd)
{
    u8 i;
    if(cmd)
        OLED_DC_Set();
    else
        OLED_DC_Clr();

    for(i=0; i<8; i++)
    {
        OLED_SCLK_Clr();
        if(dat&0x80)
            OLED_SDIN_Set();
        else
            OLED_SDIN_Clr();
        OLED_SCLK_Set();
        dat<<=1;
    }
    OLED_DC_Set();
}

/*
	* @函数作用		在OLed上找到指定的位置
	* @参数
	*				@x: 	x坐标的位置
	*				@y:   y坐标的位置
	*	@返回值 无
*/

void OLED_Set_Pos(unsigned char x, unsigned char y)
{
    OLED_WR_Byte(0xb0+y,OLED_CMD);
    OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
    OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD);
}

/*
	* @函数作用	打开OLED的显示功能
	* @参数 无
	*	@返回值 无
*/
void OLED_Display_On(void)
{
    OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
    OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
    OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}

/*
	* @函数作用	 关闭OLED的显示功能
	* @参数 无
	*	@返回值 无
*/
void OLED_Display_Off(void)
{
    OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
    OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
    OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}

/*
	* @函数作用 清除显示屏中所有内容
	* @参数 无
	*	@返回值 无
*/
void OLED_Clear(void)
{
    u8 i,n;
    //更新显示
    for(i=0; i<8; i++)
    {
        OLED_WR_Byte (0xb0+i,OLED_CMD);  		//设置页地址（0~7）
        OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
        OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址
        for(n=0; n<128; n++)OLED_WR_Byte(0,OLED_DATA);
    }
}

/*
	* @函数作用	在指定位置显示一个字符或者部分字符
	* @参数
	*				@x: 0-127
	*				@y: 0-7 每格数据代表8行
*					@chr:
	*	@返回值 无
*/
void OLED_ShowChar(u8 x,u8 y,u8 chr)
{
    unsigned char c=0,i=0;
    c=chr-' ';//得到偏移后的值
    if(x>Max_Column-1) {
        x=0;
        y=y+2;
    }
    if(SIZE ==16) {//size:选择字体 16/12
        OLED_Set_Pos(x,y);
        for(i=0; i<8; i++)
            OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
        OLED_Set_Pos(x,y+1);
        for(i=0; i<8; i++)
            OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
    }
}

/*
	* @函数作用	显示一个字符串
	* @参数
	*				@x: 0-127
	*				@y: 0-7
*					@chr:
	*	@返回值 无
*/
void OLED_ShowString(u8 x,u8 y,u8 *chr)
{
    unsigned char j=0;
    while (chr[j]!='\0')
    {   OLED_ShowChar(x,y,chr[j]);
        x+=8;
        if(x>120) {
            x=0;
            y+=2;
        }
        j++;
    }
}

/*
	* @函数作用 显示汉字
	* @参数
	*     @x:
	*     @y:
	*     @no:
	*	@返回值 无
*/
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{
    u8 t,adder=0;
    OLED_Set_Pos(x,y);
    for(t=0; t<16; t++) {
        OLED_WR_Byte(ChineseFont[2*no][t],OLED_DATA);
        adder+=1;
    }
    OLED_Set_Pos(x,y+1);
    for(t=0; t<16; t++) {
        OLED_WR_Byte(ChineseFont[2*no+1][t],OLED_DATA);
        adder+=1;
    }
}

/*
	* @函数作用 显示32X32点阵汉字
	* @参数
	*     @x:
	*     @y:
	*     @no:
	*	@返回值 无
*/
void OLED_Show_CHinese32X32(u8 x,u8 y,u8 no)
{
    u8 t;
    OLED_Set_Pos(x,y);
    for(t=0; t<32; t++)
        OLED_WR_Byte(ChineseFont_32x32[4*no][t],OLED_DATA);
    OLED_Set_Pos(x,y+1);
    for(t=0; t<32; t++)
        OLED_WR_Byte(ChineseFont_32x32[4*no+1][t],OLED_DATA);
    OLED_Set_Pos(x,y+2);
    for(t=0; t<32; t++)
        OLED_WR_Byte(ChineseFont_32x32[4*no+2][t],OLED_DATA);
    OLED_Set_Pos(x,y+3);
    for(t=0; t<32; t++)
        OLED_WR_Byte(ChineseFont_32x32[4*no+3][t],OLED_DATA);
}

/*
	* @函数作用 初始化SSD1306
	* @参数 无
	*	@返回值 无
*/
void OLED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //使能PD端口时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	 //PD4~PD7推挽输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
    GPIO_Init(GPIOD, &GPIO_InitStructure);	  //初始化GPIOD3,6
    GPIO_SetBits(GPIOD,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_3|GPIO_Pin_8);	//PD3,PD6 输出高

    OLED_RST_Set();
    dealy_ms(100);
    OLED_RST_Clr();
    dealy_ms(100);
    OLED_RST_Set();

    OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
    OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
    OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
    OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
    OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
    OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
    OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
    OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WR_Byte(0x00,OLED_CMD);//-not offset
    OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
    OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
    OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
    OLED_WR_Byte(0x12,OLED_CMD);
    OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
    OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
    OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_WR_Byte(0x02,OLED_CMD);//
    OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
    OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
    OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
    OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7)
    OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel

    OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/
    OLED_Clear();
    OLED_Set_Pos(0,0);
}






