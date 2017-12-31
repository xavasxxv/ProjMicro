/**
  EXT_INT Generated Driver File 

  @Company:
    Microchip Technology Inc.

  @File Name:
    ext_int.c

  @Summary
    This is the generated driver implementation file for the EXT_INT 
    driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs 

  @Description:
    This source file provides implementations for driver APIs for EXT_INT. 
    Generation Information : 
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs  - 1.45
        Device            :  PIC18F45K22
        Driver Version    :  1.0
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40
 */

/**
   Section: Includes
 */
#include <xc.h>
#include "ext_int.h"
#include "pin_manager.h"
//***User Area Begin->code: Add External Interrupt handler specific headers 
#include "../GlobalDefinitions.h"
//***User Area End->code: Add External Interrupt handler specific headers

/**
   Section: External Interrupt Handlers
 */
// INTn Dynamic Interrupt Handlers 
void (*INT1_InterruptHandler)(void);
void (*INT2_InterruptHandler)(void);
void (*INT0_InterruptHandler)(void);

/**
  Interrupt Handler for EXT_INT1 - INT1
 */

//interrup��o externa nr.1, correspondente a RB1
void INT1_ISR(void) {
    //***User Area Begin->code***

    //averigua qual foi o �ltimo pino que esteve a 1 e atribui um caract�r consoante a resposta
    if (RB3_GetValue() == 1) {
        TeclaTMP = '2';
    }
    if (RB4_GetValue() == 1) {
        TeclaTMP = '5';
    }
    if (RB5_GetValue() == 1) {
        TeclaTMP = '8';
    }
    if (RB6_GetValue() == 1) {
        TeclaTMP = '0';
    }
    //habilita a escrita com teclas, de modo a n�o ser poss�vel haver m�ltiplas entradas
    WrEnFl = 1;

    //***User Area End->code***

    EXT_INT1_InterruptFlagClear();

    // Callback function gets called everytime this ISR executes
    INT1_CallBack();
}

/**
  Callback function for EXT_INT1 - INT1
 */
void INT1_CallBack(void) {
    // Add your custom callback code here
    if (INT1_InterruptHandler) {
        INT1_InterruptHandler();
    }
}

/**
  Allows selecting an interrupt handler for EXT_INT1 - INT1 at application runtime
 */
void INT1_SetInterruptHandler(void (* InterruptHandler)(void)) {
    INT1_InterruptHandler = InterruptHandler;
}

/**
  Default interrupt handler for EXT_INT1 - INT1 
 */
void INT1_DefaultInterruptHandler(void) {
    // add your INT1 interrupt custom code
    // or set custom function using INT1_SetInterruptHandler()
}

/**
  Interrupt Handler for EXT_INT2 - INT2
 */

//interrup��o externa nr.2, correspondente a RB2
void INT2_ISR(void) {
    //***User Area Begin->code***

    //averigua qual foi o �ltimo pino que esteve a 1 e atribui um caract�r consoante a resposta
    if (RB3_GetValue() == 1) {
        TeclaTMP = '3';
    }
    if (RB4_GetValue() == 1) {
        TeclaTMP = '6';
    }
    if (RB5_GetValue() == 1) {
        TeclaTMP = '9';
    }
    if (RB6_GetValue() == 1) {
        TeclaTMP = '#';
    }
    //habilita a escrita com teclas, de modo a n�o ser poss�vel haver m�ltiplas entradas
    WrEnFl = 1;

    //***User Area End->code***

    EXT_INT2_InterruptFlagClear();

    // Callback function gets called everytime this ISR executes
    INT2_CallBack();
}

/**
  Callback function for EXT_INT2 - INT2
 */
void INT2_CallBack(void) {
    // Add your custom callback code here
    if (INT2_InterruptHandler) {
        INT2_InterruptHandler();
    }
}

/**
  Allows selecting an interrupt handler for EXT_INT2 - INT2 at application runtime
 */
void INT2_SetInterruptHandler(void (* InterruptHandler)(void)) {
    INT2_InterruptHandler = InterruptHandler;
}

/**
  Default interrupt handler for EXT_INT2 - INT2 
 */
void INT2_DefaultInterruptHandler(void) {
    // add your INT2 interrupt custom code
    // or set custom function using INT2_SetInterruptHandler()
}

/**
  Interrupt Handler for EXT_INT0 - INT0
 */

//interrup��o externa nr.0, correspondente a RB0
void INT0_ISR(void) {
    //***User Area Begin->code***

    //averigua qual foi o �ltimo pino que esteve a 1 e atribui um caract�r consoante a resposta
    if (RB3_GetValue() == 1) {
        TeclaTMP = '1';
    }
    if (RB4_GetValue() == 1) {
        TeclaTMP = '4';
    }
    if (RB5_GetValue() == 1) {
        TeclaTMP = '7';
    }
    if (RB6_GetValue() == 1) {
        TeclaTMP = '*';
    }
    //habilita a escrita com teclas, de modo a n�o ser poss�vel haver m�ltiplas entradas
    WrEnFl = 1;

    //***User Area End->code***

    EXT_INT0_InterruptFlagClear();

    // Callback function gets called everytime this ISR executes
    INT0_CallBack();
}

/**
  Callback function for EXT_INT0 - INT0
 */
void INT0_CallBack(void) {
    // Add your custom callback code here
    if (INT0_InterruptHandler) {
        INT0_InterruptHandler();
    }
}

/**
  Allows selecting an interrupt handler for EXT_INT0 - INT0 at application runtime
 */
void INT0_SetInterruptHandler(void (* InterruptHandler)(void)) {
    INT0_InterruptHandler = InterruptHandler;
}

/**
  Default interrupt handler for EXT_INT0 - INT0 
 */
void INT0_DefaultInterruptHandler(void) {
    // add your INT0 interrupt custom code
    // or set custom function using INT0_SetInterruptHandler()
}
/**
    Section: External Interrupt Initializers
 */

/**
    void EXT_INT_Initialize(void)

    Initializer for the following external interrupts
    INT1
    INT2
    INT0
 */
void EXT_INT_Initialize(void) {

    /*******
     * INT1
     * Clear the interrupt flag
     * Set the external interrupt edge detect
     * Enable the interrupt, if enabled in the UI. 
     ********/
    EXT_INT1_InterruptFlagClear();
    EXT_INT1_risingEdgeSet();
    // Set Default Interrupt Handler
    INT1_SetInterruptHandler(INT1_DefaultInterruptHandler);
    EXT_INT1_InterruptEnable();


    /*******
     * INT2
     * Clear the interrupt flag
     * Set the external interrupt edge detect
     * Enable the interrupt, if enabled in the UI. 
     ********/
    EXT_INT2_InterruptFlagClear();
    EXT_INT2_risingEdgeSet();
    // Set Default Interrupt Handler
    INT2_SetInterruptHandler(INT2_DefaultInterruptHandler);
    EXT_INT2_InterruptEnable();


    /*******
     * INT0
     * Clear the interrupt flag
     * Set the external interrupt edge detect
     * Enable the interrupt, if enabled in the UI. 
     ********/
    EXT_INT0_InterruptFlagClear();
    EXT_INT0_risingEdgeSet();
    // Set Default Interrupt Handler
    INT0_SetInterruptHandler(INT0_DefaultInterruptHandler);
    EXT_INT0_InterruptEnable();

}

