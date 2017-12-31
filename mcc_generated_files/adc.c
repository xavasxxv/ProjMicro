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

    //recebe o valor bin�rio da leitura do ADC
    binADC = ADC_GetConversionResult();

    //converte o valor bin�rio em temperatura
    tmpTemp = ( binADC - 83 ) * ( adcLsb );

    //converte a temp. de double para char(literal) e arredonda ao n�mero inteiro mais pr�ximo

    //recebe primeiramente a temp. calculada
    double var1 = tmpTemp;
    //recebe tamb�m a temp. calculada, mas faz um cast para char, o que faz com que se "perca" a parte decimal e arredonde sempre por defeito
    char var2 = ( char ) tmpTemp;
    //� vari�vel do tipo double, que cont�m as partes inteiras e decimais, � subtraida a parte inteira, ficando apenas a parte decimal
    var1 -= ( double ) var2;

    //a temp. atual ser� sempre (por base) a temp. arredondada por defeito
    tempAtual = ( char ) tmpTemp;

    //caso se verifique que a parte decimal era maior ou igual a 0.5, soma � temperatura (obtida atrav�s de um arrendondamento po defeito) mais 1
    if (var1 >= 0.5)
        tempAtual++;

    //passa o estado do alarme para 1 caso a temo. medida seja maior � de alarme, caso contr�rio, o alarme est� a 0
    if (tempAtual > tempAlarme)
        alarme = 1;
    else
        alarme = 0;

    //caso a escrita para a EUSART n�o esteja bloqueada por outra fun��o, escreve para o terminal os dados
    if (eusart_Tx_On == 0 && eusart_Tx_En == 0) {
        //copia para a string EUSART os valores das temps. e o estado do alarme, e limpa o ecr� de cada vez que manda a mensagem
        sprintf(strUSART, "\fTM=%2d_TA=%2d_AA=%1d\r", tempAtual, tempAlarme, alarme);
        for (i = 0; strUSART[i] != '\0'; i++) {
            //escreve o caract�r e limpa-o da string, para esta n�o ter lixo l� dentro
            EUSART1_Write(strUSART[i]);
            strUSART[i] = '\0';
        }
    }

    //escrita na EEPROM de um novo registo de dados

    //caso a posi��o relativa do registo seja no fim da EEPROM, esta tem de "dar a volta", IE: voltar � 1� possi��o
    if (regCountAux == 4095)
        regCountAux = 0;
    //enquanto a mem�ria n�o estiver cheia de registos , pode-se aumentar o n�mero de registos
    //nr. de registos M�XIMO
    //256k bits = 32768 bytes
    //32768 bytes - (1�s 8 bytes para guardar as vari�veis iniciais) = 32760
    //32760 / (8bytes por registo) = 4095 registos)
    if (regNum < 4095)
        regNum++;

    //aumenta sempre a posi��o relativa na EEPROM
    regCountAux++;

    //o endere�o inicial para a escrita do registo est� diretamente relacionado com a posi��o relativa do registo
    //como este � de 8 bytes e cada endere�o corresponde a 1 bytes, basta multiplicar por 8
    memAddr = regCountAux * 8;

    //composi��o do bloco de dados de um registo para a fun��o de escrita para a EEPROM
    //os 1�s 2 bytes a enviar correspondem ao endere�o inicial do registo a ser escrito
    //os seguites 8 s�o os dados de registo, mais umas letras para "f�cil" leitura a partir da janela de debug do PROTEUS

    //o 1� byte correspondente ao byte high do endere�o de mem�ria, tem de se fazer um shift register � direita em 8 bits, para se obter apenas os 1�s 8 bits
    i2cWriteBlock[0] = ( memAddr >> 8 ); //byte high do endere�o de mem�ria
    //o 2� byte correspondente ao byte low do endere�o de mem�ria, � aplicada uma m�scara E de 0x00ff, ou seja, bit a bit, uma opera��o E entre o bit que est� na vari�vel e o da m�scara
    i2cWriteBlock[1] = ( memAddr & 0xff ); //byte low do endere�o de mem�ria
    //letra M, correspondente a "medi��o", para diferencia��o e separa��o das vari�veis e mais f�cil leitura a partir da janela de debug da EEPROM do PROTEUS
    i2cWriteBlock[2] = 'M';
    //escrita da temp. medida, uma vez que esta cabe dentro de um byte, n�o � necess�rio fazer mais nada
    i2cWriteBlock[3] = tempAtual;
    //letra A, correspondente a "alarme", para diferencia��o e separa��o das vari�veis e mais f�cil leitura a partir da janela de debug da EEPROM do PROTEUS
    i2cWriteBlock[4] = 'A';
    //escrita da temp. de alarme, uma vez que esta cabe dentro de um byte, n�o � necess�rio fazer mais nada
    i2cWriteBlock[5] = tempAlarme;
    //letra 3, correspondente a "estado", para diferencia��o e separa��o das vari�veis e mais f�cil leitura a partir da janela de debug da EEPROM do PROTEUS
    i2cWriteBlock[6] = 'E';
    //escrita do estado do alarme, uma vez que este cabe dentro de um byte, n�o � necess�rio fazer mais nada
    i2cWriteBlock[7] = alarme;
    //1� byte do nr. de registos gravados at� agora, � aplicada a mesma t�cnica que no byte high do endere�o de mem�ria
    i2cWriteBlock[8] = ( regNum >> 8 ); //byte high do nr de registos
    //2� byte do nr. de registos gravados at� agora, � aplicada a mesma t�cnica que no byte low do endere�o de mem�ria
    i2cWriteBlock[9] = ( regNum & 0xff ); //byte low do nr de registos

    //come�a a transa��o de dados do m�dulo I2C com a EEPROM
    I2C1_MasterWrite(i2cWriteBlock, 10, eepromAddr, stateMsgI2c);

    //caso haja algum percal�o, atraso ou ainda esteja a decorer a escrita de outro bloco de dados, recome�a a transa��o at� ter sucesso, com o mesmo conjunto de dados
    do {

        if (stateMsgI2c != I2C1_MESSAGE_PENDING && stateMsgI2c != I2C1_MESSAGE_COMPLETE)
            I2C1_MasterWrite(i2cWriteBlock, 10, eepromAddr, stateMsgI2c);

    } while (( stateMsgI2c != I2C1_MESSAGE_COMPLETE ) == ( stateMsgI2c != I2C1_MESSAGE_PENDING ));

    //composi��o do bloco de dados para os valores iniciais para a fun��o de escrita para a EEPROM
    //o endere�o de mem�ria a escrever ser� sempre 0x0000, ou seja os primeiros 8
    //apesar de se deixar espa�o para 8 bytes, apenas 4 s�o usados, os restantes n�o s�o usados de modo a facilitar c�lculos

    //1� byte do endere�o, que ser� sempre 0x0000, correspondente ent�o a 0x00
    i2cWriteBlock[0] = 0;
    //2� byte do endere�o, que ser� sempre 0x0000, correspondente ent�o a 0x00
    i2cWriteBlock[1] = 0;
    //o 1� byte correspondente ao byte high do endere�o de mem�ria, � aplicada a mesma t�cnica que na composi��o do bloco para a escrita de um registo
    i2cWriteBlock[2] = ( memAddr >> 8 );
    //o 2� byte correspondente ao byte low do endere�o de mem�ria, � aplicada a mesma t�cnica que na composi��o do bloco para a escrita de um registo
    i2cWriteBlock[3] = ( memAddr & 0xff );
    //1� byte do nr. de registos gravados at� agora, � aplicada a mesma t�cnica que na composi��o do bloco para a escrita de um registo
    i2cWriteBlock[4] = ( regNum >> 8 );
    //2� byte do nr. de registos gravados at� agora, � aplicada a mesma t�cnica que na composi��o do bloco para a escrita de um registo
    i2cWriteBlock[5] = ( regNum & 0xff );

    //come�a uma nova transa��o de dados do m�dulo I2C com a EEPROM
    I2C1_MasterWrite(i2cWriteBlock, 6, eepromAddr, stateMsgI2c);

    //caso haja algum percal�o, atraso ou ainda esteja a decorer a escrita de outro bloco de dados, recome�a a transa��o at� ter sucesso, com o mesmo conjunto de dados
    do {

        if (stateMsgI2c != I2C1_MESSAGE_PENDING && stateMsgI2c != I2C1_MESSAGE_COMPLETE)
            I2C1_MasterWrite(i2cWriteBlock, 6, eepromAddr, stateMsgI2c);

    } while (( stateMsgI2c != I2C1_MESSAGE_COMPLETE ) == ( stateMsgI2c != I2C1_MESSAGE_PENDING ));

    // Clear the ADC interrupt flag
    PIR1bits.ADIF = 0;
}
/**
 End of File
 */