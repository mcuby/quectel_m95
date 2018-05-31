#include "serialport.h"

SerialPort *SerialPort::p_instance = nullptr;
SerialPortDestroyer SerialPort::destroyer;

SerialPortDestroyer::~SerialPortDestroyer()
{
    delete p_instance;
}

void SerialPortDestroyer::initialize(SerialPort *p)
{
    p_instance = p;
}

SerialPort &SerialPort::getInstance()
{
    if (!p_instance) {
        p_instance = new SerialPort();
        destroyer.initialize(p_instance);
    }
    return *p_instance;
}

SerialPort::SerialPort()
{
    /*------------------------------- Opening the Serial Port -------------------------------*/

    /* Change /dev/ttyUSB0 to the one corresponding to your system */

    fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NONBLOCK | O_SYNC);	/* ttyUSB0 is the FT232 based USB2SERIAL Converter   */
    /* O_RDWR   - Read/Write access to serial port       */
    /* O_NOCTTY - No terminal will control the process   */
    /* Open in blocking mode,read will wait              */

    if (fd == -1) {					/* Error Checking */
        std::cout << "\nError! in Opening ttyUSB0  ";
    } else {
        std::cout << "\nttyUSB0 Opened Successfully ";

        /*---------- Setting the Attributes of the serial port using termios structure --------- */

        struct termios SerialPortSettings;	/* Create the structure                          */

        tcgetattr(fd, &SerialPortSettings);	/* Get the current attributes of the Serial port */

        /* Setting the Baud rate */
        cfsetispeed(&SerialPortSettings, B115200); /* Set Read  Speed as 115200                       */
        cfsetospeed(&SerialPortSettings, B115200); /* Set Write Speed as 115200                       */

        /* 8N1 Mode */
        SerialPortSettings.c_cflag &= ~PARENB;   /* Disables the Parity Enable bit(PARENB),So No Parity   */
        SerialPortSettings.c_cflag &= ~CSTOPB;   /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
        SerialPortSettings.c_cflag &= ~CSIZE;	 /* Clears the mask for setting the data size             */
        SerialPortSettings.c_cflag |=  CS8;      /* Set the data bits = 8                                 */

        SerialPortSettings.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                         */
        SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */


        SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);          /* Disable XON/XOFF flow control both i/p and o/p */
        SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /* Non Cannonical mode                            */

        SerialPortSettings.c_oflag &= ~OPOST;/*No Output Processing*/

        /* Setting Time outs */
        SerialPortSettings.c_cc[VMIN] = 10; /* Read at least 10 characters */
        SerialPortSettings.c_cc[VTIME] = 0; /* Wait indefinetly   */


        if ((tcsetattr(fd, TCSANOW, &SerialPortSettings)) != 0) { /* Set the attributes to the termios structure*/
            std::cout << "\nERROR ! in Setting attributes\n";
            close(fd); /* Close the serial port */
        } else {
            std::cout << "\nBaudRate = 115200 \n  StopBits = 1 \n  Parity   = none\n";
        }
        std::cout << "\n----------------------------------------------------------\n";
    }
}

SerialPort::~SerialPort()
{
    if (fd != -1) {
        close(fd); /* Close the serial port */
    }
}

int SerialPort::readTestData()
{
    if (fd != -1) {
        MachineSerialPort state;
        state.at();
        state.ati();
        state.cbc();
    }
    return 0;
}

int SerialPort::getFd()
{
    return fd;
}

char *SerialPort::getReadBuffer()
{
    return readBuffer;
}

int SerialPort::getSizeReadBuffer()
{
    return SIZE;
}

int SerialPort::writeToSerial(const char *cmd, int sizeCmd)
{
    int fd = SerialPort::getInstance().getFd();
    write(fd, cmd, sizeCmd);
    tcflush(fd, TCOFLUSH);  /* Discards old data in the tx buffer            */
    return 0;
}

int SerialPort::readFromSerial()
{
    ssize_t bytes_read = 0; /* Number of bytes read by the read() system call */

    int fd = SerialPort::getInstance().getFd();
    char *readBuffer = SerialPort::getInstance().getReadBuffer();
    int size = SerialPort::getInstance().getSizeReadBuffer();

    bytes_read = read(fd, readBuffer, size);  /* Read the data                   */
    tcflush(fd, TCIFLUSH);  /* Discards old data in the rx buffer            */

    return bytes_read;
}

int SerialPort::writeToConsole(const char *buf, int size)
{
    for (int i = 0; i < size; i++)	 /*printing only the received characters*/
        printf("%c", buf[i]);

    std::cout << "----------------------------------------------------------\n";
}
