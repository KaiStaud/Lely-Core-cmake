#include "../Inc/main.h"
#include <string.h>
#include "ring_buffer.h"
#include "can.h"
#include <stm32g4xx.h>
#include "main.h"
#include <lely/util/util.h>
#include <stdio.h>
#define CAN_RX_SIZE	16
#define CAN_TX_SIZE	16
#define CAN_MSG_MAX_DATA_LEN       (8)

/** Remote Message */
#define CAN_REMOTE_MSG         ((uint32_t) (1 << 0))

/** Message use Extend ID*/
#define CAN_EXTEND_ID_USAGE     ((uint32_t) (1 << 30))

typedef struct						/*!< Message structure */
{
	uint32_t ID;					/*!< Message Identifier. If 30th-bit is set, this is 29-bit ID, othewise 11-bit ID */
	uint32_t Type;					/*!< Message Type. which can include: - CAN_REMOTE_MSG type*/
	uint32_t DLC;					/*!< Message Data Length: 0~8 */
	uint8_t  Data[CAN_MSG_MAX_DATA_LEN];/*!< Message Data */
} CAN_MSG_T;


static CAN_MSG_T rxbuff[CAN_RX_SIZE];
static CAN_MSG_T txbuff[CAN_TX_SIZE];

static RINGBUFF_T rxring;
static RINGBUFF_T txring;

static void can_flush(void);

CAN_MSG_T prv_read_can_received_msg(FDCAN_HandleTypeDef* hfdcan, uint32_t fifo, uint32_t fifo_isrs)
{

	CAN_MSG_T rcvMsg;
//    CO_CANrx_t* buffer = NULL; /* receive message buffer from CO_CANmodule_t object. */
//    uint16_t index;            /* index of received message */
//    uint8_t messageFound = 0;

    static FDCAN_RxHeaderTypeDef rx_hdr;
    /* Read received message from FIFO */
    if (HAL_FDCAN_GetRxMessage(hfdcan, fifo, &rx_hdr, rcvMsg.Data) != HAL_OK) {
        //return(1);
    }
    /* Setup identifier (with RTR) and length */
    rcvMsg.ID = rx_hdr.Identifier | (rx_hdr.RxFrameType == FDCAN_REMOTE_FRAME ? CAN_FLAG_RTR : 0x00);
    switch (rx_hdr.DataLength) {
        case FDCAN_DLC_BYTES_0:
            rcvMsg.DLC = 0;
            break;
        case FDCAN_DLC_BYTES_1:
            rcvMsg.DLC = 1;
            break;
        case FDCAN_DLC_BYTES_2:
            rcvMsg.DLC = 2;
            break;
        case FDCAN_DLC_BYTES_3:
            rcvMsg.DLC = 3;
            break;
        case FDCAN_DLC_BYTES_4:
            rcvMsg.DLC = 4;
            break;
        case FDCAN_DLC_BYTES_5:
            rcvMsg.DLC = 5;
            break;
        case FDCAN_DLC_BYTES_6:
            rcvMsg.DLC = 6;
            break;
        case FDCAN_DLC_BYTES_7:
            rcvMsg.DLC = 7;
            break;
        case FDCAN_DLC_BYTES_8:
            rcvMsg.DLC = 8;
            break;
        default:
            rcvMsg.DLC = 0;
            break; /* Invalid length when more than 8 */
    }
}

/**
 * \brief           Rx FIFO 0 callback.
 * \param[in]       hfdcan: pointer to an FDCAN_HandleTypeDef structure that contains
 *                      the configuration information for the specified FDCAN.
 * \param[in]       RxFifo0ITs: indicates which Rx FIFO 0 interrupts are signaled.
 */
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
	  FDCAN_RxHeaderTypeDef RxHeader;
	  uint8_t RxData[8];
  if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
  {
	  /* Retrieve Rx messages from RX FIFO0 */
    if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
    {
    Error_Handler();
    }
	CAN_MSG_T Msg;
	Msg.DLC = RxHeader.DataLength;
	Msg.ID = RxHeader.Identifier;
	//Msg.Type =
	for(int i=0;i<8;i++)
	{
	    Msg.Data[i] = RxData[i];

	}
	RingBuffer_Insert(&rxring, &Msg);

  }
}
int bsp_can_init(void)
{
	  /* Set CAN Filter, otherwise all messages will be filtered out */

	    if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, FDCAN_ACCEPT_IN_RX_FIFO0, FDCAN_REJECT,
	                                     FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE)
	        != HAL_OK) {
	        return -1;
	    }

	    if (HAL_FDCAN_ActivateNotification(&hfdcan1,
	                                       0 | FDCAN_IT_RX_FIFO0_NEW_MESSAGE | FDCAN_IT_RX_FIFO1_NEW_MESSAGE
	                                           | FDCAN_IT_TX_COMPLETE | FDCAN_IT_TX_FIFO_EMPTY | FDCAN_IT_BUS_OFF
	                                           | FDCAN_IT_ARB_PROTOCOL_ERROR | FDCAN_IT_DATA_PROTOCOL_ERROR
	                                           | FDCAN_IT_ERROR_PASSIVE | FDCAN_IT_ERROR_WARNING,
	                                       0xFFFFFFFF)
	        != HAL_OK) {
	        return -1;
	    }

	    /* Put CAN module in normal mode */
		if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK)
		{
		  return -1;
		}
