/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs 

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
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

#include "mcc_generated_files/mcc.h"
#include "GlobalDefinitions.h"

/*
                         Main application
 */
void main(void) {
    // Initialize the device
    SYSTEM_Initialize();

    /*
    If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    Use the following macros to:
     */

    // Enable high priority global interrupts
    INTERRUPT_GlobalInterruptHighEnable();

    // Enable low priority global interrupts.
    INTERRUPT_GlobalInterruptLowEnable();

    /*
    // Disable high priority global interrupts
    //INTERRUPT_GlobalInterruptHighDisable();

    // Disable low priority global interrupts.
    //INTERRUPT_GlobalInterruptLowDisable();

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
     */

    //l� o �ltimo registo de mem�ria gravado e o nr de registos gravados
    iniciarValoresEEPROM();

    //inicializa o LCD
    while (BusyXLCD());
    OpenXLCD(FOUR_BIT & LINES_5X7);

    while (BusyXLCD());
    WriteCmdXLCD(DON & CURSOR_OFF & BLINK_OFF);

    while (BusyXLCD());
    WriteCmdXLCD(SHIFT_CUR_RIGHT);

    while (1) {

        //corre o menu, funciona tudo a partir daqui
        menu();

    }
}

//fun��o que permite escrever para o LCD, dado a linha/posi��o e o texto
void escreveLinhaLCD(char linha, char texto[StrSIZE]) {

    while (BusyXLCD());
    WriteCmdXLCD(linha);
    while (BusyXLCD());
    putsXLCD(texto);

}

//fun��o do menu, que apenas atualiza ap�s uma mudan�a no texto
void menu(void) {

    escreveLinhaLCD(LINE1, "Menu         Opcao:");

    while (1) {

        //obt�m a posi��o da fun��o que se pretende usar atrav�s dos bot�es
        if (BOTAO_UP_GetValue() == 0) {
            menu_tmp++;
            while (BOTAO_UP_GetValue() == 0);
        }

        if (BOTAO_DOWN_GetValue() == 0) {
            menu_tmp--;
            while (BOTAO_DOWN_GetValue() == 0);
        }

        if (menu_tmp > 4)
            menu_tmp = 1;
        if (menu_tmp < 1)
            menu_tmp = 4;

        if (lastMenu != menu_tmp) {
            update = 1;
            lastMenu = menu_tmp;
        }

        //mostra o novo texto no LCD dado que foi alterado
        if (update) {
            sprintf(strLCD, "%d", menu_tmp);
            escreveLinhaLCD(LINE1 + 19, strLCD);
            switch (menu_tmp) {
                case 1:
                    escreveLinhaLCD(LINE2, "Monitorizacao        ");
                    break;
                case 2:
                    escreveLinhaLCD(LINE2, "Temp. de alarme      ");
                    break;
                case 3:
                    escreveLinhaLCD(LINE2, "Teste do teclado     ");
                    break;
                case 4:
                    escreveLinhaLCD(LINE2, "Feedback EUSART      ");
                    break;
            }
            update = 0;
        }

        //chama a fun��o pretendida
        if (BOTAO_SEL_GetValue() == 0) {
            while (BOTAO_SEL_GetValue() == 0);
            switch (menu_tmp) {
                case 1:
                    monitorizacao();
                    break;
                case 2:
                    defTempAlarme();
                    break;
                case 3:
                    testeTeclado();
                    break;
                case 4:
                    feedbackUSART();
                    break;
            }
            update = 1;
            escreveLinhaLCD(LINE1, "Menu         Opcao:");
        }

    }
}

//fun��o que mostra as temps. medidas e de alarme e alterna a informa��o se o alarme estiver ativo
void monitorizacao(void) {

    //escreve para o LCD as strings iniciais
    sprintf(strLCD, "Temp. Atual:  %2d oC ", tempAtual);
    escreveLinhaLCD(LINE1, strLCD);
    sprintf(strLCD, "Temp. Alarme: %2d oC ", tempAlarme);
    escreveLinhaLCD(LINE2, strLCD);

    //enquanto n�o for pressionado o bot�o SEL, fica na fun��o
    do {
        //atualiza o LCD a 4Hz (4 vezes por s)
        while (clk4Hz) {

            //se o alarme estiver ativo, alternar entre mostrar a temp. de alarme e o aviso de alarme ativo
            if (alarme) {
                switch (clk1_2Hz) {
                    case 1:
                        sprintf(strLCD, "!ALARME! TEMP>");
                        update = 1;
                        break;
                    case 0:
                        sprintf(strLCD, "Temp. Alarme: ");
                        break;
                }
                escreveLinhaLCD(LINE2, strLCD);
                //se o �ltimo texto no LCD for o de aviso, mostrar a temp. de alarme
            } else if (update) {
                sprintf(strLCD, "Temp. Alarme: ");
                escreveLinhaLCD(LINE2, strLCD);
                update = 0;
            }

            //atualiza os valores das temps. de alarme e medida
            sprintf(strLCD, "%2d", tempAtual);
            escreveLinhaLCD(LINE1 + 14, strLCD);
            sprintf(strLCD, "%2d", tempAlarme);
            escreveLinhaLCD(LINE2 + 14, strLCD);

            while (clk4Hz);
        }
    } while (BOTAO_SEL_GetValue() != 0);

    while (BOTAO_SEL_GetValue() == 0);

}

