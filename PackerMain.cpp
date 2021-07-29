#include "PackerMain.h"

using namespace std;
/************************************
* @Name     : UnitTest_Logger
* @Descript : unit test for logger
* @Param    : null
* @Author   : sxy
* @Version  : v2.0.0
* @Date     : 2021-06-03
* @Return   : void
************************************/
void UnitTestLogger(void)
{
    /** logger test **/
    LoggerTest logger;
    logger.logger4cxxTest();
}

/************************************
* @Name     : SAB_PACKER_GetVersion
* @Descript : 取当前编码版本信息 （ 例： V1.0.1.2020.4.10,共32位）
* @Param    : null
* @Author   : sxy
* @Version  : v2.0.0
* @Date     : 2021-06-02
* @Return   : 返回版本库个数
* ps.版本信息格式为：版本号＋年（11位19~9）＋月（4位8~5）＋日（5位 4~0）
*    其中版本号为：主版本号（2位31~30）＋子版本号（4位29~26）＋修正版本号（6位25~20）
************************************/
string SabPackerGetVersion()
{
    string MAINVER = "2";   // 主版本号
    string SUBVER = "0";    // 子版本号
    string REVER = "0";     // 修正版本号
    string version = "Version-Info: ";

    version += 'V' + MAINVER + '.' + SUBVER + '.' + REVER;

    return version;
}
/************************************
* @Name     : readConfig
* @Descript : 读取配置文件
* @Param    : m - 存储配置的表
* @Author   : sxy
* @Version  : v2.0.0
* @Date     : 2021-06-02
* @Return   : 配置文件读取结果
************************************/
int ReadConfig(void)
{
    map<string, string> tempMap;
    if (false == ReadConfig(OTHER_CONFIG_PATH, tempMap)) {
        return SAB_R_FAIL;
    }
    SingletonPropertiesTable::GetInstance().setPropertiesTable(tempMap);
    #ifdef DEBUG
    PrintConfig(SingletonPropertiesTable::GetInstance().getPropertiesTable());
    #endif
    return SAB_R_OK;
}
/************************************
* @Name     : subThreadInit
* @Descript : 子线程初始化
* @Param    : 日志类
* @Author   : sxy
* @Version  : v2.0.0
* @Date     : 2021-06-07
* @Return   : void
************************************/
void SubThreadInit(ModbusProcess* modbus)
{
    pthread_t modbusCommonThread;              // modbus通讯线程

    modbus->ModbusInit();
    #ifdef USE_MODBUS_LIB
    int result = pthread_create(&modbusCommonThread, NULL, ModbusTcpProcess, modbus);    // 建立子线程，负责modbus通讯
    #endif
    int result = pthread_create(&modbusCommonThread, NULL, ModbusTcpSocketProcess, modbus);
    if (result != 0) {
        LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "modbus thread create faliure.");
    }
}
/************************************
* @Name     : getPackerRule
* @Descript : get package rule
* @Param    :
* @Author   : sxy
* @Version  : v2.0.0
* @Date     : 2021-06-09
* @Return   :
************************************/
int getPackerRule(ModbusProcess& modbusProcesser, HttpConnect& httpPackConnect)
{
    int processResult = SAB_R_FAIL;

    LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "Send Scan Gun Trigger Signal.");
    modbusProcesser.scanGunTriggerSignal = 1;
    SingletonTimerScheduler::GetInstance().getTimer1()->Start(modbusProcesser.ScanSignalClean, 5 * 1000, UserTimer::TimerType::ONCE);

    LOG4CXX_DEBUG(SingletonUserLogger::GetInstance().getLogger(), "Begin requesting wrapper rules");

    /** 初始化接口信息 **/
    ERPInterface* erpInterface = new ERPInterface("AutoPackingRulebyLsbhPP", "lsbh");

    /** 发起erp请求　**/
    processResult = erpInterface->ErpProcessor(httpPackConnect, SingletonPackerData::GetInstance().getTickApplCode());
    if (processResult == SAB_R_FAIL) {
        LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "request packaging rules faliure.");
        delete erpInterface;
        return processResult;
    } else {
        LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "request packaging rules success.");
    }
    /**  处理数据 **/
    SingletonModbusInfo::GetInstance().setSiglePackQuant(atoi(SingletonPackerData::GetInstance().getPackRulInfo()->getPerPackQuant().c_str()));
    SingletonModbusInfo::GetInstance().setZipperLength(atof(SingletonPackerData::GetInstance().getPackRulInfo()->getZipperLength().c_str()) * 10);
