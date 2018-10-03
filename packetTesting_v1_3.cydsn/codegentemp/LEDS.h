/***************************************************************************
*                                       MICA
* File: LED_RGB_pins.c
* Workspace: micaCompnents
* Project Name: libMica
* Version: v1.0
* Author: Craig Cheney
*
* Brief:
*   Block Header for controlling an RGB LED
* 
* Authors:
*   Craig Cheney
*
* Change Log:
*   2018.02.28 CC - Document created
********************************************************************************/
/* Header Guard */
#ifndef LEDS_H
    #define LEDS_H
    /***************************************
    * Included files
    ***************************************/
    #include "cytypes.h"
    #include <stdbool.h>
    /***************************************
    * Function declarations 
    ***************************************/
    uint8  LEDS_Write(uint8 state);
    uint8 LEDS_R_Write(bool state);
    uint8 LEDS_G_Write(bool state);
    uint8 LEDS_B_Write(bool state);

    uint8  LEDS_Read(void);
    bool   LEDS_R_Read(void);
    bool   LEDS_G_Read(void);
    bool   LEDS_B_Read(void);

    void   LEDS_Test(uint8 runs);
    void   LEDS_Sleep(void);
    void   LEDS_Wakeup(void);
    /***************************************
    * Macro Definitions
    ***************************************/
    #define LEDS_ON                 (true)
    #define LEDS_OFF                (false)
    #define LEDS_ACTIVE_VAL         (!1)
       
    #define LEDS_R_PIN_SHIFT        (0u)
    #define LEDS_G_PIN_SHIFT        (1u)
    #define LEDS_B_PIN_SHIFT        (2u)
    
    #define LEDS_ON_NONE            (0u)
    #define LEDS_ON_RED             (1u << LEDS_R_PIN_SHIFT)
    #define LEDS_ON_GREEN           (1u << LEDS_G_PIN_SHIFT)
    #define LEDS_ON_BLUE            (1u << LEDS_B_PIN_SHIFT)
    #define LEDS_ON_YELLOW          (LEDS_ON_RED  | LEDS_ON_GREEN)
    #define LEDS_ON_MAGENTA         (LEDS_ON_RED  | LEDS_ON_BLUE)
    #define LEDS_ON_CYAN            (LEDS_ON_BLUE | LEDS_ON_GREEN)
    #define LEDS_ON_WHITE           (LEDS_ON_RED  | LEDS_ON_GREEN | LEDS_ON_BLUE)
    
    #define LEDS_CTRL_MASK          (0x07u)
    #define LEDS_DELAY_TEST         (500)

    #define LEDS_R_Toggle() LEDS_R_Write(!LEDS_R_Read())
    #define LEDS_G_Toggle() LEDS_G_Write(!LEDS_G_Read())
    #define LEDS_B_Toggle() LEDS_B_Write(!LEDS_B_Read())
    
   
#endif /* LEDS_H */
/* [] END OF FILE */
