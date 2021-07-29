#ifndef LABELINFO_H
#define LABELINFO_H

#include <string>
using namespace std;

class LabelInfoData
{
public:
    LabelInfoData();
    void setLabelInfoData(LabelInfoData&);

    string getBarCode() const;
    void setBarCode(const string& value);

    string getOrderId() const;
    void setOrderId(const string& value);

    string getMatteName() const;
    void setMatteName(const string& value);

    string getColor() const;
    void setColor(const string& value);

    string getSize() const;
    void setSize(const string& value);

    string getSpecs() const;
    void setSpecs(const string& value);

    string getQuant() const;
    void setQuant(const string& value);

    string getWeight() const;
    void setWeight(const string& value);

    string getStyle() const;
    void setStyle(const string& value);

    string getMnemCode() const;
    void setMnemCode(const string& value);

    string getProductDate() const;
    void setProductDate(const string& value);

    string getMatteEnglishName() const;
    void setMatteEnglishName(const string& value);

    string getEnglishSpecs() const;
    void setEnglishSpecs(const string& value);

    string getCustomNumber() const;
    void setCustomNumber(const string& value);

    string getCustomStyle() const;
    void setCustomStyle(const string& value);

    string getCustomType() const;
    void setCustomType(const string& value);

    string getCustomColor() const;
    void setCustomColor(const string& value);

    string getTabMatteName() const;
    void setTabMatteName(const string& value);

    string getPlatColorEnglishName() const;
    void setPlatColorEnglishName(const string& value);

private:
    string retParam;       // ture / false
    // 1：触发打印; 4：重新打印，反馈如下数据
    //////////////////////////
    string barCode;                 // 条码号
    string orderId;                 // 订单号
    string matteName;               // 物料名称
    string color;                   // 颜色
    string size;                    // 尺寸
    string specs;                   // 规格
    string quant;                   // 数量
    string weight;                  // 重量
    string style;                   // 款号
    string mnemCode;                // 助记码
    string productDate;             // 生产日期
    string matteEnglishName;        // 英文物料名称
    string EnglishSpecs;            // 英文规格
    string customNumber;            // 客户单号
    string customStyle;             // 客户款号
    string customType;              // 客户型号
    string customColor;             // 客户色号
    string tabMatteName;            // 拉片物料名称
    string platColorEnglishName;    // 电镀颜色英文名称
};

#endif // LABELINFO_H
