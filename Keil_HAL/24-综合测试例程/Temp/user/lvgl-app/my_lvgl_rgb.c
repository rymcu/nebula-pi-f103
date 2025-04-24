#include "my_lvgl_rgb.h"
#include "lvgl.h"
#include "bsp_rgb.h"

static void slider_event_cb(lv_event_t * e);
static lv_obj_t * slider_r_label,*slider_g_label,*slider_b_label;
static lv_obj_t * slider_r,*slider_g,*slider_b,*led;
static lv_obj_t *my_obj_rgb;
 lv_obj_t* my_lvgl_rgb(void)//入口函数
{
	/****************************************************************************************************************
	*创建
	*
	****************************************************************************************************************/
	my_obj_rgb = lv_obj_create(NULL);
		//R
    slider_r = lv_slider_create(my_obj_rgb);
    lv_obj_center(slider_r);
    lv_obj_add_event_cb(slider_r, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
		lv_slider_set_range(slider_r,0,255);
    lv_obj_set_style_anim_duration(slider_r, 2000, 0);
	
    static lv_style_t style_r;
 
    lv_style_init( &style_r );
    lv_style_set_radius( &style_r, 20 );
    lv_style_set_bg_opa( &style_r, LV_OPA_COVER );
    lv_style_set_bg_color( &style_r, lv_color_hex(0x000000));
    lv_style_set_bg_grad_color( &style_r, lv_color_hex(0xff0000));
    lv_style_set_bg_grad_dir( &style_r, LV_GRAD_DIR_HOR );	
	
	
    lv_obj_add_style(slider_r, &style_r, LV_PART_KNOB);
	  lv_obj_add_style(slider_r, &style_r, LV_STATE_PRESSED);
		lv_obj_add_style(slider_r, &style_r, LV_PART_INDICATOR);
		lv_obj_add_style(slider_r, &style_r, LV_PART_MAIN);
		
    /*Create a label below the slider*/
    slider_r_label = lv_label_create(my_obj_rgb);
    lv_label_set_text(slider_r_label, "R:0");
		lv_obj_set_style_text_color(slider_r_label, lv_color_hex(0xff0000), 0);
		
    lv_obj_align_to(slider_r_label, slider_r, LV_ALIGN_OUT_TOP_LEFT, 10, 0);
		
		//G
    slider_g = lv_slider_create(my_obj_rgb);
    lv_obj_center(slider_g);
    lv_obj_add_event_cb(slider_g, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
		lv_slider_set_range(slider_g,0,255);
    lv_obj_set_style_anim_duration(slider_g, 2000, 0);
	
    static lv_style_t style_g;
 
    lv_style_init( &style_g );
    lv_style_set_radius( &style_g, 20 );
    lv_style_set_bg_opa( &style_g, LV_OPA_COVER );
    lv_style_set_bg_color( &style_g, lv_color_hex(0x000000));
    lv_style_set_bg_grad_color( &style_g, lv_color_hex(0x00ff00));
    lv_style_set_bg_grad_dir( &style_g, LV_GRAD_DIR_HOR );	
	
	
    lv_obj_add_style(slider_g, &style_g, LV_PART_KNOB);
	  lv_obj_add_style(slider_g, &style_g, LV_STATE_PRESSED);
		lv_obj_add_style(slider_g, &style_g, LV_PART_INDICATOR);
		lv_obj_add_style(slider_g, &style_g, LV_PART_MAIN);
		
    /*Create a label below the slider*/
    slider_g_label = lv_label_create(my_obj_rgb);
    lv_label_set_text(slider_g_label, "G:0");
		lv_obj_set_pos(slider_g,0,30);
		lv_obj_set_style_text_color(slider_g_label, lv_color_hex(0x00ff00), 0);
		
    lv_obj_align_to(slider_g_label, slider_g, LV_ALIGN_OUT_TOP_LEFT, 10, 0);	

		//b
    slider_b = lv_slider_create(my_obj_rgb);
    lv_obj_center(slider_b);
    lv_obj_add_event_cb(slider_b, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
		lv_slider_set_range(slider_b,0,255);
    lv_obj_set_style_anim_duration(slider_b, 2000, 0);
	
    static lv_style_t style_b;
 
    lv_style_init( &style_b );
    lv_style_set_radius( &style_b, 20 );
    lv_style_set_bg_opa( &style_b, LV_OPA_COVER );
    lv_style_set_bg_color( &style_b, lv_color_hex(0x000000));
    lv_style_set_bg_grad_color( &style_b, lv_color_hex(0x0000ff));
    lv_style_set_bg_grad_dir( &style_b, LV_GRAD_DIR_HOR );	
	
	
    lv_obj_add_style(slider_b, &style_b, LV_PART_KNOB);
	  lv_obj_add_style(slider_b, &style_b, LV_STATE_PRESSED);
		lv_obj_add_style(slider_b, &style_b, LV_PART_INDICATOR);
		lv_obj_add_style(slider_b, &style_b, LV_PART_MAIN);
		
    /*Create a label below the slider*/
    slider_b_label = lv_label_create(my_obj_rgb);
    lv_label_set_text(slider_b_label, "B:0");
		lv_obj_set_pos(slider_b,0,60);
		lv_obj_set_style_text_color(slider_b_label, lv_color_hex(0x0000ff), 0);
		
    lv_obj_align_to(slider_b_label, slider_b, LV_ALIGN_OUT_TOP_LEFT, 10, 0);	
		
		//LED
		led  = lv_led_create(my_obj_rgb);
    lv_obj_align(led, LV_ALIGN_CENTER, 0, -80);
    lv_led_set_brightness(led, 254);
		//lv_led_set_size(30);
		lv_obj_set_size(led,60,60);
    lv_led_set_color(led, lv_color_make(255,255,255));
		lv_led_on(led);
		
	/****************************************************************************************************************
	 *设置背景色
	 *
	 ****************************************************************************************************************/
	lv_obj_set_style_bg_color(my_obj_rgb, lv_color_hex(0x0), 0); // 设置渐变起始色
	lv_obj_set_style_bg_grad_color(my_obj_rgb,lv_color_hex(0x0), 0); // 设置渐变结束色
	lv_obj_set_style_bg_grad_dir(my_obj_rgb, LV_GRAD_DIR_VER, 0); // 设置为垂直渐变
	lv_obj_set_style_bg_opa(my_obj_rgb, LV_OPA_COVER, 0); // 设置背景不透明
		return my_obj_rgb;
}



                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        

static void slider_event_cb(lv_event_t * e)
{
    lv_obj_t * slider = lv_event_get_target(e);
    char buf[8];
	uint8_t r,g,b;
		if(slider == slider_r)
		{
			lv_snprintf(buf, sizeof(buf), "R:%d", (int)lv_slider_get_value(slider));
			lv_label_set_text(slider_r_label, buf);
			//lv_obj_align_to(slider_r_label, slider, LV_ALIGN_OUT_TOP_LEFT, 10, 0);
			
		}
		else if(slider == slider_g)
		{
			lv_snprintf(buf, sizeof(buf), "G:%d", (int)lv_slider_get_value(slider));
			lv_label_set_text(slider_g_label, buf);
			//lv_obj_align_to(slider_g_label, slider, LV_ALIGN_OUT_TOP_LEFT, 10, 0);			
		}
		else
		{
			lv_snprintf(buf, sizeof(buf), "B:%d", (int)lv_slider_get_value(slider));
			lv_label_set_text(slider_b_label, buf);
			
		}
		r = lv_slider_get_value(slider_r);
		g = lv_slider_get_value(slider_g);
		b = lv_slider_get_value(slider_b);
		
		lv_led_set_color(led, lv_color_make(r,g,b));//设置屏幕灯
		ry_set_rgb(r,g,b);//设置开发板RGB灯
}
