#include <sys/time.h>
#include "ModbusProcess.h"
#include "PackerBase.h"
#include "tool/UniversalTool.h"
#include "lib/modbus-tcp.h"
#include "class/SingletonUserLogger.h"
#include "class/SingletonPropertiesTable.h"
#include "UserTimer.h"
#include "SingletonTimerScheduler.h"
#include "SingletonModbusInfo.h"
#include "PackerModbusTCP.h"
#include "PackerTcpSocket.h"

uint16_t ModbusProcess::scanGunTriggerSignal = 0;
#ifndef DEBUG
    #define ENABLE_PACKER
#endif
ModbusProcess::ModbusProcess()
{
    #ifdef USE_MODBUS_LIB
    cutModbusConnect = new ModbusProcess::UserModbus();
    packerModbusConnect = new ModbusProcess::UserModbus();
    #endif

    packertModbusTcp = new PackerModbusTcp();
}

ModbusProcess::~ModbusProcess()
{
    #ifdef USE_MODBUS_LIB
    delete cutModbusConnect;
    delete packerModbusConnect;
    #endif
    delete packertModbusTcp;
}

#ifdef USE_MODBUS_LIB
ModbusProcess::UserModbus* ModbusProcess::getCutModbusConnect() const
{
    return cutModbusConnect;
}

void ModbusProcess::setCutModbusConnect(UserModbus* value)
{
    cutModbusConnect = value;
}
ModbusProcess::UserModbus* ModbusProcess::getPackerModbusConnect() const
{
    return packerModbusConnect;
}

void ModbusProcess::setPackerModbusConnect(UserModbus* value)
{
    packerModbusConnect = value;
}
#endif

ModbusProcess::UserModbus::UserModbus()
{
    this->port = 0;
    this->ctx = nullptr;
    this->address = "\0";
    this->slaveAddress = 0;
}

ModbusProcess::UserModbus::UserModbus(string address, int port, int slaveNum)
{
    this->port = port;
    this->ctx = nullptr;
    this->address = address;
    this->slaveAddress = slaveNum;
}
/************************************
* @Name     : ModbusInit
* @Descript : modbus process 数据初始化
* @Param    : null
* @Author   : sxy
* @Version  : v2.0.0
* @Date     : 2021-06-08
* @Return   : void
************************************/
void ModbusProcess::ModbusInit()
{
    map<string, string>::iterator packerIterator;
    map<string, string>::iterator tableEnd;
    #ifdef USE_MODBUS_LIB
    map<string, string>::iterator packerIterator;
    map<string, string>::iterator tableEnd;

    tableEnd = SingletonPropertiesTable::GetInstance().getPropertiesTable().end();
    packerIterator = SingletonPropertiesTable::GetInstance().getPropertiesTable().find(MODBUS1_HOST);

    if (packerIterator == tableEnd) {
        LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), "can not find cutModbus(modbus1) host config.")
        return;
    }
    cutModbusConnect->address = packerIterator->second.data();


    packerIterator = SingletonPropertiesTable::GetInstance().getPropertiesTable().find(MODBUS1_PORT);
    if (packerIterator == tableEnd) {
        LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), "can not find cutModbus(modbus1) host config.")
        return;
    }
    cutModbusConnect->port = atoi(packerIterator->second.data());
    cutModbusConnect->slaveAddress = 1;
    cutModbusConnect->modbusName = "cuttermodbus";
    LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(),
                 "Cutter ModbusTcp Info: Ipaddress-" + cutModbusConnect->address \
                 + " Port-" + to_string(cutModbusConnect->port) \
                 + " SlaveNumber-" + to_string(cutModbusConnect->slaveAddress));

    packerIterator = SingletonPropertiesTable::GetInstance().getPropertiesTable().find(MODBUS2_HOST);
    if (packerIterator == tableEnd) {
        LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), "can not find packerModbus(modbus2) host config.")
        return;
    }
    packerModbusConnect->address = packerIterator->second.data();

    packerIterator = SingletonPropertiesTable::GetInstance().getPropertiesTable().find(MODBUS2_PORT);
    if (packerIterator == tableEnd) {
        LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), "can not find packerModbus(modbus2) port config.")
        return;
    }
    packerModbusConnect->port = atoi(packerIterator->second.data());
    packerModbusConnect->slaveAddress = 1;
    packerModbusConnect->modbusName = "packermodbus";
    LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(),
                 "Packer Modbus Info:: IpAddress-" + packerModbusConnect->address \
                 +" Port-" + to_string(packerModbusConnect->port) \
                 +" SlaveNum-" + to_string(packerModbusConnect->slaveAddress));


    int initResult = ModbusTcpInit(cutModbusConnect);
    if (initResult == SAB_R_FAIL) {
        return;
    }

    initResult = ModbusTcpInit(packerModbusConnect);
    if (initResult == SAB_R_FAIL) {
        return;
    }
    #else
    ModbusTcpInit(MODBUS1_HOST, MODBUS1_PORT, packertModbusTcp->getCutTcpSocket(), 2);
    #ifdef ENABLE_PACKER
    ModbusTcpInit(MODBUS2_HOST, MODBUS2_PORT, packertModbusTcp->getPackerTcpSocket(), 1);
    #endif
    #endif
}

