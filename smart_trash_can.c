/*
��Ӧ���ظ�����Ͱ
��Ŀ����
1��������
��⿿��ʱ������Ͱ�Զ����ǲ������һ����2���ظ�
������ʱ������Ͱ�Զ����ǲ������һ����2���ظ�
���°���ʱ������Ͱ�Զ����ǲ������һ����2���ظ�
Ӳ��˵��
SG90�����������ģ�飬�𶯴�������������
����˵��
������ƿ� P1.1��������Trig�� P1.5 ,Echo�� P1.6 ���������� P2.0 ��; �𶯴������� P3.2`��(�ⲿ�ж�0)
2���ʵ��
�������裺
1. ����ͳ�������������
����ö�ʱ��0
�������ö�ʱ��1
ʵ�����忿�����Զ����ǣ�2���ظ�
2. ��ѯ�ķ�ʽ��Ӱ�������
3. ��ѯ�ķ�ʽ����𶯿���
4. ʹ���ⲿ�ж�0����𶯿���
*/
/*
��⿿��ʱ������Ͱ�Զ����ǲ������һ����2���ظ�
������ʱ������Ͱ�Զ����ǲ������һ����2���ظ�
���°���ʱ������Ͱ�Զ����ǲ������һ����2���ظ�
*/
#include "reg52.h"
sbit D5 = P3^7;//����ԭ��ͼ����·ͼ�����豸����led1ָ��P3��IO�ڵĵ�7��
sbit D6 = P3^6;//�豸����led2ָ��P3��IO�ڵĵ�6��
sbit SW1 = P2^1;
sbit Trig = P1^5;
sbit Echo = P1^6;
sbit sg90_con = P1^1;
sbit vibrate ?= P3^2;
sbit beep ??= P2^0;
char jd;
char jd_bak;
char cnt = 0;
char mark_vibrate = 0;
void Delay150ms() //@11.0592MHz
{
	unsigned char i, j, k;
	i = 2;
	j = 13;
	k = 237;
	do{
		do{
		while (--k);
		} while (--j);
	} while (--i);
}
void Delay2000ms() //@11.0592MHz
{
	unsigned char i, j, k;
	i = 15;
	j = 2;
	k = 235;
	do{
		do{
		while (--k);
		} while (--j);
	} while (--i);
}
void Delay10us() //@11.0592MHz
{
	unsigned char i;
	i = 2;
	while (--i);
}
void Time0Init()
{
//1. ���ö�ʱ��0����ģʽλ16λ��ʱ
	TMOD &= 0xF0; //���ö�ʱ��ģʽ
	TMOD |= 0x01;
//2. ����ֵ����һ��0.5����
	TL0=0x33;
	TH0=0xFE;
//3. ��ʼ��ʱ
	TR0 = 1;
	TF0 = 0;
//4. �򿪶�ʱ��0�ж�
	ET0 = 1;
//5. �����ж�EA
	EA = 1;
}
void Time1Init()
{
	TMOD &= 0x0F; //���ö�ʱ��ģʽ
	TMOD |= 0x10;
	TH1 = 0;
	TL1 = 0;
//���ö�ʱ��0����ģʽ1����ʼֵ�趨0��ʼ���������ż�������ʱ��
}
void startHC()
{
	Trig = 0;
	Trig = 1;
	Delay10us();
	Trig = 0;
}
double get_distance()
{
	double time;
//��ʱ���������㣬�Ա���һ�β��
	TH1 = 0;
	TL1 = 0;
//1. Trig ����Trig�˿�����10us�ĸߵ�ƽ
	startHC();
//2. echo�ɵ͵�ƽ��ת���ߵ�ƽ����ʾ��ʼ���Ͳ�
	while(Echo == 0);
//������ȥ����һ�£���ʼ������ʱ��
	TR1 = 1;
//3. �ɸߵ�ƽ��ת�ص͵�ƽ����ʾ��������
	while(Echo == 1);
//����������һ�£�ֹͣ��ʱ��
	TR1 = 0;
//4. ������м侭������ʱ��
	time = (TH1 * 256 + TL1)*1.085;//usΪ��λ
//5. ���� = �ٶ� ��340m/s��* ʱ��/2
	return (time * 0.017);
}
void openStatusLight()
{
	D5 = 0;
	D6 = 1;
}
void closeStatusLight()
{
	D5 = 1;
	D6 = 0;
}
void initSG90_0()
{
	jd = 1; ???//��ʼ�Ƕ���0�ȣ�0.5ms,���1����0.5���ߵ�ƽ
	cnt = 0;
	sg90_con = 1;//һ��ʼ�Ӹߵ�ƽ��ʼ
}
void openDusbin()
{
	char n;
	jd = 3; //90�� 1.5ms�ߵ�ƽ
	 //�������
	if(jd_bak != jd){
		cnt = 0;
		beep = 0;
		for(n=0;n<2;n++)
			Delay150ms();
		beep = 1;
		Delay2000ms();
		}
	jd_bak = jd;
}
void closeDusbin()
{
//�ظ�
	jd = 1; //0��
	jd_bak = jd;
	cnt = 0;
	Delay150ms();
}
void EX0_Init()
{
//���ⲿ�ж�
	EX0 = 1;
//�͵�ƽ����
	IT0 = 0;
}
void main()
{
	double dis;
	Time0Init();
	Time1Init();
	EX0_Init();
//����ĳ�ʼλ��
	initSG90_0();
	while(1){
//���������
	dis = get_distance();
	if(dis < 10 || SW1 == 0 || mark_vibrate == 1){//���С��10����,����sw1����������
//���ǣ���״̬��D5��
		openStatusLight();
		openDusbin();
		mark_vibrate = 0;
		}else{
//�ظǣ���״̬��D5��
		closeStatusLight();
		closeDusbin();
		}
	}
}
void Time0Handler() interrupt 1
{
	cnt++; //ͳ�Ʊ���Ĵ���. cnt=1��ʱ�򣬱�����1
	//���¸���ֵ
	TL0=0x33;
	TH0=0xFE;
	//����PWM��
	if(cnt < jd){
		sg90_con = 1;
		}else{
		sg90_con = 0;
	}
	if(cnt == 40){//����40�Σ�������20ms
		cnt = 0; ?//��100�α�ʾ1s��������cnt��0��ʼ��������һ�ε�1s
		sg90_con = 1;
	}
}
void Ex0_Handler() interrupt 0
{
	mark_vibrate = 1;
}
