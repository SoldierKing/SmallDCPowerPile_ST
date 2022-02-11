[12/09 2021] 
程序版本：V1.0.0

蔚来小功率3.3KW双向直流充电桩初始版本。

[02/09 2022] 
程序版本：V1.0.1

修改：
1. Station_V2L从发3秒改为一直发送到接收到BRM报文；
2. BRM中RESERVED信号蔚来车型会发送0xBA，设备通过该信号判断是否是蔚来车型
3. VCU在收到Station_V2L后开始发送Veh_RES报文，直至本次流程结束
4. Station_V2L在每次开始流程时必须发送
5. 当设备放电时，Station_V2L发送后3m内未收到Veh_RES报文，继续往下走流程，当收到BRM中RESERVED信号为0xBA，则结束流程，CST报桩端故障
6. 当设备放电时，Station_V2L发送后3m内未收到Veh_RES报文，继续往下走流程，当收到BRM中RESERVED信号不为0xBA，则认为是其他车继续流程
7. 当设备放电时，Station_V2L发送后3m内收到Veh_RES中VCUV2LDischargingSts为4，则停止流程
8. 当设备充电时，收到Veh_RES报文且VCUV2LDischargingSts不为4，则停止流程。





