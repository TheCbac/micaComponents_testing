/***************************************************************************
*                                       MICA
* File: packets.h
* Workspace: micaComponents
* Project Name: libMica
* Version: v1.3
* Author: Craig Cheney
*
* Brief:
*   Binary packet specification for MICA
* 
* Authors:
*   Craig Cheney
*
* Change Log:
*   2018.04.19 CC - Document created
********************************************************************************/
/* Header Guard */
#ifndef packets_H
    #define packets_H
    /***************************************
    * Included files
    ***************************************/
    #include <stdbool.h>
    #include "cytypes.h"
    /***************************************
    * Macro Definitions
    ***************************************/
    #define packets_LEN_SYM_START       (1u) /**< Length of the Start of Packet Symbol */
    #define packets_LEN_MODULE_ID       (1u) /**< Length of the Module ID */
    #define packets_LEN_PAYLOAD_LEN     (2u) /**< Length of the Payload Length */
    #define packets_LEN_FLAGS           (4u) /**< Length of the flags  */
    #define packets_LEN_CMD             (1u) /**< Length of the Command */
    #define packets_LEN_CHECKSUM        (2u) /**< Length of the checksum */
    #define packets_LEN_SYM_END         (1u) /**< Length of the End of Packet Symbol */
    #define packets_LEN_HEADER          (packets_LEN_SYM_START + packets_LEN_MODULE_ID + packets_LEN_PAYLOAD_LEN  + packets_LEN_CMD) /**< Length of the packet overhead, for checksum validation */
    #define packets_LEN_FOOTER          (packets_LEN_FLAGS + packets_LEN_CHECKSUM + packets_LEN_SYM_END) /**< Length of the footer */
    #define packets_LEN_OVERHEAD        (packets_LEN_HEADER + packets_LEN_FOOTER) /**< Total length of the packet overhead */
    #define packets_LEN_MAX_PAYLOAD     (256) /**< Maximum length of the payload */
    #define packets_LEN_MAX_PACKET      (packets_LEN_OVERHEAD + packets_LEN_MAX_PAYLOAD) /**< Maximum length of the entire packet */

    #define packets_LEN_BLOCK_PAYLOAD  (0x80u)
    #define packets_LEN_PACKET_128   (packets_LEN_OVERHEAD  + packets_LEN_BLOCK_PAYLOAD )

    #define packets_INDEX_START         (0u) /**< Index of the start of packet symbol*/
    #define packets_INDEX_MODULE_ID     (1u) /**< Index of the module to talk to in the packet */
    #define packets_INDEX_CMD           (2u) /**< Index of the command to issue */
    #define packets_INDEX_LEN_MSB       (3u) /**< Index of the Payload length MSB */
    #define packets_INDEX_LEN_LSB       (4u) /**< Index of the Payload length LSB */
    #define packets_INDEX_PAYLOAD       (5u) /**< Index of the packet Payload */

    #define packets_SYM_START           (0x01u) /**< Start of Packet Symbol. All packets must begin with this symbol */
    #define packets_SYM_END             (0xAAu) /**< End of Packet Symbol. All packets must end with this symbol */

    #define packets_ID_MODULE_ENERGY    (0u)    /**< ID of the Energy Module */
    #define packets_ID_MODULE_ACTUATION (1u)    /**< ID of the Actuation Module */
    #define packets_ID_MODULE_POWER     (2u)    /**< ID of the Power Module */
    #define packets_ID_MODULE_SENSING   (3u)    /**< ID of the Sensing Module */
    #define packets_ID_MODULE_COMM      (4u)    /**< ID of the Communications Module */
    #define packets_ID_MODULE_CONTROL   (5u)    /**< ID of the Contorl Module */
    #define packets_ID_MODULE_MAX       (5u)    /**< Maximum value allowed */
     /* **** RESPONSES **** */
    #define packets_RESP_INDEX_START    (0u) /**< Index of the start of response symbol*/
    #define packets_RESP_INDEX_MODULE_ID (1u) /**< Index module that is responding*/
    #define packets_RESP_INDEX_STATUS   (2u) /**< Index of the response status*/
    #define packets_RESP_INDEX_LEN_MSB  (3u) /**< Index of the response payload length MSB */
    #define packets_RESP_INDEX_LEN_LSB  (4u) /**< Index of the response payload length LSB*/
    #define packets_RESP_INDEX_PAYLOAD  (5u) /**< Index of the response payload */

    #define packets_RESP_LEN_SYM_START   (1u) /**< Length of the response Start of Packet Symbol */
    #define packets_RESP_LEN_MODULE_ID   (1u) /**< Length of the module ID */        
    #define packets_RESP_LEN_STATUS      (1u) /**< Length of the response Status */        
    #define packets_RESP_LEN_PAYLOAD_LEN (2u) /**< Length of the response Payload Length */
    #define packets_RESP_LEN_CHECKSUM    (2u) /**< Length of the response checksum */
    #define packets_RESP_LEN_SYM_END     (1u) /**< Length of the response End of Packet Symbol */
    

    #define packets_RESP_LEN_HEADER     (packets_RESP_LEN_SYM_START + packets_RESP_LEN_MODULE_ID + packets_RESP_LEN_STATUS + packets_RESP_LEN_PAYLOAD_LEN) /**< Length of the response packet header */
    #define packets_RESP_LEN_FOOTER     (packets_RESP_LEN_CHECKSUM + packets_RESP_LEN_SYM_END) /**< Length of the response footer */
    #define packets_RESP_LEN_OVERHEAD   (packets_RESP_LEN_HEADER + packets_RESP_LEN_FOOTER) /**< Length of the response overhead */   
    #define packets_RESP_LEN_MAX_PAYLOAD (100) /**< Maximum length of the payload */     
    #define packets_RESP_LEN_MAX_PACKET (packets_RESP_LEN_OVERHEAD + packets_RESP_LEN_MAX_PAYLOAD) /**< Maximum length of the entire packet */
    /* **** COMMANDS **** */
    #define packets_RESP_SUCCESS        (0x00) /**< Successfully executed the previous command */
    #define packets_RESP_ASYNC          (0x80) /**< All response code that are greater than or equal to this are not responses but rather async data */
    
    #define packets_ERR_SUCCESS        (0x00u)     /**< Returned Success */
    #define packets_ERR_MEMORY         (1u << packets_ERR_SHIFT_MEMORY )     /**< Failed to allocate memory*/
    #define packets_ERR_START_SYM      (1u << packets_ERR_SHIFT_START_SYM)   /**< Incorrect start symbol was received */
    #define packets_ERR_END_SYM        (1u << packets_ERR_SHIFT_END_SYM)      /**< Incorrect end symbol was received */
    #define packets_ERR_LENGTH         (1u << packets_ERR_SHIFT_LENGTH)   /**< The amount of data available is outside the expected range. */
    #define packets_ERR_FORMAT         (1u << packets_ERR_SHIFT_FORMAT)    /**< The packet is not in the correct format */
    #define packets_ERR_INCOMPLETE     (1u << packets_ERR_SHIFT_INCOMPLETE)    /**< The packet cannot be processed as it is incomplete */
    #define packets_ERR_MODULE         (1u << packets_ERR_SHIFT_MODULE)    /**< An invalid module was specified */
    #define packets_ERR_DATA           (1u << packets_ERR_SHIFT_DATA)    /**< The data is not of the proper form  */
    #define packets_ERR_CMD            (1u << packets_ERR_SHIFT_CMD)    /**< The command is not recognized */
    #define packets_ERR_CHECKSUM       (1u << packets_ERR_SHIFT_CHECKSUM)    /**< The packet checksum does not match the expected value */
    #define packets_ERR_STATE          (1u << packets_ERR_SHIFT_STATE)     /**< Device was in the incorrect state to execute the command */
    #define packets_ERR_DEVICE         (1u << packets_ERR_SHIFT_DEVICE)     /**< An Unknown device was addressed */

    #define packets_ERR_SHIFT_MEMORY         (0u)    /**< Failed to allocate memory */
    #define packets_ERR_SHIFT_START_SYM      (1u)     /**< Incorrect start symbol was received */
    #define packets_ERR_SHIFT_END_SYM        (2u)     /**< Incorrect end symbol was received */
    #define packets_ERR_SHIFT_LENGTH         (3u)     /**< The amount of data available is outside the expected range. */
    #define packets_ERR_SHIFT_FORMAT         (4u)     /**< The packet is not in the correct format */
    #define packets_ERR_SHIFT_INCOMPLETE     (5u)     /**< The packet cannot be processed as it is incomplete */
    #define packets_ERR_SHIFT_MODULE         (6u)     /**< An invalid module was specified */
    #define packets_ERR_SHIFT_DATA           (7u)     /**< The data is not of the proper form  */
    #define packets_ERR_SHIFT_CMD            (8u)     /**< The command is not recognized */
    #define packets_ERR_SHIFT_CHECKSUM       (9u)     /**< The packet checksum does not match the expected value */
    #define packets_ERR_SHIFT_STATE          (10u)     /**< Device was in the incorrect state to execute the command */
    #define packets_ERR_SHIFT_DEVICE         (11u)     /**< An Unknown device was addressed */
    
    #define packets_ERR__UNKNOWN             (31u)     /**< An unknown error occurred - End of error space */
    /* Async data */
    #define packets_ASYNC_REPORT_ADV   (0x80u)     /**< Report an advertisement packet - Start of async space */
    #define packets_ASYNC_REPORT_CONN  (0x81u)     /**< Report a connection success  */
    #define packets_ASYNC_REPORT_DCON  (0x82u)     /**< Report a disconnection  */

    /* FLAGS */
    #define packets_FLAG_SHIFT_RESP            (0u) /**< A response is requested from the packet */
    
    #define packets_FLAG_RESP                  (1u << packets_FLAG_SHIFT_RESP) /**< A response is requested from the packet */
    

    /***************************************
    * Enumerated Types
    ***************************************/
    /* Possible states for receive buffer */
    typedef enum {
        packets_BUFFER_RECEIVE_WAIT_FOR_START, /**< Waiting for the start byte to be received */
        packets_BUFFER_RECEIVE_HEADER,         /**< Receiving the header Module ID, command, Payload length */
        packets_BUFFER_RECEIVE_PAYLOAD,        /**< Receiving N bytes of payload data */
        packets_BUFFER_RECEIVE_FOOTER,         /**< Receiving Checksum and end of packet */
        packets_BUFFER_RECEIVE_COMPLETE        /**< Packet has finished being assembled  */
    } packets_BUFFER_STATE_RECEIVE_T;
    
        /* Possible states for send buffer */
    typedef enum {
        packets_BUFFER_SEND_WAIT,              /**< Waiting for the buffer to be ready to send */
        packets_BUFFER_SEND_READY,             /**< Buffer is ready to be sent */
        packets_BUFFER_SEND_QUEUEING,          /**< Move the buffer to the FIFO UART TX buffer*/
        packets_BUFFER_SEND_COMPLETE,          /**< buffer has be sent*/
    } packets_BUFFER_STATE_SEND_T;
    
    /* State of packet */
    typedef enum {
        packets_PACKET_UNHANDLED,          /**< Packet has not been handled in any way */
        packets_PACKET_ACK,                /**< Packet has been acknowledged*/
    } packets_PACKET_STATE_T;
    /***************************************
    * Structures
    ***************************************/
    /* Process buffer struct */
    typedef struct {
        uint8_t *buffer;                            /**< Pointer to the buffer */
        uint16_t bufferLen;                         /**< Length of the buffer */
        uint16_t bufferIndex;                       /**< Index of the current value */
        uint32_t timeCount;                         /**< Time count of the last buffer action */
    } packets_BUFFER_PROCESS_S;


    /* Packet buffer struct */
    typedef struct {
        uint8_t moduleId;       /**< ID of the module commanded */
        uint8_t cmd;            /**< Command issued */
        uint8_t *payload;       /**< Pointer to the data */
        uint16_t payloadMax;   /**< Size of the payload array */
        uint16_t payloadLen;    /**< Length of the payload in the buffer*/
        uint32_t flags;         /**< Flags to include e.g. "ACK requested" */
        uint32_t error;          /**< Error code of packet*/
    } packets_PACKET_S;

    /* Buffer struct for send and receive */
    typedef struct {
        packets_PACKET_S packet;        /**< Receives data from the process buffer and passes to app */
        packets_BUFFER_PROCESS_S processBuffer;    /**< Send Buffer Structure */
        packets_BUFFER_STATE_SEND_T bufferState;         /**< State of the send buffer */
    } packets_BUFFER_SEND_S;

    /* Buffer struct for send and receive */
    typedef struct {
        packets_PACKET_S packet;      /**< Receives data from the process buffer and passes to app */
        packets_BUFFER_PROCESS_S processBuffer;     /**< Receive Buffer Structure */
        packets_BUFFER_STATE_RECEIVE_T bufferState;       /**< State of the receive buffer */
    } packets_BUFFER_RECEIVE_S;

