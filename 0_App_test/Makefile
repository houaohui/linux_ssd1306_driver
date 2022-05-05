CROSS_COMPILE 	?= arm-linux-gnueabihf-
TARGET		  	?= 0_App_test

CC 				:= $(CROSS_COMPILE)gcc

INCDIRS 		:= beep \
				   Watch \
				   HARDWARE \
				   HARDWARE/BEEP \
				   HARDWARE/LCD \
				   HARDWARE/LED \
				   HARDWARE/MPU \
				   USER \
				   WIFI 

#				   bsp/clk \
				   bsp/led \
				   bsp/delay 
				   			   
SRCDIRS			:= beep \
				   Watch \
				   HARDWARE \
				   HARDWARE/BEEP \
				   HARDWARE/LCD \
				   HARDWARE/LED \
				   HARDWARE/MPU \
				   USER \
				   WIFI 
#				   bsp/clk \
				   bsp/led \
				   bsp/delay 
				   
				   
INCLUDE			:= $(patsubst %, -I %, $(INCDIRS))  #INCLUDE =  -I dir  -I dir

CFILES			:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))  #CFILES = dir/xx.c dir/xx.c

CFILENDIR		:= $(notdir  $(CFILES)) #CFILENDIR = xx.c xx.c

COBJS			:= $(patsubst %, obj/%, $(CFILENDIR:.c=.o))  #COBJS = obj/xx.o obj/xx.o
OBJS			:= $(COBJS)

VPATH			:= $(SRCDIRS)

.PHONY: clean
	
$(TARGET) : $(OBJS)
	$(CC) -o $(TARGET) -lm -L/usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/lib/ $^ 


$(COBJS) : obj/%.o : %.c
	$(CC) -Wall -c -lm -L/usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/lib/ -std=c99 $(INCLUDE) -o $@ $<
	
clean:
	rm -rf $(TARGET).elf $(TARGET).dis $(TARGET).bin $(COBJS) $(SOBJS)

	