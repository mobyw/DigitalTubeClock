/**
  ******************************************************************************
  * @file           : main.c
  * @version        : v1.0
  * @author         : Xiaohei
  * @date           : 2021-11-20
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Project git: github.com/mobyw/DigitalTubeClock
  *
  ******************************************************************************
  */

#include "main.h"

// �򿪷�����
//#define ENABLE_BELL

// ҳ������
#define NUM_PAGE 9

// ʱ�䶯������
#define NUM_TIME_ANIMATE 2

// �ѻ���������
#define NUM_OFFLINE_ANIMATE 2

// ��ʱ���ƴ�
unsigned char timer_cnt = 0;

// ��ʱ���ƴ�
unsigned char tick = 0;

// ��ʾ��ʱ ��������
unsigned int light = 8;

// �洢�¶Ƚ��
unsigned int temperature = 20;

// ���ӿ���
bit alarm_control = 0;

// ���ȵ��ڿ���
bit light_control = 0;

// ʱ�䶯��
// ���ڴ˴����ÿ��������ע��
// 0: ��ʾ��
// 1: ��ɫ��ʾ��
unsigned char time_animate = 0;

// �ѻ�����
// ���ڴ˴����ÿ��������ע��
// 0: ������ʾ����1
// 1: ������ʾ����1
unsigned char offline_animate = 0;

// ��ǰҳ�����
// 0: ��ʾʱ�� ʱ�䶯��
// 1: ��������
// 2: ����Сʱ
// 3: �������ӷ���
// 4: ��������Сʱ
// 5: ���ӿ���
// 6: ��ʾѧ�� �ѻ�����
// 7: ��ʾ�¶�
// 8: ��������
char page = 0;

// ����ɨ��
void keyScan()
{
    // ���� ADC ת��
    disableAdc();

    // ������
    if (alarm_control == 1)
    {
        if (time_hour == alarm_hour && time_min == alarm_min)
        {
            //todo
        }
    }

    // ���� 1 ����
    if (BTN1 == 0)
    {
        delay(20);

        if (BTN1 == 0)
        {
#ifdef ENABLE_BELL
            // ��������
            bell = 0;
#endif
            // ��ת��һ��ҳ��
            page++;

            if (page == 3)
            {
                // д�����õ�ʱ��
                write_time();
            }

            // �����һ��ҳ��ʱ����ҳ�� 0
            if (page == NUM_PAGE)
            {
                page = 0;
            }

            // �ȴ������ɿ�
            while (BTN1 == 0)
                ;
#ifdef ENABLE_BELL
            // �رշ�����
            bell = 1;
#endif
        }
        delay(10);
    }

    // ʱ����� �����л� LED ʱ�䶯������
    if (page == 0)
    {
        if (BTN2 == 0)
        {
            delay(20);

            if (BTN2 == 0)
            {
#ifdef ENABLE_BELL
                bell = 0;
#endif
                time_animate = (time_animate + 1) % NUM_TIME_ANIMATE;
                while (BTN2 == 0)
                    ;
#ifdef ENABLE_BELL
                bell = 1;
#endif
            }
            delay(10);
        }
    }

    // ���÷��ӽ��� ��������
    if (page == 1)
    {
        // ���� 2 ����
        if (BTN2 == 0)
        {
            delay(20);

            if (BTN2 == 0)
            {
#ifdef ENABLE_BELL
                // ��������
                bell = 0;
#endif
                // ���ӳ��� 59 �ָ� 0
                if (time_min >= 0x60)
                    time_min = 0;

                // ���÷��Ӽ� 1
                time_min = time_min + 0x01;

                // ʮ������ת��Ϊ BCD
                if ((time_min & 0x0f) >= 0x0a)
                    time_min = (time_min & 0xf0) + 0x10;

                // �ȴ��ɿ�
                while (BTN2 == 0)
                    ;
#ifdef ENABLE_BELL
                // �رշ�����
                bell = 1;
#endif
            }
            delay(10);
        }
    }

    // ����Сʱ���� ��������
    if (page == 2)
    {
        if (BTN2 == 0)
        {
            delay(20);

            if (BTN2 == 0)
            {
#ifdef ENABLE_BELL
                bell = 0;
#endif
                time_hour += 0x01;
                if ((time_hour & 0x0f) >= 0x0a)
                    time_hour = (time_hour & 0xf0) + 0x10;
                if (time_hour >= 0x24)
                    time_hour = 0;
                while (BTN2 == 0)
                    ;
#ifdef ENABLE_BELL
                bell = 1;
#endif
            }
            delay(10);
        }
    }

    // �������ӷ��ӽ��� ��������
    if (page == 3)
    {
        // ���� 2 ����
        if (BTN2 == 0)
        {
            delay(20);

            if (BTN2 == 0)
            {
#ifdef ENABLE_BELL
                // ��������
                bell = 0;
#endif
                // ���ӳ��� 59 �ָ� 0
                if (alarm_min >= 0x60)
                    alarm_min = 0;

                // ���÷��Ӽ� 1
                alarm_min = alarm_min + 0x01;

                // ʮ������ת��Ϊ BCD
                if ((alarm_min & 0x0f) >= 0x0a)
                    alarm_min = (alarm_min & 0xf0) + 0x10;

                // �ȴ��ɿ�
                while (BTN2 == 0)
                    ;
#ifdef ENABLE_BELL
                // �رշ�����
                bell = 1;
#endif
            }
            delay(10);
        }
    }

    // ��������Сʱ���� ��������
    if (page == 4)
    {
        //todo
    }

    // �������ӿ��ؽ��� ��������
    if (page == 5)
    {
        if (BTN2 == 0)
        {
            delay(20);

            if (BTN2 == 0)
            {
#ifdef ENABLE_BELL
                bell = 0;
#endif
                alarm_control = !alarm_control;
                while (BTN2 == 0)
                    ;
#ifdef ENABLE_BELL
                bell = 1;
#endif
            }
            delay(10);
        }
    }

    // ѧ�Ž��� �����л� LED �ѻ���������
    if (page == 6)
    {
        //todo
    }

    // ���ù�ؿ��ؽ��� ��������
    if (page == 8)
    {
        //todo
    }

    // ʹ�� ADC ת��
    enableAdc();
}

