#include "SingletonPrinter.h"


SingletonPrinter::SingletonPrinter()
{
    device = new device_info();
    print = new print_option();
}

SingletonPrinter::~SingletonPrinter()
{
    delete device;
    delete print;
}

print_option* SingletonPrinter::getPrint() const
{
    return print;
}

void SingletonPrinter::setPrint(print_option* value)
{
    print = value;
}

device_info* SingletonPrinter::getDevice() const
{
    return device;
}

void SingletonPrinter::setDevice(device_info* value)
{
    device = value;
}
