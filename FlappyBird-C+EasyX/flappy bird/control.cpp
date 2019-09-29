#include "FB_headfile.h"
#include <thread>
#include<time.h>

//��ʼ����Ϸ����
void initdata(Data &data)
{
	FILE *fp;
	errno_t err;
	data.landdata = 0;
	int i = 0;
	data.banddata[0][0] = 200;
	data.banddata[0][1] = -125;
	data.banddata[0][2] = 300;

	data.banddata[1][0] = 400;
	data.banddata[1][1] = -175;
	data.banddata[1][2] = 250;

	data.banddata[2][0] = 600;
	data.banddata[2][1] = -150;
	data.banddata[2][2] = 275;

	data.banddata[3][0] = 800;
	data.banddata[3][1] = -200;
	data.banddata[3][2] = 225;

	data.banddata[4][0] = 1000;
	data.banddata[4][1] = -150;
	data.banddata[4][2] = 275;

	/*for (int i = 0; i < 5; i++)
	{
		data.banddata[i][1] =-125;
		data.banddata[i][2] = 300;
	}*/

	data.nowscore = 0;
	   
	data.speed = 3;
	data.scoreblock = -1;

	//�ļ���������ȡoldscore
	err=fopen_s(&fp, "score.txt", "r");
	if (!err)
	{
		while (!feof(fp))
		{
			fscanf_s(fp,"%d", &data.oldscore[i++]);
		}
	}
	else
	{
		err = fopen_s(&fp, "score.txt", "w");
		for (; i < 3; i++)
		{
			fprintf(fp, "%d ", 0);
			data.oldscore[i] = 0;
		}
	}
	fclose(fp);

	data.bird.birdhigh = 255;
	data.bird.birdpicture = 0;
	data.bird.UPnumber = 0;
	data.bird.DOWNnumber = 0;
	data.bird.down = 0.2;
	data.bird.up = 0.1;
	data.life = true;
	
}

//bird����
void birdct(Data &data)
{
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)//�ո���Ƿ���
	{
		if (data.bird.birdhigh <= 0)
			;
		else
		{
			data.bird.birdhigh -= data.bird.up;
			data.bird.up += 0.1;//�������ٶ�
			data.bird.down = 0.1;//�½����ٶ�
			data.bird.UPnumber++;
			if (data.bird.UPnumber % 5 == 0)//����ǽӽ�1��ˢ��һ������
			{
				data.bird.birdpicture++;
				if (data.bird.birdpicture > 11)
					data.bird.birdpicture = 9;
			}
			/*__asm
			{
				mov eax,data.bird.birdhigh
				sub eax,data.bird.up
				mov data.bird.birdhigh,eax
				inc data.bird.up
				mov data.bird.down,0.1
				inc data.bird.number
				xor edx,edx
				mov eax,data.bird.number
				mov ebx,5
				div ebx
				cmp edx,0
				jne exit
				inc data.bird.birdhigh
				mov eax,data.bird.birdhigh
				cmp eax,3
				jb exit
				mov data.bird.birdhigh,0
				exit:
					nop
			}*/
		}

	}
	else
	{
		if (data.bird.birdhigh >= 360)
			;
		else
		{
			data.bird.birdhigh += data.bird.down;
			data.bird.down += 0.3;//�½����ٶ�

			data.bird.DOWNnumber++;
			if (data.bird.DOWNnumber % 2 == 0)//����ǽӽ�1��ˢ��һ������
			{
				if (data.bird.birdpicture <= 1 || data.bird.birdpicture == 16) {
					data.bird.birdpicture = 16;
				}
				else {
					if ((data.bird.birdpicture - 1) % 3 != 0)
						data.bird.birdpicture = (((data.bird.birdpicture / 3) * 3) + 1);
					else
						data.bird.birdpicture -= 3;
				}
			}
			//data.bird.birdpicture = 2;
			data.bird.up = 0.1;//�������ٶ�
		}
	}
}

//����ѭ��
void databack(Data &data)
{
	//�ϰ���ػ�
	for (int i = 0; i < 5; i++)
	{
		if (data.banddata[i][0] <= -50)
		{
			changeband(data, data.banddata[i][0]);
			data.banddata[i][0] = 950;
			/*__asm
			{
				mov esi,dword ptr [data]
				add esi,4
				mov edi,i
				imul edi,12
				mov dword ptr [esi+edi],900
			}*/
		}
			
		else
		{
			data.banddata[i][0] -= data.speed;
			/*__asm
			{
				mov esi,dword ptr [data]
				mov edi,i
				imul edi,12
				mov eax,dword ptr[esi + edi + 4]
				sub eax,dword ptr[esi + 44h]
				mov dword ptr[esi + edi + 4],eax
			}*/
		}
			
		
	}
	data.landdata -= data.speed;
	/*__asm
	{
		mov esi,dword ptr [data]
		mov eax,dword ptr [esi]
		sub eax,dword ptr [esi+44h]
		mov dword ptr [esi],eax
	}*/

	if (data.landdata <= -50)
	{
			data.landdata = 0;
		/*__asm
		{
			mov esi, dword ptr[data]
			mov dword ptr[esi], 0
		}*/
	}
}

