/***************************************************
    ESD Coursework MuseumDB.h Macro Header
    Written by Paul Cadd
    * Requires MySQL Libraries Installed to Compile
    * gcc museumDB.c -o museumDB  -Wall 
    * Created 9th Feb 2015
***************************************************/

#ifndef MUSEUMDB_H_
#define MUSEUMDB_H_



/******************************
 Enum for Logon/Logoff
******************************/
/*enum Boolean {
	False;
	True;
};
*/



/******************************
 Museum DB Macro
******************************/
const char* ex = "exhibitions";
const char* diff = "difficulty";
const char* users = "users";
const char* uid = "user_id";
const char* upin = "userpin";
const char* orders = "orders";
const char* lang = "language";
const char* ap ="audio_path";




/******************************
 Struct for USer Data
******************************/
typedef struct {
	char *firstName;
	char *lastName;
	char *email;
	char *address1;
	char *address2;
	char *address3;
	char *postCode;
	char *phone;
} newUser;

#endif
