#ifndef STRATEGYOUTS_H
#define STRATEGYOUTS_H

#include "serialport.h"

class SelectStorage
{
public:
    virtual ~SelectStorage() {}
    virtual void init() = 0;
};

class StorageUFS: public SelectStorage
{
public:
    void init()
    {
        char cmd[] = "AT+QFLDS=\"UFS\"\r\n";

        SerialPort::getInstance().writeToSerial(cmd, sizeof(cmd) - 1);
        sleep(1);
        int bytes_read = SerialPort::getInstance().readFromSerial();
        SerialPort::getInstance().writeToConsole(SerialPort::getInstance().getReadBuffer(), bytes_read);
    }
};

class StorageRAM: public SelectStorage
{
public:
    void init()
    {
        char cmd[] = "AT+QFLDS=\"RAM\"\r\n";

        SerialPort::getInstance().writeToSerial(cmd, sizeof(cmd) - 1);
        sleep(1);
        int bytes_read = SerialPort::getInstance().readFromSerial();
        SerialPort::getInstance().writeToConsole(SerialPort::getInstance().getReadBuffer(), bytes_read);

    }
};

class StorageSD: public SelectStorage
{
public:
    void init()
    {
        char cmd[] = "AT+QFLDS=\"SD\"\r\n";

        SerialPort::getInstance().writeToSerial(cmd, sizeof(cmd) - 1);
        sleep(1);
        int bytes_read = SerialPort::getInstance().readFromSerial();
        SerialPort::getInstance().writeToConsole(SerialPort::getInstance().getReadBuffer(), bytes_read);

    }
};

class ModemStorage
{
public:
    ModemStorage(SelectStorage *comp) : p(comp) {}
    ~ModemStorage() { delete p; }
    void init()
    {
        p->init();
    }
private:
    SelectStorage *p;
};

#endif // STRATEGYOUTS_H
