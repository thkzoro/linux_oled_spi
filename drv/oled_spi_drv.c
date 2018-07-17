/*
 *SPI模式OLED显示屏
 *
 *GND：电源地
 *VC：2.V~5.V
 *SCL（D0）：CLK 时钟 （高电平 2.V~5.V）
 *SDA(D1)：MOSI 数据（高电平 2.V~5.V）
 *RST：复位（高电平 2.V~5.V）
 *D/C：数据/命令（高电平 2.V~5.V）
 *CS:spi片选
 */
/********************************头文件****************************************/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/err.h>
#include <asm/uaccess.h>
#include <linux/gpio.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/io.h>
#include <mach/regs-gpio.h>
/*******************************全局变量***************************************/
static int major;
static struct class *class;
static struct spi_device *spi_oled_dev;
static unsigned char *ker_buf;
static int spi_oled_dc_pin;

/*********************************函数*****************************************/
/*
 * OLED  DC管脚电平设置
 * 参数1:val(in):0:低电平，表示写命令;1:高电平，表示写数据
 */
static void oled_dc_lev(char val)
{
    s3c2410_gpio_setpin(spi_oled_dc_pin, val);
}

/*
 * OLED  reset管脚电平设置
 * 参数1:val(in):0:低电平;1:高电平
 */
static void oled_rst_lev(char val)
{
    s3c2410_gpio_setpin(S3C2410_GPG(2), val);
}

static long oled_ioctl(struct file *file, unsigned int cmd, unsigned long a)
{
    switch (cmd) {
    case 1:
        oled_dc_lev((char) a);
        break;
    case 2:
        oled_rst_lev((char) a);
        break;
    default:
        break;
    }

    return 0;
}

static ssize_t oled_write(struct file *file,
				      const char __user *buf,
				      size_t count, loff_t *ppos)
{
    int ret;

    if (count > 4096)
        return -EINVAL;

    ret = copy_from_user(ker_buf, buf, 1);
    spi_write(spi_oled_dev, ker_buf, 1);

    return 0;
}

static struct file_operations oled_fops = {
	.owner = THIS_MODULE,
    .unlocked_ioctl = oled_ioctl,
	.write = oled_write,
};

static int __devinit oled_spi_probe(struct spi_device *spi)
{
    spi_oled_dev = spi;

	ker_buf = devm_kzalloc(&spi->dev, 4096, GFP_KERNEL);
	if (ker_buf == NULL)
		return -ENOMEM;
    spi_oled_dc_pin = (int)spi->dev.platform_data;
    s3c2410_gpio_cfgpin(spi_oled_dc_pin, S3C2410_GPIO_OUTPUT);
    s3c2410_gpio_cfgpin(spi->chip_select, S3C2410_GPIO_OUTPUT);

    /* 注册一个 file_operations */
    major = register_chrdev(0, "oled", &oled_fops);
	class = class_create(THIS_MODULE, "oled");

	/* 为了让mdev根据这些信息来创建设备节点 */
	device_create(class, NULL, MKDEV(major, 0), NULL, "oled"); /* /dev/oled */

    return 0;
}



static int __devexit oled_spi_remove(struct spi_device *spi)
{
	device_destroy(class, MKDEV(major, 0));
	class_destroy(class);
	unregister_chrdev(major, "oled");

	return 0;
}


static struct spi_driver oled_spi_driver = {
	.driver = {
		.name	= "oled",
		.owner	= THIS_MODULE,
	},
	.probe		= oled_spi_probe,
	.remove		= __devexit_p(oled_spi_remove),
};

static int oled_drv_init(void)
{
	int ret;

	ret = spi_register_driver(&oled_spi_driver);
	if (ret)
		pr_err("Failed to register OLED SPI driver: %d\n", ret);

	return 0;
}

static void oled_drv_exit(void)
{
	spi_unregister_driver(&oled_spi_driver);
}


module_init(oled_drv_init);
module_exit(oled_drv_exit);
MODULE_LICENSE("GPL");


