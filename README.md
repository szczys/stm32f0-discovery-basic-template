#STM32F0-Discovery Application Template
This package is for use when compiling programs for STM32F05xx ARM microcontrollers using arm-none-eabi-gcc (I'm using the [Code Sourcery G++:Lite Edition](http://www.mentor.com/embedded-software/sourcery-tools/sourcery-codebench/editions/lite-edition/) toolchain). The Makefile in the main directory will call the Make file in the Libraries directory, thereby automatically building the STM peripheral library. However, running 'make clean' will not affect the peripherals library (the same command can be run from the Libraries directory to do this).

This template will serve as a quick-start for those who do not wish to use an IDE, but rather develop in a text editor of choice and build from the command line. It is based on [an example template for the F4 Discovery board](http://jeremyherbert.net/get/stm32f4_getting_started) put together by Jeremy Herbert.

##Subfolders:

1. Library/
   * This is the Library/ folder from the STM32F0xx_StdPeriph_Lib_V1.0.0 standard peripheral driver library produced by STM. This preserves the original structure which should make it easy to roll in library upgrades as they are published
   * **Makefile** is not part of the STM release, and must be copied over if the library is upgraded.
   * **stm32f0xx_conf.h** is used to configure the peripheral library. It must be copied here if the library is upgraded. The file was file taken from the STM32F0-Discovery firmware package. It is found in the following directory:
      * Project/Demonstration/
   * **Abstracting the libraries:** You may place this folder anywhere you like in order to use it for multiple projects. Just change the path of the STD_PERIPH_LIB variable in the Makefile

2. Device/
   * Folder contains device specific files from the [ARM CMSIS 3.0 Libarary](http://www.onarm.com/cmsis/). Both can be found in the following directory:
      */CMSIS/DSP_Lib/Examples/Common/G++/
   * **ARMCMx.ld** is the linker script and has been modified to match the 64k Flash and 8k RAM sizes for the STM32F05x chip.
   * **startup_ARMCM0.s** is the startup code.

3. inc/
   * All include files for this particular project.

4. src/
   * All source files for this particular project (including main.c).
   * **system_stm32f0xx.c** can be generated using an XLS file developed by STM. This sets up the system clock values for the project. The file included in this repository is taken from the STM32F0-Discovery firmware package. It is found in the following directory:
      * Libraries/CMSIS/ST/STM32F0xx/Source/Templates/

5. extra/
   * This contains a procedure file used to write the image to the board via OpenOCD
   * **Abstracting the extra folder:** the .cfg file in the extra folder may be placed anywhere so that multiple projects can use one file. Just change the OPENOCD_PROC_FILE variable in the Make file to match the new location.

##Loading the image on the board

If you have OpenOCD installed 'make program' can be used to flash the .bin file to the board. OpenOCD must be installed with stlink enabled. Clone [the git repository](http://openocd.git.sourceforge.net/git/gitweb.cgi?p=openocd/openocd;a=summary) and use these commands to compile/install it:

    ./bootstrap
    ./configure --prefix=/usr --enable-maintainer-mode --enable-stlink
    make 
    sudo make install

If there is an error finding the .cfg file, please double-check the OPENOCD_BOARD_DIR constant at the top of the Makefile (in this template directory, not in OpenOCD).
