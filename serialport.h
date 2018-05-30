#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <fcntl.h>   /* File Control Definitions           */
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions 	   */
#include <errno.h>   /* ERROR Number Definitions           */
#include <iostream>

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
    char read_buffer[SIZE]; /* Buffer to store the data received              */
    static SerialPort *p_instance;
    static SerialPortDestroyer destroyer;
protected:
    SerialPort();
    SerialPort(const SerialPort &);
    SerialPort &operator=(SerialPort &);
    ~SerialPort();
    friend class SerialPortDestroyer;
public:
    static SerialPort &getInstance();
    int readData();
};


#endif // SERIALPORT_H
