
#ifndef INC_audioBuf_H_
#define INC_audioBuf_H_

#include "log.h"

#include "usbd_audio.h"


#define BUFFER_SLOTS 6
#define SAMPLERATE_MS 48
#define SAMPLES_16_MS (SAMPLERATE_MS * 2)
#define SAMPLES_8_MS (SAMPLES_16_MS * 2)

//#define SOUND_PKG_SIZE 192
#define BUFFER_SIZE (SAMPLES_16_MS *BUFFER_SLOTS)
extern int16_t *pInBuffPtr;

extern volatile uint8_t dataReady;
extern uint16_t i2SDataBuffer[BUFFER_SIZE];

extern volatile uint16_t i2SSampleIdx;
extern USBD_HandleTypeDef *pdefRef;
extern volatile uint8_t send_audio_frame;

#define USBSAMPLES 1
extern volatile uint8_t usbData[USBSAMPLES][192];
extern volatile uint8_t usbPkgIdx;
extern volatile uint8_t usbPkgSendFlag;
extern volatile USBD_HandleTypeDef *pdevRef;

typedef enum {
	PLAYING_INIT=0,
	PLAYING_RUNNING,
} BUFFER_PLAYING_typeDef;

typedef enum {
	BUFFER_OK = 0,
	BUFFER_ERROR,
	BUFFER_EMPTY,
	BUFFER_FILLING,
	BUFFER_FULL,
	BUFFER_OVERFLOW
} BUFFER_CMD_typeDef;

typedef struct {
	uint8_t rd_overflow;
	uint8_t wr_overflow;
	uint16_t dataBuffered; //data in buffer
	uint16_t dataAvailable; //data slots open
	BUFFER_PLAYING_typeDef playingStatus;
	uint16_t max_size;
} BUFFER_extra_typeDef;


typedef struct
{
	uint8_t data[198];
	uint8_t len;
} nextUsbFrame_typeDef;



extern volatile nextUsbFrame_typeDef nextAudioFrame;

/*
 * PROTOs
 *
 */
void Buffer_Init();
BUFFER_CMD_typeDef Buffer_AddToBuffer(USBD_AUDIO_HandleTypeDef *haudio,	int16_t *inData, uint16_t inDataLen);
BUFFER_CMD_typeDef Buffer_GetNextAudioFrame(USBD_AUDIO_HandleTypeDef *haudio);





#endif
