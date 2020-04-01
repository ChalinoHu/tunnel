
//单片机头文件
#include "stm32f10x.h"

//网络设备
#include "esp8266.h"

//协议文件
#include "onenet.h"

//硬件驱动
#include "usart.h"
#include "delay.h"
#include "sht20.h"
#include "adc.h"

//C库
#include <string.h>
#include <stdio.h>


#define DEVID	"38732500"

#define APIKEY	"g3XgUeqVkcoJi=H9mkoznr9VNaE="


float lon = 118.650708;
float lat = 32.040000;



void OneNet_FillBuf(char *buf)
{
	
	char text[64];
	char buf1[256];
    

	
	memset(text, 0, sizeof(text));
	memset(buf1, 0, sizeof(buf1));
	
	strcpy(buf1, "{");
	
//	memset(text, 0, sizeof(text));
//	sprintf(text, "\"Temperature\":%0.2f,", sht20_info.tempreture);
//	strcat(buf1, text);
//	
//	memset(text, 0, sizeof(text));
//	sprintf(text, "\"Humidity\":%0.2f", sht20_info.humidity);
//	strcat(buf1, text);
//    
    
    
	memset(text, 0, sizeof(text));
	sprintf(text, "\"battery_value\":%0.2f,", get_battery());
	strcat(buf1, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"mok_value\":%0.2f", get_smoke());
	strcat(buf1, text);

//    memset(text, 0, sizeof(text));
//	sprintf(text, "\"GPS\":{\"lon\":%f,\"lat\":%f}", lon, lat);
//	strcat(buf1, text);


	
	strcat(buf1, "}");
    
    
	
	sprintf(buf, "POST /devices/%s/datapoints?type=3 HTTP/1.1\r\napi-key:%s\r\nHost:api.heclouds.com\r\n"
					"Content-Length:%d\r\n\r\n",
	
					DEVID, APIKEY, strlen(buf1));
					
	strcat(buf, buf1);
    
    

}

//==========================================================
//	函数名称：	OneNet_SendData
//
//	函数功能：	上传数据到平台
//
//	入口参数：	type：发送数据的格式
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OneNet_SendData(void)
{
	
	char buf[256];
	
	memset(buf, 0, sizeof(buf));
	
	OneNet_FillBuf(buf);									//封装数据流
	
	ESP8266_SendData((unsigned char *)buf, strlen(buf));	//上传数据
	
}

//==========================================================
//	函数名称：	OneNet_RevPro
//
//	函数功能：	平台返回数据检测
//
//	入口参数：	dataPtr：平台返回的数据
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OneNet_RevPro(unsigned char *dataPtr)
{

	if(strstr((char *)dataPtr, "CLOSED"))
	{
		UsartPrintf(USART_DEBUG, "TCP CLOSED\r\n");
	}
	else
	{
		//这里用来检测是否发送成功
		if(strstr((char *)dataPtr, "succ"))
		{
			UsartPrintf(USART_DEBUG, "Tips:	Send OK\r\n");
		}
		else
		{
			UsartPrintf(USART_DEBUG, "Tips:	Send Err\r\n");
		}
	}
	
	ESP8266_Clear();

}
