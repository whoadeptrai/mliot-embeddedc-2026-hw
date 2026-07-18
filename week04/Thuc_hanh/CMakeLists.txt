cmake_minimum_required(VERSION 3.16)

# ==============================================================================
# 1. BÁO CÁO CROSS-COMPILE (BẮT BUỘC ĐẶT TRƯỚC HÀM PROJECT)
# ==============================================================================
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Khai báo dự án hỗ trợ cả ngôn ngữ C và Assembly (ASM)
project(HW04_GPIO C ASM)

# Bật tính năng xuất file cấu hình cho IDE (Clangd)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# ==============================================================================
# 2. CẤU HÌNH TOOLCHAIN VÀ CỜ BIÊN DỊCH
# ==============================================================================
set(CMAKE_C_COMPILER "arm-none-eabi-gcc")
set(CMAKE_ASM_COMPILER "arm-none-eabi-gcc")
set(CMAKE_OBJCOPY "arm-none-eabi-objcopy")

# Định nghĩa cờ cho vi điều khiển Cortex-M3 (STM32F103xB)
set(MCU_FLAGS "-mcpu=cortex-m3 -mthumb -DSTM32F103xB")
set(CMAKE_C_FLAGS "${MCU_FLAGS} -O0 -g3 -Wall")
set(CMAKE_ASM_FLAGS "${MCU_FLAGS} -O0 -g3 -Wall")

# Cấu hình Linker Script (Bản đồ bộ nhớ) và cờ bỏ qua thư viện HĐH
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/STM32F103C8TX_FLASH.ld")
set(CMAKE_EXE_LINKER_FLAGS "${MCU_FLAGS} -T ${LINKER_SCRIPT} -Wl,--gc-sections --specs=nano.specs --specs=nosys.specs")

# ==============================================================================
# 3. ĐƯỜNG DẪN THƯ VIỆN & KHAI BÁO MÃ NGUỒN
# ==============================================================================
# Trỏ lùi ra 2 cấp thư mục để vào kho STM32CubeF1
set(STM32_CUBE_DIR "${CMAKE_SOURCE_DIR}/../../STM32CubeF1")
set(HAL_DIR "${STM32_CUBE_DIR}/Drivers/STM32F1xx_HAL_Driver")
set(CMSIS_DIR "${STM32_CUBE_DIR}/Drivers/CMSIS")

# Khai báo các file mã nguồn C và file khởi động Assembly (.s)
add_executable(${PROJECT_NAME}.elf
    main.c
    ${HAL_DIR}/Src/stm32f1xx_hal.c
    ${HAL_DIR}/Src/stm32f1xx_hal_gpio.c
    ${HAL_DIR}/Src/stm32f1xx_hal_rcc.c
    ${HAL_DIR}/Src/stm32f1xx_hal_rcc_ex.c
    ${HAL_DIR}/Src/stm32f1xx_hal_cortex.c
    ${CMSIS_DIR}/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.c
    ${CMSIS_DIR}/Device/ST/STM32F1xx/Source/Templates/gcc/startup_stm32f103xb.s
)

# Trỏ đường dẫn để tìm các file Header (.h)
target_include_directories(${PROJECT_NAME}.elf PRIVATE
    .   
    ${HAL_DIR}/Inc
    ${HAL_DIR}/Inc/Legacy
    ${CMSIS_DIR}/Include
    ${CMSIS_DIR}/Device/ST/STM32F1xx/Include
)

# ==============================================================================
# 4. TẠO FILE BIN ĐỂ NẠP XUỐNG MẠCH (Cho STM32CubeProgrammer)
# ==============================================================================
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${CMAKE_BINARY_DIR}/app_firmware.bin
    COMMENT "Generating binary flash image: build/app_firmware.bin"
)