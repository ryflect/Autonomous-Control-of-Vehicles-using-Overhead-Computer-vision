#include <errno.h>
#include <fcntl.h> 
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h> 

using namespace std;

int set_interface_attribs (int fd, int speed, int parity)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                printf ("error %d from tcgetattr", errno);
                return -1;
        }

        cfsetospeed (&tty, speed);
        cfsetispeed (&tty, speed);

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // disable break processing
        tty.c_lflag = 0;                // no signaling chars, no echo,
                                        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= parity;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
        {
                printf ("error %d from tcsetattr", errno);
                return -1;
        }
        return 0;
}

void set_blocking (int fd, int should_block)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                printf ("error %d from tggetattr", errno);
                return;
        }

        tty.c_cc[VMIN]  = should_block ? 1 : 0;
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
                printf ("error %d setting term attributes", errno);
}

int getPortHandle()
{
    char portname[20] = "/dev/rfcomm0";

    int fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);

    if (fd < 0)
    {
        printf ("error %d opening %s: %s\n", errno, portname, strerror (errno));
        return -1;
    }

    set_interface_attribs (fd, B115200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
    set_blocking (fd, 0);                    // set no blocking

    return fd;
}

//Higher Byte numsteps should be -127 -> 127
short int encodenumSteps(int numSteps)
{
    short int encodedSteps = 0;

    if (numSteps < 0)
        encodedSteps = encodedSteps + 128;

    encodedSteps = encodedSteps + abs(numSteps);

    return encodedSteps<<8;

} 

//LowerByte - speed should be -127 -> 127
short int encodeSpeed(int speed)
{   
    short int encodedSpeed = 0;

    if (speed < 0)
        encodedSpeed = encodedSpeed + 128;

    encodedSpeed = encodedSpeed + abs(speed);
    return encodedSpeed;
}

short int encodeData(int numSteps, int speed)
{
    short int encodedData = 0;
    encodedData = encodenumSteps(numSteps) + encodeSpeed(speed);

    return encodedData;
}

/*int main()
{ 
    short int encodedData = encodeData(40,200);
    printf("Encoded Data: %d\n",encodeData(40,200));

    int fd = getPortHandle();
    usleep(5000000);
    
    while(1)
    {   
        write(fd, "w", 1);
        write (fd, &encodedData, 2);           // send 7 character greeting
        cout<<"Message sent\n";
        usleep(1100000);
    }

    return 0;
}*/