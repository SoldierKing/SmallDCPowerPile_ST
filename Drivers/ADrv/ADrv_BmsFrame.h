#ifndef  __ADRV_BMSFRAME_H__
#define  __ADRV_BMSFRAME_H__

#include "Global.h"
#include "Param_UserConfig.h"

/*
���ı��� ����	��������	PGN	PGN��Hex��	����Ȩ	���ݳ���byte	��������	Դ��ַ-Ŀ�ĵ�ַ
CHM	��������	9728	002600H	6	3	250ms	����-BMS
BHM	��������	9984	002700H	6	2	250ms	BMS-����
CRM	������ʶ	256	000100H	6	8	250ms	����-BMS
BRM	BMS�ͳ�����ʶ����	512	000200H	7	41	250ms	BMS-����

BCP	�������س�����	1536	000600H	7	13	500ms	BMS-����
CTS	��������ʱ��ͬ����Ϣ	1792	000700H	6	7	500ms	����-BMS
CML	��������������	2048	000800H	6	8	250ms	����-BMS
BRO	��س��׼������״̬	2304	000900H	4	1	250ms	BMS-����
CRO	�������׼������״̬	2560	000A00H	4	1	250ms	����-BMS

BCL	��س������	4096	001000H	6	5	50ms	BMS-����
BCS	��س����״̬	4352	001100H	7	9	250ms	BMS-����
CCS	�������״̬	4608	001200H	6	6	50ms	����-BMS
BSM	��������״̬��Ϣ	4864	001400H	6	7	250ms	BMS-����
BMV	���嶯�����ص�ѹ	5376	001500H	7	����	10s	BMS-����
BMT	���������¶�	5632	001600H	7	����	10s	BMS-����
BSP	��������Ԥ������	5888	001700H	7	����	10s	BMS-����
BST	BMS��ֹ���	6400	001900H	4	4	10ms	BMS-����
CST	������ֹ���	6656	001A00H	4	4	10ms	����-BMS

BSD	BMSͳ������	7168	001C00H	6	7	250ms	BMS-����
CSD	����ͳ������	7424	001D00H	6	8	250ms	����-BMS

BEM	BMS������	7680	001E00H	2	4	250ms	BMS-����
CEM	����������	7936	001F00H	2	4	250ms	����-BMS

DM1	��ǰ������	8192	002000H	6	����	�¼���Ӧ
DM2	��ʷ������	8448	002100H	6	����	�¼���Ӧ
DM3	���׼������	8704	002200H	6	2�ֽ�	�¼���Ӧ
DM4	��ǰ����������/��λ	8906	002300H	6	0	�¼���Ӧ
DM5	��ʷ����������/��λ	9216	002400H	6	0	�¼���Ӧ
DM6	ͣ֡����	9472	002500H	6	����	�¼���Ӧ


*/

