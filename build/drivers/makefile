#
# Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/ 
#
# 
#  Redistribution and use in source and binary forms, with or without 
#  modification, are permitted provided that the following conditions 
#  are met:
#
#    Redistributions of source code must retain the above copyright 
#    notice, this list of conditions and the following disclaimer.
#
#    Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the 
#    documentation and/or other materials provided with the   
#    distribution.
#
#    Neither the name of Texas Instruments Incorporated nor the names of
#    its contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
#  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
#  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
#  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
#  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#
#
# Locating the root directory
#
ROOT=../../..

# Device definition (EVM not necessary)
#
DEVICE=am335x

#
# Include makefile definitions. Includes compilation, linker and archiving
# options.
#
include ${ROOT}/test/MAACS_makedefs

#
# Source files for given target
#
SOURCE=${MAACS_DRIVERS_SRC}/dbg.c \
       ${MAACS_DRIVERS_SRC}/control_module.c \
       ${MAACS_DRIVERS_SRC}/clocks.c \
			 ${MAACS_DRIVERS_SRC}/uart.c\
			 ${MAACS_DRIVERS_SRC}/i2c.c \
			 ${MAACS_DRIVERS_SRC}/mmcsd.c\
			 ${MAACS_DRIVERS_SRC}/sd_card.c\
			 ${MAACS_DRIVERS_SRC}/timer.c\
			 ${MAACS_DRIVERS_SRC}/pwm.c\
			 ${MAACS_DRIVERS_SRC}/util.c\
			 ${MAACS_DRIVERS_SRC}/magnetometer.c\
			 ${MAACS_DRIVERS_SRC}/gpio.c\
			 ${MAACS_DRIVERS_SRC}/torque_rod.c\
			 ${MAACS_DRIVERS_SRC}/test.c\
			 ${MAACS_DRIVERS_SRC}/test_temp.c\
			 ${MAACS_DRIVERS_SRC}/test_torq_rod.c\
			 ${MAACS_DRIVERS_SRC}/ad7991.c\
			 ${MAACS_DRIVERS_SRC}/global.c\

#
# Rules for  building the source files
#
all: debug release

debug:
	mkdir -p Debug
	$(CC) $(CFLAGS) -g $(SOURCE)
	$(AR) $(ARFLAGS) Debug/libmaacs_drivers.a *.o*
	@mv *.o* Debug/
	@mkdir -p ${MAACS_DRIVERS_BIN}/Debug
	@cp Debug/libmaacs_drivers.a ${MAACS_DRIVERS_BIN}/Debug/libmaacs_drivers.a

release:
	mkdir -p Release
	$(CC) $(CFLAGS) -g -O2 $(SOURCE)
	$(AR) $(ARFLAGS) Release/libmaacs_drivers.a *.o*
	@mv *.o* Release/
	@mkdir -p ${MAACS_DRIVERS_BIN}/Release
	@cp Release/libmaacs_drivers.a ${MAACS_DRIVERS_BIN}/Release/libmaacs_drivers.a

#
# Rules for cleaning
#
clean:
	@rm -rf Debug Release ${MAACS_DRIVERS_BIN}/Debug ${MAACS_DRIVERS_BIN}/Release