// ������
void main()
{
    unsigned char i = 0;

    // ��ʼ��
    systemInit();

    // ��ѭ��
    while (1)
    {
        // ����ɨ��
        keyScan();

        // ��ʾʱ�����
        if (page == 0)
        {
            // ��ȡʱ��
            read_time();

            // ����ʱ��
            display_array[0] = time_hour / 16;
            display_array[1] = time_hour % 16;
            display_array[2] = time_min / 16;
            display_array[3] = time_min % 16;

            // ��� LED ����
            if (time_animate == 0)
            {
                ledAllOff();
                setLed(bcd2hex(time_sec));
            }
            else if (time_animate == 1)
            {
                ledAllOn();
                resetLed(bcd2hex(time_sec));
            }
            else
            {
                // todo
            }
        }

        // ����ʱ�� ����
        if (page == 1)
        {
            ledAllOff();

            if (blink > 0)
            {
                display_array[2] = time_min / 16;
                display_array[3] = time_min % 16;
            }
            else
            {
                display_array[2] = _SPACE;
                display_array[3] = _SPACE;
            }
            display_array[0] = time_hour / 16;
            display_array[1] = time_hour % 16;
        }

        // ����ʱ�� Сʱ
        if (page == 2)
        {
            ledAllOff();

            display_array[2] = time_min / 16;
            display_array[3] = time_min % 16;
            if (blink > 0)
            {
                display_array[0] = time_hour / 16;
                display_array[1] = time_hour % 16;
            }
            else
            {
                display_array[0] = _SPACE;
                display_array[1] = _SPACE;
            }
        }

        // ��������ʱ�� ����
        if (page == 3)
        {
            ledAllOff();

            if (blink > 0)
            {
                display_array[2] = alarm_min / 16;
                display_array[3] = alarm_min % 16;
            }
            else
            {
                display_array[2] = _SPACE;
                display_array[3] = _SPACE;
            }

            display_array[0] = alarm_hour / 16;
            display_array[1] = alarm_hour % 16;
        }

        // ��������ʱ�� Сʱ
        if (page == 4)
        {
            ledAllOff();

            display_array[2] = alarm_min / 16;
            display_array[3] = alarm_min % 16;

            if (blink > 0)
            {
                display_array[0] = alarm_hour / 16;
                display_array[1] = alarm_hour % 16;
            }
            else
            {
                display_array[0] = _SPACE;
                display_array[1] = _SPACE;
            }
        }

        // ���ӿ���
        if (page == 5)
        {
            ledAllOff();

            display_array[0] = _SPACE;
            display_array[1] = _SPACE;
            display_array[2] = _SPACE;
            display_array[3] = alarm_control;
        }

        // ��ʾѧ��
        if (page == 6)
        {
            display_array[0] = _SPACE;
            display_array[1] = _SPACE;
            display_array[2] = _SPACE;
            display_array[3] = _SPACE;

            if (tick == 1)
            {
                tick = 0;
                i = (i + 1) % 15;
            }

            // ��� LED ����
            if (offline_animate == 0)
            {
                ledAllOff();
                setLed(i);
                setLed(i + 15);
                setLed(i + 30);
                setLed(i + 45);
            }
            else if (offline_animate == 1)
            {
                ledAllOn();
                resetLed(i);
                resetLed(i + 15);
                resetLed(i + 30);
                resetLed(i + 45);
            }
            else
            {
                // todo
            }
        }

        // ��ʾ�¶�
        if (page == 7)
        {
            ledAllOff();

            temperature = getTemperature();

            display_array[0] = _SPACE;
            display_array[1] = temperature / 1000;
            display_array[2] = temperature % 1000 / 100;
            display_array[3] = _C;
        }

        // ��ؿ���
        if (page == 8)
        {
            ledAllOff();

            display_array[0] = _SPACE;
            display_array[1] = _SPACE;
            display_array[2] = _SPACE;
            display_array[3] = light_control;
        }

        // ����ж�
        if (light_control == 1)
        {
            //todo
        }
        else
        {
            light = 8;
        }

        // ��ʾ
        display();
        delay(light);
    }
}

// ��ʱ�� 1 �ж�
void intTimer1() interrupt 1
{
    TL0 = (65536 - 20000) / 256; // ���ö�ʱ��ֵ
    TH0 = (65536 - 20000) % 256; // ���ö�ʱ��ֵ

    timer_cnt++;

    // �Ͽ��ʱ�� ���ڶ�����ʾ
    if (timer_cnt % 10 == 0)
    {
        tick = 1;
    }

    // ð����˸�������ж�
    if (timer_cnt == 40)
    {
        timer_cnt = 0;

        if (page <= 4)
        {
            blink = !blink;
        }
        else
        {
            blink = 1;
        }

        // ���ӹ���
        if (alarm_control == 1)
        {
            if (time_hour == alarm_hour && time_min == alarm_min)
            {
                //todo
            }
        }
        else
        {
            bell = 1;
        }
    }
}