typedef enum
{
  
  V2L = 0x000100, //�ŵ�����(���׮---����)
  RES = 0x004100, //�ŵ�������Ӧ(����---���׮)
  NTM = 0x003C00, //NTM �����汾Э��
  EVM = 0x003D00, //EVM �����汾Э��
  CHM = 0x002600,// CHM	��������	9728	002600H	6	3	250ms	����-BMS
  BHM = 0x002700,// BHM	��������	9984	002700H	6	2	250ms	BMS-����
  CRM = 0x000100,// CRM	������ʶ	256	000100H	6	8	250ms	����-BMS
  BRM = 0x000200,// BRM	BMS�ͳ�����ʶ����	512	000200H	7	41	250ms	BMS-����

  BCP = 0x000600,// BCP	�������س�����	1536	000600H	7	13	500ms	BMS-����
  CTS = 0x000700,// CTS	��������ʱ��ͬ����Ϣ	1792	000700H	6	7	500ms	����-BMS
  CML = 0x000800,// CML	��������������	2048	000800H	6	8	250ms	����-BMS
  BRO = 0x000900,// BRO	��س��׼������״̬	2304	000900H	4	1	250ms	BMS-����
  CRO = 0x000A00,// CRO	�������׼������״̬	2560	000A00H	4	1	250ms	����-BMS

  BCL = 0x001000,// BCL	��س������	4096	001000H	6	5	50ms	BMS-����
  BCS = 0x001100,// BCS	��س����״̬	4352	001100H	7	9	250ms	BMS-����
  CCS = 0x001200,// CCS	�������״̬	4608	001200H	6	6	50ms	����-BMS
  BSM = 0x001300,// BSM	��������״̬��Ϣ	4864	001400H	6	7	250ms	BMS-����
  BMV = 0x001500,// BMV	���嶯�����ص�ѹ	5376	001500H	7	����	10s	BMS-����
  BMT = 0x001600,// BMT	���������¶�	5632	001600H	7	����	10s	BMS-����
  BSP = 0x001700,// BSP	��������Ԥ������	5888	001700H	7	����	10s	BMS-����
  BST = 0x001900,// BST	BMS��ֹ���	6400	001900H	4	4	10ms	BMS-����
  CST = 0x001A00,// CST	������ֹ���	6656	001A00H	4	4	10ms	����-BMS

  BSD = 0x001C00,// BSD	BMSͳ������	7168	001C00H	6	7	250ms	BMS-����
  CSD = 0x001D00,// CSD	����ͳ������	7424	001D00H	6	8	250ms	����-BMS

  BEM = 0x001E00,// BEM	BMS������	7680	001E00H	2	4	250ms	BMS-����
  CEM = 0x001F00,// CEM	����������	7936	001F00H	2	4	250ms	����-BMS

  DM1 = 0x002000,// DM1	��ǰ������	8192	002000H	6	����	�¼���Ӧ
  DM2 = 0x002100,// DM2	��ʷ������	8448	002100H	6	����	�¼���Ӧ
  DM3 = 0x002200,// DM3	���׼������	8704	002200H	6	2�ֽ�	�¼���Ӧ
  DM4 = 0x002300,// DM4	��ǰ����������/��λ	8906	002300H	6	0	�¼���Ӧ
  DM5 = 0x002400,// DM5	��ʷ����������/��λ	9216	002400H	6	0	�¼���Ӧ
  DM6 = 0x002500,// DM6	ͣ֡����	9472	002500H	6	����	�¼���Ӧ

  REQ  = 0x00EA00,//����
  ACK  = 0x00E800,//ȷ��
  TPCM = 0x00EC00,//����Э��--���ӹ���
  TPDT = 0x00EB00,//����Э��--���ݴ���
  
}EnumBmsCommPgn;

#define Card1939Addr    (0xF4)
#define Charger1939Addr (0x56)

#define MaxDataSize1939  (512)

typedef struct
{
  u8  Flag;
  u8  FrameMaxNum;
  u8  RecvFrameCnt;
  u8  RecvBuff[MaxDataSize1939];
  u16 RecvDataCnt;
  u16 FrameDataLen;
  u32 Pgn;
}StructTransProt;

typedef union
{
  u32 Mult;
  struct
  {
    u32 SA      : 8;
    u32 DA_PS   : 8;
    u32 PF      : 8;
    u32 DP      : 1;
    u32 R       : 1;
    u32 Prio    : 3;
    u32 Reserve : 3;
  }Disp;
}Union1939Id;

typedef struct
{
  u8 Prio;
  u8 DP;
  u8 PF;
  u8 DA;
  u8 SA;
  u8 Len;
  u8 Data[8];
  u32 Pgn;
}StructPgnRecv;

typedef struct
{
  u8 Len;
  u8 Data[8];
  Union1939Id Id;
}StructChargeSend;
typedef enum
{
  Ver1_1_0 = 0,	
  MaxVer2_0_0,
  MaxVer,
}SupportedVerEnum;

typedef struct
{
  u8 Result;
  u32 VerNum;
  u8 VerisOK;   //0--��ʼֵ�����汾Э�̣�1---�汾�Ŷ��ϣ�2----�汾�ŵ�����Ͱ汾
  SupportedVerEnum svEnum;
}StructNTM;


typedef union
{

  u8 Mult;
  struct
  {
  
    u8 Undef : 3;
    u8 V2lDisChargeSta : 3;
    u8 Vehlocstatusresp: 2;
  }Value;
  
}UnionV2lValue;

typedef struct
{
  u8 Valid;

  UnionV2lValue V2LDischargeReq;

  u32 RecvTick;//���Ľ���Tick
}StructRES;


typedef struct
{
  u8 Valid;

  u16 ImdMaxVolt;

  u32 RecvTick;//���Ľ���Tick
}StructBHM;

