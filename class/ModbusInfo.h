#ifndef MODBUSCOMMUNICATIONINFO_H
#define MODBUSCOMMUNICATIONINFO_H

#include "PackerBase.h"



typedef struct _MODBUS_INFO_ {
//    this->workTickNumber;         // 工票号
//    this->tickApplCode;           // 工票申请码
//    this->tickNumber;             // 工票号
//    this->employeeID;             // 员工工号
    uint16_t interReport;           // 机联网报工 未使用
//    uint16_t cutPlcStartSignal;           // 切断机启动信号
//    uint16_t cutPlcStopSignal;            // 切断机停机信号

    // read
    uint16_t packer_proc_reg[5];   // 存放包装机其他信号
    uint16_t packerReady;          // 包装机就绪
    uint16_t packerStart;          // 包装言机启动信号
    uint16_t packerStop;           // 包装机停机信号
    uint16_t orderFinish;          // 订单完成信号
    uint16_t printTrig;            // 触发打印信号
    uint16_t printFinish;          // 打印完成信号
    uint16_t printAgain;           // 重新打印
    uint16_t thisPackWeight;       // 本包重量
    uint16_t thisPackQuant;        // 本包数量
    uint16_t cutPlcStateReg[2];    // 存放切断屏状态信号　运行和停止信号
    uint16_t currentZipperLength[2]; // 当前尺寸
    uint16_t employeeIDReg[64];    // 存放员工工号
    uint16_t workTickIDReg[64];    // 存放工票号
    uint16_t workTickApplyCodeReg[32];                                                      // 存放工票申请码


    // write
    uint16_t cutStateReg[2];       // 存放  0:切断机ＯＮ信号　　1:切断机ＯＦＦ信号
    uint16_t orderQuantityReg[2];  // 存放0:订单总量 1:每包数量
    uint16_t siglePackQuant;       // 单包数量
    uint16_t subScatteredOrderQuant;        // 散单数量
    uint16_t barCodeReg[64];       // 存放条码号
    uint16_t scanGunTriggerSignal; // 扫描枪触发信号
    uint16_t zipperLength;         // 拉链长度

    // read and write
    uint16_t orderTotal;           // 总订单数
    uint16_t packerStateReg[5];    // 存放包装机状态信号
} MODBUS_INFO;

#endif // MODBUSCOMMUNICATIONINFO_H