/************************************
* @Name     : ModbusTcpInit
* @Descript : modbus process tcp initialize
* @Param    : modbus —— UserModbus info
* @Author   : sxy
* @Version  : v2.0.0
* @Date     : 2021-06-08
* @Return   : init result
************************************/
int ModbusProcess::ModbusTcpInit(UserModbus* modbus)
{
    int result;

    modbus->ctx = modbus_new_tcp(modbus->address.data(), modbus->port);

    result = modbus_set_slave(modbus->ctx, modbus->slaveAddress);
    if (result != 0) {
        LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "modbus: " + modbus->modbusName + " slave set faliure.");
        return SAB_R_FAIL;
    }

    result = modbus_connect(modbus->ctx);
    if (result != 0) {
        string info = "modbus: " + modbus->modbusName + " connect faliure.";
        LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), info);
        return SAB_R_FAIL;
    }
    return SAB_R_OK;
}
/************************************
* @Name     :
* @Descript :
* @Param    :
* @Author   : sxy
* @Version  : v2.0.0
* @Date     : 2021-07-01
* @Return   :
************************************/
void ModbusProcess::ModbusTcpInit(const char* hostName, const char* portName, PackerTcpSocket* targetScket, int modbusSlave)
{
    map<string, string>::iterator packerIterator;
    map<string, string>::iterator tableEnd;
    PackerTcpSocket* tempSocket = new PackerTcpSocket();

    if (modbusSlave == 2) {
        tempSocket->setSocketName("Cutter-Modbus");
    } else if (modbusSlave == 1) {
        tempSocket->setSocketName("Packer-Modbus");
    } else {
        LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), "Unsupported slave number");
        delete tempSocket;
    }

    tableEnd = SingletonPropertiesTable::GetInstance().getPropertiesTable().end();
    packerIterator = SingletonPropertiesTable::GetInstance().getPropertiesTable().find(hostName);
    if (packerIterator == tableEnd) {
        LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), "can not find " + to_string(*hostName) + "host config.")
        delete tempSocket;
        return;
    }
    tempSocket->setSocketHost(packerIterator->second.data());

    packerIterator = SingletonPropertiesTable::GetInstance().getPropertiesTable().find(portName);
    if (packerIterator == tableEnd) {
        LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), "can not find" + to_string(*hostName) + " prot config.")
        delete tempSocket;
        return;
    }
    tempSocket->setSocketPort(atoi(packerIterator->second.data()));
    tempSocket->setSlaveNumber(modbusSlave);
    targetScket->init(tempSocket);

    LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), targetScket->getSocketName()
                 + " Info: Ipaddress: " + targetScket->getSocketHost()\
                 + " Port: " + to_string(targetScket->getSocketPort()) \
                 + " SlaveNumber: " + to_string(targetScket->getSlaveNumber()));
    targetScket->tcpConnect();
    delete tempSocket;
}