typedef struct
{
  u8  Valid;
  struct
  {
    u32 L : 8;
    u32 M : 8;
    u32 H : 8;
  }CommVer;//3�ֽ� BMSͨ��Э��汾�ţ�����׼�涨��ǰ�汾ΪV1.0����ʾΪ��byte3��byte2��0001H��byte1��00H ������
  u8  BatType;//1�ֽ� ������ͣ�01H��Ǧ���أ�02H�������أ�03H��������﮵�أ�04H������﮵�أ�05H������﮵�أ�06H����Ԫ���ϵ�أ�07H���ۺ�������ӵ�أ�08H������﮵�أ�FFH��������� ������
  u16 BatRateCap;//2�ֽ� ������������ϵͳ�����/Ah��0.1Ah/λ��0Ahƫ���������ݷ�Χ��0��1000Ah ������
  u16 BatRateVolt;//2�ֽ� ������������ϵͳ��ܵ�ѹ/V��0.1V/λ��0Vƫ���������ݷ�Χ��0��750V ������
  u8  Manufacturer[4];//4�ֽ� ��������������ƣ���׼ASCII�� ��ѡ��
  u8  ProduceNum[4];//4�ֽ� �������ţ�Ԥ�����ɳ������ж��� ��ѡ��
  u8  ProduceYear;//1�ֽ� ������������ڣ��꣬1��/λ��1985��ƫ���������ݷ�Χ��1985��2235�� ��ѡ��
  u8  ProduceMon;//1�ֽ� ������������ڣ��£�1��/λ��0��ƫ���������ݷ�Χ��1��12�� ��ѡ��
  u8  ProduceDay;//1�ֽ� ������������ڣ��գ�1��/λ��0��ƫ���������ݷ�Χ��1��31�� ��ѡ��
  u32 ChgCount;//3�ֽ� ������������1��/λ��0��ƫ��������BMSͳ��Ϊ׼ ��ѡ��
  u8  BatOwn;//1�ֽ� ������Ȩ��ʶ��<0>��=���ޣ�<1>��=�����У���ѡ��
  u8  Reserve;//������ ��ѡ��
  u8  Vin[17];//17�ֽ� ����ʶ���루VIN����ѡ��
  u8  BmsVer[8];

  u32 RecvTick;//���Ľ���Tick
}StructBRM;

typedef struct
{
  u8  Valid;

  u16 PerBatMaxVolt; //���嶯����������������ѹ ���ݷֱ��ʣ�0.01 V/λ��0 Vƫ������ ���ݷ�Χ��0~24 V��������
  s16 ChgMaxCurrt;//������������ ���ݷֱ��ʣ�0.1 A/λ��-400 Aƫ������ ���ݷ�Χ��-400 A~0 A��������
  u16 BatEnergy;//�������ر������ ���ݷֱ��ʣ�0.1 kWh/λ��0 kWhƫ������ ���ݷ�Χ��0~1000 kWh��������
  u16 ChgMaxVolt;//����������ܵ�ѹ ���ݷֱ��ʣ�0.1 V/λ��0 Vƫ���������ݷ�Χ��0~750 V��
  u8  MaxTemp;//��������������¶� ���ݷֱ��ʣ�1oC/λ��-50 oCƫ���������ݷ�Χ��-50 oC ~+200 oC ��
  u16 StartSoc;//�����������غɵ�״̬��SOC�����ݷֱ��ʣ�0.1%/λ��0%ƫ���������ݷ�Χ��0~100%��
  u16 StartVolt;//�����������ص�ǰ��ص�ѹ ���ݷֱ��ʣ�0.1 V/λ��0 Vƫ���������ݷ�Χ��0~750 V��

  u32 RecvTick;//���Ľ���Tick
}StructBCP;

typedef struct
{
  u8 Valid;

  u8 Status;//BMS�Ƿ���׼���ã�<0x00 >��=BMSδ���ó��׼��) ; <0xAA >��=BMS��ɳ��׼����<0xFF>��=��Ч)

  u32 RecvTick;
}StructBRO;

typedef struct
{
  u8  Valid;

  u16 NeedVolt; //��ѹ���� ���ݷֱ��ʣ�0.1 V/λ��0 Vƫ���������ݷ�Χ��0~750 V��������
  u16 NeedCurrt;//�������� ���ݷֱ��ʣ�0.1 A/λ��-400 Aƫ������������
  u8  ChgType;//���ģʽ��0x01����ѹ��磻0x02��������磩

  u32 RecvTick;//���Ľ���Tick
}StructBCL;