//fun��o de teste para a introdu��o de caract�res atrav�s do teclado matricial
void testeTeclado(void) {

    //vari�vel de teste que guarda o caract�r obtido do teclado
    char input;

    //escreve as strings iniciais
    escreveLinhaLCD(LINE1, "Teste Teclado       ");
    escreveLinhaLCD(LINE2, "Numero:             ");

    //enquanto n�o for pressionado o bot�o SEL, fica na fun��o
    while (BOTAO_SEL_GetValue() != 0) {
        
        //chama a fun��o para obten��o do caract�r
        input = getKBInput();
        
        //e mostra-o
        sprintf(strLCD, "%c", input);
        escreveLinhaLCD(LINE2 + 8, strLCD);
    }

    while (BOTAO_SEL_GetValue() == 0);

}

//fun��o que permite definir nova temp. de alarme atrav�s do teclado matricial
void defTempAlarme(void) {

    //vari�veis locais
    char c1, c2; //caract�res recebidos do teclado
    char tmp_Alarme = 0; //temp. tempor�ria para testes

    //c�digo comentado que informa que a temp. de alarme tem de estar entre 10 e 40 graus
    /**
    while (BusyXLCD());
    WriteCmdXLCD(CLEAR_LCD);
    escreveLinhaLCD(LINE1, "A temp. deve ser entre os 10 e os 40 oC");
    escreveLinhaLCD(LINE2, "                   Press SEL           ");
    __delay_ms(1500);
    for (int i = 0; i < 19; i++) {

        WriteCmdXLCD(SHIFT_DISP_LEFT);
        while (BusyXLCD());
            __delay_ms(150);
    }
    while (BOTAO_SEL != 0);
    while (BOTAO_SEL == 0);
     */

    //enquanto a temp. n�o for entre 10 e 40 gruas, n�o atualiza a temp. de alarme
    do {

        //escreve as strings iniciais
        escreveLinhaLCD(LINE1, "Introduza o alarme  ");
        escreveLinhaLCD(LINE2, "Temp:    oC         ");

        //chama a fun��o para obten��o do caract�r
        c1 = getKBInput();
        //mostra-o no LCD
        sprintf(strLCD, "%c", c1);
        escreveLinhaLCD(LINE2 + 6, strLCD);
        //e mesma coisa para o segundo caract�r
        c2 = getKBInput();
        sprintf(strLCD, "%c", c2);
        escreveLinhaLCD(LINE2 + 7, strLCD);

        //se os caract�res recebidos n�o forem digitos, a temp. tempor�ria ser� 0
        if (c1 == '*' || c1 == '#' || c2 == '*' || c2 == '#' || c1 == NULL || c2 == NULL)
            tmp_Alarme = 0;

        //ao caract�r recebido em ASCII, subtrai o "excesso", neste caso '0', que correponde a 48
        c1 = c1 - '0';
        c2 = c2 - '0';
        //a nova temp, ser� 10 vezes o 1� caract�r recebido mais o 2�
        tmp_Alarme = 10 * c1 + c2;

    } while (tmp_Alarme < 10 || tmp_Alarme > 40);

    //se a nova temp. for dentro dos limites, escreve a temp. tempor�ria na vari�vel global do sistema
    tempAlarme = tmp_Alarme;

    //pequeno delay, uma vez que se a temp. estiver dentro dos limites, vai sair imediatamente da fun��o, assim mostra a nova temp. durante 0.5 seg
    __delay_ms(500);

}

//fun��o que trata da aquisi��o do carat�r do teclado matricial e conjun��o com as interrup��es externas
char getKBInput(void) {

    //come�a por definir RB3 a HIGH, de modo a o restante c�digo poder alternar os estados dos pinos
    RB3_SetHigh();

    //enquanto a flag WrEnFl estiver a zero ou o bot�o SEL for premido, altera o estado dos pinos
    do {

        //se o pino anterior estiver a HIGH, passa esse a LOW e passa o correspondente a HIGH, enquanto a flah WrEnFl estiver a 0
        if (RB3_LAT == 1 && WrEnFl == 0) {
            RB3_SetLow();
            RB4_SetHigh();
        }
        if (RB4_LAT == 1 && WrEnFl == 0) {
            RB4_SetLow();
            RB5_SetHigh();
        }
        if (RB5_LAT == 1 && WrEnFl == 0) {
            RB5_SetLow();
            RB6_SetHigh();
        }
        if (RB6_LAT == 1 && WrEnFl == 0) {
            RB6_SetLow();
            RB3_SetHigh();
        }

    } while (WrEnFl == 0 && BOTAO_SEL_GetValue() != 0);
    //as interrup��es externas levam a flag WrEnFl a 1, e esta tem de ser limpo por SW
    WrEnFl = 0;

    //esquanto os bot�es do teclado estiverem premidos, n�o avan�a
    while (RB0_GetValue() == 1 || RB1_GetValue() == 1 || RB2_GetValue() == 1);

    //se foi premido o bot�o SEL, devolve NULL
    if (BOTAO_SEL_GetValue() == 0) {
        return NULL;
    }

    //devlove a tecla pressionada, que foi atribu�da nas interrup��es externas
    return TeclaTMP;

}