PackerModbusTcp* ModbusProcess::getPackertModbusTcp() const
{
    return packertModbusTcp;
}

void ModbusProcess::setPackertModbusTcp(PackerModbusTcp* value)
{
    packertModbusTcp = value;
}

#ifdef USE_MODBUS_LIB
/************************************
* @Name     : checkTcpHeartBeat
* @Descript : modbusConnectNum —— modbus 链接编号
*             0：切断机modbustcp　1:包装机modbustcp
* @Param    :
* @Author   : sxy
* @Version  : v2.0.0
* @Date     : 2021-06-08
* @Return   : hertbeat check result
************************************/
bool ModbusProcess::checkTcpHeartBeat(int modbusConnectNum)
{
    int heartDetectRes = 0;
    uint16_t heartbeatTest[40];
    uint16_t heartbeatAddress = 0;
    uint16_t numberOfHeartbeatVariables = 2;
    UserModbus* modbus;

    if (modbusConnectNum == MODBUS_CONNECT_1) {
        modbus = cutModbusConnect;
    } else if (modbusConnectNum == MODBUS_CONNECT_2) {
        modbus = packerModbusConnect;
    } else {
        return SAB_R_FAIL;
    }

    //check 心跳
    heartDetectRes = modbus_read_registers(modbus->ctx, heartbeatAddress,
                                           numberOfHeartbeatVariables, heartbeatTest);

    if (heartDetectRes == -1) {
        usleep(DELAY_10MS * 100);
        LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "heart beat check error.\n");
        LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "attempting to reconnect.\n");
        //ｉf未检测到心跳
        ////重新连接
        int initResult = ModbusTcpInit(modbus);
        if (initResult == SAB_R_FAIL) {
            return SAB_R_FAIL;
        }
    }
    return SAB_R_OK;
}

/************************************
* @Name     : ModbusTcpProcess
* @Descript : modbus 子线程主循环
* @Param    : arg —— ModbusProcess
* @Author   : sxy
* @Version  : v2.0.0
* @Date     : 2021-06-08
* @Return   : void*
************************************/
void* ModbusTcpProcess(void* arg)
{
    ModbusProcess* modbus = (ModbusProcess*)arg;
    MODBUS_INFO* modbus_info = modbus->modbusCommun;
    modbus_t* ctx1 = modbus->getCutModbusConnect()->ctx;
    modbus_t* ctx2 = modbus->getPackerModbusConnect()->ctx;
    uint16_t readBuffer[64];
    uint16_t cutPlcStateTemp = 0;   // 切断机运行状态临时存放变量
    uint16_t zipperLengthTemp = 0;  // 拉链长度临时存放变量
    bool flagPlcStateChanged;//　切断机状态变化信号
    int res2 = 0;

    LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "Thread: modbustcp startup.");

    memset(readBuffer, 0, sizeof(readBuffer) / 2);

    while (1) {
        if (modbus->checkTcpHeartBeat(MODBUS_CONNECT_1)) {
            cutPlcStateTemp = modbus_info->cutPlcStateReg[0];

            res2 = modbus_read_registers(ctx1, 2, 2, modbus_info->cutPlcStateReg);         // 读取PLC状态码
            if (res2 == -1) {
                LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), modbus_strerror(errno));
            } else {
                LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "read success");
            }

            res2 = modbus_read_registers(ctx1, 8, 32, readBuffer);       // 读取工票申请码
            if (res2 == -1) {
                //LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "read faliure");
                fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
            } else {
                LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "read success");
            }
            usleep(DELAY_10MS);
            UniversalTool::HighLowSwap(modbus_info->workTickApplyCodeReg, readBuffer);
            memset(readBuffer, 0, sizeof(modbus_info->workTickApplyCodeReg));

