/////////////////////////////贪吃蛇小游戏//////////////////////////
//编译环境：Visual Studio2017 Eaxsyx图形库
//作者：DODO
//修改时间：2019.12.31

//调用所需的头文件(函数库)
#include<graphics.h>						//调用Easyx图形库函数
#include<time.h>
#include<conio.h>
#include<stdio.h>
#pragma comment(lib,"Winmm.lib")			//多媒体播放的函数库
////////////////相关宏定义//////////////////////////
#define LONG 30
#define MIDDLE_X 400
#define MIDDLE_Y 270
#define ZERO 0
#define SNAKE_MAX 60
//用户结构体
typedef struct {
	CHAR name[10],access[10];
}vic;
//食物结构体
typedef struct{
	int x;
	int y;
}Food;
//蛇的结构体
typedef struct {
	int x;
	int y;
}Snake;
//相关数据变量的定义与初始化
Food food;
Snake snake[SNAKE_MAX];
IMAGE game_background, vic_background;
int snake_num = 3,snake_spead=200;
//主函数
int main(char args[])
{
	void LOGO();							//声明LOGO函数
	bool check(char Secret_key[]);			//声明秘钥检测函数 正确返回true 错误返回false
	void check_interface();					//声明秘钥检验界面设计函数
	void draw_map();						//声明游戏地图设计函数
	void bar();
	void init_snake();						//初始化蛇的函数
	void init_food();
	void produce_food();
	void draw_food();
	void home_interface();
	void draw_snake(char);
	void control_snake();
	bool check_food();
	bool login_interface();
	bool check_snake();
	srand((unsigned)time(NULL));			//创建一个时间种子
	initgraph(800, 540);					//创建一个画布 大小为1000*840
	init_snake();							//调用函数对蛇身进行初始化
	init_food();
	LOGO();
	check_interface();						//调用秘钥检验函数
	home_interface();
	bar();
	while (true)
	{
		draw_map();
		control_snake();
		outtextxy(MIDDLE_X, MIDDLE_Y, _T("按任意键继续游戏"));
		system("pause");
		snake_num = 3;
		init_snake();
	}
	return 0;
}

//**************************************环境设置辅助函数*******************************************//

