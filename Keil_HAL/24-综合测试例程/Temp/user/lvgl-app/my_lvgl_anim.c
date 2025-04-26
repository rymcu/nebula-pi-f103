#include "my_lvgl_anim.h"
#include "rtthread.h"
#include "my_lvgl_nebula.h"		 //主页面实现
#include "my_lvgl_rgb.h"		 //RGB控制页面实现
#include "my_lvgl_temperature.h" //温度界面
#include "my_lvgl_uart.h"		 //串口界面
#include "my_lvgl_led.h"		 //添加8个led
#include "bsp_rgb.h"
LV_FONT_DECLARE(my_muyu)

lv_obj_t *one=NULL, *two=NULL, *three=NULL, *four=NULL, *five=NULL;
//static lv_obj_t *one, *two, *three, *four, *five;
#define MY_ICON_MUYU "\xEE\x98\x9E"

static lv_obj_t *five_btn;
static lv_obj_t *label5, *plus_one, *plus_sum;


static lv_obj_t *home_label;
static lv_style_t home_style; // 标签样式
int counter = 0;			  // 功德总数

void my_set_bg(lv_obj_t *obj)
{
	lv_obj_set_style_bg_color(obj, lv_color_hex(0x0), 0); 
	lv_obj_set_style_bg_grad_color(obj,lv_color_hex(0x0), 0); 
	lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, 0); 
	lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0); 
}

// 按键按下事件处理函数
static void button_event_handler_press(lv_event_t *e)
{
	// 获取按钮对象
	// lv_obj_t * button = lv_event_get_target(e);

	// 改变标签颜色
	lv_obj_set_style_text_color(label5, lv_color_hex(0xFFFFFF), 0); // 设置为白色
	lv_obj_remove_flag(plus_one, LV_OBJ_FLAG_HIDDEN);
	lv_label_set_text_fmt(plus_sum, "%d", counter++);

	ry_set_rgb(counter % 10 * 25, 0, 0); // 设置开发板RGB灯
}

// 按键松开事件处理函数
static void button_event_handler_release(lv_event_t *e)
{
	// 获取按钮对象
	// lv_obj_t * button = lv_event_get_target(e);

	// 恢复标签颜色
	lv_obj_set_style_text_color(label5, lv_color_hex(0xFF0000), 0); // 恢复为红色
	lv_obj_add_flag(plus_one, LV_OBJ_FLAG_HIDDEN);
}

typedef struct my_page
{
	uint32_t obj_addr;
} my_page_t;