//            res2 = modbus_read_registers(ctx1, 41, 32, readBuffer);      // 读取工票号
//            UniversalTool::HighLowSwap(modbus_info->workTickIDReg, readBuffer);
//            memset(readBuffer, 0, sizeof(modbus_info->workTickIDReg));
//            if (res2 == -1) {
//                //LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "read faliure");
//                fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
//            }
//            usleep(DELAY_10MS);

//            res2 = modbus_read_registers(ctx1, 73, 32, readBuffer);      // 读取员工工号
//            memcpy(modbus_info->employeeIDReg, readBuffer, sizeof(modbus_info->employeeIDReg));
//            memset(readBuffer, 0, sizeof(modbus_info->employeeIDReg));
////            res2 = modbus_read_registers(ctx1, 73, 32, modbus_info->cutPlcStateReg);
//            if (res2 == -1) {
//                //LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "read faliure");
//                fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
//            }
//            usleep(DELAY_10MS);

            //read length
//            res2 = modbus_read_registers(ctx1, 199, 2, modbus_info->scatteredOrderLength);

//            if (modbus_info->subScatteredOrderQuant != 0) {
//                res2 = modbus_write_registers(ctx1, 201, 1, &modbus_info->subScatteredOrderQuant);  // 散单数量
//            }
//            usleep(DELAY_10MS);
            // writer to cut HMI
//            res2 = modbus_write_registers(ctx1, 0, 3, modbus_info->packerStateReg);    // 就绪、开机、启动状态 写操作
        }

//        if (modbus->checkTcpHeartBeat(MODBUS_CONNECT_2)) {
//            res2 = modbus_read_registers(ctx2, 0, 3, readBuffer);         // 读取packer状态，
//            if (res2 == -1) {
//                LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "read faliure");
//            }
//            usleep(DELAY_10MS);
//            modbus_info->packerStart = readBuffer[0];                      // 读取启动，
//            modbus_info->packerStop = readBuffer[1];                       // 读取停止，
//            modbus_info->packerReady = readBuffer[2];                      // 读取就绪，
//            memset(readBuffer, 0, 3);

//            res2  = modbus_read_registers(ctx2, 199,  5, readBuffer);      // 读取启动，
//            if (res2 == -1) {
//                LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "read faliure");
//            }
//            usleep(DELAY_10MS);
//            modbus_info->orderFinish =  readBuffer[0];                      // 读取订单完成信号
//            modbus_info->printTrig = readBuffer[1];                         // 读取触发打印信号，
//            modbus_info->printFinish = readBuffer[2];                       // 读取打印完成信号，
//            modbus_info->thisPackWeight = readBuffer[3];                    // 读取本包重量，
//            modbus_info->thisPackQuant = readBuffer[4];                     // 读取本包数量，
//            memset(readBuffer, 0, 5);

//            res2 = modbus_write_registers(ctx2, 3, 32, modbus_info->workTickApplyCodeReg);         // 写入工票申请码
//            if (res2 == -1) {
//                LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "read faliure");
//            }
//            usleep(DELAY_10MS);
//            res2 = modbus_write_registers(ctx2, 35, 32, modbus_info->workTickIDReg);        // 写入工票号
//            if (res2 == -1) {
//                LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "read faliure");
//            }
//            usleep(DELAY_10MS);
//            res2 = modbus_write_registers(ctx2, 67, 32, modbus_info->barCodeReg);         // 写入条码号
//            if (res2 == -1) {
//                LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "read faliure");
//            }
//            usleep(DELAY_10MS);

