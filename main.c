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
#define SER RA0         // RA0��SER�Ɩ��Â���
#define RCLK RA1        // RA1��RCLK�Ɩ��Â���
#define UPPSRCLK RA2    // RA2��UPPSRCLK�Ɩ��Â���
#define MIDSRCLK RA3    // RA3��MIDSRCLK�Ɩ��Â���
#define LOWSRCLK RA4    // RA4��LOWSRCLK�Ɩ��Â���
#define DOT RA6         // RA6��DOT�Ɩ��Â���
#define LEFT RB0        // RB0��LEFT�Ɩ��Â���
#define RIGHT RB1       // RB1��RIGHT�Ɩ��Â���
#define PLUS RB2        // RB2��PLUS�Ɩ��Â���
#define MINUS RB3       // RB3��MINUS�Ɩ��Â���
#define RST RB4         // RB4��RST�Ɩ��Â���
#define START_STOP RB5  // RB5��START_STOP�Ɩ��Â���
#define BLINKINGSPAN 50 // �_�ŊԊu(cs)
#define LONGPUSH 1500   // ���������莞��(ms)
#define PM_LONGPUSH 500 // �A�����グ���莞��(ms)(+, -�{�^���̒��������莞��)
#define SKIPSPAN 50     // �{�^���������̃X�L�b�v���ɉ�ms��1��J�E���g���邩�̎���(ms)

/*
                         Main application
 */

/* �� */
typedef enum
{
    CSEC = 0,
    SECOND,
    MINUTE,
    HOUR,
    TIME_NUM,
} TIME;

typedef enum
{
    LOWER = 0,
    MIDDLE,
    UPPER,
    ALL,
} DIGIT;
typedef enum
{
    CLOCK = 0,
    CALENDAR,
    TIMER,
    STOPWATCH_S,
    STOPWATCH_L,
    MODE_NUM,
} MODE;

typedef enum
{
    SHORT = 0,
    LONG,
    NG,
} PUSH_TYPE;

/* �֐��v���g�^�C�v�錾 */
/* ���荞�݊֐� */
void MYTMR2_ISR(void);     // �^�C�}�[2�̊��荞�ݏ���(ISR : Interrupt Service Routin)
void PLUS_ISR(void);       // +�{�^���̊��荞�ݏ���
void MINUS_ISR(void);      // -�{�^���̊��荞�ݏ���
void LEFT_ISR(void);       // ���{�^���̊��荞�ݏ���
void RIGHT_ISR(void);      // �E�{�^���̊��荞�ݏ���
void RST_ISR(void);        // ���l���Z�b�g�{�^���̊��荞�ݏ���
void START_STOP_ISR(void); // �X�^�[�g�E�X�g�b�v�{�^���̊��荞�ݏ���

/* ����֐� */
void mainApp(void);         // ���C���֐���while���[�v�ł̏���
void mode_clock(void);      // ���v���[�h�̊֐�
void mode_calendar(void);   // �J�����_�[���[�h�̊֐�
void mode_timer(void);      // �^�C�}�[���[�h�̊֐�
void mode_stopwatch(void);  // �X�g�b�v�E�H�b�`���[�h�̊֐�
void wait_chattering(void); // �`���^�����O��҂֐�
void show_nixie(void);      // ���݂̃��[�h�ɍ��킹���l��\������֐�
// void show_time(void);                     // ���Ԃ�\������֐�
// void show_date(void);                     // ���t��\������֐�
void sleep_nixie(DIGIT);                  // �j�L�V�[�ǂ��������邽�߂̊֐�
void show_values(unsigned char[]);        // �l��\������֐�
void setting_date(void);                  // ���t�ݒ�֐�
void setting_time(void);                  // �����ݒ�֐�
void up_digit(void);                      // �I�������グ��֐�
void down_digit(void);                    // �I������������֐�
void blinking_digit(void);                // �I������_�ł�����֐�
void up_mode(void);                       // ���[�h��1�i�߂�֐�
void down_mode(void);                     // ���[�h��1�߂��֐�
void incr_selected_value(void);           // ���݂̃��[�h�C�I�����ɍ��킹�Ēl�𑝉�������֐�
void decr_selected_value(void);           // ���݂̃��[�h�C�I�����ɍ��킹�Ēl������������֐�
PUSH_TYPE get_start_stop_push_type(void); // �X�^�[�g�E�X�g�b�v�{�^���̉��������m�֐�
PUSH_TYPE get_reset_push_type(void);      // ���l���Z�b�g�{�^���̉��������m�֐�
PUSH_TYPE get_left_push_type(void);       // ���{�^���̉��������m�֐�
PUSH_TYPE get_right_push_type(void);      // �E�{�^���̉��������m�֐�
PUSH_TYPE get_plus_push_type(void);       // +�{�^���̉��������m�֐�
PUSH_TYPE get_minus_push_type(void);      // -�{�^���̉��������m�֐�

