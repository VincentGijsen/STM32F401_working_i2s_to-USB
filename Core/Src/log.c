/*
 * log.c
 *
 *  Created on: 12 Apr 2021
 *      Author: vtjgi
 */


#include "xprintf.h"
#include <stdarg.h>
void log(const char *fmt, ...){
	va_list arp;


		va_start(arp, fmt);
		xvfprintf(xfunc_output, fmt, arp);
		va_end(arp);
}
