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

    //enquanto o caract�r n�o for ENTER ou a string n�o atingir o tamanho m�ximo de 40 (mais o ENTER)
    if (j >= 0 && j < StrSIZE - 1 && strUSART[j] != '\r') {

        //bloqueia a escrita para o terminal virtual por outros m�dulos
        eusart_Tx_On = 1;
        //copia o caract�r recebido para a posi��o correspondente na string EUSART
        strUSART[j] = RCREG1;

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

    //se a entrada no terminal virtual for do tipo "SA=xx\r",  calcula a nova temp. de alarme
    if (( strUSART[0] == 'S' ) && ( strUSART[1] == 'A' ) && ( strUSART[2] == '=' ) && ( strUSART[5] == '\r' )) {

        //vari�veis locais, que guardam o valor "real"/literal do caract�r recebido, tal como no caso do teclado matricial
        int i1, i2;

        //subtrai ao caracter '0', ou seja 48 "literal"
        i1 = strUSART[3] - '0';
        i2 = strUSART[4] - '0';

        //se os carac�teres corresponderem a digitos, tenta calcular a nova temp. de alarme
        if (i1 >= 0 && i1 <= 9 && i2 >= 0 && i2 <= 9) {

            //c�lculo local e tempor�rio da temp. de alarme, de modo a ser poss�vel testar
            char tmpAlarme = 10 * i1 + i2;

            //se a nova temp. estiver dentro dos limites, ent�o escreve a temp. tempor�ria na vari�vel global do sistema
            if (tmpAlarme >= 10 && tmpAlarme <= 40)
                tempAlarme = tmpAlarme;
        }

    }

    //se a entrada no terminal virtual for do tipo "RH=x\r", "RH=xx\r", "RH=xxx\r" ou "RH=xxxx\r", calcula o nr. de registo que se pretende ler
    if (( strUSART[0] == 'R' ) && ( strUSART[1] == 'H' ) && ( strUSART[2] == '=' ) && ( strUSART[4] == '\r' || strUSART[5] == '\r' || strUSART[6] == '\r' || strUSART[7] == '\r' )) {

        //se a entrada for de qualquer tipo ("RH=x\r", "RH=xx\r", "RH=xxx\r" ou "RH=xxxx\r")
        if (strUSART[4] == '\r' || strUSART[5] == '\r' || strUSART[6] == '\r' || strUSART[7] == '\r') {

            //se os carat�res recebidos s�o entre '0' e '9', uma vez que s�o sequencias na tabelas ASCII, pode se fazer este teste
            //o mesmo � feito para os restantes casos, apenas em posi��es diferentes
            if (strUSART[3] >= '0' && strUSART[3] <= '9')

                //o nr. de leituras � o caract�r menos '0', ou 48 "literal"
                numLeituras = strUSART[3] - '0';
        }

        //se a entrada for do tipo "RH=xx\r", "RH=xxx\r" ou "RH=xxxx\r"
        if (strUSART[5] == '\r' || strUSART[6] == '\r' || strUSART[7] == '\r') {

            //mesma verifica��o que no 1� caso, apenas para a posi��o seguinte
            if (strUSART[4] >= '0' && strUSART[4] <= '9')

                //o nr. de leituras � igual ao n�mero anterior vezes dez, uma vez que este avan�ou uma casa, mais o caract�r recebido menos '0'
                numLeituras = ( numLeituras * 10 ) + strUSART[4] - '0';
        }

        //se a entrada for do tipo "RH=xxx\r" ou "RH=xxxx\r"
        if (strUSART[6] == '\r' || strUSART[7] == '\r') {

            //mesma verifica��o que no 2� caso, apenas para a posi��o seguinte
            if (strUSART[5] >= '0' && strUSART[5] <= '9')

                //o nr. de leituras � igual ao n�mero anterior vezes dez, uma vez que este avan�ou uma casa, mais o caract�r recebido menos '0'
                numLeituras = ( numLeituras * 10 ) + strUSART[5] - '0';
        }

        //se a entrada for do tipo "RH=xxxx\r"
        if (strUSART[7] == '\r') {

            //mesma verifica��o que no 3� caso, apenas para a posi��o seguinte
            if (strUSART[6] >= '0' && strUSART[6] <= '9')

                //o nr. de leituras � igual ao n�mero anterior vezes dez, uma vez que este avan�ou uma casa, mais o caract�r recebido menos '0'
                numLeituras = ( numLeituras * 10 ) + strUSART[6] - '0';
        }

        //se o nr. de leituras pretendido for maior que o nr. de registos guardados, ent�o n�o mostra nenhum, IE. mostra 0
        if (numLeituras > regNum)
            numLeituras = 0;
    }

    //caso se trate do fim da rece��o (caso ENTER ou fim de string), a �ltima posi��o da string USART tem de ser null ('\0'), para que seja poss�vel copiar para o LCD
    if (j == StrSIZE - 1 || strUSART[( j - 1 )] == '\r') {
        //�ltimo caract�r passa a null ('\0')
        strUSART[j - 1] = '\0';
        //o contador local passa a 0, para puder receber outra string
        j = 0;
        //habilita a escrita por outros m�dulos
        eusart_Tx_On = 0;
    }

    //caso o nr. de leituras seja maior que 0, pretende-se mostrar esse nr. de leituras
    if (numLeituras > 0) {

        //aviso inicial que durante esta visualiza��o, os restantes m�dulos n�o funcionam, apenas o alarme, dado o �ltimo estado deste
        sprintf(strUSART, "\f!ATENCAO!ATENCAO!ATENCAO!\rAPENAS ALARME A FUNCIONAR DE MOMENTO!\rPOR FAVOR AGUARDE\r");
        for (i = 0; strUSART[i] != '\0'; i++) {
            EUSART1_Write(strUSART[i]);
            strUSART[i] = '\0';
        }

        //novo contador local, que come�a com o valor de leituras pretendido e decrementa at� 0
        for (int k = numLeituras; k != 0; k--) {

            //o endere�o a ler corresponde � posi��o do registo relativa no bloco de mem�ria
            memAddr = regCountAux * 8;

            //divide o endere�o pretendido em 2 bytes
            //para o 1� byte, tem de se fazer um shift register � direita em 8 bits, para se obter apenas os 1�s 8 bits
            i2cReadAddr[0] = ( memAddr >> 8 );
            //para o 2� byte, � aplicada uma m�scara E de 0x00ff, ou seja, bit a bit, uma opera��o E entre o bit que est� na vari�vel e o da m�scara
            i2cReadAddr[1] = ( memAddr & 0xff );

            //manda � EEPROM o comando para mudar para a linha pretendida
            I2C1_MasterWrite(i2cReadAddr, 2, eepromAddr, stateMsgI2c);
            //e depois l� os 8 bytes sequenciais, onde est�o guardadas as vari�veis
            I2C1_MasterRead(i2cReadBlock, 8, eepromAddr, stateMsgI2c);

            //copia para a string da EUSARTa temp. medida, a temp. de alarme e o seu estado, bem como o nr de registo correspondente
            //as vari�veis s�o guardadas como "literias", onde a temp. medida � guardada no 2� byte, a de alarme no 4� e o estado deste no 6�
            sprintf(strUSART, "TM=%2d_TA=%2d_AA=%1d_RN=%04d\r", i2cReadBlock[1], i2cReadBlock[3], i2cReadBlock[5], regNum);
            for (i = 0; strUSART[i] != '\0'; i++) {
                EUSART1_Write(strUSART[i]);
                strUSART[i] = '\0';
            }

            //diminui os contadores
            regCountAux--;
            regNum--;

            //se a posi��o relativa no registo j� estiver na 1�, passa para a �ltima e continua a subtrair
            if (regCountAux == 0)
                regCountAux = 4095;

        }

        //no final, volta a ler o �ltimo endere�o escrito e o nr. de registo da EEPROM, de modo a continuar a operar
        //faz praticamente a mesma coisa como no inicio de opera��o, exceto que como j� esteve a funcionar
        //assume-se que foi registado pelo menos 1 registo, pelo que n�o � verificado se a mem�ria est� vazia

        //endere�o de mem�ria a ler, o 1�, que � onde est�o guardados o �ltimo registo de mem�ria usado e o nr. de registos guardados at� ao momento
        i2cReadAddr[0] = 0;
        i2cReadAddr[1] = 0;

        //manda � EEPROM o comando para mudar para a linha 0x0000
        I2C1_MasterWrite(i2cReadAddr, 2, eepromAddr, stateMsgI2c);
        //e depois l� os quatro bytes sequenciais, onde est�o guardadas as vari�veis
        I2C1_MasterRead(i2cReadBlock, 4, eepromAddr, stateMsgI2c);

        //o �ltimo endere�o de mem�ria � do tipo INT, logo leva 2 bytes, o 1� leva um shift de 8 bits � esquerda e em seguida � somado ao segundo byte
        memAddr = ( ( i2cReadBlock[0] << 8 ) + i2cReadBlock[1] );
        //o nr. de registos � do tipo INT, logo leva 2 bytes, o 1� leva um shift de 8 bits � esquerda e em seguida � somado ao segundo byte
        regNum = ( ( i2cReadBlock[2] << 8 ) + i2cReadBlock[3] );
        //esta vari�vel corresponde � posi��o relativa ao inicio do bloco de mem�ria e est� diretamente relacionada com o �ltimo endere�o de mem�ria
        //uma vez que cada registo tem de tamanho 8 bytes, trata-se de uma divis�o por 8, o que corresponde a um shift de 3 bits para a direita
        regCountAux = memAddr >> 3;

        //espera por um ENTER para prosseguir opera��es
        //limpa o registo de rece��o da EUSART, de modo a este estar limpo para receber um ENTER
        RCREG1 = 0;
        //n�o avan�a at� � introdu��o de um novo ENTER
        while (RCREG1 != '\r');

        //reinicia o nr. de leituras a 0, para n�o reentrar na fun��o numa nova interrup��o
        numLeituras = 0;

    }

}
/**
  End of File
 */
