#ifndef PRINTERPROCESS_H
#define PRINTERPROCESS_H


class UserLogger;
class LabelInfoData;
class PrinterProcess
{
public:
    PrinterProcess();
    int PrinterInit();
    int PrintLabel(const LabelInfoData&);
private:
};

#endif // PRINTERPROCESS_H
