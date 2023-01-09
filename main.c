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
#define DOT RA6       // RA6をDOTと名づける
#define UPDIGIT RB0   // RB0をUPDIGITと名づける
#define DOWNDIGIT RB1 // RB1をDOWNDIGITと名づける
#define PLUS RB2      // RB2をPLUSと名づける
#define MINUS RB3     // RB3をMINUSと名づける
#define SWITCH RB4    // RB4をSWITCHと名づける
#define START_EA RB5  // RB5をSTART_EAと名づける
#define LONGPUSH 500  // 長押し判定時間(ms)
#define SKIPSPAN 100  // ボタン長押しのスキップ時に何msに1回カウントするかの時間
#define MODENUM 2     // モード数

/*
                         Main application
 */

/* 関数プロトタイプ宣言 */
/* 割り込み関数 */
void MYTMR0_ISR(void);    // タイマー0の割り込み処理
void MYTMR2_ISR(void);    // タイマー2の割り込み処理(ISR : Interrupt Service Routin)
void PLUS_ISR(void);      // RB0(+ボタン)の割り込み処理
void MINUS_ISR(void);     // RB1(-ボタン)の割り込み処理
void UPDIGIT_ISR(void);   // RB2(設定桁上げボタン)の割り込み処理
void DOWNDIGIT_ISR(void); // RB3(設定桁下げボタン)の割り込み処理
void SWITCH_ISR(void);    // RB4(設定モード切り替えボタン)の割り込み処理
void START_EA_ISR(void);  // RB5(スタート・イネーブルボタン)の割り込み処理

/* 自作関数 */
void init_clock(void);             // タイマーの初期化設定
void mainApp(void);                // メイン関数のwhileループでの処理
void show_nixie(void);             // 現在のモードに合わせた値を表示する関数
void show_time(void);              // 時間を表示する関数
void show_date(void);              // 日付を表示する関数
void show_values(unsigned char[]); // 値を表示する関数
void up_value(void);               // 現在のモード，選択桁に合わせてカウントアップする関数
void down_value(void);             // 現在のモード，選択桁に合わせてカウントダウンする関数
void wait_chattering(void);        // チャタリングを待つ関数
void sleep_nixie(unsigned char);   // ニキシー管を全消灯するための関数

/* グローバル変数宣言 */
unsigned char csec = 0;           // センチ秒(1/100秒)
unsigned char second = 0;         // 秒
unsigned char minute = 0;         // 分
unsigned char hour = 0;           // 時
unsigned char day = 1;            // 日
unsigned char month = 1;          // 月
unsigned char year = 0;           // 年
unsigned char enable = 1;         // ニキシー管の表示の可否 0: 消灯, 1: 点灯
unsigned char mode = 0;           // 0 : 時計, 1 : カレンダー
unsigned char start = 0;          // カウント開始フラグ
unsigned char selected_digit = 0; // 編集中の桁を示すフラグ 0 : 下位2桁, 1 : 中位2桁, 2 : 上位2桁
unsigned char is_light = 1;       // 0 : 消灯，1 : 点灯

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
    DOT = 1;
    // 未使用ピンはLowに
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
    // 全部割り込みでする?
    // if (enable)
    // {
    //     if (is_write)
    //     {
    //         is_write = 0; // 書き込みフラグを降ろす
    //         write_74HC595();
    //     }
    //     if (!enable)
    //     {                     // 途中で割り込みが発生したときの処理
    //         sleep_nixie(3); // 全消灯
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

/* 現在のモードに合わせた値を表示する関数 */
void show_nixie(void)
{
    switch (mode)
    {
    case 0: // 時計モード
        show_time();
        break;

    case 1: // カレンダーモード
        show_date();
        break;

    default:
        break;
    }
}

/* 時間を表示する関数 */
void show_time(void)
{
    unsigned char time[3] = {second, minute, hour};
    show_values(time);
    DOT = (csec < 50) ? 0 : 1;
}

/* 日付を表示する関数 */
void show_date(void)
{
    unsigned char date[3] = {day, month, year};
    show_values(date);
    DOT = 1;
}

