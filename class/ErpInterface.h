#ifndef _ERPINTERFACE_H
#define _ERPINTERFACE_H
#include <string>
#include "pugiconfig.hpp"
#include "pugixml.hpp"

using namespace pugi;
using namespace std;

class HttpConnect;
class ERPInterface
{
public:
    ERPInterface();
    ERPInterface(const string&, const string&);

    string getInterfaceName() const;
    void setInterfaceName(const string&);

    string getInterfaceAttribute() const;
    void setInterfaceAttribute(const string&);

    int ErpProcessor(HttpConnect http, const string& interface_param);

private:
    string interfaceName;
    string interfaceAttribute;
    xml_node CheckXml(xml_document& srcXml);
    string bulidErpRequestInfo(string&, string&, const string&);
};

#endif // INTERFACE_H
