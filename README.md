#STM32F0-Discovery Application Template
This package is for use when compiling programs for STM32F05xx ARM microcontrollers using arm-none-eabi-gcc (I'm using the [Code Sourcery G++:Lite Edition](http://www.mentor.com/embedded-software/sourcery-tools/sourcery-codebench/editions/lite-edition/) toolchain). The Makefile in the main directory will call the Make file in the Libraries directory, thereby automatically building the STM peripheral library. However, running 'make clean' will not affect the peripherals library (the same command can be run from the Libraries directory to do this).

This template will serve as a quick-start for those who do not wish to use an IDE, but rather develop in a text editor of choice and build from the command line. It is based on [an example template for the F4 Discovery board](http://jeremyherbert.net/get/stm32f4_getting_started) put together by Jeremy Herbert.

##Possible bug in the STM32F0xx peripheral library??
It should be noted that I currently cannot compile the stock STM32F0xx peripheral library files (v1.0.0) without getting the following error:

    STM32F0xx_StdPeriph_Driver/src/stm32f0xx_dac.c:151:26: error: 'RCC_APB1Periph_DAC' undeclared (first use in this function)
  
The workaround is to include stm32f0xx_rcc.h in the stm32f0xx_dac.c file. That is because the constant the compiler is complaining about is defined in that header file.

**Compiler warning about assert_param():** When compiling the standard peripherals a warning is thrown about implicit declaration of assert_param(). This is due to the order in which files are being included. The function prototype is found in the stm32f0xx_conf.h file which for some reason is included after the peripherals are compiled. I don't know how to give it priority so it is included at right away. Please open an issue (or submit a pull request) if you know the compiler directives necessary to fix these warnings.

##Subfolders:

1. Library/
   * This is the Library/ folder from the STM32F0xx_StdPeriph_Lib_V1.0.0 standard peripheral driver library produced by STM. This preserves the original structure which should make it easy to roll in library upgrades as they are published
   * **Makefile** is not part of the STM release, and must be copied over if the library is upgraded.
   * **IMPORTANT:** Please read the section about regarding a possible bug in one of the peripheral library files.

2. Device/
   * Folder contains device specific files:
   * **stm32_flash.ld** is the linker script taken from the STM32F0-Discovery firmware package. It is found in the following directory:
      * Project/Demonstration/TrueSTUDIO/STM32F0-Discovery_Demo/
   * **startup_stm32f0xx.s** is the startup file taken from the STM32F0-Discovery firmware package. It is found in the following directory:
      * Libraries/CMSIS/ST/STM32F0xx/Source/Templates/TrueSTUDIO/
   * **stm32f0xx_conf.h** is used to configure the peripheral library. The file was file taken from the STM32F0-Discovery firmware package. It is found in the following directory:
      * Project/Demonstration/

3. inc/
   * All include files for this particular project.

4. src/
   * All source files for this particular project (including main.c).
   * **system_stm32f0xx.c** can be generated using an XLS file developed by STM. This sets up the system clock values for the project. The file included in this repository is taken from the STM32F0-Discovery firmware package. It is found in the following directory:
      * Libraries/CMSIS/ST/STM32F0xx/Source/Templates/

5. extra/
   * This contains a procedure file used to write the image to the board via OpenOCD

##Loading the image on the board

If you have OpenOCD installed 'make program' can be used to flash the .bin file to the board. OpenOCD must be installed with stlink enabled. Clone [the git repository](http://openocd.git.sourceforge.net/git/gitweb.cgi?p=openocd/openocd;a=summary) and use these commands to compile/install it:

    ./bootstrap
    ./configure --prefix=/usr --enable-maintainer-mode --enable-stlink
    make 
    sudo make install

If there is an error finding the .cfg file, please double-check the OPENOCD_BOARD_DIR constant at the top of the Makefile (in this template directory, not in OpenOCD).