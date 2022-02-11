#ifndef  __ADRV_BMSFRAME_H__
#define  __ADRV_BMSFRAME_H__

#include "Global.h"
#include "Param_UserConfig.h"

/*
报文报文 代号	报文描述	PGN	PGN（Hex）	优先权	数据长度byte	报文周期	源地址-目的地址
CHM	充电机握手	9728	002600H	6	3	250ms	充电机-BMS
BHM	车辆握手	9984	002700H	6	2	250ms	BMS-充电机
CRM	充电机辨识	256	000100H	6	8	250ms	充电机-BMS
BRM	BMS和车辆辨识报文	512	000200H	7	41	250ms	BMS-充电机

BCP	动力蓄电池充电参数	1536	000600H	7	13	500ms	BMS-充电机
CTS	充电机发送时间同步信息	1792	000700H	6	7	500ms	充电机-BMS
CML	充电机最大输出能力	2048	000800H	6	8	250ms	充电机-BMS
BRO	电池充电准备就绪状态	2304	000900H	4	1	250ms	BMS-充电机
CRO	充电机输出准备就绪状态	2560	000A00H	4	1	250ms	充电机-BMS

BCL	电池充电需求	4096	001000H	6	5	50ms	BMS-充电机
BCS	电池充电总状态	4352	001100H	7	9	250ms	BMS-充电机
CCS	充电机充电状态	4608	001200H	6	6	50ms	充电机-BMS
BSM	动力蓄电池状态信息	4864	001400H	6	7	250ms	BMS-充电机
BMV	单体动力蓄电池电压	5376	001500H	7	不定	10s	BMS-充电机
BMT	动力蓄电池温度	5632	001600H	7	不定	10s	BMS-充电机
BSP	动力蓄电池预留报文	5888	001700H	7	不定	10s	BMS-充电机
BST	BMS中止充电	6400	001900H	4	4	10ms	BMS-充电机
CST	充电机中止充电	6656	001A00H	4	4	10ms	充电机-BMS

BSD	BMS统计数据	7168	001C00H	6	7	250ms	BMS-充电机
CSD	充电机统计数据	7424	001D00H	6	8	250ms	充电机-BMS

BEM	BMS错误报文	7680	001E00H	2	4	250ms	BMS-充电机
CEM	充电机错误报文	7936	001F00H	2	4	250ms	充电机-BMS

DM1	当前故障码	8192	002000H	6	不定	事件响应
DM2	历史故障码	8448	002100H	6	不定	事件响应
DM3	诊断准备就绪	8704	002200H	6	2字节	事件响应
DM4	当前故障码的清除/复位	8906	002300H	6	0	事件响应
DM5	历史故障码的清除/复位	9216	002400H	6	0	事件响应
DM6	停帧参数	9472	002500H	6	不定	事件响应


*/