typedef union
{
  u16 Mult;
  struct
  {
    u16 Volt  : 12;//1-12λ�����嶯�����ص�ѹ�����ݷֱ��ʣ�0.01 V/λ��0 Vƫ���������ݷ�Χ��0~24 V��
    u16 Group : 4;//13-16λ����ط���ţ����ݷֱ��ʣ�1/λ��1ƫ���������ݷ�Χ��1~16��
  }Value;
}UnionBatVoltAndGroup;
typedef struct
{
  u8  Valid;

  u16 ChgVolt; //����ѹ����ֵ ���ݷֱ��ʣ�0.1 V/λ��0 Vƫ���������ݷ�Χ��0~750 V��������
  u16 ChgCurrt;//����������ֵ ���ݷֱ��ʣ�0.1 A/λ��-400 Aƫ������������
  UnionBatVoltAndGroup BatVoltAndGroup;//��ߵ��嶯�����ص�ѹ������� 1-12λ����ߵ��嶯�����ص�ѹ�����ݷֱ��ʣ�0.01 V/λ��0 Vƫ���������ݷ�Χ��0~24 V��13-16λ����ߵ��嶯�����ص�ѹ������ţ����ݷֱ��ʣ�1/λ��0ƫ���������ݷ�Χ��0~15��������
  u8  CurSoc;//��ǰ�ɵ�״̬SOC ���ݷֱ��ʣ�1%/λ��0%ƫ���������ݷ�Χ��0~100%��
  u16 RemainMin;//����ʣ����ʱ�䣬��BMS��ʵ�ʵ���Ϊ׼���в����ʣ��ʱ�䳬��600 minʱ����600 min���͡����ݷֱ��ʣ�1 min/λ��0 minƫ���������ݷ�Χ��0~600 min��

  u32 RecvTick;//���Ľ���Tick
}StructBCS;

typedef union
{
  u8 Mult;
  struct
  {
    u8 Volt : 2;//���嶯�����ص�ѹ����/����(<00>��=����; <01>��=����; <10>��=����) ������
    u8 Soc  : 2;//�����������غɵ�״̬SOC����/����(<00>��=����; <01>��=����; <10>��=����) ������
    u8 Currt: 2;//�������س�������(<00>��=����; <01>��=����; <10>��=������״̬) ������
    u8 Temp : 2;//���������¶ȹ���(<00>��=����; <01>��=����; <10>��=������״̬) ������
  }Value;
}UnionBatStatus;
typedef union
{
  u8 Mult;
  struct
  {
    u8 Imd : 2;//�������ؾ�Ե״̬(<00>��=����; <01>��=������; <10>��=������״̬) ������
    u8 Connect  : 2;//�����������������������״̬(<00>��=����; <01>��=������; <10>��=������״̬) ������
    u8 ChgEnable : 2;//�������(<00>��=��ֹ; <01>��=����; ) ������
  }Value;
}UnionChargeStatus;
typedef struct
{
  u8  Valid;
  u8  HighTempBatNum; //��ߵ��嶯�����ص�ѹ���ڱ�� ���ݷֱ��ʣ�1/λ��1ƫ���������ݷ�Χ��1~256��������
  u8  HighTemp;//��߶��������¶� ���ݷֱ��ʣ�1oC/λ��-50 oCƫ���������ݷ�Χ��-50 oC ~+200 oC��������
  u8  HighTempPointNum; //����¶ȼ����� ���ݷֱ��ʣ�1/λ��1ƫ���������ݷ�Χ��1~128��������
  u8  LowTemp;//��Ͷ��������¶� ���ݷֱ��ʣ�1oC/λ��-50 oCƫ���������ݷ�Χ��-50 oC ~+200 oC��������
  u8  LowTempPointNum;//����¶ȼ����� ���ݷֱ��ʣ�1/λ��1ƫ���������ݷ�Χ��1~128��������
  UnionBatStatus BatSta;//
  UnionChargeStatus ChgSta;//

  u32 RecvTick;//���Ľ���Tick
}StructBSM;