//    LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), SingletonModbusInfo::GetInstance().getZipperLength());
    SingletonModbusInfo::GetInstance().setOrderTotal(atoi(SingletonPackerData::GetInstance().getPackRulInfo()->getOrderTotal().c_str()));
    SingletonModbusInfo::GetInstance().setSubScatteredOrderQuant(atoi(SingletonPackerData::GetInstance().getPackRulInfo()->getOrderTotal().c_str()));

    LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(),
                 "current workticket info: {applycode: " + SingletonPackerData::GetInstance().getTickApplCode()
                 + ", task quant: " + to_string(SingletonModbusInfo::GetInstance().getOrderTotal())
                 + ", zipper length: " +  to_string(SingletonModbusInfo::GetInstance().getZipperLength())
                 + ", sigle pack quant: " + to_string(SingletonModbusInfo::GetInstance().getSiglePackQuant())
                 + "}");
    delete erpInterface;
    return processResult;
}
/************************************
* @Name     : WorkModeProcess
* @Descript : 不同模式下，erp交互处理函数
* @Param    : httpPackConnect
*             work_mode
*             modbusInfo
* @Author   : sxy
* @Version  : v2.0.0
* @Date     : 2021-06-10
* @Return   :
************************************/
int WorkModeProcess(HttpConnect& http_pack_connect, uint8_t work_mode)
{
    string workMsg;
    string barCode;
    string packQuant;
    string packWeight;
    string interfaceRequestParam;
    /** 标签打印接口　**/
    ERPInterface erpRequest("AutoPackingSavebyGph", "info");

    if (work_mode > WORK_MODE_MAX_NUMBER) {
        LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), "work mode error.");
        return SAB_R_FAIL;
    }

    switch (work_mode) {
        case WORK_MODE_TRIGGER_PRINT:
            /** 触发打印模式 **/
            workMsg = "trigger print mode";
            packQuant = to_string(SingletonModbusInfo::GetInstance().getThisPackQuant());
            packWeight = to_string(SingletonModbusInfo::GetInstance().getThisPackWeight());
            break;
        case WORK_MODE_REPORT_WORK:
            /** 机联网报工模式 **/
            workMsg = "internet report work mode";
            break;
        case WORK_MODE_FINISH_PRINT:
            /** 打印完成模式 **/
            workMsg = "finsh print work mode";
            barCode = SingletonPackerData::GetInstance().getLabelPrintInfo()->getBarCode();
            break;

        case WORK_MODE_PRINT_AGAIN:
            /**  重新打印模式 **/
            workMsg = "reprint mode";
            barCode = SingletonPackerData::GetInstance().getLabelPrintInfo()->getBarCode();
            break;
        default:
            break;
    }

