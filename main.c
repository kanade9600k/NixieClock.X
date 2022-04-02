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
#define SER RA0       // RA0をSERと名づける
#define RCLK RA1      // RA1をRCLKと名づける
#define UPPSRCLK RA2  // RA2をUPPSRCLKと名づける
#define MIDSRCLK RA3  // RA3をMIDSRCLKと名づける
#define LOWSRCLK RA4  // RA4をLOWSRCLKと名づける
#define ENABLE RA6    // RA6をENABLEと名づける
#define UPDIGIT RB0   // RB0をUPDIGITと名づける
#define DOWNDIGIT RB1 // RB1をDOWNDIGITと名づける
#define PLUS RB2      // RB2をPLUSと名づける
#define MINUS RB3     // RB3をMINUSと名づける
#define SWITCH RB4    // RB4をSWITCHと名づける
#define START_EA RB5  // RB5をSTART_EAと名づける
#define LONGPUSH 500  // 長押し判定時間(ms)
#define SKIPSPAN 100  // ボタン長押しのスキップ時に何msに1回カウントするかの時間
#define COUNTDOWN 0   // カウントダウンを表す
#define COUNTUP 1     // カウントアップを表す

/*
                         Main application
 */

/* 関数プロトタイプ宣言 */
void MYTMR0_ISR(void);                             // タイマー0の割り込み処理
void MYTMR2_ISR(void);                             // タイマー2の割り込み処理(ISR : Interrupt Service Routin)
void PLUS_ISR(void);                               // RB0(+ボタン)の割り込み処理
void MINUS_ISR(void);                              // RB1(-ボタン)の割り込み処理
void UPDIGIT_ISR(void);                            // RB2(設定桁上げボタン)の割り込み処理
void DOWNDIGIT_ISR(void);                          // RB3(設定桁下げボタン)の割り込み処理
void SWITCH_ISR(void);                             // RB4(設定モード切り替えボタン)の割り込み処理
void START_EA_ISR(void);                           // RB5(スタート・イネーブルボタン)の割り込み処理
void init_clock(void);                             // タイマーの初期化設定
void mainApp(void);                                // メイン関数のwhileループでの処理
void write_74HC595(void);                          // 74HC595(シフトレジスタ)に情報を書き込む関数
void wait_chattering(void);                        // チャタリングを待つ関数
void updown_min_sec(unsigned char, unsigned char); // 秒，分を設定する関数
void updown_hour(unsigned char);                   // 時を設定する関数
void updown_day(unsigned char);                    // 日を設定する関数
void updown_month(unsigned char);                  // 月を設定する関数
void updown_year(unsigned char);                   // 年を設定する関数
void sleep_nixie(unsigned char);                   // ニキシー管を全消灯するための関数

/* グローバル変数宣言 */
// 配列だったものを変数に変えた
// unsigned char time[6] = {0};                   // 時間情報を保持する配列 (0..5 s,s,m,m,h,h)
// unsigned char date[6] = {0, 0, 0, 1, 0, 1};    // 日付情報を保持する配列 (0..5 d,d,m,m,y,y)
unsigned char second = 0;            // 秒
unsigned char minute = 0;            // 分
unsigned char hour = 0;              // 時
unsigned char day = 1;               // 日
unsigned char month = 1;             // 月
unsigned char year = 0;              // 年
unsigned char is_write = 0;          // シフトレジスタ書き込み信号
unsigned char mode = 0;              // 0 : 時計, 1 : カレンダー
unsigned char data = 0;              // シフトレジスタに書き込むデータ
unsigned char start = 0;             // カウント開始フラグ
unsigned char selected_digit = 0;    // 編集中の桁を示すフラグ 0 : 下位2桁, 1 : 中位2桁, 2 : 上位2桁
unsigned int count = 0;              // 共有のカウンタ 使ったら0に戻しましょう
unsigned char lower = 0;             // シフトレジスタ入力の下位桁
unsigned char upper = 0;             // シフトレジスタ入力の上位桁
unsigned char bcd_code = 0;          // シフトレジスタ入力用BCDコード
unsigned char compared = 0b00000001; // 1ビットごとに比較を行うための値
unsigned char is_light = 1;          // 0 : 消灯，1 : 点灯

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

    // タイマー0の割り込み時に呼び出される関数
    TMR0_SetInterruptHandler(MYTMR0_ISR);
    // タイマー2の割り込み時に呼び出される関数
    TMR2_SetInterruptHandler(MYTMR2_ISR);
    // RB0の割り込み時に呼び出される関数
    IOCBF0_SetInterruptHandler(UPDIGIT_ISR);
    // RB1の割り込み時に呼び出される関数
    IOCBF1_SetInterruptHandler(DOWNDIGIT_ISR);
    // RB2の割り込み時に呼び出される関数
    IOCBF2_SetInterruptHandler(PLUS_ISR);
    // RB3の割り込み時に呼び出される処理
    IOCBF3_SetInterruptHandler(MINUS_ISR);
    // RB4の割り込み時に呼び出される処理
    IOCBF4_SetInterruptHandler(SWITCH_ISR);
    // RB5の割り込み時に呼び出される処理
    IOCBF5_SetInterruptHandler(START_EA_ISR);

    // Disable the Global Interrupts
    // INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    // INTERRUPT_PeripheralInterruptDisable();

    // ここに初期の時間合わせ処理を書く
    init_clock();

    while (start)
    {
        // Add your application code
        mainApp(); // while内のメイン処理はmainApp()内に記述する
    }
}

