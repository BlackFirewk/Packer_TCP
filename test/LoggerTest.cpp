#include "LoggerTest.h"
#include "log4cxx/logger.h"
#include "log4cxx/propertyconfigurator.h"

using namespace log4cxx;

LoggerTest::LoggerTest()
{}

int LoggerTest::logger4cxxTest()
{
    PropertyConfigurator::configure("../../log4cxx.properties");
    LoggerPtr untiTest = Logger::getLogger("LoggerTest");
    LOG4CXX_INFO(untiTest, "Test for");

    return 0;
}
