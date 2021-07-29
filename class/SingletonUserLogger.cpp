#include "SingletonUserLogger.h"

//SingletonUserLogger::SingletonUserLogger()
//{

//}

SingletonUserLogger::SingletonUserLogger(const string& name)
{
    loggerName = name;
    logger = Logger::getLogger(name);
}

string SingletonUserLogger::getLoggerName() const
{
    return loggerName;
}

void SingletonUserLogger::setLoggerName(const string& value)
{
    loggerName = value;
}

LoggerPtr SingletonUserLogger::getLogger() const
{
    return logger;
}

void SingletonUserLogger::setLogger(const LoggerPtr& value)
{
    logger = value;
}

SingletonUserLogger::SingletonUserLogger()
{
    loggerName = "SystemLogger";
    logger = Logger::getLogger("SystemLogger");
}
