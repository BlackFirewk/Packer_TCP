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
    uint16_t interReport;           // ??????????????? ?????????
    uint16_t* cutPlcStateReg;       // ???????????????????????????????????????????????????
    uint16_t* currentZipperLength;  // ????????????
    uint16_t* employeeIDReg;        // ??????????????????
    uint16_t* workTickIDReg;        // ???????????????
    uint16_t* workTickApplyCodeReg; // ?????????????????????
    uint16_t* orderQuantityReg;     // ??????0:???????????? 1:????????????
    uint16_t* barCodeReg;           // ???????????????
    uint16_t* packerStateReg;       // ???????????????????????????
    uint16_t* packerProcReg;        // ???????????????????????????

    // read
    uint16_t packerReady;           // ???????????????
    uint16_t packerStart;           // ????????????????????????
    uint16_t packerStop;            // ?????????????????????
    uint16_t orderFinish;           // ??????????????????
    uint16_t printTrig;             // ??????????????????
    uint16_t printFinish;           // ??????????????????
    uint16_t printAgain;            // ????????????
    uint16_t thisPackWeight;        // ????????????
    uint16_t thisPackQuant;         // ????????????

    // write
    uint16_t siglePackQuant;        // ????????????
    uint16_t subScatteredOrderQuant;        // ????????????
    uint16_t scanGunTriggerSignal;  // ?????????????????????
    uint16_t zipperLength;          // ????????????
    // read and write
    uint16_t orderTotal;            // ????????????
};

#endif // SINGLETONMODBUSINFO_H
