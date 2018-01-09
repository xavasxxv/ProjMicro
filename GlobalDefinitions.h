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

    //Defines para o LCD
#define CLEAR_LCD 1 //0b00000001
#define LINE1 128   //0b10000000 
#define LINE2 192   //0b11000000

    //Tamanho das strings
#define StrSIZE 42

    //Equivalente em �C de 1 LSB
#define adcLsb 0.1504472215

    //endere�o I2C da EEPROM
#define eepromAddr 0b01010000

    //Vari�veis globais
    volatile char strLCD[StrSIZE];              //string para o LCD
    volatile char strUSART[StrSIZE];            //strinf para a EUSART
    volatile char i2cWriteBlock[10];            //bloco de dados (mais endere�o) a escrever para a EEPROM
    volatile char i2cReadAddr[2];               //bloco de dados que guarda endere�os para mandar ler da EEPROM
    volatile char i2cReadBlock[8];              //bloco de dados a ler da EEPROM
    volatile char TeclaTMP;                     //vari�vel que guarda a �ltima tecla pressionada no teclado matricial
    volatile char c1;                           //caract�r recebido do teclado
    volatile char c2;                           //caract�r recebido do teclado   
    volatile char lastMenu;                     //vari�vel que guarda qual a �ltima fun��o que foi pedida no menu
    volatile char update = 1;                   //vari�vel de atualiza��es do LCD
    volatile char i;                            //contador global, usado em v�rios s�tios
    volatile signed char e = 0;                 //contador local no aux�lio das interrup��es
    volatile signed char j = 0;                 //contador global, mas que s� � usado na interrup��o da EUSART, e que corresponde � posi��o de um caract�r numa string
    volatile char menu_tmp = 5;                 //vari�vel que guarda a op��o tempor�ria pretendida
    volatile char menu_sel = 5;                 //vari�vel que guarda a op��o atual selecionada
    volatile char update1hZ = 1;                //vari�vel auxiliar de atualiza��o do LCD periodicamente
    volatile char tempAlarme = 25;              //vari�vel global que guarda a mais recente temperatura de alarme
    volatile signed char tempAtual = 25;        //vari�vel global que guarda a mais recente temperatura medida
    volatile char estado = 0;                   //vari�vel de estados global, dentro desta fun��o n�o entra em conflitos com outras fun��es
    volatile char lixoEUSART;                   //vari�vel que recebe sempre o caract�r da EUSART, de modo a asserir sempre a flag de interrup���o a 0
    volatile bool intEUSART = 0;                //flag auxiliar � interrup��o da EUSART, que diz que � preciso executar o c�digo auxiliar
    volatile bool intADC = 0;                   //flag auxiliar � interrup��o do ADC, que diz que � preciso executar o c�digo auxiliar
    volatile bool intTMR0 = 0;                  //flag auxiliar � interrup��o do TMR0, que diz que � preciso executar o c�digo auxiliar
    volatile bool clk4Hz = 0;                   //rel�gio global de 4Hz (0.25s ou 250ms)
    volatile bool clk2Hz = 0;                   //rel�gio global de 2Hz (0.5s ou 500ms)
    volatile bool clk1Hz = 0;                   //rel�gio global de 1Hz (1s)
    volatile bool clk1_2Hz = 0;                 //rel�gio global de 1/2Hz (2s)
    volatile bool clk1_4Hz = 0;                 //rel�gio global de 1/4Hz (4s)
    volatile bool clk1_8Hz = 0;                 //rel�gio global de 1/8Hz (8s)
    volatile bool lastAlarme = 0;               //vari�vel que guarda o anterior estado de alarme
    volatile bool startFunc = 0;                //flag de autaliza��o que indica �s fun��es para iniciarem os seus estados/texto
    volatile bool alarme = 0;                   //vari�vel global que guarda o mais recente estado de alarme
    volatile int binADC = 0;                    //vari�vel que guarda o valor bin�rio recebido do ADC
    volatile bool WrEnFl = 0;                   //flag global de escrita do teclado matricial
    volatile char estadoEUSART = 0;             //flag global de estado do m�dulo EUSART
    volatile char bloqueiaEUSART = 0;           //flag global de bloqueio for�ado da escrita para a EUSART
    volatile signed int memAddr = 0;            //vari�vel global que guarda o endere�o de mem�ria usado para as funcionalidades da EEPROM
    volatile signed int regNum = 0;             //vari�vel global que guarda o nr. de registos total, usado para as funcionalidades da EEPROM
    volatile signed int regCountAux = 4088;     //vari�vel global que guarda a posi��o relativa no bloco da EEPROM do �ltimo registo, usado para as funcionalidades da EEPROM
    volatile I2C1_MESSAGE_STATUS stateMsgI2c;   //vari�vel que guarda o estado do m�dulo I2C
    volatile int numLeituras;                   //vari�vel que guarda o nr. de leituras pertendidas a partir do terminal virtual

    //prot�tipos das fun��es (explica��es dentro da fun��o main)
    void escreveLinhaLCD(char linha, char texto[StrSIZE]);
    void escreveCharLCD(char linha, char caracter);
    void monitorizacao(void);
    void testeTeclado(void);
    void defTempAlarme(void);
    void feedbackUSART(void);
    void extrasInterrupoes(void);
    void iniciarValoresEEPROM(void);

#ifdef	__cplusplus
}
#endif

#endif	/* GLOBALDEFINITIONS_H */