lv_ll_t ll_test;
my_page_t *page_temp;
/*--------------------------------------------------------------------------------------------------------
*初始化链表，将页面依次添加到链表，并指向头节点
*
--------------------------------------------------------------------------------------------------------*/
void ry_list_init(void)
{
	_lv_ll_init(&ll_test, sizeof(my_page_t));

	page_temp = _lv_ll_ins_head(&ll_test);
	page_temp->obj_addr = (uint32_t)five;

	page_temp = _lv_ll_ins_head(&ll_test);
	page_temp->obj_addr = (uint32_t)four;
	page_temp = _lv_ll_ins_head(&ll_test);
	page_temp->obj_addr = (uint32_t)three;
	page_temp = _lv_ll_ins_head(&ll_test);
	page_temp->obj_addr = (uint32_t)two;
	page_temp = _lv_ll_ins_head(&ll_test);
	page_temp->obj_addr = (uint32_t)one;

	page_temp = _lv_ll_get_head(&ll_test);
}
/*--------------------------------------------------------------------------------------------------------
*获取节点
*
--------------------------------------------------------------------------------------------------------*/
void ry_get_prev(void)
{
	page_temp = _lv_ll_get_prev(&ll_test, page_temp);
	if (page_temp == NULL)
	{
		//		page_temp = _lv_ll_get_head(&ll_test);
		//		LV_LOG_USER("the begin page,no more!");
		page_temp = _lv_ll_get_tail(&ll_test); // 循环模式
		LV_LOG_USER("the end page!");
	}
}
void ry_get_next(void)
{
	page_temp = _lv_ll_get_next(&ll_test, page_temp);
	if (page_temp == NULL)
	{
		// page_temp = _lv_ll_get_tail(&ll_test);
		// LV_LOG_USER("the end page,no more!");

		page_temp = _lv_ll_get_head(&ll_test); // 循环模式
		LV_LOG_USER("the begin page!");
	}
}
/*--------------------------------------------------------------------------------------------------------
*手势事件函数
*
--------------------------------------------------------------------------------------------------------*/
void ry_gesture_event(lv_event_t *e)
{/*
	lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act()); // 获取手势方向
	lv_obj_t *target_obj = lv_event_get_target_obj(e);

	switch (dir)
	{
	case LV_DIR_TOP: // 从上往下
	{
		ry_get_prev();
		lv_scr_load_anim((lv_obj_t *)(page_temp->obj_addr), LV_SCR_LOAD_ANIM_OUT_TOP, 1000, 0, false);
		break;
	}
	case LV_DIR_BOTTOM: // 从下往上
	{
		ry_get_next();
		lv_scr_load_anim((lv_obj_t *)(page_temp->obj_addr), LV_SCR_LOAD_ANIM_OUT_BOTTOM, 1000, 0, false);
		break;
	}
	case LV_DIR_LEFT: // 从左往右
	{
		// 取消RGB页面左右滑动功能
		if ((lv_obj_t *)(page_temp->obj_addr) == two)
			break;
		ry_get_next();
		lv_scr_load((lv_obj_t *)(page_temp->obj_addr)); // 显示
		// lv_scr_load_anim((lv_obj_t *)(page_temp->obj_addr), LV_SCR_LOAD_ANIM_FADE_IN, 2000, 0, false);
		break;
	}
	case LV_DIR_RIGHT: // 从右往左
	{
		// 取消RGB页面左右滑动功能
		if ((lv_obj_t *)(page_temp->obj_addr) == two)
			break;
		ry_get_prev();
		lv_scr_load((lv_obj_t *)(page_temp->obj_addr)); // 显示
		// lv_scr_load_anim((lv_obj_t *)(page_temp->obj_addr), LV_SCR_LOAD_ANIM_FADE_IN, 2000, 0, false);
		break;
	}
	default:
		break;
	}*/
}
/*--------------------------------------------------------------------------------------------------------
*每个页面的左下角显示页面数字
*
--------------------------------------------------------------------------------------------------------*/
void create_page_label(_lv_obj_t *obj, uint8_t i)
{
	lv_obj_t *label = lv_label_create(obj);
	lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -20);
	
	static lv_style_t style;
  lv_style_init(&style);
	lv_style_set_text_color(&style, lv_color_hex(0xFFFFFF)); // 设置文本颜色为白色
	lv_obj_add_style(label, &style, 0); // 0表示第一层样式
	
	switch (i)
	{
	case 1:
		lv_label_set_text(label, "1");
		break;
	case 2:
		lv_label_set_text(label, "2");
		break;
	case 3:
		lv_label_set_text(label, "3");
		break;
	case 4:
		lv_label_set_text(label, "4");
		break;
	case 5:
		lv_label_set_text(label, "5");
		break;
	default:
		break;
	}
}

