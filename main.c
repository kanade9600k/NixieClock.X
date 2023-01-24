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
#define SER RA0         // RA0をSERと名づける
#define RCLK RA1        // RA1をRCLKと名づける
#define UPPSRCLK RA2    // RA2をUPPSRCLKと名づける
#define MIDSRCLK RA3    // RA3をMIDSRCLKと名づける
#define LOWSRCLK RA4    // RA4をLOWSRCLKと名づける
#define DOT RA6         // RA6をDOTと名づける
#define LEFT RB0        // RB0をLEFTと名づける
#define RIGHT RB1       // RB1をRIGHTと名づける
#define PLUS RB2        // RB2をPLUSと名づける
#define MINUS RB3       // RB3をMINUSと名づける
#define RST RB4         // RB4をRSTと名づける
#define START_STOP RB5  // RB5をSTART_STOPと名づける
#define BLINKINGSPAN 50 // 点滅間隔(cs)
#define LONGPUSH 1500   // 長押し判定時間(ms)
#define PM_LONGPUSH 500 // 連続桁上げ判定時間(ms)(+, -ボタンの長押し判定時間)
#define SKIPSPAN 50     // ボタン長押しのスキップ時に何msに1回カウントするかの時間(ms)

/*
                         Main application
 */

/* 列挙 */
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

/* 関数プロトタイプ宣言 */
/* 割り込み関数 */
void MYTMR2_ISR(void);     // タイマー2の割り込み処理(ISR : Interrupt Service Routin)
void PLUS_ISR(void);       // +ボタンの割り込み処理
void MINUS_ISR(void);      // -ボタンの割り込み処理
void LEFT_ISR(void);       // 左ボタンの割り込み処理
void RIGHT_ISR(void);      // 右ボタンの割り込み処理
void RST_ISR(void);        // 数値リセットボタンの割り込み処理
void START_STOP_ISR(void); // スタート・ストップボタンの割り込み処理

/* 自作関数 */
void mainApp(void);         // メイン関数のwhileループでの処理
void mode_clock(void);      // 時計モードの関数
void mode_calendar(void);   // カレンダーモードの関数
void mode_timer(void);      // タイマーモードの関数
void mode_stopwatch(void);  // ストップウォッチモードの関数
void wait_chattering(void); // チャタリングを待つ関数
void show_nixie(void);      // 現在のモードに合わせた値を表示する関数
// void show_time(void);                     // 時間を表示する関数
// void show_date(void);                     // 日付を表示する関数
void sleep_nixie(DIGIT);                  // ニキシー管を消灯するための関数
void show_values(unsigned char[]);        // 値を表示する関数
void setting_date(void);                  // 日付設定関数
void setting_time(void);                  // 時刻設定関数
void up_digit(void);                      // 選択桁を上げる関数
void down_digit(void);                    // 選択桁を下げる関数
void blinking_digit(void);                // 選択桁を点滅させる関数
void up_mode(void);                       // モードを1つ進める関数
void down_mode(void);                     // モードを1つ戻す関数
void incr_selected_value(void);           // 現在のモード，選択桁に合わせて値を増加させる関数
void decr_selected_value(void);           // 現在のモード，選択桁に合わせて値を減少させる関数
PUSH_TYPE get_start_stop_push_type(void); // スタート・ストップボタンの押し方検知関数
PUSH_TYPE get_reset_push_type(void);      // 数値リセットボタンの押し方検知関数
PUSH_TYPE get_left_push_type(void);       // 左ボタンの押し方検知関数
PUSH_TYPE get_right_push_type(void);      // 右ボタンの押し方検知関数
PUSH_TYPE get_plus_push_type(void);       // +ボタンの押し方検知関数
PUSH_TYPE get_minus_push_type(void);      // -ボタンの押し方検知関数

