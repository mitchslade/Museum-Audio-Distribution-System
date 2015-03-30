#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
//#include <conio.h>

#include <netinet/in.h>
#include <netdb.h>
#include "protocol.h"
char Get_Input();
void User_Interface();
PROTOCOL Get_Data();
PROTOCOL data_in;
void connection_setup();
void startup();
void function_get();
char difficulty_menu();
char main_menu();
char language_menu();
char exhibit_menu();
void sub_menus(char decision);
void play_mode();


int check_if_number(char pin);
int sockfd, portno, n;
struct sockaddr_in serv_addr;
struct hostent *server;
char buffer[256];



void error(const char *msg)// this is a generic error function for development purposes
{
perror(msg);//an error message can be passed here
exit(0);
}


int main(int argc, const char *argv[])
{
server = gethostbyname(argv[1]); //this initialise the server to be used - passed into the program from the script
portno = atoi(argv[2]); //this initliasees the port number to be used

connection_setup(); //this sets the connection up
startup(); //this calls the login functionality
while (1) //this is the main loop of the program
{
sub_menus(main_menu()); 

}



return 0;
}


char Get_Input() //this is just a development function
{
char User_Input;
scanf(" %c", &User_Input); // this reads in a char from a console. this will be replaced by a keypad
return User_Input;
}

void connection_setup() //this sets the connection with the server up
{
int error_number = 0; // this initialises an error value to 0
unsigned int wait_time = 5; // this sets a wait time in case of issue
int fails = 0; // this sets a variable for number of failure to connects that have occured

while(1) //forever loop
{
sleep(wait_time); //a wait time between attempts is set. this can be varied
error_number = 0; // error number re-initialised for 0 for this instance
sockfd = socket(AF_INET, SOCK_STREAM, 0); //socket is setup
if (sockfd < 0) // if there is no socket
{
error("ERROR opening socket"); // then there is an error opening the socket
error_number ++; // error number is incremented.

}

if (server == NULL) // if there is no server
{
fprintf(stderr,"ERROR, no such host\n"); // there is no host
error_number ++; // error number is incremented!
}
bzero((char *) &serv_addr, sizeof(serv_addr));
serv_addr.sin_family = AF_INET;
bcopy((char *)server->h_addr,
(char *)&serv_addr.sin_addr.s_addr,
server->h_length);
serv_addr.sin_port = htons(portno);
if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
{
error_number ++;
fails ++;
printf("Connection fail # %d \r\n", fails);
}

else
{
    break; // if no errors have occured, then the loop is exited and connection is successful. program continues in main.
}
}
}

void menu()
{
startup();
}

void startup() //this function sets the hardware up for a user to use
{
int error = 0;
int attempt_count = 0; // this is the number of login attempts
printf("UWEMCSs Museum Audio Device \r\n"); // prints a nice hello message. this will also play over audio
sleep(3); // a three second wait to start the system and prepare the user for some excellent entermtainment
printf("Please Enter Pin:"); // the user is requested to enter a pin
do{ // this is done at least once, as it is a do loop
data_in.Pin_1 = Get_Input(); // the user input pin is entered, and each digit is asssigned to an item in the protocol.
error = error + check_if_number(data_in.Pin_1); //the function then calls check_if_number. this returns a 1 if there is an error, so error is incremented
data_in.Pin_2 = Get_Input();
error = error + check_if_number(data_in.Pin_2);
data_in.Pin_3 = Get_Input();
error = error + check_if_number(data_in.Pin_3);
data_in.Pin_4 = Get_Input();
error = error + check_if_number(data_in.Pin_4);
if(error != 0) // if error is not 0 - i.e there have been errors
{
printf("Incorrect Pin. Please Try Again: \r\n");//the pin is incorrect and the user is told
attempt_count++;// the number of incorrect attempts is incremented
}
if(attempt_count >= 10) // if they do 10 attempts, they are locked out
{
printf("Please contact a museum assistant"); // on 10 attempts, this is displayed and they cannot use it
}
}while (error > 0); //keep running this until there are no errors in the pin
printf("Pin Accepted! \r\n"); // when there are no errors, the pin is correct. well done to the user.
data_in.Spare = 0b1111; // assigns a value to the spare data in the protocol.
}

int check_if_number(char pin)
{
int check_result = 0;
if(pin >= '0' && pin <= '9')
{
check_result = 0;
}
else
{
check_result = 1;
}

return check_result;
}

int check_abc(char abc_input)
{
int check_result = 0;
if (abc_input == 'A' || 'a' || 'B' || 'b' || 'C' || 'c')
{
check_result  = 0;
}
else
{
check_result = 1;
}

return check_result;
}

char main_menu()
{
int error_check = 0;
char abc_input;
char decision;
getchar();
do{
printf("Enter Exhibit - press A \r\n");
printf("Change Language - press B \r\n");
printf("Change Default Difficulty - press C \r\n");
abc_input = Get_Input();

error_check = check_abc(abc_input);

}while (error_check != 0);

switch(abc_input)
{
case 'A':
decision = 'e';
break;
case 'a':
decision = 'e';
break;
case 'B':
decision = 'l';
break;
case 'b':
decision = 'l';
break;
case 'C':
decision = 'd';
break;
case 'c':
decision = 'd';
break;
}

return decision;
}

