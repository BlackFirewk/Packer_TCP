#include "SingletonModbusInfo.h"

SingletonModbusInfo::SingletonModbusInfo()
{
    cutPlcStateReg = new uint16_t[2];
    memset(cutPlcStateReg, 0, 4);

    currentZipperLength = new uint16_t[2];
    memset(currentZipperLength, 0, 2);

    employeeIDReg = new uint16_t[64];
    memset(employeeIDReg, 0, 128);

    workTickIDReg = new uint16_t[64];
    memset(workTickIDReg, 0, 128);

    workTickApplyCodeReg = new uint16_t[64];
    memset(workTickApplyCodeReg, 0, 128);

    orderQuantityReg = new uint16_t[2];
    memset(orderQuantityReg, 0, 4);

    barCodeReg = new uint16_t[64];
    memset(barCodeReg, 0, 128);

    packerStateReg = new uint16_t[5];
    memset(packerStateReg, 0, 10);
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