typedef enum
{
  
  V2L = 0x000100, //放电请求(充电桩---车端)
  RES = 0x004100, //放电请求响应(车端---充电桩)
  NTM = 0x003C00, //NTM 充电机版本协商
  EVM = 0x003D00, //EVM 车辆版本协商
  CHM = 0x002600,// CHM	充电机握手	9728	002600H	6	3	250ms	充电机-BMS
  BHM = 0x002700,// BHM	车辆握手	9984	002700H	6	2	250ms	BMS-充电机
  CRM = 0x000100,// CRM	充电机辨识	256	000100H	6	8	250ms	充电机-BMS
  BRM = 0x000200,// BRM	BMS和车辆辨识报文	512	000200H	7	41	250ms	BMS-充电机

  BCP = 0x000600,// BCP	动力蓄电池充电参数	1536	000600H	7	13	500ms	BMS-充电机
  CTS = 0x000700,// CTS	充电机发送时间同步信息	1792	000700H	6	7	500ms	充电机-BMS
  CML = 0x000800,// CML	充电机最大输出能力	2048	000800H	6	8	250ms	充电机-BMS
  BRO = 0x000900,// BRO	电池充电准备就绪状态	2304	000900H	4	1	250ms	BMS-充电机
  CRO = 0x000A00,// CRO	充电机输出准备就绪状态	2560	000A00H	4	1	250ms	充电机-BMS

  BCL = 0x001000,// BCL	电池充电需求	4096	001000H	6	5	50ms	BMS-充电机
  BCS = 0x001100,// BCS	电池充电总状态	4352	001100H	7	9	250ms	BMS-充电机
  CCS = 0x001200,// CCS	充电机充电状态	4608	001200H	6	6	50ms	充电机-BMS
  BSM = 0x001300,// BSM	动力蓄电池状态信息	4864	001400H	6	7	250ms	BMS-充电机
  BMV = 0x001500,// BMV	单体动力蓄电池电压	5376	001500H	7	不定	10s	BMS-充电机
  BMT = 0x001600,// BMT	动力蓄电池温度	5632	001600H	7	不定	10s	BMS-充电机
  BSP = 0x001700,// BSP	动力蓄电池预留报文	5888	001700H	7	不定	10s	BMS-充电机
  BST = 0x001900,// BST	BMS中止充电	6400	001900H	4	4	10ms	BMS-充电机
  CST = 0x001A00,// CST	充电机中止充电	6656	001A00H	4	4	10ms	充电机-BMS

  BSD = 0x001C00,// BSD	BMS统计数据	7168	001C00H	6	7	250ms	BMS-充电机
  CSD = 0x001D00,// CSD	充电机统计数据	7424	001D00H	6	8	250ms	充电机-BMS

  BEM = 0x001E00,// BEM	BMS错误报文	7680	001E00H	2	4	250ms	BMS-充电机
  CEM = 0x001F00,// CEM	充电机错误报文	7936	001F00H	2	4	250ms	充电机-BMS

  DM1 = 0x002000,// DM1	当前故障码	8192	002000H	6	不定	事件响应
  DM2 = 0x002100,// DM2	历史故障码	8448	002100H	6	不定	事件响应
  DM3 = 0x002200,// DM3	诊断准备就绪	8704	002200H	6	2字节	事件响应
  DM4 = 0x002300,// DM4	当前故障码的清除/复位	8906	002300H	6	0	事件响应
  DM5 = 0x002400,// DM5	历史故障码的清除/复位	9216	002400H	6	0	事件响应
  DM6 = 0x002500,// DM6	停帧参数	9472	002500H	6	不定	事件响应

  REQ  = 0x00EA00,//请求
  ACK  = 0x00E800,//确认
  TPCM = 0x00EC00,//传输协议--连接管理
  TPDT = 0x00EB00,//传输协议--数据传送
  
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
  u8 VerisOK;   //0--初始值以最大版本协商，1---版本号对上，2----版本号低于最低版本
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

  u32 RecvTick;//报文接收Tick
}StructRES;


typedef struct
{
  u8 Valid;

  u16 ImdMaxVolt;

  u32 RecvTick;//报文接收Tick
}StructBHM;

typedef struct
{
  u8  Valid;
  struct
  {
    u32 L : 8;
    u32 M : 8;
    u32 H : 8;
  }CommVer;//3字节 BMS通信协议版本号，本标准规定当前版本为V1.0，表示为：byte3，byte2—0001H；byte1—00H 必须项
  u8  BatType;//1字节 电池类型，01H：铅酸电池；02H：镍氢电池；03H：磷酸铁锂电池；04H：锰酸锂电池；05H：钴酸锂电池；06H：三元材料电池；07H：聚合物锂离子电池；08H：钛酸锂电池；FFH：其他电池 必须项
  u16 BatRateCap;//2字节 整车动力蓄电池系统额定容量/Ah，0.1Ah/位，0Ah偏移量，数据范围：0～1000Ah 必须项
  u16 BatRateVolt;//2字节 整车动力蓄电池系统额定总电压/V，0.1V/位，0V偏移量，数据范围：0～750V 必须项
  u8  Manufacturer[4];//4字节 电池生产厂商名称，标准ASCII码 可选项
  u8  ProduceNum[4];//4字节 电池组序号，预留，由厂商自行定义 可选项
  u8  ProduceYear;//1字节 电池组生产日期：年，1年/位，1985年偏移量，数据范围：1985～2235年 可选项
  u8  ProduceMon;//1字节 电池组生产日期：月，1月/位，0月偏移量，数据范围：1～12月 可选项
  u8  ProduceDay;//1字节 电池组生产日期：日，1日/位，0日偏移量，数据范围：1～31日 可选项
  u32 ChgCount;//3字节 电池组充电次数，1次/位，0次偏移量，以BMS统计为准 可选项
  u8  BatOwn;//1字节 电池组产权标识（<0>：=租赁；<1>：=车自有）可选项
  u8  Reserve;//保留项 可选项
  u8  Vin[17];//17字节 车辆识别码（VIN）可选项
  u8  BmsVer[8];

  u32 RecvTick;//报文接收Tick
}StructBRM;