/* �O���[�o���ϐ��錾 */
unsigned char clock_time[TIME_NUM] = {0};     // ���v�̎���
unsigned char timer_time[TIME_NUM] = {0};     // �^�C�}�[�̎���
unsigned char stopwatch_time[TIME_NUM] = {0}; // �X�g�b�v�E�H�b�`�̎���
// unsigned char csec = 0;                       // �Z���`�b(1/100�b)
// unsigned char second = 0;                     // �b
// unsigned char minute = 0;                     // ��
// unsigned char hour = 0;                       // ��
unsigned char day = 1;                  // ��
unsigned char month = 1;                // ��
unsigned char year = 0;                 // �N
DIGIT selected_digit = LOWER;           // �I�𒆂̌�
MODE mode = CALENDAR;                   // ���݂̃��[�h
unsigned char is_clock_active = 0;      // ���v���쓮�����ۂ�
unsigned char is_timer_active = 0;      // �^�C�}�[���쓮�����ۂ�
unsigned char is_timer_end = 1;         // �^�C�}�[�̐ݒ莞�ԂɒB�������ۂ�
unsigned char is_stopwatch_active = 0;  // �X�g�b�v�E�H�b�`���쓮�����ۂ�
unsigned char is_tmr2_interrupt = 0;    // �^�C�}�[2�̊��荞�݂��N��������
int tmr2_count = 0;                     // �^�C�}�[2�̊��荞�݃J�E���^
unsigned char is_start_stop_pushed = 0; // �X�^�[�g�E�X�g�b�v�{�^���������ꂽ��
unsigned char is_reset_pushed = 0;      // ���l���Z�b�g�{�^���������ꂽ��
unsigned char is_left_pushed = 0;       // ���{�^���������ꂽ��
unsigned char is_right_pushed = 0;      // �E�{�^���������ꂽ��
unsigned char is_plus_pushed = 0;       // �v���X�{�^���������ꂽ��
unsigned char is_minus_pushed = 0;      // �}�C�i�X�{�^���������ꂽ��
unsigned char is_nixie_on = 1;          // �j�L�V�[�ǂ̕\���̉� 0: ����, 1: �_��

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
    IOCBF0_SetInterruptHandler(LEFT_ISR);
    // RB1�̊��荞�ݎ��ɌĂяo�����֐�
    IOCBF1_SetInterruptHandler(RIGHT_ISR);
    // RB2�̊��荞�ݎ��ɌĂяo�����֐�
    IOCBF2_SetInterruptHandler(PLUS_ISR);
    // RB3�̊��荞�ݎ��ɌĂяo����鏈��
    IOCBF3_SetInterruptHandler(MINUS_ISR);
    // RB4�̊��荞�ݎ��ɌĂяo����鏈��
    IOCBF4_SetInterruptHandler(RST_ISR);
    // RB5�̊��荞�ݎ��ɌĂяo����鏈��
    IOCBF5_SetInterruptHandler(START_STOP_ISR);

    // Disable the Global Interrupts
    // INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    // INTERRUPT_PeripheralInterruptDisable();

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

    mode = CALENDAR;
    setting_date();

    mode = CLOCK;
    setting_time();
    is_clock_active = 1;

    while (1)
    {
        // Add your application code
        mainApp(); // while���̃��C��������mainApp()���ɋL�q����
    }
}

