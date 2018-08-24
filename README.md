README
======


STM32 and LibOpenCM3 template project
---------------------------------

This repository provides a simple CMake project for easier reuse.
It can be used with every processor which is supported by LibOpenCM3. Therefore one needs to edit the CMakeLists.txt and the linker file. However, the files provcided in this project are set up to work with the STM32F4 Discovery Board.


New to embedded projects?
-------------------------

The series of articles from Chris Lalancette might be useful for you:
  - [Developing STM32 microcontroller code on Linux (Part 1 of 8, introduction)](http://clalance.blogspot.de/2014/01/developing-stm32-microcontroller-code.html)
  - [Developing STM32 microcontroller code on Linux (Part 2 of 8, building the cross-compiler)](http://clalance.blogspot.de/2014/01/developing-stm32-microcontroller-code_6.html)
  - [Developing STM32 microcontroller code on Linux (Part 3 of 8, building gdb)](http://clalance.blogspot.de/2014/01/developing-stm32-microcontroller-code_8.html)
  - [Developing STM32 microcontroller code on Linux (Part 4 of 8, building openocd)](http://clalance.blogspot.de/2014/01/developing-stm32-microcontroller-code_9.html)
  - [Developing STM32 microcontroller code on Linux (Part 5 of 8, building libopencm3)](http://clalance.blogspot.de/2014/01/developing-stm32-microcontroller-code_10.html)
  - [Developing STM32 microcontroller code on Linux (Part 6 of 8, building and linking STM32 programs)](http://clalance.blogspot.de/2014/01/developing-stm32-microcontroller-code_13.html)
  - [Developing STM32 microcontroller code on Linux (Part 7 of 8, building and running a simple STM32 program)](http://clalance.blogspot.de/2014/01/developing-stm32-microcontroller-code_15.html)

For C++ on embedded projects see [Practical Guide to Bare Metal C++](https://arobenko.gitbooks.io/bare_metal_cpp/content/) or the [PDF - Practical Guide to Bare Metal C++](other/bare_metal_cpp.pdf).

External libraries
----------------

External libraries used in this project
  - [LibOpenCM3](http://libopencm3.org/) ([github](https://github.com/libopencm3/libopencm3))
  - [FreeRTOS v9](https://www.freertos.org/)
  - [FreeRTOS v10](https://www.freertos.org/)
  - [Segger Sysview](https://www.segger.com/products/development-tools/systemview/)


Prerequisites
-------------
Same prerequisites as for [LibOpenCM3](http://libopencm3.org/).

Building requires Python (Some code is generated).

**For Ubuntu/Fedora:**

 - An arm-none-eabi/arm-elf toolchain.

**For Windows:**

 Download and install:

 - msys - http://sourceforge.net/projects/mingw/files/MSYS/Base/msys-core/msys-1.0.11/MSYS-1.0.11.exe
 - Python - http://www.python.org/ftp/python/2.7/python-2.7.msi (any 2.7 release)
 - arm-none-eabi/arm-elf toolchain (for example this one https://launchpad.net/gcc-arm-embedded)

Run msys shell and set the path without standard Windows paths, so Windows programs such as 'find' won't interfere:

    export PATH="/c//Python27:/c/ARMToolchain/bin:/usr/local/bin:/usr/bin:/bin"


Building
--------
From root directory:

    $ git submodule update --init
    $ mkdir debug
    $ cd debug
    $ cmake -DCMAKE_TOOLCHAIN_FILE=arm-none-eabi-gcc.cmake -DCMAKE_BUILD_TYPE=Debug ../ (only needs to be run once)
    $ make libopencm3 (only needs to be run once)
    $ make

Make targets:
  - libopencm3 (builds LibOpenCM3)


Flashing
--------

Keep in mind: the given files in utils/openocd are for the STM32F4. They might work with different cpus but I didn't test that.
IMPORTANT NOTE: I flashed [J_link](https://www.segger.com/products/debug-probes/j-link/models/other-j-links/st-link-on-board/) to the onboard ST-Link. So this does not work with the default oboard ST-Link

From root directory:

    $ openocd -f utils/openocd/jlink.cfg -c "flash_stm32f4 bin/fancyblink.elf"


Structure
---------

There are 4 different folders:
  - inc
  - lib
  - src
  - other

The inc folder holds all the include files.
The src folder holds all the sources.
The lib folder holds all the external libraries.
The other folder holds some scripts or binaries to make your life easier.

Defines
------

Makefile defines some preprocessor defines depending on the settings.
The defines are:
  - DEBUG (compiled as debug build)
  - SEGGER_SYSVIEW_USE (compiled with segger sysview)


Fine-tuning the build
---------------------

To fine tune the build one needs to edit the cmake files.


Start own project with this one as base
---------------------------------------

Using a STM32F4 Discovery or the STM32F407VGT6: you just need to add your custom sources and includes.

Other Platform/CPU:
You need to edit the following cmake files:



License
-------

All libraries are licensed under their own License. Visit their projects to see the licenses.

If not stated otherwise in the file, the  code is released under the terms of the GNU General
Public License (GPL), version 3 or later.

See COPYING.GPL3 and COPYING.LGPL3 for details.
