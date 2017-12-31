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

    //converte o valor binário em temperatura
    tmpTemp = ( binADC - 83 ) * ( adcLsb );

    //converte a temp. de double para char(literal) e arredonda ao número inteiro mais próximo

    //recebe primeiramente a temp. calculada
    double var1 = tmpTemp;
    //recebe também a temp. calculada, mas faz um cast para char, o que faz com que se "perca" a parte decimal e arredonde sempre por defeito
    char var2 = ( char ) tmpTemp;
    //à variável do tipo double, que contém as partes inteiras e decimais, é subtraida a parte inteira, ficando apenas a parte decimal
    var1 -= ( double ) var2;

    //a temp. atual será sempre (por base) a temp. arredondada por defeito
    tempAtual = ( char ) tmpTemp;

    //caso se verifique que a parte decimal era maior ou igual a 0.5, soma à temperatura (obtida através de um arrendondamento po defeito) mais 1
    if (var1 >= 0.5)
        tempAtual++;

    //passa o estado do alarme para 1 caso a temo. medida seja maior à de alarme, caso contrário, o alarme está a 0
    if (tempAtual > tempAlarme)
        alarme = 1;
    else
        alarme = 0;

    //caso a escrita para a EUSART não esteja bloqueada por outra função, escreve para o terminal os dados
    if (eusart_Tx_On == 0 && eusart_Tx_En == 0) {
        //copia para a string EUSART os valores das temps. e o estado do alarme, e limpa o ecrã de cada vez que manda a mensagem
        sprintf(strUSART, "\fTM=%2d_TA=%2d_AA=%1d\r", tempAtual, tempAlarme, alarme);
        for (i = 0; strUSART[i] != '\0'; i++) {
            //escreve o caractér e limpa-o da string, para esta não ter lixo lá dentro
            EUSART1_Write(strUSART[i]);
            strUSART[i] = '\0';
        }
    }

    //escrita na EEPROM de um novo registo de dados

    //caso a posição relativa do registo seja no fim da EEPROM, esta tem de "dar a volta", IE: voltar à 1º possição
    if (regCountAux == 4095)
        regCountAux = 0;
    //enquanto a memória não estiver cheia de registos , pode-se aumentar o número de registos
    //nr. de registos MÁXIMO
    //256k bits = 32768 bytes
    //32768 bytes - (1ºs 8 bytes para guardar as variáveis iniciais) = 32760
    //32760 / (8bytes por registo) = 4095 registos)
    if (regNum < 4095)
        regNum++;

    //aumenta sempre a posição relativa na EEPROM
    regCountAux++;

    //o endereço inicial para a escrita do registo está diretamente relacionado com a posição relativa do registo
    //como este é de 8 bytes e cada endereço corresponde a 1 bytes, basta multiplicar por 8
    memAddr = regCountAux * 8;

    //composição do bloco de dados de um registo para a função de escrita para a EEPROM
    //os 1ºs 2 bytes a enviar correspondem ao endereço inicial do registo a ser escrito
    //os seguites 8 são os dados de registo, mais umas letras para "fácil" leitura a partir da janela de debug do PROTEUS

    //o 1º byte correspondente ao byte high do endereço de memória, tem de se fazer um shift register à direita em 8 bits, para se obter apenas os 1ºs 8 bits
    i2cWriteBlock[0] = ( memAddr >> 8 ); //byte high do endereço de memória
    //o 2º byte correspondente ao byte low do endereço de memória, é aplicada uma máscara E de 0x00ff, ou seja, bit a bit, uma operação E entre o bit que está na variável e o da máscara
    i2cWriteBlock[1] = ( memAddr & 0xff ); //byte low do endereço de memória
    //letra M, correspondente a "medição", para diferenciação e separação das variáveis e mais fácil leitura a partir da janela de debug da EEPROM do PROTEUS
    i2cWriteBlock[2] = 'M';
    //escrita da temp. medida, uma vez que esta cabe dentro de um byte, não é necessário fazer mais nada
    i2cWriteBlock[3] = tempAtual;
    //letra A, correspondente a "alarme", para diferenciação e separação das variáveis e mais fácil leitura a partir da janela de debug da EEPROM do PROTEUS
    i2cWriteBlock[4] = 'A';
    //escrita da temp. de alarme, uma vez que esta cabe dentro de um byte, não é necessário fazer mais nada
    i2cWriteBlock[5] = tempAlarme;
    //letra 3, correspondente a "estado", para diferenciação e separação das variáveis e mais fácil leitura a partir da janela de debug da EEPROM do PROTEUS
    i2cWriteBlock[6] = 'E';
    //escrita do estado do alarme, uma vez que este cabe dentro de um byte, não é necessário fazer mais nada
    i2cWriteBlock[7] = alarme;
    //1º byte do nr. de registos gravados até agora, é aplicada a mesma técnica que no byte high do endereço de memória
    i2cWriteBlock[8] = ( regNum >> 8 ); //byte high do nr de registos
    //2º byte do nr. de registos gravados até agora, é aplicada a mesma técnica que no byte low do endereço de memória
    i2cWriteBlock[9] = ( regNum & 0xff ); //byte low do nr de registos

    //começa a transação de dados do módulo I2C com a EEPROM
    I2C1_MasterWrite(i2cWriteBlock, 10, eepromAddr, stateMsgI2c);

    //caso haja algum percalço, atraso ou ainda esteja a decorer a escrita de outro bloco de dados, recomeça a transação até ter sucesso, com o mesmo conjunto de dados
    do {

        if (stateMsgI2c != I2C1_MESSAGE_PENDING && stateMsgI2c != I2C1_MESSAGE_COMPLETE)
            I2C1_MasterWrite(i2cWriteBlock, 10, eepromAddr, stateMsgI2c);

    } while (( stateMsgI2c != I2C1_MESSAGE_COMPLETE ) == ( stateMsgI2c != I2C1_MESSAGE_PENDING ));

    //composição do bloco de dados para os valores iniciais para a função de escrita para a EEPROM
    //o endereço de memória a escrever será sempre 0x0000, ou seja os primeiros 8
    //apesar de se deixar espaço para 8 bytes, apenas 4 são usados, os restantes não são usados de modo a facilitar cálculos

    //1º byte do endereço, que será sempre 0x0000, correspondente então a 0x00
    i2cWriteBlock[0] = 0;
    //2º byte do endereço, que será sempre 0x0000, correspondente então a 0x00
    i2cWriteBlock[1] = 0;
    //o 1º byte correspondente ao byte high do endereço de memória, é aplicada a mesma técnica que na composição do bloco para a escrita de um registo
    i2cWriteBlock[2] = ( memAddr >> 8 );
    //o 2º byte correspondente ao byte low do endereço de memória, é aplicada a mesma técnica que na composição do bloco para a escrita de um registo
    i2cWriteBlock[3] = ( memAddr & 0xff );
    //1º byte do nr. de registos gravados até agora, é aplicada a mesma técnica que na composição do bloco para a escrita de um registo
    i2cWriteBlock[4] = ( regNum >> 8 );
    //2º byte do nr. de registos gravados até agora, é aplicada a mesma técnica que na composição do bloco para a escrita de um registo
    i2cWriteBlock[5] = ( regNum & 0xff );

    //começa uma nova transação de dados do módulo I2C com a EEPROM
    I2C1_MasterWrite(i2cWriteBlock, 6, eepromAddr, stateMsgI2c);

    //caso haja algum percalço, atraso ou ainda esteja a decorer a escrita de outro bloco de dados, recomeça a transação até ter sucesso, com o mesmo conjunto de dados
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