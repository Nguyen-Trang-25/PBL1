#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
	

#ifdef _WIN32
#include <windows.h> // Ðoi voi Windows
#else
#include <unistd.h> // Ðoi voi Unix/Linux
#endif

#define MAX_ACCOUNTS 100
#define MAX_LOGIN_ATTEMPTS 5
#define MAX_TRANSACTIONS 10 // Gi?i h?n s? l?n giao d?ch
#define FILENAME_SIZE 150 

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

void title();
void main_bgr();
void waitForClear();
void enterPassword(char *password);
int checkPassword_admin();
int check_password(Account *accounts);
void readAccountFile(Account *accounts, int *numAccounts);
void writeAccountFile(Account *accounts, int numAccounts);
void performTransaction(Account *accounts, int numAccounts,int *reloadUser);
void deposit(Account *accounts, int numAccounts,int *reloadUser);
void withdraw(Account *accounts, int numAccounts,int *reloadUser);
void checkBalance(Account *accounts, int numAccounts,int *reloadUser);
void showMenuAndProcessChoice(Account *accounts, int numAccounts);
void save_transactions();

int main() {
    Account accounts[MAX_ACCOUNTS];
	
	title();
    // Nh?p m?t kh?u t? nhân viên IT ð? kh?i ð?ng máy ATM
	checkPassword_admin();
    // Doc file
    readAccountFile(accounts, &numAccounts);
//    	printf("%d",start);

    while(start!=2){
    	showMenuAndProcessChoice(accounts, numAccounts);
	}
  
	save_transactions();
    return 0;
}



//Giao dien ban dau
void title() {
    printf(" +----------------------------------------------------------------------------------------------------------+\n");
    printf(" '                                             DE TAI DO AN PBL1                                            '\n");
    printf(" '                          DE TAI 701: XAY DUNG UNG DUNG THUC HIEN GIAO DICH TREN ATM                      '\n");
	printf(" '                                                                                                          '\n");
    printf(" '                                  Giang Vien Huong Dan:   Tran Ho Thuy Tien                               '\n");
    printf(" '                                  Sinh Vien Thuc Hien :   Huynh Thao Nhi                                  '\n");
    printf(" '                                                          Nguyen Thi Trang                        	    '\n");
    printf(" +----------------------------------------------------------------------------------------------------------+\n");
    printf("\n");
    printf(" *----------------------------------------------------------------------------------------------------------*\n");
    printf(" |                                                                                                          |\n");
    printf(" '                                          WELCOME TO OUR ATM                                              '\n");
    printf(" |                                                                                                          |\n");
    printf(" *----------------------------------------------------------------------------------------------------------*\n");
    printf("\n\n");  
}

void main_bgr(){
	printf(" *----------------------------------------------------------------------------------------------------------*\n");
    printf(" |                                                                                                          |\n");
    printf(" '                                          WELCOME TO OUR ATM                                              '\n");
    printf(" |                                                                                                          |\n");
    printf(" *----------------------------------------------------------------------------------------------------------*\n");
    printf("\n\n"); 
}

//Ham cho xoa man hinh
void waitForClear(){
    #ifdef _WIN32
	Sleep(1500); // Trên Windows, th?i gian du?c tính b?ng mili-giây (ms)
	#else
	sleep(1.5); // Trên Unix/Linux, th?i gian du?c tính b?ng giây
	#endif
    system("cls");
}

void enterAccount(){
	printf("Enter the username: ");
    scanf("%s", &username);
}

void enterPassword() {
    printf("Enter the password: ");
    scanf("%s", &password);
}



