/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC16F1827
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.31 and above
        MPLAB 	          :  MPLAB X 5.45	
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

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set SER aliases
#define SER_TRIS                 TRISAbits.TRISA0
#define SER_LAT                  LATAbits.LATA0
#define SER_PORT                 PORTAbits.RA0
#define SER_ANS                  ANSELAbits.ANSA0
#define SER_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define SER_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define SER_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define SER_GetValue()           PORTAbits.RA0
#define SER_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define SER_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define SER_SetAnalogMode()      do { ANSELAbits.ANSA0 = 1; } while(0)
#define SER_SetDigitalMode()     do { ANSELAbits.ANSA0 = 0; } while(0)

// get/set RCLK aliases
#define RCLK_TRIS                 TRISAbits.TRISA1
#define RCLK_LAT                  LATAbits.LATA1
#define RCLK_PORT                 PORTAbits.RA1
#define RCLK_ANS                  ANSELAbits.ANSA1
#define RCLK_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define RCLK_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define RCLK_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define RCLK_GetValue()           PORTAbits.RA1
#define RCLK_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define RCLK_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define RCLK_SetAnalogMode()      do { ANSELAbits.ANSA1 = 1; } while(0)
#define RCLK_SetDigitalMode()     do { ANSELAbits.ANSA1 = 0; } while(0)

// get/set UPPSRCLK aliases
#define UPPSRCLK_TRIS                 TRISAbits.TRISA2
#define UPPSRCLK_LAT                  LATAbits.LATA2
#define UPPSRCLK_PORT                 PORTAbits.RA2
#define UPPSRCLK_ANS                  ANSELAbits.ANSA2
#define UPPSRCLK_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define UPPSRCLK_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define UPPSRCLK_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define UPPSRCLK_GetValue()           PORTAbits.RA2
#define UPPSRCLK_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define UPPSRCLK_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define UPPSRCLK_SetAnalogMode()      do { ANSELAbits.ANSA2 = 1; } while(0)
#define UPPSRCLK_SetDigitalMode()     do { ANSELAbits.ANSA2 = 0; } while(0)

// get/set MIDSRCLK aliases
#define MIDSRCLK_TRIS                 TRISAbits.TRISA3
#define MIDSRCLK_LAT                  LATAbits.LATA3
#define MIDSRCLK_PORT                 PORTAbits.RA3
#define MIDSRCLK_ANS                  ANSELAbits.ANSA3
#define MIDSRCLK_SetHigh()            do { LATAbits.LATA3 = 1; } while(0)
#define MIDSRCLK_SetLow()             do { LATAbits.LATA3 = 0; } while(0)
#define MIDSRCLK_Toggle()             do { LATAbits.LATA3 = ~LATAbits.LATA3; } while(0)
#define MIDSRCLK_GetValue()           PORTAbits.RA3
#define MIDSRCLK_SetDigitalInput()    do { TRISAbits.TRISA3 = 1; } while(0)
#define MIDSRCLK_SetDigitalOutput()   do { TRISAbits.TRISA3 = 0; } while(0)
#define MIDSRCLK_SetAnalogMode()      do { ANSELAbits.ANSA3 = 1; } while(0)
#define MIDSRCLK_SetDigitalMode()     do { ANSELAbits.ANSA3 = 0; } while(0)

// get/set LOWSRCLK aliases
#define LOWSRCLK_TRIS                 TRISAbits.TRISA4
#define LOWSRCLK_LAT                  LATAbits.LATA4
#define LOWSRCLK_PORT                 PORTAbits.RA4
#define LOWSRCLK_ANS                  ANSELAbits.ANSA4
#define LOWSRCLK_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define LOWSRCLK_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define LOWSRCLK_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define LOWSRCLK_GetValue()           PORTAbits.RA4
#define LOWSRCLK_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define LOWSRCLK_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define LOWSRCLK_SetAnalogMode()      do { ANSELAbits.ANSA4 = 1; } while(0)
#define LOWSRCLK_SetDigitalMode()     do { ANSELAbits.ANSA4 = 0; } while(0)

