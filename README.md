STM32F1xx Development Environment
=================================
Installation
------------
  1. Clone
  2. install:
        ```
	    arm-none-eabi-gcc
        arm-none-eabi-gdb
        arm-none-eabi-binutils
        newlib
        openocd
        ```
  3. Get STMCube for STM32F1 from ST's website
        
Configuration
=============
Copy the STMCube files to `./resource/STM32CubeF1` or set `CUBE_PATH` 

Usage
=====
  * use `make` to build for the target
  * use `make program` to upload to the target
  * use `openocd` (may need to run as root) to open a communication channel to the device. This is known working with JLink, not yet working with STLink
  * use `arm-none-eabi-gdb` to get a debugger shell
  * enter the following into the gdb shell to connect to openocd
     ```
     (gdb) target remote localhost:3333
     ```

Credits
=======
Nearly everything here was created by Jaseg (https://github.com/jaseg), I just packaged it up into a repo and added this Readme with user friendly instructions.

