/***************************************************
    ESD Coursework MuseumDB Connection & Functions
    Written by Paul Cadd
    * Requires MySQL Libraries Installed to Compile
    * gcc museumDB.c -o museumDB  -Wall 
    * 
***************************************************/



/**************************************************
 Included Libraries
***************************************************/
#include <mysql/my_config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/my_global.h>
#include <mysql/mysql.h>
#include "museumDB.h"



/**************************************************
 function prototypes declarations
***************************************************/
int connectDB(); //Connect to the database
int createTable();
int insertRecord();
int selectRecord();
int disconnectDB();
int check_logon();
int logoff(int pin_int);
int logon(int pin_int);
//static int pin_int = 2468;
int insertNewUser();
static void print_error (MYSQL *conn, char *message);
void process_result_set (MYSQL *conn, MYSQL_RES *res_set);
void print_dashes (MYSQL_RES *res_set);
int rtn_Exhibition(int Exhibit, char *rtn_Ex);
char *rtn_Language(char Language, char *rtn_Lang);
char *rtn_Difficulty(char Difficulty, char *rtn_Diff);



/**************************************************
 User define for DB Logon
***************************************************/
static char *server = "82.46.81.203";
static char *uname = "museum_db_admin";
static char *passw = "Z1ggystardust666";
static char *dbname = "museuminfodb";
static unsigned int port = 3306;
static char *unix_socket = NULL;
static unsigned int flag = 0;



/**************************************************
 define pointer to result set
***************************************************/
MYSQL_RES *res_set;



/**************************************************
 define pointer connection handle
***************************************************/
static MYSQL *conn;



/**************************************************
 Build File Path From DataBase
 ****************************************************/
int getFileFromDB(int Exhibit, char Language, char Difficulty, char *p_fileURL)
{
	
	getchar();
	/* Set up variables */
	printf("\nThe value of exhibition you have chosen is: %d\n", Exhibit);
	printf("\nThe value of language you have chosen is: %c\n", Language);
	printf("\nThe value of difficulty you have chosen is: %d\n", Difficulty);
	getchar();
    //char Language;
    //char Difficulty;
	char dbexhibit[40];
	char dblanguage[256];
	char dbdifficulty [256];
	char *rtn_Ex;
	char *rtn_Lang;
	char *rtn_Diff;
	char url[44] ="";
	//char buffer[2];
	char filetype[4] = ".ogg";
	char baseurl[256] = "/home/gbcadp/museuminfo/audio/exhibitions/";
	
	/* Set up pointers */
	rtn_Ex = dbexhibit;
	rtn_Lang = dblanguage;
	rtn_Diff = dbdifficulty;
	
	rtn_Exhibition(Exhibit, rtn_Ex);
	rtn_Language(Language, rtn_Lang);
	rtn_Difficulty(Difficulty, rtn_Diff);
	printf("\nThe Exhibition is set to:  %s\n", rtn_Ex);
	printf("\nThe Language is set to:  %s\n", rtn_Lang);
	printf("\nThe Difficulty is set to:  %s\n", rtn_Diff);
	snprintf(url, sizeof(url), "%s/%s/%s/difficulty/%s%s", baseurl, rtn_Ex, rtn_Lang, rtn_Diff, filetype);
	printf("The URL that will be used is: %s", url);
	
	
	return 0;
	
	/* Old Code
	strcpy(p_fileURL,"/home/eccles/ESD/TestSong.ogg");
	return 0;
	*/
}


