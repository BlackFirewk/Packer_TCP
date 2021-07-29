#include "LabelInfoData.h"

LabelInfoData::LabelInfoData()
{

}

void LabelInfoData::setLabelInfoData(LabelInfoData& data)
{
    this->barCode = data.getBarCode();                              // 条码号
    this->orderId = data.getOrderId();                              // 订单号
    this->matteName = data.getMatteName();                          // 物料名称
    this->color = data.getColor();                                  // 颜色
    this->size = data.getSize();                                    // 尺寸
    this->specs = data.getSpecs();                                  // 规格
    this->quant = data.getQuant();                                  // 数量
    this->weight = data.getWeight();                                // 重量
    this->style = data.getStyle();                                  // 款号
    this->mnemCode = data.getMnemCode();                            // 助记码
    this->productDate = data.getProductDate();                      // 生产日期
    this->matteEnglishName = data.getMatteEnglishName();            // 英文物料名称
    this->EnglishSpecs = data.getEnglishSpecs();                    // 英文规格
    this->customNumber = data.getCustomNumber();                    // 客户单号
    this->customStyle = data.getStyle();                            // 客户款号
    this->customType = data.getCustomType();                        // 客户型号
    this->customColor = data.getCustomColor();                      // 客户色号
    this->tabMatteName = data.getTabMatteName();                    // 拉片物料名称
    this->platColorEnglishName = data.getPlatColorEnglishName();    // 电镀颜色英文名称
}

string LabelInfoData::getBarCode() const
{
    return barCode;
}

void LabelInfoData::setBarCode(const string& value)
{
    barCode = value;
}

string LabelInfoData::getOrderId() const
{
    return orderId;
}

void LabelInfoData::setOrderId(const string& value)
{
    orderId = value;
}

string LabelInfoData::getMatteName() const
{
    return matteName;
}

void LabelInfoData::setMatteName(const string& value)
{
    matteName = value;
}

string LabelInfoData::getColor() const
{
    return color;
}

void LabelInfoData::setColor(const string& value)
{
    color = value;
}

string LabelInfoData::getSize() const
{
    return size;
}

void LabelInfoData::setSize(const string& value)
{
    size = value;
}

string LabelInfoData::getSpecs() const
{
    return specs;
}

void LabelInfoData::setSpecs(const string& value)
{
    specs = value;
}

string LabelInfoData::getQuant() const
{
    return quant;
}

void LabelInfoData::setQuant(const string& value)
{
    quant = value;
}

string LabelInfoData::getWeight() const
{
    return weight;
}

void LabelInfoData::setWeight(const string& value)
{
    weight = value;
}

string LabelInfoData::getStyle() const
{
    return style;
}

void LabelInfoData::setStyle(const string& value)
{
    style = value;
}

string LabelInfoData::getMnemCode() const
{
    return mnemCode;
}

void LabelInfoData::setMnemCode(const string& value)
{
    mnemCode = value;
}

string LabelInfoData::getProductDate() const
{
    return productDate;
}

void LabelInfoData::setProductDate(const string& value)
{
    productDate = value;
}

string LabelInfoData::getMatteEnglishName() const
{
    return matteEnglishName;
}

void LabelInfoData::setMatteEnglishName(const string& value)
{
    matteEnglishName = value;
}

string LabelInfoData::getEnglishSpecs() const
{
    return EnglishSpecs;
}

void LabelInfoData::setEnglishSpecs(const string& value)
{
    EnglishSpecs = value;
}

string LabelInfoData::getCustomNumber() const
{
    return customNumber;
}

void LabelInfoData::setCustomNumber(const string& value)
{
    customNumber = value;
}

string LabelInfoData::getCustomStyle() const
{
    return customStyle;
}

void LabelInfoData::setCustomStyle(const string& value)
{
    customStyle = value;
}

string LabelInfoData::getCustomType() const
{
    return customType;
}

void LabelInfoData::setCustomType(const string& value)
{
    customType = value;
}

string LabelInfoData::getCustomColor() const
{
    return customColor;
}

void LabelInfoData::setCustomColor(const string& value)
{
    customColor = value;
}

string LabelInfoData::getTabMatteName() const
{
    return tabMatteName;
}

void LabelInfoData::setTabMatteName(const string& value)
{
    tabMatteName = value;
}

string LabelInfoData::getPlatColorEnglishName() const
{
    return platColorEnglishName;
}

void LabelInfoData::setPlatColorEnglishName(const string& value)
{
    platColorEnglishName = value;
}
