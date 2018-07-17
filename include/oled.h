/*******************************************************************************
OLED显示屏
 分辨率:128x64
 尺寸:0.96寸,27mm*26mm
 通信接口:i2c
 电压:3.3V
*******************************************************************************/
#ifndef OLED_H_
#define OLED_H_

/********************************头文件****************************************/
// Display commands

#define SETCONTRAST 0x81            /* 对比度设置*/
#define DISPLAYON  0xAF             /* 开启显示 */
#define DISPLAYOFF 0xAE             /* 关闭显示 */
#define CHARGEPUMP 0x8D             /* 电荷泵设置 */
#define CHARGEPUMPON  0x14          /* 电荷泵设置 */
#define CHARGEPUMPOFF 0x10          /* 电荷泵设置 */
#define COLUMNADDR 0x21
#define COMSCANINC 0xC0             /* 设置COM扫描方向;bit3:0,普通模式; \
                                    1,重定义模式 COM[N-1]->COM0;N:驱动路数 */
#define COMSCANDEC 0xC8
#define DISPLAYALLON_RESUME 0xA4    /* 全局显示开启;bit0:1,开启;0,关闭;黑屏 */
#define DISPLAYALLON 0xA5           /* 全局显示开启;bit0:1,开启;0,关闭;白屏 */
#define NORMALDISPLAY 0xA6          /* 设置显示方式;bit0:1,反相显示;0,正常显示 */
#define EXTERNALVCC 0x1
#define INVERTDISPLAY 0xA7
#define MEMORYMODE 0x20             /* 设置内存地址模式 */
#define PAGEADDR 0x22
#define SEGREMAP 0xA1
#define SETCOMPINS 0xDA             /* 设置COM硬件引脚配置 */
#define SETDISPLAYCLOCKDIV 0xD5     /* 设置时钟分频因子,震荡频率 */
#define SETDISPLAYOFFSET 0xD3       /* 设置显示偏移 */
#define SETPAGEADDRESS 0xB0         /* 设置页地址 0~7*/
#define SETHIGHCOLUMN 0x10          /* 设置显示位置―列高4位地址 */
#define SETLOWCOLUMN  0x00          /* 设置显示位置―列低4位地址 */
#define SETMULTIPLEX  0xA8          /* 设置驱动路数 */
#define SETPRECHARGE  0xD9          /* 设置预充电周期 */
#define SETSEGMENTREMAP 0xA1
#define SETSTARTLINE 0x40
#define SETVCOMDETECT 0xDB          /* 设置VCOMH 电压倍率 */
#define SWITCHCAPVCC 0x2

/*********************************函数*****************************************/

/*
 * OLED显示屏初始化函数
 */
void oled_init(void);

/*
 * OLED清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
 */
void oled_clear(void);

/*
 * OLED显示屏在指定位置显示一个字符,包括部分字符
 * 参数1:x(in),起始列,范围:0~15
 * 参数2:y(in),起始行,范围:0~3
 * 参数3:chr(in):显示的字符
 */
void oled_show_char(unsigned char x, unsigned char y, unsigned char chr);

/*
 * OLED显示屏显示一个字符号串
 * 参数1:x(in),起始列,范围:0~15
 * 参数2:y(in),起始行,范围:0~3
 * 参数3:chr(in):显示的字符串
 */
void oled_show_string(unsigned char x, unsigned char y, char *chr);

/*
 * OLED显示屏显示一个汉字
 * 参数1:x(in),起始列,范围:0~7
 * 参数2:y(in),起始行,范围:0~3
 * 参数3:n(in):显示的汉字索引值
 */
void oled_show_chinese(unsigned char x, unsigned char y, unsigned char n);

/*
 * OLED显示屏显示整数
 * 参数1:x(in),起始列,范围:0~15
 * 参数2:y(in),起始行,范围:0~3
 * 参数3:num(in):显示的整数
 */
void oled_show_int_num(unsigned char x,unsigned char y, unsigned int num);

/*
 * OLED显示屏显示浮点数
 * 参数1:x(in),起始列,范围:0~15
 * 参数2:y(in),起始行,范围:0~3
 * 参数3:num(in):显示的小数
 */
void oled_show_float_num(unsigned char x, unsigned char y, float num);

#endif /* OLED_H_ */