int checkPassword_admin() {
    //Ham xac nhan admin
    printf(" *----------------------------------------------------------------------------------------------------------*\n");
	printf(" |                                                                                                          |\n");
    printf(" '                                    NHAP MAT KHAU DE KHOI DONG MAY                                        '\n");
    printf(" '                                    ENTER IT PASSWORD TO START ATM                                        '\n");
    printf(" |                                                                                                          |\n");
    printf(" *----------------------------------------------------------------------------------------------------------*\n");
    printf("             _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _           \n\n");
    printf("                                              Password:");
	scanf("%s", &password);
    
    while(strcmp(password, password_ad)){
    	system("cls");

		printf("                                    *- - - - - - - - - - - -  - - - - - - - - - - -* \n");
		printf("                                    |                                              |\n");
	    printf("                                    *  MAT KHAU NHAP VAO SAI!!!Vui long nhap lai.  *\n");
	    printf("                                    *  INCORRECT PASSWORD!!! Please re-enter       *\n");
	    printf("                                    |                                              |\n");
    	printf("                                    *- - - - - - - - - - - -  - - - - - - - - - - -* \n");

    	waitForClear();
		enterPassword();
	}
	++start;
	system("cls");
		printf("                                    *- - - - - - - - - - - -  - - - - - - - - - - -* \n");
		printf("                                    |                                              |\n");
	    printf("                                    *  MAT KHAU NHAP VAO DUNG!!! Dang khoi dong... *\n");
	    printf("                                    *       CORRECT PASSWORD!!! Starting...        *\n");
	    printf("                                    |                                              |\n");
    	printf("                                    *- - - - - - - - - - - -  - - - - - - - - - - -* \n");
//	waitForClear();  
}

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
	main_bgr();
	if (strcmp(username, password_ad) == 0){
		++start;
		return 0;
	}
//	enterPassword();
	int i;
    for (i = 0; i < numAccounts; i++) {
    	if (strcmp(username,accounts[i].username ) == 0 )
            {
            	return 1;
//            	if(strcmp(accounts[i].password, password) == 0)
//            	{
//            		printf("yeh");
//	            	return 1;
//				}
//				else{
////					"Sai ma PIN!!!\nMoi nhap lai\n";
//					return 0;
//				}
			}
        
    }
//    printf("Sai so tai khoan!!!\nMoi nhap lai\n");
    return 0;
}

//Ham show menu
void showMenuAndProcessChoice(Account *accounts, int numAccounts) {
	int *reloadUser;
	*reloadUser = 1; 
    int choice;
    
    enterAccount();
    int i=check_password(accounts);
    
    if(start==2) return;
    while(!i){
//    	system("cls");
    	printf("THONG TIN TAI KHOAN SAI!!!\nVui long nhap lai");
    	waitForClear();
    	i=check_password(accounts);
    	if(start==2) return;
	}
	
    do {
        printf("\nATM Menu:\n");
        printf("1. Perform Transaction\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
//            	system("cls");
                performTransaction(accounts, numAccounts,reloadUser);
                break;
            case 0:
//                system("cls");
                printf("Exiting...\n");
                waitForClear();
                void waitForClear();
                break;
            default:
                printf("\n\nInvalid choice. Please try again.\n\n");
        }
    } while(choice != 0);
}

void performTransaction(Account *accounts, int numAccounts,int *reloadUser) {
    int choice;
    printf("\n***                ---             Transaction Menu            ---               ***\n\n");
    printf("1. Withdraw\n");
    printf("2. Deposit\n");
    printf("3. Check Balance\n");
    printf("0. Back to Main Menu\n");
    
    printf("\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch(choice) {
        case 1:
//        	system("cls");
            withdraw(accounts, numAccounts,reloadUser);
            break;
        case 2:
//        	system("cls");
            deposit(accounts, numAccounts,reloadUser);
            break;
        case 3:
//        	system("cls");
            checkBalance(accounts, numAccounts, reloadUser);
            break;
        case 0:
//        	system("cls");
            printf("Returning to Main Menu...\n");
            waitForClear();
            return;
        default:
            printf("Invalid choice. Please try again.\n");
    }
}

