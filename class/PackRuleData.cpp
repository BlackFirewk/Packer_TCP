#include "PackRuleData.h"

PackRuleData::PackRuleData()
{
    tickNumber = "\0";      // 工票号
    perPackQuant = "\0";    // 每包数量
    packRule = "\0";        // 包装要求
    zipperLength = "\0";    // 拉链长度
    orderTotal = "\0";      // 订单总数
    orderOrigin = "\0";     // 订单来源
}

PackRuleData::PackRuleData(const PackRuleData& packData)
{
    tickNumber = packData.getTickNumber();
    perPackQuant = packData.getPerPackQuant();
    packRule = packData.getPackRule();
    zipperLength = packData.getZipperLength();
    orderTotal = packData.getOrderTotal();
    orderOrigin = packData.getOrderOrigin();
}

string PackRuleData::getTickNumber() const
{
    return tickNumber;
}

void PackRuleData::setTickNumber(const string& value)
{
    tickNumber = value;
}

string PackRuleData::getPerPackQuant() const
{
    return perPackQuant;
}

void PackRuleData::setPerPackQuant(const string& value)
{
    perPackQuant = value;
}

string PackRuleData::getPackRule() const
{
    return packRule;
}

void PackRuleData::setPackRule(const string& value)
{
    packRule = value;
}

string PackRuleData::getZipperLength() const
{
    return zipperLength;
}

void PackRuleData::setZipperLength(const string& value)
{
    zipperLength = value;
}

string PackRuleData::getOrderTotal() const
{
    return orderTotal;
}

void PackRuleData::setOrderTotal(const string& value)
{
    orderTotal = value;
}

string PackRuleData::getOrderOrigin() const
{
    return orderOrigin;
}

void PackRuleData::setOrderOrigin(const string& value)
{
    orderOrigin = value;
}
