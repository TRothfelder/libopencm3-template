include(utils.cmake)

cmake_minimum_required(VERSION 3.9)

project(stm32f4-discovery-sample VERSION 1.0.1 DESCRIPTION "description")
set(CMAKE_EXECUTABLE_SUFFIX .elf)

IF(NOT CMAKE_CROSSCOMPILING)
  message(FATAL_ERROR "Cross compiling only. Please use -DCMAKE_TOOLCHAIN_FILE=/PATH/TO/TOOLCHAIN_FILE .")
ENDIF(NOT CMAKE_CROSSCOMPILING)

set(LIBOPENCM3_DIR ${CMAKE_SOURCE_DIR}/lib/libopencm3)
add_custom_target(libopencm3 make WORKING_DIRECTORY ${LIBOPENCM3_DIR})
include_directories(${LIBOPENCM3_DIR}/include)
link_directories(${LIBOPENCM3_DIR}/lib)
add_definitions(-DSTM32F4)

set(STM32F4_FLAGS "-mfloat-abi=hard -mfpu=fpv4-sp-d16 -mthumb -mcpu=cortex-m4")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall ${STM32F4_FLAGS} -std=c11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++21 ${STM32F4_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${CMAKE_SOURCE_DIR}/stm32f4-discovery.ld -nostartfiles --specs=rdimon.specs")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

add_executable(button "")
target_sources(button PRIVATE
  "src/main.c"
  "src/config/clock.c"
  "src/config/gpio.c"
  "src/tasks/init.c"
  "src/tasks/ledblink.c"
  "src/tasks/usb.c"
)
target_include_directories(button PUBLIC "inc")
target_link_libraries(button opencm3_stm32f4)

include(freertos.cmake)
target_link_libraries(button freertos)

include(sysview.cmake)
target_link_libraries(button sysview)

firmware_size(button)
generate_object(button .hex ihex)
