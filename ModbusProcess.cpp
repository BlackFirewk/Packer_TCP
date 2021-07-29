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
    packertModbusTcp = new PackerModbusTcp();
}

ModbusProcess::~ModbusProcess()
{
    delete packertModbusTcp;
}

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

    ModbusTcpInit(MODBUS1_HOST, MODBUS1_PORT, packertModbusTcp->getCutTcpSocket(), 2);
    #ifdef ENABLE_PACKER
    ModbusTcpInit(MODBUS2_HOST, MODBUS2_PORT, packertModbusTcp->getPackerTcpSocket(), 1);
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

/************************************
* @Name     : CutterDataProcessor
* @Descript : 切断机数据处理器
* @Param    : modbustcp_socket ——　切断机tcp scoket
*             cutter_state —— 切断机状态
* @Author   : sxy
* @Version  : v2.0.0
* @Date     : 2021-07-29
* @Return   : 数据处理是否出错
************************************/
int CutterDataProcessor(PackerModbusTcp* modbustcp_socket, uint16_t& cutter_state)
{
    int result = 0;
    uint16_t readBuffer[64];
    uint16_t writeBuffer[64];
    F_Data currentLengthTemp;
    memset(readBuffer, 0, sizeof(readBuffer));
    memset(writeBuffer, 0, sizeof(writeBuffer));

    /** cutter modbus **/
    cutter_state = SingletonModbusInfo::GetInstance().getCutPlcStateReg()[0];

    result = modbustcp_socket->ReadRegisters(*modbustcp_socket->getCutTcpSocket(), 3, CUTTER_STATE_REG_SIZE, readBuffer);
    if (result < 0) {
        return SAB_R_ERROR;
    }
    UniversalTool::HighLowSwap(SingletonModbusInfo::GetInstance().getCutPlcStateReg(), readBuffer, CUTTER_STATE_REG_SIZE);

    memset(readBuffer, 0, 128);
    result = modbustcp_socket->ReadRegisters(*modbustcp_socket->getCutTcpSocket(), 9, WORK_TICKET_APPLY_CODE_REG_SIZE, readBuffer);
    if (result < 0) {
        return SAB_R_ERROR;
    }
    memcpy(SingletonModbusInfo::GetInstance().getWorkTickApplyCodeReg(), readBuffer, WORK_TICKET_APPLY_CODE_REG_SIZE * sizeof(uint16_t));

    memset(readBuffer, 0, 128);
    result = modbustcp_socket->ReadRegisters(*modbustcp_socket->getCutTcpSocket(), 41, WORK_TICKET_ID_REG_SIZE, readBuffer);
    if (result < 0) {
        return SAB_R_ERROR;
    }
    memcpy(SingletonModbusInfo::GetInstance().getWorkTickIDReg(), readBuffer, WORK_TICKET_ID_REG_SIZE * sizeof(uint16_t));

    memset(readBuffer, 0, 128);
    result = modbustcp_socket->ReadRegisters(*modbustcp_socket->getCutTcpSocket(), 73, 32, readBuffer);
    if (result < 0) {
        return SAB_R_ERROR;
    }
    memcpy(SingletonModbusInfo::GetInstance().getEmployeeIDReg(), readBuffer, 128);

    memset(readBuffer, 0, ZIPPER_LENGTH_REG_SIZE);
    result = modbustcp_socket->ReadRegisters(*modbustcp_socket->getCutTcpSocket(), 199, ZIPPER_LENGTH_REG_SIZE, readBuffer);
    if (result < 0) {
        return SAB_R_ERROR;
    }
    UniversalTool::HighLowSwap(SingletonModbusInfo::GetInstance().getCurrentZipperLength(), readBuffer, ZIPPER_LENGTH_REG_SIZE);
    currentLengthTemp.uintForm = SingletonModbusInfo::GetInstance().getCurrentZipperLength()[0];
    currentLengthTemp.uintForm += (currentLengthTemp.uintForm << 16) + SingletonModbusInfo::GetInstance().getCurrentZipperLength()[1];

    /** write sub scattered order quant to cutter plc **/
    if (SingletonModbusInfo::GetInstance().getSubScatteredOrderQuant() != 0) {
        writeBuffer[0] = SingletonModbusInfo::GetInstance().getSubScatteredOrderQuant();
        result = modbustcp_socket->WriteRegisters(*modbustcp_socket->getCutTcpSocket(), 201, 1, writeBuffer);
        if (result < 0) {
            return SAB_R_ERROR;
        }
        writeBuffer[0] = 0;
    }
    /** write packer state to cutter plc **/
    result = modbustcp_socket->WriteRegisters(*modbustcp_socket->getCutTcpSocket(), 0, 3,
             SingletonModbusInfo::GetInstance().getPackerStateReg());
    if (result < 0) {
        return SAB_R_ERROR;
    }
    return SAB_R_OK;
}

