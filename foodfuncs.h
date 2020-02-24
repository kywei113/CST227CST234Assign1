#ifndef FOODFUNCS_H
#define FOODFUNCS_H

#endif

/*
	COSC 292 Assignment 1
	Author: Michael Sabares - CST 227
			Kyle Wei		- CST 234

	File Name: foodfuncs.h
	Date: Feb 24, 2020
*/
void PrintData(unsigned int* iFoodArrayPtr, int iSize);
void SetFCC(char cValue, int iRecordLoc, unsigned int* iFoodArrayPtr);
void SetFHC(int iValue, int iRecordLoc, unsigned int* iFoodArrayPtr);
void SetFSN(short sValue, int iRecordLoc, unsigned int* iFoodArrayPtr);
void GetRecord(int iRecordLoc, char* FCC, int* FHC, unsigned short* FSN, unsigned int* iFoodArrayPtr);
void PrintRecordData(char FCC, int FHC, unsigned short FSN);
int* FindRecord(unsigned short FSN, unsigned int* iFoodArrayPtr, int iSize);
int SaveData(int* iFoodArrayPtr, int iSize, char* fileName);
int* ReadData(char* fileName, int* iSizePtr);