//��Ϸ��ʼ�˵�
void gamestartmenu(Picture picture,Data data)
{
	int  number = 17, key = 0, a = 0, b = 0, c = 0, i = 0, score[3];
	//int scorekey = 0;
	int oldscore[3];
	float n = 0;
	MOUSEMSG msg;
	while (true)
	{
			
		BeginBatchDraw();

		SYSTEMTIME sys;
		GetLocalTime(&sys);
		if ((sys.wHour + 8) >= 17 || (sys.wHour + 8) <= 6)
			putimage(0, 0, &picture.backgroundpt[1]);
		else
			putimage(0, 0, &picture.backgroundpt[0]);

		putimage(0, 0, &picture.gametitle[0], SRCAND);
		putimage(0, 0, &picture.gametitle[1], SRCPAINT);

		putimage(115, 180, &picture.birdpt[number][0], SRCAND);
		putimage(115, 180, &picture.birdpt[number][1],SRCPAINT);

		//��ʼ��ť
		putimage(20, 340, &picture.button[0][0], SRCAND);
		putimage(20, 340, &picture.button[0][1], SRCPAINT);
		
		//���а�ť
		putimage(150, 340, &picture.button[1][0], SRCAND);
		putimage(150, 340, &picture.button[1][1], SRCPAINT);

		//�������а�
		//if (scorekey == 1)
		//{
		//	oldscore[0] = data.oldscore[0];
		//	oldscore[1] = data.oldscore[1];
		//	oldscore[2] = data.oldscore[2];

		//	//����һ��
		//	if (oldscore[0] == 0)
		//		putimage(203, 337, &picture.sm_num[0]);
		//	else
		//	{
		//		while (oldscore[0] != 0)
		//		{
		//			score[i++] = oldscore[0] % 10;
		//			oldscore[0] = oldscore[0] / 10;
		//			/*__asm
		//			{
		//				mov eax,dword ptr oldscore[0]
		//				mov ebx,10
		//				xor edx,edx
		//				div ebx
		//				mov esi,i
		//				mov dword ptr score[esi*4],edx
		//				inc i
		//				mov dword ptr oldscore[0],eax
		//			}*/
		//		}
		//		//λ��
		//		switch (i)
		//		{
		//			case 1:
		//				putimage(203, 337, &picture.sm_num[score[0]]);
		//				break;
		//			case 2:
		//				putimage(197, 337, &picture.sm_num[score[1]]);
		//				putimage(212, 337, &picture.sm_num[score[0]]);
		//				break;
		//			case 3:
		//				putimage(190, 335, &picture.sm_num[score[2]]);
		//				putimage(203, 335, &picture.sm_num[score[1]]);
		//				putimage(216, 335, &picture.sm_num[score[0]]);
		//				break;
		//		}
		//	}
		//	i = 0;//��λ
		//	//���ڶ���
		//	if (oldscore[1] == 0)
		//		putimage(230, 352, &picture.sm_num[0]);
		//	else
		//	{
		//		while (oldscore[1] != 0)
		//		{
		//			score[i++] = oldscore[1] % 10;
		//			oldscore[1] = oldscore[1] / 10;
		//			/*__asm
		//			{
		//				mov eax, dword ptr oldscore[4]
		//				mov ebx, 10
		//				xor edx, edx
		//				div ebx
		//				mov esi, i
		//				mov dword ptr score[esi * 4], edx
		//				inc i
		//				mov dword ptr oldscore[4], eax
		//			}*/
		//		}
		//		//λ��
		//		switch (i)
		//		{
		//			case 1:
		//				putimage(230, 352, &picture.sm_num[score[0]]);
		//				break;
		//			case 2:
		//				putimage(230, 352, &picture.sm_num[score[1]]);
		//				putimage(243, 352, &picture.sm_num[score[0]]);
		//				break;
		//			case 3:
		//				putimage(230, 352, &picture.sm_num[score[2]]);
		//				putimage(243, 352, &picture.sm_num[score[1]]);
		//				putimage(256, 352, &picture.sm_num[score[0]]);
		//				break;
		//		}
		//	}
		//	i = 0;//��λ
		//	//��������
		//	if (oldscore[2] == 0)
		//		putimage(185, 356, &picture.sm_num[0]);
		//	else
		//	{
		//		while (oldscore[2] != 0)
		//		{
		//			score[i++] =oldscore[2] % 10;
		//			oldscore[2] =oldscore[2] / 10;
		//			/*__asm
		//			{
		//				mov eax, dword ptr oldscore[8]
		//				mov ebx, 10
		//				xor edx, edx
		//				div ebx
		//				mov esi, i
		//				mov dword ptr score[esi * 4], edx
		//				inc i
		//				mov dword ptr oldscore[8], eax
		//			}*/
		//		}
		//		//λ��
		//		switch (i)
		//		{
		//			case 1:
		//				putimage(185, 356, &picture.sm_num[score[0]]);
		//				break;
		//			case 2:
		//				putimage(172, 356, &picture.sm_num[score[1]]);
		//				putimage(185, 356, &picture.sm_num[score[0]]);
		//				break;
		//			case 3:
		//				putimage(159, 356, &picture.sm_num[score[2]]);
		//				putimage(172, 356, &picture.sm_num[score[1]]);
		//				putimage(185, 356, &picture.sm_num[score[0]]);
		//				break;
		//		}
		//	}
		//	i = 0;//��λ
		//}

		putimage(n, 400, &picture.landpt);

		//��Ȩ������ɾ�����滻
		//putimage(70, 425, &picture.copyright[0], SRCAND);
		//putimage(70, 425, &picture.copyright[1], SRCPAINT);

		FlushBatchDraw();//ִ��δ��ɵĻ�������
		EndBatchDraw();//�����������ƣ���ִ��δ��ɵĻ�������

		//��ͷ����
		//����bird��ͼƬ�л�
		//����Ϸ��Ӱ�죬����ɾ��
		key++;
		if (key % 200 == 0)
			number >= 19 ? number = 17 : number++;
		n -= 0.05;
		if (n <= -50)
			n = 0;
		

		//float a = 0.05;
		////int b = -50;
		//__asm
		//{
		//	inc key
		//	xor edx,edx
		//	mov eax,key
		//	mov ebx,200
		//	div ebx
		//	cmp edx,0
		//	jne opn
		//	mov eax,number
		//	cmp eax,2
		//	jb incnum
		//	mov number,0
		//	jmp opn

		//	incnum:
		//	inc number

		//	opn:
		//	finit
		//	fld n
		//	fsub a
		//	fstp n
		//	
		//	exit:

		//}
		//if (n <= -50)
			//n = 0;

		//����ѡ��
		if (MouseHit())
		{
			msg = GetMouseMsg();//��ȡһ�������Ϣ�����û�о͵ȴ�
			if (msg.uMsg == WM_LBUTTONDOWN)//��ǰ�����Ϣ=��������
				if (msg.x >= 20 && msg.x <= 136 && msg.y >= 340 && msg.y <= 410)//����ڿ�ʼ��ť��Χ��
					break;

				else if (msg.x >= 150 && msg.x <= 266 && msg.y >= 340 && msg.y <= 410)//��������а�ť��Χ��
				{
					while (true) {
						BeginBatchDraw();

						SYSTEMTIME sys;
						GetLocalTime(&sys);
						if ((sys.wHour + 8) >= 17 || (sys.wHour + 8) <= 6)
							putimage(0, 0, &picture.backgroundpt[1]);
						else
							putimage(0, 0, &picture.backgroundpt[0]);

						putimage(50, 100, &picture.medals[0][0], SRCAND);
						putimage(50, 100, &picture.medals[0][1], SRCPAINT);

						putimage(50, 170, &picture.medals[1][0], SRCAND);
						putimage(50, 170, &picture.medals[1][1], SRCPAINT);
						
						putimage(50, 230, &picture.medals[2][0], SRCAND);
						putimage(50, 230, &picture.medals[2][1], SRCPAINT);

						oldscore[0] = data.oldscore[0];
						oldscore[1] = data.oldscore[1];
						oldscore[2] = data.oldscore[2];

						if (oldscore[0] == 0) {
							putimage(160, 108, &picture.big_num[0][0], SRCAND);
							putimage(160, 108, &picture.big_num[0][1], SRCPAINT);
						}
						else
						{
							while (oldscore[0] != 0)
							{
								score[i++] = oldscore[0] % 10;
								oldscore[0] = oldscore[0] / 10;
								/*__asm
								{
									mov eax,dword ptr oldscore[0]
									mov ebx,10
									xor edx,edx
									div ebx
									mov esi,i
									mov dword ptr score[esi*4],edx
									inc i
									mov dword ptr oldscore[0],eax
								}*/
							}
							//λ��
							switch (i)
							{
								case 1:
									putimage(160, 108, &picture.big_num[score[0]][0], SRCAND);
									putimage(160, 108, &picture.big_num[score[0]][1], SRCPAINT);
									break;
								case 2:
									putimage(160, 108, &picture.big_num[score[1]][0], SRCAND);
									putimage(160, 108, &picture.big_num[score[1]][1], SRCPAINT);

									putimage(180, 108, &picture.big_num[score[0]][0], SRCAND);
									putimage(180, 108, &picture.big_num[score[0]][1], SRCPAINT);
									break;
								case 3:
									putimage(160, 108, &picture.big_num[score[2]][0], SRCAND);
									putimage(160, 108, &picture.big_num[score[2]][1], SRCPAINT);

									putimage(180, 108, &picture.big_num[score[1]][0], SRCAND);
									putimage(180, 108, &picture.big_num[score[1]][1], SRCPAINT);

									putimage(200, 108, &picture.big_num[score[0]][0], SRCAND);
									putimage(200, 108, &picture.big_num[score[0]][1], SRCPAINT);
									break;
							}
						}
						i = 0;//��λ
						//���ڶ���
						if (oldscore[1] == 0) {
							putimage(160, 185, &picture.mid_num[0][0], SRCAND);
							putimage(160, 185, &picture.mid_num[0][1], SRCPAINT);
						}
						else
						{
							while (oldscore[1] != 0)
							{
								score[i++] = oldscore[1] % 10;
								oldscore[1] = oldscore[1] / 10;
								/*__asm
								{
									mov eax, dword ptr oldscore[4]
									mov ebx, 10
									xor edx, edx
									div ebx
									mov esi, i
									mov dword ptr score[esi * 4], edx
									inc i
										mov dword ptr oldscore[4], eax
								}*/
							}
							//λ��
							switch (i)
							{
							case 1:
								putimage(160, 185, &picture.mid_num[score[0]][0], SRCAND);
								putimage(160, 185, &picture.mid_num[score[0]][1], SRCPAINT);
								break;
							case 2:
								putimage(160, 185, &picture.mid_num[score[1]][0], SRCAND);
								putimage(160, 185, &picture.mid_num[score[1]][1], SRCPAINT);

								putimage(174, 185, &picture.mid_num[score[0]][0], SRCAND);
								putimage(174, 185, &picture.mid_num[score[0]][1], SRCPAINT);
								break;
							case 3:
								putimage(160, 185, &picture.mid_num[score[2]][0], SRCAND);
								putimage(160, 185, &picture.mid_num[score[2]][1], SRCPAINT);

								putimage(174, 185, &picture.mid_num[score[1]][0], SRCAND);
								putimage(174, 185, &picture.mid_num[score[1]][1], SRCPAINT);

								putimage(188, 185, &picture.mid_num[score[0]][0], SRCAND);
								putimage(188, 185, &picture.mid_num[score[0]][1], SRCPAINT);
								break;
							}
						}
						i = 0;//��λ
						//��������
						if (oldscore[2] == 0) {
							putimage(160, 245, &picture.sm_num[0][0], SRCAND);
							putimage(160, 245, &picture.sm_num[0][1], SRCPAINT);
						}
						else
						{
							while (oldscore[2] != 0)
							{
								score[i++] =oldscore[2] % 10;
								oldscore[2] =oldscore[2] / 10;
								/*__asm
								{
									mov eax, dword ptr oldscore[8]
									mov ebx, 10
									xor edx, edx
									div ebx
									mov esi, i
									mov dword ptr score[esi * 4], edx
									inc i
									mov dword ptr oldscore[8], eax
								}*/
							}
							//λ��
							switch (i)
							{
								case 1:
									putimage(160, 245, &picture.sm_num[score[0]][0], SRCAND);
									putimage(160, 245, &picture.sm_num[score[0]][1], SRCPAINT);
									break;
								case 2:
									putimage(160, 245, &picture.sm_num[score[1]][0], SRCAND);
									putimage(160, 245, &picture.sm_num[score[1]][1], SRCPAINT);

									putimage(173, 245, &picture.sm_num[score[0]][0], SRCAND);
									putimage(173, 245, &picture.sm_num[score[0]][1], SRCPAINT);
									break;
								case 3:
									putimage(160, 245, &picture.sm_num[score[2]][0], SRCAND);
									putimage(160, 245, &picture.sm_num[score[2]][1], SRCPAINT);

									putimage(173, 245, &picture.sm_num[score[1]][0], SRCAND);
									putimage(173, 245, &picture.sm_num[score[1]][1], SRCPAINT);

									putimage(184, 245, &picture.sm_num[score[0]][0], SRCAND);
									putimage(184, 245, &picture.sm_num[score[0]][1], SRCPAINT);
									break;
							}
						}
						i = 0;//��λ
						
						putimage(110, 350, &picture.button[2][0]);

						putimage(n, 400, &picture.landpt);

						FlushBatchDraw();
						EndBatchDraw();

						if (MouseHit()) {
							msg = GetMouseMsg();
							if (msg.uMsg == WM_LBUTTONDOWN)
								if (msg.x >= 110 && msg.x <= 190 && msg.y >= 350 && msg.y <= 378)
									break;
						}
					}
					
					//if (scorekey == 0)
					//	scorekey = 1;//��ʾ����
					//else
					//	scorekey = 0;//��������

					/*__asm
					{
						mov eax, scorekey
						cmp eax, 0
						jne ske0
						mov scorekey,1
						jmp exit2
						ske0:mov scorekey,0
						exit2:nop
					}*/
				}
		}
	}
}

