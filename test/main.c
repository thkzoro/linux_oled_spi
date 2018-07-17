#include <stdio.h>
#include <string.h>
#include "oled.h"

int main(int argc, char *argv[])
{
    int result;
    int i;

	oled_init();
	oled_clear();
    result = 1;

    if (argv[1] != NULL)
        result = strcmp ("chinese", argv[1]);

    if (result == 0) {
        for (i = 0; i < 6; i++)
    	oled_show_chinese(i, 0, i);
    } else
        oled_show_string(0 ,0, "hello world");

	return 0;
}

