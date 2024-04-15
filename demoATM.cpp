#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 100
#define MAX_LOGIN_ATTEMPTS 5
#define MAX_TRANSACTIONS 10 // Gi?i h?n s? l?n giao d?ch
int start = 0;
char account_ad[] = "admin";
char password_ad[] = "0000";

char username[50];
char password[50]; 

typedef struct {
    char username[50];
    char password[50];
    float balance;
    int isLocked;
    int loginAttempts;
    int transactionCount; // S? l?n giao d?ch
} Account;

// Struct de luu tru thông tin cua moi giao dich
typedef struct {
    char account_number[50];
    char type[50];
    float amount;
} Transaction;

//// Bi?n toàn c?c lýu tr? danh sách các tài kho?n và giao d?ch
Account accounts[MAX_ACCOUNTS];
Transaction transactions[MAX_TRANSACTIONS];
int num_transactions = 0;
int numAccounts = 0;

void enterPassword(char *password);
int checkPassword(Account *accounts);
int check_password(Account *accounts);
void readAccountFile(Account *accounts, int *numAccounts);
void writeAccountFile(Account *accounts, int numAccounts);
void performTransaction(Account *accounts, int numAccounts);
void deposit(Account *accounts, int numAccounts);
void withdraw(Account *accounts, int numAccounts);
void checkBalance(Account *accounts, int numAccounts);
void showMenuAndProcessChoice(Account *accounts, int numAccounts);
void logTransaction(char *username, char *action, float amount);

void save_transactions(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int i;
    for (i = 0; i < num_transactions; i++) {
        fprintf(file, "%d %.2f\n", transactions[i].account_number, transactions[i].amount);
    }

    fclose(file);
}

int main() {
    Account accounts[MAX_ACCOUNTS];
	
    // Nh?p m?t kh?u t? nhân viên IT ð? kh?i ð?ng máy ATM
    printf("Nhap mat khau de khoi dong may\n");
	check_password(accounts);
    // Doc file
    readAccountFile(accounts, &numAccounts);

    while(start!=2){
    	showMenuAndProcessChoice(accounts, numAccounts);
	}
  
	save_transactions("transaction_history.dat");
    return 0;
}

void enterAccount(){
	printf("Enter the username: ");
    scanf("%s", &username);
}

void enterPassword() {
    printf("Enter the password: ");
    scanf("%s", &password);
}

//int checkPassword(char *password) {
//    // Trong th?c t?, b?n c?n ki?m tra m?t kh?u này v?i m?t kh?u ðý?c lýu tr? trong cõ s? d? li?u ho?c t?p tin.
//    // Trong ví d? này, m?t kh?u ðý?c xác ð?nh là "admin"
//    char correctPassword[] = "admin";
//    return strcmp(password, correctPassword) == 0;
//}

void readAccountFile(Account *accounts, int *numAccounts) {
    FILE *file = fopen("accounts.dat", "r");
    if (file == NULL) {
        printf("Cannot open file.\n");
        exit(1);
    }
    *numAccounts = 0;
    while (fscanf(file, "%s %s %f %d", accounts[*numAccounts].username, accounts[*numAccounts].password, &accounts[*numAccounts].balance, &accounts[*numAccounts].isLocked) != EOF) {
        (*numAccounts)++;
    }
    fclose(file);
}

void writeAccountFile(Account *accounts, int numAccounts) {
    FILE *file = fopen("accounts.dat", "w");
    if (file == NULL) {
        printf("Cannot open file.\n");
        exit(1);
    }
    for (int i = 0; i < numAccounts; i++) {
        fprintf(file, "%s %s %.2f %d\n", accounts[i].username, accounts[i].password, accounts[i].balance, accounts[i].isLocked);
    }
    fclose(file);
}

// Hàm de kiem tra mat khau
int check_password(Account *accounts) {
	enterAccount();
	enterPassword();
	if (strcmp(username,account_ad ) == 0 ){
		if (strcmp(password, password_ad) == 0){
			++start;
//			printf("\nnow %d\n",start);
			return 0;
		}
	}
	int i;
    for (i = 0; i < numAccounts; i++) {
    	if (strcmp(username,accounts[i].username ) == 0 )
            if(strcmp(accounts[i].password, password) == 0)
            	{
//            		printf("yeh");
	            	return 1;
	            	break;
				}
				else{
					("Sai ma PIN");
					return 0;
				}
        
    }
}

void performTransaction(Account *accounts, int numAccounts) {
    int choice;
    printf("\nTransaction Menu:\n");
    printf("1. Withdraw\n");
    printf("2. Deposit\n");
    printf("3. Check Balance\n");
    printf("4. Back to Main Menu\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch(choice) {
        case 1:
            withdraw(accounts, numAccounts);
            break;
        case 2:
            deposit(accounts, numAccounts);
            break;
        case 3:
            checkBalance(accounts, numAccounts);
            break;
        case 4:
            printf("Returning to Main Menu...\n");
            return;
        default:
            printf("Invalid choice. Please try again.\n");
    }
}