/* 受け取った値を表示する関数 (0: 下位2桁, 1: 中位2桁, 2: 上位2桁)*/
void show_values(unsigned char values[3])
{
    unsigned char bcd_code; // BCDに変換したコード
    unsigned char compared; // 比較用の値

    for (signed char i = 0; i < 3; i++)
    {
        // シフト演算は暗黙的にintになっていしまうので unsigned char にキャストする
        bcd_code = (unsigned char)((values[i] / 10) << 4) + (values[i] % 10);
        compared = 0b00000001; // 1ビットごとに比較を行うための値

        switch (i)
        {
        case 0:
            for (unsigned char j = 0; j < 8; j++)
            {
                SER = (__bit)((bcd_code & compared) >> j); // 各ビットが1か0かを記録 (__bit型にキャスト)
                LOWSRCLK = 1;                              // 立ち上がりでデータを送る
                LOWSRCLK = 0;
                compared = (unsigned char)(compared << 1);
            }
            break;

        case 1:
            for (unsigned char j = 0; j < 8; j++)
            {
                SER = (__bit)((bcd_code & compared) >> j); // 各ビットが1か0かを記録 (__bit型にキャスト)
                MIDSRCLK = 1;
                MIDSRCLK = 0;
                compared = (unsigned char)(compared << 1);
            }
            break;

        case 2:
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
void sleep_nixie(unsigned char digit)
{
    DOT = 0; // dotを消灯

    if (digit == 3 || digit == 0)
    {
        for (unsigned char i = 0; i < 8; i++)
        {
            SER = 1; // 全桁に1を書き込み PICはビットごとの書き換えができないので毎回書き込む
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

/* チャタリングを待つ関数 */
void wait_chattering(void)
{
    __delay_ms(15);
}

/* TMR0の割り込み処理(設定桁を点滅) */
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

/* TMR2の割り込み処理(1/100 sごとに時計を更新) */
void MYTMR2_ISR(void)
{
    if (start)
    {
        // 割り込みは1/100 s周期
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
}

/* +ボタンの割り込み処理(値設定フェーズで数値の増加を行う) */
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

            // 長押し検知
            while (PLUS)
            {
                if (count++ > LONGPUSH)
                {
                    break;
                }
                __delay_ms(1);
            }

            // 長押し時の処理
            while (PLUS)
            {
                // 連続桁上げ
                up_value();
                show_nixie();
                __delay_ms(SKIPSPAN);
            }
        }
    }
}

/* 現在のモード，選択桁に応じて値を増やす関数 */
void up_value(void)
{
    switch (mode)
    {
    case 0: // 時計モード
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

    case 1: // カレンダーモード
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

/* -ボタンの割り込み処理(値設定フェーズで数値の減少を行う) */
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

            // 長押し検知
            while (MINUS)
            {
                if (count++ > LONGPUSH)
                {
                    break;
                }
                __delay_ms(1);
            }

            // 長押し時の処理
            while (MINUS)
            {
                // 連続桁上げ
                down_value();
                show_nixie();
                __delay_ms(SKIPSPAN);
            }
        }
    }
}

/* 現在のモード，選択桁に応じて値を減らす関数 */
void down_value(void)
{
    switch (mode)
    {
    case 0: // 時計モード
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

    case 1: // カレンダーモード
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

/* 選択桁上げボタンの割り込み処理(値設定フェーズで設定桁の増加を行う) */
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

/* 選択桁下げボタンの割り込み処理(値設定フェーズで設定桁の減少を行う) */
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

/* モード切り替えボタンの割り込み処理(モードの変更を行う(ニキシー管表示時のみ)) */
void SWITCH_ISR(void)
{
    wait_chattering();
    if (SWITCH && enable)
    {
        (mode < (MODENUM - 1)) ? mode++ : (mode = 0);
        show_nixie();
    }
}

/* スタートボタンの割り込み処理(設定フェーズではスタートを, 動作フェーズでは表示の有無の変更を行う) */
void START_EA_ISR(void)
{
    wait_chattering();
    if (START_EA)
    {
        start = 1; // 動作開始

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