//////////////字体格式设置函数//////////
void Settextstyle(int lfWeight)
{
	LOGFONT f;							//定义一个结构体字体变量
	gettextstyle(&f);					//获取当前字体格式并把其地址赋值给变量
	f.lfQuality = PROOF_QUALITY;		//设置字体输出质量为抗锯齿
	f.lfWeight = lfWeight;				//设置字体笔画粗细程度
	settextstyle(&f);					//设置当前字体格式
}
//////////////透明贴图函数//////////
void transparent_putimage(int x,int y,IMAGE *original_image,IMAGE *mask_image)
{
	putimage(x, y, mask_image, SRCAND);
	putimage(x, y, original_image, SRCPAINT);
}
////////////游戏图标////////////////////
void game_icon(int bottom)
{
	setbkmode(ZERO);
	setcolor(WHITE);
	settextstyle(15, 0, _T("Bahnschrift"));
	outtextxy(2, bottom - 16, "WLECOME ");
	setcolor(RED);
	outtextxy(65, bottom - 16, "D");
	setcolor(GREEN);
	outtextxy(74, bottom - 16, "O");
	setcolor(RED);
	outtextxy(83, bottom - 16, "D");
	setcolor(GREEN);
	outtextxy(93, bottom - 16, "O");
	setcolor(WHITE);
	outtextxy(105, bottom - 16, "GAME");
}
/////////////进度条////////////////////
void bar()
{
	mciSendString(_T("play musics\\进度条音乐.mp3"), NULL, 0, NULL);
	Settextstyle(100);
	setbkmode(ZERO);
	IMAGE imag, image2,image3;
	loadimage(&imag, _T("images\\进度条.jpg"), MIDDLE_X*2, MIDDLE_Y*2);
	putimage(0, 0, &imag);
	getimage(&image2, 240, 370, 30, 30);
	setcolor(YELLOW);
	setfillcolor(WHITE);
	fillrectangle(50, MIDDLE_Y*2-120, 750, MIDDLE_Y*2-110);
	settextstyle(30, 0, "方正粗黑宋简体");
	outtextxy(50, 370, "加载中，请稍后");
	BeginBatchDraw();
	char str[5];
	for (int num = 0, i = 51; i <= 749; i++)
	{
		if (i  % 7 == 0)
		{
			setcolor(YELLOW);
			num++;
			_itoa_s(num, str, 10);
			outtextxy(280, 370, "%");
			putimage(240, 370, &image2);
			outtextxy(240, 370, str);
			FlushBatchDraw();
		}
		setcolor(RED);
		line(i, 421, i, 429);
		FlushBatchDraw();
		Sleep(rand() % 35);
	}
	EndBatchDraw();
	Sleep(1000);
}
//////////////游戏地图设计/////////////
void draw_map()
{
	mciSendString("play musics\\游戏音乐.mp3 repeat", NULL, ZERO, NULL);
	setbkcolor(LIGHTRED);
	cleardevice();
	Settextstyle(50);
	setbkmode(ZERO);
	IMAGE map_wall, map_vic, map_background,map_startgame,map_startgame_mask;
	bool start_game_flag = true;
	loadimage(&map_startgame, "images\\开始游戏图标.png", 150, 50);
	loadimage(&map_startgame_mask, "images\\开始游戏图标掩码图.png", 150, 50);
	loadimage(&map_vic, "images\\用户.jpg", 260, MIDDLE_Y * 2);
	loadimage(&map_wall, "images\\墙.png", 30, 30);
	loadimage(&map_background, "images\\地图背景.jpg", 510, 520);
	//开始绘制地图及用户界面
	putimage(0, 0, &map_vic);
	putimage(290, 0, &map_background);
	for (int i = 0; i <= 17; i++)
	{
		putimage(260, 0 + i * 30, &map_wall);
		putimage(MIDDLE_X * 2 - 30, 0+i*30, &map_wall);
		putimage(290 + i * 30, 0, &map_wall);
		putimage(290 + i * 30, MIDDLE_Y*2-30, &map_wall);
	}
	//绘制用户区域元素
	IMAGE vic_head_frame, vic_head,vic_name,vic_info;
	loadimage(&vic_head_frame, "images\\头像框.png",70,70);
	loadimage(&vic_info, "images\\文本框2.png",125,40);
	loadimage(&vic_name, "images\\文本框3.png", 100, 35);
	loadimage(&vic_head, "images\\头像.jpg",60,60);
	putimage(95, 33, &vic_head_frame);
	putimage(100, 38, &vic_head);
	putimage(80, 115, &vic_name);
	setlinestyle(PS_SOLID, 3);
	setlinecolor(DARKGRAY);
	line(70, 153, 190, 153);
	settextcolor(RGB(225,0,0));
	settextstyle(27, 0, "方正粗黑宋简体");
	putimage(12, 190, &vic_info);
	outtextxy(40, 200, "等级:");
	putimage( 12, 240, & vic_info);
	outtextxy(40, 250, "分数:");
	putimage(12, 290, &vic_info);
	outtextxy(40, 300, "难度:");
	putimage(12, 340, &vic_info);
	outtextxy(40, 350, "关卡:");
	putimage(12, 390, &vic_info);
	outtextxy(40, 400, "设置:");
	putimage(12, 440, &vic_info);
	outtextxy(40, 450, "帮助:");
	settextstyle(20, 0, "方正粗黑宋简体");
	setcolor(RGB(220,0,0));
	outtextxy(100, 123, "曙光女神");
	game_icon(MIDDLE_Y*2);
	//获取游戏界面游戏区域的图像
	getimage(&game_background, 290, 30, 480, 480);
	//获取游戏界面用户区域的图像
	getimage(&vic_background, 0, 0, 260, 540);
	//绘制开始游戏图标
	putimage(MIDDLE_X+50, MIDDLE_Y - 25, &map_startgame);
	MOUSEMSG mouse;
	FlushMouseMsgBuffer();
	while (true)
	{
		mouse=GetMouseMsg();
		//无按键鼠标消息处理区
		if (mouse.x >= MIDDLE_X+50 && mouse.x <= MIDDLE_X + 200 && mouse.y >= MIDDLE_Y - 25 && mouse.y <= MIDDLE_Y + 25)
		{
			putimage(MIDDLE_X+50, MIDDLE_Y - 25, &map_startgame_mask);
		}
		else
		{
			putimage(MIDDLE_X+50, MIDDLE_Y - 25, &map_startgame);
		}
		//有按键鼠标消息处理区
		switch (mouse.uMsg)
		{
		case WM_LBUTTONDOWN:
			if (mouse.x >= MIDDLE_X+50&&mouse.x <=MIDDLE_X+200 &&mouse.y >=MIDDLE_Y-25 &&mouse.y <=MIDDLE_Y+25 )
			{
				start_game_flag = false;
			}
			break;
		}
		if (!start_game_flag) { start_game_flag = true; break; }
		FlushMouseMsgBuffer();
	}
}
///////////////LOGO设计////////////////
void LOGO()
{
	Settextstyle(100);
	setbkmode(0);
	settextstyle(LONG, 0, "verdana");
	mciSendString(_T("play musics\\LOGO.mp3"), NULL, 0, NULL);
	IMAGE image1, image2, image3;
	loadimage(&image1, _T("images\\LOGO1.jpg"), MIDDLE_X*2, MIDDLE_Y*2);
	putimage(0, 0, &image1);
	BeginBatchDraw();
	for (int i = 0; i < 200; i++)
	{
		putpixel(MIDDLE_X + i, MIDDLE_Y*2-30, _T(WHITE));
		putpixel(MIDDLE_X - i, MIDDLE_Y*2-30, _T(WHITE));
		Sleep(20);
		FlushBatchDraw();
	}
	getimage(&image2, 0,0,MIDDLE_X*2,MIDDLE_Y*2);
	getimage(&image3, MIDDLE_X-50, MIDDLE_Y*2-30, 100, 30);
	for (int i = MIDDLE_Y*2-30; i > MIDDLE_Y*2-80; i--)
	{
		setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
		putimage(ZERO, ZERO, &image2);
		outtextxy(MIDDLE_X - 50, i, _T("DO DO"));
		putimage(MIDDLE_X-50, MIDDLE_Y*2-30, &image3);
		FlushBatchDraw();
		Sleep(LONG*3);
	}
	EndBatchDraw();
	Sleep(4000);
	mciSendString(_T("stop musics\\LOGO.mp3"), NULL, 0, NULL);
}
//////////////登陆界面设计////////////
bool login_interface()
{
	//登陆界面设计
	mciSendString("stop  musics\\首页界面音乐.mp3 ", NULL, ZERO, NULL);
	mciSendString("play musics\\登陆界面背景音乐.mp3 repeat", NULL, ZERO, NULL);
	Settextstyle(100);
	IMAGE login_image,login_background,login_background_help,login_text_1,login_text_2,login_back,login_back_mask,login_back_background,login_back_2;
	loadimage(&login_back, "images\\图标.png", 80,LONG+10);
	loadimage(&login_back_2, "images\\图标2.png",  80, LONG + 10);
	loadimage(&login_back_mask, "images\\图标掩码图.png", 80, LONG+10);
	loadimage(&login_image, "images\\登陆界面图片.jpg", MIDDLE_X * 2, MIDDLE_Y * 2);		//加载登陆界面图片
	putimage(ZERO, ZERO, &login_image);														//贴出背景图片
	//登陆注册图标设计
	settextstyle(40, 0, _T("方正粗黑宋简体"));
	settextcolor(LIGHTRED);
	setlinecolor(WHITE);
	setfillcolor(LIGHTGRAY);
	outtextxy(MIDDLE_X - 125, MIDDLE_Y - 100, "请选择服务项目");
	line(MIDDLE_X - 140, MIDDLE_Y - 60, MIDDLE_X + 140, MIDDLE_Y - 60);
	settextstyle(25, 0, _T("Yu Gothic UI Semilight"));
	outtextxy(MIDDLE_X - 125, MIDDLE_Y - 57, "PLEASE SELECT SERVICE ITEM");
	fillroundrect(MIDDLE_X - 200, MIDDLE_Y + 50, MIDDLE_X - 50, MIDDLE_Y + 95, 10, 10);
	fillroundrect(MIDDLE_X +50, MIDDLE_Y + 50, MIDDLE_X +200, MIDDLE_Y + 95, 10, 10);
	settextcolor(YELLOW);
	settextstyle(40, 0, "方正粗黑宋简体");
	outtextxy(MIDDLE_X - 160, MIDDLE_Y + 54, "注册");
	outtextxy(MIDDLE_X +90, MIDDLE_Y + 54, "登陆");
	//获取帮助图标设计
	setcolor(WHITE);
	settextstyle(20, 0, "宋体");
	circle(MIDDLE_X * 2 - 12, 12, 11);
	outtextxy(MIDDLE_X * 2 - 16, 2, "?");
	//游戏标志贴图
	game_icon(MIDDLE_Y * 2);
	//返回图标设计
	putimage(0, 0, &login_back_mask, SRCAND);
	putimage(0, 0, &login_back,SRCPAINT);
	//相关图标图片和背景的获取
	getimage(&login_back_background, ZERO, ZERO, 81, 41);
	getimage(&login_background, ZERO, ZERO, MIDDLE_X * 2, MIDDLE_Y * 2);
	getimage(&login_background_help, MIDDLE_X * 2 - 100, 2, 71, 20);
	getimage(&login_text_1, MIDDLE_X - 200, MIDDLE_Y + 50, 150, 45);
	getimage(&login_text_2, MIDDLE_X +50, MIDDLE_Y + 50, 150, 45);
	//鼠标消息处理区域
	MOUSEMSG mouse;
	bool login_flag=false,login_again=false;
	FlushMouseMsgBuffer();
	while (true)
	{
		mouse=GetMouseMsg();
		switch (mouse.uMsg)
		{
		case WM_MOUSEMOVE:
			if (mouse.x >= MIDDLE_X * 2 - 23 && mouse.x <= MIDDLE_X * 2 && mouse.y >= 1 && mouse.y <= 23)
			{
				setcolor(WHITE);
				fillroundrect(MIDDLE_X * 2 - 100, 2, MIDDLE_X * 2 - 30, 21, 10, 10);		//画一个填充椭圆
				setcolor(WHITE);
				settextstyle(20, 0, "方正粗黑宋简体");
				outtextxy(MIDDLE_X * 2 - 96, 2, "获取帮助");
			}
			else if (mouse.x >= MIDDLE_X + 50 && mouse.x <= MIDDLE_X + 200 && mouse.y >= MIDDLE_Y + 50 && mouse.y <= MIDDLE_Y + 95)
			{
				settextstyle(40, 0, "方正粗黑宋简体");
				settextcolor(LIGHTRED);
				outtextxy(MIDDLE_X + 90, MIDDLE_Y + 54, "登陆");

			}
			else if (mouse.x >= MIDDLE_X - 200 && mouse.x <= MIDDLE_X - 50 && mouse.y >= MIDDLE_Y + 50 && mouse.y <= MIDDLE_Y + 95)
			{
				settextstyle(40, 0, "方正粗黑宋简体");
				settextcolor(LIGHTRED);
				outtextxy(MIDDLE_X - 160, MIDDLE_Y + 54, "注册");
			}
			else if (mouse.x >= ZERO && mouse.x <= 80 && mouse.y >= ZERO && mouse.y <= 40)
			{
				putimage(0, 0, &login_back_mask, SRCAND);
				putimage(0, 0, &login_back_2, SRCPAINT);
			}
			else
			{
				putimage(ZERO, ZERO, &login_back_background);
				putimage(MIDDLE_X * 2 - 100, 2, &login_background_help);
				putimage(MIDDLE_X - 200, MIDDLE_Y + 50, &login_text_1);
				putimage(MIDDLE_X + 50, MIDDLE_Y + 50, &login_text_2);
			}
			break;
		case WM_LBUTTONDOWN:
			if (mouse.x >=MIDDLE_X-200 &&mouse.y >=MIDDLE_Y+50 &&mouse.x <= MIDDLE_X-50&&mouse.y <= MIDDLE_Y+95)
			{
				mciSendString("play musics\\鼠标点击.mp3", NULL, ZERO, NULL);
				cleardevice();
				outtextxy(100, 100, "注册");
				_getch();
				login_again = true;
			}
			else if (mouse.x >= MIDDLE_X + 50 &&mouse.y >= MIDDLE_Y + 50 &&mouse.x <= MIDDLE_X + 200 &&mouse.y <= MIDDLE_Y + 95)
			{
				mciSendString("play musics\\鼠标点击.mp3", NULL, ZERO, NULL);
				cleardevice();
				outtextxy(100, 100, "登陆");
				_getch();
				login_again = true;
			}
			else if (mouse.x >= MIDDLE_X * 2 - 23 && mouse.x <= MIDDLE_X * 2 && mouse.y >= 1 && mouse.y <= 23)
			{
				mciSendString("play musics\\鼠标点击.mp3", NULL, ZERO, NULL);
				setbkcolor(LIGHTGRAY);
				cleardevice();
				game_icon(MIDDLE_Y * 2);												//打印游戏图标
				setcolor(RED);
				settextstyle(40, 0, "方正粗黑宋简体");
				outtextxy(70, MIDDLE_Y - 30, "登陆后可对您的游戏数据读档，方便下次游戏！");
				settextstyle(30, 0, "方正粗黑宋简体");
				outtextxy(MIDDLE_X - 120, MIDDLE_Y * 2 - 40, "->按任意键返回<-");
				system("pause");
				login_again=true;
			}
			else if (mouse.x >= ZERO && mouse.x <= 80 && mouse.y >= ZERO && mouse.y <= 40) { mciSendString("play musics\\鼠标点击.mp3", NULL, ZERO, NULL); login_flag = true; break; }
			break;
		}
		if (login_flag) { login_flag = false; mciSendString("stop musics\\登陆界面背景音乐.mp3", NULL, ZERO, NULL); break; }
		if (login_again) { login_again = false; putimage(ZERO, ZERO, &login_background); }
	}
	return false;
}
///////////////首页界面////////////////
void home_interface()
{
	mciSendString("play musics\\首页界面音乐.mp3 repeat", NULL, ZERO, NULL);
	setbkcolor(BLACK);
	Settextstyle(100);
	cleardevice();
	/*定义对应的图片变量*/
	IMAGE home_image1, home_start, home_start_mask, home_music, home_login_game,home_interface_again;
	/*加载对应的图片变量*/
	loadimage(&home_image1, "images\\首页界面.png", MIDDLE_X * 2, MIDDLE_Y * 2);
	loadimage(&home_start, "images\\开始游戏.png", 100, 35);
	loadimage(&home_login_game, "images\\文本框.png", 100, 35);
	putimage(0, 0, &home_image1);
	/********开始贴图*********/
	//画游戏图标
	game_icon(MIDDLE_Y * 2);
	//画开始游戏图标
	settextstyle(20, 0, "Segoe UI Emoji");
	setcolor(RGB(225,0,0));
	putimage(MIDDLE_X - 50, MIDDLE_Y * 2 - 182, &home_login_game);
	outtextxy(MIDDLE_X -19, MIDDLE_Y * 2-175, "START");
	//画登录游戏图标
	putimage(MIDDLE_X - 50, MIDDLE_Y * 2 -132, &home_login_game);
	outtextxy(MIDDLE_X - 19, MIDDLE_Y * 2 -125, "LOGIN");
	//画帮助游戏图标
	putimage(MIDDLE_X - 50, MIDDLE_Y * 2- 82, &home_login_game);
	outtextxy(MIDDLE_X -19, MIDDLE_Y * 2 -75, "HELP");
	getimage(&home_interface_again,0,0, MIDDLE_X * 2, MIDDLE_Y * 2);
	MOUSEMSG mouse;
	bool home_again=false,home_flag=false;
	FlushMouseMsgBuffer();
	while (true)
	{
		mouse = GetMouseMsg();
		switch (mouse.uMsg)
		{
		case WM_LBUTTONDOWN:
			//左击开始游戏按钮
			if (mouse.x >= MIDDLE_X - 50 && mouse.x <= MIDDLE_X + 50 && mouse.y >= MIDDLE_Y * 2 - 182 && mouse.y <= MIDDLE_Y * 2 - 182 + 35) { home_flag = true;  mciSendString("play musics\\鼠标点击.mp3", NULL, ZERO, NULL);
			}
			//左击登陆按钮
			else if (mouse.x >= MIDDLE_X - 50 && mouse.x <= MIDDLE_X + 50 && mouse.y >= MIDDLE_Y * 2 - 132 && mouse.y <= MIDDLE_Y * 2 - 132 + 35)
			{
				mciSendString("play musics\\鼠标点击.mp3", NULL, ZERO, NULL);
				cleardevice();
				login_interface();
				home_again = true;
			}
			//左击帮助按钮
			else if (mouse.x >= MIDDLE_X - 50 && mouse.x <= MIDDLE_X + 50 && mouse.y >= MIDDLE_Y * 2 - 75 && mouse.y <= MIDDLE_Y * 2 - 75 + 35)
			{
				mciSendString("play musics\\鼠标点击.mp3", NULL, ZERO, NULL);
				cleardevice();
				outtextxy(100, 100, "这里是帮助界面");
				home_again = true;
				system("pause");
			}
			break;
		}
		if (home_flag) { mciSendString("stop musics\\首页界面音乐.mp3", NULL, ZERO, NULL); break; }
		if (home_again) { home_again = false; mciSendString("play musics\\首页界面音乐.mp3 repeat", NULL, ZERO, NULL); putimage( 0, 0, & home_interface_again ); }
		FlushMouseMsgBuffer();
	}
}
//////////////秘钥检测界面设计/////////
void check_interface()
{
	mciSendString(_T("play musics\\秘钥界面背景音乐.mp3 repeat"), NULL, NULL, NULL);	//从文件资源管理器中播放背景音乐
	settextcolor(WHITE);
	char Secret_key[10];																//定义一个字符数组用来存放用户输入的秘钥
	bool check(char Secret_key[10]);
	cleardevice();																		//擦除屏幕
	setbkmode(ZERO);																	//设置字符输出背景为透明
	Settextstyle(100);																	//设置字体为抗锯齿 笔画粗细为100
	IMAGE check_interface_image, check_interface_image_1, background_1, background_2, background_3, music_background;	//定义一个图片变量用于存放背景图片
	MOUSEMSG mouse;
	loadimage(&check_interface_image, _T("images\\秘钥检测界面背景图片.jpg"), MIDDLE_X * 2, MIDDLE_Y * 2);				//从文件资源管理器中加载背景图
	putimage(0, 0, &check_interface_image);												//将图片贴到画板上
	game_icon(MIDDLE_Y * 2);
	setcolor(RED);
	settextstyle(40, 0, _T("方正粗黑宋简体"));											//设置字体的高为50像素 宽度为自适应 字体格式为Gothic UI Semibold
	outtextxy(MIDDLE_X - 150, MIDDLE_Y - 115, "请输入此游戏的秘钥");
	setcolor(WHITE);
	line(MIDDLE_X - 150, MIDDLE_Y - 75, MIDDLE_X + 150, MIDDLE_Y - 75);
	setcolor(RED);
	settextstyle(25, 0, _T("Yu Gothic UI Semilight"));									//设置字体的高为50像素 宽度为自适应
	outtextxy(MIDDLE_X - 118, MIDDLE_Y - 75, _T("PLEASE ENTER THE SECRET KEY"));		//输出字符串
	//获取帮助图标设置
	setcolor(WHITE);
	circle(MIDDLE_X * 2 - 12, 12, 11);
	outtextxy(MIDDLE_X * 2 - 15, 0, "?");												//设置画笔为浅灰色
	//音乐播放图标设置
	circle(MIDDLE_X * 2 - 12, 40, 11);
	getimage(&music_background, MIDDLE_X * 2 - 23, 29, 22, 22);							//获取音乐图标区域的图片
	outtextxy(MIDDLE_X * 2 - 20, 28, "‖"); //♬√×◀▶‖
	//秘钥输入图标设置
	setfillcolor(LIGHTGRAY);															//设置填充色为浅灰色
	fillroundrect(MIDDLE_X - 80, MIDDLE_Y - 30, MIDDLE_X + 80, MIDDLE_Y + 10, 10, 10);	//画一个填充椭圆
	setcolor(DARKGRAY);
	settextstyle(30, 0, "方正粗黑宋简体");
	outtextxy(MIDDLE_X - 74, MIDDLE_Y - 23, "点击这里输入");
	getimage(&background_1, MIDDLE_X * 2 - 100, 0, 100, 30);							//获取帮助区域的图片
	getimage(&background_2, MIDDLE_X - 80, MIDDLE_Y - 30, 160, 40);						//输入秘钥区域的图片
	getimage(&background_3, MIDDLE_X * 2 - 100, 28, 72, 30);							//关闭音乐区域的图片
	getimage(&check_interface_image_1, ZERO, ZERO, MIDDLE_X * 2, MIDDLE_Y * 2);			//获取整个屏幕的图片
	bool key_sign = false, flag = false;												//定义鼠标消息判断标志 并初始化为false
	int back_music_flag = ZERO;
	FlushMouseMsgBuffer();																//清除当前的鼠标消息
	while (true)
	{
		mouse = GetMouseMsg();
		/*****************************************无按键鼠标消息处理*****************************************/
			//获取帮助区域检测
		if (mouse.x >= MIDDLE_X * 2 - 22 && mouse.x <= MIDDLE_X * 2 && mouse.y >= 0 && mouse.y <= 22)
		{
			setcolor(WHITE);
			fillroundrect(MIDDLE_X * 2 - 100, 2, MIDDLE_X * 2 - 30, 21, 10, 10);		//画一个填充椭圆
			setcolor(WHITE);
			settextstyle(20, 0, "方正粗黑宋简体");
			outtextxy(MIDDLE_X * 2 - 96, 2, "获取帮助");
		}
		//秘钥输入区域检测
		else if (mouse.x >= MIDDLE_X - 80 && mouse.y >= MIDDLE_Y - 30 && mouse.x <= MIDDLE_X + 80 && mouse.y <= MIDDLE_Y + 10)
		{
			setcolor(LIGHTRED);
			settextstyle(30, 0, "方正粗黑宋简体");
			outtextxy(MIDDLE_X - 74, MIDDLE_Y - 23, "点击这里输入");
		}
		//音乐播放区域检测
		else if (mouse.x >= MIDDLE_X * 2 - 22 && mouse.x <= MIDDLE_X * 2 && mouse.y >= 28 && mouse.y <= 50)
		{
			setcolor(WHITE);
			fillroundrect(MIDDLE_X * 2 - 100, 30, MIDDLE_X * 2 - 30, 50, 10, 10);		//画一个填充椭圆
			setcolor(WHITE);
			settextstyle(20, 0, "方正粗黑宋简体");
			if (back_music_flag % 2 == 0) outtextxy(MIDDLE_X * 2 - 96, 30, "关闭音乐");
			else outtextxy(MIDDLE_X * 2 - 96, 30, "打开音乐");
		}
		//刷新部分区域
		else
		{
			putimage(MIDDLE_X * 2 - 100, 0, &background_1);
			putimage(MIDDLE_X - 80, MIDDLE_Y - 30, &background_2);
			putimage(MIDDLE_X * 2 - 100, 28, &background_3);
		}
		/*********************************************有按键鼠标消息处理**************************************/
		switch (mouse.uMsg)																//对鼠标消息进行处理
		{
		case WM_LBUTTONDOWN:
			//左击输入秘钥区域
			if (mouse.x >= MIDDLE_X - 80 && mouse.y >= MIDDLE_Y - 30 && mouse.x <= MIDDLE_X + 80 && mouse.y <= MIDDLE_Y + 10)
			{
				mciSendString("play musics\\鼠标点击.mp3", NULL, ZERO, NULL);
				setcolor(WHITE);
				fillroundrect(MIDDLE_X - 80, MIDDLE_Y - 30, MIDDLE_X + 80, MIDDLE_Y + 10, 10, 10);	//画一个填充椭圆
				InputBox(Secret_key, 10, _T("请输入秘钥"));							//以对话框的形式获取用户输入的字符串
				setcolor(LIGHTRED);
				settextstyle(40, 0, "方正粗黑宋简体");
				if (check(Secret_key))												//调用秘钥检测函数对用户输入的秘钥进行检测
				{
					setcolor(LIGHTGREEN);
					outtextxy(MIDDLE_X - 68, MIDDLE_Y - 28, "秘钥正确");
					Sleep(3000);
					flag = true;
					break;
				}
				else
				{
					setcolor(LIGHTRED);
					outtextxy(MIDDLE_X - 68, MIDDLE_Y - 28, "秘钥错误");
					Sleep(3000);
				}
				key_sign = true;													//对有效的按键进行标识变量重新赋值
			}
			//左击疑问获取帮助
			else if (mouse.x >= MIDDLE_X * 2 - 22 && mouse.x <= MIDDLE_X * 2 && mouse.y >= 0 && mouse.y <= 22)
			{
				mciSendString("play musics\\鼠标点击.mp3", NULL, ZERO, NULL);
				setbkcolor(LIGHTGRAY);
				cleardevice();
				game_icon(MIDDLE_Y * 2);												//打印游戏图标
				setcolor(RED);
				settextstyle(40, 0, "方正粗黑宋简体");
				outtextxy(MIDDLE_X - 155, MIDDLE_Y - 30, "秘钥可找管理员获取");
				settextstyle(30, 0, "方正粗黑宋简体");
				outtextxy(MIDDLE_X - 120, MIDDLE_Y * 2 - 40, "->按任意键返回<-");
				system("pause");
				key_sign = true;

			}
			//左击调节音乐
			else if (mouse.x >= MIDDLE_X * 2 - 22 && mouse.x <= MIDDLE_X * 2 && mouse.y >= 28 && mouse.y <= 50)
			{
				mciSendString("play musics\\鼠标点击.mp3", NULL, ZERO, NULL);
				back_music_flag++;													//改变音乐播放标志变量
				//根据音乐标志变量判断音乐是否播放
				if (back_music_flag % 2 == 1)										//当音乐标志变量为奇数播放音乐
				{
					mciSendString(_T("pause musics\\秘钥界面背景音乐.mp3 "), NULL, NULL, NULL);
					putimage(MIDDLE_X * 2 - 23, 29, &music_background);
					setcolor(WHITE);
					settextstyle(20, 0, "黑体");
					outtextxy(MIDDLE_X * 2 - 21, 30, "×");							//改变音乐图标为puase
				}
				else																//当音乐标志变量为偶数暂停音乐
				{
					Sleep(LONG*10);
					mciSendString(_T("play musics\\秘钥界面背景音乐.mp3 repeat"), NULL, NULL, NULL);
					putimage(MIDDLE_X * 2 - 23, 29, &music_background);
					outtextxy(MIDDLE_X * 2 - 19, 31, "‖");							//改变音乐图标为open

				}
				getimage(&check_interface_image_1, ZERO, ZERO, MIDDLE_X * 2, MIDDLE_Y * 2);
				key_sign = true;
			}
		}
		if (key_sign) { key_sign = false; putimage(ZERO, ZERO, &check_interface_image_1); }				//判断是否需要刷新界面
		if (flag) { mciSendString("stop musics\\秘钥界面背景音乐.mp3", NULL, ZERO, NULL); break; }		//判断是否跳出鼠标消息循环
	}
}
/////////////打印食物//////////////////
void draw_food()
{
	IMAGE food_photo;
	loadimage(&food_photo, "images\\食物.png", LONG, LONG);
	putimage(food.x, food.y, &food_photo);
}
//////////////画蛇////////////////////
void draw_snake(char head_flag)
{
	IMAGE snake_body,snake_head_right,snake_head_left,snake_head_top,snake_head_bottom,snake_end;
	loadimage(&snake_body, "images\\蛇身.png", LONG, LONG);
	loadimage(&snake_head_right, "images\\snake_right.png", LONG, LONG);
	loadimage(&snake_head_left, "images\\snake_left.png", LONG, LONG);
	loadimage(&snake_head_top, "images\\snake_top.png", LONG, LONG);
	loadimage(&snake_head_bottom, "images\\snake_bottom.png", LONG, LONG);
	loadimage(&snake_end, "images\\蛇尾.png", LONG, LONG);
	//开始画蛇
	for (int j = ZERO; j <= snake_num - 1; j++)
	{
		/**画出蛇头**/
		if (j == ZERO)
		{
			switch (head_flag)
			{
				case'W':putimage(snake[j].x, snake[j].y, &snake_head_top);
					break;
				case'S':putimage(snake[j].x, snake[j].y, &snake_head_bottom);
					break;
				case'A':putimage(snake[j].x, snake[j].y, &snake_head_left);
					break;
				case'D':putimage(snake[j].x, snake[j].y, &snake_head_right);
					break;
			}
		}
		/**画出蛇尾**/
		else if (j == snake_num - 1) putimage(snake[j].x, snake[j].y, &snake_end);
		/**画出蛇身**/
		else putimage(snake[j].x, snake[j].y, &snake_body);
	}
}
/////////////实时打印游戏数据/////////
void game_info()
{
	Settextstyle(100);
	settextcolor(RGB(225, NULL, NULL));
	settextstyle(30, 0, "方正粗黑宋简体");
	char game_score[3];							//定义一个字符数组变量接收转换后的数据
	_itoa_s(snake_num-3, game_score, 10);
	putimage(0, 0, &vic_background);			//贴用户界面的背景图片
	//_itoa_s(snake_num - 3, game_score,10);		//使用函数将整型数据转化为字符串
	outtextxy(100, 248, game_score);			//在对应位置打印数据
}

