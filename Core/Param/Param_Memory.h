#ifndef  __PARAM_MEMORY_H__
#define  __PARAM_MEMORY_H__


#include "Global.h"



#define SpiFlashSectionSize         (0x1000)

#define FlashInitAddr               (0)//0K��ַ��ʼ �ܹ�4K

#define UserParamAddr               (FlashInitAddr+SpiFlashSectionSize)//4K��ַ��ʼ �ܹ�4K
#define UserParamBakAddr            (UserParamAddr+SpiFlashSectionSize)//8K��ַ��ʼ �ܹ�4K


extern void FlashTest(void);

#endif