void sub_menus(char decision)
{
switch(decision)
{
case 'd':
difficulty_menu();
break;

case 'l':
language_menu();
break;

case 'e':
exhibit_menu();
break;
}


}


char difficulty_menu()

{
int error_check = 0;
char abc_input;
char decision;
getchar();
do{
printf("Primary School - press A \r\n");
printf("GCSE - press B \r\n");
printf("A Level - press C \r\n");
printf("Specialist Degree - press D \r\n");
printf("Nobel Prize Winner - press E \r\n");
abc_input = Get_Input();

error_check = check_abc(abc_input);

}while (error_check != 0);

switch(abc_input)
{
case 'A':
decision = 'p';
data_in.Difficulty = 1;
printf("Primary School difficulty selected \r\n");
break;
case 'a':
decision = 'p';
data_in.Difficulty = 1;
printf("Primary School difficulty selected \r\n");
break;
case 'B':
decision = 'g';
data_in.Difficulty = 2;
printf("GCSE difficulty selected \r\n");
break;
case 'b':
decision = 'g';
data_in.Difficulty = 2;
printf("GCSE difficulty selected \r\n");
break;
case 'C':
decision = 'a';
data_in.Difficulty = 3;
printf("A level difficulty selected \r\n");
break;
case 'c':
decision = 'a';
data_in.Difficulty = 3;
printf("A level difficulty selected \r\n");
break;
case 'D':
decision = 's';
data_in.Difficulty = 4;
printf("Specialist degree difficulty selected \r\n");
break;
case 'd':
decision = 's';
data_in.Difficulty = 4;
printf("Specialist degree difficulty selected \r\n");
break;
case 'E':
decision = 'n';
data_in.Difficulty = 5;
printf("Nobel Prize Winner difficulty selected \r\n");
break;
case 'e':
decision = 'n';
data_in.Difficulty = 5;
printf("Nobel Prize Winner difficulty selected \r\n");
break;


}

return decision;
}

char language_menu()


{
int error_check = 0;
char abc_input;
char decision;
getchar();
do{
printf("English - press A \r\n");
printf("French - press B \r\n");

abc_input = Get_Input();

error_check = check_abc(abc_input);

}while (error_check != 0);

switch(abc_input)
{
case 'A':
decision = 'e';
printf("English language selected \r\n");
break;
case 'a':
decision = 'e';
printf("English language selected  selected \r\n");
break;
case 'B':
decision = 'f';
printf("French language selected \r\n");
break;
case 'b':
decision = 'f';
printf("French language selected \r\n");
break;

}

return decision;
}

char exhibit_menu()

{
char abc_input = 0;
int error_check = 0;
int exhibit_input = 0;
char decision;
getchar();
do{
printf("Enter Exhibit Code \r\n");
scanf("%i",&exhibit_input);
if(exhibit_input >= 0 && exhibit_input <= 25000)
{
	error_check = 0;
}
else
{
	error_check = 1;
	printf("Invalid exhibit. Try again");
}

}while (error_check != 0);
data_in.Exhibit = exhibit_input;

error_check = 0;
selection:
do{
printf("Play - press A \r\n");
printf("Change difficulty - press B \r\n");

abc_input = Get_Input();
error_check = check_abc(abc_input);

}while (error_check != 0);

while(1)
{
switch(abc_input)
{
case 'A':
play_mode();
break;

case 'a':
play_mode();
break;

case 'B':
difficulty_menu();
goto selection;
break;

case 'b':
difficulty_menu();
goto selection;
break;
}
}
return decision;
}

void play_mode()
{
	while(1)
	{
	printf("Playing exhibit \r\n");
	function_get();
	send_data(data_in);
	read_data();


	}

}

void function_get()
{
	char abc_input;
	char function;
	abc_input = Get_Input();
	switch(abc_input){
		case 'A':
			function = 'p'; //play
			data_in.Function = 1;
			break;
		case 'a':
			function = 'p'; //play
			data_in.Function = 1;
			break;
		case 'B':
			function = 's'; //stop
			data_in.Function = 2;
			break;
		case 'b':
			function = 's'; //stop
			data_in.Function = 2;
			break;
		case 'C':
			function = 'f'; //forward
			data_in.Function = 8;
			break;
		case 'c':
			function = 'f'; //forward
			data_in.Function = 8;
			break;
		case 'D':
			function = 'r'; //rewind
			data_in.Function = 4;
			break;
		case 'd':
			function = 'r'; //rewind
			data_in.Function = 4;
			break;
		case 'E':
			function = 'b'; //back
			main_menu(sub_menus);
			break;
		case 'e':
			function = 'b'; //back
			main_menu(sub_menus);
			break;

	}
}

void send_data(PROTOCOL testprotocol)
{
	n = 0;
	n = write(sockfd,(char*)&data_in,strlen((char*)&data_in));
	if (n < 0)
	error("ERROR writing to socket");
	bzero(buffer,256);

}

void read_data()
{
	n = read(sockfd,buffer,255);
	if (n < 0)
	{
	error("ERROR reading from socket");
	printf("%s\n",buffer);
	}
	close(sockfd);
}