/**************************************************
 Return Exhibition for user PIN
***************************************************/
int rtn_Exhibition(int exhibit, char *rtn_Ex)
{
	
	static char selectEx[80];
  	unsigned int i;
  	memset(selectEx, 0, sizeof(selectEx));
  	sprintf(selectEx, "SELECT exhibition_description FROM exhibitions WHERE exhibition_id='%d'", exhibit);
	//printf("\nThe SELECT statement you have built is: %s\n", selectEx );
	connectDB();
	if (mysql_query (conn, selectEx) != 0)
		{
			print_error (conn, "mysql_query() failed");
		}
	else
		{
		res_set = mysql_store_result (conn); /* generate result set */
		// printf("The value of  the result set is: %d\n", res_set);	
		if (res_set == NULL)
			{ 
				print_error(conn, "mysql_store_result() failed");
			}
		else
			{
			//process_result_set(conn, res_set);
			MYSQL_ROW row;
			while ((row = mysql_fetch_row (res_set)) != NULL)
			{
				for (i = 0; i < mysql_num_fields (res_set); i++)
				{
					strcpy(rtn_Ex,row[i]);
					//printf("\nThe value of selectEx_rtn is: %s", rtn_Ex);
					//printf ("\n%lu rows returned\n",(unsigned long) mysql_num_rows (res_set));
				}
								
			}
		}
		mysql_free_result(res_set);
	}
	disconnectDB();
	return 0;
}



/**************************************************
 Return Language for User PIN
***************************************************/
char *rtn_Language(char Language, char *rtn_Lang)
{
	static char selectLa[80];
  	unsigned int i;
	memset(selectLa, 0, sizeof(selectLa));
  	sprintf(selectLa, "SELECT language_code FROM languages WHERE client_lang_code='%c'", Language);
	//printf("\nThe SELECT statement you have built is: %s\n", selectLa );
	connectDB();
	if (mysql_query (conn, selectLa) != 0)
		{
			print_error (conn, "mysql_query() failed");
		}
	else
		{
		res_set = mysql_store_result (conn); /* generate result set */
		// printf("The value of  the result set is: %d\n", res_set);	
		if (res_set == NULL)
			{ 
				print_error(conn, "mysql_store_result() failed");
			}
		else
			{
			//process_result_set(conn, res_set);
			MYSQL_ROW row;
			while ((row = mysql_fetch_row (res_set)) != NULL)
			{
				for (i = 0; i < mysql_num_fields (res_set); i++)
				{
					strcpy(rtn_Lang, row[i]);
					//printf("\nThe value of selectLa_rtn is: %s", rtn_Lang);
					printf ("\n%lu rows returned\n",(unsigned long) mysql_num_rows (res_set));
				}
								
			}
		}
		mysql_free_result(res_set);
	}
	disconnectDB();
	return 0;
}


	
/**************************************************
 Return Difficulty for User PIN
***************************************************/	
char *rtn_Difficulty(char Difficulty, char *rtn_Diff)
{
	static char selectDi[80];
  	unsigned int i;
	memset(selectDi, 0, sizeof(selectDi));
  	sprintf(selectDi, "SELECT difficulty_description FROM difficulty WHERE difficulty_id='%c'", Difficulty);
	//printf("\nThe SELECT statement you have built is: %s\n", selectDi );
	connectDB();
	if (mysql_query (conn, selectDi) != 0)
		{
			print_error (conn, "mysql_query() failed");
		}
	else
		{
		res_set = mysql_store_result (conn); /* generate result set */
		// printf("The value of  the result set is: %d\n", res_set);	
		if (res_set == NULL)
			{ 
				print_error(conn, "mysql_store_result() failed");
			}
		else
			{
			//process_result_set(conn, res_set);
			MYSQL_ROW row;
			while ((row = mysql_fetch_row (res_set)) != NULL)
			{
				for (i = 0; i < mysql_num_fields (res_set); i++)
				{
					strcpy(rtn_Diff, row[i]);
					//printf("\nThe value of selectDi_rtn is: %s", rtn_Diff);
					printf ("\n%lu rows returned\n",(unsigned long) mysql_num_rows (res_set));
				}
								
			}
		}
		mysql_free_result(res_set);
	}
	disconnectDB();
	return 0;
}



/**************************************************
 Function for processing error from MySQL operation
***************************************************/
void finish_with_error()
{
  fprintf(stderr, "%s\n", mysql_error(conn));
  mysql_close(conn);
  exit(1);
}