return 0;
}
void can_init(int bitrate)
{
	RingBuffer_Init(&rxring, rxbuff, sizeof(CAN_MSG_T), CAN_RX_SIZE);
	RingBuffer_Init(&txring, txbuff, sizeof(CAN_MSG_T), CAN_TX_SIZE);
	bsp_can_init();
}

void can_fini(void)
{
}

size_t can_recv(struct can_msg *ptr, size_t n)
{
	size_t i = 0;
	for (; i < n; i++) {
		CAN_MSG_T Msg;
		if (!RingBuffer_Pop(&rxring, &Msg))
			break;

		// Convert the message to the LCI CAN frame format.
		ptr[i] = (struct can_msg)CAN_MSG_INIT;
		if (Msg.ID & CAN_EXTEND_ID_USAGE) {
			ptr[i].id = Msg.ID & CAN_MASK_EID;
			ptr[i].flags |= CAN_FLAG_IDE;
		} else {
			ptr[i].id = Msg.ID & CAN_MASK_BID;
		}
		if (Msg.Type == CAN_REMOTE_MSG)
			ptr[i].flags |= CAN_FLAG_RTR;
		ptr[i].len = MIN(Msg.DLC, CAN_MAX_LEN);
		memcpy(ptr[i].data, Msg.Data, ptr[i].len);
	}
	return i;
}

int can_send(const struct can_msg *msg, size_t n)
{
const uint16_t CANID_MASK = 0x07FF;
	FDCAN_TxHeaderTypeDef pTxHeader;
	pTxHeader.BitRateSwitch = FDCAN_BRS_OFF;
	pTxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	pTxHeader.FDFormat = FDCAN_CLASSIC_CAN;
	pTxHeader.IdType = FDCAN_STANDARD_ID;
	pTxHeader.MessageMarker = 0;
	pTxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	pTxHeader.TxFrameType = msg->flags;

	pTxHeader.Identifier = msg->id & CANID_MASK;

    switch (msg->len) {
        case 0:
        	pTxHeader.DataLength = FDCAN_DLC_BYTES_0;
            break;
        case 1:
        	pTxHeader.DataLength = FDCAN_DLC_BYTES_1;
            break;
        case 2:
        	pTxHeader.DataLength = FDCAN_DLC_BYTES_2;
            break;
        case 3:
        	pTxHeader.DataLength = FDCAN_DLC_BYTES_3;
            break;
        case 4:
        	pTxHeader.DataLength = FDCAN_DLC_BYTES_4;
            break;
        case 5:
        	pTxHeader.DataLength = FDCAN_DLC_BYTES_5;
            break;
        case 6:
        	pTxHeader.DataLength = FDCAN_DLC_BYTES_6;
            break;
        case 7:
        	pTxHeader.DataLength = FDCAN_DLC_BYTES_7;
            break;
        case 8:
        	pTxHeader.DataLength = FDCAN_DLC_BYTES_8;
            break;
        default: /* Hard error... */
            break;
    }

    // Puffer für die formatierte Ausgabe
    char data_str[256]; // Angenommene Puffergröße, die groß genug ist
    int offset = 0;

    // Formatieren der Daten in den String
    offset += sprintf(data_str + offset, "  ID: 0x%lX", msg->id);
    offset += sprintf(data_str + offset, "  Length: %d", msg->len);
    offset += sprintf(data_str + offset, "  Flags: 0x%lX\r\n", msg->flags);

    offset += sprintf(data_str + offset, "  Data: [ ");
    for (unsigned int i = 0; i < msg->len; ++i) {
        offset += sprintf(data_str + offset, "%02X ", msg->data[i]); // Ausgabe der Daten im Hex-Format
    }
    sprintf(data_str + offset, "]"); // Füge das abschließende Newline hinzu

	int e =  HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &pTxHeader, msg->data);
	if(e != HAL_OK){
	    RTC_DateTypeDef gDate;
	    RTC_TimeTypeDef gTime;
	    HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
	    HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);
	    //Display time Format: hh:mm:ss
	    //trace("[ %02d:%02d:%02d ] failed sending CAN-Frame",gTime.Hours, gTime.Minutes, gTime.Seconds);
	}
}
static void can_flush(void)
{
	CAN_MSG_T Msg;

	while (RingBuffer_Pop(&txring, &Msg))
	{
		struct can_msg msg;
		msg.id = Msg.ID;
		msg.len = Msg.DLC;
		can_send(&msg,&Msg.Data);

	}
}