/************************************
* @Name     :　PackerDataProcessor
* @Descript :　包装机数据处理器
* @Param    : modbustcp_socket ——　包装机tcp scoket
*             cutter_state —— 切断机状态
*             scangun_trigger　——　扫码枪触发信号
* @Author   : sxy
* @Version  : v2.0.0
* @Date     : 2021-07-29
* @Return   :
************************************/
int PackerDataProcessor(PackerModbusTcp* modbustcp_socket, uint16_t& cutter_state, uint16_t& scangun_trigger)
{
    static bool flagPlcStateChanged = true;//　切断机状态变化信号
    static bool flagScanGunTriggered = false;
    int result = 0;
    uint8_t dataLen = 0;
    uint16_t printAgainTemp = 0;
    uint16_t readBuffer[64];
    uint16_t writeBuffer[64];

    memset(readBuffer, 0, sizeof(readBuffer));
    memset(writeBuffer, 0, sizeof(writeBuffer));

    /************************************* packer modbus **********************************************/
    result = modbustcp_socket->ReadRegisters(*modbustcp_socket->getPackerTcpSocket(), 0, 3, readBuffer);
    if (result < 0) {
        return SAB_R_ERROR;
    }
    UniversalTool::HighLowSwap(SingletonModbusInfo::GetInstance().getPackerStateReg(), readBuffer, 6);
    memset(readBuffer, 0, 3);

    /**  get packer state from packer screen **/
    result = modbustcp_socket->ReadRegisters(*modbustcp_socket->getPackerTcpSocket(), 199, 5, readBuffer);
    if (result < 0) {
        return SAB_R_ERROR;
    }
    dataLen = strlen((char*)readBuffer);
    UniversalTool::HighLowSwap(SingletonModbusInfo::GetInstance().getPackerStateReg(), readBuffer, dataLen / 2);
    memset(readBuffer, 0, dataLen);
    SingletonModbusInfo::GetInstance().setOrderFinish(SingletonModbusInfo::GetInstance().getPackerStateReg()[0]);
    SingletonModbusInfo::GetInstance().setPrintTrig(SingletonModbusInfo::GetInstance().getPackerStateReg()[1]);
    SingletonModbusInfo::GetInstance().setPrintFinish(SingletonModbusInfo::GetInstance().getPackerStateReg()[2]);
    SingletonModbusInfo::GetInstance().setThisPackWeight(SingletonModbusInfo::GetInstance().getPackerStateReg()[3]);
    SingletonModbusInfo::GetInstance().setThisPackQuant(SingletonModbusInfo::GetInstance().getPackerStateReg()[4]);

    /** get print again signal from packer screen **/
    result = modbustcp_socket->ReadRegisters(*modbustcp_socket->getPackerTcpSocket(), 299, 1, readBuffer);
    if (result < 0) {
        return SAB_R_ERROR;
    }
    printAgainTemp = ((readBuffer[0] & 0xFF00) >> 8) + (readBuffer[0] << 8);
    SingletonModbusInfo::GetInstance().setPrintAgain(printAgainTemp);
    readBuffer[0] = 0;

    /** write workticket apply code to packer screen **/
    memset(writeBuffer, 0, 64);
    dataLen = strlen((char*)SingletonModbusInfo::GetInstance().getWorkTickApplyCodeReg());
    UniversalTool::HighLowSwap(writeBuffer, SingletonModbusInfo::GetInstance().getWorkTickApplyCodeReg(), dataLen);
    result = modbustcp_socket->WriteRegisters(*modbustcp_socket->getPackerTcpSocket(), 3, 32, writeBuffer);
    if (result < 0) {
        return SAB_R_ERROR;
    }

    /** write wokrticket id to packer screen  **/
    memset(writeBuffer, 0, 64);
    dataLen = strlen((char*)SingletonModbusInfo::GetInstance().getWorkTickIDReg());
    UniversalTool::HighLowSwap(writeBuffer, SingletonModbusInfo::GetInstance().getWorkTickIDReg(), dataLen);
    result = modbustcp_socket->WriteRegisters(*modbustcp_socket->getPackerTcpSocket(), 35, 32, writeBuffer);
    if (result < 0) {
        return SAB_R_ERROR;
    }

    /** write bar code to packer screen **/
    memset(writeBuffer, 0, 64);
    dataLen = strlen((char*)SingletonModbusInfo::GetInstance().getBarCodeReg());
    UniversalTool::HighLowSwap(writeBuffer, SingletonModbusInfo::GetInstance().getBarCodeReg(), dataLen);
    result = modbustcp_socket->WriteRegisters(*modbustcp_socket->getPackerTcpSocket(), 67, 32, writeBuffer);
    if (result < 0) {
        return SAB_R_ERROR;
    }

    /** write order data to packer screen **/
    /** for test modbusInfo->orderTotal = 500; modbusInfo->siglePackQuant = 1500; **/
    memset(writeBuffer, 0, 64);
    writeBuffer[0] = SingletonModbusInfo::GetInstance().getOrderTotal();
    writeBuffer[1] = SingletonModbusInfo::GetInstance().getSiglePackQuant();
    result = modbustcp_socket->WriteRegisters(*modbustcp_socket->getPackerTcpSocket(), 204, 2, writeBuffer);// 订单总数　每包数量设定
    if (result < 0) {
        return SAB_R_ERROR;
    }
    if (flagPlcStateChanged) {
        LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "Cutter State have changed!");
        result  = modbustcp_socket->WriteRegisters(*modbustcp_socket->getPackerTcpSocket(), 206, 2,
                  SingletonModbusInfo::GetInstance().getCutPlcStateReg());// 写入PLC状态值
    }

    /** write working zipper length to packer screen **/
    /* for test
    modbus_info->zipperLength = (uint16_t)(currentLengthTemp.floatForm * 10);
    */
    memset(writeBuffer, 0, 64);
    writeBuffer[0] = SingletonModbusInfo::GetInstance().getZipperLength();
    result = modbustcp_socket->WriteRegisters(*modbustcp_socket->getPackerTcpSocket(), 209, 1, writeBuffer);
    if (result < 0) {
        return SAB_R_ERROR;
    }
    writeBuffer[0] = 0;

    /** write scan gun trigger signal to packer screen **/
    if (scangun_trigger == 1 && flagScanGunTriggered == false) {
        flagScanGunTriggered = true;
        LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "scanGun has been trriged!");
    } else {
        if (scangun_trigger == 0 && flagScanGunTriggered == true) {
            flagScanGunTriggered = false;
            LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "scanGun signal restore!");
        }
    }
    result = modbustcp_socket->WriteRegisters(*modbustcp_socket->getPackerTcpSocket(), 300, 1, &scangun_trigger);
    if (result < 0) {
        return SAB_R_ERROR;
    }

    if (cutter_state != SingletonModbusInfo::GetInstance().getCutPlcStateReg()[0]) {// 判断切断机状态是否发生变化
        flagPlcStateChanged = true;
    } else {
        flagPlcStateChanged = false;
    }

    return SAB_R_OK;
}