void deposit(Account *accounts, int numAccounts, int *reloadUser) {
    float amount;

//    for (int i = 0; i < numAccounts; i++) {
//    if (strcmp(username, accounts[i].username) == 0) {
//        if (accounts[i].isLocked) {
//            printf("This account is locked. Please contact customer support.\n");
//            return;
//        }
//        
//        int attempt = 0; // Bi?n ð?m s? l?n nh?p m?t kh?u sai
//        while (attempt < MAX_LOGIN_ATTEMPTS) {
//            printf("Enter password: ");
//            scanf("%s", password);
//
//            // Ki?m tra m?t kh?u
//            if (strcmp(password, accounts[i].password) == 0) {
//                // N?u m?t kh?u ðúng, ðãng nh?p thành công và th?c hi?n giao d?ch
//                printf("Logged in successfully.\n");
//                printf("Your balance: %.2f\n", accounts[i].balance);
//                printf("Enter amount to deposit: ");
//                scanf("%f", &amount);
//                
//                if (amount > 0) {
//                    accounts[i].balance += amount;
//                    printf("Deposit successful. New balance: %.2f\n", accounts[i].balance);
//                    writeAccountFile(accounts, numAccounts);
//                    // Ghi l?ch s? giao d?ch
//                    strcpy(transactions[num_transactions].account_number, accounts[i].username);
//                    strcpy(transactions[num_transactions].type, "Deposit");
//                    transactions[num_transactions].amount = amount;
//                    num_transactions++; 
//                } else {
//                    printf("Invalid amount.\n");
//                }
//                return; // Thoát kh?i hàm sau khi th?c hi?n giao d?ch
//            } else {
//                // N?u m?t kh?u không ðúng, tãng s? l?n nh?p sai và hi?n th? thông báo
//                printf("Incorrect password.\n");
//                attempt++;
//                printf("Attempts remaining: %d\n", MAX_LOGIN_ATTEMPTS - attempt);
//            }
//        }
//
//        // N?u s? l?n nh?p sai vý?t quá MAX_LOGIN_ATTEMPTS, khoá tài kho?n
//        accounts[i].isLocked = 1;
//        printf("Too many incorrect login attempts. This account is now locked.\n");
//        writeAccountFile(accounts, numAccounts);
//        return;
//    }
//}

    
    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(username, accounts[i].username) == 0) {
            if (accounts[i].isLocked) {
                printf("This account is locked. Please contact customer support.\n");
                return;
            }
            if(*reloadUser){
            	printf("Enter password: ");
            	scanf("%s", password);
			}
			while(strcmp(password, accounts[i].password)){
				printf("Incorrect password.\n");
                accounts[i].loginAttempts++;
                if (accounts[i].loginAttempts >= MAX_LOGIN_ATTEMPTS) {
                    accounts[i].isLocked = 1;
                    printf("Too many incorrect login attempts. This account is now locked.\n");
                    writeAccountFile(accounts, numAccounts);
                	waitForClear();
                	return;
                }
                waitForClear();
                printf("Nhap lai\n");
                scanf("%s", password);
			}
			*reloadUser=0;	
            accounts[i].loginAttempts = 0; // Reset so lan dang nhap sai
            printf("Logged in successfully.\n");
            printf("Your balance: %.2f\n", accounts[i].balance);
            printf("Enter amount to deposit: ");
            scanf("%f", &amount);
            if (amount > 0) {
                accounts[i].balance += amount;
                printf("Deposit successful. New balance: %.2f\n", accounts[i].balance);
                writeAccountFile(accounts, numAccounts);
                
                printf("Nhan Enter de ket thuc giao dich");
    			while (getchar() != '\n');
    			getchar();
//    			system("cls");
    			
                // Ghi lich su giao dich
                strcpy(transactions[num_transactions].account_number, accounts[i].username);
				strcpy(transactions[num_transactions].type, "Deposit");
				transactions[num_transactions].amount = amount;
				num_transactions++;	
            } else {
                printf("Invalid amount.\n");
            }
            break;			
        }
    }
}