/**************************************************
 Function for connecting to Database
***************************************************/
int connectDB()
{
  
  //printf("\nConnecting to MySQL Server...\n");
  conn = mysql_init(NULL);

  if (conn == NULL)
  {
     fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
  }


   if (mysql_real_connect(conn, server, uname, passw,
          dbname, port, unix_socket, flag) == NULL)
  {
      finish_with_error();
  }
  //printf("\nConnection Successful\n");
  return(0);
}



/**************************************************
 Function for creating tableName
***************************************************/
int createTable() {

  if (mysql_query(conn, "DROP TABLE IF EXISTS tableName")) {
      finish_with_error();
  }
  printf("\nCreate tableName(uname, passwd)\n");

  if (mysql_query(conn, "CREATE TABLE tableName(accountName VARCHAR (30) primary key,password VARCHAR (30) )")) {
      finish_with_error();
  }
  return(0);
}



/**************************************************
 Function for inserting a record into Table
***************************************************/
int insertRecord() {
  printf("\nInsert records into Orders Table....\n");
  if (mysql_query(conn, "INSERT INTO orders WHERE user_id = 'xyz' VALUES('exhibition_id','language_id','difficulty_id','order_date','paid_id','audio_path','first_name','last_name','email_address','address_postcode')")) {
      finish_with_error();
  }
  return(0);
}



/**************************************************
 Function for selecting records from orders table
***************************************************/
int selectRecords() {
  printf("\nSelecting file path from orders table...\n");
  if (mysql_query(conn, "SELECT * FROM exhibitions")) {
     finish_with_error();
  }

  /*Save records selected from dataTable to the variable result*/
  MYSQL_RES *result = mysql_store_result(conn);

  if (result == NULL)
  {
      finish_with_error();
  }
  
  /* Get the number of fields of dataTable*/
  int num_fields = mysql_num_fields(result);

  MYSQL_ROW row;
  int i;

  /* Iterate through all rows selected from dataTable and print them*/
  while ((row = mysql_fetch_row(result)))
  {
      for (i = 0; i < num_fields; i++) //Iterate through all fields of each rec$
      {
          printf("%s ", row[i] ? row[i] : "NULL");
      }
          printf("\n");
  }
  mysql_free_result(result);
  return(0);
}



/**************************************************
 Function for disconnecting from MySQL DB
***************************************************/
int disconnectDB() {
  //printf("\nDisconnect from MySQL\n");
  mysql_close(conn);
  return(0);
}



/**************************************************
 Function for building the SELECT statement
***************************************************/
char *BuildSelect(int pin_int) {

  static char selectstring[256];
  memset(selectstring, 0, sizeof(selectstring));
  sprintf(selectstring, "SELECT %s FROM %s JOIN %s ON %s.%s=%s.%s WHERE %s.%s='%d'", ap, orders, users, users, uid, orders, uid, users, upin, pin_int);
  printf("The SELECT statement you have built is : %s", selectstring );
  pause();
  return selectstring;
}



/**************************************************
 Function to print Error Message Not Required in Final Build
***************************************************/
static void print_error (MYSQL *conn, char *message){

fprintf (stderr, "%s\n", message);

if (conn != NULL)
	{
	#if MYSQL_VERSION_ID >= 40101
		fprintf (stderr, "Error %u (%s): %s\n",
		mysql_errno (conn), mysql_sqlstate(conn), mysql_error (conn));
	#else
		fprintf (stderr, "Error %u: %s\n",
		mysql_errno (conn), mysql_error (conn));
	#endif
	}	
}



