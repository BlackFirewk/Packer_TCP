#ifndef PACKERMAIN_H
#define PACKERMAIN_H

#include "ModbusProcess.h"
#include "HttpConnect.h"
#include "PrinterProcess.h"
#include <sys/time.h>
#include <unistd.h>
#include <fstream>
#include "ErpInterface.h"
#include "SingletonUserLogger.h"
#include "SingletonPropertiesTable.h"
#include "SingletonPackerData.h"
#include "SingletonPrinter.h"
#include "SingletonModbusInfo.h"
#include "PackRuleData.h"
#include "test/LoggerTest.h"
#include "PackerBase.h"
#include "LabelInfoData.h"
#include "GetConfig.h"
#include "UserTimer.h"
#include "UserTimerManager.h"
#include "SingletonTimerScheduler.h"
//#define UNIT_TEST


void SubThreadInit(ModbusProcess* modbus);
int ReadConfig(void);
int getPackerRule(ModbusProcess& modbusProcesser, HttpConnect& httpPackConnect);
int WorkModeProcess(HttpConnect& http_pack_connect, uint8_t work_mode, MODBUS_INFO& modbus_info);
int PrintTriggerProcess(PrinterProcess* printProces, int operation);
int getScatteredOrderQuant(ModbusProcess& modbusProcesser, HttpConnect& httpPackConnect, F_Data& lengthData);
#endif // PACKERMAIN_H
