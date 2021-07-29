#include "SingletonPackerData.h"
#include "PackRuleData.h"
#include "LabelInfoData.h"
#include "LabelTemplateData.h"
#include "ModbusInfo.h"

SingletonPackerData::SingletonPackerData()
{
    /** device info init **/
    packRulInfo = new PackRuleData();          // 包装规则
    labPrinInfo = new LabelInfoData();        // 标签打印
    labelTemplateInfo = new LabelTemplateData();
}

SingletonPackerData::~SingletonPackerData()
{
    delete packRulInfo;
    delete labPrinInfo;
    delete labelTemplateInfo;
}


string SingletonPackerData::getScatteredOrderQuant() const
{
    return scatteredOrderQuant;
}

void SingletonPackerData::setScatteredOrderQuant(const string& value)
{
    scatteredOrderQuant = value;
}

void SingletonPackerData::SingletonInit()
{
    LabelInfoData tempLabelData;
    PackRuleData packRulInfo;
    LabelTemplateData tempLabelTemplataData;
    this->interReportBef = 0;
    this->employeeID.clear();
    this->tickApplCode.clear();
    this->tickID.clear();
    this->labPrinInfo->setLabelInfoData(tempLabelData);
    this->setPackRulInfo(&packRulInfo);
    this->scatteredOrderQuant.clear();
    this->printTrig = 0;
    this->printFinishBef = 0;
    this->interReportBef = 0;
    this->printSecondBef = 0;
}

string SingletonPackerData::getWorkZipperSize() const
{
    return workZipperSize;
}

void SingletonPackerData::setWorkzipperSize(const string& value)
{
    workZipperSize = value;
}

uint16_t SingletonPackerData::getPrintSecondBef() const
{
    return printSecondBef;
}

void SingletonPackerData::setPrintSecondBef(const uint16_t& value)
{
    printSecondBef = value;
}

uint16_t SingletonPackerData::getInterReportBef() const
{
    return interReportBef;
}

void SingletonPackerData::setInterReportBef(const uint16_t& value)
{
    interReportBef = value;
}

uint16_t SingletonPackerData::getPrintFinishBef() const
{
    return printFinishBef;
}

void SingletonPackerData::setPrintFinishBef(const uint16_t& value)
{
    printFinishBef = value;
}

LabelInfoData* SingletonPackerData::getLabelPrintInfo() const
{
    return labPrinInfo;
}

void SingletonPackerData::setLabPrinInfo(LabelInfoData* value)
{
    labPrinInfo->setLabelInfoData(* value);
}

string SingletonPackerData::getEmployeeID() const
{
    return employeeID;
}

void SingletonPackerData::setEmployeeID(const string& value)
{
    employeeID = value;
}

uint16_t SingletonPackerData::getPrintTrig() const
{
    return printTrig;
}

void SingletonPackerData::setPrintTrig(const uint16_t& value)
{
    printTrig = value;
}

LabelTemplateData* SingletonPackerData::getLabelTemplateInfo() const
{
    return labelTemplateInfo;
}

void SingletonPackerData::setLabelTemplateInfo(LabelTemplateData* value)
{
    labelTemplateInfo = value;
}

map<string, string> SingletonPackerData::getLabelDic() const
{
    return labelDic;
}

void SingletonPackerData::setLabelDic(const map<string, string>& value)
{
    labelDic = value;
}

PackRuleData* SingletonPackerData::getPackRulInfo() const
{
    return packRulInfo;
}

void SingletonPackerData::setPackRulInfo(PackRuleData* value)
{
    packRulInfo->setOrderOrigin(value->getOrderOrigin());
    packRulInfo->setOrderTotal(value->getOrderTotal());
    packRulInfo->setPackRule(value->getPackRule());
    packRulInfo->setPerPackQuant(value->getPerPackQuant());
    packRulInfo->setTickNumber(value->getTickNumber());
    packRulInfo->setZipperLength(value->getZipperLength());
}

string SingletonPackerData::getTickApplCode() const
{
    return tickApplCode;
}

void SingletonPackerData::setTickApplCode(const string& value)
{
    tickApplCode = value;
}

string SingletonPackerData::getTickID() const
{
    return tickID;
}

void SingletonPackerData::setTickID(const string& value)
{
    tickID = value;
}


