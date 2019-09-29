#pragma once
#ifndef _FB_HEADFILE_H
#define _FB_HEADFILE_H
#include "stdio.h"
#include "stdlib.h"
#include "conio.h"
#include <time.h>
#include "windows.h"
#include <iostream>
#include <thread>
#include <graphics.h>

//bird����
struct Bird
{
	int birdhigh;
	int birdpicture;
	int UPnumber;
	int DOWNnumber;
	float down;
	float up;
	
};

//��Ϸ�е�ͼƬ
typedef struct Picture
{
	IMAGE birdpt[20][2];//birdͼƬ
	IMAGE bandpt[2];//�ϰ���ͼƬ
	IMAGE backgroundpt[2];//����ͼƬ
	IMAGE landpt;//�·������ƶ�����
	IMAGE big_num[10][2];//��ŵ�����
	IMAGE mid_num[10][2];//�к�����
	IMAGE sm_num[10][2];//С������
	IMAGE gametitle[2];//��Ϸ�ı���
	IMAGE getready[2];//׼���׶α���
	/*���еİ�ť�͵÷���� 
		0:button_play; 1:button_score; 
		2:tutorial.jpg; 3��score_panel*/
	IMAGE button[3][2];
	//IMAGE copyright[2];//��ʼ����İ�Ȩ����
	IMAGE medals[3][2];//����
	IMAGE gameover[2];//��Ϸ����
	IMAGE newscore;//���µĸ߷�
};

//��Ϸ����
typedef struct Data
{
	int landdata;//�·��ƶ�����
	//�ϰ��� 0���ϰ����x���꣬1�����ϰ����y���� 2���·��ϰ���y����
	int banddata[5][3];
	int nowscore;//���ε÷�
	int speed;//�ƶ��ٶ�
	int oldscore[3];//0��һ��1�ڶ���2����
	int scoreblock;//������
	bool life;//bird�Ƿ���
	Bird bird;//bird����
	int freshcount = 0;
};
void initdata(Data &data);//��ʼ����Ϸ����
void birdct(Data &data);// bird����
bool play();//��ʼ��Ϸ
void databack(Data &data);//���ݴ���
void gamestartmenu(Picture picture, Data data);//���ƿ�ʼ����
//void getread(Picture picture,Data data);//����getready����
void picturedeal(Picture &picture);//��ϷͼƬ������������ת�� n=1 �Ǽ���ͼƬ
void drawpicture(Picture picture, Data data);//������Ϸ����
void collision(Data &data);//��ײ
void scorepanl(Picture picture,Data &data);//�÷����
void changeband(Data &data, int n);//�ı��ϰ����Y������
using namespace std;
#endif

// _FB_HEADFILE_H