// get/set ENABLE aliases
#define ENABLE_TRIS                 TRISAbits.TRISA6
#define ENABLE_LAT                  LATAbits.LATA6
#define ENABLE_PORT                 PORTAbits.RA6
#define ENABLE_SetHigh()            do { LATAbits.LATA6 = 1; } while(0)
#define ENABLE_SetLow()             do { LATAbits.LATA6 = 0; } while(0)
#define ENABLE_Toggle()             do { LATAbits.LATA6 = ~LATAbits.LATA6; } while(0)
#define ENABLE_GetValue()           PORTAbits.RA6
#define ENABLE_SetDigitalInput()    do { TRISAbits.TRISA6 = 1; } while(0)
#define ENABLE_SetDigitalOutput()   do { TRISAbits.TRISA6 = 0; } while(0)

// get/set UPDIGIT aliases
#define UPDIGIT_TRIS                 TRISBbits.TRISB0
#define UPDIGIT_LAT                  LATBbits.LATB0
#define UPDIGIT_PORT                 PORTBbits.RB0
#define UPDIGIT_WPU                  WPUBbits.WPUB0
#define UPDIGIT_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define UPDIGIT_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define UPDIGIT_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define UPDIGIT_GetValue()           PORTBbits.RB0
#define UPDIGIT_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define UPDIGIT_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define UPDIGIT_SetPullup()          do { WPUBbits.WPUB0 = 1; } while(0)
#define UPDIGIT_ResetPullup()        do { WPUBbits.WPUB0 = 0; } while(0)

// get/set DOWNDIGIT aliases
#define DOWNDIGIT_TRIS                 TRISBbits.TRISB1
#define DOWNDIGIT_LAT                  LATBbits.LATB1
#define DOWNDIGIT_PORT                 PORTBbits.RB1
#define DOWNDIGIT_WPU                  WPUBbits.WPUB1
#define DOWNDIGIT_ANS                  ANSELBbits.ANSB1
#define DOWNDIGIT_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define DOWNDIGIT_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define DOWNDIGIT_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define DOWNDIGIT_GetValue()           PORTBbits.RB1
#define DOWNDIGIT_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define DOWNDIGIT_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define DOWNDIGIT_SetPullup()          do { WPUBbits.WPUB1 = 1; } while(0)
#define DOWNDIGIT_ResetPullup()        do { WPUBbits.WPUB1 = 0; } while(0)
#define DOWNDIGIT_SetAnalogMode()      do { ANSELBbits.ANSB1 = 1; } while(0)
#define DOWNDIGIT_SetDigitalMode()     do { ANSELBbits.ANSB1 = 0; } while(0)

// get/set PULS aliases
#define PULS_TRIS                 TRISBbits.TRISB2
#define PULS_LAT                  LATBbits.LATB2
#define PULS_PORT                 PORTBbits.RB2
#define PULS_WPU                  WPUBbits.WPUB2
#define PULS_ANS                  ANSELBbits.ANSB2
#define PULS_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define PULS_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define PULS_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define PULS_GetValue()           PORTBbits.RB2
#define PULS_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define PULS_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define PULS_SetPullup()          do { WPUBbits.WPUB2 = 1; } while(0)
#define PULS_ResetPullup()        do { WPUBbits.WPUB2 = 0; } while(0)
#define PULS_SetAnalogMode()      do { ANSELBbits.ANSB2 = 1; } while(0)
#define PULS_SetDigitalMode()     do { ANSELBbits.ANSB2 = 0; } while(0)

// get/set MINUS aliases
#define MINUS_TRIS                 TRISBbits.TRISB3
#define MINUS_LAT                  LATBbits.LATB3
#define MINUS_PORT                 PORTBbits.RB3
#define MINUS_WPU                  WPUBbits.WPUB3
#define MINUS_ANS                  ANSELBbits.ANSB3
#define MINUS_SetHigh()            do { LATBbits.LATB3 = 1; } while(0)
#define MINUS_SetLow()             do { LATBbits.LATB3 = 0; } while(0)
#define MINUS_Toggle()             do { LATBbits.LATB3 = ~LATBbits.LATB3; } while(0)
#define MINUS_GetValue()           PORTBbits.RB3
#define MINUS_SetDigitalInput()    do { TRISBbits.TRISB3 = 1; } while(0)
#define MINUS_SetDigitalOutput()   do { TRISBbits.TRISB3 = 0; } while(0)
#define MINUS_SetPullup()          do { WPUBbits.WPUB3 = 1; } while(0)
#define MINUS_ResetPullup()        do { WPUBbits.WPUB3 = 0; } while(0)
#define MINUS_SetAnalogMode()      do { ANSELBbits.ANSB3 = 1; } while(0)
#define MINUS_SetDigitalMode()     do { ANSELBbits.ANSB3 = 0; } while(0)