/* while���̃��C������ */
void mainApp(void)
{
    if (is_nixie_on)
    {
        show_nixie();
    }
    else
    {
        sleep_nixie(ALL);
    }

    switch (mode)
    {
    case CLOCK:
        mode_clock();
        break;

    case CALENDAR:
        mode_calendar();
        break;

    case TIMER:
        mode_timer();
        break;

    case STOPWATCH_S:
        mode_stopwatch();
        break;

    case STOPWATCH_L:
        mode_stopwatch();
        break;

    case MODE_NUM:
        break;

    default:
        break;
    }
}

/* ���v���[�h */
void mode_clock(void)
{
    if (is_start_stop_pushed)
    {
        switch (get_start_stop_push_type())
        {
        case SHORT:
            is_nixie_on = 1; // �_��
            break;

        case LONG:
            is_nixie_on = 0; // ����

        case NG:
            break;

        default:
            break;
        }
    }

    if (is_reset_pushed)
    {
        switch (get_reset_push_type())
        {
        case SHORT:
            break;

        case LONG:
            is_clock_active = 0; // ��U���v���~�߂�
            setting_time();
            is_clock_active = 1;
            break;

        case NG:
            break;

        default:
            break;
        }
    }

    if (is_left_pushed)
    {
        if (get_left_push_type() != NG)
        {
            up_mode();
        }
    }

    if (is_right_pushed)
    {
        if (get_right_push_type() != NG)
        {
            down_mode();
        }
    }
}

/* �J�����_�[���[�h */
void mode_calendar(void)
{
    if (is_start_stop_pushed)
    {
        switch (get_start_stop_push_type())
        {
        case SHORT:
            is_nixie_on = 1; // �_��
            break;

        case LONG:
            is_nixie_on = 0; // ����

        case NG:
            break;

        default:
            break;
        }
    }

    if (is_reset_pushed)
    {
        switch (get_reset_push_type())
        {
        case SHORT:
            break;

        case LONG:
            setting_date();

        case NG:
            break;

        default:
            break;
        }
    }

    if (is_left_pushed)
    {
        if (get_left_push_type() != NG)
        {
            up_mode();
        }
    }

    if (is_right_pushed)
    {
        if (get_right_push_type() != NG)
        {
            down_mode();
        }
    }
}

/* �^�C�}�[���[�h */
void mode_timer(void)
{
    if (is_start_stop_pushed)
    {
        switch (get_start_stop_push_type())
        {
        case SHORT:
            is_nixie_on = 1;                    // �_��
            is_timer_active = !is_timer_active; // �X�^�[�g�E�X�g�b�v
            break;

        case LONG:
            is_nixie_on = 0; // ����

        case NG:
            break;

        default:
            break;
        }
    }

    if (is_reset_pushed)
    {
        switch (get_reset_push_type())
        {
        case SHORT:
            // �^�C�}�[���Z�b�g
            timer_time[CSEC] = 0;
            timer_time[SECOND] = 0;
            timer_time[MINUTE] = 0;
            timer_time[HOUR] = 0;
            is_timer_end = 1;
            is_timer_active = 0;
            break;

        case LONG:
            setting_time();
            is_timer_end = 0;
            is_timer_active = 1;
            break;

        case NG:
            break;

        default:
            break;
        }
    }

    if (is_left_pushed)
    {
        if (get_left_push_type() != NG)
        {
            up_mode();
        }
    }

    if (is_right_pushed)
    {
        if (get_right_push_type() != NG)
        {
            down_mode();
        }
    }
}

