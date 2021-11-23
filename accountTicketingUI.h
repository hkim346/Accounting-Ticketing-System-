
/*
	==================================================
	o Assignment #2 Milestone #4
	==================================================
	Name   : Hyunjung Kim
	ID     : 150360196
	Email  : hkim346@myseneca.ca
	Section: NEE
*/

#ifndef ACCOUNT_TICKETING_UI_H_
#define ACCOUNT_TICKETING_UI_H_

#include "account.h"
#include "commonHelpers.h"
#include "ticket.h"

struct AccountTicketingData
{
	struct Account* accounts; // array of accounts
	const int ACCOUNT_MAX_SIZE; // maximum elements for account array
	struct Ticket* tickets; // array of tickets
	const int TICKET_MAX_SIZE; // maximum elements for ticket array
};


void displayAccountSummaryHeader(void);

void displayAccountDetailHeader(void);

void displayAccountSummaryRecord(const struct Account* account);

void displayAccountDetailRecord(const struct Account* account);

int loadAccounts(struct Account* accounts, int ACCOUNT_SIZE);

int loadTickets(struct Ticket* tickets, int TICKET_SIZE);

void applicationStartup(struct AccountTicketingData* data);

int menuLogin(const struct Account* account, int Account_SZ);

void menuAgent(struct Account* accounts, struct Ticket* tickets, int Acount_SZ, int Ticket_SZ, const struct Account* Logged_In_Account);

void menuClient(struct Ticket* tickets, int Ticket_SZ, const struct Account* Logged_In_Account);

int findAccountIndexByAcctNum(int findAccountNum, const struct Account* account, int searchScopeIndex, int InputReq);

void displayAllAccountSummaryRecords(const struct Account account[], int Array_SZ);

void displayAllAccountDetailRecords(const struct Account account[], int Array_SZ);

void pauseExecution(void);

int validateLoginAccount(const struct Account* account, int Array_SZ);

void displayTicketDetailRecord(const struct Ticket* tickets, int Ticket_SZ);

void displayMessageDetail(const struct Ticket tickets);

void displayTicketRecord_Customer(const struct Ticket* tickets, int Ticket_SZ);



#endif //!ACCOUNT_TICKETING_UI_H
