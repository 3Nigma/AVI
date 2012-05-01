################################################################################
# AVI CAR Makefile
################################################################################

SHELL := /bin/sh
RM := rm -rf

LIBS := 
PROJ := caravi
MCU := atmega16

USER_OBJS := ../../obj/$(PROJ)
USER_LIBS := ../../lib
VPATH := ../../lib
USER_DEPS := ../../obj
PROJ_BIN := ../../bin/$(PROJ)
OBJ_CFLAGS := -funsigned-char -funsigned-bitfields -O2 -fpack-struct -fshort-enums -g2 -Wall -c -std=gnu99

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
LIB_AS_ARGS :=
OUTPUT_FILE_PATH := 
OUTPUT_FILE_PATH_AS_ARGS :=
HEX_FLASH_FILE_PATH :=
HEX_FLASH_FILE_PATH_AS_ARGS :=
HEX_EEPROM_FILE_PATH :=
HEX_EEPROM_FILE_PATH_AS_ARGS :=
LSS_FILE_PATH :=
LSS_FILE_PATH_AS_ARGS :=
MAP_FILE_PATH :=
MAP_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=/usr/bin/
ADDITIONAL_DEPENDENCIES:=

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
main.c \
cams.c \
motors.c \
rf12.c \
$(USER_LIBS)/uart.c

PREPROCESSING_SRCS += 

ASM_SRCS += 

OBJS +=  \
$(USER_OBJS)/main.o \
$(USER_OBJS)/cams.o \
$(USER_OBJS)/motors.o \
$(USER_OBJS)/rf12.o \
$(USER_OBJS)/uart.o

OBJS_AS_ARGS +=  \
$(USER_OBJS)/main.o \
$(USER_OBJS)/cams.o \
$(USER_OBJS)/motors.o \
$(USER_OBJS)/rf12.o \
$(USER_OBJS)/uart.o

C_DEPS +=  \
main.d \
cams.d \
motors.d \
rf12.d \
uart.d

C_DEPS_AS_ARGS +=  \
main.d \
cams.d \
motors.d \
rf12.d \
uart.d

OUTPUT_FILE_PATH += $(PROJ_BIN)/$(PROJ).elf
OUTPUT_FILE_PATH_AS_ARGS += $(PROJ_BIN)/$(PROJ).elf
HEX_FLASH_FILE_PATH += $(PROJ_BIN)/$(PROJ).hex
HEX_FLASH_FILE_PATH_AS_ARGS += $(PROJ_BIN)/$(PROJ).hex
HEX_EEPROM_FILE_PATH += $(PROJ_BIN)/$(PROJ).eep
HEX_EEPROM_FILE_PATH_AS_ARGS += $(PROJ_BIN)/$(PROJ).eep
LSS_FILE_PATH += $(PROJ_BIN)/$(PROJ).lss
LSS_FILE_PATH_AS_ARGS += $(PROJ_BIN)/$(PROJ).lss
MAP_FILE_PATH = $(PROJ_BIN)/$(PROJ).map
MAP_FILE_PATH_AS_ARGS = $(PROJ_BIN)/$(PROJ).map
LIB_AS_ARGS += $(PROJ_BIN)/lib$(PROJ).a
ADDITIONAL_DEPENDENCIES:= $(HEX_FLASH_FILE_PATH) $(LSS_FILE_PATH) $(HEX_EEPROM_FILE_PATH) size

# AVR/GNU C Compiler

./$(USER_OBJS)/%.o: ./%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler
	$(AVR_APP_PATH)avr-gcc -I$(USER_LIBS) -I. $(OBJ_CFLAGS) -mmcu=$(MCU) -MD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<

# AVR/GNU Assembler

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

burn_hex: $(OUTPUT_FILE_PATH)
	avrdude -p m16 -c usbtiny -U flash:w:$(PROJ_BIN)/$(PROJ).hex

# AVR/GNU C/C++ Linker
$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS)
	@echo Building target: $@
	@echo Invoking: AVR/GNU C/C++ Linker
	$(AVR_APP_PATH)avr-gcc  -mmcu=$(MCU)  -Wl,-Map=$(MAP_FILE_PATH_AS_ARGS) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(LIBS)
	@echo Finished building target: $@

$(HEX_FLASH_FILE_PATH): $(OUTPUT_FILE_PATH)
	$(AVR_APP_PATH)avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature  $< $@

$(HEX_EEPROM_FILE_PATH): $(OUTPUT_FILE_PATH)
	-$(AVR_APP_PATH)avr-objcopy -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O ihex $< $@ || exit 0

$(LSS_FILE_PATH): $(OUTPUT_FILE_PATH)
	$(AVR_APP_PATH)avr-objdump -h -S $< > $@

size: $(OUTPUT_FILE_PATH)
	@$(AVR_APP_PATH)avr-size -C --mcu=$(MCU) $(OUTPUT_FILE_PATH_AS_ARGS)

# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS)$(C_DEPS_AS_ARGS) $(EXECUTABLES) $(LIB_AS_ARGS) $(HEX_FLASH_FILE_PATH_AS_ARGS) $(HEX_EEPROM_FILE_PATH_AS_ARGS) $(LSS_FILE_PATH_AS_ARGS) $(MAP_FILE_PATH_AS_ARGS) $(OUTPUT_FILE_PATH)

