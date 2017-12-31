/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using MPLAB(c) Code Configurator

  @Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.26
        Device            :  PIC18F45K22
        Version           :  1.01
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

    Microchip licenses to you the right to use, modify, copy and distribute
    Software only when embedded on a Microchip microcontroller or digital signal
    controller that is integrated into your product or third party product
    (pursuant to the sublicense terms in the accompanying license agreement).

    You should refer to the license agreement accompanying this Software for
    additional information regarding your rights and obligations.

    SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
    EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
    MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
    IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
    CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
    OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
    INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
    CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
    SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
    (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

*/


#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set BOTAO_UP aliases
#define BOTAO_UP_TRIS               TRISAbits.TRISA0
#define BOTAO_UP_LAT                LATAbits.LATA0
#define BOTAO_UP_PORT               PORTAbits.RA0
#define BOTAO_UP_ANS                ANSELAbits.ANSA0
#define BOTAO_UP_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define BOTAO_UP_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define BOTAO_UP_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define BOTAO_UP_GetValue()           PORTAbits.RA0
#define BOTAO_UP_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define BOTAO_UP_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define BOTAO_UP_SetAnalogMode()  do { ANSELAbits.ANSA0 = 1; } while(0)
#define BOTAO_UP_SetDigitalMode() do { ANSELAbits.ANSA0 = 0; } while(0)

// get/set BOTAO_SEL aliases
#define BOTAO_SEL_TRIS               TRISAbits.TRISA1
#define BOTAO_SEL_LAT                LATAbits.LATA1
#define BOTAO_SEL_PORT               PORTAbits.RA1
#define BOTAO_SEL_ANS                ANSELAbits.ANSA1
#define BOTAO_SEL_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define BOTAO_SEL_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define BOTAO_SEL_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define BOTAO_SEL_GetValue()           PORTAbits.RA1
#define BOTAO_SEL_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define BOTAO_SEL_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define BOTAO_SEL_SetAnalogMode()  do { ANSELAbits.ANSA1 = 1; } while(0)
#define BOTAO_SEL_SetDigitalMode() do { ANSELAbits.ANSA1 = 0; } while(0)

// get/set BOTAO_DOWN aliases
#define BOTAO_DOWN_TRIS               TRISAbits.TRISA2
#define BOTAO_DOWN_LAT                LATAbits.LATA2
#define BOTAO_DOWN_PORT               PORTAbits.RA2
#define BOTAO_DOWN_ANS                ANSELAbits.ANSA2
#define BOTAO_DOWN_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define BOTAO_DOWN_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define BOTAO_DOWN_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define BOTAO_DOWN_GetValue()           PORTAbits.RA2
#define BOTAO_DOWN_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define BOTAO_DOWN_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define BOTAO_DOWN_SetAnalogMode()  do { ANSELAbits.ANSA2 = 1; } while(0)
#define BOTAO_DOWN_SetDigitalMode() do { ANSELAbits.ANSA2 = 0; } while(0)

// get/set channel_AN3 aliases
#define channel_AN3_TRIS               TRISAbits.TRISA3
#define channel_AN3_LAT                LATAbits.LATA3
#define channel_AN3_PORT               PORTAbits.RA3
#define channel_AN3_ANS                ANSELAbits.ANSA3
#define channel_AN3_SetHigh()            do { LATAbits.LATA3 = 1; } while(0)
#define channel_AN3_SetLow()             do { LATAbits.LATA3 = 0; } while(0)
#define channel_AN3_Toggle()             do { LATAbits.LATA3 = ~LATAbits.LATA3; } while(0)
#define channel_AN3_GetValue()           PORTAbits.RA3
#define channel_AN3_SetDigitalInput()    do { TRISAbits.TRISA3 = 1; } while(0)
#define channel_AN3_SetDigitalOutput()   do { TRISAbits.TRISA3 = 0; } while(0)
#define channel_AN3_SetAnalogMode()  do { ANSELAbits.ANSA3 = 1; } while(0)
#define channel_AN3_SetDigitalMode() do { ANSELAbits.ANSA3 = 0; } while(0)

// get/set RA6 procedures
#define RA6_SetHigh()    do { LATAbits.LATA6 = 1; } while(0)
#define RA6_SetLow()   do { LATAbits.LATA6 = 0; } while(0)
#define RA6_Toggle()   do { LATAbits.LATA6 = ~LATAbits.LATA6; } while(0)
#define RA6_GetValue()         PORTAbits.RA6
#define RA6_SetDigitalInput()   do { TRISAbits.TRISA6 = 1; } while(0)
#define RA6_SetDigitalOutput()  do { TRISAbits.TRISA6 = 0; } while(0)

// get/set RB0 procedures
#define RB0_SetHigh()    do { LATBbits.LATB0 = 1; } while(0)
#define RB0_SetLow()   do { LATBbits.LATB0 = 0; } while(0)
#define RB0_Toggle()   do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define RB0_GetValue()         PORTBbits.RB0
#define RB0_SetDigitalInput()   do { TRISBbits.TRISB0 = 1; } while(0)
#define RB0_SetDigitalOutput()  do { TRISBbits.TRISB0 = 0; } while(0)
#define RB0_SetPullup()     do { WPUBbits.WPUB0 = 1; } while(0)
#define RB0_ResetPullup()   do { WPUBbits.WPUB0 = 0; } while(0)
#define RB0_SetAnalogMode() do { ANSELBbits.ANSB0 = 1; } while(0)
#define RB0_SetDigitalMode()do { ANSELBbits.ANSB0 = 0; } while(0)

// get/set RB1 procedures
#define RB1_SetHigh()    do { LATBbits.LATB1 = 1; } while(0)
#define RB1_SetLow()   do { LATBbits.LATB1 = 0; } while(0)
#define RB1_Toggle()   do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define RB1_GetValue()         PORTBbits.RB1
#define RB1_SetDigitalInput()   do { TRISBbits.TRISB1 = 1; } while(0)
#define RB1_SetDigitalOutput()  do { TRISBbits.TRISB1 = 0; } while(0)
#define RB1_SetPullup()     do { WPUBbits.WPUB1 = 1; } while(0)
#define RB1_ResetPullup()   do { WPUBbits.WPUB1 = 0; } while(0)
#define RB1_SetAnalogMode() do { ANSELBbits.ANSB1 = 1; } while(0)
#define RB1_SetDigitalMode()do { ANSELBbits.ANSB1 = 0; } while(0)

// get/set RB2 procedures
#define RB2_SetHigh()    do { LATBbits.LATB2 = 1; } while(0)
#define RB2_SetLow()   do { LATBbits.LATB2 = 0; } while(0)
#define RB2_Toggle()   do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define RB2_GetValue()         PORTBbits.RB2
#define RB2_SetDigitalInput()   do { TRISBbits.TRISB2 = 1; } while(0)
#define RB2_SetDigitalOutput()  do { TRISBbits.TRISB2 = 0; } while(0)
#define RB2_SetPullup()     do { WPUBbits.WPUB2 = 1; } while(0)
#define RB2_ResetPullup()   do { WPUBbits.WPUB2 = 0; } while(0)
#define RB2_SetAnalogMode() do { ANSELBbits.ANSB2 = 1; } while(0)
#define RB2_SetDigitalMode()do { ANSELBbits.ANSB2 = 0; } while(0)

// get/set RB3 aliases
#define RB3_TRIS               TRISBbits.TRISB3
#define RB3_LAT                LATBbits.LATB3
#define RB3_PORT               PORTBbits.RB3
#define RB3_WPU                WPUBbits.WPUB3
#define RB3_ANS                ANSELBbits.ANSB3
#define RB3_SetHigh()            do { LATBbits.LATB3 = 1; } while(0)
#define RB3_SetLow()             do { LATBbits.LATB3 = 0; } while(0)
#define RB3_Toggle()             do { LATBbits.LATB3 = ~LATBbits.LATB3; } while(0)
#define RB3_GetValue()           PORTBbits.RB3
#define RB3_SetDigitalInput()    do { TRISBbits.TRISB3 = 1; } while(0)
#define RB3_SetDigitalOutput()   do { TRISBbits.TRISB3 = 0; } while(0)
#define RB3_SetPullup()      do { WPUBbits.WPUB3 = 1; } while(0)
#define RB3_ResetPullup()    do { WPUBbits.WPUB3 = 0; } while(0)
#define RB3_SetAnalogMode()  do { ANSELBbits.ANSB3 = 1; } while(0)
#define RB3_SetDigitalMode() do { ANSELBbits.ANSB3 = 0; } while(0)

// get/set RB4 aliases
#define RB4_TRIS               TRISBbits.TRISB4
#define RB4_LAT                LATBbits.LATB4
#define RB4_PORT               PORTBbits.RB4
#define RB4_WPU                WPUBbits.WPUB4
#define RB4_ANS                ANSELBbits.ANSB4
#define RB4_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define RB4_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define RB4_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define RB4_GetValue()           PORTBbits.RB4
#define RB4_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define RB4_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define RB4_SetPullup()      do { WPUBbits.WPUB4 = 1; } while(0)
#define RB4_ResetPullup()    do { WPUBbits.WPUB4 = 0; } while(0)
#define RB4_SetAnalogMode()  do { ANSELBbits.ANSB4 = 1; } while(0)
#define RB4_SetDigitalMode() do { ANSELBbits.ANSB4 = 0; } while(0)

// get/set RB5 aliases
#define RB5_TRIS               TRISBbits.TRISB5
#define RB5_LAT                LATBbits.LATB5
#define RB5_PORT               PORTBbits.RB5
#define RB5_WPU                WPUBbits.WPUB5
#define RB5_ANS                ANSELBbits.ANSB5
#define RB5_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define RB5_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define RB5_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define RB5_GetValue()           PORTBbits.RB5
#define RB5_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define RB5_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define RB5_SetPullup()      do { WPUBbits.WPUB5 = 1; } while(0)
#define RB5_ResetPullup()    do { WPUBbits.WPUB5 = 0; } while(0)
#define RB5_SetAnalogMode()  do { ANSELBbits.ANSB5 = 1; } while(0)
#define RB5_SetDigitalMode() do { ANSELBbits.ANSB5 = 0; } while(0)

// get/set RB6 aliases
#define RB6_TRIS               TRISBbits.TRISB6
#define RB6_LAT                LATBbits.LATB6
#define RB6_PORT               PORTBbits.RB6
#define RB6_WPU                WPUBbits.WPUB6
#define RB6_SetHigh()            do { LATBbits.LATB6 = 1; } while(0)
#define RB6_SetLow()             do { LATBbits.LATB6 = 0; } while(0)
#define RB6_Toggle()             do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define RB6_GetValue()           PORTBbits.RB6
#define RB6_SetDigitalInput()    do { TRISBbits.TRISB6 = 1; } while(0)
#define RB6_SetDigitalOutput()   do { TRISBbits.TRISB6 = 0; } while(0)
#define RB6_SetPullup()      do { WPUBbits.WPUB6 = 1; } while(0)
#define RB6_ResetPullup()    do { WPUBbits.WPUB6 = 0; } while(0)

// get/set RB7 aliases
#define RB7_TRIS               TRISBbits.TRISB7
#define RB7_LAT                LATBbits.LATB7
#define RB7_PORT               PORTBbits.RB7
#define RB7_WPU                WPUBbits.WPUB7
#define RB7_SetHigh()            do { LATBbits.LATB7 = 1; } while(0)
#define RB7_SetLow()             do { LATBbits.LATB7 = 0; } while(0)
#define RB7_Toggle()             do { LATBbits.LATB7 = ~LATBbits.LATB7; } while(0)
#define RB7_GetValue()           PORTBbits.RB7
#define RB7_SetDigitalInput()    do { TRISBbits.TRISB7 = 1; } while(0)
#define RB7_SetDigitalOutput()   do { TRISBbits.TRISB7 = 0; } while(0)
#define RB7_SetPullup()      do { WPUBbits.WPUB7 = 1; } while(0)
#define RB7_ResetPullup()    do { WPUBbits.WPUB7 = 0; } while(0)

// get/set RC2 procedures
#define RC2_SetHigh()    do { LATCbits.LATC2 = 1; } while(0)
#define RC2_SetLow()   do { LATCbits.LATC2 = 0; } while(0)
#define RC2_Toggle()   do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define RC2_GetValue()         PORTCbits.RC2
#define RC2_SetDigitalInput()   do { TRISCbits.TRISC2 = 1; } while(0)
#define RC2_SetDigitalOutput()  do { TRISCbits.TRISC2 = 0; } while(0)
#define RC2_SetAnalogMode() do { ANSELCbits.ANSC2 = 1; } while(0)
#define RC2_SetDigitalMode()do { ANSELCbits.ANSC2 = 0; } while(0)

// get/set RC3 procedures
#define RC3_SetHigh()    do { LATCbits.LATC3 = 1; } while(0)
#define RC3_SetLow()   do { LATCbits.LATC3 = 0; } while(0)
#define RC3_Toggle()   do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define RC3_GetValue()         PORTCbits.RC3
#define RC3_SetDigitalInput()   do { TRISCbits.TRISC3 = 1; } while(0)
#define RC3_SetDigitalOutput()  do { TRISCbits.TRISC3 = 0; } while(0)
#define RC3_SetAnalogMode() do { ANSELCbits.ANSC3 = 1; } while(0)
#define RC3_SetDigitalMode()do { ANSELCbits.ANSC3 = 0; } while(0)

// get/set RC4 procedures
#define RC4_SetHigh()    do { LATCbits.LATC4 = 1; } while(0)
#define RC4_SetLow()   do { LATCbits.LATC4 = 0; } while(0)
#define RC4_Toggle()   do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define RC4_GetValue()         PORTCbits.RC4
#define RC4_SetDigitalInput()   do { TRISCbits.TRISC4 = 1; } while(0)
#define RC4_SetDigitalOutput()  do { TRISCbits.TRISC4 = 0; } while(0)
#define RC4_SetAnalogMode() do { ANSELCbits.ANSC4 = 1; } while(0)
#define RC4_SetDigitalMode()do { ANSELCbits.ANSC4 = 0; } while(0)

// get/set RC6 procedures
#define RC6_SetHigh()    do { LATCbits.LATC6 = 1; } while(0)
#define RC6_SetLow()   do { LATCbits.LATC6 = 0; } while(0)
#define RC6_Toggle()   do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define RC6_GetValue()         PORTCbits.RC6
#define RC6_SetDigitalInput()   do { TRISCbits.TRISC6 = 1; } while(0)
#define RC6_SetDigitalOutput()  do { TRISCbits.TRISC6 = 0; } while(0)
#define RC6_SetAnalogMode() do { ANSELCbits.ANSC6 = 1; } while(0)
#define RC6_SetDigitalMode()do { ANSELCbits.ANSC6 = 0; } while(0)

// get/set RC7 procedures
#define RC7_SetHigh()    do { LATCbits.LATC7 = 1; } while(0)
#define RC7_SetLow()   do { LATCbits.LATC7 = 0; } while(0)
#define RC7_Toggle()   do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define RC7_GetValue()         PORTCbits.RC7
#define RC7_SetDigitalInput()   do { TRISCbits.TRISC7 = 1; } while(0)
#define RC7_SetDigitalOutput()  do { TRISCbits.TRISC7 = 0; } while(0)
#define RC7_SetAnalogMode() do { ANSELCbits.ANSC7 = 1; } while(0)
#define RC7_SetDigitalMode()do { ANSELCbits.ANSC7 = 0; } while(0)

// get/set RD0 aliases
#define RD0_TRIS               TRISDbits.TRISD0
#define RD0_LAT                LATDbits.LATD0
#define RD0_PORT               PORTDbits.RD0
#define RD0_ANS                ANSELDbits.ANSD0
#define RD0_SetHigh()            do { LATDbits.LATD0 = 1; } while(0)
#define RD0_SetLow()             do { LATDbits.LATD0 = 0; } while(0)
#define RD0_Toggle()             do { LATDbits.LATD0 = ~LATDbits.LATD0; } while(0)
#define RD0_GetValue()           PORTDbits.RD0
#define RD0_SetDigitalInput()    do { TRISDbits.TRISD0 = 1; } while(0)
#define RD0_SetDigitalOutput()   do { TRISDbits.TRISD0 = 0; } while(0)
#define RD0_SetAnalogMode()  do { ANSELDbits.ANSD0 = 1; } while(0)
#define RD0_SetDigitalMode() do { ANSELDbits.ANSD0 = 0; } while(0)

// get/set RD1 aliases
#define RD1_TRIS               TRISDbits.TRISD1
#define RD1_LAT                LATDbits.LATD1
#define RD1_PORT               PORTDbits.RD1
#define RD1_ANS                ANSELDbits.ANSD1
#define RD1_SetHigh()            do { LATDbits.LATD1 = 1; } while(0)
#define RD1_SetLow()             do { LATDbits.LATD1 = 0; } while(0)
#define RD1_Toggle()             do { LATDbits.LATD1 = ~LATDbits.LATD1; } while(0)
#define RD1_GetValue()           PORTDbits.RD1
#define RD1_SetDigitalInput()    do { TRISDbits.TRISD1 = 1; } while(0)
#define RD1_SetDigitalOutput()   do { TRISDbits.TRISD1 = 0; } while(0)
#define RD1_SetAnalogMode()  do { ANSELDbits.ANSD1 = 1; } while(0)
#define RD1_SetDigitalMode() do { ANSELDbits.ANSD1 = 0; } while(0)

// get/set RD2 aliases
#define RD2_TRIS               TRISDbits.TRISD2
#define RD2_LAT                LATDbits.LATD2
#define RD2_PORT               PORTDbits.RD2
#define RD2_ANS                ANSELDbits.ANSD2
#define RD2_SetHigh()            do { LATDbits.LATD2 = 1; } while(0)
#define RD2_SetLow()             do { LATDbits.LATD2 = 0; } while(0)
#define RD2_Toggle()             do { LATDbits.LATD2 = ~LATDbits.LATD2; } while(0)
#define RD2_GetValue()           PORTDbits.RD2
#define RD2_SetDigitalInput()    do { TRISDbits.TRISD2 = 1; } while(0)
#define RD2_SetDigitalOutput()   do { TRISDbits.TRISD2 = 0; } while(0)
#define RD2_SetAnalogMode()  do { ANSELDbits.ANSD2 = 1; } while(0)
#define RD2_SetDigitalMode() do { ANSELDbits.ANSD2 = 0; } while(0)

// get/set RD3 aliases
#define RD3_TRIS               TRISDbits.TRISD3
#define RD3_LAT                LATDbits.LATD3
#define RD3_PORT               PORTDbits.RD3
#define RD3_ANS                ANSELDbits.ANSD3
#define RD3_SetHigh()            do { LATDbits.LATD3 = 1; } while(0)
#define RD3_SetLow()             do { LATDbits.LATD3 = 0; } while(0)
#define RD3_Toggle()             do { LATDbits.LATD3 = ~LATDbits.LATD3; } while(0)
#define RD3_GetValue()           PORTDbits.RD3
#define RD3_SetDigitalInput()    do { TRISDbits.TRISD3 = 1; } while(0)
#define RD3_SetDigitalOutput()   do { TRISDbits.TRISD3 = 0; } while(0)
#define RD3_SetAnalogMode()  do { ANSELDbits.ANSD3 = 1; } while(0)
#define RD3_SetDigitalMode() do { ANSELDbits.ANSD3 = 0; } while(0)

// get/set RD4 aliases
#define RD4_TRIS               TRISDbits.TRISD4
#define RD4_LAT                LATDbits.LATD4
#define RD4_PORT               PORTDbits.RD4
#define RD4_ANS                ANSELDbits.ANSD4
#define RD4_SetHigh()            do { LATDbits.LATD4 = 1; } while(0)
#define RD4_SetLow()             do { LATDbits.LATD4 = 0; } while(0)
#define RD4_Toggle()             do { LATDbits.LATD4 = ~LATDbits.LATD4; } while(0)
#define RD4_GetValue()           PORTDbits.RD4
#define RD4_SetDigitalInput()    do { TRISDbits.TRISD4 = 1; } while(0)
#define RD4_SetDigitalOutput()   do { TRISDbits.TRISD4 = 0; } while(0)
#define RD4_SetAnalogMode()  do { ANSELDbits.ANSD4 = 1; } while(0)
#define RD4_SetDigitalMode() do { ANSELDbits.ANSD4 = 0; } while(0)

// get/set RD5 aliases
#define RD5_TRIS               TRISDbits.TRISD5
#define RD5_LAT                LATDbits.LATD5
#define RD5_PORT               PORTDbits.RD5
#define RD5_ANS                ANSELDbits.ANSD5
#define RD5_SetHigh()            do { LATDbits.LATD5 = 1; } while(0)
#define RD5_SetLow()             do { LATDbits.LATD5 = 0; } while(0)
#define RD5_Toggle()             do { LATDbits.LATD5 = ~LATDbits.LATD5; } while(0)
#define RD5_GetValue()           PORTDbits.RD5
#define RD5_SetDigitalInput()    do { TRISDbits.TRISD5 = 1; } while(0)
#define RD5_SetDigitalOutput()   do { TRISDbits.TRISD5 = 0; } while(0)
#define RD5_SetAnalogMode()  do { ANSELDbits.ANSD5 = 1; } while(0)
#define RD5_SetDigitalMode() do { ANSELDbits.ANSD5 = 0; } while(0)

// get/set RD6 aliases
#define RD6_TRIS               TRISDbits.TRISD6
#define RD6_LAT                LATDbits.LATD6
#define RD6_PORT               PORTDbits.RD6
#define RD6_ANS                ANSELDbits.ANSD6
#define RD6_SetHigh()            do { LATDbits.LATD6 = 1; } while(0)
#define RD6_SetLow()             do { LATDbits.LATD6 = 0; } while(0)
#define RD6_Toggle()             do { LATDbits.LATD6 = ~LATDbits.LATD6; } while(0)
#define RD6_GetValue()           PORTDbits.RD6
#define RD6_SetDigitalInput()    do { TRISDbits.TRISD6 = 1; } while(0)
#define RD6_SetDigitalOutput()   do { TRISDbits.TRISD6 = 0; } while(0)
#define RD6_SetAnalogMode()  do { ANSELDbits.ANSD6 = 1; } while(0)
#define RD6_SetDigitalMode() do { ANSELDbits.ANSD6 = 0; } while(0)

// get/set RD7 aliases
#define RD7_TRIS               TRISDbits.TRISD7
#define RD7_LAT                LATDbits.LATD7
#define RD7_PORT               PORTDbits.RD7
#define RD7_ANS                ANSELDbits.ANSD7
#define RD7_SetHigh()            do { LATDbits.LATD7 = 1; } while(0)
#define RD7_SetLow()             do { LATDbits.LATD7 = 0; } while(0)
#define RD7_Toggle()             do { LATDbits.LATD7 = ~LATDbits.LATD7; } while(0)
#define RD7_GetValue()           PORTDbits.RD7
#define RD7_SetDigitalInput()    do { TRISDbits.TRISD7 = 1; } while(0)
#define RD7_SetDigitalOutput()   do { TRISDbits.TRISD7 = 0; } while(0)
#define RD7_SetAnalogMode()  do { ANSELDbits.ANSD7 = 1; } while(0)
#define RD7_SetDigitalMode() do { ANSELDbits.ANSD7 = 0; } while(0)

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



#endif // PIN_MANAGER_H
/**
 End of File
*/