#include "my_lvgl_temperature.h"
#include "adc.h"
#include "stdio.h"

static lv_obj_t * temperature_label; // ÎÂ¶ÈÏÔÊ¾±êÇ©
static lv_obj_t * temperature_screen;
static float current_temperature; // µ±Ç°ÎÂ¶È

// ÑùÊ½³õÊ¼»¯


// ´´½¨±í¸ñÒÔÏÔÊ¾STM32¿ª·¢°åĞÅÏ¢
void create_product_info_table(lv_obj_t *parent) {
    // ´´½¨±í¸ñ¶ÔÏó
    lv_obj_t *table = lv_table_create(parent);
    lv_obj_set_size(table, 240, 320); // ÉèÖÃ±í¸ñ´óĞ¡ÒÔÊÊÓ¦ÆÁÄ»
    lv_obj_align(table, LV_ALIGN_CENTER, 0, 0); // ½«±í¸ñ¾ÓÖĞ

    // ÉèÖÃĞĞºÍÁĞµÄÊıÁ¿
    lv_table_set_col_cnt(table, 2);
    lv_table_set_row_cnt(table, 4); // 4 ĞĞÓÃÓÚÏÔÊ¾¹æ¸ñ

    // ÉèÖÃ±íÍ·
    lv_table_set_cell_value(table, 0, 0, "Specification");
    lv_table_set_cell_value(table, 0, 1, "Details");

    // Ìî³äSTM32¿ª·¢°å¹æ¸ñĞÅÏ¢
    lv_table_set_cell_value(table, 1, 0, "Processor");
    lv_table_set_cell_value(table, 1, 1, "STM32F103VE");

    lv_table_set_cell_value(table, 2, 0, "Memory");
    lv_table_set_cell_value(table, 2, 1, "512KB Flash");

    lv_table_set_cell_value(table, 3, 0, "I/O");
    lv_table_set_cell_value(table, 3, 1, "37 GPIOs");
		
    lv_table_set_cell_value(table, 4, 0, "Compaly");
    lv_table_set_cell_value(table, 4, 1, "37 GPIOs");
    // ÉèÖÃÃ¿ÁĞµÄ¿í¶È£¨¿ÉÑ¡£©
    lv_table_set_col_width(table, 0, 120); // ¹æ¸ñÁĞ¿í
    lv_table_set_col_width(table, 1, 120); // ÏêÏ¸ĞÅÏ¢ÁĞ¿í

    // åˆ›å»ºä¸€ä¸ªæ–°çš„æ ·å¼
    // è®¾ç½®å­—ä½“é¢œè‰²ä¸ºè“è‰²
    // è®¾ç½®è¡¨æ ¼èƒŒæ™¯é¢œè‰²ä¸ºæµ…ç°è‰²
		  // è®¾ç½®æ–‡æœ¬é¢œè‰²ä¸ºæ·±ç´«è‰²
    // ÉèÖÃµ¥Ôª¸ñÑùÊ½
    //style_table_cells(table);

		lv_obj_set_style_bg_color(table, lv_color_hex(0x0), 0); 
	lv_obj_set_style_bg_grad_color(table,lv_color_hex(0x0), 0); 
	lv_obj_set_style_bg_grad_dir(table, LV_GRAD_DIR_VER, 0); 
	lv_obj_set_style_bg_opa(table, LV_OPA_COVER, 0); 
}

// »ñÈ¡ÎÂ¶ÈÖµ
float  my_get_cpu_temperature(void)
{
    uint32_t Temp;//ÎÂ¶È²ÉÑù·Ö²ãÖµ
    float Vsense = 0.0;//ÎÂ¶È²ÉÑùµçÑ¹Öµ
    float Temperature = 0.0;//ÎÂ¶ÈÖµ
    //Êı¾İÊÖ²áÎÂ¶È×ª»»¹«Ê½£ºT = ((V25-Vsense)/Avg_Slope) + 25
    float V25 = 1.43;//²éÔÄÊÖ²á»ñµÃ
		 //uint16_t V25 = *(__IO uint16_t*)0x1FFFF7B8;  // 25¡æÊ±µÄµçÑ¹Ğ£×¼Öµ
    float Avg_Slope = 0.0043;//4.3mV/ÉãÊÏ¶È
		//int16_t Avg_Slope = *(__IO int16_t*)0x1FFFF7C2;  // ÎÂ¶ÈĞ±ÂÊĞ£×¼Öµ
    //LV_LOG_USER("\r\n\r\n------------------MCUÄÚ²¿ÎÂ¶È´«¸ĞÆ÷²âÊÔ------------------\r\n\r\n");
    //step1 Æô¶¯ADC
    HAL_ADC_Start(&hadc1);
    //step2 ÎÂ¶È²É¼¯×ª»»
    HAL_ADC_PollForConversion(&hadc1,10);
    //step3 ×ª»»¼ÆËã
    Temp = HAL_ADC_GetValue(&hadc1);//»ñÈ¡²ÉÑùÖµ·Ö²ãÖµ
   // Vsense = (Temp/4095.0)*3.3;//²ÉÑù¾«¶È12bit,×î´ó·Ö²ãÖµ4096
		Vsense = (float)Temp * 3.3f / 4096.0f;
    Temperature = ((V25-Vsense)/Avg_Slope) + 25;//°´¹«Ê½¼ÆËãÎÂ¶ÈÖµ

    //LV_LOG_USER("ÎÂ¶È·Ö²ãÖµ£º%d\r\nÎÂ¶ÈµçÑ¹Öµ£º%f\r\nÎÂ¶È²ÉÑùÖµ£º%0.3f\r\n",Temp,Vsense,Temperature);
	return Temperature;
}
char temp_str[20]= "25.1";
void my_timer_callback(lv_timer_t * timer)
{
	//char temp_str[20]= "25.1";
	current_temperature = my_get_cpu_temperature();
	
	sprintf(temp_str,"%0.1f", current_temperature); // ¸ñÊ½»¯×Ö·û´®
	uint16_t tt = (uint16_t)current_temperature;
	lv_label_set_text(temperature_label, temp_str); // ¸üĞÂ±êÇ©ÎÄ±¾
}	

