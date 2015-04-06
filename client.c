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
void exhibit_menu();
void sub_menus(char decision);
void play_mode();
void send_data();


int check_if_number(char pin);
int sockfd, portno, n;
struct sockaddr_in serv_addr;
struct hostent *server;
char buffer[256];
char function;



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
printf("\033[2J");
printf("UWEMCSs Museum Audio Device \r\n"); // prints a nice hello message. this will also play over audio
sleep(3); // a three second wait to start the system and prepare the user for some excellent entermtainment
printf("\033[2J");
printf("Please Enter Pin:"); // the user is requested to enter a pin
do{ // this is done at least once, as it is a do loop
data_in.Pin_1 = (Get_Input()-48); // the user input pin is entered, and each digit is asssigned to an item in the protocol.
//error = error + check_if_number(data_in.Pin_1); //the function then calls check_if_number. this returns a 1 if there is an error, so error is incremented
data_in.Pin_2 = (Get_Input()-48);
//error = error + check_if_number(data_in.Pin_2);
data_in.Pin_3 = (Get_Input()-48);
//error = error + check_if_number(data_in.Pin_3);
data_in.Pin_4 = (Get_Input()-48);
//error = error + check_if_number(data_in.Pin_4);
if(error != 0) // if error is not 0 - i.e there have been errors
{
printf("\033[2J");
printf("Incorrect Pin. Please Try Again: \r\n");//the pin is incorrect and the user is told
attempt_count++;// the number of incorrect attempts is incremented
}
if(attempt_count >= 10) // if they do 10 attempts, they are locked out
{
printf("Please contact a museum assistant"); // on 10 attempts, this is displayed and they cannot use it
}
}while (error > 0); //keep running this until there are no errors in the pin
printf("Pin Accepted! \r\n"); // when there are no errors, the pin is correct. well done to the user.
sleep(1);
printf("\033[2J");
//data_in.Spare = 0b1111; // assigns a value to the spare data in the protocol.

data_in.Function = 128;
send_data(data_in);
}

int check_if_number(char pin) // a char of an input pin is passed in
{
int check_result = 0; // the result of the check is initialised to 0
if(pin >= '0' && pin <= '9') //check if it is a number
{
check_result = 0; //if it is a number, the result reamins 0
}
else
{
check_result = 1; //else, it is not a number, the results is set to 1
}

return check_result; // the result is returned
}

int check_abc(char abc_input) // this checks if an input is a letter A-C
{
int check_result = 0;
if (abc_input == 'A' || 'a' || 'B' || 'b' || 'C' || 'c') // checks if it is a upper or lower case A-C (either will do)
{
check_result  = 0; // if it is, the result reamins 0
}
else
{
check_result = 1; //if it is not one of the above, the result is set to 1
}



return check_result; // the result is returned
}

char main_menu() // this is the main menu the user sees
{
int error_check = 0;
char abc_input;
char decision;
getchar();
do{ // the following is done at least once
printf("\033[2J");
printf("Enter Exhibit - press A \r\n");  // various menu options
printf("Change Language - press B \r\n");
printf("Change Default Difficulty - press C \r\n");
abc_input = Get_Input(); // an abc input is received

error_check = check_abc(abc_input); // ths input is checked to see if it is indeed abc

}while (error_check != 0); // continue to do this until an abc input is received

switch(abc_input) // depedning on the abc input, a different decison is set
{
case 'A':
decision = 'e'; // exhibit decision
break;
case 'a':
decision = 'e'; // exhibit decision
break;
case 'B':
decision = 'l'; // language decision
break;
case 'b':
decision = 'l'; // language decision
break;
case 'C':
decision = 'd'; // difficulty decision
break;
case 'c':
decision = 'd'; // difficulty decision
break;
}

printf("\033[2J");
return decision; // the decison is returned
}

void sub_menus(char decision) // this receiveds the decion made in the main menu
{
switch(decision)
{
case 'd':
difficulty_menu(); // if it is a d, the difficult menu is called.
break;

case 'l':
language_menu(); // if it is a l, the language menu is called.
break;

case 'e':
exhibit_menu(); // if it is a e, the exhibit menu is called.
break;
}
sub_menus(main_menu());

}


