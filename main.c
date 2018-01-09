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

    //lê o último registo de memória gravado e o nr. de registos gravados
    iniciarValoresEEPROM();

    //inicializa o LCD
    while (BusyXLCD());
    OpenXLCD(FOUR_BIT & LINES_5X7);

    while (BusyXLCD());
    WriteCmdXLCD(DON & CURSOR_OFF & BLINK_OFF);

    while (BusyXLCD());
    WriteCmdXLCD(SHIFT_CUR_RIGHT);

    //loop principal
    while (1) {

        //funcionalidades auxiliares às interrupções que "tratam" de tarefas que demoram demasiado tempo para as interrupções
        extrasInterrupoes();

        //obtém a posição da função que se pretende usar através dos botões
        if (BOTAO_UP_GetValue() == 0) {
            menu_tmp++;
            while (BOTAO_UP_GetValue() == 0);
        }

        if (BOTAO_DOWN_GetValue() == 0) {
            menu_tmp--;
            while (BOTAO_DOWN_GetValue() == 0);
        }

        //caso excessa o nr. de opções, volta ao extremo oposto, no caso do menu, entre 1 e 4, noutras funções entre 1 e 5
        if (menu_sel == 5) {
            if (menu_tmp > 4)
                menu_tmp = 1;
            if (menu_tmp < 1)
                menu_tmp = 4;
        } else {
            if (menu_tmp > 5)
                menu_tmp = 1;
            if (menu_tmp < 1)
                menu_tmp = 5;
        }

        //caso o última opção seja diferente da atual
        if (lastMenu != menu_tmp) {
            //diz para atualizar o LCD
            update = 1;
            //iguala a última opção à atual
            lastMenu = menu_tmp;
            //escreve o nr. da opção no canto superior direito do LCD
            escreveCharLCD(LINE1 + 19, menu_tmp + 48);
        }

        //no caso de ser preciso atualizar o LCD e o menu selecionado seja o 5, ou o próprio menu
        if (update && menu_sel == 5) {
            //escreve para 1º linha o texto do menu
            escreveLinhaLCD(LINE1, "Menu         Opcao:");
            //e a opção selecionada temporáriamente
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

        //no caso de querer entrar numa função
        if (BOTAO_SEL_GetValue() == 0) {
            //desbloqueio manual do estado da EUSART, no caso da última opção selecionada ter sido a 4
            if (menu_sel == 4) {
                estadoEUSART = 0;
                bloqueiaEUSART = 0;
            }
            //a opção temporária passa a ser a selecionada
            menu_sel = menu_tmp;
            //diz às funções para correr os estados iniciais de variáveis e LCD
            startFunc = 1;
        }

        //vai à função selecionada
        switch (menu_sel) {
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
            case 5:
                //no caso da função ser o menu, diz para atualizar o LCD, de modo a escrever por cima das últimas frases
                if (menu_tmp == 5)
                    update = 1;
                break;
        }

    }
}

//função que permite escrever para o LCD, dado a linha/posição e o texto
void escreveLinhaLCD(char linha, char texto[StrSIZE]) {

    while (BusyXLCD());
    WriteCmdXLCD(linha);
    while (BusyXLCD());
    putsXLCD(texto);

}

//função que permite escrever um caractér para o LCD, dado a linha/posição e o caractér
void escreveCharLCD(char linha, char caracter) {

    while (BusyXLCD());
    WriteCmdXLCD(linha);
    while (BusyXLCD());
    putcXLCD(caracter);

}

