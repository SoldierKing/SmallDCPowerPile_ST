#ifndef  __PARAM_MEMORY_H__
#define  __PARAM_MEMORY_H__


#include "Global.h"



#define SpiFlashSectionSize         (0x1000)

#define FlashInitAddr               (0)//0K地址开始 总共4K

#define UserParamAddr               (FlashInitAddr+SpiFlashSectionSize)//4K地址开始 总共4K
#define UserParamBakAddr            (UserParamAddr+SpiFlashSectionSize)//8K地址开始 总共4K


extern void FlashTest(void);

#endif