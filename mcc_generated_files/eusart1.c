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

/**
 * bool EUSART1_Read_String(void) {

    eusart_Tx_On = 1;
    eusart_Tx_En = 1;
    while (eusart_Tx_On == 1);
    eusart_Tx_On = 0;
    eusart_Tx_En = 0;

    return true;

}
 */

void EUSART1_Write(uint8_t txData) {
    while (!PIR1bits.TX1IF);
    TXREG1 = txData; // Write the data byte to the USART.
}

void EUSART1_Receive_ISR(void) {

    if (RCSTA1bits.OERR) {
        // EUSART1 error - restart
        RCSTA1bits.CREN = 0;
        RCSTA1bits.CREN = 1;
    }

    if (j >= 0 && j < StrSIZE - 1 && strUSART[j] != '\r') {

        eusart_Tx_On = 1;
        strUSART[j] = RCREG1;

        //repetir o caracter para o terminal
        if (j == 40) {
            while (!PIR1bits.TX1IF);
            TXREG1 = '\r';
        } else {
            while (!PIR1bits.TX1IF);
            TXREG1 = strUSART[j];
        }

        //apagar na string no caso de backspace
        if (strUSART[j] != '\r' && strUSART[j] == 8) {
            strUSART[j] = '\0';
            j--;
            if (j >= 0) {
                strUSART[j] = '\0';
                j--;
            }
        }
        j++;
    }

    //se a entrada for do tipo "SA=xx\r" calcular a nova temp de alarme
    if (( strUSART[0] == 'S' ) && ( strUSART[1] == 'A' ) && ( strUSART[2] == '=' ) && ( strUSART[5] == '\r' )) {

        int i1, i2;

        i1 = strUSART[3] - '0';
        i2 = strUSART[4] - '0';

        if (i1 >= 0 && i1 <= 9 && i2 >= 0 && i2 <= 9) {

            char tmpAlarme = 10 * i1 + i2;

            if (tmpAlarme >= 10 && tmpAlarme <= 40)
                tempAlarme = tmpAlarme;
        }

    }

    if (( strUSART[0] == 'R' ) && ( strUSART[1] == 'H' ) && ( strUSART[2] == '=' ) && ( strUSART[4] == '\r' || strUSART[5] == '\r' || strUSART[6] == '\r' || strUSART[7] == '\r' )) {
        if (strUSART[4] == '\r' || strUSART[5] == '\r' || strUSART[6] == '\r' || strUSART[7] == '\r') {
            if (strUSART[3] >= '0' && strUSART[3] <= '9')
                numLeituras = strUSART[3] - '0';
        }
        if (strUSART[5] == '\r' || strUSART[6] == '\r' || strUSART[7] == '\r') {
            if (strUSART[4] >= '0' && strUSART[4] <= '9')
                numLeituras = ( numLeituras * 10 ) + strUSART[4] - '0';
        }
        if (strUSART[6] == '\r' || strUSART[7] == '\r') {
            if (strUSART[5] >= '0' && strUSART[5] <= '9')
                numLeituras = ( numLeituras * 10 ) + strUSART[5] - '0';
        }
        if (strUSART[7] == '\r') {
            if (strUSART[6] >= '0' && strUSART[6] <= '9')
                numLeituras = ( numLeituras * 10 ) + strUSART[6] - '0';
        }
        if (numLeituras > regNum)
            numLeituras = 0;
    }

    if (j == StrSIZE - 1 || strUSART[( j - 1 )] == '\r') {
        strUSART[j - 1] = '\0'; //ultimo caracter vai ser sempre o nulo
        j = 0;
        eusart_Tx_On = 0;
    }

    if (numLeituras > 0) {

        sprintf(strUSART, "\f!ATENCAO!ATENCAO!ATENCAO!\rAPENAS ALARME A FUNCIONAR DE MOMENTO!\rPOR FAVOR AGUARDE\r");
        for (i = 0; strUSART[i] != '\0'; i++) {
            EUSART1_Write(strUSART[i]);
            strUSART[i] = '\0';
        }

        for (int k = numLeituras; k != 0; k--) {

            memAddr = regCountAux * 8;

            i2cReadAddr[0] = ( memAddr >> 8 );
            i2cReadAddr[1] = ( memAddr & 0xff );

            I2C1_MasterWrite(i2cReadAddr, 2, eepromAddr, stateMsgI2c);
            I2C1_MasterRead(i2cReadBlock, 8, eepromAddr, stateMsgI2c);

            sprintf(strUSART, "TM=%2d_TA=%2d_AA=%1d_RN=%04d\r", i2cReadBlock[1], i2cReadBlock[3], i2cReadBlock[5], regNum);
            for (i = 0; strUSART[i] != '\0'; i++) {
                EUSART1_Write(strUSART[i]);
                strUSART[i] = '\0';
            }

            regCountAux--;
            regNum--;

            if (regCountAux == 0)
                regCountAux = 4095;

        }

        i2cReadAddr[0] = 0;
        i2cReadAddr[1] = 0;

        I2C1_MasterWrite(i2cReadAddr, 2, eepromAddr, stateMsgI2c);
        I2C1_MasterRead(i2cReadBlock, 4, eepromAddr, stateMsgI2c);

        memAddr = ( ( i2cReadBlock[0] << 8 ) + i2cReadBlock[1] );
        regNum = ( ( i2cReadBlock[2] << 8 ) + i2cReadBlock[3] );
        regCountAux = memAddr >> 3;

        RCREG1 = 0;
        while (RCREG1 != '\r');

        numLeituras = 0;

    }

}
/**
  End of File
 */
