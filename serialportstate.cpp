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
    static constexpr std::string_view cmd= "AT\r\n";
    char bufRead[SerialPort::getInstance().getSizeReadBuffer()];
    SerialPort::getInstance().writeAndRead(std::string(cmd).c_str(), 1000, bufRead, sizeof(bufRead));

    m->setCurrent(new Ati);
    delete this;
}

void Ati::ati(MachineSerialPort *m)
{
    /*------------------------------- Read data from serial port -----------------------------*/

    static constexpr std::string_view cmd = "ATI\r\n";
    char bufRead[SerialPort::getInstance().getSizeReadBuffer()];
    SerialPort::getInstance().writeAndRead(std::string(cmd).c_str(), 1000, bufRead, sizeof(bufRead));

    m->setCurrent(new Cbc);
    delete this;
}

void Cbc::cbc(MachineSerialPort *m)
{
    /*------------------------------- Read data from serial port -----------------------------*/

    static constexpr std::string_view cmd = "AT+CBC\r\n";
    char bufRead[SerialPort::getInstance().getSizeReadBuffer()];
    SerialPort::getInstance().writeAndRead(std::string(cmd).c_str(), 1000, bufRead, sizeof(bufRead));

    delete this;
}
