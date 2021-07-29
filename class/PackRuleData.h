#ifndef PACKRULEDATA_H
#define PACKRULEDATA_H

#include <string>
using namespace std;

class PackRuleData
{
public:
    PackRuleData();
    PackRuleData(const PackRuleData&);

    string getTickNumber() const;
    void setTickNumber(const string& value);

    string getPerPackQuant() const;
    void setPerPackQuant(const string& value);

    string getPackRule() const;
    void setPackRule(const string& value);

    string getZipperLength() const;
    void setZipperLength(const string& value);

    string getOrderTotal() const;
    void setOrderTotal(const string& value);

    string getOrderOrigin() const;
    void setOrderOrigin(const string& value);

private:
    string tickNumber;      // 工票号
    string perPackQuant;    // 每包数量
    string packRule;        // 包装要求
    string zipperLength;    // 拉链长度
    string orderTotal;      // 订单总数
    string orderOrigin;     // 订单来源
};

#endif // PACKRULE_H
