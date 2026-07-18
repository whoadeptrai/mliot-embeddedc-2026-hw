#include "stm32f1xx_hal.h"



void GPIO_Init(void) {
    // 1. CẤP NGUỒN XUNG NHỊP
    // Cấp Clock cho Port C (chứa con LED) và Port A (chứa nút nhấn)
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // Khởi tạo một biến cấu trúc chung để dùng cho cả 2 chân
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // =========================================================
    // 2. CẤU HÌNH CHÂN PC13 (OUTPUT CHO LED - Giữ nguyên Task 3)
    // =========================================================
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;     
    GPIO_InitStruct.Pull = GPIO_NOPULL;             
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;    
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    // =========================================================
    // 3. CẤU HÌNH CHÂN PA0 (INPUT CHO NÚT NHẤN - Mới thêm)
    // =========================================================
    // M không cần tạo biến cấu trúc mới, cứ lấy cái cũ ghi đè lên cho tiết kiệm RAM
    GPIO_InitStruct.Pin = GPIO_PIN_0;               // Chọn chân PA0
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;         // Cấu hình là Input (Đầu vào)
    GPIO_InitStruct.Pull = GPIO_PULLUP;             // Bật điện trở kéo lên nguồn 3.3V nội bộ
    
    // (Lưu ý: Ở chế độ Input, vi điều khiển chỉ đọc trạng thái thụ động, 
    // nên thông số GPIO_InitStruct.Speed không có ý nghĩa, ta bỏ qua không ghi)
    
    // Gọi lệnh chốt cấu hình, nhưng lần này là chốt xuống Port A (GPIOA)
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

int main(void) {
    
    HAL_Init();

    GPIO_Init();
    /* TASK 3
    uint32_t last_blink_time = 0; 

    while (1) {
        if (HAL_GetTick() - last_blink_time >= 500) {
            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13); // Đảo LED
            last_blink_time = HAL_GetTick();        // Cập nhật lại mốc thời gian mới
        }
    
    } */

    while(1){
        uint8_t button_state = 1;       
        uint8_t last_reading = 1;      
        uint32_t last_debounce_time = 0; 


        uint8_t reading = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);

        if (reading != last_reading) {
            last_debounce_time = HAL_GetTick();
        }

        if ((HAL_GetTick() - last_debounce_time) >= 20) {
            
            if (reading != button_state) {
                
                if (button_state == 1) {
                    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
                }
            }
        }

        last_reading = reading;

    
    }
}


void SysTick_Handler(void) {
    HAL_IncTick();
}