//������ϷͼƬ
void picturedeal(Picture &picture)
{
		//������
		loadimage(&picture.birdpt[0][0], "image\\mybird4_.jpg", 0, 0, false);
		loadimage(&picture.birdpt[0][1], "image\\mybird4.jpg", 0, 0, false);

		loadimage(&picture.birdpt[1][0], "image\\mybird4_1_.jpg", 0, 0, false);
		loadimage(&picture.birdpt[1][1], "image\\mybird4_1.jpg", 0, 0, false);

		loadimage(&picture.birdpt[2][0], "image\\mybird4_2_.jpg", 0, 0, false);
		loadimage(&picture.birdpt[2][1], "image\\mybird4_2.jpg", 0, 0, false);

		loadimage(&picture.birdpt[3][0], "image\\mybird3_.jpg", 0, 0, false);
		loadimage(&picture.birdpt[3][1], "image\\mybird3.jpg", 0, 0, false);

		loadimage(&picture.birdpt[4][0], "image\\mybird3_1_.jpg", 0, 0, false);
		loadimage(&picture.birdpt[4][1], "image\\mybird3_1.jpg", 0, 0, false);

		loadimage(&picture.birdpt[5][0], "image\\mybird3_2_.jpg", 0, 0, false);
		loadimage(&picture.birdpt[5][1], "image\\mybird3_2.jpg", 0, 0, false);

		loadimage(&picture.birdpt[6][0], "image\\mybird6_.jpg", 0, 0, false);
		loadimage(&picture.birdpt[6][1], "image\\mybird6.jpg", 0, 0, false);

		loadimage(&picture.birdpt[7][0], "image\\mybird6_1_.jpg", 0, 0, false);
		loadimage(&picture.birdpt[7][1], "image\\mybird6_1.jpg", 0, 0, false);

		loadimage(&picture.birdpt[8][0], "image\\mybird6_2_.jpg", 0, 0, false);
		loadimage(&picture.birdpt[8][1], "image\\mybird6_2.jpg", 0, 0, false);

		loadimage(&picture.birdpt[9][0], "image\\mybird2_.jpg", 0, 0, false);
		loadimage(&picture.birdpt[9][1], "image\\mybird2.jpg", 0, 0, false);

		loadimage(&picture.birdpt[10][0], "image\\mybird2_1_.jpg", 0, 0, false);
		loadimage(&picture.birdpt[10][1], "image\\mybird2_1.jpg", 0, 0, false);

		loadimage(&picture.birdpt[11][0], "image\\mybird2_2_.jpg", 0, 0, false);
		loadimage(&picture.birdpt[11][1], "image\\mybird2_2.jpg", 0, 0, false);

		/*loadimage(&picture.birdpt[12][0], "image\\mybird1_.jpg", 0, 0, false);
		loadimage(&picture.birdpt[12][1], "image\\mybird1.jpg", 0, 0, false);

		loadimage(&picture.birdpt[13][0], "image\\mybird1_1_.jpg", 0, 0, false);
		loadimage(&picture.birdpt[13][1], "image\\mybird1_1.jpg", 0, 0, false);

		loadimage(&picture.birdpt[14][0], "image\\mybird1_2_.jpg", 0, 0, false);
		loadimage(&picture.birdpt[14][1], "image\\mybird1_2.jpg", 0, 0, false);*/

		loadimage(&picture.birdpt[15][0], "image\\mybird5_.jpg", 0, 0, false);
		loadimage(&picture.birdpt[15][1], "image\\mybird5.jpg", 0, 0, false);

		loadimage(&picture.birdpt[16][0], "image\\mybird5_1_.jpg", 0, 0, false);
		loadimage(&picture.birdpt[16][1], "image\\mybird5_1.jpg", 0, 0, false);

		loadimage(&picture.birdpt[17][0], "image\\bird2_0_.jpg", 0, 0, false);
		loadimage(&picture.birdpt[17][1], "image\\bird2_0.jpg", 0, 0, false);

		loadimage(&picture.birdpt[18][0], "image\\bird2_1_.jpg", 0, 0, false);
		loadimage(&picture.birdpt[18][1], "image\\bird2_1.jpg", 0, 0, false);

		loadimage(&picture.birdpt[19][0], "image\\bird2_2_.jpg", 0, 0, false);
		loadimage(&picture.birdpt[19][1], "image\\bird2_2.jpg", 0, 0, false);

		//����ˮ��
		//for (int i = 0; i < 5; i++)
		//{
			loadimage(&picture.bandpt[0], "image\\pipe_down.jpg", 0, 0, false);
			loadimage(&picture.bandpt[1], "image\\pipe_up.jpg", 0, 0, false);
		//}

		//������ͼƬ
		loadimage(&picture.backgroundpt[0], "image\\bg_day.jpg", 0, false);
		loadimage(&picture.backgroundpt[1], "image\\bg_night.jpg", 0, false);

		//���ص���
		loadimage(&picture.landpt, "image\\land.jpg", 0, 0, false);

		//���ش������
		loadimage(&picture.big_num[0][0], "image\\big_num0_.jpg", 0, 0, false);
		loadimage(&picture.big_num[0][1], "image\\big_num0.jpg", 0, 0, false);

		loadimage(&picture.big_num[1][0], "image\\big_num1_.jpg", 0, 0, false);
		loadimage(&picture.big_num[1][1], "image\\big_num1.jpg", 0, 0, false);

		loadimage(&picture.big_num[2][0], "image\\big_num2_.jpg", 0, 0, false);
		loadimage(&picture.big_num[2][1], "image\\big_num2.jpg", 0, 0, false);

		loadimage(&picture.big_num[3][0], "image\\big_num3_.jpg", 0, 0, false);
		loadimage(&picture.big_num[3][1], "image\\big_num3.jpg", 0, 0, false);

		loadimage(&picture.big_num[4][0], "image\\big_num4_.jpg", 0, 0, false);
		loadimage(&picture.big_num[4][1], "image\\big_num4.jpg", 0, 0, false);

		loadimage(&picture.big_num[5][0], "image\\big_num5_.jpg", 0, 0, false);
		loadimage(&picture.big_num[5][1], "image\\big_num5.jpg", 0, 0, false);

		loadimage(&picture.big_num[6][0], "image\\big_num6_.jpg", 0, 0, false);
		loadimage(&picture.big_num[6][1], "image\\big_num6.jpg", 0, 0, false);

		loadimage(&picture.big_num[7][0], "image\\big_num7_.jpg", 0, 0, false);
		loadimage(&picture.big_num[7][1], "image\\big_num7.jpg", 0, 0, false);

		loadimage(&picture.big_num[8][0], "image\\big_num8_.jpg", 0, 0, false);
		loadimage(&picture.big_num[8][1], "image\\big_num8.jpg", 0, 0, false);

		loadimage(&picture.big_num[9][0], "image\\big_num9_.jpg", 0, 0, false);
		loadimage(&picture.big_num[9][1], "image\\big_num9.jpg", 0, 0, false);
		
		//�����к�����
		loadimage(&picture.mid_num[0][0], "image\\mid_num0_.jpg", 0, 0, false);
		loadimage(&picture.mid_num[0][1], "image\\mid_num0.jpg", 0, 0, false);

		loadimage(&picture.mid_num[1][0], "image\\mid_num1_.jpg", 0, 0, false);
		loadimage(&picture.mid_num[1][1], "image\\mid_num1.jpg", 0, 0, false);

		loadimage(&picture.mid_num[2][0], "image\\mid_num2_.jpg", 0, 0, false);
		loadimage(&picture.mid_num[2][1], "image\\mid_num2.jpg", 0, 0, false);

		loadimage(&picture.mid_num[3][0], "image\\mid_num3_.jpg", 0, 0, false);
		loadimage(&picture.mid_num[3][1], "image\\mid_num3.jpg", 0, 0, false);

		loadimage(&picture.mid_num[4][0], "image\\mid_num4_.jpg", 0, 0, false);
		loadimage(&picture.mid_num[4][1], "image\\mid_num4.jpg", 0, 0, false);

		loadimage(&picture.mid_num[5][0], "image\\mid_num5_.jpg", 0, 0, false);
		loadimage(&picture.mid_num[5][1], "image\\mid_num5.jpg", 0, 0, false);

		loadimage(&picture.mid_num[6][0], "image\\mid_num6_.jpg", 0, 0, false);
		loadimage(&picture.mid_num[6][1], "image\\mid_num6.jpg", 0, 0, false);

		loadimage(&picture.mid_num[7][0], "image\\mid_num7_.jpg", 0, 0, false);
		loadimage(&picture.mid_num[7][1], "image\\mid_num7.jpg", 0, 0, false);

		loadimage(&picture.mid_num[8][0], "image\\mid_num8_.jpg", 0, 0, false);
		loadimage(&picture.mid_num[8][1], "image\\mid_num8.jpg", 0, 0, false);

		loadimage(&picture.mid_num[9][0], "image\\mid_num9_.jpg", 0, 0, false);
		loadimage(&picture.mid_num[9][1], "image\\mid_num9.jpg", 0, 0, false);

		//����С������
		loadimage(&picture.sm_num[0][0], "image\\number_context_00_.jpg", 14, 16, false);
		loadimage(&picture.sm_num[0][1], "image\\number_context_00.jpg", 14, 16, false);

		loadimage(&picture.sm_num[1][0], "image\\number_context_01_.jpg", 14, 16, false);
		loadimage(&picture.sm_num[1][1], "image\\number_context_01.jpg", 14, 16, false);
		
		loadimage(&picture.sm_num[2][0], "image\\number_context_02_.jpg", 14, 16, false);
		loadimage(&picture.sm_num[2][1], "image\\number_context_02.jpg", 14, 16, false);
		
		loadimage(&picture.sm_num[3][0], "image\\number_context_03_.jpg", 14, 16, false);
		loadimage(&picture.sm_num[3][1], "image\\number_context_03.jpg", 14, 16, false);
		
		loadimage(&picture.sm_num[4][0], "image\\number_context_04_.jpg", 14, 16, false);
		loadimage(&picture.sm_num[4][1], "image\\number_context_04.jpg", 14, 16, false);
		
		loadimage(&picture.sm_num[5][0], "image\\number_context_05_.jpg", 14, 16, false);
		loadimage(&picture.sm_num[5][1], "image\\number_context_05.jpg", 14, 16, false);
		
		loadimage(&picture.sm_num[6][0], "image\\number_context_06_.jpg", 14, 16, false);
		loadimage(&picture.sm_num[6][1], "image\\number_context_06.jpg", 14, 16, false);
		
		loadimage(&picture.sm_num[7][0], "image\\number_context_07_.jpg", 14, 16, false);
		loadimage(&picture.sm_num[7][1], "image\\number_context_07.jpg", 14, 16, false);
		
		loadimage(&picture.sm_num[8][0], "image\\number_context_08_.jpg", 14, 16, false);
		loadimage(&picture.sm_num[8][1], "image\\number_context_08.jpg", 14, 16, false);
		
		loadimage(&picture.sm_num[9][0], "image\\number_context_09_.jpg", 14, 16, false);
		loadimage(&picture.sm_num[9][1], "image\\number_context_09.jpg", 14, 16, false);

		//������Ϸ���� 
		loadimage(&picture.gametitle[0], "image\\title_.jpg", 0, 0, false);
		loadimage(&picture.gametitle[1], "image\\title.jpg", 0, 0, false);

		//����׼��ͼƬ
		loadimage(&picture.getready[0], "image\\text_ready_.jpg", 0, 0, false);
		loadimage(&picture.getready[1], "image\\text_ready.jpg", 0, 0, false);

		//���ذ�ť
		loadimage(&picture.button[0][0], "image\\button_play_.jpg", 0, 0, false);
		loadimage(&picture.button[0][1], "image\\button_play.jpg", 0, 0, false);

		loadimage(&picture.button[1][0], "image\\button_score_.jpg", 0, 0, false);
		loadimage(&picture.button[1][1], "image\\button_score.jpg", 0, 0, false);

		loadimage(&picture.button[2][0], "image\\button_menu.jpg", 0, 0, false);

		//loadimage(&picture.button[3][0], "image\\score_panel_.jpg", 0, 0, false);
		//loadimage(&picture.button[3][1], "image\\score_panel.jpg", 0, 0, false);

		//����copyrightͼƬ
		//loadimage(&picture.copyright[0], "image\\brand_copyright_.jpg", 0, 0, false);
		//loadimage(&picture.copyright[1], "image\\brand_copyright.jpg", 0, 0, false);

		//���ؽ���ͼƬ
		//loadimage(&picture.medals[0][0], "image\\medals_0_.jpg", 0, 0, false);
		//loadimage(&picture.medals[0][1], "image\\medals_0.jpg", 0, 0, false);

		loadimage(&picture.medals[0][0], "image\\medals_1_.jpg", 60, 60, false);
		loadimage(&picture.medals[0][1], "image\\medals_1.jpg", 60, 60, false);

		loadimage(&picture.medals[1][0], "image\\medals_2_.jpg", 50, 50, false);
		loadimage(&picture.medals[1][1], "image\\medals_2.jpg", 50, 50, false);

		loadimage(&picture.medals[2][0], "image\\medals_3_.jpg", 0, 0, false);
		loadimage(&picture.medals[2][1], "image\\medals_3.jpg", 0, 0, false);

		//���ء���Ϸ������ͼƬ
		loadimage(&picture.gameover[0], "image\\text_game_over_.jpg", 0, 0, false);
		loadimage(&picture.gameover[1], "image\\text_game_over.jpg", 0, 0, false);

}

