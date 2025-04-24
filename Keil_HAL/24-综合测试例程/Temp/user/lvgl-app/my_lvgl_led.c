#include "my_lvgl_led.h"
#include "main.h" // 确保你包含了适当的STM32头文件

#define LED_COUNT 8 // LED灯数量
#define LED_WIDTH  25
#define LED_HEIGHT  25
#define LED_SPACING 5

 //用于控制LED的GPIO引脚
GPIO_TypeDef* LED_GPIO_PORT[LED_COUNT] = {LED1_GPIO_Port, LED2_GPIO_Port, LED3_GPIO_Port, LED4_GPIO_Port, LED5_GPIO_Port, LED6_GPIO_Port, LED7_GPIO_Port, LED8_GPIO_Port};
uint16_t LED_GPIO_PIN[LED_COUNT] = {LED1_Pin, LED2_Pin, LED3_Pin, LED4_Pin, LED5_Pin, LED6_Pin, LED7_Pin, LED8_Pin};

 //点击事件处理函数，切换LED灯的状态
static void led_button_event_handler(lv_event_t * e) {
    lv_obj_t * led_button = lv_event_get_target(e); // 获取点击的LED按钮
    uint8_t index = (uintptr_t)lv_obj_get_user_data(led_button); // 获取按钮索引

    // 获取当前状态
		lv_color_t bg_color = lv_obj_get_style_bg_color(led_button,NULL);
    
    // 切换LED的状态
    if (0xFF == bg_color.blue) {
        HAL_GPIO_WritePin(LED_GPIO_PORT[index], LED_GPIO_PIN[index], GPIO_PIN_SET); // 关闭LED
        lv_obj_set_style_bg_color(led_button, lv_color_hex(0x808080), 0); // 设置为灰色（关闭状态）
    } else {
        HAL_GPIO_WritePin(LED_GPIO_PORT[index], LED_GPIO_PIN[index], GPIO_PIN_RESET); // 打开LED
        lv_obj_set_style_bg_color(led_button, lv_color_hex(0x0000FF), 0); // 设置为蓝色（打开状态）
    }
}

void create_leds_8(lv_obj_t * parent) {
    // 创建8个LED灯
    for (int i = 0; i < LED_COUNT; i++) {
        lv_obj_t * led_button = lv_btn_create(parent);
        lv_obj_set_size(led_button, LED_WIDTH, LED_HEIGHT);
        lv_obj_align(led_button, LV_ALIGN_BOTTOM_MID, (LED_WIDTH + LED_SPACING) * (i - (LED_COUNT / 2)) + (LED_WIDTH / 2), -60);

        // 设置初始状态为关闭
        lv_obj_set_style_bg_color(led_button, lv_color_hex(0x808080), 0); // 灰色
        lv_obj_set_style_radius(led_button, LED_WIDTH / 2, 0); // 使其呈圆形
        
        // 设置按钮用户数据为LED索引
        lv_obj_set_user_data(led_button, (void*)(uintptr_t)i); // 保存LED索引

        // 添加点击事件
        lv_obj_add_event_cb(led_button, led_button_event_handler, LV_EVENT_CLICKED, NULL);
    }
}
