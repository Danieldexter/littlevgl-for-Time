#include "lv_calendar.h"
#include "lvgl.h"
#include "key.h"
#include "sys.h"
#include "delay.h"  
#include "usart.h"  
#include "led.h"
#include "lcd.h"
#include "usmart.h"
#include "rtc.h"
#include "delay.h"
#include <string.h>

/*函数声明*/
void get_time(void);
void event_handler(lv_obj_t * obj,lv_event_t event);
static void btn_event_cb(lv_obj_t * obj,lv_event_t event);
void lv_calendar_roller_set(void);
void lv_calendar_start(void);


/*对象声明*/
lv_obj_t * label_hour;
lv_obj_t * label_min;
lv_obj_t * label_second;
lv_obj_t * scr;
lv_obj_t * scr1;
lv_obj_t * roller_hour;
lv_obj_t * roller_min;
lv_obj_t * roller_second;

/*RTC存储数组声明*/
uint8_t tbuf[40];
uint8_t tbuf_Hour[20];
uint8_t tbuf_min[20];
uint8_t tbuf_second[20];

char  set_hour[2];
char  set_min[2];
char  set_second[2];
int x;
int y;
int z;
u8 xa=0;
u8 ya=0;
u8 za=0;

lv_style_t bg_style;
lv_style_t sel_style;

int tt=0;

lv_style_t my_style_btn_release;// 按纽释放状态下的样式
lv_style_t my_style_btn_press;// 按纽按下的样式
lv_obj_t * btn1,* btn2,* btn3;
lv_obj_t * btn1_label,* btn3_label;