//    LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "WORK_MODE: " + workMsg + " starup.");
//    LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(),
//                 "current ticket apply code:" + SingletonPackerData::GetInstance().getTickApplCode() + ".");

    interfaceRequestParam += SingletonPackerData::GetInstance().getTickApplCode() + SEPARATOR;
    interfaceRequestParam += to_string(work_mode) + SEPARATOR;
    interfaceRequestParam += packQuant + SEPARATOR;
    interfaceRequestParam += packWeight + SEPARATOR;
    interfaceRequestParam += SingletonPackerData::GetInstance().getEmployeeID() + SEPARATOR;
    interfaceRequestParam += barCode;

    erpRequest.ErpProcessor(http_pack_connect, interfaceRequestParam);
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
int PrintTriggerProcess(PrinterProcess* printProces, int operation)
{
    int result = SAB_R_FAIL;
    /* check device info */
    if ((SingletonPrinter::GetInstance().getDevice()->pid == 0) &&
            (SingletonPrinter::GetInstance().getDevice()->vid == 0)) {
        LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "scan device again! ");
        result = printProces->PrinterInit();

        if (result == SAB_R_FAIL) {
            LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), "can not find print device! ");
            return SAB_R_FAIL;
        }
    }

    if (operation == PRINT_FIRST) {
        LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(),  "print start...");
    } else if (operation == PRINT_AGAIN) {
        LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(),  "reprint start...");
    }

    result = printProces->PrintLabel(*SingletonPackerData::GetInstance().getLabelPrintInfo());
    if (result == SAB_R_FAIL) {
        LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(),  "label print operation faliure.");
    }
    return SAB_R_OK;
}
/************************************
* @Name     : getScatteredOrderQuant
* @Descript : 获取正在切的拉链尺寸/仅仅适用于散单合并的情况
* @Param    :
* @Author   : sxy
* @Version  : v2.0.0
* @Date     : 2021-06-15
* @Return   :
************************************/
int getScatteredOrderQuant(ModbusProcess& modbusProcesser, HttpConnect& httpPackConnect, F_Data& lengthData)
{
    int processResult = SAB_R_FAIL;
    LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "Begin requesting work zipper size");

    /** 初始化接口信息 **/
    ERPInterface* erpInterface = new ERPInterface("AutoPackingRulebyLsbhCCSL", "info");
    string requestParam  = SingletonPackerData::GetInstance().getTickApplCode() + SEPARATOR;
    requestParam += SingletonPackerData::GetInstance().getWorkZipperSize();

    /** 发起erp请求　**/
    processResult = erpInterface->ErpProcessor(httpPackConnect, requestParam);
    if (processResult == SAB_R_FAIL) {
        LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "request work zipper size faliure.");
        delete erpInterface;
        return processResult;
    }
    /**  处理数据 **/
    SingletonModbusInfo::GetInstance().setSubScatteredOrderQuant(
                    atoi(SingletonPackerData::GetInstance().getScatteredOrderQuant().c_str())); //更新散单数量
    SingletonModbusInfo::GetInstance().setOrderTotal(
                    SingletonModbusInfo::GetInstance().getSubScatteredOrderQuant());
    SingletonModbusInfo::GetInstance().setZipperLength((uint16_t)(lengthData.floatForm * 10));
    LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "save_ZipperLength_as: " + to_string(SingletonModbusInfo::GetInstance().getZipperLength()) + "mm");
    LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "save_ScatteredOrderQuant_as: " + to_string(SingletonModbusInfo::GetInstance().getSubScatteredOrderQuant()));
    LOG4CXX_DEBUG(SingletonUserLogger::GetInstance().getLogger(), "finsh requesting work zipper size");
    LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "Send Scan Gun Trigger Signal.");
    modbusProcesser.scanGunTriggerSignal = 1;
    SingletonTimerScheduler::GetInstance().getTimer1()->Start(modbusProcesser.ScanSignalClean, 5 * 1000, UserTimer::TimerType::ONCE);

    delete erpInterface;
    return processResult;
}
/************************************
* @Name     :
* @Descript :
* @Param    :
* @Author   : sxy
* @Version  : v2.0.0
* @Date     : 2021-06-16
* @Return   :
************************************/
#ifndef UNIT_TEST
int main()
{
    /**************** logger初始化 *******************/
    /** 加载logger配置文件 **/
    SingletonUserLogger::setLoggerProperties(LOGGER_PROPERTIES_PATH);
    SingletonUserLogger::GetInstance();

    /** 读取配置文件 **/
    map<string, string>::iterator packerIterator;
    SingletonPropertiesTable::GetInstance();
    ReadConfig();

    /** 输出版本信息 **/
    LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), SabPackerGetVersion());

    /** 初始化ERP链接信息 **/
    HttpConnect* erpConnect = new HttpConnect();
    packerIterator = SingletonPropertiesTable::GetInstance().getPropertiesTable().find(ERP_HOST);
    erpConnect->setIpAddress(packerIterator->second.data());

    packerIterator = SingletonPropertiesTable::GetInstance().getPropertiesTable().find(ERP_PORT);
    erpConnect->setHostPort(atoi(packerIterator->second.data()));


