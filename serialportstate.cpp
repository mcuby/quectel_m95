#include "serialport.h"

void MachineSerialPort::at()
{
    current->at(this);
}

void MachineSerialPort::ati()
{
    current->ati(this);
}

void MachineSerialPort::cbc()
{
    current->cbc(this);
}

MachineSerialPort::MachineSerialPort()
{
    current = new At();
}

void At::at(MachineSerialPort *m)
{
    /*------------------------------- Read data from serial port -----------------------------*/
    char cmd[] = "AT\r\n";

    SerialPort::getInstance().writeToSerial(cmd, sizeof(cmd) - 1);
    sleep(1);
    int bytes_read = SerialPort::getInstance().readFromSerial();
    SerialPort::getInstance().writeToConsole(SerialPort::getInstance().getReadBuffer(), bytes_read);


    m->setCurrent(new Ati);
    delete this;
}

void Ati::ati(MachineSerialPort *m)
{
    /*------------------------------- Read data from serial port -----------------------------*/

    char cmd[] = "ATI\r\n";

    SerialPort::getInstance().writeToSerial(cmd, sizeof(cmd) - 1);
    sleep(1);
    int bytes_read = SerialPort::getInstance().readFromSerial();
    SerialPort::getInstance().writeToConsole(SerialPort::getInstance().getReadBuffer(), bytes_read);


    m->setCurrent(new Cbc);
    delete this;
}

void Cbc::cbc(MachineSerialPort *m)
{
    /*------------------------------- Read data from serial port -----------------------------*/

    char cmd[] = "AT+CBC\r\n";

    SerialPort::getInstance().writeToSerial(cmd, sizeof(cmd) - 1);
    sleep(1);
    int bytes_read = SerialPort::getInstance().readFromSerial();
    SerialPort::getInstance().writeToConsole(SerialPort::getInstance().getReadBuffer(), bytes_read);

    delete this;
}
