/*
 * ********************************************************************
 * Single thread client-img:
 * 
 * ********************************************************************
 */
/* ********************************************************************
 * Includes
 * ********************************************************************
 */
#include <pthread.h>
#include <sched.h>
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

#include <opencv2/opencv.hpp>

/* ********************************************************************
 * Variables
 * ********************************************************************
 */
#define PORT 4201
#define LOCAL "127.0.0.1"
#define REMOTE "192.168.1.50"
#define BUFLEN 30
#define WIDTH 640
#define HEIGHT 480
#define DEPTH 1
#define CHANNEL 3

int client_socket;
struct sockaddr_in client_addr;
char buffer[BUFLEN];
char ch ;
const char *server = REMOTE;
int result, len;
int displaytime = 25 ; /* in ms*/
cv::Mat image = cv::Mat(HEIGHT, WIDTH, CV_8UC3) ;
void *function_img (void *arg) ;


/* ********************************************************************
 * Main thread
 * ********************************************************************
 */
int main(void)
{	
	/* Allocate space for image display */
	cv::namedWindow ("Frame client", CV_WINDOW_AUTOSIZE) ;
	
	/* Create unnamed socket and then name it */
	client_socket = socket (AF_INET, SOCK_STREAM, 0);
	client_addr.sin_family = AF_INET;
	result = inet_aton (server, &client_addr.sin_addr);
	/* inet_aton: convert IP addresses from dots-and-number to abinary form and stores it in structure */
	if (result == 0)
	{
	perror ("inet_aton error");
	exit (1);
	}
	client_addr.sin_port = htons (PORT);
	
	/* Connect to the server */
	result = connect (client_socket, (struct sockaddr *) &client_addr, sizeof (client_addr));
	if (result < 0)
	{
	perror ("Client can't connect");
	exit (1);
	}



while(1)
{
printf("frm serveur:");
recv(client_socket, image.data, WIDTH*HEIGHT*DEPTH*CHANNEL,0);

    cv::imshow ("Frame", image) ;

    cv::waitKey(displaytime);
    
    }
	close (client_socket);
	printf ("Connection terminated.  Client shutting down ...\n");
	free (buffer);
	cv::destroyAllWindows() ;
	
	
	return 0;
}