//������Ϸ����
void drawpicture(Picture picture, Data data)
{
	int score[3],i=0;//���������������

	BeginBatchDraw();

	SYSTEMTIME sys;
	GetLocalTime(&sys);
	if ((sys.wHour + 8) >= 17 || (sys.wHour + 8) <= 6)
		putimage(0, 0, &picture.backgroundpt[1]);
	else
		putimage(0, 0, &picture.backgroundpt[0]);

	//����ˮ��
	for (int i = 0; i < 5; i++)
	{
		putimage(data.banddata[i][0], data.banddata[i][1], &picture.bandpt[0]);
		putimage(data.banddata[i][0], data.banddata[i][2],&picture.bandpt[1]);
	}

	//���Ʒ���
	{
		if (data.nowscore > 999)//��������999
		{
			putimage(120, 70, &picture.big_num[9][0], SRCAND);
			putimage(120, 70, &picture.big_num[9][1], SRCPAINT);

			putimage(140, 70, &picture.big_num[9][0], SRCAND);
			putimage(140, 70, &picture.big_num[9][1], SRCPAINT);

			putimage(160, 70, &picture.big_num[9][0], SRCAND);
			putimage(160, 70, &picture.big_num[9][1], SRCPAINT);
		}
		else
		{
			if (data.nowscore == 0)
			{
				putimage(144, 70, &picture.big_num[0][0], SRCAND);
				putimage(144, 70, &picture.big_num[0][1], SRCPAINT);
			}
			else
			{
				while (data.nowscore != 0)
				{
					score[i++] = data.nowscore % 10;
					data.nowscore = data.nowscore / 10;
				}
				switch (i)
				{
				case 1:
					putimage(144, 70, &picture.big_num[score[0]][0], SRCAND);
					putimage(144, 70, &picture.big_num[score[0]][1], SRCPAINT);
					break;
				case 2:
					putimage(132, 70, &picture.big_num[score[1]][0], SRCAND);
					putimage(132, 70, &picture.big_num[score[1]][1], SRCPAINT);

					putimage(154, 70, &picture.big_num[score[0]][0], SRCAND);
					putimage(154, 70, &picture.big_num[score[0]][1], SRCPAINT);
					break;
				case 3:
					putimage(120, 70, &picture.big_num[score[2]][0], SRCAND);
					putimage(120, 70, &picture.big_num[score[2]][1], SRCPAINT);

					putimage(140, 70, &picture.big_num[score[1]][0], SRCAND);
					putimage(140, 70, &picture.big_num[score[1]][1], SRCPAINT);

					putimage(160, 70, &picture.big_num[score[0]][0], SRCAND);
					putimage(160, 70, &picture.big_num[score[0]][1], SRCPAINT);
					break;
				}
			}
		}
	}
	
	putimage(data.landdata, 400, &picture.landpt);

	putimage(50, data.bird.birdhigh, &picture.birdpt[data.bird.birdpicture][0],SRCAND);
	putimage(50, data.bird.birdhigh, &picture.birdpt[data.bird.birdpicture][1],SRCPAINT);

	FlushBatchDraw();
	EndBatchDraw();
}

