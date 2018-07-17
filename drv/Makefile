KERN_DIR = /home/book/opensrc/kernel/linux-3.4.2

#obj-m += oled_spi_dev.o
obj-m += oled_spi_drv.o
		  
default:
	make -C $(KERN_DIR) M=`pwd` modules 
	
clean:  
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order

.PHONY: 
	clean default