// get/set SWITCH aliases
#define SWITCH_TRIS                 TRISBbits.TRISB4
#define SWITCH_LAT                  LATBbits.LATB4
#define SWITCH_PORT                 PORTBbits.RB4
#define SWITCH_WPU                  WPUBbits.WPUB4
#define SWITCH_ANS                  ANSELBbits.ANSB4
#define SWITCH_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define SWITCH_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define SWITCH_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define SWITCH_GetValue()           PORTBbits.RB4
#define SWITCH_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define SWITCH_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define SWITCH_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define SWITCH_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define SWITCH_SetAnalogMode()      do { ANSELBbits.ANSB4 = 1; } while(0)
#define SWITCH_SetDigitalMode()     do { ANSELBbits.ANSB4 = 0; } while(0)

// get/set START aliases
#define START_TRIS                 TRISBbits.TRISB5
#define START_LAT                  LATBbits.LATB5
#define START_PORT                 PORTBbits.RB5
#define START_WPU                  WPUBbits.WPUB5
#define START_ANS                  ANSELBbits.ANSB5
#define START_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define START_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define START_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define START_GetValue()           PORTBbits.RB5
#define START_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define START_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define START_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define START_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define START_SetAnalogMode()      do { ANSELBbits.ANSB5 = 1; } while(0)
#define START_SetDigitalMode()     do { ANSELBbits.ANSB5 = 0; } while(0)

// get/set IO_RB6 aliases
#define IO_RB6_TRIS                 TRISBbits.TRISB6
#define IO_RB6_LAT                  LATBbits.LATB6
#define IO_RB6_PORT                 PORTBbits.RB6
#define IO_RB6_WPU                  WPUBbits.WPUB6
#define IO_RB6_ANS                  ANSELBbits.ANSB6
#define IO_RB6_SetHigh()            do { LATBbits.LATB6 = 1; } while(0)
#define IO_RB6_SetLow()             do { LATBbits.LATB6 = 0; } while(0)
#define IO_RB6_Toggle()             do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define IO_RB6_GetValue()           PORTBbits.RB6
#define IO_RB6_SetDigitalInput()    do { TRISBbits.TRISB6 = 1; } while(0)
#define IO_RB6_SetDigitalOutput()   do { TRISBbits.TRISB6 = 0; } while(0)
#define IO_RB6_SetPullup()          do { WPUBbits.WPUB6 = 1; } while(0)
#define IO_RB6_ResetPullup()        do { WPUBbits.WPUB6 = 0; } while(0)
#define IO_RB6_SetAnalogMode()      do { ANSELBbits.ANSB6 = 1; } while(0)
#define IO_RB6_SetDigitalMode()     do { ANSELBbits.ANSB6 = 0; } while(0)

// get/set IO_RB7 aliases
#define IO_RB7_TRIS                 TRISBbits.TRISB7
#define IO_RB7_LAT                  LATBbits.LATB7
#define IO_RB7_PORT                 PORTBbits.RB7
#define IO_RB7_WPU                  WPUBbits.WPUB7
#define IO_RB7_ANS                  ANSELBbits.ANSB7
#define IO_RB7_SetHigh()            do { LATBbits.LATB7 = 1; } while(0)
#define IO_RB7_SetLow()             do { LATBbits.LATB7 = 0; } while(0)
#define IO_RB7_Toggle()             do { LATBbits.LATB7 = ~LATBbits.LATB7; } while(0)
#define IO_RB7_GetValue()           PORTBbits.RB7
#define IO_RB7_SetDigitalInput()    do { TRISBbits.TRISB7 = 1; } while(0)
#define IO_RB7_SetDigitalOutput()   do { TRISBbits.TRISB7 = 0; } while(0)
#define IO_RB7_SetPullup()          do { WPUBbits.WPUB7 = 1; } while(0)
#define IO_RB7_ResetPullup()        do { WPUBbits.WPUB7 = 0; } while(0)
#define IO_RB7_SetAnalogMode()      do { ANSELBbits.ANSB7 = 1; } while(0)
#define IO_RB7_SetDigitalMode()     do { ANSELBbits.ANSB7 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);