void lv_calendar_roller_set(void)
{
	lv_obj_set_click(scr1,false);		// 使能触屏功能
	
	//1.创建样式
	//1.1 创建背景样式
	lv_style_copy(&bg_style,&lv_style_pretty);
	bg_style.body.main_color = LV_COLOR_WHITE;//纯白背景
	bg_style.body.grad_color = bg_style.body.main_color;
	bg_style.body.border.width = 1;//板框宽度
	bg_style.body.border.color = LV_COLOR_MAKE(0xAA,0xAA,0xAA);
	//LV_COLOR_MAKE(0x30,0x30,0x30);//边框颜色
	bg_style.body.padding.left = 10;//设置左侧得内边距
	bg_style.text.color = LV_COLOR_BLACK;//文本颜色
	bg_style.body.shadow.color = bg_style.body.border.color;//阴影颜色
	bg_style.body.shadow.width = 4;//阴影宽度
	
	//1.2 创建选择项被选中时的样式
	lv_style_copy(&sel_style,&lv_style_pretty);
	sel_style.body.main_color = LV_COLOR_MAKE(0x5F,0xB8,0x78);//浅绿色背景
	sel_style.body.grad_color = sel_style.body.main_color;
	sel_style.text.color = LV_COLOR_WHITE;//文本为白色
	
	//2.创建滚轮对象
	roller_hour = lv_roller_create(scr1,NULL);
	lv_roller_set_options(roller_hour,
						"00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23"
						,LV_ROLLER_MODE_INIFINITE);//设置所有的选项值，循环滚动模式
	lv_roller_set_selected(roller_hour,0,LV_ANIM_ON);//设置默认选中值为 00
	lv_roller_set_fix_width(roller_hour,30);//设置固定宽度
	lv_roller_set_visible_row_count(roller_hour,3);//设置可见的行数
	lv_roller_set_style(roller_hour,LV_ROLLER_STYLE_BG,&bg_style);//设置背景样式
	lv_roller_set_style(roller_hour,LV_ROLLER_STYLE_SEL,&sel_style);//设置背景样式
	//lv_obj_set_event_cb(roller1,event_handler);//注册事件的回调函数
	lv_obj_align(roller_hour,NULL,LV_ALIGN_IN_LEFT_MID,50,0);//设置与屏幕对齐
	
	roller_min = lv_roller_create(scr1,roller_hour);
	lv_roller_set_options(roller_min,
						"00\n01\n02\n03\n04\n05\n06\n07\n08\n09"
						"\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n"
						"21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n"
						"31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n"
						"41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n"
						"51\n52\n52\n54\n55\n56\n57\n58\n59"
						,LV_ROLLER_MODE_INIFINITE);//设置所有的选项值，循环滚动模式
	lv_roller_set_selected(roller_min,0,LV_ANIM_ON);//设置默认选中值为 00
	lv_roller_set_fix_width(roller_min,30);//设置固定宽度
	lv_roller_set_visible_row_count(roller_min,3);//设置可见的行数
	lv_roller_set_style(roller_min,LV_ROLLER_STYLE_BG,&bg_style);//设置背景样式
	lv_roller_set_style(roller_min,LV_ROLLER_STYLE_SEL,&sel_style);//设置背景样式
	//lv_obj_set_event_cb(roller_min,event_handler);//注册事件的回调函数
	lv_obj_align(roller_min,NULL,LV_ALIGN_CENTER,0,0);//设置与屏幕对齐

	roller_second = lv_roller_create(scr1,roller_hour);
	lv_roller_set_options(roller_second,
						"00\n01\n02\n03\n04\n05\n06\n07\n08\n09"
						"\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n"
						"21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n"
						"31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n"
						"41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n"
						"51\n52\n52\n54\n55\n56\n57\n58\n59"
						,LV_ROLLER_MODE_INIFINITE);//设置所有的选项值，循环滚动模式
	lv_roller_set_selected(roller_second,0,LV_ANIM_ON);//设置默认选中值为 00
	lv_roller_set_fix_width(roller_second,30);//设置固定宽度
	lv_roller_set_visible_row_count(roller_second,3);//设置可见的行数
	lv_roller_set_style(roller_second,LV_ROLLER_STYLE_BG,&bg_style);//设置背景样式
	lv_roller_set_style(roller_second,LV_ROLLER_STYLE_SEL,&sel_style);//设置背景样式
	//lv_obj_set_event_cb(roller_second,event_handler);//注册事件的回调函数
	lv_obj_align(roller_second,NULL,LV_ALIGN_IN_RIGHT_MID,-50,0);//设置与屏幕对齐
	
	// 1.先创建2种状态下按钮样式
	// 1.1 释放状态下的样式
	lv_style_copy(&my_style_btn_release,&lv_style_plain_color);
	// 设置纯色的背景
	my_style_btn_release.body.main_color = LV_COLOR_MAKE(0x1E,0x9F,0xFF);
	my_style_btn_release.body.grad_color = my_style_btn_release.body.main_color;
	my_style_btn_release.body.opa = LV_OPA_COVER;// 设置背景色完全不透明
	my_style_btn_release.body.radius = LV_RADIUS_CIRCLE;// 绘制圆角按钮 
	my_style_btn_release.body.shadow.color = LV_COLOR_MAKE(0x1E,0x9F,0xFF);
	my_style_btn_release.body.shadow.type = LV_SHADOW_FULL;//设置四边都有阴影
	my_style_btn_release.body.shadow.width = 3;//设置阴影宽度
	my_style_btn_release.text.color = LV_COLOR_WHITE;
	my_style_btn_release.body.padding.left = 10;//设置左内边距
	my_style_btn_release.body.padding.right = 10;//设置右内边距
	
	//1.2 按下状态下的样式
	lv_style_copy(&my_style_btn_press,&lv_style_plain_color);
	my_style_btn_press.body.opa = LV_OPA_0;//设置背景色透明
	my_style_btn_press.body.radius = LV_RADIUS_CIRCLE;//绘制圆角按钮
	//设置边框的颜色
	my_style_btn_press.body.border.color = LV_COLOR_MAKE(0xC9,0xC9,0xC9);
	my_style_btn_press.body.border.part = LV_BORDER_FULL;//四条边框都绘制
	my_style_btn_press.body.border.width = 2;//设置边框的宽度
	my_style_btn_press.body.border.opa = LV_OPA_COVER;//设置边框完全不透明
	my_style_btn_press.text.color = LV_COLOR_BLACK;
	my_style_btn_press.body.padding.left = 10;//设置左内边距
	my_style_btn_press.body.padding.right = 10;//设置右内边距
	
	btn1 = lv_btn_create(scr1, NULL);
	lv_obj_set_pos(btn1,150,250);//设置坐标
	lv_btn_set_ink_in_time(btn1, 3000);//入场动画时长
	lv_btn_set_ink_wait_time(btn1, 1000);//维持等待时长

	lv_btn_set_ink_out_time(btn1, 600);
	btn1_label = lv_label_create(btn1,NULL);
	lv_label_set_text(btn1_label,"OK");
	
	lv_obj_set_event_cb(btn1,btn_event_cb);
	
}

