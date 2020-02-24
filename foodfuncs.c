#include "foodfuncs.h"
#include <stdio.h>
#include <stdlib.h>

/*
	COSC 292 Assignment 1
	Author: Michael Sabares - CST 227
			Kyle Wei		- CST 234

	File Name: foodfuncs.c
	Date: Feb 24, 2020
*/

/*
	This function will iterate through an array of Food records and print out each record's FCC, FHC, and FSN values
	Takes in an integer pointer to the start of an array, and the size of the array
*/
void PrintData(unsigned int* iFoodArrayPtr, int iSize)
{
	char* charPtr = NULL;	//char pointer to get single bytes, and move to locations in the record
	short* shortPtr = NULL;	//short pointer to get two bytes out of a record
	char fcc = ' ';				//variable to hold the FCC of a record
	int fhc = 0;				//variable to hold the FHC of a record
	unsigned short fsn = 0;		//Variable to hold the FSN of a record

	//Looping and incrementing up to the passed in size value
	for (int i = 0; i < iSize; i++)
	{
		//GetRecord will get all of our values from the specified element in the array.
		GetRecord(i, &fcc, &fhc, &fsn, iFoodArrayPtr);

		PrintRecordData(fcc, fhc, fsn);			//Passes the found FCC, FHC, and FSN values to PrintRecordData to print out
	}
}

/*
	This function will update the FCC value of a given record in an array
	Takes in a character to update, index, and integer array
*/
void SetFCC(char cValue, int iRecordLoc, unsigned int* iFoodArrayPtr)
{
	 char* charPtr = (char*)(&(iFoodArrayPtr[iRecordLoc]));
	*charPtr = cValue;
}

/*
	This function will update the FHC value of the given record in an array
	Takes in a int to update, index, and integery array
*/
void SetFHC(int iValue, int iRecordLoc, unsigned int* iFoodArrayPtr)
{
	//Validation check to ensure the value being set is a valid postive int
	if (iValue > 255 || iValue < 0)
	{
		printf("Invalid integer. Must be between 0 and 255");
	}
	else
	{
		unsigned char* charPtr = (unsigned char*)(iFoodArrayPtr + iRecordLoc) + 1;
		*charPtr = (unsigned char) iValue;
	}

}

/*
	This function will update the FSN value of a given record in an array
	Takes in a character to update, index, and integer array
*/
void SetFSN(short sValue, int iRecordLoc, unsigned int* iFoodArrayPtr)
{
	//Validation check to ensure the value being set is a valid unsigned int
	if (sValue < 0 || sValue > 65535)
	{
		printf("Invalid FSN entered. Must be a value between 0 and 65535");
	}
	else
	{
		unsigned short* shortPtr = (unsigned short*)(iFoodArrayPtr + iRecordLoc) + 1;
		*shortPtr = sValue;
	}
}

/*
	This function will look into the specfied location of an elment in an array and get and assign the value of
	FCC, FHC and FSN from the that element.
*/
void GetRecord(int iRecordLoc, char* FCC, int* FHC, unsigned short* FSN, unsigned int* iFoodArrayPtr)
{
	char* charPtr = NULL; //char pointer to get single bytes, and move to locations in the record
	unsigned short* shortPtr;  //short pointer to get two bytes out of a record

	charPtr = (char*)(iFoodArrayPtr + iRecordLoc); //Gets the first byte of a record. Adds on 4 bytes depending on which iteration we're on
	*FCC = *charPtr; //Assigns the value found at the first byte to fcc

	charPtr = charPtr + sizeof(char); //Increment the char pointer by one character size (1)
	*FHC = (int) *charPtr; //Assign the value found at the second byte to FHC as an integer

	shortPtr = (unsigned short*)(charPtr + sizeof(char)); //Increments the pointer by one character size, casts it to a short pointer to read two bytes
	*FSN = *shortPtr; //Assigns the value of the next two bytes to FSN
}

/*
	This function will print our FCC, FHC, and FSN values into the console.
*/
void PrintRecordData(char FCC, int FHC, unsigned short FSN)
{
	printf("FCC: %c FHC: %d FSN: %hu\n", FCC, FHC, FSN);
}