//função que mostra as temps. medidas e de alarme e alterna a informação se o alarme estiver ativo
void monitorizacao(void) {

    //caso seja a 1º vez a entrar na função, inicia o LCD e/ou as variáveis necessárias, e leva o estado a 0
    if (startFunc) {
        //escreve para o LCD as strings iniciais
        sprintf(strLCD, "Temp. Atual:  %2d oC", tempAtual);
        escreveLinhaLCD(LINE1, strLCD);
        sprintf(strLCD, "Temp. Alarme: %2d oC", tempAlarme);
        escreveLinhaLCD(LINE2, strLCD);
        startFunc = 0;

    }

    //atualiza o LCD a 4Hz (4 vezes por s) ou no caso de ter sido mandado atualizar
    if (update1hZ || update) {
        //se o alarme estiver ativo, alternar entre mostrar a temp. de alarme e o aviso de alarme ativo
        if (alarme) {
            switch (clk1_2Hz) {
                case 1:
                    sprintf(strLCD, "!ALARME! TEMP>");
                    break;
                case 0:
                    sprintf(strLCD, "Temp. Alarme: ");
                    break;
            }
            escreveLinhaLCD(LINE2, strLCD);
            //se o último estado do alarme tiver sido 1, mostrar a temp. de alarme
        } else if (lastAlarme) {
            sprintf(strLCD, "Temp. Alarme: ");
            escreveLinhaLCD(LINE2, strLCD);
        }

        //atualiza os valores das temps. de alarme e medida
        sprintf(strLCD, "%2d", tempAtual);
        escreveLinhaLCD(LINE1 + 14, strLCD);
        sprintf(strLCD, "%2d", tempAlarme);
        escreveLinhaLCD(LINE2 + 14, strLCD);


    }

    //limpa as variáveis de atualização
    update1hZ = 0;
    update = 0;

}

//função de teste para a introdução de caractéres através do teclado matricial
void testeTeclado(void) {

    //caso seja a 1º vez a entrar na função, inicia o LCD e/ou as variáveis necessárias, e leva o estado a 0
    if (startFunc) {
        escreveLinhaLCD(LINE1, "Teste Teclado      ");
        escreveLinhaLCD(LINE2, "Numero:             ");
        startFunc = 0;
    }


    //caso seja habilitada a escrita de um caractér, escreve-o para o LCD e volta a limpar a variável
    if (WrEnFl) {
        escreveCharLCD(LINE2 + 8, TeclaTMP);
        WrEnFl = 0;
    }

}

//função que permite definir nova temp. de alarme através do teclado matricial
void defTempAlarme(void) {

    //variável local de temperatura temporária para testes
    char tmp_Alarme;

    //caso seja a 1º vez a entrar na função, inicia o LCD e/ou as variáveis necessárias, e leva o estado a 0
    if (startFunc) {
        //assere o estado a 0
        estado = 0;
        //escreve as strings iniciais
        escreveLinhaLCD(LINE1, "Introduza o alarme ");
        escreveLinhaLCD(LINE2, "Temp:    oC         ");
        startFunc = 0;
        //variáveis globais de caractéres recebidos, assere valores iniciais
        c1 = 0;
        c2 = 0;
    }

    //caso 1º estado (estado 0)
    if (estado == 0) {
        //e seja habilitada a escrita de um caractér
        if (WrEnFl) {
            //copia-o
            c1 = TeclaTMP;
            if (c1 != 0) {
                //e mostra-o no LCD
                escreveCharLCD(LINE2 + 6, c1);
                //o estado passa a 1, para receber o próximo caractér
                estado = 1;
                //limpa a flag de escrita
                WrEnFl = 0;
            }
        }
    }

    //caso 2º estado (estado 1)
    if (estado == 1) {
        //e seja habilitada a escrita de um caractér
        if (WrEnFl) {
            //copia-o
            c2 = TeclaTMP;
            if (c2 != 0) {
                //e mostra-o no LCD
                escreveCharLCD(LINE2 + 7, c2);
                //o estado passa a 2, para calcular a temperatura introduzida
                estado = 2;
                //limpa a flag de escrita
                WrEnFl = 0;
            }
        }
    }

    //caso 3º estado (estado 2)
    if (estado == 2) {
        //se os caractéres recebidos não forem digitos, a temp. temporária será 0
        if (c1 == '*' || c1 == '#' || c2 == '*' || c2 == '#' || c1 == 0 || c2 == 0)
            tmp_Alarme = 0;
        else {
            //enquanto a temp. não for entre 10 e 40 gruas, não atualiza a temp. de alarme
            //ao caractér recebido em ASCII, subtrai o "excesso", neste caso '0', que correponde a 48
            c1 = c1 - '0';
            c2 = c2 - '0';
            //a nova temp, será 10 vezes o 1º caractér recebido mais o 2º
            tmp_Alarme = 10 * c1 + c2;

            //se a nova temp. for dentro dos limites, escreve a temp. temporária na variável global do sistema, e "manda" de volta para o menu principal
            if (tmp_Alarme >= 10 && tmp_Alarme <= 40) {
                tempAlarme = tmp_Alarme;
                menu_sel = 5;
            }
        }
        //como vai para o menu principal, reinicia a variável de estado para não causar problemas noutras funções, e as variáveis de atualização geral e do LCD
        startFunc = 1;
        estado = 0;
        update = 1;
    }
}

