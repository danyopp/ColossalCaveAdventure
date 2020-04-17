/*******************************************************
 * Author: Daniel Yopp
 * Date: 5/1/19
 * Program: Adventure
 * Desscription: program that develops a maze from files and allows user to choose how to work through it
 * ***************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

void* game();
void* timegetter();

//Global Thread and Mutex controls
pthread_t threads[2];
pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t midMutex = PTHREAD_MUTEX_INITIALIZER;
int continuethread = 1; 


/***********************************************
 * Function Name: Time Getter
 * Descrption: A function designed to run on a parallel thread that writes current time to system file 
 * * ********************************************/
void* timegetter()
{	
	while(continuethread)
	{
		//game thread starts with lock on mymutex
		pthread_mutex_lock(&midMutex);
		//game thread unlocks mymutex to allow time to run
		pthread_mutex_lock(&myMutex);
		if(continuethread)
		{
			pthread_mutex_unlock(&midMutex);
			//game thread locks midmutex to ensure only one iteration of time loop 

			//Get time data to string
			int bufsize = 80;
			char dateString[bufsize];
			time_t rawtime;
			struct tm * timeinfo;
			time (&rawtime);
			timeinfo = localtime(&rawtime);
			strftime(dateString, 80, "%I:%M%p, %A, %B %d, %Y", timeinfo); 
	
			//write time data to currentTime.txt 
			FILE*  dateData;
			dateData = fopen("currentTime.txt", "w");
			fputs (dateString, dateData);
			fclose(dateData); 	
	
			pthread_mutex_unlock(&myMutex);
			//game thread locks myMutex
			//game thread unlocks midMutex
		}
	}		
}





/**********************************************************
 * Function Name: gameplay
 * Description: Function reads in rooms from file and prompts user on how to move
 * *******************************************************/