/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handler for the IOCBF0 pin functionality
 * @Example
    IOCBF0_ISR();
 */
void IOCBF0_ISR(void);

/**
  @Summary
    Interrupt Handler Setter for IOCBF0 pin interrupt-on-change functionality

  @Description
    Allows selecting an interrupt handler for IOCBF0 at application runtime
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    InterruptHandler function pointer.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF0_SetInterruptHandler(MyInterruptHandler);

*/
void IOCBF0_SetInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Dynamic Interrupt Handler for IOCBF0 pin

  @Description
    This is a dynamic interrupt handler to be used together with the IOCBF0_SetInterruptHandler() method.
    This handler is called every time the IOCBF0 ISR is executed and allows any function to be registered at runtime.
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF0_SetInterruptHandler(IOCBF0_InterruptHandler);

*/
extern void (*IOCBF0_InterruptHandler)(void);

/**
  @Summary
    Default Interrupt Handler for IOCBF0 pin

  @Description
    This is a predefined interrupt handler to be used together with the IOCBF0_SetInterruptHandler() method.
    This handler is called every time the IOCBF0 ISR is executed. 
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF0_SetInterruptHandler(IOCBF0_DefaultInterruptHandler);

*/
void IOCBF0_DefaultInterruptHandler(void);


/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handler for the IOCBF1 pin functionality
 * @Example
    IOCBF1_ISR();
 */
void IOCBF1_ISR(void);

/**
  @Summary
    Interrupt Handler Setter for IOCBF1 pin interrupt-on-change functionality

  @Description
    Allows selecting an interrupt handler for IOCBF1 at application runtime
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    InterruptHandler function pointer.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF1_SetInterruptHandler(MyInterruptHandler);

*/
void IOCBF1_SetInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Dynamic Interrupt Handler for IOCBF1 pin

  @Description
    This is a dynamic interrupt handler to be used together with the IOCBF1_SetInterruptHandler() method.
    This handler is called every time the IOCBF1 ISR is executed and allows any function to be registered at runtime.
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF1_SetInterruptHandler(IOCBF1_InterruptHandler);

*/
extern void (*IOCBF1_InterruptHandler)(void);

/**
  @Summary
    Default Interrupt Handler for IOCBF1 pin

  @Description
    This is a predefined interrupt handler to be used together with the IOCBF1_SetInterruptHandler() method.
    This handler is called every time the IOCBF1 ISR is executed. 
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF1_SetInterruptHandler(IOCBF1_DefaultInterruptHandler);

*/
void IOCBF1_DefaultInterruptHandler(void);


/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handler for the IOCBF2 pin functionality
 * @Example
    IOCBF2_ISR();
 */
void IOCBF2_ISR(void);

/**
  @Summary
    Interrupt Handler Setter for IOCBF2 pin interrupt-on-change functionality

  @Description
    Allows selecting an interrupt handler for IOCBF2 at application runtime
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    InterruptHandler function pointer.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF2_SetInterruptHandler(MyInterruptHandler);

*/
void IOCBF2_SetInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Dynamic Interrupt Handler for IOCBF2 pin

  @Description
    This is a dynamic interrupt handler to be used together with the IOCBF2_SetInterruptHandler() method.
    This handler is called every time the IOCBF2 ISR is executed and allows any function to be registered at runtime.
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF2_SetInterruptHandler(IOCBF2_InterruptHandler);

*/
extern void (*IOCBF2_InterruptHandler)(void);

