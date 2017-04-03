FREQ=8000000
MCU=attiny861
CC=avr-gcc
LIBS=
CFLAGS=-O3 -Wall -Werror -Wno-unused -Wfatal-errors -DF_CPU=$(FREQ)L -mmcu=$(MCU)
LDFLAGS=-mmcu=$(MCU)
SRC=$(wildcard *.c)
TARGET=lcdtest

.PHONY: program
program: $(TARGET).hex
	@avr-size $(TARGET).elf --format=avr --mcu=$(MCU)
	@echo " \033[31m[FLASH]\033[0m"
	@avrdude -p $(MCU) -U flash:w:$(TARGET).hex

$(TARGET).hex: $(TARGET).elf
	avr-objcopy -O ihex -j .text -j .data $(TARGET).elf $(TARGET).hex 

$(TARGET).elf: $(SRC:.c=.o)
	@echo " \033[34m[LD]\033[0m\t$@"
	@$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	@echo " \033[33m[CC]\033[0m\t$@"
	@$(CC) $(CFLAGS) -c $< -o $@

%.d: %.c
	@echo " \033[35m[DP]\033[0m\t$@"
	@set -o errexit; rm -f $@; \
	$(CC) -M $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

-include $(SRC:.c=.d)

.PHONY: clean
clean:
	@echo " \033[31m[RM]\033[0m"
	@rm -f $(TARGET).elf $(TARGET).hex $(SRC:.c=.o) $(SRC:.c=.d) 