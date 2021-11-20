/**
  ******************************************************************************
  * @file           : api.c
  * @version        : v1.0
  * @author         : Xiaohei
  * @date           : 2021-11-20
  * @brief          : Api program body
  ******************************************************************************
  * @attention
  *
  * Project git: github.com/mobyw/DigitalTubeClock
  *
  ******************************************************************************
  */

#include "api.h"

// ����ܶ�ѡ��
// 0 1 2 3 4 5 6 7 8 9 SPACE C
unsigned char num_seg_sel[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xff, 0xc6};

// ��Χ LED ��ʾ����
unsigned char led_array[8] = {0};

// �������ʾ����
// �������±�ֱ�Ϊ 0 1 2 3
unsigned char display_array[4] = {0};

// ð����˸���Ʊ�־
char blink = 1;

// �洢 ADC ת�����
unsigned int adc_result = 0;

// ���ݷǾ�ȷ��ʱ
void delay(unsigned int t)
{
    unsigned char i;
    while (t--)
        for (i = 0; i < 100; i++)
            ;
}

// BCD ת HEX
unsigned char bcd2hex(unsigned char bcd)
{
    unsigned char temp;

    temp = bcd & 0x0f;
    bcd = bcd >> 4;
    bcd %= 0xf;

    return (bcd * 10 + temp);
}

// ��ʱ����ʼ��
void timerInit()
{
    TMOD = 0x01;
    TL0 = (65536 - 20000) / 256; // ���ö�ʱ��ֵ
    TH0 = (65536 - 20000) % 256; // ���ö�ʱ��ֵ
    ET0 = 1;
    TR0 = 1;
    EA = 1;
}

// ϵͳ��ʼ��
void systemInit()
{
    // ��ʱ����ʼ��
    timerInit();

    // �����ʼ��
    ds1302_init();

    // �˿ڳ�ʼ��
    P1M0 = 0xff;
    P1M1 = 0x00;
    P2M0 = 0xff;
    P2M1 = 0x00;
    P3M0 = 0xf8;
    P3M1 = 0x00;
    P5M0 = 0x10;
    P5M1 = 0x00;
}

// ��� LED ��ʾ
void ledAllOff()
{
    unsigned char i = 0;

    for (i = 0; i < 8; i++)
    {
        // ȫ��Ϩ��
        led_array[i] = 0xff;
    }
}

// ����ȫ�� LED
void ledAllOn()
{
    unsigned char i = 0;

    for (i = 0; i < 8; i++)
    {
        // ȫ����
        led_array[i] = 0x00;
    }
}

// ָ�� LED ����ʾ
// index: LED ���� 0-59
void setLed(unsigned char index)
{
    index = (index + 59) % 60;
    reset_bit(led_array[index / 8], index % 8);
}

// ָ�� LED �ر���ʾ
// index: LED ���� 0-59
void resetLed(unsigned char index)
{
    index = (index + 59) % 60;
    set_bit(led_array[index / 8], index % 8);
}

