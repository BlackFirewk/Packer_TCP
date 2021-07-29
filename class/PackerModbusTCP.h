#ifndef PACKERMODBUSTCP_H
#define PACKERMODBUSTCP_H

#include "PackerBase.h"

#define MODBUS_FUNCTION_CODE_READ_REGISTERS     0x03
#define MODBUS_FUNCTION_CODE_WRITE_REGISTERS    0x10

class PackerTcpSocket;

class PackerModbusTcp
{
public:
    PackerModbusTcp();
    ~PackerModbusTcp();
    PackerModbusTcp(int, int,  string, string);
    int ReadRegisters(PackerTcpSocket&, uint16_t, uint16_t, uint16_t*);
    int WriteRegisters(PackerTcpSocket&, uint16_t, uint16_t, uint16_t*);

    /** getter and setter **/
    PackerTcpSocket* getCutTcpSocket() const;
    void setCutTcpSocket(PackerTcpSocket* value);
    PackerTcpSocket* getPackerTcpSocket() const;
    void setPackerTcpSocket(PackerTcpSocket* value);

private:
    PackerTcpSocket* cutTcpSocket;
    PackerTcpSocket* packerTcpSocket;
};


#endif // PACKERMODBUSTCP_H