/* グローバル変数宣言 */
unsigned char clock_time[TIME_NUM] = {0};     // 時計の時間
unsigned char timer_time[TIME_NUM] = {0};     // タイマーの時間
unsigned char stopwatch_time[TIME_NUM] = {0}; // ストップウォッチの時間
// unsigned char csec = 0;                       // センチ秒(1/100秒)
// unsigned char second = 0;                     // 秒
// unsigned char minute = 0;                     // 分
// unsigned char hour = 0;                       // 時
unsigned char day = 1;                  // 日
unsigned char month = 1;                // 月
unsigned char year = 0;                 // 年
DIGIT selected_digit = LOWER;           // 選択中の桁
MODE mode = CALENDAR;                   // 現在のモード
unsigned char is_clock_active = 0;      // 時計が作動中か否か
unsigned char is_timer_active = 0;      // タイマーが作動中か否か
unsigned char is_timer_end = 1;         // タイマーの設定時間に達したか否か
unsigned char is_stopwatch_active = 0;  // ストップウォッチが作動中か否か
unsigned char is_tmr2_interrupt = 0;    // タイマー2の割り込みが起こったか
int tmr2_count = 0;                     // タイマー2の割り込みカウンタ
unsigned char is_start_stop_pushed = 0; // スタート・ストップボタンが押されたか
unsigned char is_reset_pushed = 0;      // 数値リセットボタンが押されたか
unsigned char is_left_pushed = 0;       // 左ボタンが押されたか
unsigned char is_right_pushed = 0;      // 右ボタンが押されたか
unsigned char is_plus_pushed = 0;       // プラスボタンが押されたか
unsigned char is_minus_pushed = 0;      // マイナスボタンが押されたか
unsigned char is_nixie_on = 1;          // ニキシー管の表示の可否 0: 消灯, 1: 点灯

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

    // タイマー2の割り込み時に呼び出される関数
    TMR2_SetInterruptHandler(MYTMR2_ISR);
    // RB0の割り込み時に呼び出される関数
    IOCBF0_SetInterruptHandler(LEFT_ISR);
    // RB1の割り込み時に呼び出される関数
    IOCBF1_SetInterruptHandler(RIGHT_ISR);
    // RB2の割り込み時に呼び出される関数
    IOCBF2_SetInterruptHandler(PLUS_ISR);
    // RB3の割り込み時に呼び出される処理
    IOCBF3_SetInterruptHandler(MINUS_ISR);
    // RB4の割り込み時に呼び出される処理
    IOCBF4_SetInterruptHandler(RST_ISR);
    // RB5の割り込み時に呼び出される処理
    IOCBF5_SetInterruptHandler(START_STOP_ISR);

    // Disable the Global Interrupts
    // INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    // INTERRUPT_PeripheralInterruptDisable();

    // 出力初期化
    SER = 0;
    RCLK = 0;
    LOWSRCLK = 0;
    MIDSRCLK = 0;
    UPPSRCLK = 0;
    DOT = 1;
    // 未使用ピンはLowに
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
        mainApp(); // while内のメイン処理はmainApp()内に記述する
    }
}

/* while内のメイン処理 */
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

