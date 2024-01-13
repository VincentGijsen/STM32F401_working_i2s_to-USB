/*
 * log.h
 *
 *  Created on: 25 Apr 2021
 *      Author: vtjgi
 */

#ifndef INC_LOG_H_
#define INC_LOG_H_

#include "xprintf.h"

#define ENABLE_LOG
#define ENABLE_DEBUG
#define ENABLE_TRACE

#ifdef ENABLE_LOG
  #define LOG xprintf
#else
  #define LOG(...) ((void)0) //strip out PRINT instructions from code
#endif



#ifdef ENABLE_DEBUG
  #define DEBUG xprintf

#else
  #define DEBUG(...) ((void)0) //strip out PRINT instructions from code
#endif




#ifdef ENABLE_TRACE
  #define TRACE xprintf
#else
  #define TRACE(...) ((void)0) //strip out PRINT instructions from code
#endif


#endif /* INC_LOG_H_ */
