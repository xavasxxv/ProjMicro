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

    while (BusyXLCD());
    OpenXLCD(FOUR_BIT & LINES_5X7);

    while (BusyXLCD());
    WriteCmdXLCD(DON & CURSOR_OFF & BLINK_OFF);

    while (BusyXLCD());
    WriteCmdXLCD(SHIFT_CUR_RIGHT);

    while (1) {

        menu();

    }
}

void escreveLinhaLCD(char linha, char texto[StrSIZE]) {

    while (BusyXLCD());
    WriteCmdXLCD(linha);
    while (BusyXLCD());
    putsXLCD(texto);

}

void menu(void) {

    escreveLinhaLCD(LINE1, "Menu         Opcao:");

    while (1) {

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

void monitorizacao(void) {

    sprintf(strLCD, "Temp. Atual:  %2d oC ", tempAtual);
    escreveLinhaLCD(LINE1, strLCD);
    sprintf(strLCD, "Temp. Alarme: %2d oC ", tempAlarme);
    escreveLinhaLCD(LINE2, strLCD);

    do {
        while (clk4Hz) {

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
            } else if (update) {
                sprintf(strLCD, "Temp. Alarme: ");
                escreveLinhaLCD(LINE2, strLCD);
                update = 0;
            }

            sprintf(strLCD, "%2d", tempAtual);
            escreveLinhaLCD(LINE1 + 14, strLCD);
            sprintf(strLCD, "%2d", tempAlarme);
            escreveLinhaLCD(LINE2 + 14, strLCD);

            while (clk4Hz);
        }
    } while (BOTAO_SEL_GetValue() != 0);

    while (BOTAO_SEL_GetValue() == 0);

}

void testeTeclado(void) {

    char input;

    escreveLinhaLCD(LINE1, "Teste Teclado       ");
    escreveLinhaLCD(LINE2, "Numero:             ");

    //teclado
    while (BOTAO_SEL_GetValue() != 0) {
        input = getKBInput();
        sprintf(strLCD, "%c", input);
        escreveLinhaLCD(LINE2 + 8, strLCD);
    }

    while (BOTAO_SEL_GetValue() == 0);

}

void defTempAlarme(void) {

    char c1, c2;
    char tmp_Alarme = 0;

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

    do {

        escreveLinhaLCD(LINE1, "Introduza o alarme  ");
        escreveLinhaLCD(LINE2, "Temp:    oC         ");

        c1 = getKBInput();
        sprintf(strLCD, "%c", c1);
        escreveLinhaLCD(LINE2 + 6, strLCD);
        c2 = getKBInput();
        sprintf(strLCD, "%c", c2);
        escreveLinhaLCD(LINE2 + 7, strLCD);

        if (c1 == '*' || c1 == '#' || c2 == '*' || c2 == '#' || c1 == NULL || c2 == NULL)
            tmp_Alarme = 0;

        c1 = c1 - '0';
        c2 = c2 - '0';
        tmp_Alarme = 10 * c1 + c2;

    } while (tmp_Alarme < 10 || tmp_Alarme > 40);

    tempAlarme = tmp_Alarme;
    
    __delay_ms(500);

}

char getKBInput(void) {

    RB3_SetHigh();

    do {

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
    WrEnFl = 0;

    while (RB0_GetValue() == 1 || RB1_GetValue() == 1 || RB2_GetValue() == 1);

    if (BOTAO_SEL_GetValue() == 0) {
        return NULL;
    }

    return TeclaTMP;

}

void feedbackUSART(void) {

    while (BusyXLCD());
    WriteCmdXLCD(CLEAR_LCD);

    while (BOTAO_SEL_GetValue() != 0) {

        eusart_Tx_On = 1;
        eusart_Tx_En = 1;
        while (eusart_Tx_On == 1);

        //copiar a string USART para a final
        for (i = 0; i < StrSIZE - 1; i++) {
            strLCD[i] = strUSART[i];
        }

        //sequencia de escrita da string para o terminal
        for (i = 0; strUSART[i] != '\0'; i++) {
            EUSART1_Write(strUSART[i]);
            strUSART[i] = '\0';
        }

        //mandar sempre um carriage return (ENTER)
        EUSART1_Write('\r');

        while (BusyXLCD());
        WriteCmdXLCD(CLEAR_LCD);
        escreveLinhaLCD(LINE1, strLCD);

    }

    while (BOTAO_SEL_GetValue() == 0);

    eusart_Tx_On = 0;
    eusart_Tx_En = 0;

}

/**
 End of File
 */