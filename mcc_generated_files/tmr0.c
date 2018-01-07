/**
  TMR0 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    tmr0.c

  @Summary
    This is the generated driver implementation file for the TMR0 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs 

  @Description
    This source file provides APIs for TMR0.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs  - 1.45
        Device            :  PIC18F45K22
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40
 */

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
 */

/**
  Section: Included Files
 */

#include <xc.h>
#include "tmr0.h"
#include "tmr2.h"
#include "epwm1.h"
#include "adc.h"
#include "../GlobalDefinitions.h"
#include "pin_manager.h"

/**
  Section: Global Variables Definitions
 */

void (*TMR0_InterruptHandler )( void );

volatile uint16_t timer0ReloadVal;

/**
  Section: TMR0 APIs
 */


void TMR0_Initialize(void) {
    // Set TMR0 to the options selected in the User Interface

    // T0PS 1:4; T08BIT 16-bit; T0SE Increment_hi_lo; T0CS FOSC/4; TMR0ON off; PSA assigned; 
    T0CON = 0x11;

    // TMR0H 72; 
    TMR0H = 0x48;

    // TMR0L 228; 
    TMR0L = 0xE4;

    // Load TMR0 value to the 16-bit reload variable
    timer0ReloadVal = ( TMR0H << 8 ) | TMR0L;

    // Clear Interrupt flag before enabling the interrupt
    INTCONbits.TMR0IF = 0;

    // Enabling TMR0 interrupt.
    INTCONbits.TMR0IE = 1;

    // Set Default Interrupt Handler
    TMR0_SetInterruptHandler(TMR0_DefaultInterruptHandler);

    // Start TMR0
    TMR0_StartTimer();
}

void TMR0_StartTimer(void) {
    // Start the Timer by writing to TMR0ON bit
    T0CONbits.TMR0ON = 1;
}

void TMR0_StopTimer(void) {
    // Stop the Timer by writing to TMR0ON bit
    T0CONbits.TMR0ON = 0;
}

uint16_t TMR0_ReadTimer(void) {
    uint16_t readVal;
    uint8_t readValLow;
    uint8_t readValHigh;

    readValLow = TMR0L;
    readValHigh = TMR0H;
    readVal = ( ( uint16_t ) readValHigh << 8 ) + readValLow;

    return readVal;
}

void TMR0_WriteTimer(uint16_t timerVal) {
    // Write to the Timer0 register
    TMR0H = timerVal >> 8;
    TMR0L = ( uint8_t ) timerVal;
}

void TMR0_Reload(void) {
    // Write to the Timer0 register
    TMR0H = timer0ReloadVal >> 8;
    TMR0L = ( uint8_t ) timer0ReloadVal;
}

void TMR0_ISR(void) {

    // clear the TMR0 interrupt flag
    INTCONbits.TMR0IF = 0;

    // reload TMR0
    // Write to the Timer0 register
    TMR0H = timer0ReloadVal >> 8;
    TMR0L = ( uint8_t ) timer0ReloadVal;

    if (TMR0_InterruptHandler) {
        TMR0_InterruptHandler();
    }

    //divisores de relógio, que permitem que tenha várias "frequências" com base no tempo de interrupção definido (8 vezes por segundo)
    //divisor para 4Hz
    clk4Hz = !clk4Hz;
    //caso o relógio de 4Hz esteja a 1, manda iniciar uma conversão do ADC, e divide também para os 2Hz, assere a variável auxiliar de update do LCD a 1
    if (clk4Hz) {
        ADC_StartConversion();
        clk2Hz = !clk2Hz;
        update4hZ = 1;
    }
    //divisor para 1Hz
    if (clk2Hz && clk4Hz)
        clk1Hz = !clk1Hz;
    //divisor para 1/2Hz (2 seg)
    if (clk1Hz && clk2Hz && clk4Hz)
        clk1_2Hz = !clk1_2Hz;
    //divisor para 1/4Hz (4 seg)
    if (clk1_2Hz && clk1Hz && clk2Hz && clk4Hz)
        clk1_4Hz = !clk1_4Hz;
    //divisor para 1/8Hz (8 seg)
    if (clk1_4Hz && clk1_2Hz && clk1Hz && clk2Hz && clk4Hz)
        clk1_8Hz = !clk1_8Hz;

    //caso o alarme esteja a 1
    if (alarme == 1) {

        //inicia o Timer2, do qual é dependente o módulo ECCP1, responsável pelo PWM do buzzer
        TMR2_StartTimer();

        //alterna o LED e o buzzer a cada ciclo relógio de 2hz, o buzzer funciona sempre a 12.5% de duty-cycle
        switch (clk2Hz) {
            //no ciclo negativo, o LED desliga e o buzzer apita a uma freq. mais baixa (375Hz)
            case 0:
                EPWM1_LoadDutyValue(124); //1/8 do PR2
                TMR2_LoadPeriodRegister(249); //xF9 b11111001
                RB7_SetLow();
                break;
            //no ciclo positivo, o LED liga e o buzzer apita a uma freq. mais alta (625Hz)
            case 1:
                EPWM1_LoadDutyValue(74); //1/8 do PR2
                TMR2_LoadPeriodRegister(149); //x95 b10010101
                RB7_SetHigh();
                break;
        }
    //caso o alarme esteja a 0
    } else {
        
        //inicia o Timer2, do qual é dependente o módulo ECCP1, responsável pelo PWM do buzzer
        TMR2_StopTimer();
        //e desliga forçosamente o LED
        RB7_SetLow();
    }

}

void TMR0_SetInterruptHandler(void (* InterruptHandler )( void )) {
    TMR0_InterruptHandler = InterruptHandler;
}

void TMR0_DefaultInterruptHandler(void) {
    // add your TMR0 interrupt custom code
    // or set custom function using TMR0_SetInterruptHandler()
}

/**
  End of File
 */