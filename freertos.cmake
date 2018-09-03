#set(FREERTOS_DIR lib/FreeRTOSv9)
#set(FREERTOS_DIR lib/FreeRTOSv9_patched)
#set(FREERTOS_DIR lib/FreeRTOSv10.0.1)
set(FREERTOS_DIR lib/FreeRTOSv10.0.1_patched)

set(FREERTOS_PORT ARM_CM4F)
set(FREERTOS_COMPILER GCC)
set(FREERTOS_HEAPMANAGER heap_4)


add_library(freertos STATIC "")

file(GLOB FREERTOS_SRC
    "${FREERTOS_DIR}/Source/*.c"
    "${FREERTOS_DIR}/Source/portable/GCC/${FREERTOS_PORT}/*.c"
    "${FREERTOS_DIR}/Source/portable/MemMang/${FREERTOS_HEAPMANAGER}.c"
)

target_sources(freertos
    PRIVATE
        ${FREERTOS_SRC}
        "src/freertos/freertosHooks.c"
)

target_include_directories(freertos PUBLIC "${FREERTOS_DIR}/Source/include")
target_include_directories(freertos PUBLIC "inc/freertos")
target_include_directories(freertos PUBLIC "${FREERTOS_DIR}/Source/portable/${FREERTOS_COMPILER}/${FREERTOS_PORT}")
