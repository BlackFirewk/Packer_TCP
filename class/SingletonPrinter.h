#ifndef SINGLETONPRINTER_H
#define SINGLETONPRINTER_H

#include "lib/DLPrinter.h"
class SingletonPrinter
{
public:
    static SingletonPrinter& GetInstance()
    {
        static SingletonPrinter instance;
        return instance;
    }

    device_info* getDevice() const;
    void setDevice(device_info* value);

    print_option* getPrint() const;
    void setPrint(print_option* value);

private:
    SingletonPrinter();
    SingletonPrinter(const SingletonPrinter&);
    ~SingletonPrinter();
    SingletonPrinter& operator = (const SingletonPrinter&);
    device_info* device;
    print_option* print;
};

#endif // SINGLETONPRINTER_H
