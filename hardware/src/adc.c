 #include "adc.h"
 #include "delay.h"
 //#include "beep.h"
	   

/*
************************************************************
*	�������ƣ�	Adc_Init
*
*	�������ܣ�	ADC��ʼ��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		��ͨͨ��5��6
************************************************************
*/
void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	
    //PA5��PA6 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڶ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

  
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������

}	


//���ADCֵ
//ch:ͨ��ֵ ADC_Channel_5  ADC_Channel_6

/*
************************************************************
*	�������ƣ�	Get_Adc
*
*	�������ܣ�	��ȡͨ��ADCת��ֵ
*
*	��ڲ�����	ch(   ADC_Channel_5   ����    ADC_Channel_6   ��
*
*	���ز�����	ת�����
*
*	˵����		
************************************************************
*/
u16 Get_Adc(u8 ch)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch,1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}


u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		DelayMs(5);
	}
	return temp_val/times;
} 	 



/*�ɼ����������ֵ*/
//�������ţ�PA5
float Get_adc_value(float ReferenceValue,uint8_t ch,uint16_t times)
{
   
    return Get_Adc_Average(ch,times)*ReferenceValue/4096 ;
    
}



/*�ɼ���صĵ�ѹ V*/
//�������ţ�PA6
float get_battery()
{  
    float BatteryValue;
    BatteryValue = Get_adc_value(4,ADC_Channel_6,5);   
    return BatteryValue;
}    




//�ɼ�����������Ũ�� ppm
float get_smoke()
      {    
        float AdTemp,SmokeValue;     
        AdTemp=Get_Adc_Average(ADC_Channel_5,5)/1024;       
        SmokeValue=AdTemp*9970/3.9+300;
    //      if(SmokeValue>5000) Fun_On();
        return SmokeValue;
      }


void yanwu_judge()
{
    if(get_smoke()>5000)
    {
        //  ������
        DelayXms(5);
        //   �ط���
    }
}




















