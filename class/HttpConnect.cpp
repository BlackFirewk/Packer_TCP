#include "HttpConnect.h"

HttpConnect::HttpConnect()
{

}

HttpConnect::HttpConnect(string& address, int port)
{
    this->ipAddress = address;
    this->hostPort = port;
}

string HttpConnect::getIpAddress() const
{
    return ipAddress;
}

void HttpConnect::setIpAddress(const string& value)
{
    ipAddress = value;
}

int HttpConnect::getHostPort() const
{
    return hostPort;
}

void HttpConnect::setHostPort(int value)
{
    hostPort = value;
}
