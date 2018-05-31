#include "serialport.h"
#include "strategyouts.h"

int main()
{
    SerialPort::getInstance().readTestData();

    ModemStorage *storage = new ModemStorage(new StorageRAM);
    storage->init();
    delete  storage;
}
