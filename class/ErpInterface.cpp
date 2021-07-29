#include "ErpInterface.h"
#include "SingletonUserLogger.h"
#include "SingletonPackerData.h"
#include "PackerBase.h"
#include "httplib.h"
#include "HttpConnect.h"
#include "LabelInfoData.h"
#include "LabelTemplateData.h"
#include "tool/UniversalTool.h"
#include "PackRuleData.h"

ERPInterface::ERPInterface()
{

}

ERPInterface::ERPInterface(const string& name, const string& attribute)
{
    this->interfaceName = name;
    this->interfaceAttribute = attribute;
}

string ERPInterface::getInterfaceName() const
{
    return interfaceName;
}

void ERPInterface::setInterfaceName(const string& value)
{
    interfaceName = value;
}

string ERPInterface::getInterfaceAttribute() const
{
    return interfaceAttribute;
}

void ERPInterface::setInterfaceAttribute(const string& value)
{
    interfaceAttribute = value;
}

/************************************************************************
* @Name     : CheckXml
* @Function : check Xml
* @Param    :
* @Author   : sxy
* @Date     : 2021-01-06
* @Operation: modify
* @Return   : process result
************************************************************************/
xml_node ERPInterface::CheckXml(xml_document& srcXml)
{
    string analysisResult;
    xml_node root;
    xml_node currentElement;
    root = srcXml.root();
    currentElement = root.child("soap:Envelope");

    if (currentElement == xml_node()) {
        LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), "miss element soap:Envelope");
        return  xml_node();
    }

    root = currentElement;
    currentElement = root.child("soap:Body");

    if (currentElement == xml_node()) {
        LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), "miss element soap:Body");
        return  xml_node();
    }

    root = currentElement;
    currentElement = root.child("AutoPackingRulebyLsbhPPResponse");

    if (currentElement == xml_node()) {
        currentElement = root.child("AutoPackingSavebyGphPPResponse");
        if (currentElement == xml_node()) {
            currentElement = root.child("AutoPackingLabelTemplateResponse");
            if (currentElement == xml_node()) {
                currentElement = root.child("AutoPackingRulebyLsbhCCSLResponse");
                if (currentElement == xml_node()) {
                    LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), "miss response")
                    return  xml_node();
                }
            }
        }
    }

    root = currentElement;
    currentElement = root.first_child();
    string name = currentElement.name();
    if (!strcmp(currentElement.name(), "")) {
        LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), "miss Result");
        return xml_node();
    }

    return currentElement;
}

