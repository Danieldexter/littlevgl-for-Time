//#include "lv_kb_test.h"
//#include "lvgl.h"
//#include "key.h"
//#include "delay.h"
//#include "delay.h"
//#include <string.h>

////坐标点集合
//const lv_point_t line_points[] = { {22, 22}, {178, 22}, {178, 118}, {22, 118},{22, 22} };
////坐标点的个数
//#define LINE_POINTS_NUM (sizeof(line_points)/sizeof(line_points[0]))
//lv_obj_t * line1;

//lv_obj_t * scr;	//对象声明
//lv_obj_t * kb1;
//lv_obj_t * ta1;
//lv_obj_t * left;
//lv_obj_t * right;
//lv_obj_t * top;
//lv_obj_t * bottom;

//	int left_i = 0;
//	int right_i = 0;
//	int top_i = 0;
//	int bottom_i = 0;

////实现自定义键盘,可以参考lv_kb.c源码中的映射表的格式
////我们这里在官方自带的小写键盘基础上增加2个按键,都放到空格键的右边,一个是普通的@符号按键,
////另外一个是输入内容清空键,即Clear键,这个键是特殊功能键,
////littleVGL中不自带此特殊功能的键,所以我们得自定义此特殊键
//#define MY_KB_CTRL_BTN_FLAGS (LV_BTNM_CTRL_NO_REPEAT | LV_BTNM_CTRL_CLICK_TRIG) //无重复按下功能,选择松手触发

//static const char * const my_kb_map_lc[] = {"1#", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "Bksp", "\n",
//                                   "ABC", "a", "s", "d", "f", "g", "h", "j", "k", "l", "Enter", "\n",
//                                   "_", "-", "z", "x", "c", "v", "b", "n", "m", ".", ",", ":", "\n",
//                                   LV_SYMBOL_CLOSE, LV_SYMBOL_LEFT, " ",/*自己添加的键*/ "@",/*自己添加的键*/ "Clear", LV_SYMBOL_RIGHT, LV_SYMBOL_OK, ""};

//static const lv_btnm_ctrl_t my_kb_ctrl_lc_map[] = {
//    MY_KB_CTRL_BTN_FLAGS | 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 7,
//    MY_KB_CTRL_BTN_FLAGS | 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 7,
//    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//    MY_KB_CTRL_BTN_FLAGS | 2, 2, /*由原来的6改为了3*/ 3,/*自己添加的键的控制属性*/ 1,/*自己添加的键的控制属性*/ MY_KB_CTRL_BTN_FLAGS | 2, 2, MY_KB_CTRL_BTN_FLAGS | 2};

///*函数声明*/
//void kb_create(lv_obj_t * parent);

///*事件回调函数*/
//void event_handler(lv_obj_t * obj,lv_event_t event)
//{
//  if(obj==ta1)
//  {
//    if(event==LV_EVENT_RELEASED)//松手事件
//    {
//      if(kb1==NULL)
//        kb_create(lv_scr_act());//如果键盘不存在的话,则先创建键盘
//      else
//        lv_kb_set_ta(kb1,obj);//重新绑定文本域对象
//    }
//  }else if(obj==kb1)
//  {
//    const char * key_txt = lv_btnm_get_active_btn_text(kb1);//获取按下键的文本标题,放到lv_kb_def_event_cb的前面调用
//    lv_kb_def_event_cb(kb1,event); // 调用键盘的默认事件回调函数,帮我们处理掉一些通用的操作,如果让我们自己写代码来处理,那就太麻烦了
//    //添加自己的功能代码
//    if(event==LV_EVENT_VALUE_CHANGED)
//    {
//      //uint16_t key_id = *(uint16_t*)lv_event_get_data(); // 获取按下键的id,第一个按键的id为0,后面的按键依次增1
//      if(key_txt==NULL)
//        return;
//      if(strcmp(key_txt,"Clear")==0) // 按下的是清空键
//      {
//        lv_obj_t * bind_ta = lv_kb_get_ta(kb1); // 获取当前绑定的文本域对象
//        lv_ta_set_text(bind_ta,""); // 将输入内容清空
//		if(left != NULL || left_i != 0)
//		{
//				lv_obj_del(left); // 条件满足删除对象left；
//				left = NULL; // 删除对象后，不进行归零，可能会导致后续运行死机。
//				left_i--;
//		}
//		if(right != NULL || right_i != 0)
//		{
//				lv_obj_del(right); // 条件满足删除对象right；
//				right = NULL; // 删除对象后，不进行归零，可能会导致后续运行死机。
//				right_i--;
//		}
//		if(top != NULL || top_i != 0)
//		{
//				lv_obj_del(top); // 条件满足删除对象top；
//				top = NULL; // 删除对象后，不进行归零，可能会导致后续运行死机。
//				top_i--;
//		}
//		if(bottom != NULL)
//		{
//				lv_obj_del(bottom); // 条件满足删除对象bottom；
//				bottom = NULL;  // 删除对象后，不进行归零，可能会导致后续运行死机
//		}
//		
//      }else if(strcmp(key_txt,"abc")==0) // 按下的是切换小写键盘键
//      {
//			// 重定向到我们自己自定义的小写键盘,而不是系统自带的小写键盘
//			lv_kb_set_map(kb1,(const char **)my_kb_map_lc); // 设置自定义按键的映射表
//			lv_kb_set_ctrl_map(kb1,my_kb_ctrl_lc_map); // 设置自定义按键的属性控制映射表
//      }
//    }else if(event==LV_EVENT_APPLY)
//    {
//      /*
//        这里可以根据用户输入的文本信息,做相应的业务逻辑处理
//      */
//      lv_obj_del(kb1); // 点击√键时把键盘删除掉
//      kb1 = NULL; // 删除对象后，不进行归零，可能会导致后续运行死机
//    }else if(event==LV_EVENT_CANCEL)
//    {
//      /*
//        这里也可以根据用户输入的文本信息,做相应的业务逻辑处理
//      */
//      lv_obj_del(kb1);//点击×键时把键盘删除掉
//      kb1 = NULL;
//    }
//  }
//}