void deposit(Account *accounts, int numAccounts) {
    float amount;
    int loggedIn = 0;
    
    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(username, accounts[i].username) == 0) {
            if (accounts[i].isLocked) {
                printf("This account is locked. Please contact customer support.\n");
                return;
            }
            printf("Enter password: ");
            scanf("%s", password);

            if (strcmp(password, accounts[i].password) == 0) {
                loggedIn = 1;
                accounts[i].loginAttempts = 0; // Reset so lan dang nhap sai
                printf("Logged in successfully.\n");
                printf("Your balance: %.2f\n", accounts[i].balance);
                printf("Enter amount to deposit: ");
                scanf("%f", &amount);
                if (amount > 0) {
                    accounts[i].balance += amount;
                    printf("Deposit successful. New balance: %.2f\n", accounts[i].balance);
                    writeAccountFile(accounts, numAccounts);
                    // Ghi lich su giao dich
                    strcpy(transactions[num_transactions].account_number, accounts[i].username);
					strcpy(transactions[num_transactions].type, "Deposit");
				    transactions[num_transactions].amount = amount;
				    num_transactions++;	
                } else {
                    printf("Invalid amount.\n");
                }
                break;
            } else {
                printf("Incorrect password.\n");
                accounts[i].loginAttempts++;
                if (accounts[i].loginAttempts >= MAX_LOGIN_ATTEMPTS) {
                    accounts[i].isLocked = 1;
                    printf("Too many incorrect login attempts. This account is now locked.\n");
                    writeAccountFile(accounts, numAccounts);
                }
                return;
            }
        }
    }
    if (!loggedIn) {
        printf("Invalid username.\n");
    }
}

void withdraw(Account *accounts, int numAccounts) {
    float amount;
    int loggedIn = 0;


    printf("Enter password: ");
    scanf("%s", password);
    for (int i = 0; i < numAccounts; i++) {
            if (strcmp(password, accounts[i].password) == 0) {
                loggedIn = 1;
                accounts[i].loginAttempts = 0; // Reset s? l?n ðãng nh?p sai
                printf("Logged in successfully.\n");
                printf("Your balance: %.2f\n", accounts[i].balance);
                printf("Enter amount to withdraw: ");
                scanf("%f", &amount);
                if (amount > 0 && amount <= accounts[i].balance) {
                    accounts[i].balance -= amount;
                    printf("Withdrawal successful. New balance: %.2f\n", accounts[i].balance);
                    writeAccountFile(accounts, numAccounts);
                    // Ghi l?i l?ch s? giao d?ch
                    strcpy(transactions[num_transactions].account_number, accounts[i].username);
					strcpy(transactions[num_transactions].type, "Withdraw");
				    transactions[num_transactions].amount = amount;
				    num_transactions++;	
                } else {
                    printf("Invalid amount or insufficient balance.\n");
                }
                break;
            } else {
                printf("Incorrect password.\n");
                accounts[i].loginAttempts++;
                if (accounts[i].loginAttempts >= MAX_LOGIN_ATTEMPTS) {
                    accounts[i].isLocked = 1;
                    printf("Too many incorrect login attempts. This account is now locked.\n");
                    writeAccountFile(accounts, numAccounts);
                }
                return;
            
        	}
	}
//    if (!loggedIn) {
//        printf("Invalid username.\n");
//    }
}

void checkBalance(Account *accounts, int numAccounts) {
    int loggedIn = 0;

    for (int i = 0; i < numAccounts; i++) {
        
            printf("Enter password: ");
            scanf("%s", password);

            if (strcmp(password, accounts[i].password) == 0) {
                loggedIn = 1;
                accounts[i].loginAttempts = 0; // Reset s? l?n ðãng nh?p sai
                printf("Logged in successfully.\n");
                printf("Current balance for %s: %.2f\n", username, accounts[i].balance);
                strcpy(transactions[num_transactions].account_number, accounts[i].username);
					strcpy(transactions[num_transactions].type, "checkBlance");
				    transactions[num_transactions].amount = 0;
				    num_transactions++;	
                break;
            } else {
                printf("Incorrect password.\n");
                accounts[i].loginAttempts++;
                if (accounts[i].loginAttempts >= MAX_LOGIN_ATTEMPTS) {
                    accounts[i].isLocked = 1;
                    printf("Too many incorrect login attempts. This account is now locked.\n");
                    writeAccountFile(accounts, numAccounts);
                }
                return;
            }
        
    }
//    if (!loggedIn) {
//        printf("Invalid username.\n");
//    }
}

void showMenuAndProcessChoice(Account *accounts, int numAccounts) {
    int choice;
    int i=check_password(accounts);
    if(start==2) return;
    while(!i){
    	i=check_password(accounts);
    	printf("%d",i);
	}
    do {
        printf("\nATM Menu:\n");
        printf("1. Perform Transaction\n");
        printf("2. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                performTransaction(accounts, numAccounts);
                break;
            case 2:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 2);
}

void logTransaction(char *username, char *action, float amount) {
    FILE *file = fopen("transaction_log.dat", "a");
    if (file == NULL) {
        printf("Cannot open transaction log file.\n");
        return;
    }
    fprintf(file, "[%s] %s %.2f\n", username, action, amount);
    fclose(file);
}