/* Struct containing both a send and receive buffer */
    typedef struct {
        packets_BUFFER_SEND_S send;
        packets_BUFFER_RECEIVE_S receive;
    } packets_BUFFER_FULL_S;


    
    /***************************************
    * Function declarations 
    ***************************************/
    uint32_t packets_initialize(packets_BUFFER_FULL_S *packetBuffer);
    uint32_t packets_generateBuffers(packets_BUFFER_FULL_S *packetBuffer, uint16_t bufferSize);
    uint32_t packets_destoryBuffers(packets_BUFFER_FULL_S *buffer);
    void packets_flushRxBuffers(packets_BUFFER_FULL_S* buffer);
    void packets_flushTxBuffers(packets_BUFFER_FULL_S* buffer);
    void packets_flushBuffers(packets_BUFFER_FULL_S* buffer);


    uint32_t packets_constructPacket(packets_BUFFER_FULL_S *buffer);
    uint32_t packets_sendPacket(packets_BUFFER_FULL_S *buffer);

    uint32_t packets_processRxByte(packets_BUFFER_FULL_S* buffer, uint8_t byte);
    uint32_t packets_parsePacket(packets_BUFFER_FULL_S* buffer);

    uint16_t packets_computeChecksum16(uint8_t* data, uint16_t length);
#endif /* packets_H */
/* [] END OF FILE */