/************************************
* @Name     :
* @Descript :
* @Param    :
* @Author   : sxy
* @Version  : v2.0.0
* @Date     : 2021-06-09
* @Return   :
************************************/
string ERPInterface::bulidErpRequestInfo(string& api_name, string& api_pro, const string& interface_param)
{
    string sendInfo = "<soap:Envelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\"><soap:Body><";
    string afAPI    = " xmlns=\"http://tempuri.org/\"><"; //info>";
    string afAPIpro = ">";
    string afinf    = "</";  // info></";
    string afinfPro = "></";
    string endstr   = "></soap:Body></soap:Envelope>";
    string Response = "Response";
    string Result   = "Result";

    sendInfo = sendInfo + api_name + afAPI + api_pro + afAPIpro + interface_param \
               + afinf + api_pro + afinfPro + api_name + endstr;      // 字符连接
    LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), sendInfo);
    return sendInfo;
}
/************************************
* @Name     :
* @Descript :
* @Param    :
* @Author   : sxy
* @Version  : v2.0.0
* @Date     : 2021-06-09
* @Return   :
************************************/
int ParseLabelTemplate(xml_node& srcData, LabelTemplateData* labelTemplate)
{
//    xml_node* root = &srcData;
//    labelTemplate->brandName = srcData.child("BrandName").first_child().value();
//    /* font Parse */
//    memset(labelTemplate->fontOption.fontName, 0, sizeof(labelTemplate->fontOption.fontName));
//    strcpy(labelTemplate->fontOption.fontName, srcData.child("FontName").first_child().value());
//    labelTemplate->fontOption.fontSize = atoi(srcData.child("FontSize").first_child().value());

//    if (atoi(srcData.child("FontSlant").first_child().value()) == 0) {
//        labelTemplate->fontOption.fontSlant = false;
//    } else {
//        labelTemplate->fontOption.fontSlant = true;
//    }

//    if (atoi(srcData.child("FontWeight").first_child().value()) == 0) {
//        labelTemplate->fontOption.fontWeight = false;
//    } else {
//        labelTemplate->fontOption.fontWeight = true;
//    }

//    /* barCode Parse */
//    if (atoi(srcData.child("BarCodeUse").first_child().value()) == 1) {
//        labelTemplate->useBarCode = true;
//        labelTemplate->barCodeXPos = atof(srcData.child("BarCodeXPos").first_child().value());
//        labelTemplate->barCodeYPos = atof(srcData.child("BarCodeYPos").first_child().value());
//        labelTemplate->barCodeOption.codeHeight = atof(srcData.child("BarHeight").first_child().value());
//        labelTemplate->barCodeOption.wScale = atoi(srcData.child("BarWScale").first_child().value());
//        labelTemplate->barCodeOption.hScale = atoi(srcData.child("BarHScale").first_child().value());
//        labelTemplate->barCodeOption.width = atoi(srcData.child("BarWidth").first_child().value());

//        if (atoi(srcData.child("BarReadchar").first_child().value()) == 0) {
//            labelTemplate->barCodeOption.readchar = false;
//        } else {
//            labelTemplate->barCodeOption.readchar = true;
//        }

//        if (atoi(srcData.child("BarReadPos").first_child().value()) == 0) {
//            labelTemplate->barCodeOption.readPos = false;
//        } else {
//            labelTemplate->barCodeOption.readPos = true;
//        }
//    } else {
//        labelTemplate->useBarCode = false;
//    }

//    /* qrCode Parse */
//    if (atoi(srcData.child("QrCodeUse").first_child().value()) == 1) {
//        labelTemplate->useQrCode = true;
//        labelTemplate->qrCodeOption.darkness = atoi(srcData.child("QrCodeDarkness").first_child().value());
//        labelTemplate->qrCodeOption.level = srcData.child("QrCodeLevel").first_child().value();
//        labelTemplate->qrCodeOption.size = atof(srcData.child("QrCodeSize").first_child().value());
//        labelTemplate->qrCodeOption.xPos = atof(srcData.child("QrCodeXPos").first_child().value());
//        labelTemplate->qrCodeOption.yPos = atof(srcData.child("QrCodeYPos").first_child().value());
//    } else {
//        labelTemplate->useQrCode = false;
//    }

//    /* items Parse */
//    xml_node nodeItems = root->child("Items");
//    xml_node* items = &nodeItems;
//    int i = 0;

//    for (xml_node item = items->child("SubItem"); item; item = item.next_sibling("SubItem")) {
//        labelTemplate->labelItem[i].itemName = item.attribute("Name").value();
//        labelTemplate->labelItem[i].option.dXPos = atof(item.attribute("XPos").value());
//        labelTemplate->labelItem[i].option.dYPos = atof(item.attribute("YPos").value());
//        //labelTemplate->labelItem[i].option.darkness = 3;
//        labelTemplate->labelItem[i].option.darkness = atoi(item.attribute("Darkness").value());
//        labelTemplate->labelItem[i].option.btype = 1;
//        i++;
//    }

//    labelTemplate->itemAmount = i;
    return SAB_R_OK;
}
/************************************
* @Name     : setPackRule
* @Descript : set pack rule information
* @Param    :
* @Author   : sxy
* @Version  : v1.0.0
* @Date     : 2021-01-06
************************************/
int SetPackRule(PackRuleData* packRule, vector<string> src)
{
    packRule->setTickNumber(src[1]);
    packRule->setPerPackQuant(src[2]);
    packRule->setPackRule(src[3]);
    packRule->setZipperLength(src[4]);
    packRule->setOrderTotal(src[5]);;
    return SAB_R_OK;
}
/************************************
* @Name     : updateLabelPrintInfo
* @Descript : update label content map
* @Param    :
* @Author   : sxy
* @Version  : v1.0.0
* @Date     : 2021-02-20
************************************/
void updateLabelPrintInfoMap(const char* targetName, string& newData)
{
    map<std::string, std::string> labelMap =  SingletonPackerData::GetInstance().getLabelDic();
    labelMap.erase(targetName);
    labelMap.insert(std::map<std::string, std::string> ::value_type(targetName, newData));
    SingletonPackerData::GetInstance().setLabelDic(labelMap);
}

