#include "PackerTcpSocket.h"
#include "SingletonUserLogger.h"
#include "PackerBase.h"
#include <unistd.h>


PackerTcpSocket::PackerTcpSocket()
{
    clientSocket = -1;
    b_isResv = true;
    b_isConnected = false;
    b_disconnectedAlert = false;

    slaveNumber = 0xff;
    socketPort = -1;
    socketHost = "\0";
    socketName = "Not_initialize_Socket";
}


PackerTcpSocket::~PackerTcpSocket()
{
    Disconnect();
}

void PackerTcpSocket::init(PackerTcpSocket* dest)
{
    clientSocket = dest->clientSocket;
    b_isResv = dest->b_isResv;
    b_isConnected = dest->b_isConnected;

    slaveNumber = dest->getSlaveNumber();
    socketPort = dest->getSocketPort();
    socketHost = dest->getSocketHost();
    socketName = dest->getSocketName();
}
int PackerTcpSocket::tcpConnect()
{
    int iLen = 0;
    int iRet = 0;

    if (socketHost.empty() || (socketPort == -1) || (slaveNumber == 0xff)) {
        LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(),
                      "socket host: " + socketHost + ";"\
                      + "socket port: " + to_string(socketPort) + ";" \
                      + "socket slave number: " + to_string(slaveNumber) + ".");
        LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), socketName + " socket param error.");
        return SAB_R_FAIL;
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), socketName + " scokect create error.");
    }
    m_connectAddress.sin_family = AF_INET;
    m_connectAddress.sin_port = htons(socketPort);
    m_connectAddress.sin_addr.s_addr = inet_addr(socketHost.c_str());
    iLen = sizeof(m_connectAddress);
    iRet = connect(clientSocket, (struct sockaddr*)&m_connectAddress, iLen);
    if (iRet < 0) {
        LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), socketName + " scokect create error.");
    } else {
        b_isConnected = true;
    }
    return iRet;
}

void PackerTcpSocket::Disconnect()
{
    close(clientSocket);
    b_isResv = false;
    b_isConnected = false;
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
int PackerTcpSocket::sendMessage(const char* src, int length)
{
    int iResult = -1;
    if (b_isConnected == true) {
        b_disconnectedAlert = false;
        iResult = send(clientSocket, src, length, 0);
        if (iResult < 0) {
            b_isConnected = false;
            LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), socketName + " send message faliure.");
        }
    } else {
        if (b_disconnectedAlert == false) {
            b_disconnectedAlert = true;
            LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), socketName + " tcp not connected.");
        }
    }
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
int PackerTcpSocket::recvMessage(char* dest, int bufferLen)
{
    int iResult = -1;

    if (b_isConnected == true) {
        b_disconnectedAlert = false;
        iResult = recv(clientSocket, dest, bufferLen, 0);
        if (iResult < 0) {
            b_isConnected = false;
            LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), socketName + " reveice message faliure.");
        }
    } else {
        if (b_disconnectedAlert == false) {
            b_disconnectedAlert = true;
            LOG4CXX_ERROR(SingletonUserLogger::GetInstance().getLogger(), socketName + " tcp not connected.");
        }
    }
    return iResult;
}



/** getter and setter **/
uint8_t PackerTcpSocket::getSlaveNumber() const
{
    return slaveNumber;
}

void PackerTcpSocket::setSlaveNumber(uint8_t value)
{
    slaveNumber = value;
}

string PackerTcpSocket::getSocketHost() const
{
    return socketHost;
}

void PackerTcpSocket::setSocketHost(const string& value)
{
    socketHost = value;
}

int PackerTcpSocket::getSocketPort() const
{
    return socketPort;
}

void PackerTcpSocket::setSocketPort(int value)
{
    socketPort = value;
}

string PackerTcpSocket::getSocketName() const
{
    return socketName;
}

void PackerTcpSocket::setSocketName(const string& value)
{
    socketName = value;
}
