#include "lv_line_test.h"
#include "lvgl.h"
//#include "my_web_font.h"
//#include "key.h"
   LV_FONT_DECLARE(my_web_font_30)
    //LV_FONT_CUSTOM_DECLARE(my_web_font_28);
//const lv_font_t my_web_font_28;

     #define MY_ICON_1 "\xEE\xA4\x8E"
     #define MY_ICON_2 "\xEE\x99\xB7"
     #define MY_ICON_3 "\xEE\x98\x8F"
     #define MY_ICON_4 "\xEE\x98\xAF"
     #define MY_ICON_5 "\xEE\x9B\x9F"
     #define MY_ICON_6 "\xEE\x98\x8A"

const lv_point_t line_points[] = { {10, 20}, {70, 50}, {120, 10}, {140, 60}, {180, 10} };//坐标点集合
#define LINE_POINTS_NUM         (sizeof(line_points)/sizeof(line_points[0]))   //坐标点的个数
const lv_point_t line2_points[] = { {60, 80}, {180, 80} };//坐标点集合
#define LINE2_POINTS_NUM         (sizeof(line2_points)/sizeof(line2_points[0]))   //坐标点的个数
lv_obj_t * line1;

//例程入口
void lv_line_test_start()
{/*
	lv_obj_t * scr = lv_scr_act();//获取当前活跃的屏幕对象

	//1.创建自定义样式
	static lv_style_t line_style;
	lv_style_copy(&line_style, &lv_style_plain);
	line_style.line.color = LV_COLOR_RED;//线条的颜色
	line_style.line.width = 4;//线条的厚度
	line_style.line.rounded = 1;//线条的末端是否为圆角

	//2.创建线条对象
	line1 = lv_line_create(scr, NULL);//创建线条对象
	lv_obj_set_pos(line1,20,20);//设置坐标
	lv_line_set_auto_size(line1,true);//使能大小自适应,当然了,你也可以不调用,因为默认就是被使能了的
	lv_line_set_points(line1, line_points, LINE_POINTS_NUM);//设置坐标点集合,同时也会在此内部计算出线条对象的大小
	lv_line_set_style(line1, LV_LINE_STYLE_MAIN, &line_style);//设置样式
	*/
}

 lv_obj_t* line2,*label1,*label2,*label3,*label4,*label5;
 lv_obj_t* cont;
 lv_layout_t layout = LV_LAYOUT_CENTER;//起始的布局方式
lv_fit_t fit = LV_FIT_TIGHT;//起始的自适应方式
void my_line(void)
{
    //获取当前屏幕对象
    lv_obj_t* scr = lv_scr_act();
    //创建线条对象

    line2 =  lv_line_create(scr,NULL);

	static lv_style_t line2_style;
	lv_style_copy(&line2_style, &lv_style_plain);
	line2_style.line.color = LV_COLOR_SILVER;//线条的颜色
	line2_style.line.width = 4;//线条的厚度
	line2_style.line.rounded = 1;//线条的末端是否为圆角

    lv_obj_set_pos(line2,0,0);
    lv_line_set_auto_size(line2,true);//使能大小自适应,当然了,你也可以不调用,因为默认就是被使能了的
    lv_line_set_points(line2, line2_points, 2);//设置坐标点集合,同时也会在此内部计算出线条对象的大小
	lv_line_set_style(line2, LV_LINE_STYLE_MAIN, &line2_style);//设置样式




    static lv_style_t label_style,label3_style,label5_style;
	lv_style_copy(&label_style, &lv_style_plain);
	label_style.text.font =&lv_font_roboto_22;



    label1 = lv_label_create(scr,NULL);//创建标签
    lv_label_set_long_mode(label1,LV_LABEL_LONG_EXPAND);
    lv_obj_set_width(label1,160);
    lv_label_set_recolor(label1,true);

    lv_label_set_text(label1,"#ff0000 nebula-pi");

    lv_label_set_align(label1,LV_LABEL_ALIGN_CENTER);
    lv_label_set_style(label1,LV_LABEL_STYLE_MAIN,&label_style);
    lv_obj_set_pos(label1,75,48);
    lv_label_set_body_draw(label1,true);

     label2 = lv_label_create(scr,label1);//创建标签
     lv_label_set_text(label2,"#383937  RYMCU ");
     lv_obj_set_pos(label2,80,280);


     label3 = lv_label_create(scr,label1);//创建标签
     lv_style_copy(&label3_style, &lv_style_plain);
     label3_style.text.font =&lv_font_roboto_12;
    lv_label_set_style(label3,LV_LABEL_STYLE_MAIN,&label3_style);
     lv_label_set_text(label3,LV_SYMBOL_WIFI " " LV_SYMBOL_BATTERY_3);
     lv_obj_set_pos(label3,198,5);
     lv_label_set_align(label3,LV_LABEL_ALIGN_LEFT);

    label4 = lv_label_create(scr,label1);//创建标签
    lv_label_set_style(label4,LV_LABEL_STYLE_MAIN,&label3_style);
     lv_label_set_text(label4,LV_SYMBOL_CALL " " LV_SYMBOL_GPS);
     lv_obj_set_pos(label4,8,5);
     lv_label_set_align(label4,LV_LABEL_ALIGN_LEFT);


     //创建容器
     cont = lv_cont_create(scr,NULL);//创建容器
	lv_obj_set_pos(cont,20,90);//设置坐标
	lv_obj_set_size(cont,200,150);
	lv_cont_set_fit4(cont,LV_FIT_NONE,LV_FIT_NONE,LV_FIT_NONE,LV_FIT_NONE);
	//先设置容器布局方式
	lv_cont_set_layout(cont,layout);

	static lv_style_t cont_style;
	lv_style_copy(&cont_style,&lv_style_plain_color);//样式拷贝
	//设置纯红色的背景
	cont_style.body.main_color = LV_COLOR_SILVER;
	cont_style.body.grad_color = LV_COLOR_SILVER;
	//设置容器的4个内边距
	cont_style.body.padding.top = 10;
	cont_style.body.padding.left = 10;
	cont_style.body.padding.right = 10;
	cont_style.body.padding.bottom = 10;
	cont_style.body.padding.inner = 10;//设置容器中子对象之间的间隙

	lv_cont_set_style(cont,LV_CONT_STYLE_MAIN,&cont_style);//给容器设置样式

    label5 = lv_label_create(cont,label1);//创建标签
     lv_style_copy(&label5_style, &lv_style_plain);
     label5_style.text.font = &my_web_font_30;
     label5_style.text.color = LV_COLOR_BLUE;

     label5_style.body.main_color = LV_COLOR_TRANSP;
     label5_style.body.grad_color = LV_COLOR_TRANSP;

    lv_label_set_style(label5,LV_LABEL_STYLE_MAIN,&label5_style);
     lv_label_set_text(label5,MY_ICON_1 "  " MY_ICON_2 "  " MY_ICON_3 "  " MY_ICON_4 "  " MY_ICON_5 "  " MY_ICON_6);
     lv_obj_set_pos(label5,25,150);
     lv_label_set_align(label5,LV_LABEL_ALIGN_LEFT);


}

//按键处理


