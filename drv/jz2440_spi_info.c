/*
 *oled_spi_dev.c
 *
 *GND：电源地
 *VC：2.V~5.V
 *SCL（D0）：CLK 时钟 （高电平 2.V~5.V）
 *SDA(D1)：MOSI 数据（高电平 2.V~5.V）
 *RST：复位（高电平 2.V~5.V）
 *D/C：数据/命令（高电平 2.V~5.V）
 */
/********************************头文件****************************************/
#include <linux/module.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/jiffies.h>
#include <linux/delay.h>
#include <linux/io.h>

#include <linux/gpio.h>
#include <mach/regs-gpio.h>
#include <plat/gpio-cfg.h>

/********************************宏定义****************************************/

/*******************************全局变量***************************************/
static struct spi_board_info oled_spi_info[] = {
	{
    	 .modalias = "oled_spi",  /* 对应的spi_driver名字也是"oled" */
    	 .max_speed_hz = 10000000,	/* max spi clock (SCK) speed in HZ */
    	 .bus_num = 1,     /* jz2440里OLED接在SPI CONTROLLER 1 */
    	 .mode    = SPI_MODE_0,
    	 .chip_select   = S3C2410_GPF(1), /* oled_cs, 它的含义由spi_master确定 */
    	 .platform_data = (const void *)S3C2410_GPG(4), /* oled_dc, 它在spi_driver里使用 */    	 
	 }
};

/*********************************函数*****************************************/


static int oled_spi_init(void)
{
    return spi_register_board_info(oled_spi_info, ARRAY_SIZE(oled_spi_info));
}

module_init(oled_spi_init);
MODULE_DESCRIPTION("OLED SPI Device");
MODULE_LICENSE("GPL");


