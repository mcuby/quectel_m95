#ifndef STRATEGYOUTS_H
#define STRATEGYOUTS_H

#include "serialport.h"
#include <string>

class SelectStorage
{
protected:
    static constexpr std::string_view EndCmd = "\r\n";
    static constexpr std::string_view QfldsCmd = "AT+QFLDS=";
    static constexpr std::string_view QflstCmd = "AT+QFLST=";
public:
    virtual ~SelectStorage() {}
    virtual void getDataStorageSize() = 0;
    virtual void listFiles(std::string fileName) = 0;
};

class StorageUFS: public SelectStorage
{
private:
    static constexpr std::string_view memoryType = "UFS";
public:
    void getDataStorageSize()
    {
        std::string cmd =  std::string(QfldsCmd) + std::string(memoryType) + std::string(EndCmd);
        SerialPort::getInstance().writeToSerial(cmd.c_str(), static_cast<int>(cmd.size()));
        sleep(1);
        int bytes_read = SerialPort::getInstance().readFromSerial();
        SerialPort::getInstance().writeToConsole(SerialPort::getInstance().getReadBuffer(), bytes_read);
    }
    void listFiles(std::string fileName)
    {
        std::string cmd =  std::string(QflstCmd) + std::string("\"") + fileName +  std::string("\"") + std::string(EndCmd);
        SerialPort::getInstance().writeToSerial(cmd.c_str(), static_cast<int>(cmd.size()));
        sleep(1);
        int bytes_read = SerialPort::getInstance().readFromSerial();
        SerialPort::getInstance().writeToConsole(SerialPort::getInstance().getReadBuffer(), bytes_read);
    }
};

class StorageRAM: public SelectStorage
{
private:
    static constexpr std::string_view memoryType = "RAM";
public:
    void getDataStorageSize()
    {
        std::string cmd =  std::string(QfldsCmd) + std::string("\"") + std::string(memoryType) + std::string("\"") + std::string(EndCmd);
        SerialPort::getInstance().writeToSerial(cmd.c_str(), static_cast<int>(cmd.size()));
        sleep(1);
        int bytes_read = SerialPort::getInstance().readFromSerial();
        SerialPort::getInstance().writeToConsole(SerialPort::getInstance().getReadBuffer(), bytes_read);
    }
    void listFiles(std::string fileName)
    {
        std::string cmd =  std::string(QflstCmd) + std::string("\"") + std::string(memoryType) + std::string(":") + fileName +  std::string("\"") + std::string(EndCmd);
        SerialPort::getInstance().writeToSerial(cmd.c_str(), static_cast<int>(cmd.size()));
        sleep(1);
        int bytes_read = SerialPort::getInstance().readFromSerial();
        SerialPort::getInstance().writeToConsole(SerialPort::getInstance().getReadBuffer(), bytes_read);
    }
};

class StorageSD: public SelectStorage
{
private:
    static constexpr std::string_view memoryType = "SD";
public:
    void getDataStorageSize()
    {
        std::string cmd =  std::string(QfldsCmd) + std::string(memoryType) + std::string(EndCmd);
        SerialPort::getInstance().writeToSerial(cmd.c_str(), static_cast<int>(cmd.size()));
        sleep(1);
        int bytes_read = SerialPort::getInstance().readFromSerial();
        SerialPort::getInstance().writeToConsole(SerialPort::getInstance().getReadBuffer(), bytes_read);
    }
    void listFiles(std::string fileName)
    {
        std::string cmd =  std::string(QflstCmd) + std::string("\"") + std::string(memoryType) + std::string(":") + fileName +  std::string("\"") + std::string(EndCmd);
        SerialPort::getInstance().writeToSerial(cmd.c_str(), static_cast<int>(cmd.size()));
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

    void getDataStorageSize()
    {
        p->getDataStorageSize();
    }

    void listFiles(std::string fileName)
    {
        p->listFiles(fileName);
    }

private:
    SelectStorage *p;
};

#endif // STRATEGYOUTS_H
