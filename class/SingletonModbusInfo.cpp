#include "SingletonModbusInfo.h"

SingletonModbusInfo::SingletonModbusInfo()
{
    int size16 = sizeof(uint16_t);
    cutPlcStateReg = new uint16_t[CUTTER_STATE_REG_SIZE];
    memset(cutPlcStateReg, 0, CUTTER_STATE_REG_SIZE * size16);

    currentZipperLength = new uint16_t[ZIPPER_LENGTH_REG_SIZE];
    memset(currentZipperLength, 0, ZIPPER_LENGTH_REG_SIZE * size16);

    employeeIDReg = new uint16_t[EMPLOYEE_ID_REG_SIZE];
    memset(employeeIDReg, 0, EMPLOYEE_ID_REG_SIZE * size16);

    workTickIDReg = new uint16_t[WORK_TICKET_ID_REG_SIZE];
    memset(workTickIDReg, 0, WORK_TICKET_ID_REG_SIZE * size16);

    workTickApplyCodeReg = new uint16_t[WORK_TICKET_APPLY_CODE_REG_SIZE];
    memset(workTickApplyCodeReg, 0, WORK_TICKET_APPLY_CODE_REG_SIZE * size16);

    orderQuantityReg = new uint16_t[ORDER_QUANT_REG_SIZE];
    memset(orderQuantityReg, 0, ORDER_QUANT_REG_SIZE * size16);

    barCodeReg = new uint16_t[BAR_CODE_REG_SIZE];
    memset(barCodeReg, 0, BAR_CODE_REG_SIZE * size16);

    packerStateReg = new uint16_t[PACKER_STATE_REG_SIZE];
    memset(packerStateReg, 0, PACKER_STATE_REG_SIZE * size16);
}

SingletonModbusInfo::~SingletonModbusInfo()
{
    delete cutPlcStateReg;
    delete currentZipperLength;
    delete employeeIDReg;
    delete workTickIDReg;
    delete workTickApplyCodeReg;
    delete orderQuantityReg;
    delete barCodeReg;
    delete packerStateReg;
}

uint16_t* SingletonModbusInfo::getCutPlcStateReg()
{
    return cutPlcStateReg;
}

uint16_t* SingletonModbusInfo::getCurrentZipperLength()
{
    return currentZipperLength;
}

uint16_t* SingletonModbusInfo::getEmployeeIDReg()
{
    return employeeIDReg;
}


uint16_t* SingletonModbusInfo::getWorkTickIDReg()
{
    return workTickIDReg;
}

uint16_t* SingletonModbusInfo::getWorkTickApplyCodeReg()
{
    return workTickApplyCodeReg;
}

//uint16_t* SingletonModbusInfo::getCutStateReg() const
//{
//    return cutStateReg;
//}

//void SingletonModbusInfo::setCutStateReg(uint16_t* value)
//{
//    cutStateReg = value;
//}

uint16_t* SingletonModbusInfo::getOrderQuantityReg()
{
    return orderQuantityReg;
}

uint16_t* SingletonModbusInfo::getBarCodeReg()
{
    return barCodeReg;
}

uint16_t* SingletonModbusInfo::getPackerStateReg()
{
    return packerStateReg;
}

uint16_t SingletonModbusInfo::getPackerReady() const
{
    return packerReady;
}

void SingletonModbusInfo::setPackerReady(const uint16_t& value)
{
    packerReady = value;
}

uint16_t SingletonModbusInfo::getPackerStart() const
{
    return packerStart;
}

void SingletonModbusInfo::setPackerStart(const uint16_t& value)
{
    packerStart = value;
}

uint16_t SingletonModbusInfo::getPackerStop() const
{
    return packerStop;
}

void SingletonModbusInfo::setPackerStop(const uint16_t& value)
{
    packerStop = value;
}

uint16_t SingletonModbusInfo::getOrderFinish() const
{
    return orderFinish;
}

void SingletonModbusInfo::setOrderFinish(const uint16_t& value)
{
    orderFinish = value;
}

uint16_t SingletonModbusInfo::getPrintTrig() const
{
    return printTrig;
}

void SingletonModbusInfo::setPrintTrig(const uint16_t& value)
{
    printTrig = value;
}

uint16_t SingletonModbusInfo::getPrintFinish() const
{
    return printFinish;
}

void SingletonModbusInfo::setPrintFinish(const uint16_t& value)
{
    printFinish = value;
}

uint16_t SingletonModbusInfo::getPrintAgain() const
{
    return printAgain;
}

void SingletonModbusInfo::setPrintAgain(const uint16_t& value)
{
    printAgain = value;
}

uint16_t SingletonModbusInfo::getThisPackWeight() const
{
    return thisPackWeight;
}

void SingletonModbusInfo::setThisPackWeight(const uint16_t& value)
{
    thisPackWeight = value;
}

uint16_t SingletonModbusInfo::getThisPackQuant() const
{
    return thisPackQuant;
}

void SingletonModbusInfo::setThisPackQuant(const uint16_t& value)
{
    thisPackQuant = value;
}

uint16_t SingletonModbusInfo::getSiglePackQuant() const
{
    return siglePackQuant;
}

void SingletonModbusInfo::setSiglePackQuant(const uint16_t& value)
{
    siglePackQuant = value;
}

uint16_t SingletonModbusInfo::getSubScatteredOrderQuant() const
{
    return subScatteredOrderQuant;
}

void SingletonModbusInfo::setSubScatteredOrderQuant(const uint16_t& value)
{
    subScatteredOrderQuant = value;
}

uint16_t SingletonModbusInfo::getScanGunTriggerSignal() const
{
    return scanGunTriggerSignal;
}

void SingletonModbusInfo::setScanGunTriggerSignal(const uint16_t& value)
{
    scanGunTriggerSignal = value;
}

uint16_t SingletonModbusInfo::getZipperLength() const
{
    return zipperLength;
}

void SingletonModbusInfo::setZipperLength(const uint16_t& value)
{
    zipperLength = value;
}

uint16_t SingletonModbusInfo::getOrderTotal() const
{
    return orderTotal;
}

void SingletonModbusInfo::setOrderTotal(const uint16_t& value)
{
    orderTotal = value;
}
