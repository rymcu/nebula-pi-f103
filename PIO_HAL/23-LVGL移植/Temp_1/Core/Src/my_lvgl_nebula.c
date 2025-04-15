#include "my_lvgl_nebula.h"
#include "lvgl.h"
const  lv_point_precise_t  line2_points[] = { {60, 80}, {180, 80} };//坐标点集合
#define LINE2_POINTS_NUM         (sizeof(line2_points)/sizeof(line2_points[0]))   //坐标点的个数

static lv_style_t line2_style;//创建线条样式
static lv_style_t label1_style;//标签样式
static lv_style_t label2_style;//标签样式
static lv_style_t label3_4_style;//标签样式
static lv_style_t label5_style;//标签样式

 lv_obj_t* my_lvgl_nebula(void)//入口函数
{
	/****************************************************************************************************************
	*创建上半部灰色线条
	*
	****************************************************************************************************************/
  lv_obj_t* scr = lv_screen_active();//获取当前屏幕对象
   lv_obj_t* line2 =  lv_line_create(scr);//创建线条对象
	
	lv_obj_set_pos(line2,0,0);//设置线条坐标
	lv_line_set_points(line2, line2_points, LINE2_POINTS_NUM);//设置坐标点集合,同时也会在此内部计算出线条对象的大小
	

	lv_style_init(&line2_style);//初始化线条样式
	lv_style_set_line_color(&line2_style,lv_palette_main(LV_PALETTE_GREY));//设置线条的颜色
	lv_style_set_line_width(&line2_style, 4);//设置线条宽度
	lv_style_set_line_rounded(&line2_style, true);////设置线条的末端为圆角
  lv_obj_add_style(line2, &line2_style, 0);//关联样式
	/****************************************************************************************************************
	*创建上半部灰色线条动画
	*
	****************************************************************************************************************/

	lv_anim_t a;//创建动画变量
	lv_anim_init(&a);//初始化
	lv_anim_set_var(&a, line2);//关联对象
	lv_anim_set_values(&a, -80, 0);
	lv_anim_set_duration(&a, 1000);
	lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
	lv_anim_set_path_cb(&a, lv_anim_path_overshoot);
	lv_anim_start(&a);

	/****************************************************************************************************************
	*创建上半部标签：nebula-pi
	*
	****************************************************************************************************************/
  lv_obj_t * label1 = lv_label_create(scr);//创建标签
  lv_label_set_long_mode(label1, LV_LABEL_LONG_WRAP);
  lv_label_set_text(label1,"nebula-pi");//创建标签文字
	lv_obj_set_pos(label1,67,48);//设置标签位置
	

	lv_style_init(&label1_style);//初始化标签样式
	lv_style_set_text_color(&label1_style, lv_palette_main(LV_PALETTE_RED));//标签文本颜色
	lv_style_set_text_font(&label1_style,&lv_font_montserrat_22);//标签文本字体
	lv_obj_add_style(label1, &label1_style, 0);//关联标签样式
	/****************************************************************************************************************
	*创建上半部标签：nebula-pi动画
	*
	****************************************************************************************************************/

	lv_anim_t b;//创建动画变量
	lv_anim_init(&b);//初始化
	lv_anim_set_var(&b, label1);//关联对象
	lv_anim_set_values(&b, 0, 48);
	lv_anim_set_duration(&b, 1500);
	lv_anim_set_exec_cb(&b, (lv_anim_exec_xcb_t)lv_obj_set_y);
	lv_anim_set_path_cb(&b, lv_anim_path_overshoot);// lv_anim_path_overshoot
	lv_anim_start(&b);
	
	/****************************************************************************************************************
	*创建下半部标签：RYMCU
	*
	****************************************************************************************************************/		
  lv_obj_t * label2 = lv_label_create(scr);//创建标签
  lv_label_set_long_mode(label2, LV_LABEL_LONG_WRAP);
  lv_label_set_text(label2,"RYMCU");//创建标签文字
	lv_obj_set_pos(label2,80,280);//设置标签位置


	lv_style_init(&label2_style);//初始化标签样式
	lv_style_set_text_color(&label2_style, lv_palette_main(LV_PALETTE_ORANGE));//标签文本颜色
	lv_style_set_text_font(&label2_style,&lv_font_montserrat_22);//标签文本字体
	lv_obj_add_style(label2, &label2_style, 0);//关联标签样式
	/****************************************************************************************************************
	*创建上半部标签：RYMCU动画
	*
	****************************************************************************************************************/

	lv_anim_t c;//创建动画变量
	lv_anim_init(&c);//初始化
	lv_anim_set_var(&c, label2);//关联对象
	lv_anim_set_values(&c, -100, 80);
	lv_anim_set_duration(&c, 1500);
	lv_anim_set_exec_cb(&c, (lv_anim_exec_xcb_t)lv_obj_set_x);
	lv_anim_set_path_cb(&c, lv_anim_path_overshoot);// lv_anim_path_overshoot
	lv_anim_start(&c);
	/****************************************************************************************************************
	*创建上半部菜单标签：使用系统图标文字
	****************************************************************************************************************/	
	lv_obj_t * label3 = lv_label_create(scr);//创建标签
  lv_label_set_long_mode(label3, LV_LABEL_LONG_WRAP);
  lv_label_set_text(label3,LV_SYMBOL_WIFI " " LV_SYMBOL_BATTERY_3);//创建标签文字
	lv_obj_set_pos(label3,198,5);//设置标签位置
	
	lv_obj_t * label4 = lv_label_create(scr);//创建标签
  lv_label_set_long_mode(label4, LV_LABEL_LONG_WRAP);
  lv_label_set_text(label4,LV_SYMBOL_CALL " " LV_SYMBOL_GPS);//创建标签文字
	lv_obj_set_pos(label4,8,5);//设置标签位置
	
	
	lv_style_init(&label3_4_style);//初始化标签样式
	lv_style_set_text_color(&label3_4_style, lv_palette_main(LV_PALETTE_DEEP_PURPLE));//标签文本颜色
	lv_style_set_text_font(&label3_4_style,&lv_font_montserrat_12);//标签文本字体
	lv_obj_add_style(label3, &label3_4_style, 0);//关联标签样式
	lv_obj_add_style(label4, &label3_4_style, 0);//关联标签样式


	/****************************************************************************************************************
	*创建中间菜单标签：使用自定义图标文字
	****************************************************************************************************************/	
	lv_obj_t * label5 = lv_label_create(scr);//创建标签
  lv_label_set_long_mode(label5, LV_LABEL_LONG_WRAP);
  lv_label_set_text(label5,MY_ICON_1 MY_ICON_2 MY_ICON_3 MY_ICON_4 MY_ICON_5 MY_ICON_6);//创建标签文字
	lv_obj_set_pos(label5,25,150);//设置标签位置


	lv_style_init(&label5_style);//初始化标签样式
	lv_style_set_text_color(&label5_style, lv_palette_main(LV_PALETTE_BLUE));//标签文本颜色
	lv_style_set_text_font(&label5_style,&my_web_font_30);//标签文本字体，自定义字体
	lv_obj_add_style(label5, &label5_style, 0);//关联标签样式
	
	return scr;

}

