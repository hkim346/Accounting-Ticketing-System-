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
#include <ctype.h>
#include <string.h>

#include "accountTicketingUI.h"
#include "account.h"
#include "commonHelpers.h"
#include "ticket.h"




void getAccount(struct Account* NewAccount)
{

	printf("New Account Data (Account#:%d)\n", NewAccount->AcctNum);
	printf("----------------------------------------");
	printf("\nEnter the account type (A=Agent | C=Customer): ");
	NewAccount->AcctType = getCharOption("AC");
	printf("\n");
}

void getUserLogin(struct Account* NewLogininfo)
{
	int valid;
	
	printf("User Login Data Input\n");
	printf("----------------------------------------\n");

	valid = 0;
	do
	{
		printf("Enter user login (%d chars max): ", Max_ID);
		getCString(NewLogininfo->LoginInfo.UserID, 1, Max_ID + 1);
		valid = ValidateUserID(NewLogininfo->LoginInfo.UserID);

	} while (valid);
	
	printf("Enter the display name (%d chars max): ", Max_Name);
	getCString(NewLogininfo->LoginInfo.UserName, 1, Max_Name + 1);


	valid = 0;
	do
	{
		printf("Enter the password (must be %d chars in length): ", Max_PW);
		getCString(NewLogininfo->LoginInfo.UserPW, Max_PW + 1, Max_PW + 1);
		valid = ValidatePW(NewLogininfo->LoginInfo.UserPW);
	} while (!valid);

	printf("\n");
}

void getDemographic(struct Account* NewDemoinfo)
{
	
	
	
	printf("Demographic Data Input\n");
	printf("----------------------------------------\n");
	printf("Enter birth year (current age must be between %d and %d): ", Min_Age, Max_Age);
	NewDemoinfo->DemoInfo.BirthYr = getIntFromRange(currentYear() - Max_Age, currentYear() - Min_Age);

	printf("Enter the household Income: $");
	NewDemoinfo->DemoInfo.Income = getPositiveDouble();


	printf("Enter the country (%d chars max.): ", Max_contry);
	getCString(NewDemoinfo->DemoInfo.contry, 1, Max_contry + 1);
	ConvertUpper(NewDemoinfo->DemoInfo.contry);

}

void updateAccount(struct Account* update_Account)
{
	int selection = 0;

	do
	{
		printf("Account: %d - Update Options\n", update_Account->AcctNum);
		printf("----------------------------------------\n");
		printf("1) Update account type (current value: %c)\n", update_Account->AcctType);
		printf("2) Login\n");
		printf("3) Demographics\n");
		printf("0) Done\n");
		printf("Selection: ");
		selection = getIntFromRange(0, 3);

		switch (selection)
		{
		case 1:
			printf("\nEnter the account type (A=Agent | C=Customer): ");
			update_Account->AcctType = getCharOption("AC");
			break;

		case 2:
			printf("\n");
			updateUserLogin(update_Account);
			printf("\n");
			break;

		case 3:
			printf("\n");
			updateDemographic(update_Account);
			printf("\n");
			break;

		case 0:
			printf("\n");
			break;
		}
	} while (selection != 0);
}

void updateUserLogin(struct Account* update_Account)
{
	int selection, valid;

	do
	{
		printf("User Login: %s - Update Options\n", update_Account->LoginInfo.UserID);
		printf("----------------------------------------\n");
		printf("1) Display name (current value: %s)\n", update_Account->LoginInfo.UserName);
		printf("2) Password\n");
		printf("0) Done\n");
		printf("Selection: ");

		selection = getIntFromRange(0, 2);

		switch (selection)
		{
		case 1:
			printf("\nEnter the display name (%d chars max): ", Max_Name);
			getCString(update_Account->LoginInfo.UserName, 1, Max_Name + 1);
			printf("\n");
			break;

		case 2:
			valid = 0;
			printf("\n");
			do
			{
				printf("Enter the password (must be %d chars in length): ", Max_PW);
				getCString(update_Account->LoginInfo.UserPW, 1, Max_PW + 1);
				valid = ValidatePW(update_Account->LoginInfo.UserPW);
			} while (!valid);
			printf("\n");
			break;

		case 0:
			break;
		}
	} while (selection != 0);
}

