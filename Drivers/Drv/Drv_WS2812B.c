
#include "Drv_WS2812B.h"
#include <stdint.h>
#include <core_cm3.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <string.h>
#include "cmsis_os.h"

//函数声明
void WS2812B_Lighting(Color_TypeDef *pColor,u8 num);



//晶振72M,每个nop是13.9ns 
#define DELAY_13_8nS	     __NOP()  	
#define DELAY_27_8nS	     DELAY_13_8nS;DELAY_13_8nS;
#define DELAY_96_6nS	     DELAY_13_8nS;DELAY_13_8nS;DELAY_13_8nS;DELAY_13_8nS;DELAY_13_8nS;DELAY_13_8nS;DELAY_13_8nS //7个nop是96.6ns
#define DELAY_866nS          DELAY_13_8nS;DELAY_96_6nS;DELAY_96_6nS;DELAY_96_6nS;DELAY_96_6nS;DELAY_96_6nS;DELAY_96_6nS;DELAY_96_6nS;DELAY_96_6nS;
//实测1028-973=56ns
#define DELAY_966nS          DELAY_96_6nS;DELAY_96_6nS;DELAY_96_6nS;DELAY_96_6nS;DELAY_96_6nS;DELAY_96_6nS;DELAY_96_6nS;DELAY_96_6nS;DELAY_96_6nS;DELAY_96_6nS


void Delay_900nS(void)//进入函数时间约为310ns
{
  u8 i=10;
  while(i--)
  DELAY_966nS;
  DELAY_13_8nS;	
}

void Delay_150uS(void)
{
  
  u8 i=155;
  while(i--)
  Delay_900nS();
		
}
void Delay_13nS(void) 
{
  
  u8 i=1;
  while(i--)
  DELAY_13_8nS;
  
}
/*---------------------------------------------------------------------------
@Function   :WS2812B_Init
@Description:初始化
@Input      :无
@Retrun     :无
@Others     :
----------------------------------------------------------------------------*/
void WS2812B_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  GPIO_InitStruct.Pin = WS2812B_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(WS2812B_PORT, &GPIO_InitStruct);
  
}

/*---------------------------------------------------------------------------
@Function   :WS2812B_Reset
@Description:复位
@Input      :无
@Retrun     :无
@Others     :
----------------------------------------------------------------------------*/
void WS2812B_Reset(void)          //复位函数
{
  WS2812B_Hi();
  DELAY_966nS;
  WS2812B_Low();
}

/*---------------------------------------------------------------------------
@Function   :WS2812B_WriteByte
@Description:写一个字节
@Input      :无
@Retrun     :无
@Others     :
----------------------------------------------------------------------------*/
void WS2812B_WriteByte(uint8_t dat)
{
  u8 i;
  for (i=0;i<8;i++)
  {
    //先发送高位
    if (dat & 0x80) //1
    {
      WS2812B_Hi();
      DELAY_27_8nS;
    
      DELAY_13_8nS;
      DELAY_866nS; //T1H 0.9-1.0us
      DELAY_96_6nS;//T1L 1.1-30us
      DELAY_13_8nS;
      WS2812B_Low();	
      DELAY_966nS;
      DELAY_96_6nS;//T1L 1.1-30us
      DELAY_13_8nS;
      DELAY_96_6nS;//T1L 1.1-30us
      DELAY_13_8nS;
    }
    else	//0
    {

      WS2812B_Hi();
    
      DELAY_96_6nS;
      DELAY_96_6nS;//T0H 0.28-0.35us
      DELAY_96_6nS;//T1L 1.1-30us
      WS2812B_Low();
      DELAY_966nS;
      DELAY_966nS;//T0L 1.72-30us
    }

    dat<<=1;
  }
}


/*---------------------------------------------------------------------------
@Function   :WS2812B_WriteColor
@Description:写入1个24bit颜色数据
@Input      :无
@Retrun     :无
@Others     :
----------------------------------------------------------------------------*/
void WS2812B_WriteColor(Color_TypeDef *pColor)
{
     
  WS2812B_WriteByte(pColor->G);
  WS2812B_WriteByte(pColor->R);
  WS2812B_WriteByte(pColor->B);
}



void WS2812B_Lighting(Color_TypeDef *pColor,u8 num)
{
  u8 i;
  for(i=0;i<num;i++)
  {
    
    WS2812B_WriteColor(pColor);
    WS2812B_WriteColor(pColor);
    WS2812B_WriteColor(pColor);
        
  }
}

//============================================================================
void Copy_Color(Color_TypeDef *pDst,Color_TypeDef *pScr)
{
  pDst->R = pScr->R;
  pDst->G = pScr->G;
  pDst->B = pScr->B;
}


void WS2812B_White(void)
{
 Color_TypeDef temp;
  temp.B = 0xFF;
  temp.R = 0xFF;
  temp.G = 0xFF;
  WS2812B_Lighting(&temp,1);
}


void WS2812B_Blue(void)
{
 Color_TypeDef temp;
  temp.B = 178;
  temp.R = 26;
  temp.G = 178;
  WS2812B_Lighting(&temp,1);
}

