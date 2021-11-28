/**
  ******************************************************************************
  * @file           : main.c
  * @version        : v1.0
  * @author         : Xiaohei
  * @date           : 2021-11-20
  * @brief          : Timer interupt
  ******************************************************************************
  */

#include <STC15F2K60S2.H>
#include <intrins.h>

#define LED_PORT P2

sbit LED_A = P3 ^ 7;
sbit LED_K = P2 ^ 0;

unsigned int count = 0; // 0-65535

void Timer0Init(void)   // 1ms@11.0592MHz
{
    AUXR &= 0x7F;       // ��ʱ��ʱ��12Tģʽ
    TMOD &= 0xF0;       // ���ö�ʱ��ģʽ
    TL0 = 0x66;         // ���ö�ʱ��ֵ
    TH0 = 0xFC;         // ���ö�ʱ��ֵ
    TF0 = 0;            // ���TF0��־
    TR0 = 1;            // ��ʱ��0��ʼ��ʱ
    ET0 = 1;
    EA = 1;
}

int main()
{
    // �˴������ִ��һ��
    
    LED_A = 1;
    LED_K = 0;
    
    ///*
    P33 = 0;
    P34 = 0;
    P35 = 0;
    P36 = 0;
    //*/
    
    ///*
    P12 = 0;
    P13 = 0;
    P14 = 0;
    P15 = 0;
    P16 = 0;
    P17 = 0;
    P54 = 0;
    //*/
    
    Timer0Init();
    
    while(1)
    {
        // �˴�����ѭ��ִ��
        
    }
}

void Timer0Interupt(void) interrupt 1
{
    count++;
    
    if(count >= 500)
    {
        count = 0;
        LED_K = !LED_K;
    }
}
