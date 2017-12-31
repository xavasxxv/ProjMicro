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

//esta função foi alterada, corresponde à equivalente caso as interrupções estivessem desligadas para a EUSART, uma vez que para envio não são usadas interrupções
void EUSART1_Write(uint8_t txData) {
    while (!PIR1bits.TX1IF);
    TXREG1 = txData; // Write the data byte to the USART.
}

void EUSART1_Receive_ISR(void) {

    //em caso de erro, reinicia o módulo
    if (RCSTA1bits.OERR) {
        // EUSART1 error - restart
        RCSTA1bits.CREN = 0;
        RCSTA1bits.CREN = 1;
    }

    //enquanto o caractér não for ENTER ou a string não atingir o tamanho máximo de 40 (mais o ENTER)
    if (j >= 0 && j < StrSIZE - 1 && strUSART[j] != '\r') {

        //bloqueia a escrita para o terminal virtual por outros módulos
        eusart_Tx_On = 1;
        //copia o caractér recebido para a posição correspondente na string EUSART
        strUSART[j] = RCREG1;

        //repete o caractér para o terminal virtual
        //caso esteja no limite de caractéres manda um ENTER
        if (j == 40) {
            while (!PIR1bits.TX1IF);
            TXREG1 = '\r';
            //se não repete o caractér recebido
        } else {
            while (!PIR1bits.TX1IF);
            TXREG1 = strUSART[j];
        }

        //apaga o caractér recebido da string no caso de se ter apagado (BACKSPACE)
        //apaga 1º o BACKSPACE em si da string
        if (strUSART[j] != '\r' && strUSART[j] == 8) {
            strUSART[j] = '\0';
            j--;
            //se não estiver na posição zero da string já, apaga então efetivamente o caractér pretendido
            if (j >= 0) {
                strUSART[j] = '\0';
                j--;
            }
        }
        //acerta a posição de receção
        j++;
    }

    //se a entrada no terminal virtual for do tipo "SA=xx\r",  calcula a nova temp. de alarme
    if (( strUSART[0] == 'S' ) && ( strUSART[1] == 'A' ) && ( strUSART[2] == '=' ) && ( strUSART[5] == '\r' )) {

        //variáveis locais, que guardam o valor "real"/literal do caractér recebido, tal como no caso do teclado matricial
        int i1, i2;

        //subtrai ao caracter '0', ou seja 48 "literal"
        i1 = strUSART[3] - '0';
        i2 = strUSART[4] - '0';

        //se os caracéteres corresponderem a digitos, tenta calcular a nova temp. de alarme
        if (i1 >= 0 && i1 <= 9 && i2 >= 0 && i2 <= 9) {

            //cálculo local e temporário da temp. de alarme, de modo a ser possível testar
            char tmpAlarme = 10 * i1 + i2;

            //se a nova temp. estiver dentro dos limites, então escreve a temp. temporária na variável global do sistema
            if (tmpAlarme >= 10 && tmpAlarme <= 40)
                tempAlarme = tmpAlarme;
        }

    }

    //se a entrada no terminal virtual for do tipo "RH=x\r", "RH=xx\r", "RH=xxx\r" ou "RH=xxxx\r", calcula o nr. de registo que se pretende ler
    if (( strUSART[0] == 'R' ) && ( strUSART[1] == 'H' ) && ( strUSART[2] == '=' ) && ( strUSART[4] == '\r' || strUSART[5] == '\r' || strUSART[6] == '\r' || strUSART[7] == '\r' )) {

        //se a entrada for de qualquer tipo ("RH=x\r", "RH=xx\r", "RH=xxx\r" ou "RH=xxxx\r")
        if (strUSART[4] == '\r' || strUSART[5] == '\r' || strUSART[6] == '\r' || strUSART[7] == '\r') {

            //se os caratéres recebidos são entre '0' e '9', uma vez que são sequencias na tabelas ASCII, pode se fazer este teste
            //o mesmo é feito para os restantes casos, apenas em posições diferentes
            if (strUSART[3] >= '0' && strUSART[3] <= '9')

                //o nr. de leituras é o caractér menos '0', ou 48 "literal"
                numLeituras = strUSART[3] - '0';
        }

        //se a entrada for do tipo "RH=xx\r", "RH=xxx\r" ou "RH=xxxx\r"
        if (strUSART[5] == '\r' || strUSART[6] == '\r' || strUSART[7] == '\r') {

            //mesma verificação que no 1º caso, apenas para a posição seguinte
            if (strUSART[4] >= '0' && strUSART[4] <= '9')

                //o nr. de leituras é igual ao número anterior vezes dez, uma vez que este avançou uma casa, mais o caractér recebido menos '0'
                numLeituras = ( numLeituras * 10 ) + strUSART[4] - '0';
        }

        //se a entrada for do tipo "RH=xxx\r" ou "RH=xxxx\r"
        if (strUSART[6] == '\r' || strUSART[7] == '\r') {

            //mesma verificação que no 2º caso, apenas para a posição seguinte
            if (strUSART[5] >= '0' && strUSART[5] <= '9')

                //o nr. de leituras é igual ao número anterior vezes dez, uma vez que este avançou uma casa, mais o caractér recebido menos '0'
                numLeituras = ( numLeituras * 10 ) + strUSART[5] - '0';
        }

        //se a entrada for do tipo "RH=xxxx\r"
        if (strUSART[7] == '\r') {

            //mesma verificação que no 3º caso, apenas para a posição seguinte
            if (strUSART[6] >= '0' && strUSART[6] <= '9')

                //o nr. de leituras é igual ao número anterior vezes dez, uma vez que este avançou uma casa, mais o caractér recebido menos '0'
                numLeituras = ( numLeituras * 10 ) + strUSART[6] - '0';
        }

        //se o nr. de leituras pretendido for maior que o nr. de registos guardados, então não mostra nenhum, IE. mostra 0
        if (numLeituras > regNum)
            numLeituras = 0;
    }

    //caso se trate do fim da receção (caso ENTER ou fim de string), a última posição da string USART tem de ser null ('\0'), para que seja possível copiar para o LCD
    if (j == StrSIZE - 1 || strUSART[( j - 1 )] == '\r') {
        //último caractér passa a null ('\0')
        strUSART[j - 1] = '\0';
        //o contador local passa a 0, para puder receber outra string
        j = 0;
        //habilita a escrita por outros módulos
        eusart_Tx_On = 0;
    }

    //caso o nr. de leituras seja maior que 0, pretende-se mostrar esse nr. de leituras
    if (numLeituras > 0) {

        //aviso inicial que durante esta visualização, os restantes módulos não funcionam, apenas o alarme, dado o último estado deste
        sprintf(strUSART, "\f!ATENCAO!ATENCAO!ATENCAO!\rAPENAS ALARME A FUNCIONAR DE MOMENTO!\rPOR FAVOR AGUARDE\r");
        for (i = 0; strUSART[i] != '\0'; i++) {
            EUSART1_Write(strUSART[i]);
            strUSART[i] = '\0';
        }

        //novo contador local, que começa com o valor de leituras pretendido e decrementa até 0
        for (int k = numLeituras; k != 0; k--) {

            //o endereço a ler corresponde à posição do registo relativa no bloco de memória
            memAddr = regCountAux * 8;

            //divide o endereço pretendido em 2 bytes
            //para o 1º byte, tem de se fazer um shift register à direita em 8 bits, para se obter apenas os 1ºs 8 bits
            i2cReadAddr[0] = ( memAddr >> 8 );
            //para o 2º byte, é aplicada uma máscara E de 0x00ff, ou seja, bit a bit, uma operação E entre o bit que está na variável e o da máscara
            i2cReadAddr[1] = ( memAddr & 0xff );

            //manda à EEPROM o comando para mudar para a linha pretendida
            I2C1_MasterWrite(i2cReadAddr, 2, eepromAddr, stateMsgI2c);
            //e depois lê os 8 bytes sequenciais, onde estão guardadas as variáveis
            I2C1_MasterRead(i2cReadBlock, 8, eepromAddr, stateMsgI2c);

            //copia para a string da EUSARTa temp. medida, a temp. de alarme e o seu estado, bem como o nr de registo correspondente
            //as variáveis são guardadas como "literias", onde a temp. medida é guardada no 2º byte, a de alarme no 4º e o estado deste no 6º
            sprintf(strUSART, "TM=%2d_TA=%2d_AA=%1d_RN=%04d\r", i2cReadBlock[1], i2cReadBlock[3], i2cReadBlock[5], regNum);
            for (i = 0; strUSART[i] != '\0'; i++) {
                EUSART1_Write(strUSART[i]);
                strUSART[i] = '\0';
            }

            //diminui os contadores
            regCountAux--;
            regNum--;

            //se a posição relativa no registo já estiver na 1º, passa para a última e continua a subtrair
            if (regCountAux == 0)
                regCountAux = 4095;

        }

        //no final, volta a ler o último endereço escrito e o nr. de registo da EEPROM, de modo a continuar a operar
        //faz praticamente a mesma coisa como no inicio de operação, exceto que como já esteve a funcionar
        //assume-se que foi registado pelo menos 1 registo, pelo que não é verificado se a memória está vazia

        //endereço de memória a ler, o 1º, que é onde estão guardados o último registo de memória usado e o nr. de registos guardados até ao momento
        i2cReadAddr[0] = 0;
        i2cReadAddr[1] = 0;

        //manda à EEPROM o comando para mudar para a linha 0x0000
        I2C1_MasterWrite(i2cReadAddr, 2, eepromAddr, stateMsgI2c);
        //e depois lê os quatro bytes sequenciais, onde estão guardadas as variáveis
        I2C1_MasterRead(i2cReadBlock, 4, eepromAddr, stateMsgI2c);

        //o último endereço de memória é do tipo INT, logo leva 2 bytes, o 1º leva um shift de 8 bits à esquerda e em seguida é somado ao segundo byte
        memAddr = ( ( i2cReadBlock[0] << 8 ) + i2cReadBlock[1] );
        //o nr. de registos é do tipo INT, logo leva 2 bytes, o 1º leva um shift de 8 bits à esquerda e em seguida é somado ao segundo byte
        regNum = ( ( i2cReadBlock[2] << 8 ) + i2cReadBlock[3] );
        //esta variável corresponde à posição relativa ao inicio do bloco de memória e está diretamente relacionada com o último endereço de memória
        //uma vez que cada registo tem de tamanho 8 bytes, trata-se de uma divisão por 8, o que corresponde a um shift de 3 bits para a direita
        regCountAux = memAddr >> 3;

        //espera por um ENTER para prosseguir operações
        //limpa o registo de receção da EUSART, de modo a este estar limpo para receber um ENTER
        RCREG1 = 0;
        //não avança até à introdução de um novo ENTER
        while (RCREG1 != '\r');

        //reinicia o nr. de leituras a 0, para não reentrar na função numa nova interrupção
        numLeituras = 0;

    }

}
/**
  End of File
 */
