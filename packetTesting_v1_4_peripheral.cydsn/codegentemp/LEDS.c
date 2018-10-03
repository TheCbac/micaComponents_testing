/***************************************************************************
*                                       MICA
* File: LED_RGB_pins.c
* Workspace: micaCompnents
* Project Name: libMica
* Version: v1.0
* Author: Craig Cheney
*
* Brief:
*   Block API for controlling an RGB LED
* 
* Authors:
*   Craig Cheney
*
* Change Log:
*   2018.02.28 CC - Document created
********************************************************************************/
#include "LEDS.h"
#include "LEDS_Control_reg.h"
#include "CyLib.h"
#include "micaCommon.h"

/* Set bit at position 'bitShift' to 'val' */
#define SET_BIT_STATE(reg, bitShift, val) (reg = (reg & ~(1u << bitShift)) | (!!val << bitShift))

#define LEDS_ON_VAL             (LEDS_ACTIVE_VAL)
#define LEDS_OFF_VAL            (!LEDS_ACTIVE_VAL)

/*******************************************************************************
* Function Name: LEDS_Write()
********************************************************************************
* Summary:
*   Sets the state of the LED control register
*
* Parameters:
*   uint8 State - The new state mask to write to the control register.
*
* Return:
*   uint8: Value of the control register after it was written. 
*
*******************************************************************************/
uint8 LEDS_Write(uint8 state) {
    /* Set the State accounting for active mod */
    LEDS_Control_reg_Write( (LEDS_ACTIVE_VAL ? state : ~state) & LEDS_CTRL_MASK );
    /* Return the value */
    return LEDS_Control_reg_Read();
}

/*******************************************************************************
* Function Name: LEDS_R_Write()
********************************************************************************
* Summary:
*   Sets the state of the Red LED to the state passed in, Regardless of the 
*   polarity of the LEDs. I.E. LEDS_R_Write(true) will always
*   turn on the LED. 
*
* Parameters:
*   bool State - The new state to write.
*
* Return:
*   The value stored in the control register
*
*******************************************************************************/
uint8 LEDS_R_Write(bool state) {
    /* Read the Current state */
    uint8 reg = LEDS_Control_reg_Read();
    /* Set the bit to the new state */
    SET_BIT_STATE(reg, LEDS_R_PIN_SHIFT, state ? LEDS_ON_VAL: LEDS_OFF_VAL);
    /* Write the bits */
    LEDS_Control_reg_Write(reg);
    /* Return the current state */
    return reg;
}

/*******************************************************************************
* Function Name: LEDS_G_Write()
********************************************************************************
* Summary:
*   Sets the state of the Red LED to the state passed in, Regardless of the 
*   polarity of the LEDs. I.E. LEDS_G_Write(true) will always
*   turn on the LED. 
*
* Parameters:
*   bool State - The new state to write.
*
* Return:
*   The value stored in the control register
*
*******************************************************************************/
uint8 LEDS_G_Write(bool state) {
    /* Read the Current state */
    uint8 reg = LEDS_Control_reg_Read();
    /* Set the bit to the new state */
    SET_BIT_STATE(reg, LEDS_G_PIN_SHIFT, state ? LEDS_ON_VAL: LEDS_OFF_VAL);
    /* Write the bits */
    LEDS_Control_reg_Write(reg);
    /* Return the current state */
    return reg;
}

/*******************************************************************************
* Function Name: LEDS_B_Write()
********************************************************************************
* Summary:
*   Sets the state of the Red LED to the state passed in, Regardless of the 
*   polarity of the LEDs. I.E. LEDS_B_Write(true) will always
*   turn on the LED. 
*
* Parameters:
*   bool State - The new state to write.
*
* Return:
*   The value stored in the control register
*
*******************************************************************************/
uint8 LEDS_B_Write(bool state) {
    /* Read the Current state */
    uint8 reg = LEDS_Control_reg_Read();
    /* Set the bit to the new state */
    SET_BIT_STATE(reg, LEDS_B_PIN_SHIFT, state ? LEDS_ACTIVE_VAL : !LEDS_ACTIVE_VAL);
    /* Write the bits */
    LEDS_Control_reg_Write(reg);
    /* Return the current state */
    return reg;
}

/*******************************************************************************
* Function Name: LEDS_Read()
********************************************************************************
* Summary:
*   Returns the current value of the LED control register
*
* Parameters:
*   None
*
* Return:
*   uint8: Value held in the LED control register
*
*******************************************************************************/
uint8 LEDS_Read(void){
    return LEDS_Control_reg_Read() & LEDS_CTRL_MASK;
}

