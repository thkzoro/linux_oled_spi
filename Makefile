# Makefile编译说明 
#
target := all


nodevcfg := 0

default:
ifeq ($(target), ko)
	make -C drv
else ifeq ($(target), lib)
	make -C lib
else ifeq ($(target), test)
	make -C test
else
	make -C drv
	make -C lib
	make -C test
endif

clean:  
ifeq ($(target), ko)
	make -C drv clean
else ifeq ($(target), lib)
	make -C lib clean
else ifeq ($(target), test)
	make -C test clean
else
	make -C drv clean
	make -C lib clean
	make -C test clean
endif

.PHONY: 
	clean default