char difficulty_menu() // this is the menu where the user can select a difficulty

{
int error_check = 0;
char abc_input;
char decision;
getchar();
do{
printf("\033[2J");
printf("Primary School - press A \r\n"); // these are the various difficulty levels
printf("GCSE - press B \r\n");
printf("A Level - press C \r\n");
printf("Specialist Degree - press D \r\n");
printf("Nobel Prize Winner - press E \r\n");
abc_input = Get_Input(); // an abc input is received

error_check = check_abc(abc_input); // the input is checked to see if it is an abc input

}while (error_check != 0); // continue to do this until an abc input is received

switch(abc_input) // switch decision based on abc input
{
case 'A':
decision = 'p'; //difficulty level chosen.
data_in.Difficulty = 1; // difficulty level set in protocol
printf("Primary School difficulty selected \r\n"); //difficulty level displayed
break;
case 'a':
decision = 'p'; //difficulty level chosen.
data_in.Difficulty = 1; // difficulty level set in protocol
printf("Primary School difficulty selected \r\n"); //difficulty level displayed
break;
case 'B':
decision = 'g'; //difficulty level chosen.
data_in.Difficulty = 2; // difficulty level set in protocol
printf("GCSE difficulty selected \r\n"); //difficulty level displayed
break;
case 'b':
decision = 'g'; //difficulty level chosen.
data_in.Difficulty = 2; // difficulty level set in protocol
printf("GCSE difficulty selected \r\n"); //difficulty level displayed
break;
case 'C':
decision = 'a'; //difficulty level chosen.
data_in.Difficulty = 3; // difficulty level set in protocol
printf("A level difficulty selected \r\n"); //difficulty level displayed
break;
case 'c':
decision = 'a'; //difficulty level chosen.
data_in.Difficulty = 3; // difficulty level set in protocol
printf("A level difficulty selected \r\n"); //difficulty level displayed
break;
case 'D':
decision = 's'; //difficulty level chosen.
data_in.Difficulty = 4; // difficulty level set in protocol
printf("Specialist degree difficulty selected \r\n"); //difficulty level displayed
break;
case 'd':
decision = 's'; //difficulty level chosen.
data_in.Difficulty = 4; // difficulty level set in protocol
printf("Specialist degree difficulty selected \r\n"); //difficulty level displayed
break;
case 'E': //difficulty level chosen.
decision = 'n';
data_in.Difficulty = 5; // difficulty level set in protocol
printf("Nobel Prize Winner difficulty selected \r\n"); //difficulty level displayed
break;
case 'e': //difficulty level chosen.
decision = 'n';
data_in.Difficulty = 5; // difficulty level set in protocol
printf("Nobel Prize Winner difficulty selected \r\n"); //difficulty level displayed
break;


}

return decision;
}

char language_menu() // this is the menu where the user can selecte a language


{
int error_check = 0;
char abc_input;
char decision;
getchar();
do{ //this is done at least once
printf("English - press A \r\n"); //the user is given language options
printf("French - press B \r\n");

abc_input = Get_Input();  // an abc input is received

error_check = check_abc(abc_input); // the input is checked to see if it is an abc input

}while (error_check != 0); // continue to do this until an abc input is received

switch(abc_input) // switch decision based on abc input
{
case 'A':
decision = 'e';
printf("English language selected \r\n"); //english selected
data_in.Language= 1; //language is set in the protocol
break;
case 'a':
decision = 'e';
printf("English language selected  selected \r\n"); //english selected
data_in.Language = 1; //language is set in the protocol
break;
case 'B':
decision = 'f';
printf("French language selected \r\n"); //french selected
data_in.Language= 2; //language is set in the protocol
break;
case 'b':
decision = 'f';
printf("French language selected \r\n"); //french selected
data_in.Language = 2; //language is set in the protocol
break;

}

return decision;
}


void exhibit_menu() //this menu allows the user to select an exhibit