void back_home(lv_event_t *e)
{
	page_temp = _lv_ll_get_head(&ll_test); // back to homepage
	lv_scr_load_anim((lv_obj_t *)(page_temp->obj_addr), LV_SCR_LOAD_ANIM_FADE_IN, 0, 0, false);
	LV_LOG_USER("back to homepage!");
}
void create_home_button(lv_obj_t *parent)
{
	// 创建一个按钮
	lv_obj_t *btn = lv_btn_create(parent);

	// 设置按钮大小和位置
	lv_obj_set_size(btn, 30, 30);				 // 设定按钮的宽度和高度
	lv_obj_align(btn, LV_ALIGN_TOP_RIGHT, 0, 0); // 右上对齐

	// 设置按钮的透明样式
	lv_obj_set_style_bg_color(btn, lv_color_hex(0xFFFFFF), 0);	   // 背景颜色
	lv_obj_set_style_bg_opa(btn, LV_OPA_TRANSP, 0);				   // 设置背景透明度
	lv_obj_set_style_border_color(btn, lv_color_hex(0xFFFFFF), 0); // 边框颜色
	lv_obj_set_style_border_opa(btn, LV_OPA_TRANSP, 0);			   // 边框透明
	lv_obj_set_style_outline_width(btn, 0, 0);					   // 去掉外轮廓
	lv_obj_set_style_shadow_width(btn, 0, 0);					   // 去掉阴影

	// 添加事件处理函数
	lv_obj_add_event_cb(btn, back_home, LV_EVENT_CLICKED, NULL);

	home_label = lv_label_create(btn); // 创建标签
	lv_label_set_long_mode(home_label, LV_LABEL_LONG_WRAP);
	lv_label_set_text(home_label, LV_SYMBOL_HOME); // 创建标签文字
	lv_obj_align(home_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_pos(home_label, 0, 0); // 设置标签位置

	lv_style_init(&home_style);												// 初始化标签样式
	lv_style_set_text_color(&home_style, lv_palette_main(LV_PALETTE_BLUE)); // 标签文本颜色
	lv_style_set_text_font(&home_style, &lv_font_montserrat_12);			// 标签文本字体，自定义字体
	lv_obj_add_style(home_label, &home_style, 0);							// 关联标签样式
}

/*--------------------------------------------------------------------------------------------------------
*创建5个小灯，指示页面状态
*
--------------------------------------------------------------------------------------------------------*/
// 创建LED灯的函数
lv_obj_t *create_led(lv_obj_t *parent, int x, int y, lv_color_t color)
{
	// 创建一个圆形对象
	lv_obj_t *led = lv_obj_create(parent);

	// 设置大小为5x5
	lv_obj_set_size(led, 5, 5);

	// 设置圆形样式
	lv_obj_set_style_radius(led, LV_RADIUS_CIRCLE, 0); // 使其为圆形
	lv_obj_set_style_bg_color(led, color, 0);		   // 设置颜色
	lv_obj_set_style_bg_opa(led, LV_OPA_COVER, 0);	   // 设为不透明

	// 设置位置
	lv_obj_set_pos(led, x, y);

	return led;
}

void create_leds(lv_obj_t *parent, uint8_t page_num)
{
	// 获取屏幕宽度
	int screen_width = lv_obj_get_width(parent);

	// LED灯数量和大小
	int led_count = 5;
	int led_size = 5;
	int spacing = 5; // LED灯的间距

	// 计算总宽度
	int total_width = (led_count * led_size) + ((led_count - 1) * spacing);

	// 计算起始位置，确保LED灯居中
	int start_x = (screen_width - total_width) / 2;
	int start_y = lv_obj_get_height(parent) - 20; // 设置距离底部的高度

	// 创建5个LED灯并设置它们的位置和颜色
	for (int i = 0; i < led_count; i++)
	{
		int x_position = start_x + i * (led_size + spacing);
		// 根据颜色数组设置每个LED灯的颜色
		if (i == (page_num - 1))
			create_led(parent, x_position, start_y, lv_palette_main(LV_PALETTE_RED));
		else
			create_led(parent, x_position, start_y, lv_palette_main(LV_PALETTE_BLUE));
	}
}

/*--------------------------------------------------------------------------------------------------------
*创建4个不同颜色页面，并关联手势事件
*
--------------------------------------------------------------------------------------------------------*/
void create_page1(void)
{
	one = my_lvgl_nebula();
	if(one == NULL)
	{
		//rt_kprintf("one fail\n");
		return;
	}
	//rt_kprintf("one ok\n");
	// create_home_button(one);//主页不需要
	create_leds(one, 1);
	create_page_label(one, 1);
	//lv_obj_add_event_cb(one, ry_gesture_event, LV_EVENT_GESTURE, NULL);
	//my_set_bg(one);
}
void create_page2(void)
{
	two = my_lvgl_rgb();
	if(two == NULL)
	{
		rt_kprintf("two fail\n");
		return;
	}
	//rt_kprintf("two ok\n");
	create_home_button(two);
	create_leds(two, 2);
	create_page_label(two, 2);
	create_leds_8(two);
	//lv_obj_add_event_cb(two, ry_gesture_event, LV_EVENT_GESTURE, NULL);
	my_set_bg(two);
}
void create_page3(void)
{
	three = my_lvgl_temperature();
	if(three == NULL)
	{
		rt_kprintf("three fail\n");
		return;
	}
	//rt_kprintf("three ok\n");
	//three = lv_obj_create(NULL);
	
	create_home_button(three);
	create_leds(three, 3);
	create_page_label(three, 3);
//	lv_obj_add_event_cb(three, ry_gesture_event, LV_EVENT_GESTURE, NULL);
	my_set_bg(three);
}
void create_page4(void)
{
	four = my_lvgl_uart();
	//four = lv_obj_create(NULL);
		if(four == NULL)
	{
		rt_kprintf("four fail\n");
		return;
	}
	//rt_kprintf("four OK\n");

	create_home_button(four);
	create_leds(four, 4);
	create_page_label(four, 4);
//	lv_obj_add_event_cb(four, ry_gesture_event, LV_EVENT_GESTURE, NULL);
	my_set_bg(four);
}

void create_page5(void)
{
	five = lv_obj_create(NULL);															 
		if(five == NULL)
	{
		rt_kprintf("five fail\n");
		return;
	}
	//rt_kprintf("five ok\n");
	//lv_obj_set_style_bg_color(five, lv_palette_main(LV_PALETTE_GREY), LV_STATE_DEFAULT); 
	// lv_obj_set_size(five, 100, 100); 
	// lv_obj_align(five, LV_ALIGN_CENTER, 0, 0);        

	five_btn = lv_btn_create(five);
	lv_obj_align(five_btn, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_size(five_btn, 80, 80); 

	
	lv_obj_set_style_bg_color(five_btn, lv_color_hex(0xFFFFFF), 0);		
	lv_obj_set_style_bg_opa(five_btn, LV_OPA_TRANSP, 0);				
	lv_obj_set_style_border_color(five_btn, lv_color_hex(0xFFFFFF), 0); 
	lv_obj_set_style_border_opa(five_btn, LV_OPA_TRANSP, 0);			
	lv_obj_set_style_outline_width(five_btn, 0, 0);						
	lv_obj_set_style_shadow_width(five_btn, 0, 0);						

	label5 = lv_label_create(five); 
	lv_label_set_long_mode(label5, LV_LABEL_LONG_WRAP);
	lv_obj_align(label5, LV_ALIGN_CENTER, 0, 0); 
	lv_label_set_text(label5, MY_ICON_MUYU);	 

	static lv_style_t label5_style;											 
	lv_style_init(&label5_style);											 
	lv_style_set_text_color(&label5_style, lv_palette_main(LV_PALETTE_RED)); 
	lv_style_set_text_font(&label5_style, &my_muyu);						
	lv_obj_add_style(label5, &label5_style, 0);								 

	lv_obj_add_event_cb(five_btn, button_event_handler_press, LV_EVENT_PRESSED, NULL);
	lv_obj_add_event_cb(five_btn, button_event_handler_release, LV_EVENT_RELEASED, NULL);
	
	plus_one = lv_label_create(five);
	lv_label_set_long_mode(plus_one, LV_LABEL_LONG_WRAP);
	lv_label_set_text(plus_one, "+1"); // 
	
	static lv_style_t style;
  lv_style_init(&style);
	lv_style_set_text_color(&style, lv_color_hex(0x00FF00)); // 
	lv_obj_add_style(plus_one, &style, 0); // 
	lv_obj_set_pos(plus_one, 80, 100); // 
	// lv_obj_set_hidden(plus_one,true);
	lv_obj_add_flag(plus_one, LV_OBJ_FLAG_HIDDEN);
	// 
	plus_sum = lv_label_create(five);
	lv_label_set_long_mode(plus_sum, LV_LABEL_LONG_WRAP);
	lv_label_set_text_fmt(plus_sum, "%d", counter); // 初始化时显示初始值
	lv_obj_align(plus_sum, LV_ALIGN_TOP_MID, 0, 60);

	static lv_style_t plus_sum_style;											  // 标签样式
	lv_style_init(&plus_sum_style);												  // 初始化标签样式
	lv_style_set_text_color(&plus_sum_style, lv_palette_main(LV_PALETTE_ORANGE)); // 标签文本颜色
	lv_style_set_text_font(&plus_sum_style, &lv_font_montserrat_48);			  // 标签文本字体
	lv_obj_add_style(plus_sum, &plus_sum_style, 0);								  // 关联标签样式

	// five = my_lvgl_uart();
	create_leds(five, 5);
	create_page_label(five, 5);
	create_home_button(five);
//	lv_obj_add_event_cb(five, ry_gesture_event, LV_EVENT_GESTURE, NULL);
	
		/****************************************************************************************************************
	 *设置背景色
	 *
	 ****************************************************************************************************************/
	lv_obj_set_style_bg_color(five, lv_color_hex(0x0), 0); // 设置渐变起始色
	lv_obj_set_style_bg_grad_color(five,lv_color_hex(0x0), 0); // 设置渐变结束色
	lv_obj_set_style_bg_grad_dir(five, LV_GRAD_DIR_VER, 0); // 设置为垂直渐变
	lv_obj_set_style_bg_opa(five, LV_OPA_COVER, 0); // 设置背景不透明
	my_set_bg(five);
}
//加载切换页面
void load_my_page(uint8_t page)
{
	switch (page) {
			case 1:
					lv_scr_load(one);
					break;
			case 2:
				 lv_scr_load(two);
					break;
			case 3:
					lv_scr_load(three);
					break;
			case 4:
				 lv_scr_load(four);
					break;  
			case 5:
				 lv_scr_load(five);
					break;  		
			default:
					break;
	}
}
void lv_example_anim_1(void)
{
	create_page1();
	create_page2();
	create_page3();
	create_page4();
	create_page5();
	//rt_kprintf("after create_page5\n");
	ry_list_init(); // 初始化链表
	//rt_kprintf("after ry_list_init\n");
	lv_scr_load(one); // 显示page1
	//rt_kprintf("after lv_scr_load\n");

	//LV_LOG_USER("begin");
}