void* game()
{
	//Hold main mutex control over time thread
	pthread_mutex_lock(&myMutex); 
	int successNum;
	successNum = pthread_create(&threads[1], NULL, timegetter, NULL );
	assert(0 == successNum);





/////////////get newest rooms directory
	int newestDirTime = -1;
	char targetDirPrefix[32] = "yoppd.rooms.";

	//initialize a cstring with all null terms
	char newestDirName[256]; 
	memset(newestDirName, '\0', sizeof(newestDirName));

	DIR* dirToCheck; //starting director
	struct dirent *fileInDir; //Hold the current subdir
	struct stat dirAttributes; //holds subdir info

	dirToCheck = opendir("."); //Opens directory that program is in

	if (dirToCheck > 0) //can open current directory
	{
		while((fileInDir = readdir(dirToCheck)) != NULL)
		{
			if (strstr(fileInDir->d_name, targetDirPrefix) != NULL) //If entry has prefix
			{
				stat(fileInDir->d_name, &dirAttributes); //get entry attributes
				if ((int)dirAttributes.st_mtime > newestDirTime) //If this time is bigger then this directory is selected
				{
					newestDirTime = (int)dirAttributes.st_mtime;
					memset(newestDirName, '\0', sizeof(newestDirName));
					strcpy(newestDirName, fileInDir->d_name);
				}
			} 
		}
	}
	closedir(dirToCheck);




///////////////Save all file names in target directory to an array
	//arrays to hold each room title
	char r0Name[14], r1Name[14], r2Name[14], r3Name[14], r4Name[14], r5Name[14], r6Name[14];
	char* nameArray[7] = {r0Name, r1Name, r2Name, r3Name, r4Name, r5Name, r6Name};

	//cycle through and save each file name in the target directory
	DIR * files;
	struct dirent *dir;
	files = opendir(newestDirName);
	if (files)
		{	
			int filecounter = 0;	
			while ((dir = readdir(files)) != NULL)
			{
			
				if (dir->d_type == DT_REG) //Avoid the system files "." and ".."
				{	
					strcpy( nameArray[filecounter], dir->d_name);
					filecounter++;
				}
			}
			closedir(files);
		}	





//////////////Find the first room - looking for "ROOM TYPE: START_ROOM
	int foundStart = 0; //bool to denote that the sarting room file is found
	int filecounter = 0; //used to index through array of file names  
	FILE * roomFile;

	char fileLocation[50]; //file location in directory, will hold name of sart room when loop breaks
	char lineExtract[50]; //line pulled from file
	
	while (!foundStart)  //cycle through array of file names and check each file until you find the start room 
	{
		sprintf(fileLocation, "%s/%s", newestDirName, nameArray[filecounter]); 	//sets file location variable to hold name of room
		roomFile = fopen (fileLocation, "r");
		while ( fscanf(roomFile, "%s", lineExtract) !=EOF)
		{
			if (strcmp(lineExtract, "START_ROOM") == 0)
			{
				foundStart = 1;
			}
		}
		fclose(roomFile);
		filecounter++;
	}
	
  



	
////////////////Loop for game play, reads in data from file location and prompts user for input
//		Evaluates user input and provides appropriate output		

	char path[600];				//variable holds the path of rooms user has taken so far in game as a string
	int steps = 0;				//number of steps user has taken
	memset(path, '\0', sizeof(path));	//clearing string var
	int optionDisplayed = 0;		//variable used for preventing a comma output before first connection 
	int atEnd = 0;				//variable used to break out of while loop once endroom is found 
	while(atEnd == 0)
	{
		//clear nameArray to hold new connections
		memset(r0Name, '\0', 14);	
		memset(r1Name, '\0', 14);	
		memset(r2Name, '\0', 14);	
		memset(r3Name, '\0', 14);	
		memset(r4Name, '\0', 14);	
		memset(r5Name, '\0', 14);	
		memset(r6Name, '\0', 14);	

		int totalOptions = 0;	//keeps track of how many connections the current room has
		optionDisplayed = 0;	//comma deciding variable

		roomFile = fopen (fileLocation, "r");
		while ( fscanf(roomFile, "%s", lineExtract) !=EOF)	//loop that reads file word by word
		{
			if (strcmp(lineExtract, "NAME:") == 0)		//when name is found, next token holds roomname
			{
				fscanf(roomFile, "%s", lineExtract);	//print room info to console
				printf("CURRENT LOCATION: %s\n", lineExtract);
				printf("POSSIBLE CONNECTIONS: ");
			}
			if (strcmp(lineExtract, "CONNECTION") == 0)	//when connection is found, next token holds a connection
			{
				if (optionDisplayed == 1)		//comma deciding variable
				{printf(", "); }
				fscanf(roomFile, "%s", lineExtract);	//junk read of number and colon
				fscanf(roomFile, "%s", lineExtract);	//connection name
				printf("%s", lineExtract);
				optionDisplayed = 1;
			
				//save option
				strcpy(nameArray[totalOptions], lineExtract);	//store connection name
				totalOptions++;					//increment count	
			}
			if (strcmp(lineExtract, "TYPE:") == 0)		//token signifies all connections have been read
			{
				printf(".\nWHERE TO? >");
			}
	
		}
		
		
		fclose(roomFile);


		// get input from user
		char *userInput = NULL ;
		size_t inputSize = 0;
		int goodInput = 0;	//variable used to mark if the user input matches a room in the array
		
		
		getline(&userInput, &inputSize, stdin);	
  
		char* strEnd = strchr(userInput, '\n'); //remove newline char from end of user input
		*strEnd = '\0';

		int timecall = 0;	//variable used when user asks for time so that bad input message is not given
		int LoopCounter = 0;	//variable used to increment loop for comparing user input to room connections 
	
		//test for time input
		if ( strcmp( userInput, "time") == 0)
		{
			pthread_mutex_unlock(&myMutex);
			timecall = 1;	//mark time call flag so no bad input message to user
			pthread_mutex_lock(&midMutex); 
			//time thread will not unlock midMutex until it has locked myMutex
			//this ensures that time thread will run one iteration of its loop	
			pthread_mutex_lock(&myMutex);
			//time thread will not unlock myMutex until it has created time file	
			pthread_mutex_unlock(&midMutex);
			
			
			//read and output time from updated time file 
			char *fInput = NULL ;
			size_t iSize = 0;
			FILE * tryit;
			
			tryit = fopen("currentTime.txt", "r");
			getline(&fInput, &inputSize, tryit);	
		
			printf("\n%s\n\n", fInput);
			fclose(tryit);
			free(fInput);
			fInput = NULL;
		}		

		
		//Loop tests if the userInput matches and of the connections of the room
		//If a match is found, it updates the filelocation where data is pulled from 
		while(LoopCounter < 7)
		{
			 
			
			if (strcmp(userInput, nameArray[LoopCounter]) == 0)
			{	
				sprintf(path, "%s\n%s", path, nameArray[LoopCounter]);
				//update fileLocation for next game  while loop iteration
				sprintf(fileLocation, "%s/%s_room", newestDirName, nameArray[LoopCounter]);  
				goodInput = 1; //flag used to say a match in array has been found
				steps++;	
				printf("\n");
			}
			LoopCounter++;
		}

		//If user entered unrecognized input, then display reprompt message. Note: filelocation stays the same for a new iteration  
		if (goodInput == 0 && timecall == 0)
		{	printf("\nHUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN.\n\n");}		
		
		//free getline memory
		free(userInput);
		userInput = NULL;

				
		// check for end of path in filelocation variable
		roomFile = fopen (fileLocation, "r");
		while ( fscanf(roomFile, "%s", lineExtract) !=EOF)
		{
		
			if (strcmp(lineExtract, "END_ROOM") == 0)
			{
				atEnd = 1;
				printf("YOU HAVE FOUND THE END ROOM. CONGRADULATIONS!\n");
				printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:", steps);
				printf("%s\n", path);			 

				continuethread = 0;
				pthread_mutex_unlock(&myMutex);
				pthread_cancel(threads[1]);		
				successNum = pthread_join(threads[1], NULL);
				assert(0 == successNum);
			}
		}
	
		fclose(roomFile);
	}
}








int main()
{
	//create thread game first so that it can get mymutex lock
	int successNum = pthread_create(&threads[0], NULL, game, NULL);
	assert(0 == successNum);

	//when game thread ends, kill infinate loop in time thread and wait for thread to stop
	successNum = pthread_join(threads[0], NULL);
	assert(0 == successNum);
	continuethread = 0;

	//cancel both threads
	pthread_cancel(threads[0]);
	pthread_cancel(threads[1]);	
	return 0;
}


