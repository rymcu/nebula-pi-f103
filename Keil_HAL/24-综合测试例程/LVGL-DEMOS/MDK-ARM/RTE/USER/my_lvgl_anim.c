#include "my_lvgl_anim.h"

#include "my_lvgl_nebula.h"//主页面实现
#include "my_lvgl_rgb.h" //RGB控制页面实现

static lv_obj_t *one_btn,*two_btn,*three_btn,*four_btn;
static lv_obj_t *label,*label2,*label3,*label4;

static lv_obj_t * current_screen = NULL; // 当前屏幕对象

//static lv_obj_t *one,*two,*three,*four;
typedef struct my_page
{
	uint32_t page_func_addr;//存储创建页面函数的地址
} my_page_t;

my_page_t *page_temp;

lv_ll_t ll_test;//链表，创建页面函数的地址

lv_obj_t * (*screen_func_ptr)(void);//定义函数指针类型

void delete_current_screen(void) 
	{
    if (current_screen != NULL) {
        lv_obj_delete(current_screen); // 删除当前屏幕及其所有对象
        current_screen = NULL; // 清空指针
			LV_LOG_USER("delete screen ok!");
    }
}
/*--------------------------------------------------------------------------------------------------------
*初始化链表，将页面创建函数地址依次添加到链表，并指向头节点
*
--------------------------------------------------------------------------------------------------------*/
void ry_list_init(void)
{	
	_lv_ll_init(&ll_test, sizeof(my_page_t));

	page_temp = _lv_ll_ins_head(&ll_test);
	page_temp->page_func_addr = (uint32_t)create_page4;
	page_temp = _lv_ll_ins_head(&ll_test);
	page_temp->page_func_addr = (uint32_t)create_page3;
	page_temp = _lv_ll_ins_head(&ll_test);
	page_temp->page_func_addr = (uint32_t)create_page2;
	page_temp = _lv_ll_ins_head(&ll_test);
	page_temp->page_func_addr = (uint32_t)create_page1;
	
	page_temp = _lv_ll_get_head(&ll_test);
}
/*--------------------------------------------------------------------------------------------------------
*获取节点
*
--------------------------------------------------------------------------------------------------------*/
void ry_get_prev(void)
{
	page_temp = _lv_ll_get_prev(&ll_test,page_temp);
	if(page_temp == NULL)
	{
		page_temp = _lv_ll_get_head(&ll_test);
		LV_LOG_USER("the begin page,no more!");
	}
}
void ry_get_next(void)
{
	page_temp = _lv_ll_get_next(&ll_test,page_temp);
	if(page_temp == NULL)
	{
		page_temp = _lv_ll_get_tail(&ll_test);
		LV_LOG_USER("the end page,no more!");
	}
}
/*--------------------------------------------------------------------------------------------------------
*手势事件函数
*
--------------------------------------------------------------------------------------------------------*/
void ry_gesture_event(lv_event_t *e)
{
	lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());//获取手势方向
	lv_obj_t *target_obj = lv_event_get_target_obj(e);
	lv_obj_t* temp_screen = NULL;
	switch(dir)
	{
		case LV_DIR_TOP://从上往下
		{
			ry_get_prev();
			screen_func_ptr = (lv_obj_t *(*)(void))(page_temp->page_func_addr);//获取当前页面创建函数地址
			temp_screen = screen_func_ptr();
			lv_scr_load_anim(temp_screen, LV_SCR_LOAD_ANIM_OUT_TOP, 1000, 0, false);//调用函数并显示

			break;
		}
		case LV_DIR_BOTTOM://从下往上
		{
			ry_get_next();
			screen_func_ptr = (lv_obj_t *(*)(void))(page_temp->page_func_addr);//获取当前页面创建函数地址
			temp_screen = screen_func_ptr();
			lv_scr_load_anim(temp_screen, LV_SCR_LOAD_ANIM_OUT_BOTTOM, 1000, 0, false);
			break;
		}
		case LV_DIR_LEFT://从左往右
		{
//			ry_get_next();
//			lv_scr_load_anim((lv_obj_t *)(page_temp->obj_addr), LV_SCR_LOAD_ANIM_OUT_LEFT, 1000, 0, false);
			break;
		}
		case LV_DIR_RIGHT://从右往左
		{
//			ry_get_prev();	
//			lv_scr_load_anim((lv_obj_t *)(page_temp->obj_addr), LV_SCR_LOAD_ANIM_OUT_RIGHT, 1000, 0, false);
			break;
		}
		default:
			break;
	}

}



