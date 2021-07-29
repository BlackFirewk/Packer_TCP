#ifndef UNIVERSALTOOL_H
#define UNIVERSALTOOL_H
#include <string>
#include <algorithm>
#include <stdint.h>
#include "PackerBase.h"
#include "log4cxx/logger.h"
#include "log4cxx/propertyconfigurator.h"

using namespace std;
using namespace log4cxx;


class UniversalTool
{
public:
    static int HighLowSwap(uint16_t* out_reg, uint16_t* reg, int size)
    {
        int i;
        for (i = 0; i < size / 2; i++) {
            out_reg[i] = (reg[i] & 0x00FF) << 8;    // 将低位移到高位
            out_reg[i] += (reg[i] >> 8);            // 高位移到低位，相加
        }
        return SAB_R_OK;
    }

    static vector<string> AnalysisResultString(const string& src)
    {
        size_t pos = 0;
        string msg = src;
        vector<string> result;

        if (msg.empty()) {
            return result;
        }

        msg += "$";                   //将分隔符加入到最后一个位置，方便分割最后一位
        pos = msg.find("$");

        while (pos != string::npos) {
            result.push_back(msg.substr(0, pos));
            msg = msg.substr(pos + 1);
            pos = msg.find("$");
        }

        return result;
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
    static int modbusDataParse(uint16_t* dest, char* src, int length, const LoggerPtr& logger)
    {
        uint16_t dataLen = 0;

        dataLen = ((uint16_t)src[4] << 8) + src[5];
        if (dataLen != (length - 6)) {
            LOG4CXX_ERROR(logger, "invalid data.");
            return -1;
        }

        dataLen = src[8];
        if (dataLen != (length - 9)) {
            LOG4CXX_ERROR(logger, "invalid data.");
            return -1;
        }
        memcpy(dest, &src[9], dataLen);
        return dataLen;
    }
};

#endif // UNIVERSALTOOL_H