////创建键盘
//void kb_create(lv_obj_t * parent)
//{
//  kb1 = lv_kb_create(parent,NULL);
//  lv_kb_set_cursor_manage(kb1,true);//使能对光标的接管
//  lv_kb_set_mode(kb1,LV_KB_MODE_TEXT);//设置为文本键盘模式,这也是默认值
//  lv_kb_set_ta(kb1,ta1);//先默认绑定ta1文本域对象,后面可以通过点击某文本域来重新绑定到相应的文本域对象
//  lv_kb_set_map(kb1,(const char **)my_kb_map_lc);//设置自定义按键的映射表
//  lv_kb_set_ctrl_map(kb1,my_kb_ctrl_lc_map);//设置自定义按键的属性控制映射表
//  lv_obj_set_event_cb(kb1,event_handler);//设置自定义的事件回调函数
//}

////例程入口
//void lv_kb_test_start()
//{
//	lv_obj_t *scr = lv_scr_act();//获取当前活跃的屏幕对象
//	lv_obj_set_click(scr,true);
//	lv_obj_set_event_cb(scr,event_handler);
//	//1.创建俩个文本域对象
//	//1.1 创建ta1文本域对象
//	ta1 = lv_ta_create(scr,NULL);
//	lv_obj_set_width(ta1,100);//设置宽度
//	lv_obj_align(ta1,NULL,LV_ALIGN_IN_TOP_MID,0,80);
//	lv_ta_set_one_line(ta1,true);//使能单行模式,在单行模式下,文本域的高度是不能被设置的
//	lv_ta_set_text(ta1,"");//设置为空文本
//	lv_obj_set_event_cb(ta1,event_handler);//设置事件回调函数
//	
//	//  //1.2 创建ta2文本域对象
//	//  ta2 = lv_ta_create(scr,ta1);//直接从ta1进行复制
//	//  lv_ta_set_pwd_mode(ta2,true);//使能密码模式
//	//  lv_ta_set_cursor_type(ta2,LV_CURSOR_LINE|LV_CURSOR_HIDDEN);//先隐藏掉光标
//	//  lv_obj_align(ta2,ta1,LV_ALIGN_OUT_BOTTOM_MID,0,15);
//	

//	//2.创建键盘对象,创建出来的键盘默认是与父对象底部居中对齐的,
//	//默认宽度等于父对象的可适应宽度(即除掉左右内边距后的宽度),
//	//默认高度等于父对象的可适应高度(即除掉上下内边距后的高度)的一半
//	kb_create(scr);
//  
//	//1.创建自定义样式
//	static lv_style_t line_style;
//	lv_style_copy(&line_style, &lv_style_plain);
//	line_style.line.color = LV_COLOR_RED;//线条的颜色
//	line_style.line.width = 4;//线条的厚度
//	line_style.line.rounded = 1;//线条的末端是否为圆角
//	//2.创建线条对象
//	line1 = lv_line_create(scr, NULL);//创建线条对象
//	lv_obj_set_pos(line1,20,20);//设置坐标
//	//使能大小自适应,当然了,你也可以不调用,因为默认就是被使能了的
//	lv_line_set_auto_size(line1,true);
//	//设置坐标点集合,同时也会在此内部计算出线条对象的大小
//	lv_line_set_points(line1, line_points, LINE_POINTS_NUM);
//	lv_line_set_style(line1, LV_LINE_STYLE_MAIN, &line_style);//设置样式
//	}	


//void key_handler()
//{
//	
//	u8 key = KEY_Scan(0);
//	lv_obj_t *scr = lv_scr_act();
//	if(key==KEY2_PRES)
//	{
//		if(left == NULL)
//		{
//			left = lv_obj_create(scr,NULL);
//			lv_obj_set_pos(left,20,40);//设置坐标位置
//			lv_obj_set_size(left,20,100);//设置大小
//			left_i++;
//		}
//	}else if(key==KEY0_PRES)
//	{
//		right = lv_obj_create(scr,NULL);
//		lv_obj_set_pos(right,200,40);//设置坐标位置
//		lv_obj_set_size(right,20,100);//设置大小
//		right_i++;
//	}else if(key==KEY1_PRES)
//	{
//		bottom = lv_obj_create(scr,NULL);
//		lv_obj_set_pos(bottom,40,140);//设置坐标位置
//		lv_obj_set_size(bottom,160,20);//设置大小
//		bottom_i++;
//	}else if(key==WKUP_PRES)
//	{
//		top = lv_obj_create(scr,NULL);
//		lv_obj_set_pos(top,40,20);//设置坐标位置
//		lv_obj_set_size(top,160,20);//设置大小
//		top_i++;
//	}
//	
//}
