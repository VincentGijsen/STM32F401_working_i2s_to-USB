/*
 * audioBuf.c
 *
 *  Created on: Jan 7, 2024
 *      Author: vtjgi
 */
#include "main.h"
#include "cmsis_os.h"


#include "audioBuf.h"

volatile uint8_t usbPkgIdx;

volatile uint8_t usbPkgSendFlag;
volatile uint8_t dataReady;

volatile uint16_t i2SSampleIdx;
uint16_t i2SDataBuffer[BUFFER_SIZE];

int16_t *pInBuffPtr = &i2SDataBuffer[0];
volatile uint8_t usbData[USBSAMPLES][192];


volatile BUFFER_extra_typeDef bufferExtrainfo; //extra info for the audio buffer
volatile nextUsbFrame_typeDef nextAudioFrame; //holds next audio frame


uint8_t Buffer_isOverflowFit(uint16_t idx) {
	const uint8_t MAXUSBPKGSize = 194;
	if (idx + MAXUSBPKGSize >= AUDIO_TOTAL_BUF_SIZE /*usbd_audio.h*/) {
		return 1;
	}
	return 0;
}

BUFFER_CMD_typeDef Buffer_checkSize(USBD_AUDIO_HandleTypeDef *haudio,
		uint16_t position_ref) {

}

void Buffer_Init(){
	bufferExtrainfo.dataBuffered =0;
	bufferExtrainfo.rd_overflow=0;
	bufferExtrainfo.wr_overflow =0;
	bufferExtrainfo.playingStatus = PLAYING_INIT;
	bufferExtrainfo.max_size = AUDIO_TOTAL_BUF_SIZE;
}

static inline uint16_t min(uint16_t a, uint16_t b) {
    return a < b ? a : b;
}

BUFFER_CMD_typeDef Buffer_AddToBuffer(USBD_AUDIO_HandleTypeDef *haudio,
		int16_t *inData, uint16_t inDataLen) {

	uint16_t bytes = inDataLen *2;
//
	 if(bufferExtrainfo.max_size - bufferExtrainfo.dataBuffered < bytes){
	        return BUFFER_OVERFLOW;
	    }

	const uint16_t part1 = min(bytes, bufferExtrainfo.max_size -  haudio->wr_ptr);
	const uint16_t part2 = bytes - part1;

	uint16_t offsetIdx=0;
	const uint16_t halfPart1 = part1 / 2; //have to mange 16bits -> 8 bits, therefore half versions
	const uint16_t halfPart2 = part2 / 2;

	for (uint16_t it = 0; it < halfPart1; it++) {
		offsetIdx = it*2;
		haudio->buffer[haudio->wr_ptr +offsetIdx] = (uint8_t)(inData[it] & 0xff);;
		haudio->buffer[haudio->wr_ptr +offsetIdx + 1 ] = (uint8_t)((inData[it] >> 8)& 0xff);
	}

	//2nd part always starts at wr_ptr=0;
	if (part2 >0){
		haudio->wr_ptr=0;
		for (uint16_t it = 0; it < halfPart2; it++) {
			 offsetIdx = it*2;
			haudio->buffer[haudio->wr_ptr +offsetIdx ] = (uint8_t)(inData[it+halfPart1] & 0xff);;
			haudio->buffer[haudio->wr_ptr +offsetIdx + 1 ] = (uint8_t)((inData[it+halfPart1] >> 8)& 0xff);
		}
		//haudio->wr_ptr=part2;
		haudio->wr_ptr=part2;

	}
	else{
		//part1 only executed ( and < max buffer)
		haudio->wr_ptr =haudio->wr_ptr + part1;
	}
	//haudio->wr_ptr = ( haudio->wr_ptr + bytes) % bufferExtrainfo.max_size;
	bufferExtrainfo.dataBuffered += bytes;

	return BUFFER_OK;
}

/*
 * when called, generates the next audio frame with variable length
 */
BUFFER_CMD_typeDef Buffer_GetNextAudioFrame(USBD_AUDIO_HandleTypeDef *haudio){
	uint8_t nextFrameSize = 192;

	//used for buffer tuning

	const uint16_t LOWER_ROOM = (10* 192) + 20;
	const uint16_t UPPER_ROOM = AUDIO_TOTAL_BUF_SIZE - LOWER_ROOM;

	uint16_t packetsBuffered = 0;
	//uint8_t buffer_read_fragemented =0;
	//BUFFER_CMD_typeDef res = BUFFER_OK;
/*
 * we must measure lenght of buffered packages (distance between wr (future) and rd
 */
	if (bufferExtrainfo.dataBuffered < nextFrameSize){
		bufferExtrainfo.playingStatus = PLAYING_INIT;
				return BUFFER_EMPTY;
	}


	//logic for descision making
	const uint16_t halfBufferFilled = ( AUDIO_TOTAL_BUF_SIZE / 2) -2;

	switch(bufferExtrainfo.playingStatus)
	{
		case PLAYING_INIT:
			if(bufferExtrainfo.dataBuffered < halfBufferFilled){
					return BUFFER_FILLING;
				}
			else
			{
				bufferExtrainfo.playingStatus = PLAYING_RUNNING;
			}

		break;

		case PLAYING_RUNNING:

			if(bufferExtrainfo.dataBuffered  < LOWER_ROOM){
				nextFrameSize-=4;
				break;
				}
			if(bufferExtrainfo.dataBuffered  > UPPER_ROOM){
				nextFrameSize+=4;
				break;

			}
			break;

	}

	const uint16_t part1 = min(nextFrameSize, bufferExtrainfo.max_size -  haudio->rd_ptr);
	const uint16_t part2 = nextFrameSize - part1;

	//copy part 1
	memcpy((uint8_t*)&nextAudioFrame.data, (uint8_t*)&haudio->buffer[haudio->rd_ptr], part1);
	//copy part 2
	memcpy(((uint8_t*)((uint8_t*) nextAudioFrame.data ) + part1), (uint8_t*) haudio->buffer,part2);

	haudio->rd_ptr = (haudio->rd_ptr + nextFrameSize) % bufferExtrainfo.max_size;
	bufferExtrainfo.dataBuffered -= nextFrameSize;
	nextAudioFrame.len = nextFrameSize;

	return BUFFER_OK;

}