{
char abc_input = 0;
int error_check = 0;
int exhibit_input = 0;
getchar();
do{ //this is done at least once
printf("Enter Exhibit Code \r\n"); //user can enter an exhbit code
scanf("%i",&exhibit_input);
if(exhibit_input >= 0 && exhibit_input <= 25000) //checks if it is a valid number
{
	error_check = 0; //if it is valid, error check is still 0
}
else
{
	error_check = 1; // if it is not valid, error check is set to 1
	printf("Invalid exhibit. Try again"); //user is told this is invalid
}

}while (error_check != 0); // keep doing this until the exhibit is valid
data_in.Exhibit = exhibit_input; // the exhibit number is set in the protocol

error_check = 0;
selection:
do{
printf("\033[2J");
printf("Play - press A \r\n"); // the user can select to play the exhibit
printf("Change difficulty - press B \r\n"); //the user can select to change difficulty

abc_input = Get_Input(); // an abc input is receieved
error_check = check_abc(abc_input); // the input is checked to see if it is valid

}while (error_check != 0); // keep doing this until the input is valid

while(1)
{
switch(abc_input)
{
case 'A':
case 'a':
printf("\033[2J");
play_mode(); // play mode selected
break;

case 'B':
difficulty_menu(); //difficulty mode is selected
goto selection; // the user can then choose the play the track
break;

case 'b':
difficulty_menu(); //difficulty mode is selected
goto selection; // the user can then choose the play the track
break;
}
}
}

void play_mode()
{

	data_in.Function = 16 ;
	send_data(data_in);
	printf("\033[2J");
	printf("Playing exhibit %i \r\n", data_in.Exhibit); // the exhibit is playing!
	
	while(1)
	{
	function_get(); // the program waits for a function
	if(function == 'b' && data_in.Function == 2) // if the function is back and it is currently paused
	{
	sub_menus(main_menu()); // just go straight back to menu
	}

	else if(function == 'b' && (data_in.Function == 1 || data_in.Function == 16)) //if it has just started playing, is playing and back is selected
	{
	data_in.Function = 2; // pause
	send_data(data_in); //send data
	sub_menus(main_menu()); // main menu
	}

	else
	{
	send_data(data_in);
	}

	}

}

void function_get() // this gets the function
{
	char abc_input;

	abc_input = Get_Input();
	switch(abc_input){ // this reads an abc input
		case 'a':
		case 'A':
			if(data_in.Function == 8)
			{
			function = 'g' ;
			data_in.Function = 9;
			printf("\033[2J");
			printf("Playing exhibit %i \r\n", data_in.Exhibit); // the exhibit is playing!
			}
			else if(data_in.Function == 4)
			{
			function = 't';
			data_in.Function = 5;
			printf("\033[2J");
			printf("Playing exhibit %i \r\n", data_in.Exhibit); // the exhibit is playing!
			}
			else
			{
			function = 'p'; //play
			printf("\033[2J");
			data_in.Function = 1; //the function is set in the protocol.
			printf("Playing exhibit %i \r\n", data_in.Exhibit); // the exhibit is playing!
			}
			break;

		case 'B':
		case 'b':
			function = 's'; //stop
			data_in.Function = 2; //the function is set in the protocol.
			printf("\033[2J");
			printf("Exhibit %i paused! \r\n", data_in.Exhibit); // the exhibit is playing!
			break;
		case 'C':
		case 'c':
			function = 'f'; //forward
			printf("\033[2J");
			printf("Exhibit %i fastforwarding! \r\n", data_in.Exhibit); // the exhibit is playing!
			data_in.Function = 8; //the function is set in the protocol.
			break;
		case 'D':
		case 'd':
			function = 'r'; //rewind
			printf("\033[2J");
			printf("Exhibit %i rewinding! \r\n", data_in.Exhibit); // the exhibit is playing!
			data_in.Function = 4; //the function is set in the protocol.
			break;
		case 'E':
		case 'e':
			function = 'b'; //back
			//data_in.Function = 2;
			//printf("\033[2J");
			break;

	}
}


void send_data() // this sends the data to the server
{

	n = write(sockfd,(char*)&data_in,strlen((char*)&data_in)); // writes the data in the buffer
	if (n < 0)
	{

	error("ERROR writing to socket"); //socket error!
	}

	bzero(buffer,256); // re-initialises the buffer
	

}

void read_data()
{
	n = read(sockfd,buffer,255); //reads the data from the buffer
	if (n < 0)
	{
	error("ERROR reading from socket"); //error reading
	printf("%s\n",buffer);
	}
	close(sockfd);
}
