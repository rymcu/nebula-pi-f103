#include "my_lvgl_nebula.h"
#include "lvgl.h"

#include "my_lvgl_anim.h"

const lv_point_precise_t line2_points[] = {{60, 80}, {180, 80}};		  // 坐标点集合
#define LINE2_POINTS_NUM (sizeof(line2_points) / sizeof(line2_points[0])) // 坐标点的个数
void create_buttons(lv_obj_t * parent);

lv_obj_t *my_lvgl_nebula(void) // 入口函数
{
	/****************************************************************************************************************
	 *创建上半部灰色线条
	 *
	 ****************************************************************************************************************/
	lv_obj_t *scr = lv_screen_active();	   // 获取当前屏幕对象
	lv_obj_t *line2 = lv_line_create(scr); // 创建线条对象

	lv_obj_set_pos(line2, 0, 0);							   // 设置线条坐标
	lv_line_set_points(line2, line2_points, LINE2_POINTS_NUM); // 设置坐标点集合,同时也会在此内部计算出线条对象的大小

	static lv_style_t line2_style;											 // 创建线条样式
	lv_style_init(&line2_style);											 // 初始化线条样式
	lv_style_set_line_color(&line2_style, lv_palette_main(LV_PALETTE_GREY)); // 设置线条的颜色
	lv_style_set_line_width(&line2_style, 4);								 // 设置线条宽度
	lv_style_set_line_rounded(&line2_style, true);							 ////设置线条的末端为圆角
	lv_obj_add_style(line2, &line2_style, 0);								 // 关联样式
	/****************************************************************************************************************
	 *创建上半部灰色线条动画
	 *
	 ****************************************************************************************************************/

	lv_anim_t a;				// 创建动画变量
	lv_anim_init(&a);			// 初始化
	lv_anim_set_var(&a, line2); // 关联对象
	lv_anim_set_values(&a, -80, 0);
	lv_anim_set_duration(&a, 1000);
	lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
	lv_anim_set_path_cb(&a, lv_anim_path_overshoot);
	lv_anim_start(&a);

	/****************************************************************************************************************
	 *创建上半部标签：nebula-pi
	 *
	 ****************************************************************************************************************/
	lv_obj_t *label1 = lv_label_create(scr); // 创建标签
	lv_label_set_long_mode(label1, LV_LABEL_LONG_WRAP);
	lv_label_set_text(label1, "nebula-pi"); // 创建标签文字
	lv_obj_set_pos(label1, 67, 48);			// 设置标签位置

	static lv_style_t label1_style;											 // 标签样式
	lv_style_init(&label1_style);											 // 初始化标签样式
	lv_style_set_text_color(&label1_style, lv_palette_main(LV_PALETTE_RED)); // 标签文本颜色
	lv_style_set_text_font(&label1_style, &lv_font_montserrat_22);			 // 标签文本字体
	lv_obj_add_style(label1, &label1_style, 0);								 // 关联标签样式
	/****************************************************************************************************************
	 *创建上半部标签：nebula-pi动画
	 *
	 ****************************************************************************************************************/

	lv_anim_t b;				 // 创建动画变量
	lv_anim_init(&b);			 // 初始化
	lv_anim_set_var(&b, label1); // 关联对象
	lv_anim_set_values(&b, 0, 48);
	lv_anim_set_duration(&b, 1500);
	lv_anim_set_exec_cb(&b, (lv_anim_exec_xcb_t)lv_obj_set_y);
	lv_anim_set_path_cb(&b, lv_anim_path_overshoot); // lv_anim_path_overshoot
	lv_anim_start(&b);

	/****************************************************************************************************************
	 *创建中部部标签：powered by:LVGL 9.2
	 *
	 ****************************************************************************************************************/
	lv_obj_t *label_lvgl = lv_label_create(scr); // 创建标签
	lv_label_set_long_mode(label_lvgl, LV_LABEL_LONG_SCROLL_CIRCULAR);
	lv_obj_set_width(label_lvgl, 80);
	lv_label_set_text(label_lvgl, "powered by:LVGL 9.2     "); // 创建标签文字
	lv_obj_align(label_lvgl, LV_ALIGN_CENTER, 0, -60);

	static lv_style_t label_lvgl_style;											 // 标签样式
	lv_style_init(&label_lvgl_style);											 // 初始化标签样式
	lv_style_set_text_color(&label_lvgl_style, lv_palette_main(LV_PALETTE_RED)); // 标签文本颜色
	lv_style_set_text_font(&label_lvgl_style, &lv_font_montserrat_12);			 // 标签文本字体
	lv_obj_add_style(label_lvgl, &label_lvgl_style, 0);							 // 关联标签样式
	/****************************************************************************************************************
	 *设置背景色
	 *
	 ****************************************************************************************************************/
//	lv_obj_set_style_bg_color(scr, lv_color_hex(0xFFFFFF), 0); // 设置渐变起始色
//	lv_obj_set_style_bg_grad_color(scr,lv_color_hex(0xFFFFFF), 0); // 设置渐变结束色
//	lv_obj_set_style_bg_grad_dir(scr, LV_GRAD_DIR_VER, 0); // 设置为垂直渐变
//	lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0); // 设置背景不透明
	

lv_obj_set_style_bg_color(scr, lv_color_white(), 0); // 背景色为白色

	// 创建图案
	LV_IMG_DECLARE(logo);
	lv_obj_t * pattern = lv_img_create(scr);
	lv_img_set_src(pattern, &logo); // 将你的图案图片替换为实际图案
	lv_obj_align(pattern, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_size(pattern, 100, 100);	
	lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0); // 设置背景不透明
	
	/****************************************************************************************************************
	 *创建下半部标签：RYMCU
	 *
	 ****************************************************************************************************************/
	lv_obj_t *label2 = lv_label_create(scr); // 创建标签
	lv_label_set_long_mode(label2, LV_LABEL_LONG_WRAP);
	lv_label_set_text(label2, "RYMCU"); // 创建标签文字
	lv_obj_set_pos(label2, 80, 240);	// 设置标签位置

	static lv_style_t label2_style;												// 标签样式
	lv_style_init(&label2_style);												// 初始化标签样式
	lv_style_set_text_color(&label2_style, lv_palette_main(LV_PALETTE_ORANGE)); // 标签文本颜色
	lv_style_set_text_font(&label2_style, &lv_font_montserrat_22);				// 标签文本字体
	lv_obj_add_style(label2, &label2_style, 0);									// 关联标签样式
	/****************************************************************************************************************
	 *创建上半部标签：RYMCU动画
	 *
	 ****************************************************************************************************************/

	lv_anim_t c;				 // 创建动画变量
	lv_anim_init(&c);			 // 初始化
	lv_anim_set_var(&c, label2); // 关联对象
	lv_anim_set_values(&c, -100, 80);
	lv_anim_set_duration(&c, 1500);
	lv_anim_set_exec_cb(&c, (lv_anim_exec_xcb_t)lv_obj_set_x);
	lv_anim_set_path_cb(&c, lv_anim_path_overshoot); // lv_anim_path_overshoot
	lv_anim_start(&c);
	/****************************************************************************************************************
	 *创建上半部菜单标签：使用系统图标文字
	 ****************************************************************************************************************/
	lv_obj_t *label3 = lv_label_create(scr); // 创建标签
	lv_label_set_long_mode(label3, LV_LABEL_LONG_WRAP);
	lv_label_set_text(label3, LV_SYMBOL_WIFI " " LV_SYMBOL_BATTERY_3); // 创建标签文字
	lv_obj_set_pos(label3, 198, 5);									   // 设置标签位置

	lv_obj_t *label4 = lv_label_create(scr); // 创建标签
	lv_label_set_long_mode(label4, LV_LABEL_LONG_WRAP);
	lv_label_set_text(label4, LV_SYMBOL_CALL " " LV_SYMBOL_GPS); // 创建标签文字
	lv_obj_set_pos(label4, 8, 5);								 // 设置标签位置
	static lv_style_t label3_4_style;
	
	lv_style_init(&label3_4_style);//初始化标签样式
	lv_style_set_text_color(&label3_4_style, lv_palette_main(LV_PALETTE_DEEP_PURPLE));//标签文本颜色
	lv_style_set_text_font(&label3_4_style,&lv_font_montserrat_12);//标签文本字体
	lv_obj_add_style(label3, &label3_4_style, 0);//关联标签样式
	lv_obj_add_style(label4, &label3_4_style, 0);//关联标签样式
	/****************************************************************************************************************
	 *创建中间菜单标签：使用自定义图标文字
	 ****************************************************************************************************************/

	// create_heartbeat_image(scr);
	create_buttons(scr);
	return scr;
}
// 按钮事件处理函数
static void btn_event_handler(lv_event_t * e)
{
    lv_obj_t *btn = lv_event_get_target(e); // 获取事件源对象（按钮）
    const char *label = lv_label_get_text(lv_obj_get_child(btn, NULL)); // 获取按钮上的标签

    // 处理点击事件
    if (lv_strcmp(label, MY_ICON_RGB) == 0) {
        // 按钮 1 被点击
        load_my_page(2);
    } else if (lv_strcmp(label,MY_ICON_SET) == 0) {
        // 按钮 2 被点击
        load_my_page(3);
    } else if (lv_strcmp(label, MY_ICON_UART) == 0) {
        // 按钮 3 被点击
        load_my_page(4);
    } else if (lv_strcmp(label, MY_ICON_MUYU_S) == 0) {
        // 按钮 4 被点击
        load_my_page(5);
    }
}
// 创建按钮函数
void create_buttons(lv_obj_t * parent)
{
    const char *button_labels[] = 
		{
        MY_ICON_RGB, 
        MY_ICON_SET, 
        MY_ICON_UART, 
        MY_ICON_MUYU_S
    };

	static lv_style_t label5_style;											  // 标签样式
	lv_style_init(&label5_style);											  // 初始化标签样式
	lv_style_set_text_color(&label5_style, lv_palette_main(LV_PALETTE_BLUE)); // 标签文本颜色
	lv_style_set_text_font(&label5_style, &my_web_font_30);					  // 标签文本字体，自定义字体
		static lv_obj_t * btn;
		static lv_obj_t * label;
    for (int i = 0; i < 4; i++)
		{
        // 创建按钮
         btn = lv_btn_create(parent);
        lv_obj_set_size(btn, 52, 55); // 设置按钮大小
        lv_obj_align(btn, LV_ALIGN_CENTER,(i-1.5)*55,0); // 设置按钮垂直对齐

				 label= lv_label_create(btn);
				lv_obj_add_style(label, &label5_style, 0);								  // 关联标签样式
			lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
        // 创建标签并设置文本
        lv_label_set_text(label, button_labels[i]); // 设置标签文本
        lv_obj_center(label); // 使标签居中
				// 设置按钮的透明样式
				lv_obj_set_style_bg_color(btn, lv_color_hex(0xFFFFFF), 0);	   // 背景颜色
				lv_obj_set_style_bg_opa(btn, LV_OPA_TRANSP, 0);				   // 设置背景透明度
				lv_obj_set_style_border_color(btn, lv_color_hex(0xFFFFFF), 0); // 边框颜色
				lv_obj_set_style_border_opa(btn, LV_OPA_TRANSP, 0);			   // 边框透明
				lv_obj_set_style_outline_width(btn, 0, 0);					   // 去掉外轮廓
				lv_obj_set_style_shadow_width(btn, 0, 0);					   // 去掉阴影
        // 为按钮添加点击事件处理
        lv_obj_add_event_cb(btn, btn_event_handler, LV_EVENT_CLICKED, NULL);
    }
}