void get_time(void)
{	
		if(label_hour != NULL)
		{
		RTC_TimeTypeDef RTC_TimeStruct;
		tt++;
		if((tt%10)==0)	//每100ms更新一次显示数据
		{	
			RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct); // 获取RTC里面得时间	
			//sprintf((char*)tbuf,"Time:%02d:%02d:%02d",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds); 
			sprintf((char*)tbuf_Hour,"%02d",RTC_TimeStruct.RTC_Hours); // 将RTC里面的时间的-时-放入tbuf_hour中 
			lv_label_set_text(label_hour,(char*)tbuf_Hour);
			sprintf((char*)tbuf_min,"%02d",RTC_TimeStruct.RTC_Minutes); // 将RTC里面的时间的-分-放入tbuf_hour中 
			lv_label_set_text(label_min,(char*)tbuf_min);
			sprintf((char*)tbuf_second,"%02d",RTC_TimeStruct.RTC_Seconds); // 将RTC里面的时间的-秒-放入tbuf_hour中 
			lv_label_set_text(label_second,(char*)tbuf_second);
		}
	}

}

void event_handler(lv_obj_t * obj,lv_event_t event) //设置时间开始回调函数
{
	if(obj == scr)
	{
		label_hour = NULL ;
		//lv_obj_del(label_hour);
		lv_obj_clean(scr); // 清除scr下的所有对象
//		lv_obj_set_hidden(label_hour,1);
//		lv_obj_set_hidden(label_min,1);
//		lv_obj_set_hidden(label_second,1);
		scr1 = lv_scr_act();
		lv_calendar_roller_set();
	}
}

static void btn_event_cb(lv_obj_t * obj,lv_event_t event)//设置时间完毕返回回调函数
{
		int aaa=00;
	if(event == LV_EVENT_RELEASED)
	{
		if(obj == btn1)
		{
			lv_btn_state_t state = lv_btn_get_state(btn1); // 获取切换按钮的当前状态
			lv_roller_get_selected_str(roller_hour,set_hour,0);
			lv_roller_get_selected_str(roller_min,set_min,0);
			lv_roller_get_selected_str(roller_second,set_second,0);	
			for(int i=0;i<1;i++)
			{
				x=*(int*)(set_hour-0x30);			
//				xa=set_hour[i]-0x30;
//				xa=((set_hour[i+1]-0x30)<<4)+xa;
//				ya=set_min[i]-0x30;
//				ya=((set_min[i+1]-0x30)<<4)+ya;
//				za=set_second[i]-0x30;
//				za=((set_second[i+1]-0x30)<<4)+za;
			}
			
					
			RTC_Set_Time(x,aaa,aaa,RTC_H12_AM);
			//RTC_Set_Time((u16)*x-0x30,(u16)*set_min-0x30,(u16)*set_second-0x30,RTC_H12_AM);
			lv_obj_clean(scr1);
			scr = lv_scr_act();
			lv_calendar_start();
		}
	}
}

void lv_calendar_start(void)
{	
	
	//页面初始化
	scr = lv_scr_act(); // 获取当前活跃的屏幕对象
	lv_obj_set_click(scr,true); // 使能触屏功能
	lv_obj_set_event_cb(scr,event_handler);//设置事件回调函数
	
	//创建label标签--时
	label_hour = lv_label_create(scr,NULL);
	lv_obj_set_width(label_hour,200);
	lv_label_set_recolor(label_hour,true);
	lv_label_set_align(label_hour,LV_LABEL_ALIGN_CENTER); 
	lv_label_set_style(label_hour,LV_LABEL_STYLE_MAIN,&lv_style_pretty);
	lv_label_set_body_draw(label_hour,true);
	lv_obj_align(label_hour,NULL,LV_ALIGN_CENTER,-35,-20);
	
	
	//创建label标签--分
	label_min = lv_label_create(scr,NULL);
	lv_obj_set_width(label_min,200);
	lv_label_set_recolor(label_min,true);
	lv_label_set_align(label_min,LV_LABEL_ALIGN_CENTER);
	lv_label_set_style(label_min,LV_LABEL_STYLE_MAIN,&lv_style_pretty);
	lv_label_set_body_draw(label_min,true);
	lv_obj_align(label_min,NULL, LV_ALIGN_CENTER,5,-20);
	

	//创建label标签--秒
	label_second = lv_label_create(scr,NULL);
	lv_obj_set_width(label_second,200);
	lv_label_set_recolor(label_second,true);
	lv_label_set_align(label_second,LV_LABEL_ALIGN_CENTER);
	lv_label_set_style(label_second,LV_LABEL_STYLE_MAIN,&lv_style_pretty);
	lv_label_set_body_draw(label_second,true);
	lv_obj_align(label_second,NULL,LV_ALIGN_CENTER,45,-20);

	
}



