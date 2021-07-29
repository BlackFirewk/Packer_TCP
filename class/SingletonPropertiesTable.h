#ifndef SINGLETONPROPERTIESTABLE_H
#define SINGLETONPROPERTIESTABLE_H

#include <string>
#include <map>
using namespace std;

class SingletonPropertiesTable
{
public:
    static SingletonPropertiesTable& GetInstance()
    {
        static SingletonPropertiesTable instance;//静态局部变量仅会被初始化一次
        return instance;
    }

    map<string, string> getPropertiesTable()const ;
    void setPropertiesTable(const map<string, string>& value);

private:
    SingletonPropertiesTable();
    SingletonPropertiesTable(const SingletonPropertiesTable&);
    SingletonPropertiesTable& operator = (const SingletonPropertiesTable&);
    map<string, string> propertiesTable;
};

#endif // PROPERTIESTABLE_H