//fun��o de teste da EUSART, obt�m uma string atrav�s da EUSART e repete-a para o terminal virtual e para o LCD
void feedbackUSART(void) {

    //limpa o LCD
    while (BusyXLCD());
    WriteCmdXLCD(CLEAR_LCD);

    //enquanto o bot�o SEL n�o for pressionado, fica na fun��o
    //NOTA: devido � maneira como � executado, apenas depois de receber um ENTER e o bot�o estiver pressionado � que s�i da fun��o
    while (BOTAO_SEL_GetValue() != 0) {

        //flags de conflito entre a EUSART, uma vez que sempre que � efetuada uma medi��o,
        //pretende-se escrever para o terminal virtual, estas flags desabilitam esta funcionalidade, de modo a n�o haver conflitos
        eusart_Tx_On = 1;
        eusart_Tx_En = 1;
        //espera pela introdu��o da string, que � tradada na interrup��o da EUSART_Receive
        while (eusart_Tx_On == 1);

        //copia a string USART para string do LCD
        for (i = 0; i < StrSIZE - 1; i++) {
            strLCD[i] = strUSART[i];
        }

        //repete a string toda para o terminal virtual
        //ao mesmo tempo que limpa o caract�r que escreveu por �ltimo, para evitar que tenha lixo dentro desta
        for (i = 0; strUSART[i] != '\0'; i++) {
            EUSART1_Write(strUSART[i]);
            strUSART[i] = '\0';
        }

        //no final de escrever a string toda, manda um CARRIAGE RETURN, para mudar de linha no terminal
        EUSART1_Write('\r');

        //escreve a string LCD no LCD
        while (BusyXLCD());
        WriteCmdXLCD(CLEAR_LCD);
        escreveLinhaLCD(LINE1, strLCD);

    }

    while (BOTAO_SEL_GetValue() == 0);

    //habilita a escrita para o terminal por outros m�dulos
    eusart_Tx_On = 0;
    eusart_Tx_En = 0;

}

//fun��o que l� o �ltimo registo de mem�ria usado para gravar dados na EEPROM, bem como o nr de registos nela guardado
void iniciarValoresEEPROM(void) {

    //endere�o de mem�ria a ler, o 1�, que � onde est�o guardados o �ltimo registo de mem�ria usado e o nr. de registos guardados at� ao momento
    i2cReadAddr[0] = 0;
    i2cReadAddr[1] = 0;

    //manda � EEPROM o comando para mudar para a linha 0x0000
    I2C1_MasterWrite(i2cReadAddr, 2, eepromAddr, stateMsgI2c);
    //e depois l� os quatro bytes sequenciais, onde est�o guardadas as vari�veis
    I2C1_MasterRead(i2cReadBlock, 4, eepromAddr, stateMsgI2c);

    //caso a mem�ria esteja vazia, inicializa as vari�veis a 0
    if (( i2cReadBlock[0] == 0xff && i2cReadBlock[1] == 0xff && i2cReadBlock[2] == 0xff && i2cReadBlock[3] == 0xff ) || ( i2cReadAddr[0] != 0 || i2cReadAddr[1] != 0 )) {
        regNum = 0;
        regCountAux = 0;
        memAddr = 0;
        //caso contr�rio, passa os valores para as vari�veis
    } else {
        //o �ltimo endere�o de mem�ria � do tipo INT, logo leva 2 bytes, o 1� leva um shift de 8 bits � esquerda e em seguida � somado ao segundo byte
        memAddr = ( ( i2cReadBlock[0] << 8 ) + i2cReadBlock[1] );
        //o nr. de registos � do tipo INT, logo leva 2 bytes, o 1� leva um shift de 8 bits � esquerda e em seguida � somado ao segundo byte
        regNum = ( ( i2cReadBlock[2] << 8 ) + i2cReadBlock[3] );
        //esta vari�vel corresponde � posi��o relativa ao inicio do bloco de mem�ria e est� diretamente relacionada com o �ltimo endere�o de mem�ria
        //uma vez que cada registo tem de tamanho 8 bytes, trata-se de uma divis�o por 8, o que corresponde a um shift de 3 bits para a direita
        regCountAux = memAddr >> 3;
    }

}

/**
 End of File
 */