//            uint16_t writeBuffer[2] = {0, 0};
//            writeBuffer[0] = modbus_info->orderTotal;
//            writeBuffer[1] = modbus_info->siglePackQuant;
////           pack_reg[2] = modbus_info->W_PLC_Start;
////           pack_reg[3] = modbus_info->W_PLC_Stop;
//            res2 = modbus_write_registers(ctx2, 204, 2, writeBuffer);                     // 订单总数　每包数量设定
//            if (res2 == -1) {
//                LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "read faliure");
//            }
//            usleep(DELAY_10MS);
//            res2 = modbus_write_registers(ctx2, 209, 1, &modbus_info->zipperLength);                   // 写入拉链长度
//            if (res2 == -1) {
//                LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "read faliure");
//            }
//            usleep(DELAY_10MS);
//            res2 = modbus_write_registers(ctx2, 300, 1, &modbus->scanGunTriggerSignal);           // 写入扫瞄枪触发信号
//            if (res2 == -1) {
//                LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "read faliure");
//            }
//            usleep(DELAY_10MS);
//            //            res2 = modbus_write_registers(ctx2, 000, 1, &modbus_info->subOrderQuant);                  // 写入散单数量
//        }

        if (cutPlcStateTemp != modbus_info->cutPlcStateReg[0]) {// 判断切断机状态是否发生变化
            flagPlcStateChanged = true;
        }

        usleep(DELAY_10MS * 10);
    }
}
#endif