//função de teste da EUSART, obtém uma string através da EUSART e repete-a para o terminal virtual e para o LCD
void feedbackUSART(void) {

    //caso seja a 1º vez a entrar na função, inicia o LCD e/ou as variáveis necessárias, e leva o estado a 0
    if (startFunc) {
        escreveLinhaLCD(LINE1, "Feedback EUSART    ");
        escreveLinhaLCD(LINE2, "                    ");
        EUSART1_Write('\f');
        startFunc = 0;
        //variável de estados a 0
        estado = 0;
        //flags de conflito entre a EUSART, uma vez que sempre que é efetuada uma medição,
        //pretende-se escrever para o terminal virtual, estas flags desabilitam esta funcionalidade, de modo a não haver conflitos
        estadoEUSART = 2;
        bloqueiaEUSART = 2;
    }

    //caso 1º estado (estado 0)
    if (estado == 0) {
        //e o estado da EUSART seja 1 (pausa)
        if (estadoEUSART == 1)
            estado = 1;
        else
            //caso contrário, continua no mesmo estado
            estado = 0;
    }

    //caso 2º estado (estado 1)
    if (estado == 1) {

        //como só escreve a string depois de ter terminado de a receber, e tem de estar pronto para receber outra, reinicia os estado da EUSART e geral
        estadoEUSART = 2;
        estado = 0;

        //copia a string da EUSART para string do LCD
        for (i = 0; i < StrSIZE - 1; i++) {
            if (strUSART[i] == '\0')
                strLCD[i] = ' ';
            else
                strLCD[i] = strUSART[i];
        }

        //repete a string toda para o terminal virtual
        //ao mesmo tempo que limpa o caractér que escreveu por último, para evitar que tenha lixo dentro desta
        for (i = 0; strUSART[i] != '\0'; i++) {
            EUSART1_Write(strUSART[i]);
            strUSART[i] = '\0';
        }

        //escreve a string LCD no LCD
        escreveLinhaLCD(LINE2, strLCD);

    }
}

