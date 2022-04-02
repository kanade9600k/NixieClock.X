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
#define ENABLE RA6    // RA6��ENABLE�Ɩ��Â���
#define UPDIGIT RB0   // RB0��UPDIGIT�Ɩ��Â���
#define DOWNDIGIT RB1 // RB1��DOWNDIGIT�Ɩ��Â���
#define PLUS RB2      // RB2��PLUS�Ɩ��Â���
#define MINUS RB3     // RB3��MINUS�Ɩ��Â���
#define SWITCH RB4    // RB4��SWITCH�Ɩ��Â���
#define START_EA RB5  // RB5��START_EA�Ɩ��Â���
#define LONGPUSH 500  // ���������莞��(ms)
#define SKIPSPAN 100  // �X�L�b�v���ɉ�ms��1��J�E���g���邩�̎���
#define COUNTDOWN 0   // �J�E���g�_�E����\��
#define COUNTUP 1     // �J�E���g�A�b�v��\��
/*
                         Main application
 */

/* �֐��v���g�^�C�v�錾 */
void MYTMR2_ISR(void);                            // �^�C�}�[2�̊��荞�ݏ���(ISR : Interrupt Service Routin)
void PLUS_ISR(void);                              // RB0(+�{�^��)�̊��荞�ݏ���
void MINUS_ISR(void);                             // RB1(-�{�^��)�̊��荞�ݏ���
void UPDIGIT_ISR(void);                           // RB2(�ݒ茅�グ�{�^��)�̊��荞�ݏ���
void DOWNDIGIT_ISR(void);                         // RB3(�ݒ茅�����{�^��)�̊��荞�ݏ���
void SWITCH_ISR(void);                            // RB4(�ݒ胂�[�h�؂�ւ��{�^��)�̊��荞�ݏ���
void START_EA_ISR(void);                          // RB5(�X�^�[�g�E�C�l�[�u���{�^��)�̊��荞�ݏ���
void init_clock(void);                            // �^�C�}�[�̏������ݒ�
void mainApp(void);                               // ���C���֐���while���[�v�ł̏���
void write_74HC595(unsigned char, unsigned char); // 74HC595(�V�t�g���W�X�^)�ɏ����������ފ֐�
void wait_chattering(void);                       // �`���^�����O��҂֐�
void updown_second(unsigned char);                // �b��ݒ肷��֐�
void updown_minute(unsigned char);                // ����ݒ肷��֐�
void updown_hour(unsigned char);                  // ����ݒ肷��֐�
void updown_day(unsigned char);                   // ����ݒ肷��֐�
void updown_month(unsigned char);                 // ����ݒ肷��֐�
void updown_year(unsigned char);                  // �N��ݒ肷��֐�
void sleep_nixie(void);                           // �j�L�V�[�ǂ�S�������邽�߂̊֐�

/* �O���[�o���ϐ��錾 */
// �z�񂾂������̂�ϐ��ɕς���
// unsigned char time[6] = {0};                   // ���ԏ���ێ�����z�� (0..5 s,s,m,m,h,h)
// unsigned char date[6] = {0, 0, 0, 1, 0, 1};    // ���t����ێ�����z�� (0..5 d,d,m,m,y,y)
unsigned char second = 0;            // �b
unsigned char minute = 0;            // ��
unsigned char hour = 0;              // ��
unsigned char day = 1;               // ��
unsigned char month = 1;             // ��
unsigned char year = 0;              // �N
unsigned char write_lower = 0;       // ��2���������݃t���O
unsigned char write_middle = 0;      // ��2���������݃t���O
unsigned char write_upper = 0;       // ��2���������݃t���O
unsigned char mode = 0;              // 0 : ���v, 1 : �J�����_�[
unsigned char data = 0;              // �V�t�g���W�X�^�ɏ������ރf�[�^
unsigned char start = 0;             // �J�E���g�J�n�t���O
unsigned char selected_digit = 0;    // �ҏW���̌��������t���O 0 : ����2��, 1 : ����2��, 2 : ���2��
unsigned int count = 0;              // ���L�̃J�E���^ �g������0�ɖ߂��܂��傤
unsigned char lower = 0;             // �V�t�g���W�X�^���͂̉��ʌ�
unsigned char upper = 0;             // �V�t�g���W�X�^���͂̏�ʌ�
unsigned char bcd_code = 0;          // �V�t�g���W�X�^���͗pBCD�R�[�h
unsigned char compared = 0b00000001; // 1�r�b�g���Ƃɔ�r���s�����߂̒l

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
    ENABLE = 1;
    RA7 = 0;
    RB6 = 0;
    RB7 = 0;
    while (!start)
    {
        ; // �������͂��ׂĊ��荞�݂ōs���̂ŉ������Ȃ�
    }
}