/**************************************************
 Function to Log ON the User from the System
***************************************************/
int logon(int pin_int) {
	connectDB();
  	static char selectstring[256];
  	static char updatestring[256];
  	unsigned int i;
	unsigned int logonstatus;
  	memset(selectstring, 0, sizeof(selectstring));
  	memset(updatestring, 0, sizeof(updatestring));
  	sprintf(selectstring, "SELECT logon_status FROM users WHERE userpin='%d'",pin_int);
  	sprintf(updatestring, "UPDATE users SET logon_status='1' WHERE userpin='%d'",pin_int); 
	//printf("\nThe SELECT statement you have built is: %s\n", selectstring );
	/* process result set, and then deallocate it */
	if (mysql_query (conn, selectstring) != 0)
		{
			print_error (conn, "mysql_query() failed");
		}
	else
		{
		res_set = mysql_store_result (conn); /* generate result set */
		// printf("The value of  the result set is: %d\n", res_set);	
		if (res_set == NULL)
			{ 
				print_error(conn, "mysql_store_result() failed");
			}
		else
			{
			//process_result_set(conn, res_set);
			MYSQL_ROW row;
			while ((row = mysql_fetch_row (res_set)) != NULL)
			{
				for (i = 0; i < mysql_num_fields (res_set); i++)
				{
					logonstatus = atoi(row[i]);
					if (logonstatus == 1)
					{
						printf("\nThe User is already logged on\n");
					}
					if (logonstatus == 0)
					{
						//mysql_query(conn, "UPDATE users SET logon_status='1' WHERE userpin='2468'");
						mysql_query(conn, updatestring);
						printf("\nThe user has been logged on successfully\n");
						//printf("\nThe value of logon status is: %d\n", logonstatus);
						//printf("\nThe value of the logon field is:%s\n", row[i]);
					 }	

				  }
		
				}
				if (mysql_errno (conn) != 0)
				{
			    print_error (conn, "mysql_fetch_row() failed");
				}
				else
				{
				//printf ("\n%lu rows returned\n",(unsigned long) mysql_num_rows (res_set));
			}
		}
		mysql_free_result(res_set);
	}
	disconnectDB();
	//free(memset(selectstring, 0, sizeof(selectstring)));
  	//free(memset(updatestring, 0, sizeof(updatestring)));
	//free(selectstring);
	//free(updatestring);
	
	return (0);
}



/**************************************************
 Function to Log Off the User from the System
***************************************************/
int logoff(int pin_int) {
	connectDB();
  	static char selectstring[256];
  	static char updatestring[256];
  	unsigned int i;
	unsigned int logonstatus;
  	memset(selectstring, 0, sizeof(selectstring));
  	memset(updatestring, 0, sizeof(updatestring));
  	sprintf(selectstring, "SELECT logon_status FROM users WHERE userpin='%d'",pin_int);
  	sprintf(updatestring, "UPDATE users SET logon_status='0' WHERE userpin='%d'",pin_int); 
	printf("\nThe SELECT statement you have built is: %s\n", selectstring );
	/* process result set, and then deallocate it */
	if (mysql_query (conn, selectstring) != 0)
		{
			print_error (conn, "mysql_query() failed");
		}
	else
		{
		res_set = mysql_store_result (conn); /* generate result set */
		// printf("The value of  the result set is: %d\n", res_set);	
		if (res_set == NULL)
			{ 
				print_error(conn, "mysql_store_result() failed");
			}
		else
			{
			//process_result_set(conn, res_set);
			MYSQL_ROW row;
			while ((row = mysql_fetch_row (res_set)) != NULL)
			{
				for (i = 0; i < mysql_num_fields (res_set); i++)
				{
					logonstatus = atoi(row[i]);
					if (logonstatus == 0)
					{
						printf("\nThe User is already logged off\n");
					}
					if (logonstatus == 1)
					{
						mysql_query(conn, updatestring);
						//mysql_query(conn, "UPDATE users SET logon_status='0' WHERE userpin='2468'");
						printf("\n The user has been logged off successfully\n");
						printf("\nThe value of logon status is: %d\n", logonstatus);
						//printf("\nThe value of the logon field is:%s\n", row[i]);
					 }	

				  }
		
				}
				if (mysql_errno (conn) != 0)
				{
			    print_error (conn, "mysql_fetch_row() failed");
				}
				else
				{
				printf ("%lu rows returned\n",(unsigned long) mysql_num_rows (res_set));
			}
		}
		mysql_free_result(res_set);
	}
	disconnectDB();
	//free(memset(selectstring, 0, sizeof(selectstring)));
  	//free(memset(updatestring, 0, sizeof(updatestring)));
	//free(selectstring);
	//free(updatestring);
	return 0;
}	
	