/* �X�g�b�v�E�H�b�`���[�h */
void mode_stopwatch(void)
{
    if (is_start_stop_pushed)
    {
        switch (get_start_stop_push_type())
        {
        case SHORT:
            is_nixie_on = 1;                            // �_��
            is_stopwatch_active = !is_stopwatch_active; // �X�^�[�g�E�X�g�b�v
            break;

        case LONG:
            is_nixie_on = 0; // ����

        case NG:
            break;

        default:
            break;
        }
    }

    if (is_reset_pushed)
    {
        switch (get_reset_push_type())
        {
        case SHORT:
            // �X�g�b�v�E�H�b�`���Z�b�g
            stopwatch_time[CSEC] = 0;
            stopwatch_time[SECOND] = 0;
            stopwatch_time[MINUTE] = 0;
            stopwatch_time[HOUR] = 0;
            is_stopwatch_active = 0;
            break;

        case LONG:
            break;

        case NG:
            break;

        default:
            break;
        }
    }

    if (is_left_pushed)
    {
        if (get_left_push_type() != NG)
        {
            up_mode();
        }
    }

    if (is_right_pushed)
    {
        if (get_right_push_type() != NG)
        {
            down_mode();
        }
    }
}

/* ���t�ݒ� */
void setting_date(void)
{
    show_nixie();

    while (1)
    {
        // ���[�v�𔲂���
        if (is_start_stop_pushed)
        {
            if (get_start_stop_push_type() != NG)
            {
                break;
            }
        }

        // �I�����グ�̏���
        if (is_left_pushed)
        {
            if (get_left_push_type() != NG)
            {
                up_digit();
            }
        }

        // �I���������̏���
        if (is_right_pushed)
        {
            if (get_right_push_type() != NG)
            {
                down_digit();
            }
        }

        // �I�����̐��l�グ����
        if (is_plus_pushed)
        {
            switch (get_plus_push_type())
            {
            case SHORT:
                incr_selected_value();
                show_nixie();
                break;

            case LONG:
                while (PLUS)
                {
                    // �A�����グ
                    incr_selected_value();
                    show_nixie();
                    __delay_ms(SKIPSPAN);
                }

            case NG:
                break;

            default:
                break;
            }
        }

        // �I�����̐��l��������
        if (is_minus_pushed)
        {
            switch (get_minus_push_type())
            {
            case SHORT:
                decr_selected_value();
                show_nixie();
                break;

            case LONG:
                while (MINUS)
                {
                    // �A��������
                    decr_selected_value();
                    show_nixie();
                    __delay_ms(SKIPSPAN);
                }

            case NG:
                break;

            default:
                break;
            }
        }

        // �I�����̓_�ŏ���
        if (is_tmr2_interrupt)
        {
            blinking_digit();
        }
    }
}

/* �����ݒ�֐� */
void setting_time(void)
{
    show_nixie();

    while (1)
    {
        // ���[�v�𔲂���
        if (is_start_stop_pushed)
        {
            if (get_start_stop_push_type() != NG)
            {
                break;
            }
        }

        // �I�����グ�̏���
        if (is_left_pushed)
        {
            if (get_left_push_type() != NG)
            {
                up_digit();
            }
        }

        // �I���������̏���
        if (is_right_pushed)
        {
            if (get_right_push_type() != NG)
            {
                down_digit();
            }
        }

        // �I�����̐��l�グ����
        if (is_plus_pushed)
        {
            switch (get_plus_push_type())
            {
            case SHORT:
                incr_selected_value();
                show_nixie();
                break;

            case LONG:
                while (PLUS)
                {
                    // �A�����グ
                    incr_selected_value();
                    show_nixie();
                    __delay_ms(SKIPSPAN);
                }

            case NG:
                break;

            default:
                break;
            }
        }

        // �I�����̐��l��������
        if (is_minus_pushed)
        {
            switch (get_minus_push_type())
            {
            case SHORT:
                decr_selected_value();
                show_nixie();
                break;

            case LONG:
                while (MINUS)
                {
                    // �A��������
                    decr_selected_value();
                    show_nixie();
                    __delay_ms(SKIPSPAN);
                }

            case NG:
                break;

            default:
                break;
            }
        }
        // �I�����̓_�ŏ���
        if (is_tmr2_interrupt)
        {
            blinking_digit();
        }
    }
}

