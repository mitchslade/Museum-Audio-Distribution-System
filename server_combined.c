/*******************************************************************
    ESD Coursework Server
    Written by James Harvey
    * gcc -pthread -Wall -c "%f"
    * cc -pthread -Wall -o "%e" "%f"
*******************************************************************/

//Includes 
#include <stdio.h>
#include <string.h>    //strlen
#include <stdlib.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <pthread.h> //for threading , link with lpthread
#include "protocol.h" // protocol structure
#include "Functions.h"//definitions for functions
#include <string.h>
#include <gst/gst.h>

//Variables
#define LOGGED_OFF 0
#define LOGGED_ON 1
#define AlarmfileURL "/home/eccles/ESD/AlarmSound.ogg"
CustomData data[DEVICELIMIT];


//Functions
void *alert();
void *connection_handler(void *);
int logon(int deviceID);
int logoff(int deviceID);
int getFileFromDB(int Exhibit, char Language, char Difficulty, char *p_fileURL);
int rateChange(int deviceID, int rate);
int playtoggle(int deviceID);
int charbcd2int(Protocol_struct protocol);
int playstatus = 0;



  
int main(int argc , char *argv[])
{
 printf("Server Started\r\n");
 	  /* Initialize GStreamer */
  int i;
  gst_init (&argc, &argv);
  
  /* Initialize our data structure */
  
  memset (&data, 0, sizeof (data));
  printf("Device array made\r\n");
  for(i=0; i<=DEVICELIMIT; i++)
  {
	data[i].loop = g_main_loop_new (NULL, FALSE);
	//g_main_loop_run (data[i].loop);
  }
  printf("Device array filled\r\n");
  
  pthread_t thread_id;
  
  pthread_create( &thread_id , NULL ,  alert , NULL);
  
	//Variables
    int socket_desc;
	int	client_sock;
	int c;
    struct sockaddr_in server;
	struct sockaddr_in client;
     
    //Create TCP socket 
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket\r\n");
    }
    puts("Socket created\r\n");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("Bind failed. Error\r\n");
        return 1;
    }
    puts("Bind done\r\n");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept any incoming connection
    puts("Waiting for incoming connections...\r\n");
    c = sizeof(struct sockaddr_in);
	
	
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted\r\n");
        if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &client_sock) < 0)
        {
            perror("could not create thread\r\n");
            return 1;
        }
        
        //Now join the thread , so that we don't terminate before the thread
        //pthread_join( thread_id , NULL);
        puts("Handler assigned\r\n");
		
		//Delay added to reduce likelihood of client_sock and host_name
		//changing before the values have been saved by the thread. 
		int i =0;
		while(i<10)
		{
			i++;
		}
		
    }
     
    if (client_sock < 0)
    {
        perror("accept failed\r\n");
        return 1;
    }
     
    return 0;
}
 
/*
 * This will handle connection for each client
 * */

