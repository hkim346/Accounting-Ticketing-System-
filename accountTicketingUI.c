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
#include <string.h>
#include "accountTicketingUI.h"
#include "account.h"
#include "commonHelpers.h"
#include "ticket.h"


void displayAccountSummaryHeader(void)
{
	printf("Acct# Acct.Type Birth\n");
	printf("----- --------- -----\n");
}

void displayAccountDetailHeader(void)
{
	printf("Acct# Acct.Type Birth Income      Country    Disp.Name       Login      Password\n");
	printf("----- --------- ----- ----------- ---------- --------------- ---------- --------\n");
}

void displayAccountSummaryRecord(const struct Account* account)
{
	if (account->AcctType == 'C')
		printf("%05d %-9s %5d\n", account->AcctNum, "CUSTOMER", account->DemoInfo.BirthYr);

	else if (account->AcctType == 'A')
		printf("%05d %-9s %5d\n", account->AcctNum, "AGENT", account->DemoInfo.BirthYr);
}

void displayAccountDetailRecord(const struct Account* account)
{
	int i;
	char securePW[100] = { '\0' };

	for (i = 0; account->LoginInfo.UserPW[i] != '\0'; i++)
	{
		if (i % 2 == 0)
			securePW[i] = account->LoginInfo.UserPW[i];
		else
			securePW[i] = '*';
	}

	if (account->AcctType == 'C')
		printf("%05d %-9s %5d $%10.2lf %-10s %-15s %-10s %8s\n", account->AcctNum, "CUSTOMER", account->DemoInfo.BirthYr, account->DemoInfo.Income, account->DemoInfo.contry, account->LoginInfo.UserName, account->LoginInfo.UserID, securePW);
	else if (account->AcctType == 'A')
		printf("%05d %-9s %5d $%10.2lf %-10s %-15s %-10s %8s\n", account->AcctNum, "AGENT", account->DemoInfo.BirthYr, account->DemoInfo.Income, account->DemoInfo.contry, account->LoginInfo.UserName, account->LoginInfo.UserID, securePW);
}



