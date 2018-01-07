/**
  ADC Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    adc.c

  @Summary
    This is the generated driver implementation file for the ADC driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs 

  @Description
    This source file provides implementations for driver APIs for ADC.
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
#include "adc.h"
#include "eusart1.h"
#include "i2c1.h"
#include "../GlobalDefinitions.h"

/**
  Section: ADC Module APIs
 */

void ADC_Initialize(void) {
    // set the ADC to the options selected in the User Interface

    // GO_nDONE stop; ADON enabled; CHS AN3; 
    ADCON0 = 0x0D;

    // TRIGSEL CCP5; NVCFG VSS; PVCFG VDD; 
    ADCON1 = 0x00;

    // ADFM right; ACQT 2; ADCS FOSC/8; 
    ADCON2 = 0x89;

    // ADRESL 0; 
    ADRESL = 0x00;

    // ADRESH 0; 
    ADRESH = 0x00;

    // Enabling ADC interrupt.
    PIE1bits.ADIE = 1;
}

void ADC_SelectChannel(adc_channel_t channel) {
    // select the A/D channel
    ADCON0bits.CHS = channel;
    // Turn on the ADC module
    ADCON0bits.ADON = 1;
}

void ADC_StartConversion() {
    // Start the conversion
    ADCON0bits.GO_nDONE = 1;
}

bool ADC_IsConversionDone() {
    // Start the conversion
    return (( bool ) ( !ADCON0bits.GO_nDONE ) );
}

adc_result_t ADC_GetConversionResult(void) {
    // Conversion finished, return the result
    return (( adc_result_t ) ( ( ADRESH << 8 ) + ADRESL ) );
}

adc_result_t ADC_GetConversion(adc_channel_t channel) {
    // select the A/D channel
    ADCON0bits.CHS = channel;

    // Turn on the ADC module
    ADCON0bits.ADON = 1;

    // Start the conversion
    ADCON0bits.GO_nDONE = 1;

    // Wait for the conversion to finish
    while (ADCON0bits.GO_nDONE) {
    }

    // Conversion finished, return the result
    return (( adc_result_t ) ( ( ADRESH << 8 ) + ADRESL ) );
}

void ADC_ISR(void) {

    //recebe o valor binário da leitura do ADC
    binADC = ADC_GetConversionResult();

    //calcula a temperatura atual
    //faz um cast de um char com sinal do valor binário deslocado vezes o equivalente em graus de 1 LSB e adiciona 0.5
    //de modo a arrendondar no caso de ser por exemplo, 26.6ºC, passa a 27.1 e o cast trunca a parte decimal excedente
    tempAtual = ( signed char ) ( ( ( binADC - 83 ) * ( adcLsb ) ) + 0.5 );

    //antes de verificar o novo estado do alarme, guarda o último estado
    lastAlarme = alarme;

    //passa o estado do alarme para 1 caso a temp. medida seja maior à de alarme, caso contrário, o alarme está a 0
    if (tempAtual > tempAlarme)
        alarme = 1;
    else
        alarme = 0;

    //a interrupção executou, falta o código auxiliar assim que possível
    intADC = 1;

    // Clear the ADC interrupt flag
    PIR1bits.ADIF = 0;
}
/**
 End of File
 */