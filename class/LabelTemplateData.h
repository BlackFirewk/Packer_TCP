#ifndef LABELTEMPLATEDATA_H
#define LABELTEMPLATEDATA_H

#include <string>
#include "lib/DLPrinter.h"

using namespace std;

class QrCodeData;

class LabelTemplateData
{
public:
    LabelTemplateData();

private:
    class LabelItemData
    {
    public:
        LabelItemData();

    private:
        string itemName;
        print_option option;
    };

    bool useBarCode;
    bool useQrCode;
    int itemAmount;
    double barCodeXPos;
    double barCodeYPos;
    string brandName;
    string qrCode;
    string barCode;
    font_option* fontOption;
    code128_option* barCodeOption;
    LabelItemData* labelItem[20];
    QrCodeData* qrCodeOption;
};

#endif // LABELTEMPLATE_H