/* 時計の初期化設定 */
void init_clock(void)
{
    // 出力初期化
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
        ; // 初期設定はすべて割り込みで行う
    }
}

/* while内のメイン処理 */
void mainApp(void)
{
    if (ENABLE)
    {
        if (is_write)
        {
            is_write = 0; // 書き込みフラグを降ろす
            write_74HC595();
        }
        if (!ENABLE)
        {                     // 途中で割り込みが発生したときの処理
            sleep_nixie(255); // 全消灯
        }
    }
    //    else {
    //        sleep_nixie(255);
    //    }
}

/* 74HC595(シフトレジスタ)に情報を書き込む関数 */
void write_74HC595(void)
{
    unsigned char data;

    for (unsigned char i = 0; i < 3; i++)
    {
        switch (i)
        {
        case 0:
            data = mode ? day : second;
            break;

        case 1:
            data = mode ? month : minute;
            break;

        case 2:
            data = mode ? year : hour;
            break;

        default:
            break;
        }

        upper = data / 10;
        lower = data % 10;
        // シフト演算は暗黙的にintになっていしまうので unsigned char にキャストする
        bcd_code = (unsigned char)(upper << 4) + lower;

        for (unsigned char j = 0; j < 8; j++)
        {
            SER = (__bit)((bcd_code & compared) >> j); // 各ビットが1か0かを記録 (__bit型にキャスト)
            switch (i)
            {
            case 0:
                LOWSRCLK = 1; // 立ち上がりでデータを送る
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
        RCLK = 1; // 全データを送り終わったらシフトレジスタの値を出力に反映
        RCLK = 0;
    }
}

/* ニキシー管を消灯させるための関数 0: 下位2桁 1: 中位2桁 2: 上位2桁 255: 全桁 */
void sleep_nixie(unsigned char digit)
{
    for (unsigned char i = 0; i < 8; i++)
    {
        if (digit == 255 || digit == 0)
        {
            SER = 1; // 全桁に1を書き込み PICはビットごとの書き換えができないので毎回書き込む
            LOWSRCLK = 1;
            LOWSRCLK = 0;
        }

        if (digit == 255 || digit == 1)
        {
            SER = 1;
            MIDSRCLK = 1;
            MIDSRCLK = 0;
        }

        if (digit == 255 || digit == 2)
        {
            SER = 1;
            UPPSRCLK = 1;
            UPPSRCLK = 0;
        }
    }
    RCLK = 1;
    RCLK = 0;
}

/* チャタリングを待つ関数 */
void wait_chattering(void)
{
    __delay_ms(15);
}

/* 秒，分を設定する関数 */
void updown_min_sec(unsigned char is_up, unsigned char unit)
{
    is_up ? unit++ : unit--;
    if (unit >= 60)
    {
        unit = is_up ? 0 : 59;
    }
    write_74HC595();
}

/* 時を設定する関数 */
void updown_hour(unsigned char is_up)
{
    is_up ? hour++ : hour--;
    if (hour >= 24)
    {
        hour = is_up ? 0 : 23;
    }
    write_74HC595();
}

/* 日を設定する関数 */
// 現状 2月31日などの存在しない日時も設定できてしまいますがおそらくカウントアップが始まると修正される？
// 1~31の範囲で変化させます
void updown_day(unsigned char is_up)
{
    is_up ? day++ : day--;
    if (day >= 32)
    {
        day = is_up ? 1 : 31; // 念のためカウントダウン時のリセットも合わせて書く
    }
    if (!(is_up) && (day == 0))
    {
        day = 31;
    }
    write_74HC595();
}

/* 月を設定する関数 */
// 1~12の範囲で変化させます
void updown_month(unsigned char is_up)
{
    is_up ? month++ : month--;
    if (month >= 13)
    {
        month = is_up ? 1 : 12; // 念のため
    }
    if (!(is_up) && (month == 0))
    {
        month = 12;
    }
    write_74HC595();
}

/* 年を設定する関数 */
void updown_year(unsigned char is_up)
{
    is_up ? year++ : year--;
    if (year >= 100)
    {
        year = is_up ? 0 : 99;
    }
    write_74HC595();
}

/* TMR0の割り込み処理(200msごとに設定桁を点滅) */
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
            write_74HC595();
        }
    }
}

