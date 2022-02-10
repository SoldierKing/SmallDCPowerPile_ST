#ifndef __DRV_RUNLED_H__
#define __DRV_RUNLED_H__
#include "Global.h"

//CC1输入检测IO定义
#define CC1In_Pin                  GPIO_PIN_13
#define CC1In_GPIO_Port            GPIOC
//电子锁IO定义
#define EleLock_Pin                GPIO_PIN_14
#define EleLock_GPIO_Port          GPIOC
#define EleUnlock_Pin              GPIO_PIN_15
#define EleUnlock_GPIO_Port        GPIOC
//绝缘检测辅电源使能
#define BoostEN_Pin                GPIO_PIN_2
#define BoostEN_GPIO_Port          GPIOA
//一键启停使能
#define StartON_Pin                GPIO_PIN_3
#define StartON_GPIO_Port          GPIOA
//一建启停按键
#define KeySta_Pin                 GPIO_PIN_4
#define KeySta_GPIO_Port           GPIOA
//电池充电标志位IO定义
#define VbatChg_Pin                GPIO_PIN_7
#define VbatChg_GPIO_Port          GPIOA
//电池反接标志位IO定义
#define VbatNeg_Pin                GPIO_PIN_4
#define VbatNeg_GPIO_Port          GPIOC
//运行指示灯IO定义
#define RunLed_Pin                 GPIO_PIN_5
#define RunLed_GPIO_Port           GPIOC
//BMS 辅源使能IO定义
#define BmsON_Pin                  GPIO_PIN_6
#define BmsON_GPIO_Port            GPIOC
//IMD PE 高压继电器IO定义
#define IMDLrly_Pin                GPIO_PIN_7
#define IMDLrly_GPIO_Port          GPIOC
#define IMDHrly_Pin                GPIO_PIN_8
#define IMDHrly_GPIO_Port          GPIOC
#define PeRly_Pin                  GPIO_PIN_9
#define PeRly_GPIO_Port            GPIOC
//漏电检测IO定义
#define LeakIn_Pin                 GPIO_PIN_8
#define LeakIn_GPIO_Port           GPIOA
//高压继电器IO定义
#define Lrly_Pin                   GPIO_PIN_10
#define Lrly_GPIO_Port             GPIOC
#define Hrly_Pin                   GPIO_PIN_11
#define Hrly_GPIO_Port             GPIOC
//电子锁反馈输入IO定义
#define LockSW_Pin                 GPIO_PIN_12
#define LockSW_GPIO_Port           GPIOC
//充电枪上按钮IO定义（实现一建停止充电）
#define OBSBtn_Pin                 GPIO_PIN_2
#define OBSBtn_GPIO_Port           GPIOD

extern void GPIOInit(void);

#endif