/* ���݂̃��[�h�ɍ��킹���l��\������֐� */
void show_nixie(void)
{
    switch (mode)
    {
    case CLOCK:
    {
        // {}�Ńu���b�N�����Ȃ���switch�����ł̕ϐ���`���ł��Ȃ�
        unsigned char time[3] = {clock_time[SECOND], clock_time[MINUTE], clock_time[HOUR]};
        show_values(time);
        DOT = (clock_time[CSEC] < 50) ? 0 : 1;
    }
    break;

    case CALENDAR:
    {
        unsigned char date[3] = {day, month, year};
        show_values(date);
        DOT = 1;
    }
    break;

    case TIMER:
    {
        unsigned char time[3] = {timer_time[SECOND], timer_time[MINUTE], timer_time[HOUR]};
        show_values(time);
        DOT = (timer_time[CSEC] < 50) ? 0 : 1;
    }
    break;

    case STOPWATCH_S:
    {
        unsigned char time[3] = {stopwatch_time[CSEC], stopwatch_time[SECOND], stopwatch_time[MINUTE]};
        show_values(time);
        DOT = (stopwatch_time[CSEC] < 50) ? 0 : 1;
    }
    break;

    case STOPWATCH_L:
    {
        unsigned char time[3] = {stopwatch_time[SECOND], stopwatch_time[MINUTE], stopwatch_time[HOUR]};
        show_values(time);
        DOT = (stopwatch_time[CSEC] < 50) ? 0 : 1;
    }
    break;

    case MODE_NUM:
        break;

    default:
        break;
    }
}

// /* ���Ԃ�\������֐� */
// void show_time(void)
// {
//     unsigned char time[3] = {second, minute, hour};
//     show_values(time);
//     DOT = (csec < 50) ? 0 : 1;
// }

// /* ���t��\������֐� */
// void show_date(void)
// {
//     unsigned char date[3] = {day, month, year};
//     show_values(date);
//     DOT = 1;
// }

