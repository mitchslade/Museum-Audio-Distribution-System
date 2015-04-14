// Keypad Functionality for Musium aduio system
// Mitch Slade

//includes
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>




void writeToScreen(char* MessageToDisplay)
{ //sart writetoscreen
	
	
	// open communitcation
	// variable declaration
	struct termios serial;
	char buffer[32];		// setting buffer to 32, the size of the screen
	
	int fd = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);
	
	//Internal error check
	if (fd == -1) 
	{
		printf("Something Went Wrong! Your penis was computed as being too small");
		return -1;
	}
	
	// Set up serial
	serial.c_cflag &= ~(CSIZE|CSTOPB|HUPCL);
	serial.c_cflag |= (CS8|PARENB|PARODD|CLOCAL|CREAD);
	serial.c_iflag &= ~(IXON|IXOFF|IXANY|INLCR|IGNBRK|BRKINT|PARMRK|INLCR|IGNCR);
	serial.c_iflag |= (IGNPAR|INPCK|ISTRIP|ICRNL);
	serial.c_oflag &= ~OPOST;
	serial.c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
	cfsetispeed(&serial, B9600);
	cfsetospeed(&serial, B9600);

	tcsetattr(fd, TCSANOW, &serial); // Apply configuration
	printf(MessageToDisplay);
	
	write(fd, MessageToDisplay, strlen(MessageToDisplay));
	
	
	//close comms
	close(fd);
	
	
} //end writetoscreen





int main(int argc, char* argv[]) AMA
{
	
	char* MessageToOutput = malloc(sizeof(char)*32);
	
	
	printf("insert message:");
	scanf("%[^\n]s", MessageToOutput);
	printf(MessageToOutput);
	
	writeToScreen(MessageToOutput);
	
	
	



return 0;

}
