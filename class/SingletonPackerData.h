#ifndef SINGLETONPACKERDATA_H
#define SINGLETONPACKERDATA_H

#include <string>
#include <map>

using namespace std;

class PackRuleData;
class LabelInfoData;
class LabelTemplateData;
class ModbusCommunicationInfo;

class SingletonPackerData
{
public:
    static SingletonPackerData& GetInstance()
    {
        static SingletonPackerData instance;
        return instance;
    }
    string getTickID() const;
    void setTickID(const string& value);

    string getTickApplCode() const;
    void setTickApplCode(const string& value);

    PackRuleData* getPackRulInfo() const;
    void setPackRulInfo(PackRuleData* value);

    map<string, string> getLabelDic() const;
    void setLabelDic(const map<string, string>& value);


    LabelTemplateData* getLabelTemplateInfo() const;
    void setLabelTemplateInfo(LabelTemplateData* value);

    uint16_t getPrintTrig() const;
    void setPrintTrig(const uint16_t& value);

    string getEmployeeID() const;
    void setEmployeeID(const string& value);

    LabelInfoData* getLabelPrintInfo() const;
    void setLabPrinInfo(LabelInfoData* value);

    uint16_t getPrintFinishBef() const;
    void setPrintFinishBef(const uint16_t& value);

    uint16_t getInterReportBef() const;
    void setInterReportBef(const uint16_t& value);

    uint16_t getPrintSecondBef() const;
    void setPrintSecondBef(const uint16_t& value);

    string getWorkZipperSize() const;
    void setWorkzipperSize(const string& value);

    string getScatteredOrderQuant() const;
    void setScatteredOrderQuant(const string& value);

    void SingletonInit();

private:
    SingletonPackerData();
    SingletonPackerData(const SingletonPackerData&);
    ~SingletonPackerData();
    SingletonPackerData& operator = (const SingletonPackerData&);
    string tickApplCode;                // 工票申请码
    string tickID;                      // 工票号
    string employeeID;                  // 员工工号
    string scatteredOrderQuant;         // 散单数量
    string workZipperSize;              // 正在切的拉链尺寸
    uint16_t printTrig;                 // 打印觸發信號
    uint16_t printFinishBef;            // 记录前一次打印完成数据
    uint16_t interReportBef;            // 记录前一次报工数据
    uint16_t printSecondBef;            // 记录前一次重复打印数据
    PackRuleData* packRulInfo;          // 包装规则
    LabelInfoData* labPrinInfo;         // 标签打印
    LabelTemplateData* labelTemplateInfo;
    map<string, string> labelDic;
};

#endif // SINGLEETONPACKERDATA_H
