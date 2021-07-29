#ifndef MODBUSPROCESS_H
#define MODBUSPROCESS_H

#include "class/ModbusInfo.h"
#include "lib/modbus.h"

#define MODBUS1_HOST    "Modbus1_Host"
#define MODBUS1_PORT    "Modbus1_Port"
#define MODBUS2_HOST    "Modbus2_Host"
#define MODBUS2_PORT    "Modbus2_Port"

#define DELAY_10MS      10000           // 时延10 ms

#define MODBUS_CONNECT_1 0
#define MODBUS_CONNECT_2 1

class UserLogger;
class UserTimer;
class PackerModbusTcp;
class PackerTcpSocket;

class ModbusProcess
{
public:
    ModbusProcess();
    ~ModbusProcess();
    static uint16_t scanGunTriggerSignal; // 扫描枪触发信号

    static void ScanSignalClean()
    {
        scanGunTriggerSignal = 0;
    }

    class UserModbus
    {
    public:
        UserModbus();
        UserModbus(string address, int port, int slaveNum);
        string modbusName;
        modbus_t* ctx;
        int port;
        int slaveAddress;
        string address;
    };

    void ModbusInit();
    bool checkTcpHeartBeat(int modbusConnectNum);

    #ifdef USE_MODBUS_LIB
    UserModbus* getPackerModbusConnect() const;
    void setPackerModbusConnect(UserModbus* value);

    UserModbus* getCutModbusConnect() const;
    void setCutModbusConnect(UserModbus* value);
    #endif
    UserLogger* getModbusLogger() const;
    void setModbusLogger(UserLogger* value);
    PackerModbusTcp* getPackertModbusTcp() const;
    void setPackertModbusTcp(PackerModbusTcp* value);


private:
    int  ModbusTcpInit(UserModbus* modbus);
    void ModbusTcpInit(const char*, const char*, PackerTcpSocket*, int);

private:
    #ifdef USE_MODBUS_LIB
    UserModbus* cutModbusConnect;
    UserModbus* packerModbusConnect;
    #endif
    UserTimer* modbusTimer;

    PackerModbusTcp* packertModbusTcp;
};

void* ModbusTcpProcess(void* arg);
void* ModbusTcpSocketProcess(void* arg);
#endif // MODBUSPROCESS_H
