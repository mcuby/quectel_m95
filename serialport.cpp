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

        SerialPortSettings.c_cflag = (SerialPortSettings.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        SerialPortSettings.c_iflag &= ~IGNBRK;         // disable break processing
        SerialPortSettings.c_lflag = 0;                // no signaling chars, no echo,
        // no canonical processing
        SerialPortSettings.c_oflag = 0;                // no remapping, no delays
        SerialPortSettings.c_cc[VMIN]  = 0;            // read doesn't block
        SerialPortSettings.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        SerialPortSettings.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
        // enable reading
        SerialPortSettings.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        SerialPortSettings.c_cflag |= 0;
        SerialPortSettings.c_cflag &= ~CSTOPB;
        SerialPortSettings.c_cflag &= ~CRTSCTS;

//        /* 8N1 Mode */
//        SerialPortSettings.c_cflag &= ~PARENB;   /* Disables the Parity Enable bit(PARENB),So No Parity   */
//        SerialPortSettings.c_cflag &= ~CSTOPB;   /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
//        SerialPortSettings.c_cflag &= ~CSIZE;	 /* Clears the mask for setting the data size             */
//        SerialPortSettings.c_cflag |=  CS8;      /* Set the data bits = 8                                 */

//        SerialPortSettings.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                         */
//        SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */


//        SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);          /* Disable XON/XOFF flow control both i/p and o/p */
//        SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /* Non Cannonical mode                            */

//        SerialPortSettings.c_oflag &= ~OPOST;/*No Output Processing*/

//        /* Setting Time outs */
//        SerialPortSettings.c_cc[VMIN] = 10; /* Read at least 10 characters */
//        SerialPortSettings.c_cc[VTIME] = 0; /* Wait indefinetly   */


        if ((tcsetattr(fd, TCSANOW, &SerialPortSettings)) != 0) { /* Set the attributes to the termios structure*/
            std::cout << "\nERROR ! in Setting attributes\n";
            close(fd); /* Close the serial port */
        } else {
            std::cout << "\nBaudRate = 115200 \n  StopBits = 1 \n  Parity   = none\n";
            if (tcgetattr(fd, &SerialPortSettings) != 0) {
                perror("error from tggetattr");
                SerialPortSettings.c_cc[VMIN]  = 0 ? 1 : 0;
                SerialPortSettings.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

                if (tcsetattr(fd, TCSANOW, &SerialPortSettings) != 0)
                    perror("error setting term attributes");
//                return;
            }


//            tcflush(fd, TCIOFLUSH);  /* Discards old data in the rx buffer            */
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
    tcflush(fd, TCOFLUSH);  /* Discards old data in the tx buffer            */
    write(fd, cmd, static_cast<size_t>(sizeCmd));
    return 0;
}

int SerialPort::readFromSerial()
{
    ssize_t bytes_read = 0; /* Number of bytes read by the read() system call */

    int fd = SerialPort::getInstance().getFd();
    char *readBuffer = SerialPort::getInstance().getReadBuffer();
    int size = SerialPort::getInstance().getSizeReadBuffer();

    bytes_read = read(fd, readBuffer, static_cast<size_t>(size));  /* Read the data                   */
    tcflush(fd, TCIFLUSH);  /* Discards old data in the rx buffer            */

    return static_cast<int>(bytes_read);
}

int SerialPort::writeToConsole(const char *buf, int size)
{
    for (int i = 0; i < size; i++)	 /*printing only the received characters*/
        printf("%c", buf[i]);

    std::cout << "----------------------------------------------------------\n";
    return 0;
}

int SerialPort::writeAndRead(const std::string &bufWrite, int sleepTime, char *bufRead, int sizeBufRead, int log)
{
    SerialPort::getInstance().writeToSerial(bufWrite.c_str(), static_cast<int>(bufWrite.size()));
    usleep(static_cast<unsigned int>(sleepTime));
    int bytes_read = SerialPort::getInstance().readFromSerial();

    std::copy(SerialPort::getInstance().getReadBuffer(), SerialPort::getInstance().getReadBuffer() + ((bytes_read > sizeBufRead) ? sizeBufRead : bytes_read), bufRead);

    if (log)
        SerialPort::getInstance().writeToConsole(SerialPort::getInstance().getReadBuffer(), bytes_read);

    return bytes_read;
}
