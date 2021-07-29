#include "PackerModbusTCP.h"
#include "PackerTcpSocket.h"
#include "SingletonUserLogger.h"
#include "tool/UniversalTool.h"

static char modbus_tcp_read_request_head[] = {0x00, 0x01, 0x00, 0x00, 0x00, 0x06};
static char modbus_tcp_write_request_head[] = {0x00, 0x01, 0x00, 0x00};



PackerModbusTcp::PackerModbusTcp()
{
    cutTcpSocket = new PackerTcpSocket();
    packerTcpSocket = new PackerTcpSocket();
}

PackerModbusTcp::~PackerModbusTcp()
{
    delete cutTcpSocket;
    delete packerTcpSocket;
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
int PackerModbusTcp::ReadRegisters(PackerTcpSocket& socketClient, uint16_t startAddress,
                                   uint16_t number, uint16_t* dest)
{
    int iResult = -1;
    int index = 0;
    char requestBuffer[12];
    uint16_t sendCheckData = 0;
    uint16_t recvCheckData = 0;

    /** build modbus read quest data **/
    sendCheckData = socketClient.getSlaveNumber();
    sendCheckData = (sendCheckData << 8) + MODBUS_FUNCTION_CODE_READ_REGISTERS;
    index = sizeof(modbus_tcp_read_request_head);
    memcpy(requestBuffer, modbus_tcp_read_request_head, index);
    requestBuffer[index++] = socketClient.getSlaveNumber();
    requestBuffer[index++] = MODBUS_FUNCTION_CODE_READ_REGISTERS;
    requestBuffer[index++] = startAddress >> 8;
    requestBuffer[index++] = (char)startAddress;
    requestBuffer[index++] = number >> 8;
    requestBuffer[index++] = number;
    /** send read request **/
    iResult = socketClient.sendMessage(requestBuffer, sizeof(requestBuffer));
    if (iResult < 0) {
        return -1;
    }

    int forecastRecvLength = 9 + number * 2;
    int actualRecv = 0;
    char recvBuffer[forecastRecvLength];
    actualRecv = socketClient.recvMessage(recvBuffer, forecastRecvLength);
    if (actualRecv < 0) {
        return -1;
    }

    /** modbus data parse **/
    if (actualRecv < 8) {
        LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), "data length discrepancy.");
        return SAB_R_FAIL;
    }
    recvCheckData = ((uint16_t)recvBuffer[6] << 8) + recvBuffer[7];
    if (sendCheckData != recvCheckData) {
        LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), "invalid data.");
        return SAB_R_FAIL;
    }
    iResult = UniversalTool::modbusDataParse(dest, recvBuffer, actualRecv, SingletonUserLogger::GetInstance().getLogger());

    return iResult;
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
int PackerModbusTcp::WriteRegisters(PackerTcpSocket& socketClient, uint16_t startAddress,
                                    uint16_t number, uint16_t* dest)
{
    int iResult = -1;
    int index = 0;
    uint16_t requestLength = 0;
    char requestBuffer[13 + number * 2];
    uint16_t sendCheckData = 0;
    uint16_t recvCheckData = 0;

    sendCheckData = socketClient.getSlaveNumber();
    sendCheckData = (sendCheckData << 8) + MODBUS_FUNCTION_CODE_WRITE_REGISTERS;
    requestLength = 7 + number * 2;
    index = sizeof(modbus_tcp_write_request_head);
    memcpy(requestBuffer, modbus_tcp_write_request_head, index);
    requestBuffer[index++] = requestLength >> 8;
    requestBuffer[index++] = (char)requestLength;
    requestBuffer[index++] = socketClient.getSlaveNumber();
    requestBuffer[index++] = MODBUS_FUNCTION_CODE_WRITE_REGISTERS;
    requestBuffer[index++] = startAddress >> 8;
    requestBuffer[index++] = (char)startAddress;
    requestBuffer[index++] = number >> 8;
    requestBuffer[index++] = number;
    requestBuffer[index++] = 2 * number;
    for (int i = 0; i < number; i++) {
        requestBuffer[index++] = dest[i] >> 8;
        requestBuffer[index++] = (char)dest[i];
    }
    /** send write request **/
    iResult = socketClient.sendMessage(requestBuffer, sizeof(requestBuffer));
    if (iResult < 0) {
        return -1;
    }

    int actualRecv = 0;
    char recvBuffer[12];
    actualRecv = socketClient.recvMessage(recvBuffer, 12);
    if (actualRecv < 0) {
        return -1;
    }
    /** data check **/
    if (actualRecv < 8) {
        LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), "data length discrepancy.");
        return SAB_R_FAIL;
    }
    recvCheckData = ((uint16_t)recvBuffer[6] << 8) + recvBuffer[7];
    if (sendCheckData != recvCheckData) {
        LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), "check failure, write data failure.");
        return -1;
    }
    return iResult;
}
/** getter and setter **/

PackerTcpSocket* PackerModbusTcp::getCutTcpSocket() const
{
    return cutTcpSocket;
}

void PackerModbusTcp::setCutTcpSocket(PackerTcpSocket* value)
{
    cutTcpSocket = value;
}

PackerTcpSocket* PackerModbusTcp::getPackerTcpSocket() const
{
    return packerTcpSocket;
}

void PackerModbusTcp::setPackerTcpSocket(PackerTcpSocket* value)
{
    packerTcpSocket = value;
}