void WS2812B_Green(void)
{
 Color_TypeDef temp;
  temp.B = 0x00;
  temp.R = 0x00;
  temp.G = 0xFF;
  WS2812B_Lighting(&temp,1);
}
void WS2812B_Red(void)
{
 Color_TypeDef temp;
  temp.B = 0x00;
  temp.R = 0xFF;
  temp.G = 0x00;
  WS2812B_Lighting(&temp,1);
}
void WS2812B_Black(void)
{
  Color_TypeDef temp;
  temp.B = 0x00;
  temp.R = 0x00;
  temp.G = 0x00;
  WS2812B_Lighting(&temp,1);
}
void WS2812B_Dispaly(EnumDisplay Color)
{
 if(Color == Blue) 
  WS2812B_Blue();
 else if(Color == Green)
  WS2812B_Green();
 else if(Color == Red)
  WS2812B_Red();
}
void WS2812B_Blinking(EnumDisplay Color)
{
if(Color == Blue)
{
 osDelay(100);
 WS2812B_Blue();
 osDelay(100);
 WS2812B_Black();
}
if(Color == Green)
{
 osDelay(100);
 WS2812B_Green();
 osDelay(100);
 WS2812B_Black();
}
if(Color == Red)
{
 osDelay(100);
 WS2812B_Red();
 osDelay(100);
 WS2812B_Black();
}
}


void LedWs2812Breathe(pWS2812B_Lighting WS2812B_Lighting,WS2812BCfgStruct *pWS2812BCfgStruct)
{

  if (pWS2812BCfgStruct->BreatheStep == BreatheUp)
  {
    //if (pWS2812BCfgStruct->UpTickB == 0)
    	//pWS2812BCfgStruct->UpTickB = GetSystemTick();
    //else
    //{
       
      if (GetSystemTick() - pWS2812BCfgStruct->UpTickB > pWS2812BCfgStruct->UpTime/
  		pWS2812BCfgStruct->RGB.B)
      {
      
        pWS2812BCfgStruct->UpTickB = GetSystemTick();
        if (pWS2812BCfgStruct->RGBTemp.B < pWS2812BCfgStruct->RGB.B)
  	     pWS2812BCfgStruct->RGBTemp.B++;
  	  } 
    //}
  
    //if (pWS2812BCfgStruct->UpTickG== 0)
    	//pWS2812BCfgStruct->UpTickG= GetSystemTick();
    //else
    //{
       
      if (GetSystemTick() - pWS2812BCfgStruct->UpTickG > pWS2812BCfgStruct->UpTime/
  		pWS2812BCfgStruct->RGB.G)
      {
      
        pWS2812BCfgStruct->UpTickG = GetSystemTick();
        if (pWS2812BCfgStruct->RGBTemp.G < pWS2812BCfgStruct->RGB.G)
  	     pWS2812BCfgStruct->RGBTemp.G++;
  	  } 
    //}
  
    //if (pWS2812BCfgStruct->UpTickR== 0)
    	//pWS2812BCfgStruct->UpTickR= GetSystemTick();
    //else
    //{
       
      if (GetSystemTick() - pWS2812BCfgStruct->UpTickR > pWS2812BCfgStruct->UpTime/
  		pWS2812BCfgStruct->RGB.R)
      {
      
        pWS2812BCfgStruct->UpTickR = GetSystemTick();
        if (pWS2812BCfgStruct->RGBTemp.R < pWS2812BCfgStruct->RGB.R)
  	     pWS2812BCfgStruct->RGBTemp.R++;

		//WS2812B_Lighting(&pWS2812BCfgStruct->RGBTemp,1);
  	  } 
    //}

	
	if (pWS2812BCfgStruct->RGBTemp.G >= pWS2812BCfgStruct->RGB.G
		&& pWS2812BCfgStruct->RGBTemp.B >= pWS2812BCfgStruct->RGB.B
		&& pWS2812BCfgStruct->RGBTemp.R >= pWS2812BCfgStruct->RGB.R)
	{
      //pWS2812BCfgStruct->UpTickB = 0;
	  //pWS2812BCfgStruct->UpTickR = 0;
	  //pWS2812BCfgStruct->UpTickG = 0;
	  pWS2812BCfgStruct->BreatheStep = BreatheKeepOn;
      
	}
  }
  else if (pWS2812BCfgStruct->BreatheStep == BreatheKeepOn)
  {
    if (pWS2812BCfgStruct->KeepOnTick == 0) pWS2812BCfgStruct->KeepOnTick = GetSystemTick();
	else if (GetSystemTick() - pWS2812BCfgStruct->KeepOnTick > pWS2812BCfgStruct->KeepOnTime)
	{
      pWS2812BCfgStruct->KeepOnTick = 0;
	  pWS2812BCfgStruct->BreatheStep = BreatheDown;
	}
	//WS2812B_Lighting(&pWS2812BCfgStruct->RGBTemp,1);
  }
  else if (pWS2812BCfgStruct->BreatheStep == BreatheDown)
  {

	//if (pWS2812BCfgStruct->DownTickB == 0)
    	//pWS2812BCfgStruct->DownTickB = GetSystemTick();
    //else
    //{
       
      if (GetSystemTick() - pWS2812BCfgStruct->DownTickB > pWS2812BCfgStruct->DownTime/
  		pWS2812BCfgStruct->RGB.B)
      {
      
        pWS2812BCfgStruct->DownTickB = GetSystemTick();
        if (pWS2812BCfgStruct->RGBTemp.B)
  	     pWS2812BCfgStruct->RGBTemp.B--;
  	  } 
    //}
  
    //if (pWS2812BCfgStruct->DownTickG== 0)
    	//pWS2812BCfgStruct->DownTickG= GetSystemTick();
    //else
    //{
       
      if (GetSystemTick() - pWS2812BCfgStruct->DownTickG > pWS2812BCfgStruct->DownTime/
  		pWS2812BCfgStruct->RGB.G)
      {
      
        pWS2812BCfgStruct->DownTickG = GetSystemTick();
        if (pWS2812BCfgStruct->RGBTemp.G)
  	     pWS2812BCfgStruct->RGBTemp.G--;
  	  } 
    //}
  
    //if (pWS2812BCfgStruct->DownTickR== 0)
    	//pWS2812BCfgStruct->DownTickR= GetSystemTick();
    //else
    //{
       
      if (GetSystemTick() - pWS2812BCfgStruct->DownTickR > pWS2812BCfgStruct->DownTime/
  		pWS2812BCfgStruct->RGB.R)
      {
      
        pWS2812BCfgStruct->DownTickR = GetSystemTick();
        if (pWS2812BCfgStruct->RGBTemp.R)
  	     pWS2812BCfgStruct->RGBTemp.R--;

		//WS2812B_Lighting(&pWS2812BCfgStruct->RGBTemp,1);
  	  } 
    //}
	if (pWS2812BCfgStruct->RGBTemp.G == 0
		&& pWS2812BCfgStruct->RGBTemp.B == 0
		&& pWS2812BCfgStruct->RGBTemp.R == 0)
	{
      //pWS2812BCfgStruct->DownTickB = 0;
	  //pWS2812BCfgStruct->DownTickR = 0;
	  //pWS2812BCfgStruct->DownTickG = 0;
	  pWS2812BCfgStruct->BreatheStep = BreatheKeepOff;
      
	}
  }
  else if (pWS2812BCfgStruct->BreatheStep == BreatheKeepOff)
  {

	pWS2812BCfgStruct->RGBTemp.G = 0;
	pWS2812BCfgStruct->RGBTemp.B = 0;
	pWS2812BCfgStruct->RGBTemp.R = 0;
	
    if (pWS2812BCfgStruct->KeepOffTick == 0) pWS2812BCfgStruct->KeepOffTick = GetSystemTick();
	else if (GetSystemTick() - pWS2812BCfgStruct->KeepOffTick > pWS2812BCfgStruct->KeepOffTime)
	{
      pWS2812BCfgStruct->KeepOffTick = 0;
	  pWS2812BCfgStruct->BreatheStep = BreatheUp;
	}
	//WS2812B_Lighting(&pWS2812BCfgStruct->RGBTemp,1);
  }

  
  WS2812B_Lighting(&pWS2812BCfgStruct->RGBTemp,1);
   
}


