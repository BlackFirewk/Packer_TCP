#ifndef SINGLETONMODBUSINFO_H
#define SINGLETONMODBUSINFO_H
#include "PackerBase.h"

#define CUTTER_STATE_REG_SIZE   2
#define ZIPPER_LENGTH_REG_SIZE  2
#define EMPLOYEE_ID_REG_SIZE    32
#define WORK_TICKET_ID_REG_SIZE 32
#define WORK_TICKET_APPLY_CODE_REG_SIZE 32
#define ORDER_QUANT_REG_SIZE    2
#define BAR_CODE_REG_SIZE       32
#define PACKER_STATE_REG_SIZE   3
#define PACKER_PROC_REG_SIZE    5


class SingletonModbusInfo
{
public:
    static SingletonModbusInfo& GetInstance()
    {
        static SingletonModbusInfo instance;
        return instance;
    }

    uint16_t getOrderTotal() const;
    void setOrderTotal(const uint16_t& value);

    uint16_t getZipperLength() const;
    void setZipperLength(const uint16_t& value);

    uint16_t getScanGunTriggerSignal() const;
    void setScanGunTriggerSignal(const uint16_t& value);

    uint16_t getSubScatteredOrderQuant() const;
    void setSubScatteredOrderQuant(const uint16_t& value);

    uint16_t getSiglePackQuant() const;
    void setSiglePackQuant(const uint16_t& value);

    uint16_t getThisPackQuant() const;
    void setThisPackQuant(const uint16_t& value);

    uint16_t getThisPackWeight() const;
    void setThisPackWeight(const uint16_t& value);

    uint16_t getPrintAgain() const;
    void setPrintAgain(const uint16_t& value);

    uint16_t getPrintFinish() const;
    void setPrintFinish(const uint16_t& value);

    uint16_t getPrintTrig() const;
    void setPrintTrig(const uint16_t& value);

    uint16_t getOrderFinish() const;
    void setOrderFinish(const uint16_t& value);

    uint16_t getPackerStop() const;
    void setPackerStop(const uint16_t& value);

    uint16_t getPackerStart() const;
    void setPackerStart(const uint16_t& value);

    uint16_t getPackerReady() const;
    void setPackerReady(const uint16_t& value);

    uint16_t* getPackerStateReg();
    uint16_t* getPackerProcReg();
    uint16_t* getBarCodeReg();
    uint16_t* getOrderQuantityReg();
    uint16_t* getWorkTickApplyCodeReg();
    uint16_t* getWorkTickIDReg();
    uint16_t* getEmployeeIDReg();
    uint16_t* getCurrentZipperLength();
    uint16_t* getCutPlcStateReg();

private:
    SingletonModbusInfo();
    ~SingletonModbusInfo();
    SingletonModbusInfo& operator = (const SingletonModbusInfo&);

private:
    uint16_t interReport;           // 机联网报工 未使用
    uint16_t* cutPlcStateReg;       // 存放切断屏状态信号　运行和停止信号
    uint16_t* currentZipperLength;  // 当前尺寸
    uint16_t* employeeIDReg;        // 存放员工工号
    uint16_t* workTickIDReg;        // 存放工票号
    uint16_t* workTickApplyCodeReg; // 存放工票申请码
    uint16_t* orderQuantityReg;     // 存放0:订单总量 1:每包数量
    uint16_t* barCodeReg;           // 存放条码号
    uint16_t* packerStateReg;       // 存放包装机状态信号
    uint16_t* packerProcReg;        // 存放包装机其他信号

    // read
    uint16_t packerReady;           // 包装机就绪
    uint16_t packerStart;           // 包装言机启动信号
    uint16_t packerStop;            // 包装机停机信号
    uint16_t orderFinish;           // 订单完成信号
    uint16_t printTrig;             // 触发打印信号
    uint16_t printFinish;           // 打印完成信号
    uint16_t printAgain;            // 重新打印
    uint16_t thisPackWeight;        // 本包重量
    uint16_t thisPackQuant;         // 本包数量

    // write
    uint16_t siglePackQuant;        // 单包数量
    uint16_t subScatteredOrderQuant;        // 散单数量
    uint16_t scanGunTriggerSignal;  // 扫描枪触发信号
    uint16_t zipperLength;          // 拉链长度
    // read and write
    uint16_t orderTotal;            // 总订单数
};

#endif // SINGLETONMODBUSINFO_H