void withdraw(Account *accounts, int numAccounts, int *reloadUser) {
    float amount;
    int loggedIn = 0;

	for (int i = 0; i < numAccounts; i++) {
        if (strcmp(username, accounts[i].username) == 0) {
            if (accounts[i].isLocked) {
                printf("This account is locked. Please contact customer support.\n");
                return;
            }
            if(*reloadUser){
            	printf("Enter password: ");
            	scanf("%s", password);
			}

			while(strcmp(password, accounts[i].password)){
				printf("Incorrect password.\n");
                accounts[i].loginAttempts++;
                if (accounts[i].loginAttempts >= MAX_LOGIN_ATTEMPTS) {
                    accounts[i].isLocked = 1;
                    printf("Too many incorrect login attempts. This account is now locked.\n");
                    writeAccountFile(accounts, numAccounts);
                	waitForClear();
                	return;
                }
                waitForClear();
                printf("Nhap lai\n");
                scanf("%s", password);
			}
            loggedIn = 1;
            *reloadUser=0;
            accounts[i].loginAttempts = 0; // Reset s? l?n ðãng nh?p sai
            printf("Logged in successfully.\n");
            printf("Your balance: %.2f\n", accounts[i].balance);
            printf("Enter amount to withdraw: ");
            scanf("%f", &amount);
            if (amount > 0 && amount <= accounts[i].balance) {
                accounts[i].balance -= amount;
                printf("Withdrawal successful. New balance: %.2f\n", accounts[i].balance);
                writeAccountFile(accounts, numAccounts);
                printf("Nhan Enter de ket thuc giao dich");
    			while (getchar() != '\n');
    			getchar();
//    			system("cls");
                
                // Ghi l?i l?ch s? giao d?ch
                strcpy(transactions[num_transactions].account_number, accounts[i].username);
				strcpy(transactions[num_transactions].type, "Withdraw");
				transactions[num_transactions].amount = amount;
				num_transactions++;	
            } else {
                printf("Invalid amount or insufficient balance.\n");
            }
            break;			
        }
    }
}

void checkBalance(Account *accounts, int numAccounts,int *reloadUser) {
    int loggedIn = 0;

    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(username, accounts[i].username) == 0) {
            if (accounts[i].isLocked) {
                printf("This account is locked. Please contact customer support.\n");
                return;
            }
            if(*reloadUser){
            	printf("Enter password: ");
            	scanf("%s", password);
			}

			while(strcmp(password, accounts[i].password)){
				printf("Incorrect password.\n");
                accounts[i].loginAttempts++;
                if (accounts[i].loginAttempts >= MAX_LOGIN_ATTEMPTS) {
                    accounts[i].isLocked = 1;
                    printf("Too many incorrect login attempts. This account is now locked.\n");
                    writeAccountFile(accounts, numAccounts);
                	return;
                }
                waitForClear();
                printf("Nhap lai\n");
                scanf("%s", password);
			}
            loggedIn = 1;
            *reloadUser = 0;
            accounts[i].loginAttempts = 0; // Reset s? l?n ðãng nh?p sai
            printf("Logged in successfully.\n");
            printf("Current balance for %s: %.2f\n", username, accounts[i].balance);
            strcpy(transactions[num_transactions].account_number, accounts[i].username);
				strcpy(transactions[num_transactions].type, "checkBlance");
				transactions[num_transactions].amount = 0;
				num_transactions++;	
				
			printf("Nhan Enter de ket thuc giao dich");
    		while (getchar() != '\n');
    		getchar();
//    		system("cls");
    		
            break;			
        }
    }
    
}


//void save_transactions(const char *filename) {
//    FILE *file = fopen(filename, "w");
//    if (file == NULL) {
//        perror("Error opening file");
//        exit(EXIT_FAILURE);
//    }
//
//    int i;
//    for (i = 0; i < num_transactions; i++) {
//        fprintf(file, "%s %.2f %s\n", transactions[i].account_number, transactions[i].amount,transactions[i].type);
//    }
//
//    fclose(file);
//}

void save_transactions() {
    char filename[FILENAME_SIZE];
    char full_path[FILENAME_SIZE];

    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // YYYY-MM-DD.txt
    strftime(filename, sizeof(filename), "_%Y-%m-%d_.txt", timeinfo);

    // link from folder_path,filename
//    snprintf(full_path, sizeof(full_path), "%s/%s", folder_path, filename);

    // new file
    FILE *file = fopen(filename, "w");
    if (file != NULL) {
        int i;
    	for (i = 0; i < num_transactions; i++) {
        	fprintf(file, "%s %.2f %s\n", transactions[i].account_number, transactions[i].amount,transactions[i].type);
    	}
        fclose(file);
        printf("END _%Y-%m-%d_\n", timeinfo);
    } else {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
}







