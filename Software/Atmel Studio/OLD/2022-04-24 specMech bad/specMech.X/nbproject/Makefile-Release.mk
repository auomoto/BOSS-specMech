#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-Release.mk)" "nbproject/Makefile-local-Release.mk"
include nbproject/Makefile-local-Release.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=Release
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/specMech.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/specMech.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../ad590.c ../ads1115.c ../commands.c ../ds3231.c ../eeprom.c ../errors.c ../fram.c ../humidity.c ../initialize.c ../ionpump.c ../main.c ../mcp23008.c ../mcp9808.c ../mma8451.c ../nmea.c ../oled.c ../pneu.c ../ports.c ../pushbutton.c ../report.c ../roboclaw.c ../set.c ../temperature.c ../testroutine.c ../timers.c ../twi.c ../usart.c ../wdt.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/ad590.o ${OBJECTDIR}/_ext/1472/ads1115.o ${OBJECTDIR}/_ext/1472/commands.o ${OBJECTDIR}/_ext/1472/ds3231.o ${OBJECTDIR}/_ext/1472/eeprom.o ${OBJECTDIR}/_ext/1472/errors.o ${OBJECTDIR}/_ext/1472/fram.o ${OBJECTDIR}/_ext/1472/humidity.o ${OBJECTDIR}/_ext/1472/initialize.o ${OBJECTDIR}/_ext/1472/ionpump.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/mcp23008.o ${OBJECTDIR}/_ext/1472/mcp9808.o ${OBJECTDIR}/_ext/1472/mma8451.o ${OBJECTDIR}/_ext/1472/nmea.o ${OBJECTDIR}/_ext/1472/oled.o ${OBJECTDIR}/_ext/1472/pneu.o ${OBJECTDIR}/_ext/1472/ports.o ${OBJECTDIR}/_ext/1472/pushbutton.o ${OBJECTDIR}/_ext/1472/report.o ${OBJECTDIR}/_ext/1472/roboclaw.o ${OBJECTDIR}/_ext/1472/set.o ${OBJECTDIR}/_ext/1472/temperature.o ${OBJECTDIR}/_ext/1472/testroutine.o ${OBJECTDIR}/_ext/1472/timers.o ${OBJECTDIR}/_ext/1472/twi.o ${OBJECTDIR}/_ext/1472/usart.o ${OBJECTDIR}/_ext/1472/wdt.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/ad590.o.d ${OBJECTDIR}/_ext/1472/ads1115.o.d ${OBJECTDIR}/_ext/1472/commands.o.d ${OBJECTDIR}/_ext/1472/ds3231.o.d ${OBJECTDIR}/_ext/1472/eeprom.o.d ${OBJECTDIR}/_ext/1472/errors.o.d ${OBJECTDIR}/_ext/1472/fram.o.d ${OBJECTDIR}/_ext/1472/humidity.o.d ${OBJECTDIR}/_ext/1472/initialize.o.d ${OBJECTDIR}/_ext/1472/ionpump.o.d ${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/1472/mcp23008.o.d ${OBJECTDIR}/_ext/1472/mcp9808.o.d ${OBJECTDIR}/_ext/1472/mma8451.o.d ${OBJECTDIR}/_ext/1472/nmea.o.d ${OBJECTDIR}/_ext/1472/oled.o.d ${OBJECTDIR}/_ext/1472/pneu.o.d ${OBJECTDIR}/_ext/1472/ports.o.d ${OBJECTDIR}/_ext/1472/pushbutton.o.d ${OBJECTDIR}/_ext/1472/report.o.d ${OBJECTDIR}/_ext/1472/roboclaw.o.d ${OBJECTDIR}/_ext/1472/set.o.d ${OBJECTDIR}/_ext/1472/temperature.o.d ${OBJECTDIR}/_ext/1472/testroutine.o.d ${OBJECTDIR}/_ext/1472/timers.o.d ${OBJECTDIR}/_ext/1472/twi.o.d ${OBJECTDIR}/_ext/1472/usart.o.d ${OBJECTDIR}/_ext/1472/wdt.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/ad590.o ${OBJECTDIR}/_ext/1472/ads1115.o ${OBJECTDIR}/_ext/1472/commands.o ${OBJECTDIR}/_ext/1472/ds3231.o ${OBJECTDIR}/_ext/1472/eeprom.o ${OBJECTDIR}/_ext/1472/errors.o ${OBJECTDIR}/_ext/1472/fram.o ${OBJECTDIR}/_ext/1472/humidity.o ${OBJECTDIR}/_ext/1472/initialize.o ${OBJECTDIR}/_ext/1472/ionpump.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/mcp23008.o ${OBJECTDIR}/_ext/1472/mcp9808.o ${OBJECTDIR}/_ext/1472/mma8451.o ${OBJECTDIR}/_ext/1472/nmea.o ${OBJECTDIR}/_ext/1472/oled.o ${OBJECTDIR}/_ext/1472/pneu.o ${OBJECTDIR}/_ext/1472/ports.o ${OBJECTDIR}/_ext/1472/pushbutton.o ${OBJECTDIR}/_ext/1472/report.o ${OBJECTDIR}/_ext/1472/roboclaw.o ${OBJECTDIR}/_ext/1472/set.o ${OBJECTDIR}/_ext/1472/temperature.o ${OBJECTDIR}/_ext/1472/testroutine.o ${OBJECTDIR}/_ext/1472/timers.o ${OBJECTDIR}/_ext/1472/twi.o ${OBJECTDIR}/_ext/1472/usart.o ${OBJECTDIR}/_ext/1472/wdt.o

# Source Files
SOURCEFILES=../ad590.c ../ads1115.c ../commands.c ../ds3231.c ../eeprom.c ../errors.c ../fram.c ../humidity.c ../initialize.c ../ionpump.c ../main.c ../mcp23008.c ../mcp9808.c ../mma8451.c ../nmea.c ../oled.c ../pneu.c ../ports.c ../pushbutton.c ../report.c ../roboclaw.c ../set.c ../temperature.c ../testroutine.c ../timers.c ../twi.c ../usart.c ../wdt.c

# Pack Options 
PACK_COMPILER_OPTIONS=-I "${DFP_DIR}/include"
PACK_COMMON_OPTIONS=-B "${DFP_DIR}/gcc/dev/atmega4809"



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-Release.mk dist/${CND_CONF}/${IMAGE_TYPE}/specMech.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=ATmega4809
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/ad590.o: ../ad590.c  .generated_files/flags/Release/3a0c3f08036ba331a162ee39e606e698c4e30caa .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ad590.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ad590.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/ad590.o.d" -MT "${OBJECTDIR}/_ext/1472/ad590.o.d" -MT ${OBJECTDIR}/_ext/1472/ad590.o  -o ${OBJECTDIR}/_ext/1472/ad590.o ../ad590.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/ads1115.o: ../ads1115.c  .generated_files/flags/Release/67c3fef74e0d4564a135df177d8ef915008045f8 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ads1115.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ads1115.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/ads1115.o.d" -MT "${OBJECTDIR}/_ext/1472/ads1115.o.d" -MT ${OBJECTDIR}/_ext/1472/ads1115.o  -o ${OBJECTDIR}/_ext/1472/ads1115.o ../ads1115.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/commands.o: ../commands.c  .generated_files/flags/Release/a2c12e18f81eb6644b64d1e13e1d84f719260fa8 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/commands.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/commands.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/commands.o.d" -MT "${OBJECTDIR}/_ext/1472/commands.o.d" -MT ${OBJECTDIR}/_ext/1472/commands.o  -o ${OBJECTDIR}/_ext/1472/commands.o ../commands.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/ds3231.o: ../ds3231.c  .generated_files/flags/Release/f2e71cb7d37f3dd41641848963f77a9b9b0b89a0 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ds3231.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ds3231.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/ds3231.o.d" -MT "${OBJECTDIR}/_ext/1472/ds3231.o.d" -MT ${OBJECTDIR}/_ext/1472/ds3231.o  -o ${OBJECTDIR}/_ext/1472/ds3231.o ../ds3231.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/eeprom.o: ../eeprom.c  .generated_files/flags/Release/c49eb0ba6df73544430443eca0d12b29f3a44e12 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/eeprom.o.d" -MT "${OBJECTDIR}/_ext/1472/eeprom.o.d" -MT ${OBJECTDIR}/_ext/1472/eeprom.o  -o ${OBJECTDIR}/_ext/1472/eeprom.o ../eeprom.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/errors.o: ../errors.c  .generated_files/flags/Release/47d90b8c7794247b732a43d749d06ff1d9b22279 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/errors.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/errors.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/errors.o.d" -MT "${OBJECTDIR}/_ext/1472/errors.o.d" -MT ${OBJECTDIR}/_ext/1472/errors.o  -o ${OBJECTDIR}/_ext/1472/errors.o ../errors.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/fram.o: ../fram.c  .generated_files/flags/Release/4a0ece078db8a04f4a550b994681fe6c4eabe017 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/fram.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/fram.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/fram.o.d" -MT "${OBJECTDIR}/_ext/1472/fram.o.d" -MT ${OBJECTDIR}/_ext/1472/fram.o  -o ${OBJECTDIR}/_ext/1472/fram.o ../fram.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/humidity.o: ../humidity.c  .generated_files/flags/Release/9d84c56dc606171d43f19f6f8d0c865899e3b2d1 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/humidity.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/humidity.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/humidity.o.d" -MT "${OBJECTDIR}/_ext/1472/humidity.o.d" -MT ${OBJECTDIR}/_ext/1472/humidity.o  -o ${OBJECTDIR}/_ext/1472/humidity.o ../humidity.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/initialize.o: ../initialize.c  .generated_files/flags/Release/d505d1ecaa6d2bd0229acda2c43bc1c54702face .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/initialize.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/initialize.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/initialize.o.d" -MT "${OBJECTDIR}/_ext/1472/initialize.o.d" -MT ${OBJECTDIR}/_ext/1472/initialize.o  -o ${OBJECTDIR}/_ext/1472/initialize.o ../initialize.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/ionpump.o: ../ionpump.c  .generated_files/flags/Release/5d850683de4e260fa391b40e1c80d443ece4e4f1 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ionpump.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ionpump.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/ionpump.o.d" -MT "${OBJECTDIR}/_ext/1472/ionpump.o.d" -MT ${OBJECTDIR}/_ext/1472/ionpump.o  -o ${OBJECTDIR}/_ext/1472/ionpump.o ../ionpump.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/Release/58dfe29bf12d8baf5f003c4486e09b480fa9f111 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/main.o.d" -MT "${OBJECTDIR}/_ext/1472/main.o.d" -MT ${OBJECTDIR}/_ext/1472/main.o  -o ${OBJECTDIR}/_ext/1472/main.o ../main.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/mcp23008.o: ../mcp23008.c  .generated_files/flags/Release/5f24044ddda3a925f6c5980c7851783557f74e0d .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/mcp23008.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/mcp23008.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/mcp23008.o.d" -MT "${OBJECTDIR}/_ext/1472/mcp23008.o.d" -MT ${OBJECTDIR}/_ext/1472/mcp23008.o  -o ${OBJECTDIR}/_ext/1472/mcp23008.o ../mcp23008.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/mcp9808.o: ../mcp9808.c  .generated_files/flags/Release/36bc613d7ce4e5d4a61ecd2afd34a43f44b9b95b .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/mcp9808.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/mcp9808.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/mcp9808.o.d" -MT "${OBJECTDIR}/_ext/1472/mcp9808.o.d" -MT ${OBJECTDIR}/_ext/1472/mcp9808.o  -o ${OBJECTDIR}/_ext/1472/mcp9808.o ../mcp9808.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/mma8451.o: ../mma8451.c  .generated_files/flags/Release/ef42197e7b943300f3ac1186974cb83cf8e88eb7 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/mma8451.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/mma8451.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/mma8451.o.d" -MT "${OBJECTDIR}/_ext/1472/mma8451.o.d" -MT ${OBJECTDIR}/_ext/1472/mma8451.o  -o ${OBJECTDIR}/_ext/1472/mma8451.o ../mma8451.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/nmea.o: ../nmea.c  .generated_files/flags/Release/2523a568f2aae0c3601d1276779b6954d2ca9ccf .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/nmea.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/nmea.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/nmea.o.d" -MT "${OBJECTDIR}/_ext/1472/nmea.o.d" -MT ${OBJECTDIR}/_ext/1472/nmea.o  -o ${OBJECTDIR}/_ext/1472/nmea.o ../nmea.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/oled.o: ../oled.c  .generated_files/flags/Release/4d903512aeb29558097ab71a66871e5a7ecac3d3 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/oled.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/oled.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/oled.o.d" -MT "${OBJECTDIR}/_ext/1472/oled.o.d" -MT ${OBJECTDIR}/_ext/1472/oled.o  -o ${OBJECTDIR}/_ext/1472/oled.o ../oled.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/pneu.o: ../pneu.c  .generated_files/flags/Release/66cef085b1c8b9ac3c4b74264bdd849c927e904b .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/pneu.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pneu.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/pneu.o.d" -MT "${OBJECTDIR}/_ext/1472/pneu.o.d" -MT ${OBJECTDIR}/_ext/1472/pneu.o  -o ${OBJECTDIR}/_ext/1472/pneu.o ../pneu.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/ports.o: ../ports.c  .generated_files/flags/Release/132229bc67a087014e3ff48090d33651bbeaff23 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ports.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ports.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/ports.o.d" -MT "${OBJECTDIR}/_ext/1472/ports.o.d" -MT ${OBJECTDIR}/_ext/1472/ports.o  -o ${OBJECTDIR}/_ext/1472/ports.o ../ports.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/pushbutton.o: ../pushbutton.c  .generated_files/flags/Release/f848eab80080ec4d70e5354361f17499b6e4edba .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/pushbutton.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pushbutton.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/pushbutton.o.d" -MT "${OBJECTDIR}/_ext/1472/pushbutton.o.d" -MT ${OBJECTDIR}/_ext/1472/pushbutton.o  -o ${OBJECTDIR}/_ext/1472/pushbutton.o ../pushbutton.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/report.o: ../report.c  .generated_files/flags/Release/ab38a5afcdf3c955eef7f7f1c7570691128f6f0f .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/report.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/report.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/report.o.d" -MT "${OBJECTDIR}/_ext/1472/report.o.d" -MT ${OBJECTDIR}/_ext/1472/report.o  -o ${OBJECTDIR}/_ext/1472/report.o ../report.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/roboclaw.o: ../roboclaw.c  .generated_files/flags/Release/2ba3d94ecd95ad5079079d0af641ae834bd294f4 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/roboclaw.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/roboclaw.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/roboclaw.o.d" -MT "${OBJECTDIR}/_ext/1472/roboclaw.o.d" -MT ${OBJECTDIR}/_ext/1472/roboclaw.o  -o ${OBJECTDIR}/_ext/1472/roboclaw.o ../roboclaw.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/set.o: ../set.c  .generated_files/flags/Release/f1d9da6fdd2efefc6ca5ec2205f08cfe3811b433 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/set.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/set.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/set.o.d" -MT "${OBJECTDIR}/_ext/1472/set.o.d" -MT ${OBJECTDIR}/_ext/1472/set.o  -o ${OBJECTDIR}/_ext/1472/set.o ../set.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/temperature.o: ../temperature.c  .generated_files/flags/Release/cd38a13d78a9a2e346e0ff036a7c09e2f6b02b77 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/temperature.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/temperature.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/temperature.o.d" -MT "${OBJECTDIR}/_ext/1472/temperature.o.d" -MT ${OBJECTDIR}/_ext/1472/temperature.o  -o ${OBJECTDIR}/_ext/1472/temperature.o ../temperature.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/testroutine.o: ../testroutine.c  .generated_files/flags/Release/f725c3765426e5554ab5423d3ff3aae25e4fc71c .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/testroutine.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/testroutine.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/testroutine.o.d" -MT "${OBJECTDIR}/_ext/1472/testroutine.o.d" -MT ${OBJECTDIR}/_ext/1472/testroutine.o  -o ${OBJECTDIR}/_ext/1472/testroutine.o ../testroutine.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/timers.o: ../timers.c  .generated_files/flags/Release/b39ad565f682143191fc3f6879c28a8b442ff82 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/timers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/timers.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/timers.o.d" -MT "${OBJECTDIR}/_ext/1472/timers.o.d" -MT ${OBJECTDIR}/_ext/1472/timers.o  -o ${OBJECTDIR}/_ext/1472/timers.o ../timers.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/twi.o: ../twi.c  .generated_files/flags/Release/5c354f36d20d3fa8b97df6535329f2e4145d149b .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/twi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/twi.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/twi.o.d" -MT "${OBJECTDIR}/_ext/1472/twi.o.d" -MT ${OBJECTDIR}/_ext/1472/twi.o  -o ${OBJECTDIR}/_ext/1472/twi.o ../twi.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/usart.o: ../usart.c  .generated_files/flags/Release/499c8d43f2e37e089c9b505665b83860f2935155 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/usart.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/usart.o.d" -MT "${OBJECTDIR}/_ext/1472/usart.o.d" -MT ${OBJECTDIR}/_ext/1472/usart.o  -o ${OBJECTDIR}/_ext/1472/usart.o ../usart.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/wdt.o: ../wdt.c  .generated_files/flags/Release/95263e55e7246f64c2540875728f4935151f439 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/wdt.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/wdt.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/wdt.o.d" -MT "${OBJECTDIR}/_ext/1472/wdt.o.d" -MT ${OBJECTDIR}/_ext/1472/wdt.o  -o ${OBJECTDIR}/_ext/1472/wdt.o ../wdt.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
else
${OBJECTDIR}/_ext/1472/ad590.o: ../ad590.c  .generated_files/flags/Release/72a622227c5773af66a123ee2f7b04c23621db9f .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ad590.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ad590.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/ad590.o.d" -MT "${OBJECTDIR}/_ext/1472/ad590.o.d" -MT ${OBJECTDIR}/_ext/1472/ad590.o  -o ${OBJECTDIR}/_ext/1472/ad590.o ../ad590.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/ads1115.o: ../ads1115.c  .generated_files/flags/Release/8660dbf3d5217fcf14afe80303ed0a477c2c2467 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ads1115.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ads1115.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/ads1115.o.d" -MT "${OBJECTDIR}/_ext/1472/ads1115.o.d" -MT ${OBJECTDIR}/_ext/1472/ads1115.o  -o ${OBJECTDIR}/_ext/1472/ads1115.o ../ads1115.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/commands.o: ../commands.c  .generated_files/flags/Release/18dcaa2a821ff8273aae6f80a15e0940306d87c2 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/commands.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/commands.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/commands.o.d" -MT "${OBJECTDIR}/_ext/1472/commands.o.d" -MT ${OBJECTDIR}/_ext/1472/commands.o  -o ${OBJECTDIR}/_ext/1472/commands.o ../commands.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/ds3231.o: ../ds3231.c  .generated_files/flags/Release/6a250db4f2655cc981a8b76d54567ca3d0bfd08f .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ds3231.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ds3231.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/ds3231.o.d" -MT "${OBJECTDIR}/_ext/1472/ds3231.o.d" -MT ${OBJECTDIR}/_ext/1472/ds3231.o  -o ${OBJECTDIR}/_ext/1472/ds3231.o ../ds3231.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/eeprom.o: ../eeprom.c  .generated_files/flags/Release/8042d9d043774b952f61885ac19175442a4c2c0b .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/eeprom.o.d" -MT "${OBJECTDIR}/_ext/1472/eeprom.o.d" -MT ${OBJECTDIR}/_ext/1472/eeprom.o  -o ${OBJECTDIR}/_ext/1472/eeprom.o ../eeprom.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/errors.o: ../errors.c  .generated_files/flags/Release/839c5665d4f1cb568dcd70ce20bcb84eb2e562d .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/errors.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/errors.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/errors.o.d" -MT "${OBJECTDIR}/_ext/1472/errors.o.d" -MT ${OBJECTDIR}/_ext/1472/errors.o  -o ${OBJECTDIR}/_ext/1472/errors.o ../errors.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/fram.o: ../fram.c  .generated_files/flags/Release/de6c06cc87fe4287b6c3a6dc450ca8c88aae8417 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/fram.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/fram.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/fram.o.d" -MT "${OBJECTDIR}/_ext/1472/fram.o.d" -MT ${OBJECTDIR}/_ext/1472/fram.o  -o ${OBJECTDIR}/_ext/1472/fram.o ../fram.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/humidity.o: ../humidity.c  .generated_files/flags/Release/dfe5c4180560250df3ff389019edef5f74437d7b .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/humidity.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/humidity.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/humidity.o.d" -MT "${OBJECTDIR}/_ext/1472/humidity.o.d" -MT ${OBJECTDIR}/_ext/1472/humidity.o  -o ${OBJECTDIR}/_ext/1472/humidity.o ../humidity.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/initialize.o: ../initialize.c  .generated_files/flags/Release/4475fd29660e1679ed648be84ff8e848ebe841fd .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/initialize.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/initialize.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/initialize.o.d" -MT "${OBJECTDIR}/_ext/1472/initialize.o.d" -MT ${OBJECTDIR}/_ext/1472/initialize.o  -o ${OBJECTDIR}/_ext/1472/initialize.o ../initialize.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/ionpump.o: ../ionpump.c  .generated_files/flags/Release/1ac2199653eb46a6bb9407c7d0998e64f23e4a3c .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ionpump.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ionpump.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/ionpump.o.d" -MT "${OBJECTDIR}/_ext/1472/ionpump.o.d" -MT ${OBJECTDIR}/_ext/1472/ionpump.o  -o ${OBJECTDIR}/_ext/1472/ionpump.o ../ionpump.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/Release/1d5d506072b4c17d60e72aa6132a9afbd6144493 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/main.o.d" -MT "${OBJECTDIR}/_ext/1472/main.o.d" -MT ${OBJECTDIR}/_ext/1472/main.o  -o ${OBJECTDIR}/_ext/1472/main.o ../main.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/mcp23008.o: ../mcp23008.c  .generated_files/flags/Release/97bb595127e5d582f016e8a062d063f8c18b1304 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/mcp23008.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/mcp23008.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/mcp23008.o.d" -MT "${OBJECTDIR}/_ext/1472/mcp23008.o.d" -MT ${OBJECTDIR}/_ext/1472/mcp23008.o  -o ${OBJECTDIR}/_ext/1472/mcp23008.o ../mcp23008.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/mcp9808.o: ../mcp9808.c  .generated_files/flags/Release/666cfb4839eb5611ae6f0a7882c6f09a45572380 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/mcp9808.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/mcp9808.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/mcp9808.o.d" -MT "${OBJECTDIR}/_ext/1472/mcp9808.o.d" -MT ${OBJECTDIR}/_ext/1472/mcp9808.o  -o ${OBJECTDIR}/_ext/1472/mcp9808.o ../mcp9808.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/mma8451.o: ../mma8451.c  .generated_files/flags/Release/b690158c76655c1fcc1c26e3af1a89291ed5eef9 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/mma8451.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/mma8451.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/mma8451.o.d" -MT "${OBJECTDIR}/_ext/1472/mma8451.o.d" -MT ${OBJECTDIR}/_ext/1472/mma8451.o  -o ${OBJECTDIR}/_ext/1472/mma8451.o ../mma8451.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/nmea.o: ../nmea.c  .generated_files/flags/Release/757bac9375e37cbbe770dfda574c6f130f535a74 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/nmea.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/nmea.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/nmea.o.d" -MT "${OBJECTDIR}/_ext/1472/nmea.o.d" -MT ${OBJECTDIR}/_ext/1472/nmea.o  -o ${OBJECTDIR}/_ext/1472/nmea.o ../nmea.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/oled.o: ../oled.c  .generated_files/flags/Release/7288be40f1570b5a632b7354740fcc28cc69f53a .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/oled.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/oled.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/oled.o.d" -MT "${OBJECTDIR}/_ext/1472/oled.o.d" -MT ${OBJECTDIR}/_ext/1472/oled.o  -o ${OBJECTDIR}/_ext/1472/oled.o ../oled.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/pneu.o: ../pneu.c  .generated_files/flags/Release/77d3ffc364164c11bbdb85acd0da0e314e1eee44 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/pneu.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pneu.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/pneu.o.d" -MT "${OBJECTDIR}/_ext/1472/pneu.o.d" -MT ${OBJECTDIR}/_ext/1472/pneu.o  -o ${OBJECTDIR}/_ext/1472/pneu.o ../pneu.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/ports.o: ../ports.c  .generated_files/flags/Release/961fba62571b535a277c983f4015d6caaf44ca86 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ports.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ports.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/ports.o.d" -MT "${OBJECTDIR}/_ext/1472/ports.o.d" -MT ${OBJECTDIR}/_ext/1472/ports.o  -o ${OBJECTDIR}/_ext/1472/ports.o ../ports.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/pushbutton.o: ../pushbutton.c  .generated_files/flags/Release/9ce3f6f6647fd44995d4af5b73b4c5a06e9fbca7 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/pushbutton.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pushbutton.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/pushbutton.o.d" -MT "${OBJECTDIR}/_ext/1472/pushbutton.o.d" -MT ${OBJECTDIR}/_ext/1472/pushbutton.o  -o ${OBJECTDIR}/_ext/1472/pushbutton.o ../pushbutton.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/report.o: ../report.c  .generated_files/flags/Release/e5d5665d8a577ecf8f9b909eff6e63510ed0fd56 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/report.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/report.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/report.o.d" -MT "${OBJECTDIR}/_ext/1472/report.o.d" -MT ${OBJECTDIR}/_ext/1472/report.o  -o ${OBJECTDIR}/_ext/1472/report.o ../report.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/roboclaw.o: ../roboclaw.c  .generated_files/flags/Release/550d8d47962193fbce8680a7a041c0e22fc9c6c0 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/roboclaw.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/roboclaw.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/roboclaw.o.d" -MT "${OBJECTDIR}/_ext/1472/roboclaw.o.d" -MT ${OBJECTDIR}/_ext/1472/roboclaw.o  -o ${OBJECTDIR}/_ext/1472/roboclaw.o ../roboclaw.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/set.o: ../set.c  .generated_files/flags/Release/d0a433e876a1ad48592111d10e0230ef0121c995 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/set.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/set.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/set.o.d" -MT "${OBJECTDIR}/_ext/1472/set.o.d" -MT ${OBJECTDIR}/_ext/1472/set.o  -o ${OBJECTDIR}/_ext/1472/set.o ../set.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/temperature.o: ../temperature.c  .generated_files/flags/Release/5446bf9a1681a8f8650e17d032f3aef83ba6c93f .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/temperature.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/temperature.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/temperature.o.d" -MT "${OBJECTDIR}/_ext/1472/temperature.o.d" -MT ${OBJECTDIR}/_ext/1472/temperature.o  -o ${OBJECTDIR}/_ext/1472/temperature.o ../temperature.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/testroutine.o: ../testroutine.c  .generated_files/flags/Release/4b603f28a01186343ff53853966cd947ad9cfbfd .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/testroutine.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/testroutine.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/testroutine.o.d" -MT "${OBJECTDIR}/_ext/1472/testroutine.o.d" -MT ${OBJECTDIR}/_ext/1472/testroutine.o  -o ${OBJECTDIR}/_ext/1472/testroutine.o ../testroutine.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/timers.o: ../timers.c  .generated_files/flags/Release/5dfc0f8bcfe7e082914e179e87f76797c6d5c767 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/timers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/timers.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/timers.o.d" -MT "${OBJECTDIR}/_ext/1472/timers.o.d" -MT ${OBJECTDIR}/_ext/1472/timers.o  -o ${OBJECTDIR}/_ext/1472/timers.o ../timers.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/twi.o: ../twi.c  .generated_files/flags/Release/450084d2f491dbcbf5cdbb8cd75767ec9bd3cd3f .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/twi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/twi.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/twi.o.d" -MT "${OBJECTDIR}/_ext/1472/twi.o.d" -MT ${OBJECTDIR}/_ext/1472/twi.o  -o ${OBJECTDIR}/_ext/1472/twi.o ../twi.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/usart.o: ../usart.c  .generated_files/flags/Release/41f3ce6c71304822ea1bac3c3fe75c2aaa1ec526 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/usart.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/usart.o.d" -MT "${OBJECTDIR}/_ext/1472/usart.o.d" -MT ${OBJECTDIR}/_ext/1472/usart.o  -o ${OBJECTDIR}/_ext/1472/usart.o ../usart.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/wdt.o: ../wdt.c  .generated_files/flags/Release/9e5e66a33d271311986ad7e6f3ca4716b3874e17 .generated_files/flags/Release/af643b557b3216c4a3387f3c4cb044d1432d1243
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/wdt.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/wdt.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega4809  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/include" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DNDEBUG -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/wdt.o.d" -MT "${OBJECTDIR}/_ext/1472/wdt.o.d" -MT ${OBJECTDIR}/_ext/1472/wdt.o  -o ${OBJECTDIR}/_ext/1472/wdt.o ../wdt.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/specMech.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mmcu=atmega4809 -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809"   -gdwarf-2 -D__$(MP_PROCESSOR_OPTION)__  -Wl,-Map="dist\${CND_CONF}\${IMAGE_TYPE}\specMech.X.${IMAGE_TYPE}.map"    -o dist/${CND_CONF}/${IMAGE_TYPE}/specMech.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1 -Wl,--gc-sections -Wl,--start-group  -Wl,-lm -Wl,-lm -Wl,--end-group  -lprintf_flt
	
	${MP_CC_DIR}\\avr-objcopy -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O ihex "dist/${CND_CONF}/${IMAGE_TYPE}/specMech.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "dist/${CND_CONF}/${IMAGE_TYPE}/specMech.X.${IMAGE_TYPE}.eep" || exit 0
	${MP_CC_DIR}\\avr-objdump -h -S "dist/${CND_CONF}/${IMAGE_TYPE}/specMech.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" > "dist/${CND_CONF}/${IMAGE_TYPE}/specMech.X.${IMAGE_TYPE}.lss"
	${MP_CC_DIR}\\avr-objcopy -O srec -R .eeprom -R .fuse -R .lock -R .signature "dist/${CND_CONF}/${IMAGE_TYPE}/specMech.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "dist/${CND_CONF}/${IMAGE_TYPE}/specMech.X.${IMAGE_TYPE}.srec"
	
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/specMech.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mmcu=atmega4809 -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/atmel/ATmega_DFP/1.6.364/gcc/dev/atmega4809"  -D__$(MP_PROCESSOR_OPTION)__  -Wl,-Map="dist\${CND_CONF}\${IMAGE_TYPE}\specMech.X.${IMAGE_TYPE}.map"    -o dist/${CND_CONF}/${IMAGE_TYPE}/specMech.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION) -Wl,--gc-sections -Wl,--start-group  -Wl,-lm -Wl,-lm -Wl,--end-group  -lprintf_flt
	${MP_CC_DIR}\\avr-objcopy -O ihex "dist/${CND_CONF}/${IMAGE_TYPE}/specMech.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "dist/${CND_CONF}/${IMAGE_TYPE}/specMech.X.${IMAGE_TYPE}.hex"
	${MP_CC_DIR}\\avr-objcopy -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O ihex "dist/${CND_CONF}/${IMAGE_TYPE}/specMech.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "dist/${CND_CONF}/${IMAGE_TYPE}/specMech.X.${IMAGE_TYPE}.eep" || exit 0
	${MP_CC_DIR}\\avr-objdump -h -S "dist/${CND_CONF}/${IMAGE_TYPE}/specMech.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" > "dist/${CND_CONF}/${IMAGE_TYPE}/specMech.X.${IMAGE_TYPE}.lss"
	${MP_CC_DIR}\\avr-objcopy -O srec -R .eeprom -R .fuse -R .lock -R .signature "dist/${CND_CONF}/${IMAGE_TYPE}/specMech.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "dist/${CND_CONF}/${IMAGE_TYPE}/specMech.X.${IMAGE_TYPE}.srec"
	
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} -r dist/Release

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
