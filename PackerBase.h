#ifndef PACKERBACE_H
#define PACKERBACE_H

#include <stdio.h>
#include <stdint.h>
#include <string>
#include "lib/httplib.h"
using namespace std;

#ifndef _HRESULT_DEFINED
    #define _HRESULT_DEFINED
    typedef unsigned int    HRESULT;
#endif /* _HRESULT_DEFINED */


#ifndef SAB_R_OK
    #define SAB_R_OK             1        // 定义成功返回码
#endif
#ifndef SAB_R_FAIL
    #define SAB_R_FAIL           0        // 定义失败返回码
#endif
#ifndef SAB_R_ERROR
    #define SAB_R_ERROR          -1       // 定义错误返回码
#endif
//#define DEBUG

typedef union {
    float floatForm;
    unsigned int uintForm;
} F_Data;

#define CHECK_ERROR(state,  error_code)     \
    if (state)                              \
    {                                       \
        return      error_code;             \
    }

// 参数输入类型枚举
typedef enum _PACKER_SET_PARAM_ {
    SAB_ERP_IP_ADDR                     =   1,      // ERP的IP地址
    SAB_ERP_HOST_PORT                   =   2,      // ERP的主机端口号
    SAB_ERP_PACKRULENAME                =   3,      // ERP获取包装规则接口名称
    SAB_ERP_PACKRULEPRO                 =   4,      // ERP获取包装规则接口属性，lsbh
    SAB_ERP_LABELPRINTNAME              =   5,      // ERP标签打印接口名称
    SAB_ERP_LABELPRINTPRO               =   6,      // ERP标签打印接口属性,info
    SAB_PACKER_LOG_PATH                 =   7,      // 包装机项目的log路径
    SAB_PACKER_INIT                     =   8,      // 初始化
    SAB_ERP_LABEL_TEMPLATE_APINAME      =   9,      //author sxy
    SAB_ERP_LABEL_TEMPLATE_APIPRO       =   10
} PACKER_SET_PARAM;

/*********** character constants  *************/
#define SEPARATOR  "$"
#ifdef DEBUG
    #define LOGGER_PROPERTIES_PATH  "./config/log4cxx.properties"
    #define OTHER_CONFIG_PATH "./config/config.cfg"
#else
    #define LOGGER_PROPERTIES_PATH  "/home/sabvi/hiPacker_soft/HiPacker/config/log4cxx.properties"
    #define OTHER_CONFIG_PATH "/home/sabvi/hiPacker_soft/HiPacker/config/config.cfg"
#endif
#define ERP_HOST    "ERP_Address"
#define ERP_PORT    "ERP_RequestPort"
/** Printer **/
#define DEFAULT_DPI 304
#define DEFAULT_DARKNESS 20
#define DEFAULT_START_X_POS 0
#define DEFAULT_START_Y_POS 0

/** error code **/
#define SAB_PACKER_S_FAIL                  0x1100   // 失败
#define SAB_PACKER_E_PARA_NULL             0x1101   // 参数指针为空
#define SAB_PACKER_E_MEM_NULL              0x1102   // 传入的内存为空
#define SAB_PACKER_E_MEM_OVER              0x1103   // 内存溢出
#define SAB_PACKER_MEM_ALLOC_FAIL_ERR      0x1104   // 内存分配失败
#define SAB_PACKER_PARAM_KEY_INVALID       0x1105   // 参数索引号越界
#define SAB_PACKER_PARAM_VALUE_INVALID     0x1106   // 设置参数值出错

#define WORK_TICKET_APPLYID_LENGTH  8
#define WORK_MODE_MAX_NUMBER        10
#define WORK_MODE_TRIGGER_PRINT     1
#define WORK_MODE_REPORT_WORK       2
#define WORK_MODE_FINISH_PRINT      3
#define WORK_MODE_PRINT_AGAIN       4
#define PRINT_FIRST                 0
#define PRINT_AGAIN                 0



#endif // PACKERBACE_H
