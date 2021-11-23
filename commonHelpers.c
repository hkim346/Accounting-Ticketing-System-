/*
	==================================================
	o Assignment #2 Milestone #4
	==================================================
	Name   : Hyunjung Kim
	ID     : 150360196
	Email  : hkim346@myseneca.ca
	Section: NEE
*/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include "commonHelpers.h"


// Uses the time.h library to obtain current year information
// Get the current 4-digit year from the system
int currentYear(void)
{
	time_t currentTime = time(NULL);
	return localtime(&currentTime)->tm_year + 1900;
}

void clearStandardInputBuffer(void)
{
	while (getchar() != '\n')
	{
		; // On purpose: do nothing
	}
}

int getInteger(void)
{
	int IntNum, valid = 1;
	char NewLine = 'x';

	while (valid)
	{
		scanf("%d%c", &IntNum, &NewLine);

		if (NewLine == '\n')
		{
			valid = 0;
		}

		else
		{
			clearStandardInputBuffer();
			printf("ERROR: Value must be an integer: ");
		}
	}
	return IntNum;
}

int getPositiveInteger(void)
{
	int IntPosNum, valid = 1;
	char NewLine = 'x';

	while (valid)
	{
		scanf("%d%c", &IntPosNum, &NewLine);

		if (NewLine == '\n')
		{
			if (IntPosNum < 0)
				printf("ERROR: Value must be positive or zero: ");
			else
				valid = 0;
		}

		else
		{
			clearStandardInputBuffer();
			printf("ERROR: Value must be an integer: ");
		}
	}
	return IntPosNum;
}

double getDouble(void)
{
	double DouNum;
	int valid = 1;
	char NewLine = 'x';

	while (valid)
	{
		scanf("%lf%c", &DouNum, &NewLine);

		if (NewLine == '\n')
		{
			valid = 0;
		}

		else
		{
			clearStandardInputBuffer();
			printf("ERROR: Value must be a double floating-point number: ");
		}
	}
	return DouNum;

}

double getPositiveDouble(void)
{
	double DouPosNum = 0, valid = 1;
	char NewLine = 'x';

	while (valid)
	{
		scanf("%lf%c", &DouPosNum, &NewLine);

		if (NewLine == '\n')
		{
			if (DouPosNum <= 0)
				printf("ERROR: Value must be a positive double floating-point number: ");

			else
				valid = 0;
		}

		else
		{
			clearStandardInputBuffer();
			printf("ERROR: Value must be a double floating-point number: ");
		}
	}
	return DouPosNum;
}

int getIntFromRange(int MinNum, int MaxNum)
{
	int IntNum = 0, valid = 1;
	char NewLine;

	while (valid)
	{
		NewLine = 'x';
		scanf("%d%c", &IntNum, &NewLine);

		if (NewLine != '\n')
		{
			clearStandardInputBuffer();
			printf("ERROR: Value must be an integer: ");
		}
		else if (NewLine == '\n')
		{
			if (IntNum >= MinNum && IntNum <= MaxNum)
				valid = 0;
			else
				printf("ERROR: Value must be between %d and %d inclusive: ", MinNum, MaxNum);
		}
	}
	return IntNum;
}

char getCharOption(const char ValidChar[])
{
	int i = 0, v1 = 1, v2 = 1;
	char a, newline = 'x';


	while (v1)
	{
		scanf("%c%c", &a, &newline);

		if (newline == '\n')
		{
			for (i = 0; ValidChar[i] != '\0'; i++)
			{
				if (ValidChar[i] == a)
					v2 = 0;
			}

			if (v2 == 0)
				v1 = 0;

			else
				printf("ERROR: Character must be one of [%s]: ", ValidChar);
		}
		else
		{
			clearStandardInputBuffer();
			printf("ERROR: Character must be one of [%s]: ", ValidChar);
		}
	}
	return a;
}

void getCString(char* Cstring, int min, int max)
{
	int i = 0, Valid = 1;
	char newline = 'x';

	while (Valid)
	{
		scanf("%[^\n]%c", Cstring, &newline);

		if (newline == '\n')
		{
			i = strlen(Cstring);

			if (min == max && !(i >= min - 1 && i <= max - 1))
				printf("ERROR: String length must be exactly %d chars: ", min - 1);

			else if (min - 1 != max - 1 && i < min - 1)
				printf("ERROR: String length must be between %d and %d chars: ", min - 1, max - 1);

			else if (min - 1 != max - 1 && i > max - 1)
				printf("ERROR: String length must be no more than %d chars: ", max - 1);
			else
				Valid = 0;
		}
		else
		{
			if (min == max)
				printf("ERROR: String length must be exactly %d chars: ", min - 1);

			else
				printf("ERROR: String length must be between %d and %d chars: ", min - 1, max - 1);
		}
	}
}

void ConvertUpper(char* userCountry)
{
	int i = 0;

	for (i = 0; userCountry[i] != '\0'; i++)
	{
		userCountry[i] = toupper(userCountry[i]);
	}

}