void* ModbusTcpSocketProcess(void* arg)
{
    ModbusProcess* modbusPorcess = (ModbusProcess*)arg;
    PackerModbusTcp* modbusTcpSocket = modbusPorcess->getPackertModbusTcp();
    string workApplyCodeStr;
    uint16_t readBuffer[64];
    uint16_t writeBuffer[64];
    uint16_t cutPlcStateTemp = 0;   // 切断机运行状态临时存放变量
    uint16_t printAgainTemp = 0;
    uint8_t dataLen = 0;
    bool flagPlcStateChanged = true;//　切断机状态变化信号
    bool flagScanGunTriggered = false;
    int res2 = 0;


    LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "Thread: modbustcp startup.");

    memset(readBuffer, 0, sizeof(readBuffer));
    memset(writeBuffer, 0, sizeof(writeBuffer));

    /** debug variable **/
    string debugDisplayStr;

    F_Data currentLengthTemp;

    while (1) {
        /** cutter modbus **/
        cutPlcStateTemp = SingletonModbusInfo::GetInstance().getCutPlcStateReg()[0];

        res2 = modbusTcpSocket->ReadRegisters(*modbusTcpSocket->getCutTcpSocket(), 3, 2, readBuffer);
        UniversalTool::HighLowSwap(SingletonModbusInfo::GetInstance().getCutPlcStateReg(), readBuffer, 2);

        memset(readBuffer, 0, 128);
        res2 = modbusTcpSocket->ReadRegisters(*modbusTcpSocket->getCutTcpSocket(), 9, 32, readBuffer);
        memcpy(SingletonModbusInfo::GetInstance().getWorkTickApplyCodeReg(), readBuffer, 128);


        memset(readBuffer, 0, 128);
        res2 = modbusTcpSocket->ReadRegisters(*modbusTcpSocket->getCutTcpSocket(), 41, 32, readBuffer);
        memcpy(SingletonModbusInfo::GetInstance().getWorkTickIDReg(), readBuffer, 128);

        memset(readBuffer, 0, 128);
        res2 = modbusTcpSocket->ReadRegisters(*modbusTcpSocket->getCutTcpSocket(), 73, 32, readBuffer);
        memcpy(SingletonModbusInfo::GetInstance().getEmployeeIDReg(), readBuffer, 128);


//        debugDisplayStr.assign((char*)SingletonModbusInfo::GetInstance().getWorkTickIDReg()); //转化工票申请码
//        LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), debugDisplayStr);

        res2 = modbusTcpSocket->ReadRegisters(*modbusTcpSocket->getCutTcpSocket(), 199, 2, readBuffer);
        dataLen = strlen((char*)SingletonModbusInfo::GetInstance().getCurrentZipperLength());
        UniversalTool::HighLowSwap(SingletonModbusInfo::GetInstance().getCurrentZipperLength(),
                                   readBuffer, dataLen);
        memset(readBuffer, 0, dataLen);

        currentLengthTemp.uintForm = SingletonModbusInfo::GetInstance().getCurrentZipperLength()[0];
        currentLengthTemp.uintForm += (currentLengthTemp.uintForm << 16) + SingletonModbusInfo::GetInstance().getCurrentZipperLength()[1];

        /** write sub scattered order quant to cutter plc **/
        if (SingletonModbusInfo::GetInstance().getSubScatteredOrderQuant() != 0) {
            writeBuffer[0] = SingletonModbusInfo::GetInstance().getSubScatteredOrderQuant();
            res2 = modbusTcpSocket->WriteRegisters(*modbusTcpSocket->getCutTcpSocket(), 201, 1, writeBuffer);
            writeBuffer[0] = 0;
        }

        /** write packer state to cutter plc **/
//        modbus_info->packerStateReg[0] = 10;
//        modbus_info->packerStateReg[1] = 20;
//        modbus_info->packerStateReg[2] = 30;
        res2 = modbusTcpSocket->WriteRegisters(*modbusTcpSocket->getCutTcpSocket(), 0, 3,
                                               SingletonModbusInfo::GetInstance().getPackerStateReg());

        #ifdef ENABLE_PACKER
        /************************************* packer modbus **********************************************/
        res2 = modbusTcpSocket->ReadRegisters(*modbusTcpSocket->getPackerTcpSocket(), 0, 3, readBuffer);
        UniversalTool::HighLowSwap(SingletonModbusInfo::GetInstance().getPackerStateReg(), readBuffer, 6);
        memset(readBuffer, 0, 3);

        /**  get packer state from packer screen **/
        res2 = modbusTcpSocket->ReadRegisters(*modbusTcpSocket->getPackerTcpSocket(), 199, 5, readBuffer);
        dataLen = strlen((char*)SingletonModbusInfo::GetInstance().getPackerStateReg());
        UniversalTool::HighLowSwap(SingletonModbusInfo::GetInstance().getPackerStateReg(), readBuffer, dataLen);
        memset(readBuffer, 0, dataLen);
        SingletonModbusInfo::GetInstance().setOrderFinish(SingletonModbusInfo::GetInstance().getPackerStateReg()[0]);
        SingletonModbusInfo::GetInstance().setPrintTrig(SingletonModbusInfo::GetInstance().getPackerStateReg()[1]);
        SingletonModbusInfo::GetInstance().setPrintFinish(SingletonModbusInfo::GetInstance().getPackerStateReg()[2]);
        SingletonModbusInfo::GetInstance().setThisPackWeight(SingletonModbusInfo::GetInstance().getPackerStateReg()[3]);
        SingletonModbusInfo::GetInstance().setThisPackQuant(SingletonModbusInfo::GetInstance().getPackerStateReg()[4]);

        /** get print again signal from packer screen **/
        res2 = modbusTcpSocket->ReadRegisters(*modbusTcpSocket->getPackerTcpSocket(), 299, 1, readBuffer);
        printAgainTemp = ((readBuffer[0] & 0xFF00) >> 8) + (readBuffer[0] << 8);
        SingletonModbusInfo::GetInstance().setPrintAgain(printAgainTemp);
        readBuffer[0] = 0;

        /** write workticket apply code to packer screen **/
        memset(writeBuffer, 0, 64);
        dataLen = strlen((char*)SingletonModbusInfo::GetInstance().getWorkTickApplyCodeReg());
        UniversalTool::HighLowSwap(writeBuffer, SingletonModbusInfo::GetInstance().getWorkTickApplyCodeReg(), dataLen);
        res2 = modbusTcpSocket->WriteRegisters(*modbusTcpSocket->getPackerTcpSocket(), 3, 32, writeBuffer);

        /** write wokrticket id to packer screen  **/
        memset(writeBuffer, 0, 64);
        dataLen = strlen((char*)SingletonModbusInfo::GetInstance().getWorkTickIDReg());
        UniversalTool::HighLowSwap(writeBuffer, SingletonModbusInfo::GetInstance().getWorkTickIDReg(), dataLen);
        res2 = modbusTcpSocket->WriteRegisters(*modbusTcpSocket->getPackerTcpSocket(), 35, 32, writeBuffer);

        /** write bar code to packer screen **/
        memset(writeBuffer, 0, 64);
        dataLen = strlen((char*)SingletonModbusInfo::GetInstance().getBarCodeReg());
        UniversalTool::HighLowSwap(writeBuffer, SingletonModbusInfo::GetInstance().getBarCodeReg(), dataLen);
        res2 = modbusTcpSocket->WriteRegisters(*modbusTcpSocket->getPackerTcpSocket(), 67, 32, writeBuffer);

        /** write order data to packer screen **/
        /** for test modbusInfo->orderTotal = 500; modbusInfo->siglePackQuant = 1500; **/
        memset(writeBuffer, 0, 64);
        writeBuffer[0] = SingletonModbusInfo::GetInstance().getOrderTotal();
        writeBuffer[1] = SingletonModbusInfo::GetInstance().getSiglePackQuant();
        res2 = modbusTcpSocket->WriteRegisters(*modbusTcpSocket->getPackerTcpSocket(), 204, 2, writeBuffer);// 订单总数　每包数量设定
        if (flagPlcStateChanged) {
            LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "Cutter State have changed!");
            res2  = modbusTcpSocket->WriteRegisters(*modbusTcpSocket->getPackerTcpSocket(), 206, 2,
                                                    SingletonModbusInfo::GetInstance().getCutPlcStateReg());// 写入PLC状态值
        }


        /** write working zipper length to packer screen **/
        /* for test
        modbus_info->zipperLength = (uint16_t)(currentLengthTemp.floatForm * 10);
        */
        memset(writeBuffer, 0, 64);
        writeBuffer[0] = SingletonModbusInfo::GetInstance().getZipperLength();
        res2 = modbusTcpSocket->WriteRegisters(*modbusTcpSocket->getPackerTcpSocket(), 209, 1, writeBuffer);
        writeBuffer[0] = 0;

        /** write scan gun trigger signal to packer screen **/
        if (modbusPorcess->scanGunTriggerSignal == 1 && flagScanGunTriggered == false) {
            flagScanGunTriggered = true;
            LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "ScanGun has been trriged!");
        } else {
            if (modbusPorcess->scanGunTriggerSignal == 0 && flagScanGunTriggered == true) {
                flagScanGunTriggered = false;
                LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "ScanGun signal restore!");
            }
        }
        res2 = modbusTcpSocket->WriteRegisters(*modbusTcpSocket->getPackerTcpSocket(), 300, 1, &modbusPorcess->scanGunTriggerSignal);

        if (cutPlcStateTemp != SingletonModbusInfo::GetInstance().getCutPlcStateReg()[0]) {// 判断切断机状态是否发生变化
            flagPlcStateChanged = true;
        } else {
            flagPlcStateChanged = false;
        }
        #else
        if (modbusPorcess->scanGunTriggerSignal == 1 && flagScanGunTriggered == false) {
            flagScanGunTriggered = true;
            LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "ScanGunSignal Value :" + to_string(modbusPorcess->scanGunTriggerSignal));
            LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "ScanGun has been trriged!");
        } else {
            if (modbusPorcess->scanGunTriggerSignal == 0 && flagScanGunTriggered == true) {
                flagScanGunTriggered = false;
                LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "ScanGunSignal Value :" + to_string(modbusPorcess->scanGunTriggerSignal));
                LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "ScanGun signal restore!");
            }
        }
        #endif
    }
}
