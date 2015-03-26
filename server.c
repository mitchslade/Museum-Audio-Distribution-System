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
#include <Protocol.h> // protocol structure
#include <Functions.h>//definitions for functions
#include <string.h>
#include <gst/gst.h>

//Functions
void *connection_handler(void *);
int logon(protocol_struct protocol);
int logoff(protocol_struct protocol);
int getFileFromDB(int exhibit, char Language, char Difficulty, char *p_fileURL);

int rateChange(int deviceID, int rate);
int playtoggle(int deviceID);
int charbcd2int(protocol_struct protocol);
int playstatus = 0;
CustomData data[DEVICELIMIT];

  
int main(int argc , char *argv[])
{
 printf("first first");
 	  /* Initialize GStreamer */
  int i;
  printf("first");
  gst_init (&argc, &argv);
  printf("second");
  
  /* Initialize our data structure */
  
  memset (&data, 0, sizeof (data));
  printf("array made");
  for(i=0; i<=DEVICELIMIT; i++)
  {
	data[i].loop = g_main_loop_new (NULL, FALSE);
	//g_main_loop_run (data[i].loop);
  }
  printf("array filled");
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
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
	pthread_t thread_id;
	
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");
        if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &client_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
        
        //Now join the thread , so that we don't terminate before the thread
        //pthread_join( thread_id , NULL);
        puts("Handler assigned");
		
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
        perror("accept failed");
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
    int sock = *(int*)socket_desc;
	int pin_int = 0;
	int read_size;
    int num_err;
    struct sockaddr_in addr;
	protocol_struct client_message;
	protocol_struct server_msg;
    char clientip[20];
	char filename[256];
	char *fileURL;
	char *message;
	char *client_msg;
	
	//Initialise Pointers
	fileURL = filename;
	client_msg = (char*) &client_message;
	message = "Client Connected!!!";

    socklen_t addr_size = sizeof(struct sockaddr_in);
    getpeername(sock, (struct sockaddr *)&addr, &addr_size);
    strcpy(clientip, inet_ntoa(addr.sin_addr)); 
	
    //Send some messages to the client
    write(sock , message , strlen(message));
     
    //Receive a message from client
    while( (read_size = recv(sock , (char *)&client_message , sizeof(client_message) , 0)) > 0 )
    {
			//convert Device Pin to int
			pin_int = charbcd2int(client_message);
			
			//print out message received for debugging
			puts("---------------------------------------------------");
			printf("Pin (chars) = %d,%d,%d,%d\r\n", client_message.Pin_1, client_message.Pin_2, client_message.Pin_3, client_message.Pin_4);
			printf("Pin (int) = %d\r\n", pin_int);
			printf("Function = %d\r\n", client_message.Function);
			printf("Language %d\r\n", client_message.Language);
			printf("Difficulty = %d\r\n", client_message.Difficulty);
			printf("Exhibit = %d\r\n", client_message.Exhibit);
			puts("---------------------------------------------------");
			
			//switch based on the function value
			switch(client_message.Function)
			{
				case LOGON:
					num_err = logon(client_message);
					break;
				case LOGOFF:
					num_err = logoff(client_message);
					break;
				case PLAY:		
					num_err = playtoggle(pin_int);
					break;
				case PAUSE:
					num_err = playtoggle(pin_int);
					break;
				case REWIND:
					num_err = rateChange(pin_int, -3);
					break;
				case FAST_FORWARD:
					num_err = rateChange(pin_int, 3);
					break;
				case REWIND_STOP:
					num_err = rateChange(pin_int, 1);
					break;
				case FAST_FORWARD_STOP:
					num_err = rateChange(pin_int, 1);
					break;
				case FILE_REQ:
					num_err = getFileFromDB(client_message.Exhibit,client_message.Language, client_message.Difficulty, fileURL);// needs to interact with database
					if (num_err != 0)
					{
						break;
					}

					num_err = buildPipeline(fileURL, clientip, &data[pin_int]);
					break;
				default :
					printf("Unknown Command - %d\r\n",client_message.Function);
					num_err = 101;
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
        printf("Client disconnected from socket %d", sock);
        fflush(stdout);
    }
    else if(read_size == -1) //
    {
        printf("Failed to receive data from socket %d", sock);
    }
         
    return 0;
}
int logon(protocol_struct protocol)
{
	printf("LOGON - %d\r\n", protocol.Function);
	return 0;
}
int logoff(protocol_struct protocol)
{
	printf("LOGOFF - %d\r\n", protocol.Function);
	return 0;
}
int getFileFromDB(int exhibit, char Language, char Difficulty, char *p_fileURL)
{
	strcpy(p_fileURL,"/home/lee/EmbeddedProject/TestSong.ogg");
	return 0;
}

//add device id
/*int BuildPipeline(char *p_fileURL2, char *client_ip, int pin)
{
	printf("Start streaming %s to device %d at ip %s\r\n", p_fileURL2, pin, client_ip);
	return 0;
}*/
int charbcd2int(protocol_struct protocol)
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
	if (playstatus == 0)
	{
		printf("Track on %d is playing", deviceID);
		playstatus = 1;
	}
	else
	{
		printf("Track on %d is paused", deviceID);
		playstatus = 0;
	}
	return 0;
}
