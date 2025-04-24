#include "my_lvgl_temperature.h"
#include "adc.h"
#include "stdio.h"

static lv_obj_t * temperature_label; // 温度显示标签
static lv_obj_t * temperature_screen;
static float current_temperature; // 当前温度

// 样式初始化


// 创建表格以显示STM32开发板信息
void create_product_info_table(lv_obj_t *parent) {
    // 创建表格对象
    lv_obj_t *table = lv_table_create(parent);
    lv_obj_set_size(table, 240, 320); // 设置表格大小以适应屏幕
    lv_obj_align(table, LV_ALIGN_CENTER, 0, 0); // 将表格居中

    // 设置行和列的数量
    lv_table_set_col_cnt(table, 2);
    lv_table_set_row_cnt(table, 4); // 4 行用于显示规格

    // 设置表头
    lv_table_set_cell_value(table, 0, 0, "Specification");
    lv_table_set_cell_value(table, 0, 1, "Details");

    // 填充STM32开发板规格信息
    lv_table_set_cell_value(table, 1, 0, "Processor");
    lv_table_set_cell_value(table, 1, 1, "STM32F103VET6-COTEX-M3-RYMCU-nebula-pi");

    lv_table_set_cell_value(table, 2, 0, "Memory");
    lv_table_set_cell_value(table, 2, 1, "1024KB Flash");

    lv_table_set_cell_value(table, 3, 0, "I/O");
    lv_table_set_cell_value(table, 3, 1, "37 GPIOs");

    // 设置每列的宽度（可选）
    lv_table_set_col_width(table, 0, 120); // 规格列宽
    lv_table_set_col_width(table, 1, 120); // 详细信息列宽

    // 设置单元格样式
    //style_table_cells(table);
}

// 获取温度值
float  my_get_cpu_temperature(void)
{
    uint32_t Temp;//温度采样分层值
    float Vsense = 0.0;//温度采样电压值
    float Temperature = 0.0;//温度值
    //数据手册温度转换公式：T = ((V25-Vsense)/Avg_Slope) + 25
    float V25 = 1.43;//查阅手册获得
    float Avg_Slope = 0.0043;//4.3mV/摄氏度
    //LV_LOG_USER("\r\n\r\n------------------MCU内部温度传感器测试------------------\r\n\r\n");
    //step1 启动ADC
    HAL_ADC_Start(&hadc1);
    //step2 温度采集转换
    HAL_ADC_PollForConversion(&hadc1,5);
    //step3 转换计算
    Temp = HAL_ADC_GetValue(&hadc1);//获取采样值分层值
    Vsense = (Temp/4095.0)*3.3;//采样精度12bit,最大分层值4096
    Temperature = ((1.43-Vsense)/0.0043) + 25;//按公式计算温度值

    //LV_LOG_USER("温度分层值：%d\r\n温度电压值：%f\r\n温度采样值：%0.3f\r\n",Temp,Vsense,Temperature);
	return Temperature;
}
void my_timer_callback(lv_timer_t * timer)
{
	char temp_str[20]= "25.1";
	current_temperature = my_get_cpu_temperature();
	sprintf(temp_str,"Temp: %0.1f °C", current_temperature); // 格式化字符串
	lv_label_set_text(temperature_label, temp_str); // 更新标签文本
}	

lv_obj_t* my_lvgl_temperature(void) 
	{
		 lv_timer_create(my_timer_callback, 1000, NULL); // 每1000ms调用一次
    temperature_screen = lv_obj_create(NULL); // 创建新屏幕
    lv_obj_set_style_bg_color(temperature_screen, lv_color_hex(0xFFFFFF), 0); // 设置背景颜色

    temperature_label = lv_label_create(temperature_screen); // 创建标签
    lv_obj_align(temperature_label, LV_ALIGN_CENTER, 0, -20); // 对齐标签
		create_product_info_table(temperature_screen);
	return temperature_screen;
}