/* �󂯎�����l��\������֐� (0: ����2��, 1: ����2��, 2: ���2��)*/
void show_values(unsigned char values[3])
{
    unsigned char bcd_code; // BCD�ɕϊ������R�[�h
    unsigned char compared; // ��r�p�̒l

    for (DIGIT digit = LOWER; digit <= UPPER; digit++)
    {
        // �V�t�g���Z�͈ÖٓI��int�ɂȂ��Ă����܂��̂� unsigned char �ɃL���X�g����
        bcd_code = (unsigned char)((values[digit] / 10) << 4) + (values[digit] % 10);
        compared = 0b00000001; // 1�r�b�g���Ƃɔ�r���s�����߂̒l

        switch (digit)
        {
        case LOWER:
            for (unsigned char j = 0; j < 8; j++)
            {
                SER = (__bit)((bcd_code & compared) >> j); // �e�r�b�g��1��0�����L�^ (__bit�^�ɃL���X�g)
                LOWSRCLK = 1;                              // �����オ��Ńf�[�^�𑗂�
                LOWSRCLK = 0;
                compared = (unsigned char)(compared << 1);
            }
            break;

        case MIDDLE:
            for (unsigned char j = 0; j < 8; j++)
            {
                SER = (__bit)((bcd_code & compared) >> j); // �e�r�b�g��1��0�����L�^ (__bit�^�ɃL���X�g)
                MIDSRCLK = 1;
                MIDSRCLK = 0;
                compared = (unsigned char)(compared << 1);
            }
            break;

        case UPPER:
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
void sleep_nixie(DIGIT digit)
{
    DOT = 0; // dot������

    if (digit == ALL || digit == LOWER)
    {
        for (unsigned char i = 0; i < 8; i++)
        {
            SER = 1; // �S����1���������� PIC�̓r�b�g���Ƃ̏����������ł��Ȃ��̂Ŗ��񏑂�����
            LOWSRCLK = 1;
            LOWSRCLK = 0;
        }
    }

    if (digit == ALL || digit == MIDDLE)
    {
        for (unsigned char i = 0; i < 8; i++)
        {
            SER = 1;
            MIDSRCLK = 1;
            MIDSRCLK = 0;
        }
    }

    if (digit == ALL || digit == UPPER)
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

/* �I�������グ��֐� */
void up_digit(void)
{
    (selected_digit < UPPER) ? selected_digit++ : (selected_digit = LOWER);
}

/* �I������������֐� */
void down_digit(void)
{
    (selected_digit > LOWER) ? selected_digit-- : (selected_digit = UPPER);
}

/* �I�����̂�_�ł�����֐� */
void blinking_digit(void)
{
    is_tmr2_interrupt = 0;
    tmr2_count++;

    {
        if (tmr2_count < BLINKINGSPAN)
        {
            show_nixie();
        }
        else if (tmr2_count < 2 * BLINKINGSPAN)
        {
            sleep_nixie(selected_digit);
        }
        else
        {
            tmr2_count = 0;
        }
    }
}

/* ���[�h��1�i�߂�֐� */
void up_mode(void)
{
    (mode < (MODE_NUM - 1)) ? mode++ : (mode = 0);
}

/* ���[�h��1�߂��֐� */
void down_mode(void)
{
    (mode > 0) ? mode-- : (mode = (MODE_NUM - 1));
}

/* ���݂̃��[�h�C�I�����ɉ����Ēl�𑝂₷�֐� */
void incr_selected_value(void)
{
    switch (mode)
    {
    case CLOCK:
        switch (selected_digit)
        {
        case LOWER:
            (clock_time[SECOND] < 59) ? clock_time[SECOND]++ : (clock_time[SECOND] = 0);
            break;

        case MIDDLE:
            (clock_time[MINUTE] < 59) ? clock_time[MINUTE]++ : (clock_time[MINUTE] = 0);
            break;

        case UPPER:
            (clock_time[HOUR] < 23) ? clock_time[HOUR]++ : (clock_time[HOUR] = 0);
            break;

        default:
            break;
        }
        break;

    case CALENDAR:
        switch (selected_digit)
        {
        case LOWER:
            (day < 31) ? day++ : (day = 1);
            break;

        case MIDDLE:
            (month < 12) ? month++ : (month = 1);
            break;

        case UPPER:
            (year < 99) ? year++ : (year = 0);
            break;

        default:
            break;
        }
        break;

    case TIMER:
        switch (selected_digit)
        {
        case LOWER:
            (timer_time[SECOND] < 59) ? timer_time[SECOND]++ : (timer_time[SECOND] = 0);
            break;

        case MIDDLE:
            (timer_time[MINUTE] < 59) ? timer_time[MINUTE]++ : (timer_time[MINUTE] = 0);
            break;

        case UPPER:
            (timer_time[HOUR] < 23) ? timer_time[HOUR]++ : (timer_time[HOUR] = 0);
            break;

        default:
            break;
        }
        break;

    case STOPWATCH_S:
        break;

    case STOPWATCH_L:
        break;

    case MODE_NUM:
        break;

    default:
        break;
    }
}

/* ���݂̃��[�h�C�I�����ɉ����Ēl�����炷�֐� */
void decr_selected_value(void)
{
    switch (mode)
    {
    case CLOCK:
        switch (selected_digit)
        {
        case LOWER:
            (clock_time[SECOND] > 0) ? clock_time[SECOND]-- : (clock_time[SECOND] = 59);
            break;

        case MIDDLE:
            (clock_time[MINUTE] > 0) ? clock_time[MINUTE]-- : (clock_time[MINUTE] = 59);
            break;

        case UPPER:
            (clock_time[HOUR] > 0) ? clock_time[HOUR]-- : (clock_time[HOUR] = 23);
            break;

        default:
            break;
        }
        break;

    case CALENDAR:
        switch (selected_digit)
        {
        case LOWER:
            (day > 1) ? day-- : (day = 31);
            break;

        case MIDDLE:
            (month > 1) ? month-- : (month = 12);
            break;

        case UPPER:
            (year > 0) ? year-- : (year = 99);
            break;

        default:
            break;
        }
        break;

    case TIMER:
        switch (selected_digit)
        {
        case LOWER:
            (timer_time[SECOND] > 0) ? timer_time[SECOND]-- : (timer_time[SECOND] = 59);
            break;

        case MIDDLE:
            (timer_time[MINUTE] > 0) ? timer_time[MINUTE]-- : (timer_time[MINUTE] = 59);
            break;

        case UPPER:
            (timer_time[HOUR] > 0) ? timer_time[HOUR]-- : (timer_time[HOUR] = 23);
            break;

        default:
            break;
        }
        break;

    case STOPWATCH_S:
        break;

    case STOPWATCH_L:
        break;

    case MODE_NUM:
        break;

    default:
        break;
    }
}

/* �`���^�����O��҂֐� */
void wait_chattering(void)
{
    __delay_ms(25);
}

/* TMR2�̊��荞�ݏ���(1/100 s���ƂɎ��v���X�V) */
void MYTMR2_ISR(void)
{
    is_tmr2_interrupt = 1;

    if (is_clock_active)
    {
        // ���Ԋ֌W�͂����ƍ���̂Ŋ��荞�ݓ��ŏ���
        clock_time[CSEC]++; // ���荞�݂�1/100 s����

        if (clock_time[CSEC] > 99)
        {
            clock_time[CSEC] = 0;
            clock_time[SECOND]++;
        }
        if (clock_time[SECOND] > 59)
        {
            clock_time[SECOND] = 0;
            clock_time[MINUTE]++;
        }
        if (clock_time[MINUTE] > 59)
        {
            clock_time[MINUTE] = 0;
            clock_time[HOUR]++;
        }
        if (clock_time[HOUR] > 23)
        {
            clock_time[HOUR] = 0;
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
    if (is_timer_active)
    {
        if (!is_timer_end)
        {
            timer_time[CSEC]--;
        }

        if (timer_time[CSEC] > 99) // unsigned�Ȃ̂� 1 -> 0 -> 255 -> 254�ƂȂ�
        {
            timer_time[CSEC] = 99;
            timer_time[SECOND]--;
        }
        if (timer_time[SECOND] > 59)
        {
            timer_time[SECOND] = 59;
            timer_time[MINUTE]--;
        }
        if (timer_time[MINUTE] > 59)
        {
            timer_time[MINUTE] = 59;
            timer_time[HOUR]--;
        }
        if ((timer_time[CSEC] + timer_time[SECOND] + timer_time[MINUTE] + timer_time[HOUR]) == 0)
        {
            // �S��0�ɂȂ������~
            is_timer_end = 1;
            is_timer_active = 0;
        }
    }
    if (is_stopwatch_active)
    {
        stopwatch_time[CSEC]++;

        if (stopwatch_time[CSEC] > 99)
        {
            stopwatch_time[CSEC] = 0;
            stopwatch_time[SECOND]++;
        }
        if (stopwatch_time[SECOND] > 59)
        {
            stopwatch_time[SECOND] = 0;
            stopwatch_time[MINUTE]++;
        }
        if (stopwatch_time[MINUTE] > 59)
        {
            stopwatch_time[MINUTE] = 0;
            stopwatch_time[HOUR]++;
        }
        if (stopwatch_time[HOUR] > 23)
        {
            stopwatch_time[HOUR] = 0;
        }
    }
}

/* �{�^���֌W */
/* �X�^�[�g�E�X�g�b�v�{�^���̊��荞�ݏ��� */
void START_STOP_ISR(void)
{
    is_start_stop_pushed = 1;
}

/* �X�^�[�g�E�X�g�b�v�{�^���̉��������m�֐� */
PUSH_TYPE get_start_stop_push_type(void)
{
    is_start_stop_pushed = 0;
    wait_chattering();

    if (START_STOP)
    {
        int count = 0;

        // ���������m
        while (START_STOP)
        {
            if (count++ > LONGPUSH)
            {
                break;
            }
            __delay_ms(1);
        }

        return START_STOP ? LONG : SHORT;
    }
    else
    {
        return NG;
    }
}

/* ���l���Z�b�g�{�^���̊��荞�ݏ��� */
void RST_ISR(void)
{
    is_reset_pushed = 1;
}

/* ���l���Z�b�g�{�^���̉��������m�֐� */
PUSH_TYPE get_reset_push_type(void)
{
    is_reset_pushed = 0;
    wait_chattering();

    if (RST)
    {
        int count = 0;

        // ���������m
        while (RST)
        {
            if (count++ > LONGPUSH)
            {
                break;
            }
            __delay_ms(1);
        }

        return RST ? LONG : SHORT;
    }
    else
    {
        return NG;
    }
}

/* ���{�^���̊��荞�ݏ��� */
void LEFT_ISR(void)
{
    is_left_pushed = 1;
}

/* ���{�^���̉��������m�֐� */
PUSH_TYPE get_left_push_type(void)
{
    is_left_pushed = 0;
    wait_chattering();

    if (LEFT)
    {
        int count = 0;

        // ���������m
        while (LEFT)
        {
            if (count++ > LONGPUSH)
            {
                break;
            }
            __delay_ms(1);
        }

        return LEFT ? LONG : SHORT;
    }
    else
    {
        return NG;
    }
}

/* �E�{�^���̊��荞�ݏ��� */
void RIGHT_ISR(void)
{
    is_right_pushed = 1;
}

/* �E�{�^���̉��������m�֐� */
PUSH_TYPE get_right_push_type(void)
{
    is_right_pushed = 0;
    wait_chattering();

    if (RIGHT)
    {
        int count = 0;

        // ���������m
        while (RIGHT)
        {
            if (count++ > LONGPUSH)
            {
                break;
            }
            __delay_ms(1);
        }

        return RIGHT ? LONG : SHORT;
    }
    else
    {
        return NG;
    }
}

/* +�{�^���̊��荞�ݏ��� */
void PLUS_ISR(void)
{
    is_plus_pushed = 1;
}

/* +�{�^���̉��������m�֐� */
PUSH_TYPE get_plus_push_type(void)
{
    is_plus_pushed = 0;
    wait_chattering();

    if (PLUS)
    {
        tmr2_count = 0; // �_�Ŏ����̃��Z�b�g
        int count = 0;

        // ���������m
        while (PLUS)
        {
            if (count++ > PM_LONGPUSH)
            {
                break;
            }
            __delay_ms(1);
        }

        return PLUS ? LONG : SHORT;
    }
    else
    {
        return NG;
    }
}

/* -�{�^���̊��荞�ݏ��� */
void MINUS_ISR(void)
{
    is_minus_pushed = 1;
}

/* -�{�^���̉��������m�֐� */
PUSH_TYPE get_minus_push_type(void)
{
    is_minus_pushed = 0;
    wait_chattering();

    if (MINUS)
    {
        tmr2_count = 0; // �_�Ŏ����̃��Z�b�g
        int count = 0;

        // ���������m
        while (MINUS)
        {
            if (count++ > PM_LONGPUSH)
            {
                break;
            }
            __delay_ms(1);
        }

        return MINUS ? LONG : SHORT;
    }
    else
    {
        return NG;
    }
}

/**
 End of File
*/