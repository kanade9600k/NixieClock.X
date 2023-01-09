/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC16F1827
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip software and any
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party
    license terms applicable to your use of third party software (including open source software) that
    may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
    FOR A PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#define SER RA0       // RA0��SER�Ɩ��Â���
#define RCLK RA1      // RA1��RCLK�Ɩ��Â���
#define UPPSRCLK RA2  // RA2��UPPSRCLK�Ɩ��Â���
#define MIDSRCLK RA3  // RA3��MIDSRCLK�Ɩ��Â���
#define LOWSRCLK RA4  // RA4��LOWSRCLK�Ɩ��Â���
#define DOT RA6       // RA6��DOT�Ɩ��Â���
#define UPDIGIT RB0   // RB0��UPDIGIT�Ɩ��Â���
#define DOWNDIGIT RB1 // RB1��DOWNDIGIT�Ɩ��Â���
#define PLUS RB2      // RB2��PLUS�Ɩ��Â���
#define MINUS RB3     // RB3��MINUS�Ɩ��Â���
#define SWITCH RB4    // RB4��SWITCH�Ɩ��Â���
#define START_EA RB5  // RB5��START_EA�Ɩ��Â���
#define LONGPUSH 500  // ���������莞��(ms)
#define SKIPSPAN 100  // �{�^���������̃X�L�b�v���ɉ�ms��1��J�E���g���邩�̎���
#define MODENUM 2     // ���[�h��

/*
                         Main application
 */

/* �֐��v���g�^�C�v�錾 */
/* ���荞�݊֐� */
void MYTMR0_ISR(void);    // �^�C�}�[0�̊��荞�ݏ���
void MYTMR2_ISR(void);    // �^�C�}�[2�̊��荞�ݏ���(ISR : Interrupt Service Routin)
void PLUS_ISR(void);      // RB0(+�{�^��)�̊��荞�ݏ���
void MINUS_ISR(void);     // RB1(-�{�^��)�̊��荞�ݏ���
void UPDIGIT_ISR(void);   // RB2(�ݒ茅�グ�{�^��)�̊��荞�ݏ���
void DOWNDIGIT_ISR(void); // RB3(�ݒ茅�����{�^��)�̊��荞�ݏ���
void SWITCH_ISR(void);    // RB4(�ݒ胂�[�h�؂�ւ��{�^��)�̊��荞�ݏ���
void START_EA_ISR(void);  // RB5(�X�^�[�g�E�C�l�[�u���{�^��)�̊��荞�ݏ���

/* ����֐� */
void init_clock(void);             // �^�C�}�[�̏������ݒ�
void mainApp(void);                // ���C���֐���while���[�v�ł̏���
void show_nixie(void);             // ���݂̃��[�h�ɍ��킹���l��\������֐�
void show_time(void);              // ���Ԃ�\������֐�
void show_date(void);              // ���t��\������֐�
void show_values(unsigned char[]); // �l��\������֐�
void up_value(void);               // ���݂̃��[�h�C�I�����ɍ��킹�ăJ�E���g�A�b�v����֐�
void down_value(void);             // ���݂̃��[�h�C�I�����ɍ��킹�ăJ�E���g�_�E������֐�
void wait_chattering(void);        // �`���^�����O��҂֐�
void sleep_nixie(unsigned char);   // �j�L�V�[�ǂ�S�������邽�߂̊֐�

/* �O���[�o���ϐ��錾 */
unsigned char csec = 0;           // �Z���`�b(1/100�b)
unsigned char second = 0;         // �b
unsigned char minute = 0;         // ��
unsigned char hour = 0;           // ��
unsigned char day = 1;            // ��
unsigned char month = 1;          // ��
unsigned char year = 0;           // �N
unsigned char enable = 1;         // �j�L�V�[�ǂ̕\���̉� 0: ����, 1: �_��
unsigned char mode = 0;           // 0 : ���v, 1 : �J�����_�[
unsigned char start = 0;          // �J�E���g�J�n�t���O
unsigned char selected_digit = 0; // �ҏW���̌��������t���O 0 : ����2��, 1 : ����2��, 2 : ���2��
unsigned char is_light = 1;       // 0 : �����C1 : �_��

void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // �^�C�}�[0�̊��荞�ݎ��ɌĂяo�����֐�
    TMR0_SetInterruptHandler(MYTMR0_ISR);
    // �^�C�}�[2�̊��荞�ݎ��ɌĂяo�����֐�
    TMR2_SetInterruptHandler(MYTMR2_ISR);
    // RB0�̊��荞�ݎ��ɌĂяo�����֐�
    IOCBF0_SetInterruptHandler(UPDIGIT_ISR);
    // RB1�̊��荞�ݎ��ɌĂяo�����֐�
    IOCBF1_SetInterruptHandler(DOWNDIGIT_ISR);
    // RB2�̊��荞�ݎ��ɌĂяo�����֐�
    IOCBF2_SetInterruptHandler(PLUS_ISR);
    // RB3�̊��荞�ݎ��ɌĂяo����鏈��
    IOCBF3_SetInterruptHandler(MINUS_ISR);
    // RB4�̊��荞�ݎ��ɌĂяo����鏈��
    IOCBF4_SetInterruptHandler(SWITCH_ISR);
    // RB5�̊��荞�ݎ��ɌĂяo����鏈��
    IOCBF5_SetInterruptHandler(START_EA_ISR);

    // Disable the Global Interrupts
    // INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    // INTERRUPT_PeripheralInterruptDisable();

    // �����ɏ����̎��ԍ��킹����������
    init_clock();

    while (start)
    {
        // Add your application code
        mainApp(); // while���̃��C��������mainApp()���ɋL�q����
    }
}

/* ���v�̏������ݒ� */
void init_clock(void)
{
    // �o�͏�����
    SER = 0;
    RCLK = 0;
    LOWSRCLK = 0;
    MIDSRCLK = 0;
    UPPSRCLK = 0;
    DOT = 1;
    // ���g�p�s����Low��
    RB6 = 0;
    RB7 = 0;

    while (!start)
    {
        ; // �����ݒ�͂��ׂĊ��荞�݂ōs��
    }
}

/* while���̃��C������ */
void mainApp(void)
{
    // �S�����荞�݂ł���?
    // if (enable)
    // {
    //     if (is_write)
    //     {
    //         is_write = 0; // �������݃t���O���~�낷
    //         write_74HC595();
    //     }
    //     if (!enable)
    //     {                     // �r���Ŋ��荞�݂����������Ƃ��̏���
    //         sleep_nixie(3); // �S����
    //     }
    // }

    if (enable)
    {
        show_nixie();
    }
    else
    {
        sleep_nixie(3);
    }
}

/* ���݂̃��[�h�ɍ��킹���l��\������֐� */
void show_nixie(void)
{
    switch (mode)
    {
    case 0: // ���v���[�h
        show_time();
        break;

    case 1: // �J�����_�[���[�h
        show_date();
        break;

    default:
        break;
    }
}

/* ���Ԃ�\������֐� */
void show_time(void)
{
    unsigned char time[3] = {second, minute, hour};
    show_values(time);
    DOT = (csec < 50) ? 0 : 1;
}

/* ���t��\������֐� */
void show_date(void)
{
    unsigned char date[3] = {day, month, year};
    show_values(date);
    DOT = 1;
}

