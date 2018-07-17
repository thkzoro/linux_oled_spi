/*******************************************************************************
OLED显示屏
 分辨率:128x64
 尺寸:0.96寸,27mm*26mm
 电压:3.3V
*******************************************************************************/
/********************************头文件****************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "oled.h"
#include "oledfont.h"
/********************************宏定义****************************************/
/*******************************全局变量***************************************/
static int oled_fd;
/*********************************函数*****************************************/
/*
 *往OLED发送一个字节
 *参数1:byte(in)    发送的字节
 *参数1:iscmd(in) 是否为命令 1:命令; 0:数据
 */
static void oled_write_byte(unsigned char byte, int iscmd)
{
    if (iscmd)
        ioctl(oled_fd, 1, 0);   /* 写命令 */
    else
        ioctl(oled_fd, 1, 1);   /* 写数据 */
    write(oled_fd, &byte, 1);
}

/*
 *往OLED写入一个命令
 *参数1:cmd(in)
 */
static void oled_cmd_write_byte(unsigned char cmd_data)
{
    oled_write_byte(cmd_data, 1);
}

/*
 *往OLED写入一字节数据
 *参数1:data(in)
 */
static void oled_data_write_byte(unsigned char data)
{
    oled_write_byte(data, 0);
}

/*
 * OLED显示屏设置坐标
 * 参数1:col(in):0~128
 * 参数2:page(in):0~7
 */
static void oled_set_pos(unsigned char col, unsigned char page)
{
    oled_cmd_write_byte(SETPAGEADDRESS + page); /*设置页地址(0~7)*/
    oled_cmd_write_byte(SETHIGHCOLUMN | ((col & 0xf0) >> 4));/*设置显示位置―列高地址*/
    oled_cmd_write_byte(SETLOWCOLUMN | (col & 0x0f));/*设置显示位置―列低地址*/
}

/*
 * OLED显示屏初始化函数
 */
void oled_init(void)
{
    oled_fd = open("/dev/oled", O_RDWR);
    if (oled_fd < 0) {
        printf("open /dev/oled file failed!\n");
        return;
    }

    ioctl(oled_fd, 2, 0);
    usleep(100000);
    ioctl(oled_fd, 2, 1);

    oled_cmd_write_byte(DISPLAYOFF);    /* 关闭显示 */
    oled_cmd_write_byte(MEMORYMODE);    /* 设置内存地址模式 */
    oled_cmd_write_byte(0x10);
    oled_cmd_write_byte(SETPAGEADDRESS);    /* 设置页地址 0~7*/
    oled_cmd_write_byte(COMSCANDEC);    /* 设置COM扫描方向;bit3:0,普通模式; \
                                        1,重定义模式 COM[N-1]->COM0;N:驱动路数 */
    oled_cmd_write_byte(SETLOWCOLUMN);  /* 设置显示位置―列低4位地址 */
    oled_cmd_write_byte(SETHIGHCOLUMN); /* 设置显示位置―列高4位地址 */
    oled_cmd_write_byte(SETSTARTLINE);  /*--set start line address*/
    oled_cmd_write_byte(SETCONTRAST);   /* 对比度设置*/
    oled_cmd_write_byte(0xff);          /*亮度调节 0x00~0xff*/
    oled_cmd_write_byte(SEGREMAP);      /*set segment re-map 0 to 127*/
    oled_cmd_write_byte(NORMALDISPLAY); /* 设置显示方式;bit0:1,反相显示;0,正常显示 */
    oled_cmd_write_byte(SETMULTIPLEX);  /* 设置驱动路数 */
    oled_cmd_write_byte(0x3F);
    oled_cmd_write_byte(DISPLAYALLON_RESUME); /* 全局显示开启;bit0:1,开启;0,关闭;黑屏 */
    oled_cmd_write_byte(SETDISPLAYOFFSET);    /* 设置显示偏移 */
    oled_cmd_write_byte(0x00); //-not offset
    oled_cmd_write_byte(SETDISPLAYCLOCKDIV); /* 设置时钟分频因子,震荡频率 */
    oled_cmd_write_byte(0xf0);               /* [3:0],分频因子;[7:4],震荡频率;*/
    oled_cmd_write_byte(SETPRECHARGE);       /* 设置预充电周期 */
    oled_cmd_write_byte(0x22);               /* [3:0],Phase 1;[7:4],Phase 2; */
    oled_cmd_write_byte(SETCOMPINS);         /* 设置COM硬件引脚配置 */
    oled_cmd_write_byte(0x12);
    oled_cmd_write_byte(SETVCOMDETECT);      /* 设置VCOMH 电压倍率 */
    oled_cmd_write_byte(0x20);               //0x20,0.77xVcc
    oled_cmd_write_byte(0x8d); //--set DC-DC enable
    oled_cmd_write_byte(0x14); //
    oled_cmd_write_byte(DISPLAYON);         /* 开启显示 */
}