typedef struct
{
  u8  Valid;

  UnionBatVoltAndGroup BatVoltAndGroup[256];//�ֱ��Ӧ#1~#256���嶯�����ص�ѹ

  u32 RecvTick;//���Ľ���Tick
}StructBMV;

typedef struct
{
  u8  Valid;

  u8  Temp[128];//1~128��������¶� ���ݷֱ��ʣ�1oC/λ��-50 oCƫ���������ݷ�Χ��-50 oC ~+200 oC����ѡ��

  u32 RecvTick;//���Ľ���Tick
}StructBMT;

typedef struct
{
  u8  Valid;

  u8  Bsp[16];//��������Ԥ���ֶ�16

  u32 RecvTick;//���Ľ���Tick
}StructBSP;

typedef union
{
  u8 Mult;
  struct
  {
    u8 NeedSoc : 2;//�ﵽ�������SOCĿ��ֵ <00>��=δ�ﵽ����SOCĿ��ֵ��<01>��=�ﵽ����SOCĿ��ֵ��<10>��=������״̬��������
    u8 SetVolt  : 2;//�ﵽ�ܵ�ѹ���趨ֵ <00>��=δ�ﵽ�ܵ�ѹ�趨ֵ��<01>��=�ﵽ�ܵ�ѹ�趨ֵ��<10>��=������״̬��������
    u8 PerSetVolt: 2;//�ﵽ�����ѹ���趨ֵ <00>��=δ�ﵽ�����ѹ�趨ֵ��<01>��=�ﵽ�����ѹ�趨ֵ��<10>��=������״̬��������
    u8 ChgStop : 2;//����������ֹ <00>��=������<01>��=������ֹ(�յ�CST֡)��<10>��=������״̬��������
  }Value;
}UnionBmsStopReason;
typedef union
{
  u16 Mult;
  struct
  {
    u16 Imd : 2;//��Ե���� <00>��=������<01>��=���ϣ�<10>��=������״̬��
    u16 OutletTemp  : 2;//<00>��=������<01>��=���ϣ�<10>��=������״̬��
    u16 ComponentTemp: 2;//BMSԪ�����������������<00>��=������<01>��=���ϣ�<10>��=������״̬��
    u16 ChgGun : 2;//������������� <00>��=���������������<01>��=������������ϣ�<10>��=������״̬��
    u16 BatPackTemp : 2;//������¶ȹ��߹��� <00>��=������¶�������<01>��=������¶ȹ��ߣ�<10>��=������״̬��
    u16 HighRelay : 2;//��ѹ�̵�������
    u16 DetectionPoint2 : 2;//����2��ѹ����
    u16 Other : 2;//�������� <00>��=������<01>��=���ϣ�<10>��=������״̬��
  }Value;
}UnionBmsStopTruble;
typedef union
{
  u8 Mult;
  struct
  {
    u8 Currt : 2;//�������� <00>��=����������<01>��=������������ֵ��<10>��=������״̬��
    u8 Volt  : 2;//��ѹ�쳣 <00>��=������<01>��=��ѹ�쳣��<10>��=������״̬��
  }Value;
}UnionBmsStopFault;
typedef struct
{
  u8 Valid;

  UnionBmsStopReason StopReason;//BMS��ֹ���ԭ�� ������
  UnionBmsStopTruble StopTruble;//BMS��ֹ������ԭ�� ������
  UnionBmsStopFault  StopFault;//BMS��ֹ������ԭ�� ������

//  u32 RecvTick;//���Ľ���Tick
}StructBST;

typedef struct
{
  u8  Valid;

  u8  StopSOC;//��ֹ�ɵ�״̬SOC��%�� ������
  u16 PerMinVolt;//�������ص�����͵�ѹ��V�� ������
  u16 PerMaxVolt;//�������ص�����ߵ�ѹ��V�� ������
  u8  MinTemp;//������������¶ȣ�oC�� ������
  u8  MaxTemp;//������������¶ȣ�oC�� ������

//  u32 RecvTick;//���Ľ���Tick
}StructBSD;