//�÷ֻ���ײ
void collision(Data &data)
{
	bool first = false, second = false, third = false;
	
	if (data.bird.birdhigh >= 360)//����������
		data.life = false;
	else
	{
		int i = 0;
		for (; i < 5; i++)
		{
			//birdͼƬ���� x=50,y=data.bird.birdhigh,r=13
			//�ϰ������y=320
			if (data.banddata[i][0] < 110 && data.banddata[i][0]>-20)//�ϰ�������жϷ�Χ
			{
				//Բ��X,Y
				int rx = 50 + 24;
				int ry = data.bird.birdhigh + 24;

				//�Ϸ��ϰ���ײ�X��Y
				int bux = data.banddata[i][0];
				int buy = data.banddata[i][1] + 320;//�ϰ����·��߽� //320Ϊˮ�ܳ���

				//�·��ϰ��ﶥ��X��Y
				int bdx = data.banddata[i][0];
				int bdy = data.banddata[i][2];

				float du = 0, dd = 0;

				if (ry >  buy && ry < bdy)//bird���ϰ����м䣺//����㵽�����
				{
					if (rx < bux || rx>(bux + 52))//bird��Բ��û����з��� //52Ϊˮ�ܵĿ��
					{
						if (rx > bux && data.scoreblock != i)//�÷�
						{
							data.scoreblock = i;
							data.nowscore++;
						}
						du = sqrt(pow((rx - bux), 2) + pow((ry - buy), 2));
						dd = sqrt(pow((rx - bdx), 2) + pow((ry - bdy), 2));
						if ((du - 13) <= 0 || (dd - 13) <= 0)
							data.life = false;
					}
					else//bird����з���
					{
						du = sqrt(pow(ry - buy, 2));
						dd = sqrt(pow(ry - bdy, 2));
						if ((du - 13) <= 0 || (dd - 13) <= 0)
							data.life = false;
					}
				}
				else if ( rx < data.banddata[i][0] && (rx + 13) >= data.banddata[i][0] )
				{
						data.life = false;
				}
				break;
			}
		}
	}
}

