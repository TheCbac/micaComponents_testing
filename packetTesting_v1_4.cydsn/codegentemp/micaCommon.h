/***************************************************************************
*                                       MICA
* File: micaCommon.h
* Workspace: micaCompnents
* Project Name: libMica
* Version: v1.0
* Author: Craig Cheney
*
* Brief:
*   common Definitions used in MICA
* 
* Authors:
*   Craig Cheney
*
* Change Log:
*   2018.03.03 CC - Document created
********************************************************************************/
/* Header Guard */
#ifndef micaCommon_H
    #define micaCommon_H
    
    /***************************************
    * Included files
    ***************************************/
    #include "CyLib.h"
    #include <stdbool.h>
    /***************************************
    * Macro Definitions
    ***************************************/
    #define ZERO            (0u)    /**< Constant 0 */
    #define ONE             (1u)    /**< Constant 1 */
    #define TWO             (2u)    /**< Constant 2 */
    #define THREE           (3u)    /**< Constant 3 */
    #define FOUR            (4u)    /**< Constant 4 */
    #define FIVE            (5u)    /**< Constant 5 */
    #define SIX             (6u)    /**< Constant 6 */
    #define SEVEN           (7u)    /**< Constant 7 */
    #define EIGHT           (8u)    /**< Constant 8 */
    #define NINE            (9u)    /**< Constant 9 */
    #define TEN             (10u)   /**< Constant 10 */
    #define ELEVEN          (11u)   /**< Constant 11 */
    #define TWELVE          (12u)   /**< Constant 12 */
    #define THIRTEEN        (13u)   /**< Constant 13 */
    #define FOURTEEN        (14u)   /**< Constant 14 */
    #define FIFTEEN         (15u)   /**< Constant 15 */
    #define SIXTEEN         (16u)   /**< Constant 16 */
    
    #define NULL_OCTET      (0u)    /**< Null Octet */
    
    #define PI              (3.14159265358979f) /**< Pi */
    
    #define ZERO_F          (0.0f)  /**< Floating point Zero */
    #define HALF_F          (0.5f)  /**< Floating point half */
    #define ONE_F           (1.0f)  /**< Floating point One */
    #define TWO_F           (2.0f)  /**< Floating point two */
    
    #define BITS_SINGLE_BIT         (0x01u)             /**< Single bit mask */
    #define BITS_ONE_NIBBLE         (4u)                /**< Number of bits in a nibble */
    #define BITS_ONE_BYTE           (8u)                /**< Number of bits in one byte */
    #define BITS_TWO_BYTES          (16u)               /**< Number of bits in two bytes */
    #define BITS_THREE_BYTES        (24u)               /**< Number of bits in three bytes */
    
    #define SHIFT_BYTE_HALF         (BITS_ONE_NIBBLE)   /**< Half Byte Shift*/
    #define SHIFT_BYTE_ONE          (BITS_ONE_BYTE)     /**< One Byte Shift*/
    #define SHIFT_BYTE_TWO          (BITS_TWO_BYTES)    /**< Two Byte Shift*/
    #define SHIFT_BYTE_THREE        (BITS_THREE_BYTES)  /**< Three Byte Shift*/
    
    #define MASK_NIBBLE_HIGH        (0xF0u)             /**< High nibble mask*/
    #define MASK_NIBBLE_LOW         (0x0Fu)             /**< Low nibble mask*/
    #define MASK_BYTE_ONE           (0xFFu)             /**< One byte mask*/
    #define MASK_BYTE_TWO           (0xFFFFu)           /**< Two byte mask*/
    #define MASK_BYTE_THREE         (0xFFFFFFu)         /**< Three byte mask*/
    
    #define MASK_BIT_ONE            (0x1u)              /**< One bit mask*/
    #define MASK_BIT_TWELVE         (0xFFFu)            /**< One and a half byte mask*/
    #define MASK_ODD                (MASK_BIT_ONE)      /**< Odd mask*/
    
    
    #define INDEX_ZERO_CORRECT      (1u)                /**< Corrects for a zero indexed array*/
    
    #define MICA_TEST_INFINITE      (0u)                /**< Run test an infinite number of times  */
    #define MICA_TEST_ONCE          (1u)                /**< Run test one time */
   
    #define MICA_DELAY_MS_SEC_TENTH     (100)           /**< Delay in ms for 0.1 seconds  */
    #define MICA_DELAY_MS_SEC_QUARTER   (250)           /**< Delay in ms for 0.25 seconds  */
    #define MICA_DELAY_MS_SEC_HALF      (500)           /**< Delay in ms for 0.5 second  */
    #define MICA_DELAY_MS_SEC_ONE       (1000)          /**< Delay in ms for 1 second  */
    
    #define MICA_DELAY_US_SEC_TENTH     (100000)        /**< Delay in us for 0.1 second  */
    #define MICA_DELAY_US_SEC_QUARTER   (250000)        /**< Delay in us for 0.25 second  */
    #define MICA_DELAY_US_SEC_HALF      (500000)        /**< Delay in us for 0.5 second  */
    #define MICA_DELAY_US_SEC_ONE       (1000000)       /**< Delay in us for 1.0 second  */
    

    #define MICA_delayMs(val)     CyDelay(val)    /**< Delay function */
    
    /***************************************
    * Structs
    ***************************************/   
    /* Struct for handling Float accelerometer data [m/s^2] */
    typedef struct {
        float Ax; /**< Acceleration X */
        float Ay; /**< Acceleration Y */
        float Az; /**< Acceleration Z */
    } ACC_DATA_F;
    /* Struct for handling Float Gyro data [deg/s] */
    typedef struct {
        float Wx; /**< Angular velocity, X */
        float Wy; /**< Angular velocity, Y */
        float Wz; /**< Angular velocity, Z */
    } GYR_DATA_DEG_F;
    /* Struct for handling Float Gyro data [rad/s] */
    typedef struct {
        float Wx; /**< Angular velocity, X */
        float Wy; /**< Angular velocity, Y */
        float Wz; /**< Angular velocity, Z */
    } GYR_DATA_RAD_F;
    /* Struct for handling Float magnetometer data [uT] */
    typedef struct{
        float X; /**< Field Strength, X */
        float Y; /**< Field Strength, Y */
        float Z; /**< Field Strength, Z */
    } MAG_DATA_F;
        
    /* Struct for handling int16 accelerometer data */
    typedef struct {
        int16 Ax; /**< Acceleration X */
        int16 Ay; /**< Acceleration Y */
        int16 Az; /**< Acceleration Z */
    } ACC_DATA_T;
    /* Struct for handling Float Gyro data */
    typedef struct {
        int16 Wx; /**< Angular velocity, X */
        int16 Wy; /**< Angular velocity, Y */
        int16 Wz; /**< Angular velocity, Z */
    } GYR_DATA_T;
    /* Struct for handling Float magnetometer data [uT] */
    typedef struct{
        int16 X; /**< Field Strength, X */
        int16 Y; /**< Field Strength, Y */
        int16 Z; /**< Field Strength, Z */
    } MAG_DATA_T;
        
    
    
    /* Keep track of which channels are enabled */
    typedef struct {
        bool X;     /**< Indicates whether the X channel is active or not */
        bool Y;     /**< Indicates whether the Y channel is active or not */
        bool Z;     /**< Indicates whether the Z channel is active or not */
    } CHANNELS_XYZ_T;
    
    /* Struct for representing a Quaternion */
    typedef struct {
        float q1; /**< First element (Scalar) */
        float q2; /**< Second element (i) */
        float q3; /**< Second element (j) */
        float q4; /**< Second element (k) */
    } QUATERNION_T;
    /* Struct for handling Euler angles */
    typedef struct {
        float yaw; /**< Yaw of system */
        float pitch; /**< pitch of system */
        float roll; /**< roll of system */
    } EULER_ANGLE_T;
    
    /* Struct for Encoders */
    typedef struct {
        uint32 rightCount;  /**< Right encoder count value */
        uint32 leftCount;   /**< Left encoder count value */
    } ENCODER_DUAL_T;
    
    
    /* Structs for packets */
    
    
    
    /***************************************
    * Typedefs
    ***************************************/  
    typedef void FUNCTION_T(void); /**< void/void function type (for interrupts) */
    
    /***************************************
    * Function prototypes
    ***************************************/  
    float to_degrees(float radians);
    float to_radians(float degrees);
    
#endif /* micaCommon_H */

/* [] END OF FILE */

