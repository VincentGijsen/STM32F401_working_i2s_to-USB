# STM32F401 I2S source to USB audio

repo: https://github.com/VincentGijsen/STM32F401_working_i2s_to-USB

This is working working code to read I2S (44.1khz), and offer that to the USB-host in form of a soundcard.
framing is rudementary
closing in on buffered room is 'crude' implemented

## TODO's
* more elegant pid tuning from source (i2s bluetooth classic module) to usb clocking.

## Fixed
* fix the buffer mechanism to get rid of artifacts and slave the USB to the I2S Source (external)


## Versions:

* Works only with specific version of the STM4-HAL drivers. 
* Latest STM-HAL(1.28)


 
/vincent