/* while���̃��C������ */
void mainApp(void)
{
    if (ENABLE)
    {
        if (write_lower)
        {
            write_lower = 0;            // �������݃t���O���~�낷
            data = mode ? day : second; // mode 0 : ���v, 1 : �J�����_�[
            write_74HC595(data, 0);
        }
        if (write_middle)
        {
            write_middle = 0; // �������݃t���O���~�낷
            data = mode ? month : minute;
            write_74HC595(data, 1);
        }
        if (write_upper)
        {
            write_upper = 0; // �������݃t���O���~�낷
            data = mode ? year : hour;
            write_74HC595(data, 2);
        }
        if (!ENABLE)
        { // �r���Ŋ��荞�݂����������Ƃ��̏���
            sleep_nixie();
        }
    }
    //    else {
    //        sleep_nixie();
    //    }
}

/* 74HC595(�V�t�g���W�X�^)�ɏ����������ފ֐� */
// select�͂ǂ̃V�t�g���W�X�^�ɏ������ނ�(0 : ����, 1 : ����, 2 : ���)
void write_74HC595(unsigned char data, unsigned char select)
{
    upper = data / 10;
    lower = data % 10;
    // �V�t�g���Z�͈ÖٓI��int�ɂȂ��Ă����܂��̂� unsigned char �ɃL���X�g����
    bcd_code = (unsigned char)(upper << 4) + lower;
    for (unsigned char i = 0; i < 8; i++)
    {
        SER = (__bit)((bcd_code & compared) >> i); // �e�r�b�g��1��0�����L�^ (__bit�^�ɃL���X�g)
        switch (select)
        {
        case 0:
            LOWSRCLK = 1; // �����オ��Ńf�[�^�𑗂�
            LOWSRCLK = 0;
            break;
        case 1:
            MIDSRCLK = 1;
            MIDSRCLK = 0;
            break;
        case 2:
            UPPSRCLK = 1;
            UPPSRCLK = 0;
            break;
        default:
            break;
        }
        compared = (unsigned char)(compared << 1);
    }
    compared = 0b00000001;
    RCLK = 1; // �S�f�[�^�𑗂�I�������V�t�g���W�X�^�̒l���o�͂ɔ��f
    RCLK = 0;
}

/* �j�L�V�[�ǂ�S���������邽�߂̊֐� */
void sleep_nixie(void)
{
    for (unsigned char i = 0; i < 8; i++)
    {
        SER = 1; // �S����1���������� PIC�̓r�b�g���Ƃ̏����������ł��Ȃ��̂Ŗ��񏑂�����
        LOWSRCLK = 1;
        LOWSRCLK = 0;
        SER = 1;
        MIDSRCLK = 1;
        MIDSRCLK = 0;
        SER = 1;
        UPPSRCLK = 1;
        UPPSRCLK = 0;
    }
    RCLK = 1;
    RCLK = 0;
}

/* �`���^�����O��҂֐� */
void wait_chattering(void)
{
    __delay_ms(15);
}

/* �b��ݒ肷��֐� */
void updown_second(unsigned char is_up)
{
    is_up ? second++ : second--;
    if (second >= 60)
    {
        second = is_up ? 0 : 59;
    }
    write_74HC595(second, 0);
}

/* ����ݒ肷��֐� */
void updown_minute(unsigned char is_up)
{
    is_up ? minute++ : minute--;
    if (minute >= 60)
    {
        minute = is_up ? 0 : 59;
    }
    write_74HC595(minute, 1);
}

/* ����ݒ肷��֐� */
void updown_hour(unsigned char is_up)
{
    is_up ? hour++ : hour--;
    if (hour >= 24)
    {
        hour = is_up ? 0 : 23;
    }
    write_74HC595(hour, 2);
}

/* ����ݒ肷��֐� */
// ���� 2��31���Ȃǂ̑��݂��Ȃ��������ݒ�ł��Ă��܂��܂��������炭�J�E���g�A�b�v���n�܂�ƏC�������H
// 1~31�͈̔͂ŕω������܂�
void updown_day(unsigned char is_up)
{
    is_up ? day++ : day--;
    if (day >= 32)
    {
        day = is_up ? 1 : 31; // �O�̂��߃J�E���g�_�E�����̃��Z�b�g�����킹�ď���
    }
    if (!(is_up) && (day == 0))
    {
        day = 31;
    }
    write_74HC595(day, 0);
}

/* ����ݒ肷��֐� */
// 1~12�͈̔͂ŕω������܂�
void updown_month(unsigned char is_up)
{
    is_up ? month++ : month--;
    if (month >= 13)
    {
        month = is_up ? 1 : 12; // �O�̂���
    }
    if (!(is_up) && (month == 0))
    {
        month = 12;
    }
    write_74HC595(month, 1);
}