int loadAccounts(struct Account* accounts, int ACCOUNT_SIZE)
{
	int i = 0; // i has a meaning of the number of loaded accounts
	//char newline = 'x';

	FILE* fp = fopen("accounts.txt", "r");

	if (fp != NULL)
	{
		while(i <ACCOUNT_SIZE && fscanf(fp, "%d~%c~%30[^~]~%10[^~]~%8[^~]~%d~%lf~%[^\n]%*c", &accounts[i].AcctNum, &accounts[i].AcctType, accounts[i].LoginInfo.UserName, accounts[i].LoginInfo.UserID, accounts[i].LoginInfo.UserPW, &accounts[i].DemoInfo.BirthYr, &accounts[i].DemoInfo.Income, accounts[i].DemoInfo.contry)==8)
		{
			i++;
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



int loadTickets(struct Ticket* tickets, int TICKET_SIZE)
{
	// i has a meaning of the number of loaded tickets

	int i = 0, j = 0;
	char newline = 0;

	FILE* fp = fopen("tickets.txt", "r");

	if (fp != NULL)
	{
		
		while (fscanf(fp, "%d|%d|%d|%[^|]|%d|", &tickets[i].num_ticket, &tickets[i].num_account, &tickets[i].ticket_status, tickets[i].sub_line, &tickets[i].num_msg) == 5 && i < TICKET_SIZE)
		{

			for (j = 0; j < tickets[i].num_msg; j++)
			{
				fscanf(fp, "%c|%[^|]|%[^|]|%c", &tickets[i].message[j].acctType_Author, tickets[i].message[j].UserName, tickets[i].message[j].Msg_details, &newline);

				if (newline != '\n')
				{
					ungetc(newline, fp);
				}
			}
			i++;
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



void applicationStartup(struct AccountTicketingData* data)
{
	int Index = data->ACCOUNT_MAX_SIZE;


	while (Index != -1) // if -1 is returned, this program finishes
	{
		Index = menuLogin(data->accounts, data->ACCOUNT_MAX_SIZE);

		if (data->accounts[Index].AcctType == 'C')
		{
			menuClient(data->tickets, data->TICKET_MAX_SIZE, &data->accounts[Index]);
		}
		else if (data->accounts[Index].AcctType == 'A')
		{
			menuAgent(data->accounts, data->tickets, data->ACCOUNT_MAX_SIZE, data->TICKET_MAX_SIZE, &data->accounts[Index]);
		}
		else if (Index == data->ACCOUNT_MAX_SIZE)
		{
			printf("ERROR:  Login failed!\n\n");
			pauseExecution();
		}
	}

	printf("==============================================\n");
	printf("Account Ticketing System - Terminated\n");
	printf("==============================================\n");

}


int menuLogin(const struct Account* account, int Max_Account_SZ)
{
	int selection, foundIndex = Max_Account_SZ;
	char exit = 'x';


	printf("==============================================\n");
	printf("Account Ticketing System - Login\n");
	printf("==============================================\n");
	printf("1) Login to the system\n");
	printf("0) Exit application\n");
	printf("----------------------------------------------\n\n");
	printf("Selection: ");

	selection = getIntFromRange(0, 1);

	switch (selection)
	{
	case 1:
		printf("\n");
		foundIndex = validateLoginAccount(account, Max_Account_SZ);
		break;

	case 0:

		printf("\nAre you sure you want to exit? ([Y]es|[N]o): ");
		exit = getCharOption("yYnN");
		printf("\n");

		if (exit == 'Y' || exit == 'y')
			foundIndex = -1;				 //'foundIndex' gets -1 for finishing this program

		if (exit == 'N' || exit == 'n')
			foundIndex = Max_Account_SZ; //'foundIndex' gets invalid Index that going back to the system
		break;
	}

	return foundIndex;
}


void menuAgent(struct Account* accounts, struct Ticket* tickets, int Account_SZ, int Ticket_SZ, const struct Account* Logged_In_Account)
{

	int selection = 0;

	do
	{
		int TargetIndex = 0, displayeticket_sz = 0, temp = 0, i = 0, j = 0;
		char removal = 0;
		struct Ticket tempTicket[20] = { {0} };
		struct Account tempAccount[20] = { { 0 } };


		printf("AGENT: %s (%d)\n", Logged_In_Account->LoginInfo.UserName, Logged_In_Account->AcctNum);
		printf("==============================================\n");
		printf("Account Ticketing System - Agent Menu\n");
		printf("==============================================\n");
		printf(" 1) Add a new account\n");
		printf(" 2) Modify an existing account\n");
		printf(" 3) Remove an account\n");
		printf(" 4) List accounts: summary view\n");
		printf(" 5) List accounts: detailed view\n");
		printf("----------------------------------------------\n");
		printf(" 6) List new tickets\n");
		printf(" 7) List active tickets\n");
		printf(" 8) List closed tickets\n");
		printf(" 9) Manage a ticket\n");
		printf("10) Archive closed tickets\n");
		printf("----------------------------------------------\n");
		printf("11) View archived account statistics\n");
		printf("12) View archived tickets statistics\n");
		printf("----------------------------------------------\n");
		printf("0) Logout\n\n");
		printf("Selection: ");
		selection = getIntFromRange(0, 12);

		printf("\n");
		switch (selection)
		{

		case 1: //add new account
			TargetIndex = 0;
			

			for (i = 0; 0 < accounts[i].AcctNum && i < Account_SZ; i++) // checking empty account array
			{
				TargetIndex = i + 1;
			}

			if (TargetIndex == Account_SZ) // if account array is full, display error message
			{
				printf("ERROR: Account listing is FULL, call ITS Support!\n\n");
				pauseExecution();
			}

			else // if account aray is not full
			{
				for (i = 0; i < Account_SZ; i++)
				{
					if (temp <= accounts[i].AcctNum)
					{
						temp = accounts[i].AcctNum;
					}
					else
					{
						temp = temp;
					}
				}

				tempAccount[0] = accounts[14];

				accounts[TargetIndex].AcctNum = temp + 1;
				getAccount(&accounts[TargetIndex]);
				getUserLogin(&accounts[TargetIndex]);
				getDemographic(&accounts[TargetIndex]);
				accounts[14] = tempAccount[0];
				strcpy(accounts[13].LoginInfo.UserID, "HarryHorse");
				printf("\n*** New account added! ***\n\n");
				pauseExecution();
			}
			break;

		case 2: //modify an exist account

			TargetIndex = Account_SZ;
			
			printf("Enter the account#: ");
			temp= getPositiveInteger();

			for (i = 0; i < Account_SZ; i++)   //find target index of ticket
			{
				if (temp==accounts[i].AcctNum)
				{
					TargetIndex = i;
				}
				else
				{
					TargetIndex = TargetIndex;
				}
			}

			if (TargetIndex == Account_SZ)
			{
				printf("\nERROR:  Access Denied.\n\n");
			}
			else
			{
				printf("\n");
				updateAccount(&accounts[TargetIndex]);
				
			}
		
			break;

		case 3: // remove accounts
			TargetIndex = Account_SZ;
			TargetIndex = findAccountIndexByAcctNum(0, accounts, Account_SZ, 1); // find target account index

			if (TargetIndex == -1) // can't find account
			{
				printf("\nERROR:  Access Denied.\n\n");
				pauseExecution();
			}

			else if (Logged_In_Account->AcctNum == accounts[TargetIndex].AcctNum)
			{
				printf("\nERROR: You can't remove your own account!\n\n");
				pauseExecution();
			}

			else
			{
				displayAccountDetailHeader();
				displayAccountDetailRecord(&accounts[TargetIndex]);
				printf("\nAre you sure you want to remove this record? ([Y]es|[N]o): ");

				removal = getCharOption("YN");

				if (removal == 'Y')
				{
					for (i = 0; i < Ticket_SZ; i++)
					{
						// active ticket deletes
						if ((accounts[TargetIndex].AcctNum == tickets[i].num_account) && (tickets[i].ticket_status == 1)) 
						{
							tickets[i].num_ticket = 0;    // set to an empty state to free-up space for new

						}
						// closed tickets are stored to temp ticket array
						else if ((accounts[TargetIndex].AcctNum == tickets[i].num_account) && (tickets[i].ticket_status == 0) &&(0<tickets[i].num_ticket))
						{
							tempTicket[temp]=tickets[i]; 
							temp++;//copy 
							tickets[i].num_ticket = 0;    //an empty state to free-up space for new 
							
						}
					}
				
					saveTickets_ARC("tickets_arc.txt", tempTicket, temp);    //call a function for storing closed tickets to tickets_arc.txt

					saveAccount_ARC("accounts_arc.txt", &accounts[TargetIndex]); //call a fuctnion for stroing deleting account to account_arc
				
					accounts[TargetIndex].AcctNum = 0;       //set to empty state to free up sapce for new acount

					printf("\n*** Account Removed! ***\n\n");
					pauseExecution();
				}

				else if (removal == 'N')
				{
					printf("\n*** No changes made! ***\n\n");
					pauseExecution();
				}
			}
			break;

		case 4: //display account summary
			displayAllAccountSummaryRecords(accounts, Account_SZ);
			printf("\n");
			pauseExecution();
			break;

		case 5: //display accout details
			displayAllAccountDetailRecords(accounts, Account_SZ);
			printf("\n");
			pauseExecution();
			break;

		case 6: //display new tickets
			displayeticket_sz = 0;

			for (i = 0; i < Ticket_SZ; i++)
			{
				if (tickets[i].ticket_status == 1 && tickets[i].num_msg == 1 && tickets[i].num_ticket != 0)
				{
					tempTicket[displayeticket_sz] = tickets[i];
					displayeticket_sz++;
				}
			}

			displayTicketDetailRecord(tempTicket, displayeticket_sz);
			printf("\n");
			break;

		case 7: // list acive tickets
			displayeticket_sz = 0;

			for (i = 0; i < Ticket_SZ; i++)
			{
				if ((tickets[i].ticket_status == 1) && (tickets[i].num_ticket != 0))
				{
					tempTicket[displayeticket_sz] = tickets[i];
					displayeticket_sz++;
				}
			}

			displayTicketDetailRecord(tempTicket, displayeticket_sz);
			printf("\n");
			break;

		case 8: // list closed tickets
			displayeticket_sz = 0;

			for (i = 0; i < Ticket_SZ; i++)
			{
				if (tickets[i].ticket_status == 0 && tickets[i].num_ticket != 0)
				{
					tempTicket[displayeticket_sz] = tickets[i];
					displayeticket_sz++;
				}
			}

			displayTicketDetailRecord(tempTicket, displayeticket_sz);
			printf("\n");
			break;

		case 9: // ticket management
			TargetIndex = Ticket_SZ;

			printf("Enter ticket number: ");
			temp = getPositiveInteger();

			for (i = 0; i < Ticket_SZ; i++)
			{
				if (tickets[i].num_ticket == temp)
				{
					TargetIndex = i;
				}
				else
				{
					TargetIndex = TargetIndex;
				}
			}

			if (TargetIndex == Ticket_SZ) 
			{
				printf("ERROR:  Invalid Ticket Number!\n\n");
			}
			else
			{
				AgentTicketManage(&tickets[TargetIndex], Logged_In_Account);
			}
			break;


		case 10: //Archive closed tickets
			printf("Are you sure? This action cannot be reversed. ([Y]es|[N]o): ");
			removal = getCharOption("YN");

			if (removal == 'Y')
			{
				for (i = 0; i < Ticket_SZ; i++)
				{
					if (0 < tickets[i].num_ticket && tickets[i].ticket_status == 0)
					{
						tempTicket[temp] = tickets[i];
						temp++;
						tickets[i].num_ticket = 0; //set to empty state
					}
				}
				saveTickets_ARC("tickets_arc.txt", tempTicket, temp);
				printf("\n*** %d tickets archived ***\n\n", temp);
			}

			else if (removal == 'N')
			{
				printf("NO change\n");
			}
			pauseExecution();
			break;

		case 11: 
			i = readAccount_ARC("accounts_arc.txt");
			printf("There are %d account(s) currently archived.\n\n", i);
			pauseExecution();
			break;

		case 12:
			readTickets_ARC("tickets_arc.txt", &i, &j);
			printf("There are %d ticket(s) and a total of %d message(s) archived.\n\n", i, j);
			pauseExecution();
			break;

		case 0:
			printf("Saving session modifications...\n");
		

			for (i = 0; i < Account_SZ ; i++)
			{
				if (0 < accounts[i].AcctNum)
				{
					tempAccount[temp] = accounts[i];
					temp++;
				}
			}

			j= saveAccount("accounts.txt", tempAccount, temp);
			printf("   %d account saved.\n", j);

			j = 0;
			for (i = 0; i < Ticket_SZ; i++)
			{
				if (0 < tickets[i].num_ticket)
				{
					tempTicket[temp] = tickets[i];
					temp++;
				}
			}
			j = saveTickets("tickets.txt", tempTicket, temp);
			printf("   %d tickets saved.\n", j);
			printf("### LOGGED OUT ###\n\n");
			break;
		}
	} while (selection != 0);
	
	return;
}


void menuClient(struct Ticket* tickets, int Ticket_SZ, const struct Account* Logged_In_Account)
{
	int selection;

	do
	{
		int i = 0, TargetTicketIndex = Ticket_SZ, findTicketNum = 0, tempticket_sz = 0;
		struct Ticket tempTicket[50] = { {0} };

		printf("CUSTOMER: %s (%d)\n", Logged_In_Account->LoginInfo.UserName, Logged_In_Account->AcctNum);
		printf("==============================================\n");
		printf("Customer Main Menu\n");
		printf("==============================================\n");
		printf("1) View your account detail\n");
		printf("2) Create a new ticket\n");
		printf("3) Modify an active ticket\n");
		printf("4) List my tickets\n");
		printf("----------------------------------------------\n");
		printf("0) Logout\n\n");
		printf("Selection: ");
		selection = getIntFromRange(0, 4);

		printf("\n");
		switch (selection)
		{
		case 1:
			displayAccountDetailHeader();
			displayAccountDetailRecord(Logged_In_Account);
			printf("\n");
			pauseExecution();
			break;

		case 2: // adding new ticket

			//count current tickets quantities
			for (TargetTicketIndex = 0; tickets[TargetTicketIndex].num_ticket != 0 && TargetTicketIndex < Ticket_SZ; TargetTicketIndex++)
			{
			}

			if (TargetTicketIndex == Ticket_SZ) // if ticket array is full, display error message
			{
				printf("ERROR: Ticket listing is FULL, call ITS Support!\n\n");
			}

			else if (TargetTicketIndex < Ticket_SZ) // if account array is not full
			{
				for (i = 0; i < Ticket_SZ; i++) // get new ticket number : it will be biggest number 
				{
					if (findTicketNum <= tickets[i].num_ticket)
					{
						findTicketNum = tickets[i].num_ticket;
					}
				}
	
				tickets[TargetTicketIndex].num_ticket = findTicketNum + 1; // then, +1
				tickets[TargetTicketIndex].num_account = Logged_In_Account->AcctNum;
				tickets[TargetTicketIndex].ticket_status = 1;

				printf("New Ticket (Ticket#:%06d)\n", tickets[TargetTicketIndex].num_ticket);
				printf("----------------------------------------\n");
				getTicketSubLine(&tickets[TargetTicketIndex]);
				printf("\n");
				getMessageDetails(&tickets[TargetTicketIndex].message[0], Logged_In_Account);
				printf("\n*** New ticket created! ***\n\n");
				tickets[TargetTicketIndex].num_msg = 1;
			}
			pauseExecution();
			break;

		case 3:// update ticket

			printf("Enter ticket number: ");

			findTicketNum = getPositiveInteger();

			for (i = 0; i < Ticket_SZ; i++)   //find target index of ticket
			{
				if (tickets[i].num_ticket == findTicketNum)
				{
					TargetTicketIndex = i;
				}
				else
				{
					TargetTicketIndex = TargetTicketIndex;
				}
			}

			if (tickets[TargetTicketIndex].num_account == Logged_In_Account->AcctNum && tickets[TargetTicketIndex].ticket_status == 1)
			{
				UpdateTicket(&tickets[TargetTicketIndex], Logged_In_Account);
			}
			else if (tickets[TargetTicketIndex].num_account == Logged_In_Account->AcctNum && tickets[TargetTicketIndex].ticket_status == 0)
			{
				printf("\nERROR: Ticket is closed - changes are not permitted.\n\n");
			}
			else
			{
				printf("\nERROR: Invalid ticket number - you may only modify your own ticket.\n\n");
			}
			pauseExecution();
			break;

		case 4: //display ticket

			for (i = 0; i < Ticket_SZ; i++)
			{
				if (Logged_In_Account->AcctNum == tickets[i].num_account)
				{
					tempTicket[tempticket_sz] = tickets[i];
					tempticket_sz++;
				}
			}
			displayTicketRecord_Customer(tempTicket, tempticket_sz);
			
			printf("\n");
			break;

		case 0:
			printf("Saving session modifications...\n");
			i = saveTickets("tickets.txt", tickets, Ticket_SZ);
			printf("   %d tickets saved.\n", i);
			printf("### LOGGED OUT ###\n\n");
			break;
		}
	} while (selection != 0);

	return;
}


int findAccountIndexByAcctNum(int findAccountNum, const struct Account* account, int searchScopeIndex, int InputReq)
{
	int i;
	int getIndex = -1;
	/*
	
		for (i = 0; i < searchScopeIndex; i++)
		{
			printf(" %d ~ %c ~ %s ~ %s ~ %s ~ %d ~ %.2lf %s\n", account[i].AcctNum, account[i].AcctType, account[i].LoginInfo.UserName, account[i].LoginInfo.UserID, account[i].LoginInfo.UserPW, account[i].DemoInfo.BirthYr, account[i].DemoInfo.Income, account[i].DemoInfo.contry);
		}
		
		*/

	//	TargetIndex = findAccountIndexByAcctNum(0, accounts, Account_SZ, 1); // find target account index
	if (InputReq != 0)
	{
		printf("Enter the account#: ");
		findAccountNum = getPositiveInteger();
	}

	for (i = 0; i < searchScopeIndex; i++)
	{
		if (account[i].AcctNum == findAccountNum)
		{
			getIndex = i;
			InputReq = 0;
		}

		else
			getIndex = getIndex;
	}

	return getIndex;
}


void displayAllAccountSummaryRecords(const struct Account account[], int Array_SZ)
{
	int i = 0;


	displayAccountSummaryHeader();
	for (i = 0; i < Array_SZ; i++)
	{
		if (0 < account[i].AcctNum)
			displayAccountSummaryRecord(&account[i]);
	}
}


void displayAllAccountDetailRecords(const struct Account account[], int Array_SZ)
{
	int i = 0;

	displayAccountDetailHeader();

	for (i = 0; i < Array_SZ; i++)
	{
		if (0 < account[i].AcctNum)
			displayAccountDetailRecord(&account[i]);
	}
}


void pauseExecution(void)
{
	printf("<< ENTER key to Continue... >>");
	clearStandardInputBuffer();
	putchar('\n');
}


int validateLoginAccount(const struct Account* account, int Array_SZ)
{
	char TempID[100] = { '\0' };
	char TempPW[100] = { '\0' };
	int IDcheck = 0, PWcheck = 0, try = 3, TempAccount = 0, i = 0;
	int foundIndex = Array_SZ + 1;

	do
	{

		for (i = 0; i < 100; i++)
		{
			TempID[i] = '\0';
			TempPW[i] = '\0';
		}

		foundIndex = findAccountIndexByAcctNum(TempAccount, account, Array_SZ, 1);
		printf("User Login    : ");
		getCString(TempID, 1, 100);
		printf("Password      : ");
		getCString(TempPW, 1, 100);


		IDcheck = strcmp(TempID, account[foundIndex].LoginInfo.UserID);
		PWcheck = strcmp(TempPW, account[foundIndex].LoginInfo.UserPW);


		if (foundIndex == -1 || IDcheck != 0 || PWcheck != 0)
		{
			try--;
			printf("INVALID user login/password combination! [attempts remaining:%d]\n\n", try);
			if (try == 0)
				foundIndex = Array_SZ;
		}

		else if (foundIndex != -1 && IDcheck == 0 && PWcheck == 0)
		{
			try = 0;
			printf("\n");
		}
	} while (try > 0);

	return foundIndex;
}


void displayTicketDetailRecord(const struct Ticket* tickets, int Ticket_SZ)
{
	int i = 0, ticket_indenx = Ticket_SZ, temp_TicketNum = 0;

	do
	{
		ticket_indenx = Ticket_SZ;
		printf("------ ----- --------------- ------ ------------------------------ --------\n");
		printf("Ticket Acct# Display Name    Status Subject                        Messages\n");
		printf("------ ----- --------------- ------ ------------------------------ --------\n");

		for (i = 0; i < Ticket_SZ; i++)
		{
			printf("%06d %5d %-15s %-6s %-30s %5d\n", tickets[i].num_ticket, tickets[i].num_account, tickets[i].message->UserName, tickets[i].ticket_status == 1 ? "ACTIVE" : "CLOSED", tickets[i].sub_line, tickets[i].num_msg);
		}
		printf("------ ----- --------------- ------ ------------------------------ --------\n");


		printf("\nEnter the ticket number to view the messages or\n");
		printf("0 to return to previous menu: ");
		temp_TicketNum = getPositiveInteger();


		if (temp_TicketNum != 0)
		{
			for (i = 0; i < Ticket_SZ; i++)
			{
				if (temp_TicketNum == tickets[i].num_ticket)
					ticket_indenx = i;
				else
					ticket_indenx = ticket_indenx;
			}

			if (ticket_indenx == Ticket_SZ)
			{
				printf("\nERROR: Invalid ticket number.\n\n");
				pauseExecution();
			}

			else if (ticket_indenx >= 0 && ticket_indenx != Ticket_SZ)
			{
				printf("\n");
				displayMessageDetail(tickets[ticket_indenx]);
				pauseExecution();
			}
		}
	} while (temp_TicketNum != 0);

}


void displayMessageDetail(const struct Ticket tickets)
{
	int i = 0;

	printf("================================================================================\n");
	printf("%06d (%s) Re: %s\n", tickets.num_ticket, tickets.ticket_status == 1 ? "ACTIVE" : "CLOSED", tickets.sub_line);
	printf("================================================================================\n");

	for (i = 0; i < tickets.num_msg; i++)
	{
		printf("%s (%s):\n", tickets.message[i].acctType_Author == 'C' ? "CUSTOMER" : "AGENT", tickets.message[i].UserName);
		printf("   %s\n", tickets.message[i].Msg_details);
		printf("\n");

		if (tickets.num_msg > 5)
		{
			if (i % 5 == 4 && i % 19 != 0)
			{
				pauseExecution();
			}
			if (i % 5 == 4 && i % 19 == 0)
				break;
		}
	}
}

void displayTicketRecord_Customer(const struct Ticket* tickets, int Ticket_SZ)
{
	int i = 0, ticket_indenx = Ticket_SZ, temp_TicketNum = 0;

	do
	{
		ticket_indenx = Ticket_SZ;
		printf("------ ------ ------------------------------ --------\n");
		printf("Ticket Status Subject                        Messages\n");
		printf("------ ------ ------------------------------ --------\n");

		for (i = 0; i < Ticket_SZ; i++)
		{
			printf("%06d %-6s %-30s %5d\n", tickets[i].num_ticket, tickets[i].ticket_status == 1 ? "ACTIVE" : "CLOSED", tickets[i].sub_line, tickets[i].num_msg);
		}
		printf("------ ------ ------------------------------ --------\n");

		printf("\nEnter the ticket number to view the messages or\n");
		printf("0 to return to previous menu: ");
		temp_TicketNum = getPositiveInteger();


		if (temp_TicketNum != 0)
		{
			for (i = 0; i < Ticket_SZ; i++)
			{
				if (temp_TicketNum == tickets[i].num_ticket)
					ticket_indenx = i;
				else
					ticket_indenx = ticket_indenx;
			}

			if (ticket_indenx == Ticket_SZ)
			{
				printf("\nERROR: Invalid ticket number - you may only access your own tickets.\n\n");
				pauseExecution();
			}

			else if (ticket_indenx >= 0)
			{
				printf("\n");
				displayMessageDetail(tickets[ticket_indenx]);
				pauseExecution();
			}
		}
	} while (temp_TicketNum != 0);

}