void draw_thermometer(lv_obj_t * parent)
{

    // ç»˜åˆ¶æ¸©åº¦è®¡çš„ä¸»ä½“
    lv_obj_t * body = lv_obj_create(parent);
    lv_obj_set_size(body, 10, 60);
    //lv_obj_center_x(body);
    lv_obj_align_to(body, parent, LV_ALIGN_BOTTOM_LEFT, 40,-40);
    lv_obj_set_style_bg_color(body, lv_color_white(), 0);
    lv_obj_set_style_radius(body, LV_RADIUS_CIRCLE, 0);

    // ç»˜åˆ¶æ¸©åº¦è®¡çš„åº•éƒ¨åœ†å½¢
    lv_obj_t * bottom = lv_obj_create(parent);
    lv_obj_set_size(bottom, 30, 30);
    lv_obj_align_to(bottom, body, LV_ALIGN_BOTTOM_MID, 0, 10);
    lv_obj_set_style_bg_color(bottom, lv_color_white(), 0);
    lv_obj_set_style_radius(bottom, LV_RADIUS_CIRCLE, 0);

    // ç»˜åˆ¶æ¸©åº¦è®¡å†…çš„æ°´é“¶æŸ±
    lv_obj_t * mercury = lv_obj_create(body);
    lv_obj_set_size(mercury, 6, 40);
    lv_obj_align_to(mercury, body, LV_ALIGN_BOTTOM_MID, 0, -2);
    lv_obj_set_style_bg_color(mercury, lv_palette_main(LV_PALETTE_RED), 0);
    lv_obj_set_style_radius(mercury, LV_RADIUS_CIRCLE, 0);
}
lv_obj_t* my_lvgl_temperature(void) 
	{
		 lv_timer_create(my_timer_callback, 1000, NULL); 
    temperature_screen = lv_obj_create(NULL); 
    lv_obj_set_style_bg_color(temperature_screen, lv_color_hex(0x0), 0); 
		
		create_product_info_table(temperature_screen);
    temperature_label = lv_label_create(temperature_screen); 
    lv_obj_align(temperature_label, LV_ALIGN_CENTER, 0, 100); 
		
		static lv_style_t temp_style;											  
	lv_style_init(&temp_style);												 
	lv_style_set_text_color(&temp_style, lv_palette_main(LV_PALETTE_ORANGE)); 
	lv_style_set_text_font(&temp_style, &lv_font_montserrat_48);			 
	lv_obj_add_style(temperature_label, &temp_style, 0);
		draw_thermometer(temperature_screen);
		
			lv_obj_t *label2 = lv_label_create(temperature_screen); // Ô´İ¨ÒªÇ©
	lv_label_set_long_mode(label2, LV_LABEL_LONG_WRAP);
	lv_label_set_text(label2, "Core temperature"); // Ô´İ¨ÒªÇ©Ï„Ø–
	lv_obj_set_pos(label2, 60, 215);	// Ê¨×ƒÒªÇ©Î»×ƒ

	static lv_style_t label2_style;												// ÒªÇ©Ò¹Ê½
	lv_style_init(&label2_style);												// ÔµÊ¼Û¯ÒªÇ©Ò¹Ê½
	lv_style_set_text_color(&label2_style, lv_palette_main(LV_PALETTE_ORANGE)); // ÒªÇ©Ï„Ñ¾Ò•É«
	lv_style_set_text_font(&label2_style, &lv_font_montserrat_12);				// ÒªÇ©Ï„Ñ¾Ø–Í¥
	lv_obj_add_style(label2, &label2_style, 0);		
	return temperature_screen;
}
