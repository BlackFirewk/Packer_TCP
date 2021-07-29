#ifndef QRCODEDATA_H
#define QRCODEDATA_H

#include <string>
using namespace std;

class QrCodeData
{
public:
    QrCodeData();

private:
    double xPos;
    double yPos;
    int darkness;
    int size;
    string level;
    string qrCode;
};

#endif // QRCODE_H