typedef struct
{
  u8  Valid;

  u16 PerBatMaxVolt; //单体动力蓄电池最高允许充电电压 数据分辨率：0.01 V/位，0 V偏移量； 数据范围：0~24 V；必须项
  s16 ChgMaxCurrt;//最高允许充电电流 数据分辨率：0.1 A/位，-400 A偏移量； 数据范围：-400 A~0 A；必须项
  u16 BatEnergy;//动力蓄电池标称总能 数据分辨率：0.1 kWh/位，0 kWh偏移量； 数据范围：0~1000 kWh；必须项
  u16 ChgMaxVolt;//最高允许充电总电压 数据分辨率：0.1 V/位，0 V偏移量；数据范围：0~750 V；
  u8  MaxTemp;//最高允许动力蓄电池温度 数据分辨率：1oC/位，-50 oC偏移量；数据范围：-50 oC ~+200 oC ；
  u16 StartSoc;//整车动力蓄电池荷电状态（SOC）数据分辨率：0.1%/位，0%偏移量；数据范围：0~100%；
  u16 StartVolt;//整车动力蓄电池当前电池电压 数据分辨率：0.1 V/位，0 V偏移量：数据范围：0~750 V。

  u32 RecvTick;//报文接收Tick
}StructBCP;

typedef struct
{
  u8 Valid;

  u8 Status;//BMS是否充电准备好（<0x00 >：=BMS未做好充电准备) ; <0xAA >：=BMS完成充电准备；<0xFF>：=无效)

  u32 RecvTick;
}StructBRO;

typedef struct
{
  u8  Valid;

  u16 NeedVolt; //电压需求 数据分辨率：0.1 V/位，0 V偏移量；数据范围：0~750 V；必须项
  u16 NeedCurrt;//电流需求 数据分辨率：0.1 A/位，-400 A偏移量；必须项
  u8  ChgType;//充电模式（0x01：恒压充电；0x02：恒流充电）

  u32 RecvTick;//报文接收Tick
}StructBCL;

typedef union
{
  u16 Mult;
  struct
  {
    u16 Volt  : 12;//1-12位：单体动力蓄电池电压，数据分辨率：0.01 V/位，0 V偏移量；数据范围：0~24 V；
    u16 Group : 4;//13-16位：电池分组号，数据分辨率：1/位，1偏移量；数据范围：1~16。
  }Value;
}UnionBatVoltAndGroup;
typedef struct
{
  u8  Valid;

  u16 ChgVolt; //充电电压测量值 数据分辨率：0.1 V/位，0 V偏移量；数据范围：0~750 V；必须项
  u16 ChgCurrt;//充电电流测量值 数据分辨率：0.1 A/位，-400 A偏移量；必须项
  UnionBatVoltAndGroup BatVoltAndGroup;//最高单体动力蓄电池电压及其组号 1-12位：最高单体动力蓄电池电压，数据分辨率：0.01 V/位，0 V偏移量；数据范围：0~24 V；13-16位：最高单体动力蓄电池电压所在组号，数据分辨率：1/位，0偏移量；数据范围：0~15；必须项
  u8  CurSoc;//当前荷电状态SOC 数据分辨率：1%/位，0%偏移量；数据范围：0~100%；
  u16 RemainMin;//估算剩余充电时间，当BMS以实际电流为准进行测算的剩余时间超过600 min时，按600 min发送。数据分辨率：1 min/位，0 min偏移量；数据范围：0~600 min。

  u32 RecvTick;//报文接收Tick
}StructBCS;

