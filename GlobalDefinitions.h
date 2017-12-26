/* 
 * File:   GlobalDefinitions.h
 * Author: Xavier
 *
 * Created on November 23, 2017, 1:53 PM
 */

#ifndef GLOBALDEFINITIONS_H
#define	GLOBALDEFINITIONS_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "xlcd.h"
#include "mcc_generated_files/i2c1.h"

#ifdef	__cplusplus
extern "C" {
#endif

    //Defines LCD
#define CLEAR_LCD 1 //0b00000001
#define LINE1 128 //0b10000000 
#define LINE2 192 //0b11000000

    //Tamanho das strings
#define StrSIZE 42

    //Equivalente em ºC de 1LSB
#define adcLsb 0.24841333435
    
#define eepromAddr 0b01010000

    //Variaveis globais
    volatile char strLCD[StrSIZE];
    volatile char strUSART[StrSIZE];
    volatile char i2cWriteBlock[10];
    volatile char TeclaTMP;
    volatile double tmpTemp;
    volatile char lastMenu;
    volatile char update = 1;
    volatile char i;
    volatile signed char j = 0;
    volatile char menu_tmp = 1;
    volatile char tempAlarme = 25;
    volatile char tempAtual = 25;
    volatile bool clk4Hz = 0;
    volatile bool clk2Hz = 0;
    volatile bool clk1Hz = 0;
    volatile bool clk1_2Hz = 0;
    volatile bool clk1_4Hz = 0;
    volatile bool clk1_8Hz = 0;
    volatile bool alarme = 0;
    volatile int binADC = 0;
    volatile bool WrEnFl = 0;
    volatile bool eusart_Tx_En = 0;
    volatile bool eusart_Tx_On = 0;
    volatile signed int memAddr = 0;
    volatile char memAddrBytes[2];
    volatile signed int regNum = 0;
    volatile I2C1_MESSAGE_STATUS stateMsgI2c;

    //prototipos das funcoes
    void escreveLinhaLCD(char linha, char texto[StrSIZE]);
    void menu(void);
    void monitorizacao(void);
    void testeTeclado(void);
    void defTempAlarme(void);
    char getKBInput(void);
    void feedbackUSART(void);

#ifdef	__cplusplus
}
#endif

#endif	/* GLOBALDEFINITIONS_H */