/* 時計モード */
void mode_clock(void)
{
    if (is_start_stop_pushed)
    {
        switch (get_start_stop_push_type())
        {
        case SHORT:
            is_nixie_on = 1; // 点灯
            break;

        case LONG:
            is_nixie_on = 0; // 消灯

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
            is_clock_active = 0; // 一旦時計を止める
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

/* カレンダーモード */
void mode_calendar(void)
{
    if (is_start_stop_pushed)
    {
        switch (get_start_stop_push_type())
        {
        case SHORT:
            is_nixie_on = 1; // 点灯
            break;

        case LONG:
            is_nixie_on = 0; // 消灯

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

/* タイマーモード */
void mode_timer(void)
{
    if (is_start_stop_pushed)
    {
        switch (get_start_stop_push_type())
        {
        case SHORT:
            is_nixie_on = 1;                    // 点灯
            is_timer_active = !is_timer_active; // スタート・ストップ
            break;

        case LONG:
            is_nixie_on = 0; // 消灯

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
            // タイマーリセット
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

/* ストップウォッチモード */
void mode_stopwatch(void)
{
    if (is_start_stop_pushed)
    {
        switch (get_start_stop_push_type())
        {
        case SHORT:
            is_nixie_on = 1;                            // 点灯
            is_stopwatch_active = !is_stopwatch_active; // スタート・ストップ
            break;

        case LONG:
            is_nixie_on = 0; // 消灯

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
            // ストップウォッチリセット
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

/* 日付設定 */
void setting_date(void)
{
    show_nixie();

    while (1)
    {
        // ループを抜ける
        if (is_start_stop_pushed)
        {
            if (get_start_stop_push_type() != NG)
            {
                break;
            }
        }

        // 選択桁上げの処理
        if (is_left_pushed)
        {
            if (get_left_push_type() != NG)
            {
                up_digit();
            }
        }

        // 選択桁下げの処理
        if (is_right_pushed)
        {
            if (get_right_push_type() != NG)
            {
                down_digit();
            }
        }

        // 選択桁の数値上げ処理
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
                    // 連続桁上げ
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

        // 選択桁の数値下げ処理
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
                    // 連続桁下げ
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

        // 選択桁の点滅処理
        if (is_tmr2_interrupt)
        {
            blinking_digit();
        }
    }
}

/* 時刻設定関数 */
void setting_time(void)
{
    show_nixie();

    while (1)
    {
        // ループを抜ける
        if (is_start_stop_pushed)
        {
            if (get_start_stop_push_type() != NG)
            {
                break;
            }
        }

        // 選択桁上げの処理
        if (is_left_pushed)
        {
            if (get_left_push_type() != NG)
            {
                up_digit();
            }
        }

        // 選択桁下げの処理
        if (is_right_pushed)
        {
            if (get_right_push_type() != NG)
            {
                down_digit();
            }
        }

        // 選択桁の数値上げ処理
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
                    // 連続桁上げ
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

        // 選択桁の数値下げ処理
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
                    // 連続桁下げ
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
        // 選択桁の点滅処理
        if (is_tmr2_interrupt)
        {
            blinking_digit();
        }
    }
}

/* 現在のモードに合わせた値を表示する関数 */
void show_nixie(void)
{
    switch (mode)
    {
    case CLOCK:
    {
        // {}でブロックを作らないとswitch文内での変数定義ができない
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

// /* 時間を表示する関数 */
// void show_time(void)
// {
//     unsigned char time[3] = {second, minute, hour};
//     show_values(time);
//     DOT = (csec < 50) ? 0 : 1;
// }

// /* 日付を表示する関数 */
// void show_date(void)
// {
//     unsigned char date[3] = {day, month, year};
//     show_values(date);
//     DOT = 1;
// }

/* 受け取った値を表示する関数 (0: 下位2桁, 1: 中位2桁, 2: 上位2桁)*/
void show_values(unsigned char values[3])
{
    unsigned char bcd_code; // BCDに変換したコード
    unsigned char compared; // 比較用の値

    for (DIGIT digit = LOWER; digit <= UPPER; digit++)
    {
        // シフト演算は暗黙的にintになっていしまうので unsigned char にキャストする
        bcd_code = (unsigned char)((values[digit] / 10) << 4) + (values[digit] % 10);
        compared = 0b00000001; // 1ビットごとに比較を行うための値

        switch (digit)
        {
        case LOWER:
            for (unsigned char j = 0; j < 8; j++)
            {
                SER = (__bit)((bcd_code & compared) >> j); // 各ビットが1か0かを記録 (__bit型にキャスト)
                LOWSRCLK = 1;                              // 立ち上がりでデータを送る
                LOWSRCLK = 0;
                compared = (unsigned char)(compared << 1);
            }
            break;

        case MIDDLE:
            for (unsigned char j = 0; j < 8; j++)
            {
                SER = (__bit)((bcd_code & compared) >> j); // 各ビットが1か0かを記録 (__bit型にキャスト)
                MIDSRCLK = 1;
                MIDSRCLK = 0;
                compared = (unsigned char)(compared << 1);
            }
            break;

        case UPPER:
            for (unsigned char j = 0; j < 8; j++)
            {
                SER = (__bit)((bcd_code & compared) >> j); // 各ビットが1か0かを記録 (__bit型にキャスト)
                UPPSRCLK = 1;
                UPPSRCLK = 0;
                compared = (unsigned char)(compared << 1);
            }
            break;

        default:
            break;
        }

        RCLK = 1; // 全データを送り終わったらシフトレジスタの値を出力に反映
        RCLK = 0;
    }
}

/* ニキシー管を消灯させるための関数 0: 下位2桁 1: 中位2桁 2: 上位2桁 3: 全桁 */
void sleep_nixie(DIGIT digit)
{
    DOT = 0; // dotを消灯

    if (digit == ALL || digit == LOWER)
    {
        for (unsigned char i = 0; i < 8; i++)
        {
            SER = 1; // 全桁に1を書き込み PICはビットごとの書き換えができないので毎回書き込む
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

/* 選択桁を上げる関数 */
void up_digit(void)
{
    (selected_digit < UPPER) ? selected_digit++ : (selected_digit = LOWER);
}

/* 選択桁を下げる関数 */
void down_digit(void)
{
    (selected_digit > LOWER) ? selected_digit-- : (selected_digit = UPPER);
}

/* 選択桁のを点滅させる関数 */
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

/* モードを1つ進める関数 */
void up_mode(void)
{
    (mode < (MODE_NUM - 1)) ? mode++ : (mode = 0);
}

/* モードを1つ戻す関数 */
void down_mode(void)
{
    (mode > 0) ? mode-- : (mode = (MODE_NUM - 1));
}

/* 現在のモード，選択桁に応じて値を増やす関数 */
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

/* 現在のモード，選択桁に応じて値を減らす関数 */
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

/* チャタリングを待つ関数 */
void wait_chattering(void)
{
    __delay_ms(25);
}

/* TMR2の割り込み処理(1/100 sごとに時計を更新) */
void MYTMR2_ISR(void)
{
    is_tmr2_interrupt = 1;

    if (is_clock_active)
    {
        // 時間関係はずれると困るので割り込み内で処理
        clock_time[CSEC]++; // 割り込みは1/100 s周期

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
        // 2月の処理(うるう年は考えない)
        if ((month == 2) && (day > 28))
        {
            month++;
            day = 1;
        }
        // 4,6,9,11月の処理
        if (((month == 4) || (month == 6) || (month == 9) || (month == 11)) && (day > 30))
        {
            month++;
            day = 1;
        }
        // 1,3,5,7,8,10月の処理
        if (((month == 1) || (month == 3) || (month == 5) || (month == 7) || (month == 8) || (month == 10)) && (day > 31))
        {
            month++;
            day = 1;
        }
        // 12月の処理
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

        if (timer_time[CSEC] > 99) // unsignedなので 1 -> 0 -> 255 -> 254となる
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
            // 全部0になったら停止
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

/* ボタン関係 */
/* スタート・ストップボタンの割り込み処理 */
void START_STOP_ISR(void)
{
    is_start_stop_pushed = 1;
}

/* スタート・ストップボタンの押し方検知関数 */
PUSH_TYPE get_start_stop_push_type(void)
{
    is_start_stop_pushed = 0;
    wait_chattering();

    if (START_STOP)
    {
        int count = 0;

        // 長押し検知
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

/* 数値リセットボタンの割り込み処理 */
void RST_ISR(void)
{
    is_reset_pushed = 1;
}

/* 数値リセットボタンの押し方検知関数 */
PUSH_TYPE get_reset_push_type(void)
{
    is_reset_pushed = 0;
    wait_chattering();

    if (RST)
    {
        int count = 0;

        // 長押し検知
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

/* 左ボタンの割り込み処理 */
void LEFT_ISR(void)
{
    is_left_pushed = 1;
}

/* 左ボタンの押し方検知関数 */
PUSH_TYPE get_left_push_type(void)
{
    is_left_pushed = 0;
    wait_chattering();

    if (LEFT)
    {
        int count = 0;

        // 長押し検知
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

/* 右ボタンの割り込み処理 */
void RIGHT_ISR(void)
{
    is_right_pushed = 1;
}

/* 右ボタンの押し方検知関数 */
PUSH_TYPE get_right_push_type(void)
{
    is_right_pushed = 0;
    wait_chattering();

    if (RIGHT)
    {
        int count = 0;

        // 長押し検知
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

/* +ボタンの割り込み処理 */
void PLUS_ISR(void)
{
    is_plus_pushed = 1;
}

/* +ボタンの押し方検知関数 */
PUSH_TYPE get_plus_push_type(void)
{
    is_plus_pushed = 0;
    wait_chattering();

    if (PLUS)
    {
        tmr2_count = 0; // 点滅周期のリセット
        int count = 0;

        // 長押し検知
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

/* -ボタンの割り込み処理 */
void MINUS_ISR(void)
{
    is_minus_pushed = 1;
}

/* -ボタンの押し方検知関数 */
PUSH_TYPE get_minus_push_type(void)
{
    is_minus_pushed = 0;
    wait_chattering();

    if (MINUS)
    {
        tmr2_count = 0; // 点滅周期のリセット
        int count = 0;

        // 長押し検知
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