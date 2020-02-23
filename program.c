#include <stdio.h>
#include <stdlib.h>
#include "foodfuncs.h"

//Author:
//Date:

void TestGetRecord(int index, unsigned int* iFoodArrayPtr)
{
	char FCC = ' ';
	int FHC = -1;
	unsigned short FSN = -1;
	
	GetRecord(index, &FCC, &FHC, &FSN, iFoodArrayPtr);
	printf("FCC: %c FHC: %d FSN: %u\n", FCC, FHC, FSN);
}

void TestFindRecord(unsigned int* iFoodArrayPtr, int iSize)
{
	int* findPtr = FindRecord(30001, iFoodArrayPtr, iSize);

	findPtr = FindRecord(60000, iFoodArrayPtr, iSize);
}

void TestSaveData(unsigned int* iFoodArrayPtr, int iSize, char* fileName)
{

	SaveData(iFoodArrayPtr, iSize, fileName);

}

void TestReadData(unsigned int* iFoodArrayPtr, int iSize, char* fileName)
{
	PrintData(iFoodArrayPtr, iSize);

	iFoodArrayPtr = ReadData(fileName, &iSize);

	PrintData(iFoodArrayPtr, iSize);
}

void main(void)
{
	char fileName[] = "saveDataFile.bin";

	unsigned int iFoodArray[] = { 1310740313, 1966096717, 2621456717 };
	int iSize = sizeof(iFoodArray) / sizeof(unsigned int);
	//Calls to your functions

	//Write test code to write to file and read from file
	TestSaveData(iFoodArray, iSize, &fileName);

	//Print the data from each record
	printf("PrintData\n");
	PrintData(iFoodArray, iSize);

	//Change the Food Category Code in Record 2 to a Y
	printf("\nSetFCC\n");
	SetFCC('Y', 1, iFoodArray);
	PrintData(iFoodArray, iSize);
	//PrintData(iFoodArray, iSize);

	//Change the Food Handling Code in Record 2 to 79
	printf("\nSetFHC\n");
	SetFHC(79, 1, iFoodArray);
	PrintData(iFoodArray, iSize);

	//Change the Food Serial Number in Record 3 to 30001
	printf("\nSetFSN\n");
	SetFSN(30001, 2, iFoodArray);
	PrintData(iFoodArray, iSize);

	//Test code for Get Record
	printf("\nGetRecord\n");
	TestGetRecord(0, iFoodArray);
	TestGetRecord(1, iFoodArray);
	TestGetRecord(2, iFoodArray);

	//Print records to see if you get the expected results.
	printf("\nReprinting Records\n");
	PrintData(iFoodArray, iSize);

	//Write your own test case to search for a particular record.
	printf("\nFind Record\n");
	TestFindRecord(iFoodArray, iSize);

	//Reading in Food Data from a file
	printf("\nRead Record\n");
	TestReadData(iFoodArray, iSize, &fileName);
}