typedef union
{
  u8 Mult;
  struct
  {
    u8 Volt : 2;//单体动力蓄电池电压过高/过低(<00>：=正常; <01>：=过高; <10>：=过低) 必须项
    u8 Soc  : 2;//整车动力蓄电池荷电状态SOC过高/过低(<00>：=正常; <01>：=过高; <10>：=过低) 必须项
    u8 Currt: 2;//动力蓄电池充电过电流(<00>：=正常; <01>：=过流; <10>：=不可信状态) 必须项
    u8 Temp : 2;//动力蓄电池温度过高(<00>：=正常; <01>：=过高; <10>：=不可信状态) 必须项
  }Value;
}UnionBatStatus;
typedef union
{
  u8 Mult;
  struct
  {
    u8 Imd : 2;//动力蓄电池绝缘状态(<00>：=正常; <01>：=不正常; <10>：=不可信状态) 必须项
    u8 Connect  : 2;//动力蓄电池组输出连接器连接状态(<00>：=正常; <01>：=不正常; <10>：=不可信状态) 必须项
    u8 ChgEnable : 2;//充电允许(<00>：=禁止; <01>：=允许; ) 必须项
  }Value;
}UnionChargeStatus;
typedef struct
{
  u8  Valid;
  u8  HighTempBatNum; //最高单体动力蓄电池电压所在编号 数据分辨率：1/位，1偏移量；数据范围：1~256；必须项
  u8  HighTemp;//最高动力蓄电池温度 数据分辨率：1oC/位，-50 oC偏移量；数据范围：-50 oC ~+200 oC；必须项
  u8  HighTempPointNum; //最高温度检测点编号 数据分辨率：1/位，1偏移量；数据范围：1~128；必须项
  u8  LowTemp;//最低动力蓄电池温度 数据分辨率：1oC/位，-50 oC偏移量；数据范围：-50 oC ~+200 oC；必须项
  u8  LowTempPointNum;//最低温度检测点编号 数据分辨率：1/位，1偏移量；数据范围：1~128。必须项
  UnionBatStatus BatSta;//
  UnionChargeStatus ChgSta;//

  u32 RecvTick;//报文接收Tick
}StructBSM;

typedef struct
{
  u8  Valid;

  UnionBatVoltAndGroup BatVoltAndGroup[256];//分别对应#1~#256单体动力蓄电池电压

  u32 RecvTick;//报文接收Tick
}StructBMV;

typedef struct
{
  u8  Valid;

  u8  Temp[128];//1~128采样点的温度 数据分辨率：1oC/位，-50 oC偏移量；数据范围：-50 oC ~+200 oC。可选项

  u32 RecvTick;//报文接收Tick
}StructBMT;

typedef struct
{
  u8  Valid;

  u8  Bsp[16];//动力蓄电池预留字段16

  u32 RecvTick;//报文接收Tick
}StructBSP;