typedef union
{
  u32 Mult;
  struct
  {
    u32 Crm_00  : 2;//����SPN2560=0x00�ĳ�����ʶ���ĳ�ʱ��<00>��=������<01>��=��ʱ�� <10>��=������״̬��
    u32 Crm_AA  : 2;//����SPN2560=0xAA�ĳ�����ʶ���ĳ�ʱ��<00>��=������<01>��=��ʱ�� <10>��=������״̬��
    u32 Reserve1  : 4;
    u32 Cml_Cts : 2;//���ճ�����ʱ��ͬ���ͳ����������������ĳ�ʱ��<00>��=������<01>��=��ʱ�� <10>��=������״̬��
    u32 Cro     : 2;//���ճ�����ɳ��׼�����ĳ�ʱ��<00>��=������<01>��=��ʱ�� <10>��=������״̬��
    u32 Reserve2  : 4;
    u32 Ccs     : 2;//���ճ������״̬���ĳ�ʱ��<00>��=������<01>��=��ʱ�� <10>��=������״̬��
    u32 Cst     : 2;//���ճ�����ֹ��籨�ĳ�ʱ��<00>��=������<01>��=��ʱ�� <10>��=������״̬��
    u32 Reserve3  : 4;
    u32 Csd     : 2;//���ճ������ͳ�Ʊ��ĳ�ʱ��<00>��=������<01>��=��ʱ�� <10>��=������״̬��
    u32 Other   : 6;
  }Value;
}UnionBEM;
typedef struct
{
  u8  Valid;

  UnionBEM Bem;//BMS�����ģ�BEM��

//  u32 RecvTick;//���Ľ���Tick
}StructBEM;

typedef union
{
  u8 Mult;
  struct
  {
    u8 AutoSet : 2;//�ﵽ�����趨��������ֹ <00>��=������<01>��=�ﵽ�����趨������ֹ��<10>��=������״̬��
    u8 Manual  : 2;//�˹���ֹ <00>��=������<01>��=�˹���ֹ��<10>��=������״̬��
    u8 Truble  : 2;//������ֹ <00>��=������<01>��=������ֹ��<10>��=������״̬��
    u8 BmsStop : 2;//BMS������ֹ <00>��=������<01>��=BMS��ֹ(�յ�BST֡)��<10>��=������״̬��
  }Value;
}UnionChgStopReason;
typedef union
{
  u16 Mult;
  struct
  {
    u16 PillarTemp : 2;//�������¹��� <00>��=�����¶�������<01>��=�������£�<10>��=������״̬��
    u16 ChgGun  : 2;//������������� <00>��=���������������<01>��=������������ϣ�<10>��=������״̬��
    u16 ModTemp: 2;//�����ڲ����¹��� <00>��=�����ڲ��¶�������<01>��=�����ڲ����£�<10>��=������״̬��
    u16 Power : 2;//����������ܴ��� <00��=��������������<01>���������ܴ��ͣ�<10>��=������״̬��
    u16 Emergency : 2;//������ͣ���� <00>��=������<01>��=������ͣ��<10>��=������״̬��
    u16 Other : 2;//�������� <00>��=������<01>��=���ϣ�<10>��=������״̬��
    u16 Reserve : 4;
  }Value;
}UnionChgStopTruble;
typedef union
{
  u8 Mult;
  struct
  {
    u8 Currt : 2;//������ƥ�� <00>��=����ƥ�䣻<01>��=������ƥ�䣻<10>��=������״̬��
    u8 Volt  : 2;//��ѹ�쳣 <00>��=������<01>��=��ѹ�쳣��<10>��=������״̬��
    u8 Reserve : 4;
  }Value;
}UnionChgStopFault;
typedef struct
{
  u8 Valid;
  UnionChgStopReason StopReason;//������ֹ���ԭ��
  UnionChgStopTruble StopTruble;//������ֹ������ԭ��
  UnionChgStopFault  StopFault;//������ֹ������ԭ��
}StructCST;

typedef struct
{
//  u8  Valid;
  u16 ChgMin;//�ۼƳ��ʱ�� ���ݷֱ��ʣ�1 min/λ��0 minƫ���������ݷ�Χ��0~600 min��������
  u16 ChgEnergy;//������� ���ݷֱ��ʣ�0.1 kWh/λ��0 kWhƫ���������ݷ�Χ��0~1000 kWh��������
  u8  ChgNum[4];//������ţ�1/λ��1ƫ���������ݷ�Χ��0��0xFFFFFFFF ������
}StructCSD;