/* TMR2の割り込み処理(1sごとに時計を更新) */
void MYTMR2_ISR(void)
{
    if (start)
    {
        // 割り込みは1s周期
        second++;
        is_write = 1; // シフトレジスタ書き込みフラグを立てる
        if (second >= 60)
        {
            second = 0;
            minute++;
        }
        if (minute >= 60)
        {
            minute = 0;
            hour++;
        }
        if (hour >= 24)
        {
            hour = 0;
            day++;
        }
        // 2月の処理(うるう年は考えない)
        if ((month == 2) && (day >= 29))
        {
            month++;
            day = 1;
        }
        // 4,6,9,11月の処理
        if (((month == 4) || (month == 6) || (month == 9) || (month == 11)) && (day >= 31))
        {
            month++;
            day = 1;
        }
        // 1,3,5,7,9,10月の処理
        if (((month == 1) || (month == 3) || (month == 5) || (month == 7) || (month == 9) || (month == 10)) && (day >= 32))
        {
            month++;
            day = 1;
        }
        // 12月の処理
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

/* +ボタンの割り込み処理(値設定フェーズで数値の増加を行う) */
void PLUS_ISR(void)
{
    if (!start)
    {
        wait_chattering();
        if (PLUS)
        {
            if (!mode)
            { // 時計モード
                switch (selected_digit)
                {
                case 0:
                    updown_min_sec(COUNTUP, second);
                    // 長押しで連続上げ
                    while (PLUS)
                    {
                        if (LONGPUSH <= ++count)
                        {
                            // 長押しモード突入
                            __delay_ms(SKIPSPAN);
                            updown_min_sec(COUNTUP, second);
                        }
                    }
                    count = 0;
                    break;
                case 1:
                    updown_min_sec(COUNTUP, minute);
                    while (PLUS)
                    {
                        if (LONGPUSH <= ++count)
                        {
                            // 長押しモード突入
                            __delay_ms(SKIPSPAN);
                            updown_min_sec(COUNTUP, minute);
                        }
                    }
                    count = 0;
                    break;
                case 2:
                    updown_hour(COUNTUP);
                    // 長押しで連続上げ
                    while (PLUS)
                    {
                        if (LONGPUSH <= ++count)
                        {
                            // 長押しモード突入
                            __delay_ms(SKIPSPAN);
                            updown_hour(COUNTUP);
                        }
                    }
                    count = 0;
                    break;
                default:
                    break;
                }
            }
            else
            { // カレンダーモード
                switch (selected_digit)
                {
                case 0:
                    updown_day(COUNTUP);
                    // 長押しで連続上げ
                    while (PLUS)
                    {
                        if (LONGPUSH <= ++count)
                        {
                            // 長押しモード突入
                            __delay_ms(SKIPSPAN);
                            updown_day(COUNTUP);
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
                            // 長押しモード突入
                            __delay_ms(SKIPSPAN);
                            updown_month(COUNTUP);
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
                            // 長押しモード突入
                            __delay_ms(SKIPSPAN);
                            updown_year(COUNTUP);
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

/* -ボタンの割り込み処理(値設定フェーズで数値の減少を行う) */
void MINUS_ISR(void)
{
    if (!start)
    {
        wait_chattering();
        if (MINUS)
        {
            if (!mode)
            { // 時計モード
                switch (selected_digit)
                {
                case 0:
                    updown_min_sec(COUNTDOWN, second);
                    // 長押しで連続下げ
                    while (MINUS)
                    {
                        if (LONGPUSH <= ++count)
                        {
                            // 長押しモード突入
                            __delay_ms(SKIPSPAN);
                            updown_min_sec(COUNTDOWN, second);
                        }
                    }
                    count = 0;
                    break;
                case 1:
                    updown_min_sec(COUNTDOWN, minute);
                    while (MINUS)
                    {
                        if (LONGPUSH <= ++count)
                        {
                            // 長押しモード突入
                            __delay_ms(SKIPSPAN);
                            updown_min_sec(COUNTDOWN, minute);
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
                            // 長押しモード突入
                            __delay_ms(SKIPSPAN);
                            updown_hour(COUNTDOWN);
                        }
                    }
                    count = 0;
                    break;
                default:
                    break;
                }
            }
            else
            { // カレンダーモード
                switch (selected_digit)
                {
                case 0:
                    updown_day(COUNTDOWN);
                    // 長押しで連続下げ
                    while (MINUS)
                    {
                        if (LONGPUSH <= ++count)
                        {
                            // 長押しモード突入
                            __delay_ms(SKIPSPAN);
                            updown_day(COUNTDOWN);
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
                            // 長押しモード突入
                            __delay_ms(SKIPSPAN);
                            updown_month(COUNTDOWN);
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
                            // 長押しモード突入
                            __delay_ms(SKIPSPAN);
                            updown_year(COUNTDOWN);
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

/* 選択桁上げボタンの割り込み処理(値設定フェーズで設定桁の増加を行う) */
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

/* 選択桁下げボタンの割り込み処理(値設定フェーズで設定桁の減少を行う) */
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

/* モード切り替えボタンの割り込み処理(モードの変更を行う) */
void SWITCH_ISR(void)
{
    wait_chattering();
    if (SWITCH)
    {
        mode = !mode;
        if (ENABLE)
        {
            write_74HC595();
        }
    }
}

/* スタートボタンの割り込み処理(設定フェーズではスタートを, 動作フェーズでは表示の有無の変更を行う) */
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
                sleep_nixie(255);
            }
            else
            {
                write_74HC595();
            }
        }
    }
}

/**
 End of File
*/