/*******************************************************************************
* Function Name: LEDS_R_Read()
********************************************************************************
* Summary:
*   Return the current state of the Red LED
*
* Parameters:
*   None
*
* Return:
*   State of the Red LED. True if on, False if off, regardless of LED polarity
*
*******************************************************************************/
bool LEDS_R_Read(void) {
    /* Read the Current state */
    uint8 val = LEDS_Control_reg_Read();
    /* Mask with the bit value */
    val &= LEDS_ON_RED;
    /* Take into effect the polarity */
    return (LEDS_ACTIVE_VAL ? (val) : !val);
}

/*******************************************************************************
* Function Name: LEDS_G_Read()
********************************************************************************
* Summary:
*   Return the current state of the Green LED
*
* Parameters:
*   None
*
* Return:
*   State of the Green LED. True if on, False if off, regardless of LED polarity
*
*******************************************************************************/
bool LEDS_G_Read(void) {
    /* Read the Current state */
    uint8 val = LEDS_Control_reg_Read();
    /* Mask with the bit value */
    val &= LEDS_ON_GREEN;
    /* Take into effect the polarity */
    return (LEDS_ACTIVE_VAL ? (val) : !val);
}

/*******************************************************************************
* Function Name: LEDS_B_Read()
********************************************************************************
* Summary:
*   Return the current state of the Blue LED
*
* Parameters:
*   None
*
* Return:
*   State of the Blue LED. True if on, False if off, regardless of LED polarity
*
*******************************************************************************/
bool LEDS_B_Read(void) {
    /* Read the Current state */
    uint8 val = LEDS_Control_reg_Read();
    /* Mask with the bit value */
    val &= LEDS_ON_BLUE;
    /* Take into effect the polarity */
    return (LEDS_ACTIVE_VAL ? (val) : !val);
}


/*******************************************************************************
* Function Name: LEDS_Sleep()
********************************************************************************
* Summary:
*   Prepares the component for sleep. Sleeps the control Register.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void LEDS_Sleep(void) {
    /* Sleep Control reg */
    LEDS_Control_reg_Sleep();
}

/*******************************************************************************
* Function Name: LEDS_Wakeup()
********************************************************************************
* Summary:
*   Wakes up the component from sleep. Wakes the control Register.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void LEDS_Wakeup(void) {
    /* Wakeup Control reg */
    LEDS_Control_reg_Wakeup();
}




/*******************************************************************************
* Function Name: LEDS_Test()
********************************************************************************
* Summary:
*   Runs through the board level testing routine for the LEDs.
*
* Parameters:
*   uint8: The number of times to iterate through the test. 0 means infinite. 
*
* Expected Outcome: 
*   0. All LEDs off
*   1. Red LED on
*   2. Green LED on
*   3. Blue LED on
*   4. Yellow (RB)
*   5. Cyan (BG)
*   6. Magenta (RG)
*   7. White (RGB) LEDs on
*
* Return:
*   None
*
*******************************************************************************/
void LEDS_Test(uint8 runs){
    /* Iteration variable */
    uint8 i = ZERO;     
    do{
        /* 0 - All LEDs off */
        LEDS_Write(LEDS_ON_NONE);
        CyDelay(LEDS_DELAY_TEST);
        /* 1 - Red LED on */
        LEDS_Write(LEDS_ON_RED);
        CyDelay(LEDS_DELAY_TEST);
        /* 2 - Green LED on */
        LEDS_Write(LEDS_ON_GREEN);
        CyDelay(LEDS_DELAY_TEST);
        /* 3 - Blue LED on */
        LEDS_Write(LEDS_ON_BLUE);
        CyDelay(LEDS_DELAY_TEST);
        /* 4 - Yellow on */
        LEDS_Write(LEDS_ON_YELLOW);
        CyDelay(LEDS_DELAY_TEST);
        /* 5 - Cyan on */
        LEDS_Write(LEDS_ON_CYAN);
        CyDelay(LEDS_DELAY_TEST);
        /* 6 - Magneta on */
        LEDS_Write(LEDS_ON_MAGENTA);
        CyDelay(LEDS_DELAY_TEST);
        /* 7 -White on*/
        LEDS_Write(LEDS_ON_WHITE);
        CyDelay(LEDS_DELAY_TEST);
        /* Increment the iteration */
        i++;
    } while(runs == ZERO || (i < runs));
    
}


/* [] END OF FILE */
