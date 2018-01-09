/**
  Generated Pin Manager File

  Company:
    Microchip Technology Inc.

  File Name:
    pin_manager.c

  Summary:
    This is the Pin Manager file generated using MPLAB(c) Code Configurator

  Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.26
        Device            :  PIC18F45K22
        Driver Version    :  1.02
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

#include <xc.h>
#include "pin_manager.h"
#include "stdbool.h"



void PIN_MANAGER_Initialize(void)
{
    /**
    LATx registers
    */   
    //nenhum destes registos precisa de valores inicias, excepto o porto B, configurados a 0
    LATE = 0x00;    
    LATD = 0x00;    
    LATA = 0x00;    
    LATB = 0x78; //linhas 3, 4, 5, 6 e 7 a HIGH   
    LATC = 0x00;    

    /**
    TRISx registers
    */
    //registos que indicam a "dire��o" do pino
    TRISE = 0x07; //apenas RE3 usado como entrada, restantes n�o usados s�o configurados como entradas
    TRISA = 0xBF; //RA0, 1 e 2 usados como entradas (bot�es);RA3 usado como Vref+, configurado como entrada; RA5 usado como entrada (ADC); RA4 n�o usado, configurado como entrada, RA6 como saida e RA7 como entrada(OSC)
    TRISB = 0x07; //RB0, 1 e 2 usados para interrup��es externas, RB 3 a 6 usados como sa�das para teclado matricial, RB7 usado como ss�da para LED de alarme
    TRISC = 0xBB; //RC2 como sa�da para PWM, RC6 como sa�da, RC7 como entrada, ambos para EUSART, RC3 e 4 para I2C, n�o interessa, configurados como entrada, restantes, n�o usados, configurados como entrada
    TRISD = 0x00; //todo o porto � usado para o LCD, tudo configurado como sa�da

    /**
    ANSELx registers
    */   
    ANSELC = 0x20; //nenhum � usado como anal�gico, mas como RC5 pode ser anal�gico e n�o � configurado, o seu default � ser anal�gico, restantes s�o digitais ou default
    ANSELB = 0x00; //nenhum � usado como anal�gico
    ANSELD = 0x00; //nenhum � usado como anal�gico
    ANSELE = 0x07; //nenhum � usado como anal�gico, mas como RE0, 1 e 2 podem ser anal�gicos e n�o s�o configurados, o seu default � serem anal�gicos, RE3 � digital
    ANSELA = 0x28; //RA3 � usado como anal�gico para o Vref+, RA5 usado como entrada anal�gica para o ADC; restantes s�o digitais ou default

    /**
    WPUx registers
    */ 
    //n�o s�o usadas resist�ncias de pull-up internas
    WPUB = 0x00;
    INTCON2bits.nRBPU = 0;   
    
}       

void PIN_MANAGER_IOC(void)
{   

}

/**
 End of File
*/