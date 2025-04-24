#include "my_lvgl_uart.h"
#include "usart.h"
#include "stdio.h"
#include "main.h"

static lv_obj_t *input_box;   // 输入框
static lv_obj_t *receive_box;   // 输入框
static lv_obj_t *keyboard;  // 虚拟键盘
static lv_obj_t *send_btn,*clear_btn;
static lv_obj_t * uart_screen;
lv_obj_t * obj;
void send_data_event_handler(lv_event_t * e) 
{
   obj = lv_event_get_target(e); // 获取触发事件的对象	
	if(obj == send_btn)
	{
		const char *data = lv_textarea_get_text(input_box);
		HAL_UART_Transmit(&huart1, (uint8_t *)data,lv_strlen(data), HAL_MAX_DELAY); // 发送数据
	}
	else
	{
		lv_textarea_set_text(input_box, "");
		lv_textarea_set_text(receive_box, "");			
	}
}

// 回调函数，用于处理输入框的点击事件
void textarea_click_event_handler(lv_event_t * e) 
{
			bool is_hidden = lv_obj_has_flag(keyboard,LV_OBJ_FLAG_HIDDEN);
		
			if(is_hidden) 
			{
				lv_obj_remove_flag(keyboard,LV_OBJ_FLAG_HIDDEN);	
				lv_obj_move_foreground(keyboard);
			}	
			else lv_obj_add_flag(keyboard,LV_OBJ_FLAG_HIDDEN);
} 

void my_lvgl_set_receive_box(void)
{
	if(lv_scr_act()== uart_screen) lv_textarea_add_text(receive_box,(char *)rx_buff);
}
lv_obj_t * my_lvgl_uart(void) {
	
    uart_screen = lv_obj_create(NULL); // 创建新屏幕
		// 创建输入框
    input_box = lv_textarea_create(uart_screen);
    lv_obj_set_size(input_box, 155, 40);
    lv_obj_align(input_box, LV_ALIGN_TOP_LEFT, 10, 30);
    lv_textarea_set_placeholder_text(input_box, "input...");
    // 创建虚拟键盘
    keyboard = lv_keyboard_create(uart_screen); // 创建键盘
    lv_keyboard_set_textarea(keyboard, input_box); // 关联输入框与键盘
    lv_obj_set_height(keyboard, 150); // 设置键盘高度
    lv_obj_align(keyboard, LV_ALIGN_TOP_MID, 0, 70); // 键盘输入框下方
	
		lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);//初始时隐藏虚拟键盘
    // 为输入框添加事件处理器
    lv_obj_add_event_cb(input_box, textarea_click_event_handler, LV_EVENT_CLICKED, NULL);
	
    // 创建发送按钮
    send_btn = lv_btn_create(uart_screen);
    lv_obj_set_size(send_btn, 65, 40);
    lv_obj_align(send_btn, LV_ALIGN_TOP_RIGHT, -5, 30);
    lv_obj_add_event_cb(send_btn, send_data_event_handler, LV_EVENT_CLICKED, NULL);    
    lv_obj_t *send_btn_label = lv_label_create(send_btn);
    lv_label_set_text(send_btn_label, "Send");
		lv_obj_align(send_btn_label, LV_ALIGN_CENTER, 0, 0); 
		
		// 创建发送按钮
    clear_btn = lv_btn_create(uart_screen);
    lv_obj_set_size(clear_btn, 65, 40);
    lv_obj_align(clear_btn, LV_ALIGN_TOP_RIGHT, -5, 80);
    lv_obj_add_event_cb(clear_btn, send_data_event_handler, LV_EVENT_CLICKED, NULL);    
    lv_obj_t *clear_btn_label = lv_label_create(clear_btn);
    lv_label_set_text(clear_btn_label, "Clear");
		lv_obj_align(clear_btn_label, LV_ALIGN_CENTER, 0, 0); 
		// 创建接收框
    receive_box = lv_textarea_create(uart_screen);
    lv_obj_set_size(receive_box, 155, 180);
    lv_obj_align(receive_box, LV_ALIGN_TOP_LEFT, 10, 80);
    lv_textarea_set_placeholder_text(receive_box, "receive...");
    // 创建虚拟键盘	
		
	/****************************************************************************************************************
	 *设置背景色
	 *
	 ****************************************************************************************************************/
	lv_obj_set_style_bg_color(uart_screen, lv_color_hex(0x0), 0); // 设置渐变起始色
	lv_obj_set_style_bg_grad_color(uart_screen,lv_color_hex(0x0), 0); // 设置渐变结束色
	lv_obj_set_style_bg_grad_dir(uart_screen, LV_GRAD_DIR_VER, 0); // 设置为垂直渐变
	lv_obj_set_style_bg_opa(uart_screen, LV_OPA_COVER, 0); // 设置背景不透明		
		
		return uart_screen;
}