/*--------------------------------------------------------------------------------------------------------
*创建4个不同颜色页面，并关联手势事件
*
--------------------------------------------------------------------------------------------------------*/
lv_obj_t * create_page1(void)
{
	delete_current_screen();//在新建之前，删除窗口
//	
//	lv_obj_t * one = lv_obj_create(NULL); //在默认屏上创建obj对象
//	lv_obj_set_style_bg_color(one,lv_color_hex(0xeeffcc), LV_STATE_DEFAULT); // obj背景色设成黄色
//	lv_obj_set_size(one, LV_HOR_RES, LV_VER_RES); // 设置到屏幕大小
//	
//	one_btn = lv_btn_create(one);
//	lv_obj_align(one_btn, LV_ALIGN_TOP_MID, 0, 20);

//	label = lv_label_create(one_btn);             // 创建label
//	lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);         // label居屏幕中心对齐
//	lv_label_set_text(label, "ONE");                    // label显示ONE

	lv_obj_t * one = my_lvgl_nebula();
	lv_obj_add_event_cb(one,ry_gesture_event,LV_EVENT_GESTURE,NULL);
	current_screen = one;
	return one;
}
lv_obj_t * create_page2(void)
{
	delete_current_screen();//在新建之前，删除窗口
	
//	lv_obj_t * two = lv_obj_create(NULL); // 创建新屏幕但未加载到显示
//	lv_obj_set_style_bg_color(two,lv_color_hex(0x00d8db), LV_STATE_DEFAULT); // 背影色设成蓝色
//	lv_obj_set_size(two, LV_HOR_RES, LV_VER_RES); //设置到屏幕大小

//	two_btn = lv_btn_create(two);
//	lv_obj_align(two_btn, LV_ALIGN_TOP_MID, 0, 20);

//	label2 = lv_label_create(two_btn);                       // 创建label
//	lv_obj_align(label2, LV_ALIGN_CENTER, 0, 0);         // 居中对齐
//	lv_label_set_text(label2, "TWO");                    // label上显示TWO
	
	lv_obj_t * two = my_lvgl_rgb();
	lv_obj_add_event_cb(two,ry_gesture_event,LV_EVENT_GESTURE,NULL);
		current_screen = two;
	return two;
}
lv_obj_t * create_page3(void)
{
	delete_current_screen();//在新建之前，删除窗口
	
	lv_obj_t *three = lv_obj_create(NULL); // 创建新屏幕但未加载到显示
	lv_obj_set_style_bg_color(three,lv_color_hex(0x00FF00), LV_STATE_DEFAULT); // 背影色设成蓝色
	lv_obj_set_size(three, LV_HOR_RES, LV_VER_RES); //设置到屏幕大小

	three_btn = lv_btn_create(three);
	lv_obj_align(three_btn, LV_ALIGN_TOP_MID, 0, 20);

	label3 = lv_label_create(three_btn);                       // 创建label
	lv_obj_align(label3, LV_ALIGN_CENTER, 0, 0);         // 居中对齐
	lv_label_set_text(label3, "THREE");                    // label上显示THREE

	lv_obj_add_event_cb(three,ry_gesture_event,LV_EVENT_GESTURE,NULL);
	current_screen = three;
	return three;	
}
lv_obj_t * create_page4(void)
{
		delete_current_screen();//在新建之前，删除窗口
	
	lv_obj_t *four = lv_obj_create(NULL); // 创建新屏幕但未加载到显示
	lv_obj_set_style_bg_color(four,lv_color_hex(0x0000FF), LV_STATE_DEFAULT); // 背影色设成蓝色
	lv_obj_set_size(four, LV_HOR_RES, LV_VER_RES); //设置到屏幕大小

	four_btn = lv_btn_create(four);
	lv_obj_align(four_btn, LV_ALIGN_TOP_MID, 0, 20);

	label4 = lv_label_create(four_btn);                       // 创建label
	lv_obj_align(label4, LV_ALIGN_CENTER, 0, 0);         // 居中对齐
	lv_label_set_text(label4, "FOUR");                    // label上显示FOUR

	lv_obj_add_event_cb(four,ry_gesture_event,LV_EVENT_GESTURE,NULL);
	current_screen = four;
	return four;	
}

void lv_example_anim_1(void)
{
	lv_scr_load(create_page1());//创建第一个页面，并显示

	ry_list_init();//初始化链表
	
	LV_LOG_USER("begin");
}
