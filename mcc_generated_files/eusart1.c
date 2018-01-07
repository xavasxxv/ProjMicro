/**
  EUSART1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    eusart1.c

  @Summary
    This is the generated driver implementation file for the EUSART1 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs 

  @Description
    This header file provides implementations for driver APIs for EUSART1.
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
#include "eusart1.h"
#include "../GlobalDefinitions.h"

/**
  Section: Macro Declarations
 */

/**
  Section: Global Variables
 */

/**
  Section: EUSART1 APIs
 */

void EUSART1_Initialize(void) {
    // disable interrupts before changing states
    PIE1bits.RC1IE = 0;

    // Set the EUSART1 module to the options selected in the user interface.

    // ABDOVF no_overflow; CKTXP async_noninverted_sync_fallingedge; BRG16 16bit_generator; WUE disabled; ABDEN disabled; DTRXP not_inverted; 
    BAUDCON1 = 0x08;

    // SPEN enabled; RX9 8-bit; CREN enabled; ADDEN disabled; SREN disabled; 
    RCSTA1 = 0x90;

    // TX9 8-bit; TX9D 0; SENDB sync_break_complete; TXEN enabled; SYNC asynchronous; BRGH hi_speed; CSRC slave_mode; 
    TXSTA1 = 0x24;

    // Baud Rate = 9600; 
    SPBRG1 = 0x9B;

    // Baud Rate = 9600; 
    SPBRGH1 = 0x00;

    // enable receive interrupt
    PIE1bits.RC1IE = 1;
}

//esta fun��o foi alterada, corresponde � equivalente caso as interrup��es estivessem desligadas para a EUSART, uma vez que para envio n�o s�o usadas interrup��es
void EUSART1_Write(uint8_t txData) {
    while (!PIR1bits.TX1IF);
    TXREG1 = txData; // Write the data byte to the USART.
}

void EUSART1_Receive_ISR(void) {

    //em caso de erro, reinicia o m�dulo
    if (RCSTA1bits.OERR) {
        // EUSART1 error - restart
        RCSTA1bits.CREN = 0;
        RCSTA1bits.CREN = 1;
    }
    
    //copia o caract�r recebido para a posi��o correspondente na string EUSART
    strUSART[j] = RCREG1;

    //enquanto o caract�r n�o for ENTER ou a string n�o atingir o tamanho m�ximo de 40 (mais o ENTER)
    if (j >= 0 && j < StrSIZE - 1) {

        //indica que est� no meio da rece��o de uma string
        estadoEUSART = 1;

        //repete o caract�r para o terminal virtual
        //caso esteja no limite de caract�res manda um ENTER
        if (j == 40) {
            while (!PIR1bits.TX1IF);
            TXREG1 = '\r';
            //se n�o repete o caract�r recebido
        } else {
            while (!PIR1bits.TX1IF);
            TXREG1 = strUSART[j];
        }

        //apaga o caract�r recebido da string no caso de se ter apagado (BACKSPACE)
        //apaga 1� o BACKSPACE em si da string
        if (strUSART[j] != '\r' && strUSART[j] == 8) {
            strUSART[j] = '\0';
            j--;
            //se n�o estiver na posi��o zero da string j�, apaga ent�o efetivamente o caract�r pretendido
            if (j >= 0) {
                strUSART[j] = '\0';
                j--;
            }
        }
        //acerta a posi��o de rece��o
        j++;
    }

    //caso se trate do fim da rece��o (caso ENTER ou fim de string)
    if (j == StrSIZE - 1 || strUSART[( j - 1 )] == '\r') {
        //o contador passa a 0, para puder receber outra string
        j = 0;
        //no caso de ter estado bloqueada anteriormente, desbloqueia a escrita da EUSART
        if (bloqueiaEUSART == 1)
            bloqueiaEUSART = 0;
        //indica que terminou de recebar a string
        estadoEUSART = 0;
    }

    //no caso de haver algum tipo de problema, recebe o caract�r recebido, de modo a puder ser limpa a flag de interrup��o da EUSART
    lixoEUSART = RCREG1;
    
    //a interrup��o executou, falta o c�digo auxiliar assim que poss�vel
    intEUSART = 1;

}
/**
  End of File
 */
