#ifndef __DRV_RUNLED_H__
#define __DRV_RUNLED_H__
#include "Global.h"

//CC1������IO����
#define CC1In_Pin                  GPIO_PIN_13
#define CC1In_GPIO_Port            GPIOC
//������IO����
#define EleLock_Pin                GPIO_PIN_14
#define EleLock_GPIO_Port          GPIOC
#define EleUnlock_Pin              GPIO_PIN_15
#define EleUnlock_GPIO_Port        GPIOC
//��Ե��⸨��Դʹ��
#define BoostEN_Pin                GPIO_PIN_2
#define BoostEN_GPIO_Port          GPIOA
//һ����ͣʹ��
#define StartON_Pin                GPIO_PIN_3
#define StartON_GPIO_Port          GPIOA
//һ����ͣ����
#define KeySta_Pin                 GPIO_PIN_4
#define KeySta_GPIO_Port           GPIOA
//��س���־λIO����
#define VbatChg_Pin                GPIO_PIN_7
#define VbatChg_GPIO_Port          GPIOA
//��ط��ӱ�־λIO����
#define VbatNeg_Pin                GPIO_PIN_4
#define VbatNeg_GPIO_Port          GPIOC
//����ָʾ��IO����
#define RunLed_Pin                 GPIO_PIN_5
#define RunLed_GPIO_Port           GPIOC
//BMS ��Դʹ��IO����
#define BmsON_Pin                  GPIO_PIN_6
#define BmsON_GPIO_Port            GPIOC
//IMD PE ��ѹ�̵���IO����
#define IMDLrly_Pin                GPIO_PIN_7
#define IMDLrly_GPIO_Port          GPIOC
#define IMDHrly_Pin                GPIO_PIN_8
#define IMDHrly_GPIO_Port          GPIOC
#define PeRly_Pin                  GPIO_PIN_9
#define PeRly_GPIO_Port            GPIOC
//©����IO����
#define LeakIn_Pin                 GPIO_PIN_8
#define LeakIn_GPIO_Port           GPIOA
//��ѹ�̵���IO����
#define Lrly_Pin                   GPIO_PIN_10
#define Lrly_GPIO_Port             GPIOC
#define Hrly_Pin                   GPIO_PIN_11
#define Hrly_GPIO_Port             GPIOC
//��������������IO����
#define LockSW_Pin                 GPIO_PIN_12
#define LockSW_GPIO_Port           GPIOC
//���ǹ�ϰ�ťIO���壨ʵ��һ��ֹͣ��磩
#define OBSBtn_Pin                 GPIO_PIN_2
#define OBSBtn_GPIO_Port           GPIOD

extern void GPIOInit(void);

#endif