/**************************************************
 Function to process the result set from DB
***************************************************/
void process_result_set (MYSQL *conn, MYSQL_RES *res_set) {

MYSQL_ROW row;
unsigned int i;
unsigned int logonstatus;

while ((row = mysql_fetch_row (res_set)) != NULL)
{
	for (i = 0; i < mysql_num_fields (res_set); i++)
	{
	logonstatus = atoi(row[i]);
	if (logonstatus == 0)
		{
		printf("\nThe User is already logged off");
		}
	if (logonstatus == 1)
		{
		mysql_query(conn, "UPDATE users SET logon_status='0' WHERE userpin='2468'");
		printf("\n The user has been logged off successfully\n");
		}
	
	{
		printf("\nThe value of logon status is: %d\n", logonstatus);
			printf("\nThe value of the logon field is:%s\n", row[i]);
		}	
		//else
		//fputc ('\t', stdout);
		//printf ("%s", row[i] != NULL ? row[i] : "NULL");
	
		
		//printf("\nThe value of the logon field is:%s\n", row[i]);
		//logonstatus = row[i];
		//printf("\nThe value of the logon field is:%s\n", logonstatus);
		
	}
		//fputc ('\n', stdout);
}
	if (mysql_errno (conn) != 0)
		print_error (conn, "mysql_fetch_row() failed");
	else
		printf ("%lu rows returned\n",(unsigned long) mysql_num_rows (res_set));
}



/**************************************************
 Function for printing the dashes around direct SQL queries
***************************************************/
void print_dashes (MYSQL_RES *res_set)
{

	MYSQL_FIELD *field;
	unsigned int i, j;

	mysql_field_seek (res_set, 0);
	fputc ('+', stdout);
	for (i = 0; i < mysql_num_fields (res_set); i++)
	{
	field = mysql_fetch_field (res_set);
		for (j = 0; j < field->max_length + 2; j++)
			fputc ('-', stdout);
			fputc ('+', stdout);
	}
	fputc ('\n', stdout);
}



/**************************************************
 Function for checking LOGON status - Test
***************************************************/
int check_logon()

{

	connectDB();
	if (mysql_query (conn,"SELECT logon_status FROM users WHERE userpin='2468'") != 0)
	print_error (conn, "mysql_query() failed");
else
	{
		res_set = mysql_store_result (conn); /* generate result set */
if (res_set == NULL)
	print_error (conn, "mysql_store_result() failed");
else
	{
	/* process result set, and then deallocate it */
	process_result_set (conn, res_set);
	mysql_free_result (res_set);
	}
}

	return(0);
}



/**************************************************
 Function for generating random and unique 4 digit PIN
***************************************************/
/*
{
	//Will require this SELECT statement:
	//SELECT FLOOR(RAND() * 9999) AS random_num FROM users WHERE "random_num" NOT IN(SELECT userpin FROM users) LIMIT 1;

	//variables
	char url[44] ="";
	char buffer[2];
	char filetype[3] = ".ogg";
	char baseurl[32] = "/home/gbcadp/museuminfo/audio/ex";
	int pin_int;
	int exhibition = 1;
	sprintf(buffer,"%d", exhibition);
	char language[] = "1";
	char difficulty[] = "2";
	snprintf(url, sizeof(url), "%s%s_%s_%s%s", baseurl, buffer, language, difficulty, filetype);
	printf("The URL that will be used is: %s", url);
	return(0);

	
}
*/