/*
	This function will find a record base on the given FSN value and if found return it's address.
	Otherwise, return an error message.
*/
int* FindRecord(unsigned short FSN, unsigned int* iFoodArrayPtr, int iSize)
{
	int* recordAddress = NULL;

	//Check if the given FSN value is valid.
	if (FSN < 0 || FSN > 65535)
	{
		printf("Invalid FSN. Must be between 0 and 65535");
	}
	else
	{
		char* charPtr = NULL;
		short* shortPtr = NULL;

		int iFound = 0; //A flag that will be set to 1 if the record is found. Otherwise, 0 means not found.

		//Loop through each record.
		for (int i = 0; i < iSize && iFound == 0; i++)
		{
			//Move our pointer based on the current index.
			charPtr = (char*)(iFoodArrayPtr + i);

			//Get the FSN value of the current record.
			shortPtr = (short*)(charPtr + sizeof(short));

			//Check if the current FSN matches. If so, we exist a the loop and save the address to recordAddress
			if (*shortPtr == FSN)
			{	
				iFound = 1;
				recordAddress = (int*)charPtr;
			}
		}

		//Print our results on if recordAddress is still NULL or not.
		recordAddress == NULL
			? printf("Could not find a record which corresponds with the given FSN: %hu\n", FSN)
			: printf("Found FSN %hu record at address: %p\n", FSN, recordAddress);

	}

	return recordAddress;
}


/*
	This function will save the the array into a .bin file as binary.
*/
int SaveData(int* iFoodArrayPtr, int iSize, char* fileName)
{
	FILE* filePtr; //Pointer to a file stream
	int iErr = EXIT_SUCCESS; //Store our error code (0)
	int retVal; //Stores our status code when using fwrite method.

	
	//Create our file using fileName
	if ((filePtr = fopen(fileName, "wb")) != NULL)
	{
		//First write the size of our array.
		retVal = fwrite((void*) &iSize, sizeof(int), 1, filePtr);
		fflush(filePtr); //Flush entries to file

		for (int i = 0; i < iSize; i++)
		{
			//Writes the current element of the array to our file.
			retVal = fwrite((void*)&(iFoodArrayPtr[i]), sizeof(int), 1, filePtr);		//Flipping write order to allow for SSN reading before Name reading
			fflush(filePtr); //Flush entries to file
		}

		if (retVal > 0) //Prints a success message based on the last outcome written to retVal.
		{
			printf("File Written: %s\n", fileName);
		}
		else //Prints out failure messages.
		{
			if (iErr = ferror(filePtr))
			{
				printf("Error writing to the file %s: %s\n", fileName, strerror(iErr));
			}
			else
			{
				printf("Could not write the data\n");
			}
		}
		fclose(filePtr);
	}
	else
	{
		printf("Error accessing the file %s: %s", fileName, strerror(iErr));
	}

	//Returns our error code which the default value is 0 for success.
	return iErr;
}

/*
	This function will read a binary file, save the values to an allocated space and return that address.
*/
int* ReadData(char* fileName, int* iSizePtr)
{ 
	FILE* filePtr; //Pointer to a file stream
	int iErr = EXIT_SUCCESS; //Store our error code (0)
	int retVal; //Stores our status code when using fread method.
	unsigned int* intArrayPtr; //The value we'll assign our allocated space


	//Reads in the given file name.
	if ((filePtr = fopen(fileName, "rb")) != NULL)
	{
		//Reads the first int (4 bytes) and assign it to iSizePtr
		retVal = fread((void*)iSizePtr, sizeof(int), 1, filePtr);

		//We allocated memory for our address.
		intArrayPtr = malloc(*iSizePtr * sizeof(int));

		//We loop loop through our read the rest of the array and allocated that to intArrayPtr
		for(int i = 0; i < *iSizePtr; i++)
		{
			retVal = fread((void*)(intArrayPtr + i), sizeof(int), 1, filePtr);

			//printf("Name: %s \t SSN: %d\n", cBuffer, readSSN);	//Print out the record
		}

		fclose(filePtr);

		return intArrayPtr;
	}
	else
	{
		printf("Error accessing the file %s: %s", fileName, strerror(iErr));
	}
}