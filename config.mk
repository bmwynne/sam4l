#
# Copyright (c) 2011 Atmel Corporation. All rights reserved.
#
# \asf_license_start
#
# \page License
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. The name of Atmel may not be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# 4. This software may only be redistributed and used in connection with an
#    Atmel microcontroller product.
#
# THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
# EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
# STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# \asf_license_stop
#

# Path to top level ASF directory relative to this project directory.
PRJ_PATH = src
BUILD_DIR = build 
OBJ_DIR = obj

# Target CPU architecture: cortex-m3, cortex-m4
ARCH = cortex-m4

# Target part: none, sam3n4 or sam4l4aa
PART = sam4lc2a

# Application target name. Given with suffix .a for library and .elf for a
# standalone application.
TARGET_FLASH = sam4l-printer-flash.elf
TARGET_SRAM = sam4l-printer-sram.elf

# List of C source files.
CSRCS = \
       main.c                                          \
       cellular/cellular.c                             \
       modem_usart/mdm_usart.c                            \
       gps/gps.c                                       \
       modem_service/mdm_service.c                     \
       power/power.c                                   \
       printer/cp_printer.c                            \
       usb/cp_usb.c                                    \
       usb/cp_uhi/uhi_zt230.c                          \
       asf/common/services/clock/sam4l/dfll.c                 \
       asf/common/services/clock/sam4l/osc.c                  \
       asf/common/services/clock/sam4l/pll.c                  \
       asf/common/services/clock/sam4l/sysclk.c               \
       asf/common/services/serial/usart_serial.c              \
       asf/common/services/sleepmgr/sam4l/sleepmgr.c          \
       asf/common/services/usb/class/vendor/host/example/sam4lc8c_sam4l8_xplained_pro/ui.c \
       asf/common/services/usb/class/vendor/host/uhi_vendor.c \
       asf/common/services/usb/uhc/uhc.c                      \
       asf/common/services/delay/sam/cycle_counter.c          \
       asf/common/utils/interrupt/interrupt_sam_nvic.c        \
       asf/common/utils/stdio/read.c                          \
       asf/common/utils/stdio/write.c                         \
       asf/sam/boards/user_board/init.c                       \
       asf/sam/drivers/bpm/bpm.c                              \
       asf/sam/drivers/flashcalw/flashcalw.c                  \
       asf/sam/drivers/pdca/pdca.c                            \
       asf/sam/drivers/tc/tc.c                                \
       asf/sam/drivers/usart/usart.c                          \
       asf/sam/drivers/wdt/wdt_sam4l.c                        \
       asf/sam/drivers/eic/eic.c                              \
       asf/sam/drivers/gpio/gpio.c                            \
       asf/sam/drivers/usbc/usbc_host.c                       \
       asf/sam/utils/cmsis/sam4l/source/templates/exceptions.c \
       asf/sam/utils/cmsis/sam4l/source/templates/gcc/startup_sam4l.c \
       asf/sam/utils/cmsis/sam4l/source/templates/system_sam4l.c \
       asf/sam/utils/syscalls/gcc/syscalls.c

# List of assembler source files.
ASSRCS = 

# List of include paths.
INC_PATH = \
       /                                               \
       config                                          \
       cellular                                        \
       modem_usart                                     \
       gps                                             \
       modem_service                                   \
       power                                           \
       printer                                         \
       usb                                             \
       usb/cp_uhi                                      \
       asf/                                            \
       asf/common/boards                                      \
       asf/common/services/clock                              \
       asf/common/services/ioport                             \
       asf/common/services/serial                             \
       asf/common/services/serial/sam_uart                    \
       asf/common/services/sleepmgr                           \
       asf/common/services/usb                                \
       asf/common/services/usb/uhc                            \
       asf/common/services/usb/class/vendor                   \
       asf/common/services/usb/class/vendor/host              \
       asf/common/services/delay                              \
       asf/common/services/delay/sam                          \
       asf/common/utils                                       \
       asf/common/utils/stdio/stdio_serial                    \
       asf/sam/boards                                         \
       asf/sam/boards/user_board                              \
       asf/sam/drivers/usbc                                   \
       asf/sam/drivers/bpm                                    \
       asf/sam/drivers/flashcalw                              \
       asf/sam/drivers/pdca                                   \
       asf/sam/drivers/tc                                     \
       asf/sam/drivers/usart                                  \
       asf/sam/drivers/wdt                                    \
       asf/sam/drivers/eic                                    \
       asf/sam/drivers/gpio                                   \
       asf/sam/utils                                          \
       asf/sam/utils/cmsis/sam4l/include                      \
       asf/sam/utils/cmsis/sam4l/source/templates             \
       asf/sam/utils/header_files                             \
       asf/sam/utils/preprocessor                             \
       asf/thirdparty/CMSIS/Include                           \
       asf/thirdparty/CMSIS/Lib/GCC 

# Additional search paths for libraries.
LIB_PATH =  \
       asf/thirdparty/CMSIS/Lib/GCC                          

# List of libraries to use during linking.
LIBS =  \
       arm_cortexM4l_math                                 \
       m                                                 

# Path relative to top level directory pointing to a linker script.
LINKER_SCRIPT_FLASH = asf/sam/utils/linker_scripts/sam4l/sam4l2/gcc/flash.ld
#LINKER_SCRIPT_SRAM  = sam/utils/linker_scripts/sam4l/sam4l2/gcc/sram.ld

# Path relative to top level directory pointing to a linker script.
#DEBUG_SCRIPT_FLASH = sam/boards/sam4l8_xplained_pro/debug_scripts/gcc/sam4l8_xplained_pro_flash.gdb
#DEBUG_SCRIPT_SRAM  = sam/boards/sam4l8_xplained_pro/debug_scripts/gcc/sam4l8_xplained_pro_sram.gdb

# Project type parameter: all, sram or flash
PROJECT_TYPE        = flash

# Additional options for debugging. By default the common Makefile.in will
# add -g3.
DBGFLAGS = 

# Application optimization used during compilation and linking:
# -O0, -O1, -O2, -O3 or -Os
OPTIMIZATION = -O1

# Extra flags to use when archiving.
ARFLAGS = 

# Extra flags to use when assembling.
ASFLAGS = 

# Extra flags to use when compiling.
CFLAGS = 

# Extra flags to use when preprocessing.
#
# Preprocessor symbol definitions
#   To add a definition use the format "-D name[=definition]".
#   To cancel a definition use the format "-U name".
#
# The most relevant symbols to define for the preprocessor are:
#   BOARD      Target board in use, see boards/board.h for a list.
#   EXT_BOARD  Optional extension board in use, see boards/board.h for a list.
CPPFLAGS = \
       -D ARM_MATH_CM4=true                               \
       -D BOARD=USER_BOARD                                \
       -D __SAM4LC2A__                                    \
       -D printf=iprintf                                  \
       -D scanf=iscanf

# Extra flags to use when linking
LDFLAGS = \

# Pre- and post-build commands
PREBUILD_CMD = 
POSTBUILD_CMD = 