typedef union
{
  u8 Mult;
  struct
  {
    u8 NeedSoc : 2;//达到所需求的SOC目标值 <00>：=未达到所需SOC目标值；<01>：=达到所需SOC目标值；<10>：=不可信状态；必须项
    u8 SetVolt  : 2;//达到总电压的设定值 <00>：=未达到总电压设定值；<01>：=达到总电压设定值；<10>：=不可信状态；必须项
    u8 PerSetVolt: 2;//达到单体电压的设定值 <00>：=未达到单体电压设定值；<01>：=达到单体电压设定值；<10>：=不可信状态；必须项
    u8 ChgStop : 2;//充电机主动中止 <00>：=正常；<01>：=充电机中止(收到CST帧)；<10>：=不可信状态。必须项
  }Value;
}UnionBmsStopReason;
typedef union
{
  u16 Mult;
  struct
  {
    u16 Imd : 2;//绝缘故障 <00>：=正常；<01>：=故障；<10>：=不可信状态；
    u16 OutletTemp  : 2;//<00>：=正常；<01>：=故障；<10>：=不可信状态；
    u16 ComponentTemp: 2;//BMS元件、输出连接器过温<00>：=正常；<01>：=故障；<10>：=不可信状态；
    u16 ChgGun : 2;//充电连接器故障 <00>：=充电连接器正常；<01>：=充电连接器故障；<10>：=不可信状态；
    u16 BatPackTemp : 2;//电池组温度过高故障 <00>：=电池组温度正常；<01>：=电池组温度过高；<10>：=不可信状态；
    u16 HighRelay : 2;//高压继电器故障
    u16 DetectionPoint2 : 2;//监测点2电压故障
    u16 Other : 2;//其他故障 <00>：=正常；<01>：=故障；<10>：=不可信状态。
  }Value;
}UnionBmsStopTruble;
typedef union
{
  u8 Mult;
  struct
  {
    u8 Currt : 2;//电流过大 <00>：=电流正常；<01>：=电流超过需求值；<10>：=不可信状态；
    u8 Volt  : 2;//电压异常 <00>：=正常；<01>：=电压异常；<10>：=不可信状态；
  }Value;
}UnionBmsStopFault;
typedef struct
{
  u8 Valid;

  UnionBmsStopReason StopReason;//BMS中止充电原因 必须项
  UnionBmsStopTruble StopTruble;//BMS中止充电故障原因 必须项
  UnionBmsStopFault  StopFault;//BMS中止充电错误原因 必须项

//  u32 RecvTick;//报文接收Tick
}StructBST;

typedef struct
{
  u8  Valid;

  u8  StopSOC;//中止荷电状态SOC（%） 必须项
  u16 PerMinVolt;//动力蓄电池单体最低电压（V） 必须项
  u16 PerMaxVolt;//动力蓄电池单体最高电压（V） 必须项
  u8  MinTemp;//动力蓄电池最低温度（oC） 必须项
  u8  MaxTemp;//动力蓄电池最高温度（oC） 必须项

//  u32 RecvTick;//报文接收Tick
}StructBSD;

typedef union
{
  u32 Mult;
  struct
  {
    u32 Crm_00  : 2;//接收SPN2560=0x00的充电机辨识报文超时（<00>：=正常；<01>：=超时； <10>：=不可信状态）
    u32 Crm_AA  : 2;//接收SPN2560=0xAA的充电机辨识报文超时（<00>：=正常；<01>：=超时； <10>：=不可信状态）
    u32 Reserve1  : 4;
    u32 Cml_Cts : 2;//接收充电机的时间同步和充电机最大输出能力报文超时（<00>：=正常；<01>：=超时； <10>：=不可信状态）
    u32 Cro     : 2;//接收充电机完成充电准备报文超时（<00>：=正常；<01>：=超时； <10>：=不可信状态）
    u32 Reserve2  : 4;
    u32 Ccs     : 2;//接收充电机充电状态报文超时（<00>：=正常；<01>：=超时； <10>：=不可信状态）
    u32 Cst     : 2;//接收充电机中止充电报文超时（<00>：=正常；<01>：=超时； <10>：=不可信状态）
    u32 Reserve3  : 4;
    u32 Csd     : 2;//接收充电机充电统计报文超时（<00>：=正常；<01>：=超时； <10>：=不可信状态）
    u32 Other   : 6;
  }Value;
}UnionBEM;
typedef struct
{
  u8  Valid;

  UnionBEM Bem;//BMS错误报文（BEM）

//  u32 RecvTick;//报文接收Tick
}StructBEM;