/* �N��ݒ肷��֐� */
void updown_year(unsigned char is_up)
{
    is_up ? year++ : year--;
    if (year >= 100)
    {
        year = is_up ? 0 : 99;
    }
    write_74HC595(year, 2);
}

/* TMR2�̊��荞�ݏ���(1s���ƂɎ��v���X�V) */
void MYTMR2_ISR(void)
{
    if (start)
    {
        // ���荞�݂�1s����
        second++;
        write_lower = 1; // ��2���������ݐM��
        if (second >= 60)
        {
            second = 0;
            minute++;
            write_middle = 1; // ��2���������ݐM��
        }
        if (minute >= 60)
        {
            minute = 0;
            hour++;
            write_upper = 1; // ��2���������ݐM��
        }
        if (hour >= 24)
        {
            hour = 0;
            day++;
        }
        // 2���̏���(���邤�N�͍l���Ȃ�)
        if ((month == 2) && (day >= 29))
        {
            month++;
            day = 1;
        }
        // 4,6,9,11���̏���
        if (((month == 4) || (month == 6) || (month == 9) || (month == 11)) && (day >= 31))
        {
            month++;
            day = 1;
        }
        // 1,3,5,7,9,10���̏���
        if (((month == 1) || (month == 3) || (month == 5) || (month == 7) || (month == 9) || (month == 10)) && (day >= 32))
        {
            month++;
            day = 1;
        }
        // 12���̏���
        if ((month == 12) && (day >= 32))
        {
            month = 1;
            day = 1;
            year++;
        }
        if (year >= 100)
        {
            year = 0;
        }
    }
}

/* RB0�̊��荞�ݏ���(�l�ݒ�t�F�[�Y�Ő��l�̑������s��) */
void PLUS_ISR(void)
{
    if (!start)
    {
        wait_chattering();
        if (PLUS)
        {
            if (!mode)
            { // ���v���[�h
                switch (selected_digit)
                {
                case 0:
                    updown_second(COUNTUP);
                    // �������ŘA���グ
                    while (PLUS)
                    {
                        if (LONGPUSH <= ++count)
                        {
                            // ���������[�h�˓�
                            __delay_ms(SKIPSPAN);
                            updown_second(COUNTUP);
                        }
                        else
                        {
                            __delay_ms(1);
                        }
                    }
                    count = 0;
                    break;
                case 1:
                    updown_minute(COUNTUP);
                    while (PLUS)
                    {
                        if (LONGPUSH <= ++count)
                        {
                            // ���������[�h�˓�
                            __delay_ms(SKIPSPAN);
                            updown_minute(COUNTUP);
                        }
                        else
                        {
                            __delay_ms(1);
                        }
                    }
                    count = 0;
                    break;
                case 2:
                    updown_hour(COUNTUP);
                    // �������ŘA���グ
                    while (PLUS)
                    {
                        if (LONGPUSH <= ++count)
                        {
                            // ���������[�h�˓�
                            __delay_ms(SKIPSPAN);
                            updown_hour(COUNTUP);
                        }
                        else
                        {
                            __delay_ms(1);
                        }
                    }
                    count = 0;
                    break;
                default:
                    break;
                }
            }
            else
            { // �J�����_�[���[�h
                switch (selected_digit)
                {
                case 0:
                    updown_day(COUNTUP);
                    // �������ŘA���グ
                    while (PLUS)
                    {
                        if (LONGPUSH <= ++count)
                        {
                            // ���������[�h�˓�
                            __delay_ms(SKIPSPAN);
                            updown_day(COUNTUP);
                        }
                        else
                        {
                            __delay_ms(1);
                        }
                    }
                    count = 0;
                    break;
                case 1:
                    updown_month(COUNTUP);
                    while (PLUS)
                    {
                        if (LONGPUSH <= ++count)
                        {
                            // ���������[�h�˓�
                            __delay_ms(SKIPSPAN);
                            updown_month(COUNTUP);
                        }
                        else
                        {
                            __delay_ms(1);
                        }
                    }
                    count = 0;
                    break;
                case 2:
                    updown_year(COUNTUP);
                    while (PLUS)
                    {
                        if (LONGPUSH <= ++count)
                        {
                            // ���������[�h�˓�
                            __delay_ms(SKIPSPAN);
                            updown_year(COUNTUP);
                        }
                        else
                        {
                            __delay_ms(1);
                        }
                    }
                    count = 0;
                    break;
                default:
                    break;
                }
            }
        }
    }
}