void *connection_handler(void *socket_desc)
{   
    //Variables
	int loggedState;
    int sock = *(int*)socket_desc;
	int pin_int = 0;
	int read_size;
    int num_err;
    struct sockaddr_in addr;
	Protocol_struct client_message;
	Protocol_struct server_msg;
    char clientip[20];
	char filename[256];
	char *fileURL;
	char *message;
	char *client_msg;
	
	//Initialise Pointers
	fileURL = filename;
	client_msg = (char*) &client_message;
	message = "Client Connected!!!\r\n";

	loggedState = LOGGED_OFF;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    getpeername(sock, (struct sockaddr *)&addr, &addr_size);
    strcpy(clientip, inet_ntoa(addr.sin_addr)); 
	
    //Send some messages to the client
    write(sock , message , strlen(message));
     
    //Receive a message from client
    while( (read_size = recv(sock , (char *)&client_message , sizeof(client_message) , 0)) > 0 )
    {
			num_err=0;
			//convert Device Pin to int
			pin_int = charbcd2int(client_message);
			if ((pin_int > 9999) || (pin_int < 0)) {
				//update to invalid PIN number
				printf("Invalid PIN number\r\n");
				num_err = 1;
			}
			//print out message received for debugging
			puts("---------------------------------------------------");
			//printf("Pin (chars) = %d,%d,%d,%d\r\n", client_message.Pin_1, client_message.Pin_2, client_message.Pin_3, client_message.Pin_4);
			printf("Thread = %d || Pin (int) = %d || Function = %d\r\n", sock, pin_int, client_message.Function);
			//printf("Function = %d\r\n", client_message.Function);
			//printf("Language %d\r\n", client_message.Language);
			//printf("Difficulty = %d\r\n", client_message.Difficulty);
			//printf("Exhibit = %d\r\n", client_message.Exhibit);
			puts("---------------------------------------------------");
			
			//switch based on the function value
			if (num_err == 0) {
				switch(client_message.Function)
				{
					case LOGON:
						if (loggedState != LOGGED_ON)
						{
							num_err = logon(pin_int);
							if (num_err == 0)
							{
								printf("\nUser logged on Paul Cadd\n");
								data[pin_int].HostName = clientip;
								loggedState = LOGGED_ON;
							}
							else 
							{
								//update to failed logon number
								printf("Failed to logon\r\n");
								num_err = 100;
							}
						}
						else
						{
							//update to already logged on number
							printf("Already logged on\r\n");
							num_err = 101;
						}
						break;
					case LOGOFF:
						if (loggedState == LOGGED_ON)
						{
							num_err = logoff(pin_int);
							if (num_err == 0)
							{
								loggedState = LOGGED_OFF;
							}
							else 
							{
								//update to failed logoff number
								printf("Failed to logoff\r\n");
								num_err = 1;
							}
						}
						else
						{
							//update to already logged off number
							printf("Already logged off\r\n");
							num_err = 1;
						}
						break;
					case PLAY:		
						if (loggedState == LOGGED_ON)
						{
							num_err = playtoggle(pin_int);
						}
						else
						{
							printf("Device is not logged on");
							num_err = 102;
						}
						break;
					case PAUSE:
						if (loggedState == LOGGED_ON)
						{
							num_err = playtoggle(pin_int);
						}
						else
						{
							printf("Device is not logged on");
							num_err = 102;
						}
						break;
					case REWIND:
						if (loggedState == LOGGED_ON)
						{
							num_err = rateChange(pin_int,-3);
						}
						else
						{
							printf("Device is not logged on");
							num_err = 102;
						}
						break;
					case FAST_FORWARD:
						if (loggedState == LOGGED_ON)
						{
							num_err = rateChange(pin_int,3);
						}
						else
						{
							printf("Device is not logged on");
							num_err = 102;
						}
						break;
					case REWIND_STOP:
						if (loggedState == LOGGED_ON)
						{
							num_err = rateChange(pin_int,1);
						}
						else
						{
							printf("Device is not logged on");
							num_err = 102;
						}
						break;
					case FAST_FORWARD_STOP:
						if (loggedState == LOGGED_ON)
						{
							num_err = rateChange(pin_int,1);
						}
						else
						{
							printf("Device is not logged on");
							num_err = 102;
						}
						break;
					case FILE_REQ:
						if (loggedState == LOGGED_ON)
						{
							printf("\nCode has got to getFileFromDB line\n");
							printf("\nThe value of Exhibit is: %d", client_message.Exhibit);
							printf("\nThe value of Language is: %c", client_message.Language);
							printf("\nThe value of Difficulty is: %c", client_message.Difficulty);
							num_err = getFileFromDB(client_message.Exhibit,client_message.Language, client_message.Difficulty, fileURL);// needs to interact with database
							if (num_err != 0)
							{
								break;
							}
							num_err = buildPipeline(fileURL, &data[pin_int]);
						}
						else
						{
							printf("Device is not logged on");
							num_err = 102;
						}
						break;
					default :
						printf("Unknown Command - %d\r\n",client_message.Function);
						//update to unknown command number
						num_err = 104;
				}
			}

		//generate standard response
		server_msg.Pin_1 = client_message.Pin_1;
		server_msg.Pin_2 = client_message.Pin_2;
		server_msg.Pin_3 = client_message.Pin_3;
		server_msg.Pin_4 = client_message.Pin_4;
		server_msg.Function = SRV_RESPONSE;
		server_msg.Language = 0;
		server_msg.Difficulty = 0;
		server_msg.Exhibit = num_err;
		//convert to char array
		//client_message[4] = server_msg.Function;
		//Send the message back to client
		
		client_msg = (char*) &server_msg;
        write(sock , client_msg , sizeof(server_msg));
		
		
		
	
		//set messages to 0 so they can be repopulated
		memset(&client_message, 0, sizeof(client_message));
		memset(&server_msg, 0, sizeof(server_msg));
    }
    //If nothing is read from the socket (as blocking function used will only return zero when disconnected) 
    if(read_size == 0)
    {
        printf("Client disconnected from socket %d\r\n", sock);
        fflush(stdout);
    }
    else if(read_size == -1) //
    {
        printf("Failed to receive data from socket %d\r\n", sock);
    }
         
    return 0;
}

/*  LOGON Funtion Moved to museumDB.c - P. Cadd 26/4/2015
int logon(int deviceID)
{
	printf("LOGON - %d\r\n", deviceID);
	return 0;
}
*/


/* LOGOFF Function Moved to museumDB.c - P. Cadd 26/4/2015
int logoff(int deviceID)
{
	printf("LOGOFF - %d\r\n", deviceID);
	return 0;
}
*/

/* getFileFrom DB Function moved to museumDB.c - P. Cadd 26/4/2015
int getFileFromDB(int exhibit, char Language, char Difficulty, char *p_fileURL)
{
	strcpy(p_fileURL,"/home/eccles/ESD/TestSong.ogg");
	return 0;
}
*/

int charbcd2int(Protocol_struct protocol)
{
	int answer = 0;
	answer += 	((protocol.Pin_4) * 1);
	answer += 	((protocol.Pin_3) * 10);
	answer += 	((protocol.Pin_2) * 100);
	answer += 	((protocol.Pin_1) * 1000);
	return answer;
}

int rateChange(int deviceID, int rate)
{
	data[deviceID].rate = rate;
	send_seek_event(&data[deviceID]);
	printf("Change play rate of %d to %d\r\n",deviceID,rate);
	return 0;
}

int playtoggle(int deviceID)
{
	togglePlayPause(&data[deviceID]);
	return 0;
}

void *alert()
{
	char alert;
	while(1)
	{
		alert = getchar();
		if ((alert == 'f') || (alert == 'F'))
		{
			printf("I am alerting!!!");
			int i;
			for (i=0; i<DEVICELIMIT; i++)
			{
				if (data[i].pipeline != NULL)
				{
					buildPipeline(AlarmfileURL, &data[i]);
				}
			}
		}
	}	
}