typedef union
{
  u8 Mult;
  struct
  {
    u8 AutoSet : 2;//达到充电机设定的条件中止 <00>：=正常；<01>：=达到充电机设定条件中止；<10>：=不可信状态；
    u8 Manual  : 2;//人工中止 <00>：=正常；<01>：=人工中止；<10>：=不可信状态；
    u8 Truble  : 2;//故障中止 <00>：=正常；<01>：=故障中止；<10>：=不可信状态；
    u8 BmsStop : 2;//BMS主动中止 <00>：=正常；<01>：=BMS中止(收到BST帧)；<10>：=不可信状态。
  }Value;
}UnionChgStopReason;
typedef union
{
  u16 Mult;
  struct
  {
    u16 PillarTemp : 2;//充电机过温故障 <00>：=充电机温度正常；<01>：=充电机过温；<10>：=不可信状态；
    u16 ChgGun  : 2;//充电连接器故障 <00>：=充电连接器正常；<01>：=充电连接器故障；<10>：=不可信状态；
    u16 ModTemp: 2;//充电机内部过温故障 <00>：=充电机内部温度正常；<01>：=充电机内部过温；<10>：=不可信状态；
    u16 Power : 2;//所需电量不能传送 <00：=电量传送正常；<01>：电量不能传送；<10>：=不可信状态；
    u16 Emergency : 2;//充电机急停故障 <00>：=正常；<01>：=充电机急停；<10>：=不可信状态；
    u16 Other : 2;//其他故障 <00>：=正常；<01>：=故障；<10>：=不可信状态。
    u16 Reserve : 4;
  }Value;
}UnionChgStopTruble;
typedef union
{
  u8 Mult;
  struct
  {
    u8 Currt : 2;//电流不匹配 <00>：=电流匹配；<01>：=电流不匹配；<10>：=不可信状态；
    u8 Volt  : 2;//电压异常 <00>：=正常；<01>：=电压异常；<10>：=不可信状态。
    u8 Reserve : 4;
  }Value;
}UnionChgStopFault;
typedef struct
{
  u8 Valid;
  UnionChgStopReason StopReason;//充电机中止充电原因
  UnionChgStopTruble StopTruble;//充电机中止充电故障原因
  UnionChgStopFault  StopFault;//充电机中止充电错误原因
}StructCST;

typedef struct
{
//  u8  Valid;
  u16 ChgMin;//累计充电时间 数据分辨率：1 min/位，0 min偏移量；数据范围：0~600 min；必须项
  u16 ChgEnergy;//输出能量 数据分辨率：0.1 kWh/位，0 kWh偏移量；数据范围：0~1000 kWh。必须项
  u8  ChgNum[4];//充电机编号，1/位，1偏移量，数据范围：0～0xFFFFFFFF 必须项
}StructCSD;

typedef union
{
  u32 Mult;
  struct
  {
    u32 Brm     : 2;//接收BMS和车辆的辨识报文超时（<00>：=正常；<01>：=超时； <10>：=不可信状态）
    u32 Reserve1  : 6;
    u32 Bcp     : 2;//接收电池充电参数报文超时（<00>：=正常；<01>：=超时； <10>：=不可信状态）
    u32 Bro     : 2;//接收BMS完成充电准备报文超时（<00>：=正常；<01>：=超时； <10>：=不可信状态）
    u32 Reserve2  : 4;
    u32 Bcs     : 2;//接收电池充电总状态报文超时（<00>：=正常；<01>：=超时； <10>：=不可信状态）
    u32 Bcl     : 2;//接收电池充电要求报文超时（<00>：=正常；<01>：=超时； <10>：=不可信状态）
    u32 Bst     : 2;//接收BMS中止充电报文超时（<00>：=正常；<01>：=超时； <10>：=不可信状态）
    u32 Reserve3  : 2;
    u32 Bsd     : 2;//接收BMS充电统计报文超时（<00>：=正常；<01>：=超时； <10>：=不可信状态）
    u32 Other   : 6;
  }Value;
}UnionCEM;
typedef struct
{
//  u8  Valid;
  UnionCEM Cem;//BMS错误报文（BEM）
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