//�÷����
void scorepanl(Picture picture, Data &data)
{
	FILE *fp;
	errno_t err;
	int score[3],i=0, nowscore=data.nowscore,oldscore=data.oldscore[0], scorekey = 0;
	int medalscore = data.oldscore[0];
	bool isnewrecored=false,datachange=true;
	//int medal = -1;

	MOUSEMSG msg;

	while (true)
	{
		BeginBatchDraw();

		SYSTEMTIME sys;
		GetLocalTime(&sys);
		if ((sys.wHour + 8) >= 17 || (sys.wHour + 8) <= 6)
			putimage(0, 0, &picture.backgroundpt[1]);
		else
			putimage(0, 0, &picture.backgroundpt[0]);

		//ʧ�ܽ���
		putimage(0, 0, &picture.gameover[0], SRCAND);
		putimage(0, 0, &picture.gameover[1], SRCPAINT);
		   
		//�÷����
		//putimage(25, 180, &picture.button[3][0], SRCAND);
		//putimage(25, 180, &picture.button[3][1], SRCPAINT);

		//���Ʒ���������
		{
			//nowscore���Ʒ���
			if (data.nowscore > 999)
			{
				putimage(194, 215, &picture.mid_num[9][0], SRCAND);
				putimage(194, 215, &picture.mid_num[9][1], SRCPAINT);

				putimage(207, 215, &picture.mid_num[9][0], SRCAND);
				putimage(207, 215, &picture.mid_num[9][1], SRCPAINT);

				putimage(220, 215, &picture.mid_num[9][0], SRCAND);
				putimage(220, 215, &picture.mid_num[9][1], SRCPAINT);
			}
			else
			{
				if (data.nowscore == 0)
				{
					putimage(220, 215, &picture.mid_num[0][0], SRCAND);
					putimage(220, 215, &picture.mid_num[0][1], SRCPAINT);
				}
				else
				{
					i = 0;//��λ
					nowscore = data.nowscore;
					while (nowscore != 0)
					{
						score[i++] = nowscore % 10;
						nowscore = nowscore / 10;
					}
					switch (i)
					{
					case 1:
						putimage(220, 215, &picture.mid_num[score[0]][0], SRCAND);
						putimage(220, 215, &picture.mid_num[score[0]][1], SRCPAINT);
						break;
					case 2:
						putimage(207, 215, &picture.mid_num[score[1]][0], SRCAND);
						putimage(207, 215, &picture.mid_num[score[1]][1], SRCPAINT);

						putimage(220, 215, &picture.mid_num[score[0]][0], SRCAND);
						putimage(220, 215, &picture.mid_num[score[0]][1], SRCPAINT);
						break;
					case 3:
						putimage(194, 215, &picture.mid_num[score[2]][0], SRCAND);
						putimage(194, 215, &picture.mid_num[score[2]][1], SRCPAINT);

						putimage(207, 215, &picture.mid_num[score[1]][0], SRCAND);
						putimage(207, 215, &picture.mid_num[score[1]][1], SRCPAINT);

						putimage(220, 215, &picture.mid_num[score[0]][0], SRCAND);
						putimage(220, 215, &picture.mid_num[score[0]][1], SRCPAINT);
						break;
					}
				}
			}

			//�����������ж�һ���Ƿ����¼�¼
			/*for(int i=0;i<3;i++)
				data.oldscore[0] = 0;*/
			if (datachange)
			{
				if (data.nowscore > data.oldscore[0])
				{
					data.oldscore[2] = data.oldscore[1];
					data.oldscore[1] = data.oldscore[0];
					data.oldscore[0] = data.nowscore;
					isnewrecored = true;

				}
				else if (data.nowscore > data.oldscore[1])
				{
					data.oldscore[2] = data.oldscore[1];
					data.oldscore[1] = data.nowscore;
				}
				else if (data.nowscore > data.oldscore[2])
					data.oldscore[1] = data.nowscore;
				datachange = false;
			}

			//������߼�¼��ֱ�ӻ���nowscore[3]���������oldscore[0]
			if (isnewrecored)
			{
				switch (i)
				{
				case 1:
					putimage(220, 255, &picture.mid_num[score[0]][0], SRCAND);
					putimage(220, 255, &picture.mid_num[score[0]][1], SRCPAINT);
					break;
				case 2:
					putimage(207, 255, &picture.mid_num[score[1]][0], SRCAND);
					putimage(207, 255, &picture.mid_num[score[1]][1], SRCPAINT);

					putimage(220, 255, &picture.mid_num[score[0]][0], SRCAND);
					putimage(220, 255, &picture.mid_num[score[0]][1], SRCPAINT);
					break;
				case 3:
					putimage(194, 255, &picture.mid_num[score[2]][0], SRCAND);
					putimage(194, 255, &picture.mid_num[score[2]][1], SRCPAINT);

					putimage(207, 255, &picture.mid_num[score[1]][0], SRCAND);
					putimage(207, 255, &picture.mid_num[score[1]][1], SRCPAINT);

					putimage(220, 255, &picture.mid_num[score[0]][0], SRCAND);
					putimage(220, 255, &picture.mid_num[score[0]][1], SRCPAINT);
					break;
				}
				putimage(160, 257, &picture.newscore);
			}
			else
			{
				oldscore = data.oldscore[0];
				i = 0;//��λ
				if (oldscore == 0)
				{
					putimage(220, 255, &picture.mid_num[0][0], SRCAND);
					putimage(220, 255, &picture.mid_num[0][1], SRCPAINT);
				}
				else
				{
					while (oldscore != 0)
					{
						score[i++] = oldscore % 10;
						oldscore = oldscore / 10;
					}
					//λ��
					switch (i)
					{
					case 1:
						putimage(220, 255, &picture.mid_num[score[0]][0], SRCAND);
						putimage(220, 255, &picture.mid_num[score[0]][1], SRCPAINT);
						break;
					case 2:
						putimage(207, 255, &picture.mid_num[score[1]][0], SRCAND);
						putimage(207, 255, &picture.mid_num[score[1]][1], SRCPAINT);

						putimage(220, 255, &picture.mid_num[score[0]][0], SRCAND);
						putimage(220, 255, &picture.mid_num[score[0]][1], SRCPAINT);
						break;
					case 3:
						putimage(194, 255, &picture.mid_num[score[2]][0], SRCAND);
						putimage(194, 255, &picture.mid_num[score[2]][1], SRCPAINT);

						putimage(207, 255, &picture.mid_num[score[1]][0], SRCAND);
						putimage(207, 255, &picture.mid_num[score[1]][1], SRCPAINT);

						putimage(220, 255, &picture.mid_num[score[0]][0], SRCAND);
						putimage(220, 255, &picture.mid_num[score[0]][1], SRCPAINT);
						break;
					}
				}
			}
		}

		//����ѡ��
		/*if ((data.nowscore - medalscore) > 20)
			medal = 3;
		else if((data.nowscore - medalscore) > 15)
			medal = 2;
		else if ((data.nowscore - medalscore) > 10)
			 medal = 1;
		else if ((data.nowscore - medalscore) > 5)
			 medal = 0;
		if (medal != -1)
		{
			putimage(56, 225, &picture.medals[medal][0], SRCAND);
			putimage(56, 225, &picture.medals[medal][1], SRCPAINT);
		}*/
		putimage(0, 400, &picture.landpt);

		putimage(80, 342, &picture.button[0][0], SRCAND);
		putimage(80, 342, &picture.button[0][1], SRCPAINT);

		FlushBatchDraw();
		EndBatchDraw();

		//����ѡ��
		if (MouseHit())
		{
			msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN)
				if (msg.x >= 80 && msg.x <=190 && msg.y >= 347 && msg.y <= 517)
				{
		//			BeginBatchDraw();
		//			GetLocalTime(&sys);
		//			if ((sys.wHour + 8) >= 17 || (sys.wHour + 8) <= 6)
		//				putimage(0, 0, &picture.backgroundpt[1]);
		//			else
		//				putimage(0, 0, &picture.backgroundpt[0]);

		//			//putimage(40, 100, &picture.gameover[0], SRCAND);
		//			//putimage(40, 100, &picture.gameover[1], SRCPAINT);

		//			//�÷����
		//			//putimage(25, 180, &picture.button[3][0], SRCAND);
		//			//putimage(25, 180, &picture.button[3][1], SRCPAINT);
		//			
		//			putimage(80, 347, &picture.button[0][0], SRCAND);
		//			putimage(80, 347, &picture.button[0][1], SRCPAINT);

		//			putimage(0, 400, &picture.landpt);

		//			if (medal != -1)
		//			{
		//				putimage(56, 225, &picture.medals[medal][0], SRCAND);
		//				putimage(56, 225, &picture.medals[medal][1], SRCPAINT);
		//			}

		//			FlushBatchDraw();
		//			Sleep(50);
		//			GetLocalTime(&sys);
		//			if ((sys.wHour + 8) >= 17 || (sys.wHour + 8) <= 6)
		//				putimage(0, 0, &picture.backgroundpt[1]);
		//			else
		//				putimage(0, 0, &picture.backgroundpt[0]);

		//			putimage(40, 100, &picture.gameover[0], SRCAND);
		//			putimage(40, 100, &picture.gameover[1], SRCPAINT);

		//			putimage(0, 400, &picture.landpt);

		//			FlushBatchDraw();
		//			EndBatchDraw();

					//д�����
					err = fopen_s(&fp, "score.txt", "w");
					i = 0;//��λ
					for (; i < 3; i++)
					{
						fprintf(fp, "%d ", data.oldscore[i]);
					}
					fclose(fp);
					break;
				}
		}			
	}
}

//�ı��ϰ����Y������
void changeband(Data &data,int dstX)
{
	srand((unsigned int)time(NULL));
	int j = rand() % 5 + 1;
	int key= rand() % 10 + 1;

	for(int i=0;i<5;i++)
		if (dstX == data.banddata[i][0])
		{
			int temp = rand() % 30 + 1;
			if (temp % 2 == 0)
			{
				data.banddata[i][1] = -175 + temp;//�Ϲ�������
			}
			else
			{
				data.banddata[i][1] = -175 - temp;//�Ϲ�������
			}

			temp = rand() % 30 + 1;
			if (temp % 2 == 0)
			{
				data.banddata[i][2] = 250 + temp;//�¹�������
			}
			else
			{
				data.banddata[i][2] = 250 - temp;//�¹�������
			}

			if (key % 2 == 0)
			{
				key =  rand() % 40 + j * 10;
				data.banddata[i][1] += key;
				data.banddata[i][2] += key;
			}
			else
			{
				key = rand() % 40 + j * 10;
				data.banddata[i][1] -= key;
				data.banddata[i][2] -= key;
			}
		}

}
