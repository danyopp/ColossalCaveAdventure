#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main()
{
	srand(time(0));
// Declare Room Types
	
	//array to hold room names
	char* roomNames[10];

	//hardcoded array names
	char r1Str[8] = "Bed";
		roomNames[0] = r1Str;
	char r2Str[8] = "Study";
		roomNames[1] = r2Str;
	char r3Str[8] = "Billard";
		roomNames[2] = r3Str;
	char r4Str[8] = "Kitchen";
		roomNames[3] = r4Str;
	char r5Str[8] = "Foyer";
		roomNames[4] = r5Str;
	char r6Str[8] = "Drawing";
		roomNames[5] = r6Str;
	char r7Str[8] = "Music";
		roomNames[6] = r7Str;
	char r8Str[8] = "Library";
		roomNames[7] = r8Str;
	char r9Str[8] = "Armory";
		roomNames[8] = r9Str;
	char r10Str[8] = "Tack";
		roomNames[9] = r10Str;	


//Select 7 random rooms

	int selectedRooms[7];
	int avalibleRooms[10];

	//Create array of avalible rooms for rand to pick from
	int k = 0;
	while ( k < 10)
	{
		avalibleRooms[k] = k;
		k++;
	}
	
	//Use rand function to pick 7 distinct rooms from the avalible array
	//rooms picked are id by 0-9 repersenting the hardcoding above
	//order picked is not relevant to outcome but will be order files are made
	int m = 0;
	while( m < 7)
	{
		int randRoom = rand() % 10;
		if (avalibleRooms[randRoom] >= 0 )
		{
			selectedRooms[m]= randRoom;
			avalibleRooms[randRoom] = -1;
			m++;
		}

	}	

	


//create matches between rooms (min3 - max6)

	//inti matrix to hold matches- Note: negative 1 repersents placeholder
	int R0connect[6] = {-1, -1, -1, -1, -1, -1};
	int R1connect[6] = {-1, -1, -1, -1, -1, -1};
	int R2connect[6] = {-1, -1, -1, -1, -1, -1};
	int R3connect[6] = {-1, -1, -1, -1, -1, -1};
	int R4connect[6] = {-1, -1, -1, -1, -1, -1}; 
	int R5connect[6] = {-1, -1, -1, -1, -1, -1}; 
	int R6connect[6] = {-1, -1, -1, -1, -1, -1};

	int* ConnectRoom[7] = {R0connect, R1connect, R2connect, R3connect, R4connect, R5connect, R6connect};
	
	//create array of counters to keep track of how many matches each room has
	int ConnectCount[7] = {0, 0, 0, 0, 0, 0, 0}; 

	//start producing matches until each room has a minimum of 3
	while (ConnectCount[0] < 3 || ConnectCount[1] < 3 || ConnectCount[2] < 3 || ConnectCount[3] < 3 || ConnectCount[4] < 3 || ConnectCount[5] < 3 || ConnectCount[6] < 3 )
	{
		int randmatchOne = rand() % 7;
		int randmatchTwo = rand() % 7;

//		printf("MATCHMADE: %d-%d\n", randmatchOne, randmatchTwo);
	
		if (ConnectCount[randmatchOne] >= 6 || ConnectCount[randmatchTwo] >= 6)
		{ //skip because one of the rooms already has max connections
//			printf("match fail - Full\n");	
		}   

		else if ( randmatchOne == randmatchTwo)
		{ //skip because room cannot link to self
//			printf("match fail - Same\n");
		}

		else 
		{
		  //test to see if connection already exists
			//counters for while loop 	
		  	k=0;
		  		 
			int matchExists = 0;  
			while (k < 7)
			{
				//test if room link already exists
				if (ConnectRoom[randmatchOne][k] == randmatchTwo)
				{matchExists = 1;}
				k++;
			}
		  
			if(matchExists == 1)
			{
//				printf("match fail - Exists\n");
				//skip - room link already present
			}		
		  	else
			{
				//add match to both rooms and increment counters
				ConnectRoom[randmatchOne][ConnectCount[randmatchOne]] = randmatchTwo;
				ConnectRoom[randmatchTwo][ConnectCount[randmatchTwo]] = randmatchOne;
				ConnectCount[randmatchOne]++;
				ConnectCount[randmatchTwo]++; 	
					
			}
		}
	} 

/*
	//print connections between the rooms
	
	printf("\n");
	printf("Room0: %d\t %d\t %d\t %d\t %d\t %d\n", R0connect[0], R0connect[1], R0connect[2], R0connect[3], R0connect[4], R0connect[5]);
	printf("Room1: %d\t %d\t %d\t %d\t %d\t %d\n", R1connect[0], R1connect[1], R1connect[2], R1connect[3], R1connect[4], R1connect[5]);
	printf("Room2: %d\t %d\t %d\t %d\t %d\t %d\n", R2connect[0], R2connect[1], R2connect[2], R2connect[3], R2connect[4], R2connect[5]);
	printf("Room3: %d\t %d\t %d\t %d\t %d\t %d\n", R3connect[0], R3connect[1], R3connect[2], R3connect[3], R3connect[4], R3connect[5]);
	printf("Room4: %d\t %d\t %d\t %d\t %d\t %d\n", R4connect[0], R4connect[1], R4connect[2], R4connect[3], R4connect[4], R4connect[5]);
	printf("Room5: %d\t %d\t %d\t %d\t %d\t %d\n", R5connect[0], R5connect[1], R5connect[2], R5connect[3], R5connect[4], R5connect[5]);
	printf("Room6: %d\t %d\t %d\t %d\t %d\t %d\n", R6connect[0], R6connect[1], R6connect[2], R6connect[3], R6connect[4], R6connect[5]);
*/


	 	


//select start and end and mid rooms
	int startRoom = rand() % 7;
	int endRoom = rand() % 7;
	while (startRoom == endRoom)
	{
		endRoom = rand() % 7;
	}

//	printf("StartRoom: %d\nEndRoom: %d\n", startRoom, endRoom);


//make directory

	int pid = getpid();
//	printf("PID: %d\n", pid);

	char strbuffer[50];
	sprintf (strbuffer, "yoppd.rooms.%d", pid);
//	printf("%s\n", strbuffer);

	int checkbool = mkdir(strbuffer, 0777);
	if(checkbool)
	{ printf("Directory failed!\n");
	}

//write room files

	int z = 0;
	while(z < 7)
	{
		char fileLoc[50]; 
		sprintf (fileLoc, "%s/%s_room", strbuffer, roomNames[selectedRooms[z]] );
	
//		printf("%s\n", fileLoc );
		FILE * currentFile;	
		currentFile = fopen(fileLoc, "w");

		fprintf(currentFile, "ROOM NAME: %s\n", roomNames[selectedRooms[z]] );  

		int y = 0;
		while (y < ConnectCount[z])
		{
			fprintf( currentFile, "CONNECTION %d: %s\n" , (y+1), roomNames[selectedRooms[ConnectRoom[z][y]]]);
			y++;
		}

		if (z == startRoom)
			{ fprintf( currentFile, "ROOM TYPE: START_ROOM\n"); }
		else if ( z == endRoom)
			{ fprintf( currentFile, "ROOM TYPE: END_ROOM\n");   }
		else
			{ fprintf( currentFile, "ROOM TYPE: MID_ROOM\n");   }

		fclose(currentFile);
		z++;
	}
return 0;
}		
