#include "SingletonPropertiesTable.h"

SingletonPropertiesTable::SingletonPropertiesTable()
{

}

map<string, string> SingletonPropertiesTable::getPropertiesTable() const
{
    return propertiesTable;
}

void SingletonPropertiesTable::setPropertiesTable(const map<string, string>& value)
{
    propertiesTable = value;
}