//    ERPInterface* packerRuler = new ERPInterface("AutoPackingRulebyLsbh", "lsbh");

    /** 其他初始化 **/
    /** 各类数据初始化　**/
    SingletonPackerData::GetInstance();
    SingletonTimerScheduler::GetInstance();

    /** 打印机初始化　**/
    PrinterProcess* printProces = new PrinterProcess();
    printProces->PrinterInit();

    /** 子线程初始化　**/
    ModbusProcess* modbus = new ModbusProcess();
    SubThreadInit(modbus);
    string applyCode;
    string workID;
    string employeeID;
    /** 主程序 **/
    while (1) {
        applyCode.assign((char*)SingletonModbusInfo::GetInstance().getWorkTickApplyCodeReg()); //转化工票申请码
        workID.assign((char*)SingletonModbusInfo::GetInstance().getWorkTickIDReg());
        employeeID.assign((char*)SingletonModbusInfo::GetInstance().getEmployeeIDReg()); //转化员工工号

        if (applyCode.length() == WORK_TICKET_APPLYID_LENGTH) {
            if (applyCode.compare(SingletonPackerData::GetInstance().getTickApplCode()) != 0) {
                LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "find new task: " + applyCode);
                // 字符串不同, 表示新工票，获取打印规则
                SingletonPackerData::GetInstance().setTickApplCode(applyCode);
                SingletonPackerData::GetInstance().setEmployeeID(employeeID);
                SingletonPackerData::GetInstance().setTickID(workID);
                getPackerRule(*modbus, *erpConnect);
            } else {
                //工票号不变的情况下，判断是否是新尺寸
//                float fLengthTemp = atof(SingletonPackerData::GetInstance().getWorkZipperSize().c_str());
//                F_Data currentLengthTemp;
//                currentLengthTemp.uintForm = SingletonModbusInfo::GetInstance().getCurrentZipperLength()[1]
//                                             + (SingletonModbusInfo::GetInstance().getCurrentZipperLength()[0] << 16);
//                if (currentLengthTemp.floatForm != fLengthTemp) {
//                    SingletonPackerData::GetInstance().setWorkzipperSize(to_string(currentLengthTemp.floatForm));
//                    //getScatteredOrderQuant(*modbus, *erpConnect, currentLengthTemp);
//                }
            }

            //　判断是否获取打印上升沿
            if ((SingletonPackerData::GetInstance().getPrintTrig() == 0) && (SingletonModbusInfo::GetInstance().getPrintTrig() == 1)) {
                WorkModeProcess(*erpConnect, WORK_MODE_TRIGGER_PRINT);
                //print
                PrintTriggerProcess(printProces, PRINT_FIRST);
            }
            SingletonPackerData::GetInstance().setPrintTrig(SingletonModbusInfo::GetInstance().getPrintTrig()); // 更新printTrig状态值

            //判断是否获取到报工上升沿
//            if ((SingletonPackerData::GetInstance().getInterReportBef() == 0) && (modbusInfo->interReport == 1)) {                        // 获取到报工上升沿时，进入报工处理
//                WorkModeProcess(*erpConnect, WORK_MODE_REPORT_WORK, *modbusInfo);
//            }
//            SingletonPackerData::GetInstance().setInterReportBef(modbusInfo->interReport);

            // 判断是否有打印完成上升沿
            if ((SingletonPackerData::GetInstance().getPrintFinishBef() == 0) && (SingletonModbusInfo::GetInstance().getPrintFinish() == 1)) {
                WorkModeProcess(*erpConnect, WORK_MODE_FINISH_PRINT);
            }
            SingletonPackerData::GetInstance().setPrintFinishBef(SingletonModbusInfo::GetInstance().getPrintFinish());  // 更新printFinish_bef状态值

            if ((SingletonPackerData::GetInstance().getPrintSecondBef() == 0) &&  (SingletonModbusInfo::GetInstance().getPrintAgain() == 1) ) {                        // 获取到重新打印上升沿时，进入打印完成处理
                WorkModeProcess(*erpConnect, WORK_MODE_PRINT_AGAIN);
                PrintTriggerProcess(printProces, PRINT_AGAIN);
            }
            SingletonPackerData::GetInstance().setPrintSecondBef(SingletonModbusInfo::GetInstance().getPrintAgain());  // 更新printFinish_bef状态值
        } else {
            SingletonPackerData::GetInstance().SingletonInit();
            SingletonModbusInfo::GetInstance().setOrderTotal(0);
            SingletonModbusInfo::GetInstance().setZipperLength(0);
            SingletonModbusInfo::GetInstance().setSiglePackQuant(0);
            memset(SingletonModbusInfo::GetInstance().getBarCodeReg(), 0,
                   sizeof(*SingletonModbusInfo::GetInstance().getBarCodeReg()));
            applyCode.clear();
            workID.clear();
            employeeID.clear();
//            LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "not work ticket.");
        }
        SingletonTimerScheduler::GetInstance().getTimerManager()->DetectTimers();
    }
    delete erpConnect;
    delete printProces;
    delete modbus;
}
#endif
