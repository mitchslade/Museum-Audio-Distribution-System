#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "protocol.h"

unsigned char Get_Input();
void User_Interface();
PROTOCOL Get_Data();

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
	User_Interface();
	PROTOCOL testprotocol;
    
    while (1)
    {
	//	printf("Please enter the message: ");

	//	bzero(buffer,256);
	//	fgets(buffer,255,stdin);
		testprotocol = Get_Data();
		//n = write(sockfd,buffer,strlen(buffer));
		n = write(sockfd,(char*)&testprotocol,strlen((char*)&testprotocol));
		if (n < 0)
			 error("ERROR writing to socket");
		bzero(buffer,256);
		n = read(sockfd,buffer,255);
		if (n < 0)
			 error("ERROR reading from socket");
		printf("%s\n",buffer);

    }
    close(sockfd);
    return 0;

}

PROTOCOL Get_Data()
{
	PROTOCOL data_in;

	//so h1ere, mitch has a function that reads a keypad and gets an input key, i.e number8.
	//the output of that function is then set equal to protocol.pin_1. and so on.

	printf("Enter pin 1:");
	data_in.Pin_1 = Get_Input();
	printf("Enter pin 2:");
	data_in.Pin_2 = Get_Input();
	printf("Enter pin 3:");
	data_in.Pin_3 = Get_Input();
	printf("Enter pin 4:");
	data_in.Pin_4 = Get_Input();

	data_in.Spare = 0b1111;

	printf("Enter function:");
	data_in.Function = Get_Input();

	printf("Enter difficulty:");
	data_in.Difficulty = Get_Input();

	printf("Enter exhibit:");
	data_in.Exhibit = Get_Input();

	return data_in;
}

void User_Interface()
{
	printf("Welcome to the museum!\r\n");


}

unsigned char Get_Input()
{
	int input;
	unsigned char User_Input;
	scanf("%i",&input);
	User_Input = (unsigned char)(input);
	return User_Input;
}


void connection setup()
{
	int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int error_number = 0;
    char buffer[256];
    unsigned int wait_time = 5;

    
    do{ //opens up a do...while loop that checks if an error has occured
    
    sleep(wait_time);

    if (argc < 3) { //checks correct arguments have been passed to program
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("ERROR opening socket");
        error_number ++;
        
    }
    
    
    server = gethostbyname(argv[1]);
    if (server == NULL) 
    {
        fprintf(stderr,"ERROR, no such host\n");
        error_number ++;
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
        error("ERROR connecting");
        error_number ++;
    }
    
    }while (error > 0);
        
}
