/***************************************************************************
*                                       MICA
* File: micaCommon.c
* Workspace: micaCompnents
* Project Name: libMica
* Version: v1.0
* Author: Craig Cheney
*
* Brief:
*   Generic Functions used in MICA
* 
* Authors:
*   Craig Cheney
*
* Change Log:
*   2018.03.03 CC - Document created
********************************************************************************/
#include "micaCommon.h"

/*******************************************************************************
* Function Name: to_degrees()
****************************************************************************//**
*
* \brief Converts a radian argument to degrees
*
* \param radian - value to convert
*
* \return the angle in degrees
*
*
*******************************************************************************/
inline float to_degrees(float radians){
    return radians * (180.0f) / PI;
}
/*******************************************************************************
* Function Name: to_radians()
****************************************************************************//**
*
* \brief Converts a degree argument to radians
*
* \param degrees - value to convert, in degrees
*
* \return the angle in radians
*
*
*******************************************************************************/
inline float to_radians(float degrees){
    return degrees *  PI / (180.0f);
}

/* [] END OF FILE */
