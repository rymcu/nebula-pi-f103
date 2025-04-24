
#include "lvgl.h"
#include "ry_test.h"
// 定义函数指针类型
void (*screen_func_ptr)(void);



typedef struct my_page
{
	uint32_t screen_addr;
} my_page_t;

lv_ll_t ll_test1;
my_page_t *page_temp1;
/*--------------------------------------------------------------------------------------------------------
*初始化链表，将页面依次添加到链表，并指向头节点
*
--------------------------------------------------------------------------------------------------------*/
void ry_list_init1(void)
{	
	_lv_ll_init(&ll_test1, sizeof(my_page_t));

	page_temp1 = _lv_ll_ins_head(&ll_test1);
	page_temp1->screen_addr = (uint32_t)create_screen2;//函数名称就是函数地址
	page_temp1 = _lv_ll_ins_head(&ll_test1);
	page_temp1->screen_addr = (uint32_t)create_screen1;

	
	page_temp1 = _lv_ll_get_head(&ll_test1);
}

void ry_test(void)
{
	ry_list_init1();
	screen_func_ptr = (void(*)(void))(page_temp1->screen_addr);
	screen_func_ptr();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 全局变量
static lv_obj_t * current_screen = NULL; // 当前屏幕对象

void delete_current_screen(void) 
	{
    if (current_screen != NULL) {
        lv_obj_del(current_screen); // 删除当前屏幕及其所有对象
        current_screen = NULL; // 清空指针
    }
}

void event_next_button(lv_event_t * e) {
    create_screen2(); // 切换到屏幕2
}

void event_back_button(lv_event_t * e) {
    create_screen1(); // 切换回屏幕1
}


// 创建屏幕1
void create_screen1(void) {
    // 删除旧屏幕
    delete_current_screen();

    // 创建新屏幕1并设置样式
    lv_obj_t * screen1 = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen1, lv_color_hex(0xFFFFFF), 0); 

    // 创建标题标签
    lv_obj_t * label1 = lv_label_create(screen1);
    lv_label_set_text(label1, "This is Screen 1");
    lv_obj_align(label1, LV_ALIGN_CENTER, 0, -20);

    // 创建切换到屏幕2的按钮
    lv_obj_t * btn_next = lv_btn_create(screen1);
    lv_obj_align(btn_next, LV_ALIGN_CENTER, 0, 20);
    lv_obj_t * label_next = lv_label_create(btn_next);
    lv_label_set_text(label_next, "Go to Screen 2");
    lv_obj_add_event_cb(btn_next, event_next_button, LV_EVENT_CLICKED, NULL);

    current_screen = screen1; // 更新当前屏幕
    lv_scr_load(screen1); // 加载新屏幕
}

// 创建屏幕2
void create_screen2(void) {
    // 删除旧屏幕
    delete_current_screen();

    // 创建新屏幕2并设置样式
    lv_obj_t * screen2 = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen2, lv_color_hex(0xFFFFFF), 0); 

    // 创建标题标签
    lv_obj_t * label2 = lv_label_create(screen2);
    lv_label_set_text(label2, "This is Screen 2");
    lv_obj_align(label2, LV_ALIGN_CENTER, 0, -20);

    // 创建切换回屏幕1的按钮
    lv_obj_t * btn_back = lv_btn_create(screen2);
    lv_obj_align(btn_back, LV_ALIGN_CENTER, 0, 20);
    lv_obj_t * label_back = lv_label_create(btn_back);
    lv_label_set_text(label_back, "Back to Screen 1");
    lv_obj_add_event_cb(btn_back, event_back_button, LV_EVENT_CLICKED, NULL);

    current_screen = screen2; // 更新当前屏幕
    lv_scr_load(screen2); // 加载新屏幕
}