/*
u32 UpTick = 0;
u32 DownTick = 0;
u8 i=0;
u8 b=0;

u8 a=178;

u8 c=0;
u8 d=0;
u8 e=26;
u8 Breathingflag=0;

void WS2812B_Blue_Breathing(u16 UpTime,u16 DownTime )
{
 if((GetSystemTick()-UpTick>(UpTime/178))&&(Breathingflag==0))
    {
     if(a==0)
     	{
     	a=178;
         c=26;
     	osDelay(100);
     	}
     Color_TypeDef temp;
     temp.B = i;
     temp.R = b;
     temp.G = i;
     WS2812B_Lighting(&temp,1);
     i++;
     if((i%6==1)&&(b<26))
     b++;	
     UpTick=GetSystemTick();
     if(i>178)
     Breathingflag=1;	
    }
 
if((GetSystemTick()-DownTick>(UpTime/178))&&(Breathingflag==1))
{
     if(i>178)
     {
      i=0;
      b=0;
      osDelay(500);
     }
      Color_TypeDef temp;
      temp.B = a;
      temp.R = c;
      temp.G = a;
      WS2812B_Lighting(&temp,1);
      if(a>0)
      a--;
      if((a%6==1)&&(c>0))
      c--;
      DownTick=GetSystemTick();
      if(a==0)
      Breathingflag=0;
}
	
}
void LedEffectSwitch(EnumLedSwitchStatus Sta,EnumDisplay Color)
{

  if(Sta  == Lighting)
    WS2812B_Dispaly(Color);
  else if(Sta == Blinking)
    WS2812B_Blinking(Color);
  else if(Sta == Breathing)
    WS2812B_Blue_Breathing(1000,2000);
 
}
*/