/* �󂯎�����l��\������֐� (0: ����2��, 1: ����2��, 2: ���2��)*/
void show_values(unsigned char values[3])
{
    unsigned char bcd_code; // BCD�ɕϊ������R�[�h
    unsigned char compared; // ��r�p�̒l

    for (signed char i = 0; i < 3; i++)
    {
        // �V�t�g���Z�͈ÖٓI��int�ɂȂ��Ă����܂��̂� unsigned char �ɃL���X�g����
        bcd_code = (unsigned char)((values[i] / 10) << 4) + (values[i] % 10);
        compared = 0b00000001; // 1�r�b�g���Ƃɔ�r���s�����߂̒l

        switch (i)
        {
        case 0:
            for (unsigned char j = 0; j < 8; j++)
            {
                SER = (__bit)((bcd_code & compared) >> j); // �e�r�b�g��1��0�����L�^ (__bit�^�ɃL���X�g)
                LOWSRCLK = 1;                              // �����オ��Ńf�[�^�𑗂�
                LOWSRCLK = 0;
                compared = (unsigned char)(compared << 1);
            }
            break;

        case 1:
            for (unsigned char j = 0; j < 8; j++)
            {
                SER = (__bit)((bcd_code & compared) >> j); // �e�r�b�g��1��0�����L�^ (__bit�^�ɃL���X�g)
                MIDSRCLK = 1;
                MIDSRCLK = 0;
                compared = (unsigned char)(compared << 1);
            }
            break;

        case 2:
            for (unsigned char j = 0; j < 8; j++)
            {
                SER = (__bit)((bcd_code & compared) >> j); // �e�r�b�g��1��0�����L�^ (__bit�^�ɃL���X�g)
                UPPSRCLK = 1;
                UPPSRCLK = 0;
                compared = (unsigned char)(compared << 1);
            }
            break;

        default:
            break;
        }

        RCLK = 1; // �S�f�[�^�𑗂�I�������V�t�g���W�X�^�̒l���o�͂ɔ��f
        RCLK = 0;
    }
}

/* �j�L�V�[�ǂ����������邽�߂̊֐� 0: ����2�� 1: ����2�� 2: ���2�� 3: �S�� */
void sleep_nixie(unsigned char digit)
{
    DOT = 0; // dot������

    if (digit == 3 || digit == 0)
    {
        for (unsigned char i = 0; i < 8; i++)
        {
            SER = 1; // �S����1���������� PIC�̓r�b�g���Ƃ̏����������ł��Ȃ��̂Ŗ��񏑂�����
            LOWSRCLK = 1;
            LOWSRCLK = 0;
        }
    }

    if (digit == 3 || digit == 1)
    {
        for (unsigned char i = 0; i < 8; i++)
        {
            SER = 1;
            MIDSRCLK = 1;
            MIDSRCLK = 0;
        }
    }

    if (digit == 3 || digit == 2)
    {
        for (unsigned char i = 0; i < 8; i++)
        {
            SER = 1;
            UPPSRCLK = 1;
            UPPSRCLK = 0;
        }
    }

    RCLK = 1;
    RCLK = 0;
}

/* �`���^�����O��҂֐� */
void wait_chattering(void)
{
    __delay_ms(15);
}

/* TMR0�̊��荞�ݏ���(�ݒ茅��_��) */
void MYTMR0_ISR(void)
{
    if (!start)
    {
        if (is_light)
        {
            is_light = 0;
            sleep_nixie(selected_digit);
        }
        else
        {
            is_light = 1;
            show_nixie();
        }
    }
}

/* TMR2�̊��荞�ݏ���(1/100 s���ƂɎ��v���X�V) */
void MYTMR2_ISR(void)
{
    if (start)
    {
        // ���荞�݂�1/100 s����
        csec++;
        if (csec > 99)
        {
            csec = 0;
            second++;
        }
        if (second > 59)
        {
            second = 0;
            minute++;
        }
        if (minute > 59)
        {
            minute = 0;
            hour++;
        }
        if (hour > 23)
        {
            hour = 0;
            day++;
        }
        // 2���̏���(���邤�N�͍l���Ȃ�)
        if ((month == 2) && (day > 28))
        {
            month++;
            day = 1;
        }
        // 4,6,9,11���̏���
        if (((month == 4) || (month == 6) || (month == 9) || (month == 11)) && (day > 30))
        {
            month++;
            day = 1;
        }
        // 1,3,5,7,8,10���̏���
        if (((month == 1) || (month == 3) || (month == 5) || (month == 7) || (month == 8) || (month == 10)) && (day > 31))
        {
            month++;
            day = 1;
        }
        // 12���̏���
        if ((month == 12) && (day > 31))
        {
            month = 1;
            day = 1;
            year++;
        }
        if (year > 99)
        {
            year = 0;
        }
    }
}

/* +�{�^���̊��荞�ݏ���(�l�ݒ�t�F�[�Y�Ő��l�̑������s��) */
void PLUS_ISR(void)
{
    if (!start)
    {
        int count = 0;
        wait_chattering();
        if (PLUS)
        {
            up_value();
            show_nixie();

            // ���������m
            while (PLUS)
            {
                if (count++ > LONGPUSH)
                {
                    break;
                }
                __delay_ms(1);
            }

            // ���������̏���
            while (PLUS)
            {
                // �A�����グ
                up_value();
                show_nixie();
                __delay_ms(SKIPSPAN);
            }
        }
    }
}

