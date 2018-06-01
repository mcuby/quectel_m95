#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <fcntl.h>   /* File Control Definitions           */
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions 	   */
#include <errno.h>   /* ERROR Number Definitions           */
#include <iostream>
#include <algorithm>

class SerialPort; // опережающее объявление

class SerialPortDestroyer
{
private:
    SerialPort *p_instance;
public:
    ~SerialPortDestroyer();
    void initialize(SerialPort *p);
};

class SerialPort
{
private:
    int fd; /* File Descriptor */

    static const int SIZE = 2048;
    char readBuffer[SIZE]; /* Buffer to store the data received              */

    static SerialPort *p_instance;
    static SerialPortDestroyer destroyer;
protected:
    SerialPort();
    SerialPort(const SerialPort &);
    SerialPort &operator=(SerialPort &);
    ~SerialPort();
    friend class SerialPortDestroyer;
public:
    int getFd();
    int getSizeReadBuffer();
    char *getReadBuffer();
    static SerialPort &getInstance();
    int readTestData();
    int readFromSerial();
    int writeToSerial(const char *cmd, int sizeCmd);
    int writeToConsole(const char *buf, int size);
    int writeAndRead(const std::string &bufWrite, int sleepTime, char *bufRead, int sizeBufRead, int log = 1);
};

class MachineSerialPort
{
    class StateSerialPort *current;
public:
    MachineSerialPort();
    void setCurrent(StateSerialPort *s)
    {
        current = s;
    }
    void at();
    void ati();
    void cbc();
};

class StateSerialPort
{
public:
    virtual void at(MachineSerialPort *m) {}
    virtual void ati(MachineSerialPort *m) {}
    virtual void cbc(MachineSerialPort *m) {}
};

class At: public StateSerialPort
{
public:
    At() {}
    ~At() {}
    void at(MachineSerialPort *m);
};

class Ati: public StateSerialPort
{
public:
    Ati() {}
    ~Ati() {}
    void ati(MachineSerialPort *m);
};

class Cbc: public StateSerialPort
{
public:
    Cbc() {}
    ~Cbc() {}
    void cbc(MachineSerialPort *m);
};

#endif // SERIALPORT_H
