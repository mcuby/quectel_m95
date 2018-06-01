#include "serialport.h"
#include "strategyouts.h"
#include <string>

int main()
{
    SerialPort::getInstance().readTestData();

    ModemStorage *storage = new ModemStorage(new StorageRAM);
    storage->getDataStorageSize();
    storage->listFiles(std::string("*"));
    storage->listFiles(std::string("test"));
    delete  storage;
}