void updateDemographic(struct Account* update_Account)
{
	int selection;

	do
	{
		printf("Demographic Update Options\n");
		printf("----------------------------------------\n");
		printf("1) Household Income (current value: $%.2lf)\n", update_Account->DemoInfo.Income);
		printf("2) Country (current value: %s)\n", update_Account->DemoInfo.contry);
		printf("0) Done\n");
		printf("Selection: ");

		selection = getIntFromRange(0, 2);

		switch (selection)
		{
		case 1:
			printf("\nEnter the household Income: $");
			update_Account->DemoInfo.Income = getPositiveDouble();
			printf("\n");
			break;

		case 2:
			printf("\nEnter the country (%d chars max.): ", Max_contry);
			getCString(update_Account->DemoInfo.contry, 1, Max_contry + 1);
			ConvertUpper(update_Account->DemoInfo.contry);
			printf("\n");
			break;

		case 0:
			break;
		}
	} while (selection != 0);
}

int ValidateUserID(char userID[])
{
	int whitespace = 0;
	int i;


	for (i = 0; userID[i] != '\0' && !whitespace; i++)
	{
		if (isblank(userID[i]))
		{
			whitespace = 1;
			printf("ERROR:  The user login must NOT contain whitespace characters.\n");
		}
		else
			whitespace = whitespace;
	}
	return whitespace;
}

int ValidatePW(char userPW[])
{
	int UpperCounter = 0, LowerCounter = 0, SymbolCounter = 0, NumberCounter = 0;
	int i, valid = 1;

	for (i = 0; userPW[i] != '\0'; i++)
	{
		if (isdigit(userPW[i]))
			NumberCounter++;
		else if (islower(userPW[i]))
			LowerCounter++;
		else if (isupper(userPW[i]))
			UpperCounter++;
		else if ((userPW[i] == 33) || (userPW[i] == 64) || (userPW[i] == 35) || (userPW[i] == 36) || (userPW[i] == 37) || (userPW[i] == 94) || (userPW[i] == 38) || (userPW[i] == 42))
			SymbolCounter++;
	}

	if (NumberCounter == 2 && LowerCounter == 2 && UpperCounter == 2 && SymbolCounter == 2)
		valid = 1;

	else
	{
		printf("SECURITY: Password must contain 2 of each:\n");
		printf("          Digit: 0-9\n");
		printf("          UPPERCASE character\n");
		printf("          lowercase character\n");
		printf("          symbol character: !@#$%%^&*\n");
		valid = 0;
	}

	return valid;
}


int saveAccount(const char* filename, struct Account* accounts, int Account_SZ)
{
	int i = 0;
	FILE* fp = fopen(filename, "w");

	if (fp != NULL)
	{
		for (i = 0; i < Account_SZ; i++)
		{
			fprintf(fp, "%d~%c~%s~%s~%s~%d~%.2lf~%s\n", accounts[i].AcctNum, accounts[i].AcctType, accounts[i].LoginInfo.UserName, accounts[i].LoginInfo.UserID, accounts[i].LoginInfo.UserPW, accounts[i].DemoInfo.BirthYr, accounts[i].DemoInfo.Income, accounts[i].DemoInfo.contry);
		}
		fclose(fp);
		fp = NULL;
	}
	
	else
	{
		printf("unable to open the file\n");
	}

	return i;
}


void saveAccount_ARC(const char* filename, struct Account* account)
{

	FILE* fp = fopen(filename , "a");

		if (fp != NULL)
		{
			fprintf(fp, "%d~%c~%s~%s~%s~%d~%.2lf~%s\n", account->AcctNum, account->AcctType, account->LoginInfo.UserName, account->LoginInfo.UserID, account->LoginInfo.UserPW, account->DemoInfo.BirthYr, account->DemoInfo.Income, account->DemoInfo.contry);

			fclose(fp);
			fp = NULL;
		}
		else
		{
			printf("unable to open the file\n");
		}

}


int readAccount_ARC(const char* filename)
{
	int i = 0, numAccounts = 0;
	struct Account accounts[30] = { { 0 } };

	FILE* fp = fopen(filename, "r");

	if (fp != NULL)
	{
		for (i = 0; fscanf(fp, "%d~%c~%30[^~]~%10[^~]~%8[^~]~%d~%lf~%[^\n]%*c", &accounts[i].AcctNum, &accounts[i].AcctType, accounts[i].LoginInfo.UserName, accounts[i].LoginInfo.UserID, accounts[i].LoginInfo.UserPW, &accounts[i].DemoInfo.BirthYr, &accounts[i].DemoInfo.Income, accounts[i].DemoInfo.contry) == 8; i++)
		{
			numAccounts++;
		}
		fclose(fp);
		fp = NULL;
	}
	else
	{
		printf("unable to open the file\n");
	}

	return numAccounts;
}