// ������� LED ��ʾ
void display()
{
    // -- �������ʾʱ��
    // ��һλ
    SEG_COM1 = ENABLE;
    SEG_PORT = num_seg_sel[display_array[0]];
    delay(2);
    SEG_PORT = ALLOFF;
    SEG_COM1 = DISABLE;

    // �ڶ�λ
    // �ж�ð����ʾʱ���λ��λ
    SEG_COM2 = ENABLE;
    if (blink == 0)
        SEG_PORT = num_seg_sel[display_array[1]] - 0x80;
    if (blink == 1)
        SEG_PORT = num_seg_sel[display_array[1]];
    delay(2);
    SEG_PORT = ALLOFF;
    SEG_COM2 = DISABLE;

    // ����λ
    SEG_COM3 = ENABLE;
    SEG_PORT = num_seg_sel[display_array[2]];
    delay(2);
    SEG_PORT = ALLOFF;
    SEG_COM3 = DISABLE;

    // ����λ
    SEG_COM4 = ENABLE;
    SEG_PORT = num_seg_sel[display_array[3]];
    delay(2);
    SEG_PORT = ALLOFF;
    SEG_COM4 = DISABLE;

    // -- LED ��ʾ
    // 1-8
    LED_COM1 = ENABLE;
    SEG_PORT = led_array[0];
    delay(2);
    SEG_PORT = ALLOFF;
    LED_COM1 = DISABLE;

    // 9-16
    LED_COM2 = ENABLE;
    SEG_PORT = led_array[1];
    delay(2);
    SEG_PORT = ALLOFF;
    LED_COM2 = DISABLE;

    // 17-24
    LED_COM3 = ENABLE;
    SEG_PORT = led_array[2];
    delay(2);
    SEG_PORT = ALLOFF;
    LED_COM3 = DISABLE;

    // 25-32
    LED_COM4 = ENABLE;
    SEG_PORT = led_array[3];
    delay(2);
    SEG_PORT = ALLOFF;
    LED_COM4 = DISABLE;

    // 33-40
    LED_COM5 = ENABLE;
    SEG_PORT = led_array[4];
    delay(2);
    SEG_PORT = ALLOFF;
    LED_COM5 = DISABLE;

    // 41-48
    LED_COM6 = ENABLE;
    SEG_PORT = led_array[5];
    delay(2);
    SEG_PORT = ALLOFF;
    LED_COM6 = DISABLE;

    // 49-56
    LED_COM7 = ENABLE;
    SEG_PORT = led_array[6];
    delay(2);
    SEG_PORT = ALLOFF;
    LED_COM7 = DISABLE;

    // 57-60
    LED_COM8 = ENABLE;
    SEG_PORT = led_array[7];
    delay(2);
    SEG_PORT = ALLOFF;
    LED_COM8 = DISABLE;

    // ���� �ر�������ʾ
    SEG_COM1 = SEG_COM2 = SEG_COM3 = SEG_COM4 = ENABLE;
    LED_COM1 = LED_COM2 = LED_COM3 = LED_COM4 = ENABLE;
    LED_COM5 = LED_COM6 = LED_COM7 = LED_COM8 = ENABLE;
    SEG_PORT = ALLOFF;
    SEG_COM1 = SEG_COM2 = SEG_COM3 = SEG_COM4 = DISABLE;
    LED_COM1 = LED_COM2 = LED_COM3 = LED_COM4 = DISABLE;
    LED_COM5 = LED_COM6 = LED_COM7 = LED_COM8 = DISABLE;
}

// ʹ�� ADC ת��
void enableAdc()
{
    P1ASF = 0x03;
}

// ���� ADC ת��
void disableAdc()
{
    P1ASF = 0x00;
}

// ��ȡ ADC ͨ����ֵ
// ch: ADC ͨ�����
unsigned int getAdcResult(unsigned char ch)
{
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;
    _nop_(); // Must wait before inquiry
    _nop_();
    _nop_();
    _nop_();
    _nop_(); // Must wait before inquiry
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG))
        ;                   // Wait complete flag
    ADC_CONTR &= ~ADC_FLAG; // Close ADC

    return (ADC_RES * 4 + ADC_RESL); // Return ADC result.
}

// ��ȡ�¶�
unsigned int getTemperature(void)
{
    adc_result = getAdcResult(0);
    return (unsigned int)((3950.0 / (11.33657 + log(6.04 * (float)adc_result / (1024.0 - (float)adc_result))) - 278.15) * 100);
}

// ��ȡ���������ѹ
unsigned int getLight(void)
{
    adc_result = getAdcResult(1);

    if (adc_result < 100)
    {
        return 8;
    }
    if (adc_result > 100 && adc_result < 300)
    {
        return 10;
    }
    if (adc_result > 300 && adc_result < 500)
    {
        return 20;
    }
    if (adc_result > 500 && adc_result < 600)
    {
        return 40;
    }
    if (adc_result > 600 && adc_result < 700)
    {
        return 60;
    }
    if (adc_result > 700 && adc_result < 800)
    {
        return 90;
    }
    if (adc_result > 800 && adc_result < 900)
    {
        return 120;
    }
    if (adc_result > 900)
    {
        return 150;
    }

    return 8;
}
