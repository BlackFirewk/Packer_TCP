#include "MainUnitTest.h"
#include "PackerMain.h"

MainUnitTest::MainUnitTest()
{

}

void UnitTest_PackRule(ModbusProcess& modbusProcesser, HttpConnect& httpPackConnect)
{
    SingletonPackerData::GetInstance().setTickApplCode("25118733");
    getPackerRule(modbusProcesser, httpPackConnect);
}

void UnitTest_Scattered(ModbusProcess& modbusProcesser, HttpConnect& httpPackConnect)
{
    SingletonPackerData::GetInstance().setTickApplCode("25118733");
    SingletonPackerData::GetInstance().setWorkzipperSize("14.5");
    F_Data test;
    test.floatForm = 14.5;
    getScatteredOrderQuant(modbusProcesser, httpPackConnect, test);
}

#ifdef UNIT_TEST
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

    /** 初始化ERP链接信息 **/
    HttpConnect* erpConnect = new HttpConnect();
    packerIterator = SingletonPropertiesTable::GetInstance().getPropertiesTable().find(ERP_HOST);
    erpConnect->setIpAddress(packerIterator->second.data());

    packerIterator = SingletonPropertiesTable::GetInstance().getPropertiesTable().find(ERP_PORT);
    erpConnect->setHostPort(atoi(packerIterator->second.data()));


    ERPInterface* packerRuler = new ERPInterface("AutoPackingRulebyLsbh", "lsbh");

    /** 其他初始化 **/
    /** 各类数据初始化　**/
    SingletonPackerData::GetInstance();
    SingletonTimerScheduler::GetInstance();

    /** 打印机初始化　**/
    PrinterProcess* printProces = new PrinterProcess();
    printProces->PrinterInit();

    /** 子线程初始化　**/
    LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "欢迎使用");

    packerRuler->ErpProcessor(*erpConnect, "test");
//    ModbusProcess* modbus = new ModbusProcess();
//    SubThreadInit(modbus);
//    MODBUS_INFO* modbusInfo = modbus->modbusCommun;

//    UnitTest_Scattered(*modbus, *erpConnect);
//    UnitTest_PackRule(*modbus, *erpConnect);
    while (1);
    delete erpConnect;
    delete printProces;
//    delete modbus;
}
#endif
