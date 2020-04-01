#ifndef __ADC_H
#define __ADC_H
#include"sys.h"
void  Adc_Init(void);
u16 Get_Adc(u8 ch);
u16 Get_Adc_Average(u8 ch,u8 times);
float Get_adc_value(float ReferenceValue,uint8_t ch,uint16_t times);

 float get_battery(void); 
 float get_smoke(void);
 void yanwu_judge(void);
  
  
#endif