typedef union
{
  u32 Mult;
  struct
  {
    u32 Brm     : 2;//����BMS�ͳ����ı�ʶ���ĳ�ʱ��<00>��=������<01>��=��ʱ�� <10>��=������״̬��
    u32 Reserve1  : 6;
    u32 Bcp     : 2;//���յ�س��������ĳ�ʱ��<00>��=������<01>��=��ʱ�� <10>��=������״̬��
    u32 Bro     : 2;//����BMS��ɳ��׼�����ĳ�ʱ��<00>��=������<01>��=��ʱ�� <10>��=������״̬��
    u32 Reserve2  : 4;
    u32 Bcs     : 2;//���յ�س����״̬���ĳ�ʱ��<00>��=������<01>��=��ʱ�� <10>��=������״̬��
    u32 Bcl     : 2;//���յ�س��Ҫ���ĳ�ʱ��<00>��=������<01>��=��ʱ�� <10>��=������״̬��
    u32 Bst     : 2;//����BMS��ֹ��籨�ĳ�ʱ��<00>��=������<01>��=��ʱ�� <10>��=������״̬��
    u32 Reserve3  : 2;
    u32 Bsd     : 2;//����BMS���ͳ�Ʊ��ĳ�ʱ��<00>��=������<01>��=��ʱ�� <10>��=������״̬��
    u32 Other   : 6;
  }Value;
}UnionCEM;
typedef struct
{
//  u8  Valid;
  UnionCEM Cem;//BMS�����ģ�BEM��
}StructCEM;

extern StructBHM BhmMsg;
extern StructBRM BrmMsg;
extern StructBCP BcpMsg;
extern StructBRO BroMsg;
extern StructBCL BclMsg;
extern StructBCS BcsMsg;
extern StructBSM BsmMsg;
extern StructBMV BmvMsg;
extern StructBMT BmtMsg;
extern StructBSP BspMsg;
extern StructBST BstMsg;
extern StructBSD BsdMsg;
extern StructBEM BemMsg;
extern StructNTM NtmMsg;

extern StructCST CstMsg;
extern StructCSD CsdMsg;
extern StructCEM CemMsg;
extern StructRES ResMsg;

extern void BhmMsgMake(u8 *pBuff);
extern void BrmMsgMake(u8 *pBuff, u16 Len);
extern void BcpMsgMake(u8 *pBuff);
extern void BroMsgMake(u8 *pBuff);
extern void BclMsgMake(u8 *pBuff);
extern void BcsMsgMake(u8 *pBuff);
extern void BsmMsgMake(u8 *pBuff);
extern void BmvMsgMake(u8 *pBuff, u16 Len);
extern void BmtMsgMake(u8 *pBuff, u16 Len);
extern void BspMsgMake(u8 *pBuff, u16 Len);
extern void BstMsgMake(u8 *pBuff);
extern void BsdMsgMake(u8 *pBuff);
extern void BemMsgMake(u8 *pBuff);

extern osMessageQId BmsMsg;
extern StructTransProt TransProt;

extern u8 BmsCommInit(void);
extern u8 BmsCommSendMsg(StructChargeSend *pMsg);

extern void NACK_Send(StructPgnRecv *pFrameRecv);

extern void TPCM_AbortSendBegin(StructPgnRecv *pFrameRecv);
extern void TPCM_Cts(StructPgnRecv *pFrameRecv);
extern void TPCM_AbortSending(StructTransProt *pTransProt);
extern void TPCM_EndOfMsgAck(StructTransProt *pTransProt);

extern void CHM_Send(void);
extern void CRM_Send(u8 Recog);
extern void CTS_Send(void);
extern void CML_Send(u16 MaxVolt, u16 MinVolt, u16 MaxCurrt, u16 MinCurrt);
extern void CRO_Send(u8 Status);
extern void CCS_Send(u16 Volt, u16 Currt, u16 Min, u16 ChgEnable);
extern void CST_Send(StructCST *pCst);
extern void CSD_Send(StructCSD *pCsd);
extern void CEM_Send(StructCEM *pCem);
extern void NTM_Send(u8  isOK,u32 verNum);
extern void NTM_SendLenErro(u8  isOK,u32 verNum);

extern void DM1_Send(u32 Dtc);
extern void DM3_Send(u16 Num);
extern void V2L_Send(void);

extern EnumChgVersion ChgVer;
extern s32 ChgPgnRecv(StructPgnRecv *pPgnRecv);

#endif