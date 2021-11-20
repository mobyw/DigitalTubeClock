// DS1302 ʱ��оƬ����ͷ�ļ�

#ifndef __DS1302_H_
#define __DS1302_H_

#include "STC15F2K.h" // ���õ�Ƭ��ͷ�ļ�

sbit clk = P3 ^ 2; // ʱ����
sbit io = P3 ^ 1;  // ������
sbit rst = P5 ^ 5; // ��λ��

// ʹʱ�����������ļ���ֱ��ʹ��
extern unsigned char time_sec, time_min, time_hour;
extern unsigned char alarm_min, alarm_hour;

void ds1302_init();
void ds1302_init_io();
unsigned char ds1302_readram(unsigned char addr);
unsigned char ds1302_read(unsigned char addr);
void ds1302_writeram(unsigned char addr, unsigned char dat);
void ds1302_write(unsigned char addr, unsigned char dat);
void read_time();
void write_time();

#endif // __DS1302_H_