//função auxiliar às interrupções, com código extenso ou que demoraria demasiado tempo para uma interrupção
void extrasInterrupoes(void) {

    if (intTMR0) {

        //calcula a temperatura atual
        //faz um cast de um char com sinal do valor binário deslocado vezes o equivalente em graus de 1 LSB e adiciona 0.5
        //de modo a arrendondar no caso de ser por exemplo, 26.6ºC, passa a 27.1 e o cast trunca a parte decimal excedente
        tempAtual = ( signed char ) ( ( binADC * adcLsb ) + 0.5 );

        //antes de verificar o novo estado do alarme, guarda o último estado
        lastAlarme = alarme;

        //passa o estado do alarme para 1 caso a temp. medida seja maior à de alarme, caso contrário, o alarme está a 0
        if (tempAtual > tempAlarme)
            alarme = 1;
        else
            alarme = 0;

        //divisores de relógio, que permitem que tenha várias "frequências" com base no tempo de interrupção definido (8 vezes por segundo)
        //divisor para 4Hz
        clk4Hz = !clk4Hz;
        //caso o relógio de 4Hz esteja a 1, manda iniciar uma conversão do ADC, e divide também para os 2Hz, assere a variável auxiliar de update do LCD a 1
        if (clk4Hz)
            clk2Hz = !clk2Hz;
        //divisor para 1Hz
        if (clk2Hz && clk4Hz)
            clk1Hz = !clk1Hz;
        //divisor para 1/2Hz (2 seg)
        if (clk1Hz && clk2Hz && clk4Hz) {
            clk1_2Hz = !clk1_2Hz;
            ADC_StartConversion();
            update1hZ = 1;
        }
        //divisor para 1/4Hz (4 seg)
        if (clk1_2Hz && clk1Hz && clk2Hz && clk4Hz)
            clk1_4Hz = !clk1_4Hz;
        //divisor para 1/8Hz (8 seg)
        if (clk1_4Hz && clk1_2Hz && clk1Hz && clk2Hz && clk4Hz)
            clk1_8Hz = !clk1_8Hz;

        //caso o alarme esteja a 1
        if (alarme == 1) {

            //inicia o Timer2, do qual é dependente o módulo ECCP1, responsável pelo PWM do buzzer
            TMR2_StartTimer();

            //alterna o LED e o buzzer a cada ciclo relógio de 2hz, o buzzer funciona sempre a 12.5% de duty-cycle
            switch (clk2Hz) {
                    //no ciclo negativo, o LED desliga e o buzzer apita a uma freq. mais baixa (375Hz)
                case 0:
                    EPWM1_LoadDutyValue(124); //1/8 do PR2
                    TMR2_LoadPeriodRegister(249); //xF9 b11111001
                    RB7_SetLow();
                    break;
                    //no ciclo positivo, o LED liga e o buzzer apita a uma freq. mais alta (625Hz)
                case 1:
                    EPWM1_LoadDutyValue(74); //1/8 do PR2
                    TMR2_LoadPeriodRegister(149); //x95 b10010101
                    RB7_SetHigh();
                    break;
            }
            //caso o alarme esteja a 0
        } else {

            //inicia o Timer2, do qual é dependente o módulo ECCP1, responsável pelo PWM do buzzer
            TMR2_StopTimer();
            //e desliga forçosamente o LED
            RB7_SetLow();
        }

        //executou o que precisou, flag a 0 para não executar novamente até nova interrupção
        intTMR0 = 0;
    }

    //caso a interrupção da EUSART precise de executar código auxiliar, no caso de entradas específicas
    if (intEUSART) {

        //enquanto o caractér não for ENTER ou a string não atingir o tamanho máximo de 40 (mais o ENTER)
        if (j >= 0 && j < StrSIZE - 1) {

            //indica que está no meio da receção de uma string
            estadoEUSART = 1;

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

        //caso se trate do fim da receção (caso ENTER ou fim de string)
        if (j == StrSIZE - 1 || strUSART[( j - 1 )] == '\r') {
            //o contador passa a 0, para puder receber outra string
            j = 0;
            //no caso de ter estado bloqueada anteriormente, desbloqueia a escrita da EUSART
            if (bloqueiaEUSART == 1)
                bloqueiaEUSART = 0;
            //indica que terminou de recebar a string
            estadoEUSART = 0;
        }

        //no caso de haver algum tipo de problema, recebe o caractér recebido, de modo a puder ser limpa a flag de interrupção da EUSART
        lixoEUSART = RCREG1;

        //a interrupção executou, falta o código auxiliar assim que possível
        //se a entrada no terminal virtual for do tipo "SA=xx\r",  calcula a nova temp. de alarme
        if (( strUSART[0] == 'S' ) && ( strUSART[1] == 'A' ) && ( strUSART[2] == '=' ) && ( strUSART[5] == '\r' ) && bloqueiaEUSART != 2) {

            //variáveis locais, que guardam o valor "real"/literal do caractér recebido, tal como no caso do teclado matricial
            unsigned char i1, i2;

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
        if (( strUSART[0] == 'R' ) && ( strUSART[1] == 'H' ) && ( strUSART[2] == '=' ) && ( strUSART[4] == '\r' || strUSART[5] == '\r' || strUSART[6] == '\r' || strUSART[7] == '\r' ) && bloqueiaEUSART != 2) {

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
        if (j == StrSIZE - 1 || strUSART[( j - 1 )] == '\r')
            //último caractér passa a null ('\0')
            strUSART[j - 1] = '\0';


        //caso o nr. de leituras seja maior que 0, pretende-se mostrar esse nr. de leituras
        if (numLeituras > 0) {

            //aviso inicial para aguardar, dado as limitações de velocidade de transmissão EUSART e I2C
            sprintf(strUSART, "\fPOR FAVOR AGUARDE\r");
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
                //as variáveis são guardadas como "literias" e a temperatura medida como char com sinal pois pode ser -9ºC
                //a temp. medida é guardada no 2º byte, a de alarme no 4º e o estado deste no 6º
                sprintf(strUSART, "TM=%02d_TA=%02d_AA=%1d_RN=%04d\r", (signed char) i2cReadBlock[1], i2cReadBlock[3], i2cReadBlock[5], regNum);
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

            //bloqueia a escrita para o terminal virtual por outros módulos, de modo a ser possível visualizar os registos
            estadoEUSART = 2;
            bloqueiaEUSART = 1;

            //reinicia o nr. de leituras a 0, para não reentrar na função numa nova interrupção
            numLeituras = 0;

        }

        //executou o que precisou, flag a 0 para não executar novamente até nova interrupção
        intEUSART = 0;
    }

    //caso a interrupção do ADC precise de executar código auxiliar, que é mostrar a saída no terminal virtual e gravar os dados na EEPROM
    if (intADC) {

        //caso a escrita para a EUSART não esteja bloqueada por outra função, escreve para o terminal os dados
        if (estadoEUSART == 0 && bloqueiaEUSART == 0) {
            //copia para a string EUSART os valores das temps. e o estado do alarme, e limpa o ecrã de cada vez que manda a mensagem
            sprintf(strUSART, "\fTM=%02d_TA=%02d_AA=%1d\r", tempAtual, tempAlarme, alarme);
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

        //o estado do I2C tem de ser diferente de 1 e 3, sem ser os restantes 5, logo será 8, para ser forçado a reescrever o estado do módulo
        stateMsgI2c = 8;

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

        //mesma coisa que na 1º escrita, para forçar a reescrever o estado do módulo
        stateMsgI2c = 8;

        //executou o que precisou, flag a 0 para não executar novamente até nova interrupção
        intADC = 0;

    }
}

//função que lê o último registo de memória usado para gravar dados na EEPROM, bem como o nr de registos nela guardado
void iniciarValoresEEPROM(void) {

    //endereço de memória a ler, o 1º, que é onde estão guardados o último registo de memória usado e o nr. de registos guardados até ao momento
    i2cReadAddr[0] = 0x7F;
    i2cReadAddr[1] = 0xF8;

    //manda à EEPROM o comando para mudar para a linha 0x0000
    I2C1_MasterWrite(i2cReadAddr, 2, eepromAddr, stateMsgI2c);
    //e depois lê os quatro bytes sequenciais, onde estão guardadas as variáveis
    I2C1_MasterRead(i2cReadBlock, 4, eepromAddr, stateMsgI2c);

    //caso a memória esteja vazia, inicializa as variáveis a 0
    if (( i2cReadBlock[0] == 0xff && i2cReadBlock[1] == 0xff && i2cReadBlock[2] == 0xff && i2cReadBlock[3] == 0xff ) || ( i2cReadAddr[0] != 0x7F || i2cReadAddr[1] != 0xF8 )) {
        regNum = 0;
        regCountAux = 0;
        memAddr = 0;
        //caso contrário, passa os valores para as variáveis
    } else {
        //o último endereço de memória é do tipo INT, logo leva 2 bytes, o 1º leva um shift de 8 bits à esquerda e em seguida é somado ao segundo byte
        memAddr = ( ( i2cReadBlock[0] << 8 ) + i2cReadBlock[1] ) + 8;
        //o nr. de registos é do tipo INT, logo leva 2 bytes, o 1º leva um shift de 8 bits à esquerda e em seguida é somado ao segundo byte
        regNum = ( ( i2cReadBlock[2] << 8 ) + i2cReadBlock[3] );
        //esta variável corresponde à posição relativa ao inicio do bloco de memória e está diretamente relacionada com o último endereço de memória
        //uma vez que cada registo tem de tamanho 8 bytes, trata-se de uma divisão por 8, o que corresponde a um shift de 3 bits para a direita
        regCountAux = ( memAddr >> 3 );
    }

}

/**
 End of File
 */