/************************************
* @Name     :
* @Descript :
* @Param    :
* @Author   : sxy
* @Version  : v2.0.0
* @Date     : 2021-07-29
* @Return   :
************************************/
void* ModbusTcpSocketProcess(void* arg)
{
    ModbusProcess* modbusPorcess = (ModbusProcess*)arg;
    PackerModbusTcp* modbusTcpSocket = modbusPorcess->getPackertModbusTcp();
    uint16_t cutPlcStateTemp = 0;   // 切断机运行状态临时存放变量
    bool tcpConnectFlag = true;
    int res2 = 0;

    LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "thread: modbustcp startup.");

    while (1) {
        if (tcpConnectFlag == true) {
            res2 = CutterDataProcessor(modbusTcpSocket, cutPlcStateTemp);
            if (res2 < 0) {
                tcpConnectFlag = false;
                continue;
            }
            res2 = PackerDataProcessor(modbusTcpSocket, cutPlcStateTemp, modbusPorcess->scanGunTriggerSignal);
            if (res2 < 0) {
                continue;
            }
        } else {
            modbusTcpSocket->getCutTcpSocket()->Disconnect();
            modbusTcpSocket->getPackerTcpSocket()->Disconnect();
            usleep(1000 * 1000 * 10);

            modbusPorcess->ModbusInit();
            tcpConnectFlag = true;
            cutPlcStateTemp = 0;
        }
    }
}