//**************************************功能实现函数**********************************************//

///////////////秘钥检测///////////////
bool check(char Secret_key[10])
{
	char Correct_Secret_key[] = "WsAd001";												//初始化正确的秘钥字符串
	if (strcmp(Secret_key, Correct_Secret_key)) return false;							//对用户输入的字符串和正确的秘钥进行检测
	else return true;
}
////////////////产生食物//////////////
void produce_food()
{
	bool flag = true;												//创建一个循环标志
	//开始产生有效食物
	while (true)
	{
		//用随机数来产生食物的两个坐标
		food.x = rand() % 451+290;
		food.y = rand() % 451+LONG;
		//检验食物的合法性
		if ((food.x+10)%LONG != ZERO||food.y%LONG != ZERO) continue;
		for (int i = ZERO; i <= snake_num - 1; i++)
		{
			if (snake[i].x == food.x&&snake[i].y == food.y) flag=false;

		}
		if (flag) break;		//判断是否跳出最外层循环
		else flag = true;
	}
}
///////////////初始化蛇//////////////
void init_snake()
{
	for (int i = ZERO; i <=snake_num - 1; i++)
	{
		snake[i] = { 500-i*LONG,420 };
	}
}
//////////////初始化食物/////////////
void init_food()
{
	food.x = 290;
	food.y = 90;
}
/////////////检测食物刷新////////////
bool check_food()
{
	//判断蛇头是否吃到食物 吃到返回true 没吃到返回false
	if (snake[ZERO].x == food.x&&snake[ZERO].y == food.y) return true;
	return false;
}
/////////////检测蛇的存活性//////////
bool check_snake()
{
	//检测蛇头是否咬到身体
	for (int i = 2; i <= snake_num - 1; i++)
	{
		if (snake[ZERO].x == snake[i].x&&snake[ZERO].y == snake[i].y) return true;
	}
	//判断蛇头是否撞墙
	if (snake[ZERO].x < 290 || snake[ZERO].x>=MIDDLE_X*2-LONG || snake[ZERO].y<LONG || snake[ZERO].y>=MIDDLE_Y * 2 - LONG)return true;
	return false;
}
/////////////控制蛇的移动////////////
void control_snake()
{
	char move_flag = 'D', flag;					//初始化蛇的移动方向为'D'
	BeginBatchDraw();
	while (true)
	{
		//对蛇的位置重新计算
		for (int i = snake_num - 1; i >= 1; i--)
		{
			snake[i] = snake[i - 1];
		}
		//设置一个键盘按键监测
		while (_kbhit())						//当用户有按键操作时进入循环
		{
			//用一个字符变量用来接收用户的按键操作
			flag = _getch();
			//对按键的有效性进行判断并分类
			if (flag == move_flag + 3 || flag == move_flag - 3 || flag == move_flag - 4 || flag == move_flag + 4 || flag == move_flag)break;  //甄别无效按键
			//对有效按键进行分类
			else
			{
				//根据按键重新设置蛇头移动标志变量
				switch (flag)
				{
				case 'w':
				case'W': move_flag = 'W'; break;
				case 'd':
				case'D': move_flag = 'D'; break;
				case 's':
				case'S': move_flag = 'S'; break;
				case 'a':
				case'A': move_flag = 'A'; break;
				}
			}
		}
		//判断蛇头的移动方向标志并重新计算蛇头
		switch (move_flag)
		{
		case'W':snake[ZERO].y -= LONG; break;
		case'S':snake[ZERO].y += LONG; break;
		case'A':snake[ZERO].x -= LONG; break;
		case'D':snake[ZERO].x += LONG; break;
		}
		//判断食物是否需要刷新 刷新之后蛇的长度加一
		if (check_food())
		{
			mciSendString("stop musics\\转向.mp3", NULL, ZERO, NULL);
			snake_num++;
			produce_food(); mciSendString("play musics\\转向.mp3", NULL, ZERO, NULL);
		}
		//判断蛇是否存活
		if (check_snake()) { mciSendString("play musics\\撞墙或撞到自己.mp3", NULL, ZERO, NULL); cleardevice(); mciSendString("play musics\\游戏结束音乐.mp3",NULL,NULL,NULL); system("pause"); break; }
		//判断游戏是否胜利
		if (snake_num >= 20)
		{
			mciSendString("stop musics\\游戏音乐.mp3 ", NULL, ZERO, NULL);
			cleardevice();
			mciSendString("play musics\\战斗胜利.mp3", NULL, ZERO, NULL);
			outtextxy(MIDDLE_X - 155, MIDDLE_Y - 30, "恭喜，您已经通关了!");
			FlushBatchDraw();
			system("pause");
			continue;
		}
		//贴背景
		putimage(290, LONG, &game_background);
		//重新画蛇和地图
		draw_food();
		draw_snake(move_flag);
		//画用户界面信息
		game_info();
		//完成贴图
;		FlushBatchDraw();
		Sleep(snake_spead);
		EndBatchDraw();
	}
}
