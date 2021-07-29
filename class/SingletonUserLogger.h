#ifndef SIGLETONUSERLOGGER_H
#define SIGLETONUSERLOGGER_H

#include <string>
#include "log4cxx/logger.h"
#include "log4cxx/propertyconfigurator.h"

using namespace log4cxx;
using namespace std;

class SingletonUserLogger
{
public:
    static SingletonUserLogger& GetInstance()
    {
        static SingletonUserLogger instance;
        return instance;
    }

    static void setLoggerProperties(const File& propertiesFile)
    {
        PropertyConfigurator::configure(propertiesFile);
    }

    string getLoggerName() const;
    void setLoggerName(const string& value);

    LoggerPtr getLogger() const;
    void setLogger(const LoggerPtr& value);

private:
    SingletonUserLogger();
    SingletonUserLogger(const string& name);
    SingletonUserLogger& operator = (const SingletonUserLogger&);
    string loggerName;
    LoggerPtr logger;
};

#endif // USERLOGGER_H
