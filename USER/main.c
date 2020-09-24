#include "sys.h"
#include "delay.h"  
#include "usart.h"   
#include "led.h"
#include "lcd.h"
#include "key.h"  
#include "beep.h"
#include "timer.h"
#include "sram.h"
#include "touch.h" 
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
//#include "lv_kb_test.h"
#include "lv_calendar.h"
#include "rtc.h"


	
int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	usmart_dev.init(84); 	//初始化USMART
	My_RTC_Init();		 		//初始化RTC
	LED_Init();					//初始化LED 
 	LCD_Init();					//LCD初始化 
	KEY_Init(); 				//按键初始化  
	BEEP_Init();				//蜂鸣器初始化
	RTC_Set_WakeUp(RTC_WakeUpClock_CK_SPRE_16bits,0);		//配置WAKE UP中断,1秒钟中断一次

	TIM3_Int_Init(999,83);	//定时器初始化(1ms中断),用于给lvgl提供1ms的心跳节拍
	FSMC_SRAM_Init();		//初始化外部sram
	tp_dev.init();			//触摸屏初始化

	lv_init();						//lvgl系统初始化
	lv_port_disp_init();	//lvgl显示接口初始化,放在lv_init()的后面
	lv_port_indev_init();	//lvgl输入接口初始化,放在lv_init()的后面
	
	
	lv_calendar_start();
	
	
	
	
	while(1)
	{
		get_time();
		tp_dev.scan(0);//触摸扫描
		lv_task_handler();//lvgl事务处理 
		
	}
}
