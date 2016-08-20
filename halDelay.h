

#ifndef __HALDELAY_H__
#define __HALDELAY_H__
#include "app/framework/include/af.h"




#define _500ms      500
#define _1s         1000
#define _100ms      100
#define _200ms      200
#define _2s         2000

int16u CommonGetDurationTime(int16u CommonBeforeTimer);
void __delay_ms(int32u n);



#endif