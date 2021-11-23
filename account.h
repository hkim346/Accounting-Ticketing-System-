
/*
	==================================================
	o Assignment #2 Milestone #4
	==================================================
	Name   : Hyunjung Kim
	ID     : 150360196
	Email  : hkim346@myseneca.ca
	Section: NEE
*/

#ifndef ACCOUNT_H_
#define ACCOUNT_H_

#define _XOPEN_SOURCE 200
#define Max_ID 10
#define Max_PW 8
#define Max_Name 30
#define Max_contry 30
#define Max_Age 110
#define Min_Age 18

struct UserLogin
{
	char UserName[Max_Name + 1];
	char UserID[Max_ID + 1];
	char UserPW[Max_PW + 1];
};

struct Demographic
{
	int BirthYr;
	double Income;
	char contry[Max_contry + 1];
};


struct Account
{
	int AcctNum;
	char AcctType;
	struct UserLogin LoginInfo;
	struct Demographic DemoInfo;
};



void getAccount(struct Account* NewAccount);

void getUserLogin(struct Account* NewLogininfo);

void getDemographic(struct Account* NewDemoinfo);

void updateAccount(struct Account* update_Account);

void updateUserLogin(struct Account* update_Account);

void updateDemographic(struct Account* update_Account);

int ValidateUserID(char userID[]);

int ValidatePW(char userPW[]);

int saveAccount(const char* filename, struct Account* accounts, int Account_SZ);

void saveAccount_ARC(const char* filename, struct Account* account);

int readAccount_ARC(const char* filename);

#endif //!ACCOUNT_H_