/*
 * OLED清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
 */
void oled_clear(void)
{
    int page;
    int col;

    for(page = 0; page < 8; page++) {
        oled_set_pos(0, page);
        for(col = 0; col < 128; col++)
            oled_data_write_byte(0x00);
    }
}


/*
 * OLED显示屏在指定位置显示一个字符,包括部分字符
 * 参数1:x(in),起始列,范围:0~15
 * 参数2:y(in),起始行,范围:0~3
 * 参数3:chr(in):显示的字符
 */
void oled_show_char(unsigned char x, unsigned char y, unsigned char chr)
{
    unsigned char c = 0;
    unsigned char i = 0;

    c = chr - ' ';//得到偏移后的值
    if(x > 15) {
        x = 0;
        y = y + 2;
    }

    oled_set_pos(x*8, y*2);
    for(i = 0; i < 8; i++)
        oled_data_write_byte(F8X16[c*16+i]);
    oled_set_pos(x*8, y*2+1);
    for(i = 0; i < 8; i++)
        oled_data_write_byte(F8X16[c*16+i+8]);
}

/*
 * OLED显示屏显示一个字符号串
 * 参数1:x(in),起始列,范围:0~15
 * 参数2:y(in),起始行,范围:0~3
 * 参数3:chr(in):显示的字符串
 */
void oled_show_string(unsigned char x, unsigned char y, char *chr)
{
    unsigned int i = 0;

    while (chr[i] != '\0') {
        oled_show_char(x+i, y, chr[i]);
        i++;
    }
}

/*
 * OLED显示屏显示一个汉字
 * 参数1:x(in),起始列,范围:0~7
 * 参数2:y(in),起始行,范围:0~3
 * 参数3:n(in):显示的汉字索引值
 */
void oled_show_chinese(unsigned char x, unsigned char y, unsigned char n)
{
    unsigned char  i;

    oled_set_pos(x*16, y*2);
    for(i = 0; i < 16; i++)
        oled_data_write_byte(Hzk[2*n][i]);
    oled_set_pos(x*16, y*2+1);
    for(i = 0; i < 16; i++)
        oled_data_write_byte(Hzk[2*n+1][i]);
}

/*
 * OLED显示屏显示整数
 * 参数1:x(in),起始列,范围:0~15
 * 参数2:y(in),起始行,范围:0~3
 * 参数3:num(in):显示的整数
 */
void oled_show_int_num(unsigned char x,unsigned char y, unsigned int num)
{
    char buf[10];

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%d", num);
    oled_show_string(x, y, buf);
}

/*
 * OLED显示屏显示浮点数
 * 参数1:x(in),起始列,范围:0~15
 * 参数2:y(in),起始行,范围:0~3
 * 参数3:num(in):显示的小数
 */
void oled_show_float_num(unsigned char x, unsigned char y, float num)
{
    char buf[10];

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%.1f", num);
    oled_show_string(x, y, buf);
}

