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

    ssize_t bytes_read = 0; /* Number of bytes read by the read() system call */

    char at[] = "AT\r\n";

    int fd = SerialPort::getInstance().getFd();
    char *readBuffer = SerialPort::getInstance().getReadBuffer();
    int size = SerialPort::getInstance().getSizeReadBuffer();

    write(fd, at, sizeof(at) - 1);
    tcflush(fd, TCOFLUSH);  /* Discards old data in the tx buffer            */

    sleep(1);

    bytes_read = read(fd, readBuffer, size);  /* Read the data                   */
    tcflush(fd, TCIFLUSH);  /* Discards old data in the rx buffer            */

    for (int i = 0; i < bytes_read; i++)	 /*printing only the received characters*/
        printf("%c", readBuffer[i]);

    std::cout << "----------------------------------------------------------\n";

    m->setCurrent(new Ati);
    delete this;
}

void Ati::ati(MachineSerialPort *m)
{
    /*------------------------------- Read data from serial port -----------------------------*/

    ssize_t bytes_read = 0; /* Number of bytes read by the read() system call */

    char ati[] = "ATI\r\n";

    int fd = SerialPort::getInstance().getFd();
    char *readBuffer = SerialPort::getInstance().getReadBuffer();
    int size = SerialPort::getInstance().getSizeReadBuffer();

    write(fd, ati, sizeof(ati) - 1);
    tcflush(fd, TCOFLUSH);  /* Discards old data in the tx buffer            */

    sleep(1);

    bytes_read = read(fd, readBuffer, size);  /* Read the data                   */
    tcflush(fd, TCIFLUSH);  /* Discards old data in the rx buffer            */

    for (int i = 0; i < bytes_read; i++)	 /*printing only the received characters*/
        printf("%c", readBuffer[i]);

    std::cout << "----------------------------------------------------------\n";

    m->setCurrent(new Cbc);
    delete this;
}

void Cbc::cbc(MachineSerialPort *m)
{
    /*------------------------------- Read data from serial port -----------------------------*/

    ssize_t bytes_read = 0; /* Number of bytes read by the read() system call */

    char cbc[] = "AT+CBC\r\n";

    int fd = SerialPort::getInstance().getFd();
    char *readBuffer = SerialPort::getInstance().getReadBuffer();
    int size = SerialPort::getInstance().getSizeReadBuffer();

    write(fd, cbc, sizeof(cbc) - 1);
    tcflush(fd, TCOFLUSH);  /* Discards old data in the tx buffer            */

    sleep(1);

    bytes_read = read(fd, readBuffer, size);  /* Read the data                   */
    tcflush(fd, TCIFLUSH);  /* Discards old data in the rx buffer            */

    for (int i = 0; i < bytes_read; i++)	 /*printing only the received characters*/
        printf("%c", readBuffer[i]);

    std::cout << "----------------------------------------------------------\n";

    delete this;
}
