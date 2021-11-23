#pragma once
/*
	==================================================
	o Assignment #2 Milestone #4
	==================================================
	Name   : Hyunjung Kim
	ID     : 150360196
	Email  : hkim346@myseneca.ca
	Section: NEE
*/
#ifndef COMMON_HELPERS_H_
#define COMMON_HELPERS_H_


int currentYear(void);

void clearStandardInputBuffer(void);

int getInteger(void);

int getPositiveInteger(void);

double getDouble(void);

double getPositiveDouble(void);

int getIntFromRange(int MinNum, int MaxNum);

char getCharOption(const char ValidChar[]);

void getCString(char* Cstring, int min, int max);

void ConvertUpper(char* userCountry);


#endif  //!COMMON_HELPERS_H_