/************************************
* @Name     : setPrintInfo
* @Descript : set label print information
* @Param    : printInfo -- ponit of ERP_LabelPrint struct
*             src       -- source data
* @Author   : sxy
* @Version  : v1.0.0
* @Date     : 2021-01-06
************************************/
int SetPrintInfo(LabelInfoData* printInfo, vector<string> src)
{
    //TODO：　有时间的话，使用反射实现获取变量名是最优解
    printInfo->setBarCode(src[1]);
    updateLabelPrintInfoMap("barCode",  src[1]);
    printInfo->setOrderId(src[2]);
    updateLabelPrintInfoMap("orderId",  src[2]);
    printInfo->setMatteName(src[3]);
    updateLabelPrintInfoMap("matteName", src[3]);
    printInfo->setColor(src[4]);
    updateLabelPrintInfoMap("color",    src[4]);
    printInfo->setSize(src[5]);
    updateLabelPrintInfoMap("size",     src[5]);
    printInfo->setSpecs(src[6]);
    updateLabelPrintInfoMap("specs",    src[6]);
    printInfo->setQuant(src[7]);
    updateLabelPrintInfoMap("quant",    src[7]);
    printInfo->setWeight(src[8]);
    updateLabelPrintInfoMap("weight",   src[8]);
    printInfo->setStyle(src[9]);
    updateLabelPrintInfoMap("style",    src[9]);
    printInfo->setMnemCode(src[10]);
    updateLabelPrintInfoMap("mnemCode",     src[10]);
    printInfo->setProductDate(src[11]);
    updateLabelPrintInfoMap("producDate",   src[11]);
    printInfo->setMatteEnglishName(src[12]);
    updateLabelPrintInfoMap("matteEngName", src[12]);
    printInfo->setEnglishSpecs(src[13]);
    updateLabelPrintInfoMap("EngSpecs",     src[13]);
    printInfo->setCustomNumber(src[14]);
    updateLabelPrintInfoMap("custNumber",   src[14]);
    printInfo->setCustomStyle(src[15]);
    updateLabelPrintInfoMap("custStyle",    src[15]);
    printInfo->setCustomType(src[16]);
    updateLabelPrintInfoMap("custType",     src[16]);
    printInfo->setCustomColor(src[17]);
    updateLabelPrintInfoMap("custColor",    src[17]);
    printInfo->setTabMatteName(src[18]);
    updateLabelPrintInfoMap("tabMatteName", src[18]);
    printInfo->setPlatColorEnglishName(src[19]);
    updateLabelPrintInfoMap("PlatColEngName", src[19]);
    return SAB_R_OK;
}
/************************************************************************
* @Name     : EepProcessor
* @Function : 上位机与ERP交互
* @Param    : handle    - I/O   算法句柄  IP_addr    - I     IP地址
*             host_port - I     主机端口号 ApiName    - I     API名称
*             APiPro    - I     API处理的标识名称
*             input - I     根据API接口，组包输入参数
*             output    - O     输出参数
* @Author   : sxy
* @Date     : 2020-12-23
* @Operation: create
* @Return   : process resultz
************************************************************************/
int ERPInterface::ErpProcessor(HttpConnect http, const string& interface_param)
{
    string sendInfo;

    httplib::Client  client(http.getIpAddress(), http.getHostPort());
    sendInfo = bulidErpRequestInfo(interfaceName, interfaceAttribute, interface_param);

    /** request erp **/
    auto erpRes = client.Post("/Service1.asmx", sendInfo, "text/xml");
    if ((!erpRes) || (200 != erpRes->status)) {
        LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), "ERP request faliure.");
        LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), "current call interface name:" + interfaceName);
        return SAB_R_FAIL;
    }
    string xmlStr = erpRes->body;
    LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "ResponeData: " + xmlStr);
    if (xmlStr.empty()) {
        LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), "ResponeData is empty!");
        return SAB_R_FAIL;
    }

    xml_document doc;
    xml_node  checkResultElement;

    doc.load_string(xmlStr.c_str());          //paser string to xml
    checkResultElement = CheckXml(doc);

    if (checkResultElement == xml_node()) {
        return SAB_R_FAIL;
    }

    if (!strcmp(checkResultElement.name(), "AutoPackingLabelTemplateResult")) {
        LabelTemplateData resultData;
        if (!ParseLabelTemplate(checkResultElement, &resultData)) {
            LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), "Label Template Parse Failure!");
            return SAB_R_FAIL;
        }
    } else {
        vector<string> resultMessage = UniversalTool::AnalysisResultString(checkResultElement.first_child().value());

        if (resultMessage.empty()) {
            return SAB_R_FAIL;
        }

        if (1 == resultMessage.size()) {
            LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), resultMessage[0]);
            return SAB_R_FAIL;
        }

        /* packing rule */
        if (!strcmp(checkResultElement.name(), "AutoPackingRulebyLsbhPPResult")) {
            PackRuleData resultPackRule;
            SetPackRule(&resultPackRule, resultMessage);
            SingletonPackerData::GetInstance().setPackRulInfo(&resultPackRule);
        }/* label content */
        else if (!strcmp(checkResultElement.name(), "AutoPackingSavebyGphPPResult")) {
            LabelInfoData resultLabelPrint;
            SetPrintInfo(&resultLabelPrint, resultMessage);
            SingletonPackerData::GetInstance().setLabPrinInfo(&resultLabelPrint);
        } else if (!strcmp(checkResultElement.name(), "AutoPackingRulebyLsbhCCSLResult")) {
            SingletonPackerData::GetInstance().setScatteredOrderQuant(resultMessage[1]);
        } else {
            LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), "ERP Request Parse Failure!");
            return SAB_R_FAIL;
        }
    }
    /************** author sxy **************/
    return SAB_R_OK;
}

