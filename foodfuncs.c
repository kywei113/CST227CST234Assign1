#include "foodfuncs.h"
#include <stdio.h>
#include <stdlib.h>

/*
	COSC 292 Assignment 1
	Michael Sabares - CST 227
	Kyle Wei		- CST 234
	
	foodfuncs.c
*/

/*
	This function will iterate through an array of Food records and print out each record's FCC, FHC, and FSN values
	Takes in an integer pointer to the start of an array, and the size of the array
*/
void PrintData(unsigned int* iFoodArrayPtr, int iSize)
{
	char* charPtr = NULL;	//char pointer to get single bytes, and move to locations in the record
	short* shortPtr = NULL;	//short pointer to get two bytes out of a record
	char fcc;					//variable to hold the FCC of a record
	int fhc = 0;				//variable to hold the FHC of a record
	unsigned short fsn = 0;		//Variable to hold the FSN of a record

	//Looping and incrementing up to the passed in size value
	for (int i = 0; i < iSize; i++)
	{
		charPtr = (char*)iFoodArrayPtr + (i * sizeof(int));		//Gets the first byte of a record. Adds on 4 bytes depending on which iteration we're on
		fcc = *charPtr;			//Assigns the value found at the first byte to fcc

		charPtr = charPtr + sizeof(char);		//Increment the char pointer by one character size (1)
		fhc = (int)(*charPtr);			//Assign the value found at the second byte to FHC as an integer

		shortPtr = (short*)(charPtr + sizeof(char));	//Increments the pointer by one character size, casts it to a short pointer to read two bytes
		fsn = (unsigned short) *shortPtr;		//Assigns the value of the next two bytes to FSN

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

void SetFHC(int iValue, int iRecordLoc, unsigned int* iFoodArrayPtr)
{
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

void SetFSN(short sValue, int iRecordLoc, unsigned int* iFoodArrayPtr)
{
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

void GetRecord(int iRecordLoc, char* FCC, int* FHC, unsigned short* FSN, unsigned int* iFoodArrayPtr)
{
	char* charPtr = NULL;
	unsigned short* shortPtr;

	charPtr = (char*)(iFoodArrayPtr + iRecordLoc);
	*FCC = *charPtr;

	charPtr = charPtr + sizeof(char);
	*FHC = (int) *charPtr;

	shortPtr = (unsigned short*)(charPtr + sizeof(char));
	*FSN = *shortPtr;
}

void PrintRecordData(char FCC, int FHC, unsigned short FSN)
{
	printf("FCC: %c FHC: %d FSN: %hu\n", FCC, FHC, FSN);
}

int* FindRecord(unsigned short FSN, unsigned int* iFoodArrayPtr, int iSize)
{
	int* recordAddress = NULL;

	if (FSN < 0 || FSN > 65535)
	{
		printf("Invalid FSN. Must be between 0 and 65535");
	}
	else
	{
		char* charPtr = NULL;
		short* shortPtr = NULL;
		int iFound = 0;

		for (int i = 0; i < iSize && iFound == 0; i++)
		{
			charPtr = (char*)(iFoodArrayPtr + i);
			shortPtr = (short*)(charPtr + sizeof(short));

			if (*shortPtr == FSN)
			{	
				iFound = 1;
				recordAddress = (int*)charPtr;
			}
		}

		recordAddress == NULL
			? printf("Could not find a record which corresponds with the given FSN: %hu\n", FSN)
			: printf("Found FSN %hu record at address: %p\n", FSN, recordAddress);

	}

	return recordAddress;
}

int SaveData(int* iFoodArrayPtr, int iSize, char* fileName)
{
	FILE* filePtr;
	int iErr = EXIT_SUCCESS;
	int retVal;
	int* recordPtr;

	if ((filePtr = fopen(fileName, "wb")) != NULL)
	{
		retVal = fwrite((void*) &iSize, sizeof(int), 1, filePtr);
		fflush(filePtr);

		for (int i = 0; i < iSize; i++)
		{
			retVal = fwrite((void*)&(iFoodArrayPtr[i]), sizeof(int), 1, filePtr);		//Flipping write order to allow for SSN reading before Name reading
			fflush(filePtr);	//Flush entries to file
		}

		if (retVal > 0)
		{
			printf("File Written: %s\n", fileName);
		}
		else
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
}

int* ReadData(char* fileName, int* iSizePtr)
{
	FILE* filePtr;
	int iErr = EXIT_SUCCESS;
	int retVal;
	unsigned int* intArrayPtr;


	if ((filePtr = fopen(fileName, "rb")) != NULL)
	{
		retVal = fread((void*)iSizePtr, sizeof(int), 1, filePtr);

		intArrayPtr = malloc(*iSizePtr * sizeof(int));

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