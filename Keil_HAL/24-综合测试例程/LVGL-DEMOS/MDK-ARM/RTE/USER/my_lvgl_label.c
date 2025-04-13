#include "my_lvgl_label.h"
#include "lvgl.h"

void lv_ex_label_1(void)
{
		//LCD_SetColors(BLACK,RED);
	lv_obj_t* label2 = lv_label_create(lv_scr_act());
	//lv_label_set_recolor(label2,true);
lv_label_set_long_mode(label2,LV_LABEL_LONG_SCROLL_CIRCULAR);/*circular scroll*/
lv_obj_set_width(label2,240);
// Hello world ! Trisuborn.
lv_label_set_text(label2,"#ff Hello www.rymcu.com,LVGL STM32F103VET6 RT-thread os#");
lv_obj_align(label2,LV_ALIGN_CENTER,0,0);

}
