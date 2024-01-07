
#ifndef INC_audioBuf_H_
#define INC_audioBuf_H_
#define BUFFER_SLOTS 24
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
#endif
