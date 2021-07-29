#include "PrinterProcess.h"
#include "PackerBase.h"
#include "class/SingletonUserLogger.h"
#include "class/SingletonPrinter.h"
#include "LabelInfoData.h"

PrinterProcess::PrinterProcess()
{
    SingletonPrinter::GetInstance();
}

/************************************
* @Name     : PrinterInit
* @Descript : printer initialize
* @Param    :
* @Author   : sxy
* @Version  : v2.0.0
* @Date     : 2021-06-07
* @Return   : void
************************************/
int PrinterProcess::PrinterInit()
{
    device_info device;
    memset(device.name, 0, sizeof(device.name));
    device.pid = 0;
    device.vid = 0;
    SingletonPrinter::GetInstance().setDevice(&device);

    print_option print;
    print.dXPos = DEFAULT_START_X_POS;
    print.dYPos = DEFAULT_START_Y_POS;
    print.darkness = DEFAULT_DARKNESS;
    print.btype = false;
    SingletonPrinter::GetInstance().setPrint(&print);

    setdpi(DEFAULT_DPI);

    int result = enumDevice(SingletonPrinter::GetInstance().getDevice());
    if (result != 0) {
        LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), "can not find print device.");
        return SAB_R_FAIL;
    }
    LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(),
                 "find device: " + to_string(SingletonPrinter::GetInstance().getDevice()->pid));
    return SAB_R_OK;
}

/************************************
* @Name     :
* @Descript :
* @Param    :
* @Author   : sxy
* @Version  : v2.0.0
* @Date     : 2021-06-10
* @Return   :
************************************/
int PrinterProcess::PrintLabel(const LabelInfoData& labPrinInfo)
{
    int exeResult;
    char level;                     // 用在二维码
    double size;                    // 尺寸用在二维码
    char printText[64];
    string str;                     // 记录需打印的字符串
    font_option font;               // 字体
    code128_option code128;         // 条形码
    print_option prints;

    memset(printText, 0, sizeof(printText));

    exeResult = openDevice(SingletonPrinter::GetInstance().getDevice());          // 打开打印设备
    if (exeResult != 0) {
        return  SAB_R_FAIL;
    }

    startPrint();            // 开始打印
    font.fontSize = 24;      // 字体大小
    font.fontSlant = false;
    font.fontWeight = false;

    prints.dXPos = 0.20;   // 打印左边开始位置
    prints.dYPos = 0.10;   // 打印上边开始位置

    str = "款 号:" + labPrinInfo.getStyle();
    strcpy(printText, str.c_str());
    exeResult = DLPrintText(&prints, &font, printText);

    prints.dYPos = 0.30;
    str = "合同号:" + labPrinInfo.getOrderId() + " 颜色:" + labPrinInfo.getColor();
    strcpy(printText, str.c_str());
    exeResult = DLPrintText(&prints, &font, printText);

    prints.dYPos = 0.45;
    str = "型号:" + labPrinInfo.getCustomType();
    strcpy(printText, str.c_str());
    exeResult = DLPrintText(&prints, &font,  printText);

    prints.dYPos = 0.65;
    str = "尺寸:" + labPrinInfo.getSize() + labPrinInfo.getSpecs()
          + " 数量:" + labPrinInfo.getQuant() + "条"
          + " 重量:" + labPrinInfo.getWeight() + "KG";
    strcpy(printText, str.c_str());
    exeResult = DLPrintText(&prints, &font, printText);

    prints.dYPos = 0.80;
    str = "检验序号:JY00" + labPrinInfo.getProductDate()
          + labPrinInfo.getMnemCode();     // 生产日期 + 助记码
    strcpy(printText, str.c_str());
    exeResult = DLPrintText(&prints, &font, printText);

    prints.dYPos = 1.0;
    code128.codeHeight = 0.2;   //str.toDouble();
    code128.wScale = 2;         // 更改条码数字大小
    code128.hScale = 2;         // 更改条码数字大小
    code128.width = 3;
    code128.readchar = true;
    code128.readPos = false;
    str = labPrinInfo.getBarCode();
    strcpy(printText, str.c_str());
    exeResult = DLPrintCode(&prints, &code128, printText);    // 一维码
    /*
    prints.dXPos               =      2.4;
    prints.dYPos               =      0.10;
    prints.darkness       =   20;
    size                       =   1;
    level                         =    'L';
    str                        =   "得实集团abcXYZ1234567890QRcode";
    strcpy(text, str.c_str());
    res                        =   DLPrintQR(prints,level,size,text);    // 二维码
    */
    endPrint();               // 结束打印
    closeDevice(SingletonPrinter::GetInstance().getDevice());      // 关闭设备
    return     SAB_R_OK;
}
