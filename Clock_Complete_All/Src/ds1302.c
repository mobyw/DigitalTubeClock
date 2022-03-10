// DS1302 ʱ��оƬ�����ļ�

#include "ds1302.h"

// ��ŵ�ǰ�롢�֡�ʱ����
// ���� `read_time()` ��������
// ��ͷ�ļ��������� extern
unsigned char time_sec = 0;
unsigned char time_min = 0;
unsigned char time_hour = 0;

// ������ӵķ֡�ʱ����
// ��ʾ����δʹ�õ�
unsigned char alarm_min = 0;
unsigned char alarm_hour = 0;

// ��оƬ�Ĵ����ĵ�ַ����
// �� �� ʱ �� �� �� ����
unsigned char code write_addr[] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8c, 0x8a}; // д��ַ
unsigned char code read_addr[] = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8d, 0x8b};  // ����ַ

// ��ʼʱ������
// �� �� ʱ �� �� �� ����
// ʹ�� BCD ��, �� 55 ��Ϊ 0x55
unsigned char code init_ds[] = {0x56, 0x34, 0x12, 0x10, 0x10, 0x21, 0x07};

// д����
void ds1302_write(unsigned char addr, unsigned char dat)
{
    unsigned char i;

    rst = 1; // ��λ������

    for (i = 0; i < 8; i++)
    {            // ��λ��ǰ
        clk = 0; // ʱ�������Ϳ�ʼд����
        io = addr & 0x01;
        addr >>= 1; // ��ַ����һλ
        clk = 1;    // ʱ��������
    }

    for (i = 0; i < 8; i++)
    {
        clk = 0; // ʱ�������Ϳ�ʼд����
        io = dat & 0x01;
        dat >>= 1; // ��������һλ
        clk = 1;   // ʱ��������
    }

    rst = 0; // ��λ�ߺϵ�
    clk = 0;
    io = 0;
}

// ������
unsigned char ds1302_read(unsigned char addr)
{
    unsigned char value, i;

    rst = 1; // ��λ������

    for (i = 0; i < 8; i++)
    {            // ��λ��ǰ
        clk = 0; // ʱ�������Ϳ�ʼд����
        io = addr & 0x01;
        addr >>= 1; // ��ַ����һλ
        clk = 1;    // ʱ��������
    }

    for (i = 0; i < 8; i++)
    {
        clk = 0; // ʱ�������Ϳ�ʼ������
        value >>= 1;
        if (io == 1)
            value |= 0x80;
        clk = 1; // ʱ��������
    }

    rst = 0; // ��λ������
    clk = 0;
    io = 0;
    return value; // ��������
}

// ��ȡʱ��
void read_time()
{
    time_sec = ds1302_read(read_addr[0]);  // ����
    time_min = ds1302_read(read_addr[1]);  // ����
    time_hour = ds1302_read(read_addr[2]); // ��ʱ

//  date_day    = ds1302_read(read_addr[3]);    // ����
//  date_month  = ds1302_read(read_addr[4]);    // ����
//  date_year   = ds1302_read(read_addr[5]);    // ����
//  date_week   = ds1302_read(read_addr[6]);    // ������
}

// дʱ��
void write_time()
{
    ds1302_write(0x8e, 0x00); // ��д����

//  ds1302_write(write_addr[0], time_sec);   // д��
    ds1302_write(write_addr[1], time_min);   // д��
    ds1302_write(write_addr[2], time_hour);  // дʱ
//  ds1302_write(write_addr[3], date_day);   // д��
//  ds1302_write(write_addr[4], date_month); // д��
//  ds1302_write(write_addr[5], date_year);  // д��
//  ds1302_write(write_addr[6], date_week);  // д����

    ds1302_write(0xc2, alarm_hour); // д����ʱ
    ds1302_write(0xc4, alarm_min);  // д���ӷ�

    ds1302_write(0x8e, 0x80); // �ر�д����
}

/* // ��ȡ����ʱ��
void read_alarm()
{
    alarm_hour = ds1302_read(0xc3);   // ������ʱ
    alarm_min = ds1302_read(0xc5);    // �����ӷ�
}
*/

// д ds1302 RAM
void ds1302_writeram(unsigned char addr, unsigned char dat)
{
    addr <<= 1;   // ��ַ�ӵڶ�λ��ʼ
    addr &= 0xfe; // ���λ��λ д����
    addr |= 0xc0; // ��ַ�����λΪ 1

    ds1302_write(0x8e, 0x00);
    ds1302_write(addr, dat);
    ds1302_write(0x8e, 0x80);
}

// �� ds1302 RAM
unsigned char ds1302_readram(unsigned char addr)
{
    addr <<= 1;   // ��ַ�ӵڶ�λ��ʼ
    addr |= 0x01; // ���λ��λ ������
    addr |= 0xc0; // ��ַ�����λΪ 1

    return (ds1302_read(addr));
}

// ��ʼ�� ds1302 ʱ��
void ds1302_init()
{
    unsigned char i;

    ds1302_init_io();

    i = ds1302_readram(30);

    if (i != 3)
    {
        i = 3;

        ds1302_writeram(30, i);
        ds1302_write(0x8e, 0x00); //��д����

        for (i = 0; i < 3; i++)
        {
            ds1302_write(write_addr[i], init_ds[i]); // ���λ��λ ʹ��оƬ
        }

        ds1302_write(0x8e, 0x80); // ��д����
    }
}

void ds1302_init_io()
{
    // ��һ�ζ�д IO ����
    rst = 0;
    clk = 0;
    io = 0;
}