/* RB1�̊��荞�ݏ���(�l�ݒ�t�F�[�Y�Ő��l�̌������s��) */
void MINUS_ISR(void)
{
    if (!start)
    {
        wait_chattering();
        if (MINUS)
        {
            if (!mode)
            { // ���v���[�h
                switch (selected_digit)
                {
                case 0:
                    updown_second(COUNTDOWN);
                    // �������ŘA������
                    while (MINUS)
                    {
                        if (LONGPUSH <= ++count)
                        {
                            // ���������[�h�˓�
                            __delay_ms(SKIPSPAN);
                            updown_second(COUNTDOWN);
                        }
                        else
                        {
                            __delay_ms(1);
                        }
                    }
                    count = 0;
                    break;
                case 1:
                    updown_minute(COUNTDOWN);
                    while (MINUS)
                    {
                        if (LONGPUSH <= ++count)
                        {
                            // ���������[�h�˓�
                            __delay_ms(SKIPSPAN);
                            updown_minute(COUNTDOWN);
                        }
                        else
                        {
                            __delay_ms(1);
                        }
                    }
                    count = 0;
                    break;
                case 2:
                    updown_hour(COUNTDOWN);
                    while (MINUS)
                    {
                        if (LONGPUSH <= ++count)
                        {
                            // ���������[�h�˓�
                            __delay_ms(SKIPSPAN);
                            updown_hour(COUNTDOWN);
                        }
                        else
                        {
                            __delay_ms(1);
                        }
                    }
                    count = 0;
                    break;
                default:
                    break;
                }
            }
            else
            { // �J�����_�[���[�h
                switch (selected_digit)
                {
                case 0:
                    updown_day(COUNTDOWN);
                    // �������ŘA������
                    while (MINUS)
                    {
                        if (LONGPUSH <= ++count)
                        {
                            // ���������[�h�˓�
                            __delay_ms(SKIPSPAN);
                            updown_day(COUNTDOWN);
                        }
                        else
                        {
                            __delay_ms(1);
                        }
                    }
                    count = 0;
                    break;
                case 1:
                    updown_month(COUNTDOWN);
                    while (MINUS)
                    {
                        if (LONGPUSH <= ++count)
                        {
                            // ���������[�h�˓�
                            __delay_ms(SKIPSPAN);
                            updown_month(COUNTDOWN);
                        }
                        else
                        {
                            __delay_ms(1);
                        }
                    }
                    count = 0;
                    break;
                case 2:
                    updown_year(COUNTDOWN);
                    while (MINUS)
                    {
                        if (LONGPUSH <= ++count)
                        {
                            // ���������[�h�˓�
                            __delay_ms(SKIPSPAN);
                            updown_year(COUNTDOWN);
                        }
                        else
                        {
                            __delay_ms(1);
                        }
                    }
                    count = 0;
                    break;
                default:
                    break;
                }
            }
        }
    }
}

/* RB2�̊��荞�ݏ���(�l�ݒ�t�F�[�Y�Őݒ茅�̑������s��) */
void UPDIGIT_ISR(void)
{
    if (!start)
    {
        wait_chattering();
        if (UPDIGIT)
        {
            selected_digit++;
            if (selected_digit >= 3)
            {
                selected_digit = 0;
            }
        }
    }
}

/* RB3�̊��荞�ݏ���(�l�ݒ�t�F�[�Y�Őݒ茅�̌������s��) */
void DOWNDIGIT_ISR(void)
{
    if (!start)
    {
        wait_chattering();
        if (DOWNDIGIT)
        {
            selected_digit--;
            if (selected_digit >= 3)
            {
                selected_digit = 2;
            }
        }
    }
}

/* RB4�̊��荞�ݏ���(���[�h�̕ύX���s��) */
void SWITCH_ISR(void)
{
    wait_chattering();
    if (SWITCH)
    {
        mode = !mode;
        if (ENABLE)
        {
            write_74HC595(mode ? day : second, 0);
            write_74HC595(mode ? month : minute, 1);
            write_74HC595(mode ? year : hour, 2);
        }
    }
}

/* RB5�̊��荞�ݏ���(�ݒ�t�F�[�Y�ł̓X�^�[�g��, ����t�F�[�Y�ł͕\���̗L���̕ύX���s��) */
void START_EA_ISR(void)
{
    wait_chattering();
    if (START_EA)
    {
        if (!start)
        {
            start = 1;
        }
        else
        {
            ENABLE = !ENABLE;
            if (!ENABLE)
            {
                sleep_nixie();
            }
            else
            {
                write_74HC595(mode ? day : second, 0);
                write_74HC595(mode ? month : minute, 1);
                write_74HC595(mode ? year : hour, 2);
            }
        }
    }
}

/**
 End of File
*/