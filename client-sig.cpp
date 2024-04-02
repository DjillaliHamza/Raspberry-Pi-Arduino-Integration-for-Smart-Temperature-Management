/*
 * ********************************************************************
 * Single thread client-sig:
 * Automatic reception of signals
 * ********************************************************************
 */

/* ********************************************************************
 * Includes
 * ********************************************************************
 */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions

/* ********************************************************************
 * Variables
 * ********************************************************************
 */
#define BUFLEN 10000
/* Serial port */
#define BDR B9600
int USB = 0;
struct termios tty;
struct termios tty_old;
int n = 0 ;
char buffer[BUFLEN];
/* serial port configuration */
void configure_serialport(void) ;

/* ********************************************************************
 * Main thread
 * 
 * ********************************************************************
 */
int main(void)
{	
	configure_serialport() ;
	
	/*  Automatic reception of signals */
	while(1){
	n=read(USB,&buffer,sizeof(buffer));
	
	if (n > 0) {
    printf("temp is: %s\n", buffer);
	}
	sleep(0.1);
}
	return 0;
}

void configure_serialport(void)
{
	/* Open port */
	// int USB = open( "/dev/ttyUSB0", O_RDWR);
	USB = open( "/dev/ttyACM0", O_RDWR | O_NONBLOCK );
	if (USB < 0) {
    printf("Error %i from open: %s\n", errno, strerror(errno));
	}
	
	/* Set parameters */
	memset (&tty, 0, sizeof tty);

	/* Error Handling */
	if ( tcgetattr ( USB, &tty ) != 0 ) {
	   printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
	}

	/* Save old tty parameters */
	tty_old = tty;

	/* Set Baud Rate */
	cfsetospeed (&tty, (speed_t)BDR);
	cfsetispeed (&tty, (speed_t)BDR);

	/* Setting other Port Stuff */
	tty.c_cflag     &=  ~PARENB;            // Make 8n1: disable parity bit
	tty.c_cflag     &=  ~CSTOPB;			// one stop bit is used
	tty.c_cflag     &=  ~CSIZE;
	tty.c_cflag     |=  CS8;				// 8 bits per byte

	tty.c_cflag     &=  ~CRTSCTS;           // no flow control
	tty.c_cc[VMIN]   =  1;                  // read doesn't block
	tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
	tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines
	
	/* Make raw */
	cfmakeraw(&tty);

	/* Flush Port, then applies attributes */
	tcflush( USB, TCIFLUSH );
	if ( tcsetattr ( USB, TCSANOW, &tty ) != 0) {
	   printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
	}
}