/**
  @Summary
    Default Interrupt Handler for IOCBF2 pin

  @Description
    This is a predefined interrupt handler to be used together with the IOCBF2_SetInterruptHandler() method.
    This handler is called every time the IOCBF2 ISR is executed. 
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF2_SetInterruptHandler(IOCBF2_DefaultInterruptHandler);

*/
void IOCBF2_DefaultInterruptHandler(void);


/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handler for the IOCBF3 pin functionality
 * @Example
    IOCBF3_ISR();
 */
void IOCBF3_ISR(void);

/**
  @Summary
    Interrupt Handler Setter for IOCBF3 pin interrupt-on-change functionality

  @Description
    Allows selecting an interrupt handler for IOCBF3 at application runtime
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    InterruptHandler function pointer.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF3_SetInterruptHandler(MyInterruptHandler);

*/
void IOCBF3_SetInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Dynamic Interrupt Handler for IOCBF3 pin

  @Description
    This is a dynamic interrupt handler to be used together with the IOCBF3_SetInterruptHandler() method.
    This handler is called every time the IOCBF3 ISR is executed and allows any function to be registered at runtime.
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF3_SetInterruptHandler(IOCBF3_InterruptHandler);

*/
extern void (*IOCBF3_InterruptHandler)(void);

/**
  @Summary
    Default Interrupt Handler for IOCBF3 pin

  @Description
    This is a predefined interrupt handler to be used together with the IOCBF3_SetInterruptHandler() method.
    This handler is called every time the IOCBF3 ISR is executed. 
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF3_SetInterruptHandler(IOCBF3_DefaultInterruptHandler);

*/
void IOCBF3_DefaultInterruptHandler(void);


/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handler for the IOCBF4 pin functionality
 * @Example
    IOCBF4_ISR();
 */
void IOCBF4_ISR(void);

/**
  @Summary
    Interrupt Handler Setter for IOCBF4 pin interrupt-on-change functionality

  @Description
    Allows selecting an interrupt handler for IOCBF4 at application runtime
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    InterruptHandler function pointer.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF4_SetInterruptHandler(MyInterruptHandler);

*/
void IOCBF4_SetInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Dynamic Interrupt Handler for IOCBF4 pin

  @Description
    This is a dynamic interrupt handler to be used together with the IOCBF4_SetInterruptHandler() method.
    This handler is called every time the IOCBF4 ISR is executed and allows any function to be registered at runtime.
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF4_SetInterruptHandler(IOCBF4_InterruptHandler);

*/
extern void (*IOCBF4_InterruptHandler)(void);

/**
  @Summary
    Default Interrupt Handler for IOCBF4 pin

  @Description
    This is a predefined interrupt handler to be used together with the IOCBF4_SetInterruptHandler() method.
    This handler is called every time the IOCBF4 ISR is executed. 
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF4_SetInterruptHandler(IOCBF4_DefaultInterruptHandler);

*/
void IOCBF4_DefaultInterruptHandler(void);


/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handler for the IOCBF5 pin functionality
 * @Example
    IOCBF5_ISR();
 */
void IOCBF5_ISR(void);

/**
  @Summary
    Interrupt Handler Setter for IOCBF5 pin interrupt-on-change functionality

  @Description
    Allows selecting an interrupt handler for IOCBF5 at application runtime
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    InterruptHandler function pointer.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF5_SetInterruptHandler(MyInterruptHandler);

*/
void IOCBF5_SetInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Dynamic Interrupt Handler for IOCBF5 pin

  @Description
    This is a dynamic interrupt handler to be used together with the IOCBF5_SetInterruptHandler() method.
    This handler is called every time the IOCBF5 ISR is executed and allows any function to be registered at runtime.
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF5_SetInterruptHandler(IOCBF5_InterruptHandler);

*/
extern void (*IOCBF5_InterruptHandler)(void);

/**
  @Summary
    Default Interrupt Handler for IOCBF5 pin

  @Description
    This is a predefined interrupt handler to be used together with the IOCBF5_SetInterruptHandler() method.
    This handler is called every time the IOCBF5 ISR is executed. 
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF5_SetInterruptHandler(IOCBF5_DefaultInterruptHandler);

*/
void IOCBF5_DefaultInterruptHandler(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/