/* ���݂̃��[�h�C�I�����ɉ����Ēl�𑝂₷�֐� */
void up_value(void)
{
    switch (mode)
    {
    case 0: // ���v���[�h
        switch (selected_digit)
        {
        case 0:
            (second < 59) ? second++ : (second = 0);
            break;

        case 1:
            (minute < 59) ? minute++ : (minute = 0);
            break;

        case 2:
            (hour < 23) ? hour++ : (hour = 0);
            break;

        default:
            break;
        }

    case 1: // �J�����_�[���[�h
        switch (selected_digit)
        {
        case 0:
            (day < 31) ? day++ : (day = 1);
            break;

        case 1:
            (month < 12) ? month++ : (month = 1);
            break;

        case 2:
            (year < 99) ? year++ : (year = 0);
            break;

        default:
            break;
        }
    }
}

/* -�{�^���̊��荞�ݏ���(�l�ݒ�t�F�[�Y�Ő��l�̌������s��) */
void MINUS_ISR(void)
{
    if (!start)
    {
        int count = 0;
        wait_chattering();
        if (MINUS)
        {
            down_value();
            show_nixie();

            // ���������m
            while (MINUS)
            {
                if (count++ > LONGPUSH)
                {
                    break;
                }
                __delay_ms(1);
            }

            // ���������̏���
            while (MINUS)
            {
                // �A�����グ
                down_value();
                show_nixie();
                __delay_ms(SKIPSPAN);
            }
        }
    }
}

/* ���݂̃��[�h�C�I�����ɉ����Ēl�����炷�֐� */
void down_value(void)
{
    switch (mode)
    {
    case 0: // ���v���[�h
        switch (selected_digit)
        {
        case 0:
            (second > 0) ? second-- : (second = 59);
            break;

        case 1:
            (minute > 0) ? minute-- : (minute = 59);
            break;

        case 2:
            (hour > 0) ? hour-- : (hour = 23);
            break;

        default:
            break;
        }

    case 1: // �J�����_�[���[�h
        switch (selected_digit)
        {
        case 0:
            (day > 1) ? day-- : (day = 31);
            break;

        case 1:
            (month > 1) ? month-- : (month = 12);
            break;

        case 2:
            (year > 0) ? year-- : (year = 99);
            break;

        default:
            break;
        }
    }
}

/* �I�����グ�{�^���̊��荞�ݏ���(�l�ݒ�t�F�[�Y�Őݒ茅�̑������s��) */
void UPDIGIT_ISR(void)
{
    if (!start)
    {
        wait_chattering();
        if (UPDIGIT)
        {
            (selected_digit < 2) ? selected_digit++ : (selected_digit = 0);
        }
    }
}

/* �I���������{�^���̊��荞�ݏ���(�l�ݒ�t�F�[�Y�Őݒ茅�̌������s��) */
void DOWNDIGIT_ISR(void)
{
    if (!start)
    {
        wait_chattering();
        if (DOWNDIGIT)
        {
            (selected_digit > 0) ? selected_digit-- : (selected_digit = 2);
        }
    }
}

/* ���[�h�؂�ւ��{�^���̊��荞�ݏ���(���[�h�̕ύX���s��(�j�L�V�[�Ǖ\�����̂�)) */
void SWITCH_ISR(void)
{
    wait_chattering();
    if (SWITCH && enable)
    {
        (mode < (MODENUM - 1)) ? mode++ : (mode = 0);
        show_nixie();
    }
}

/* �X�^�[�g�{�^���̊��荞�ݏ���(�ݒ�t�F�[�Y�ł̓X�^�[�g��, ����t�F�[�Y�ł͕\���̗L���̕ύX���s��) */
void START_EA_ISR(void)
{
    wait_chattering();
    if (START_EA)
    {
        start = 1; // ����J�n

        enable = !enable;

        if (!enable)
        {
            sleep_nixie(3);
        }
        else
        {
            show_nixie();
        }
    }
}

/**
 End of File
*/