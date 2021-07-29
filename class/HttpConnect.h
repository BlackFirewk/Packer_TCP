#ifndef HTTPCONNECT_H
#define HTTPCONNECT_H

#include <string>
using namespace std;

class HttpConnect
{
public:
    HttpConnect();
    HttpConnect(string& address, int port);

    string getIpAddress() const;
    void setIpAddress(const string& value);

    int getHostPort() const;
    void setHostPort(int value);

private:
    string ipAddress;
    int hostPort;
};

#endif // HTTPCONNECT_H
