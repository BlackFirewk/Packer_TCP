#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "PackerBase.h"
#include <sys/socket.h>
#include <netinet/in.h>

class PackerTcpSocket
{
public:
    PackerTcpSocket();
    ~PackerTcpSocket();
    void init(PackerTcpSocket*);
    int tcpConnect();
    void Disconnect();
    int sendMessage(const char*, int);
    int recvMessage(char*, int);

    /** getter and setter **/
    uint8_t getSlaveNumber() const;
    void setSlaveNumber(uint8_t value);
    int getSocketPort() const;
    void setSocketPort(int value);
    string getSocketName() const;
    void setSocketName(const string& value);
    string getSocketHost() const;
    void setSocketHost(const string& value);

private:
    struct sockaddr_in m_connectAddress;
    int clientSocket;
    bool b_isResv;
    bool b_disconnectedAlert;
    bool b_isConnected;
    uint8_t slaveNumber;
    int socketPort;
    string socketHost;
    string socketName;
};

#endif // TCPSOCKET_H
