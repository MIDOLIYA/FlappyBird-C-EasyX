#include "FB_headfile.h"
#pragma comment(lib, "winmm.lib")

bool play()
{
	Picture picture;//定义游戏中用到图片
	Data data;//定义游戏中的数据

	initdata(data);//初始化数据

	picturedeal(picture);//加载游戏图片

						 //游戏开始界面实现
	gamestartmenu(picture, data);

	//游戏开始
	while (true)
	{
		birdct(data);//bird控制
		drawpicture(picture, data);//开始游戏画图
		collision(data);//在这里判断死亡加分情况
		if (!data.life)
			break;
		databack(data);//数据回环
		Sleep(16);
	}

	//得分面板
	scorepanl(picture, data);
	return 0;
}

int main()
{
	bool isfirst=true;
	initgraph(288, 512);   // 创建绘图窗口，大小为 288x512 像素
	while (true)
	{
		play();
	
	}
 	_getch();
	closegraph();
}
