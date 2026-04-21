/* 
   Bank Management System Mini Project
   Surabhi Datta - 1262253459
   Saloni Kaushik - 1262253425
   Nakshatra Mate - 1262252608
   Aaliya Shrivastava - 1262252034
   Raga Varma - 1262252551
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "Bank_Management_System.csv"

struct Account {
    int accountNumber;
    char *name;
    char *password;
    char *birthdate;
    char *email;
    float balance;
    int isActive;
};

struct Account *accounts = NULL;
int accountCount = 0;
int nextAccountNumber = 100001;

/* Function Prototypes */
void DisplayMenu();
void LoadAccounts();
void SaveAccounts();
void CreateAccount();
void CheckBalance();
void DepositMoney();
void WithdrawMoney();
void TransferMoney();
void CheckCredentials();
void UpdateCredentials();
void DeleteAccount();
int FindAccount(int accNum);
int VerifyPassword(int accNum, char *pass);

/*Main Program */
int main() {

    LoadAccounts();

    int choice;

    printf("=== BANK MANAGEMENT SYSTEM ===\n");

    while(1) {
        DisplayMenu();
        printf("Enter your choice (1-9): ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: CreateAccount(); break;
            case 2: CheckBalance(); break;
            case 3: DepositMoney(); break;
            case 4: WithdrawMoney(); break;
            case 5: TransferMoney(); break;
            case 6: CheckCredentials(); break;
            case 7: UpdateCredentials(); break;
            case 8: DeleteAccount(); break;
            case 9:
                printf("Thank you for using Bank Management System!\n");
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }
    return 0;
}

/*Display Menu*/
void DisplayMenu() {
    printf("\n========== MAIN MENU ==========\n");
    printf("1. Create Account\n");
    printf("2. Check Balance\n");
    printf("3. Deposit Money\n");
    printf("4. Withdraw Money\n");
    printf("5. Transfer Money\n");
    printf("6. Check Credentials\n");
    printf("7. Update Credentials\n");
    printf("8. Delete Account\n");
    printf("9. Exit\n");
}

/*Load From CSV*/
void LoadAccounts() {

    FILE *fp = fopen(FILE_NAME, "r");
    if(fp == NULL) return;

    char line[4096];

    while(fgets(line, sizeof(line), fp)) {

        accounts = realloc(accounts, (accountCount + 1) * sizeof(struct Account));

        char *token = strtok(line, ",");

        accounts[accountCount].accountNumber = atoi(token);

        token = strtok(NULL, ",");
        accounts[accountCount].name = strdup(token);

        token = strtok(NULL, ",");
        accounts[accountCount].password = strdup(token);

        token = strtok(NULL, ",");
        accounts[accountCount].birthdate = strdup(token);

        token = strtok(NULL, ",");
        accounts[accountCount].email = strdup(token);

        token = strtok(NULL, ",");
        accounts[accountCount].balance = atof(token);

        token = strtok(NULL, ",");
        accounts[accountCount].isActive = atoi(token);

        if(accounts[accountCount].accountNumber >= nextAccountNumber)
            nextAccountNumber = accounts[accountCount].accountNumber + 1;

        accountCount++;
    }

    fclose(fp);
}

/*Save To CSV*/
void SaveAccounts() {

    FILE *fp = fopen(FILE_NAME, "w");

    for(int i = 0; i < accountCount; i++) {
        fprintf(fp, "%d,%s,%s,%s,%s,%.2f,%d\n",
            accounts[i].accountNumber,
            accounts[i].name,
            accounts[i].password,
            accounts[i].birthdate,
            accounts[i].email,
            accounts[i].balance,
            accounts[i].isActive);
    }

    fclose(fp);
}

/* Find Account */
int FindAccount(int accNum) {

    for(int i = 0; i < accountCount; i++) {
        if(accounts[i].accountNumber == accNum && accounts[i].isActive)
            return i;
    }
    return -1;
}

/* Verify Password */
int VerifyPassword(int accNum, char *pass) {

    int index = FindAccount(accNum);

    if(index == -1) {
        printf("Account not found!\n");
        return 0;
    }

    if(strcmp(accounts[index].password, pass) == 0)
        return 1;

    printf("Incorrect password!\n");
    return 0;
}

/* Create Account */
void CreateAccount() {

    struct Account newAcc;
    newAcc.accountNumber = nextAccountNumber++;

    char tempName[1000], tempPass[1000];
    char tempBirth[1000], tempEmail[1000];

    getchar();

    printf("Enter Name: ");
    fgets(tempName, sizeof(tempName), stdin);
    tempName[strcspn(tempName, "\n")] = 0;

    printf("Enter Password: ");
    fgets(tempPass, sizeof(tempPass), stdin);
    tempPass[strcspn(tempPass, "\n")] = 0;

    printf("Enter Birthdate (DD-MM-YYYY): ");
    fgets(tempBirth, sizeof(tempBirth), stdin);
    tempBirth[strcspn(tempBirth, "\n")] = 0;

    printf("Enter Email ID: ");
    fgets(tempEmail, sizeof(tempEmail), stdin);
    tempEmail[strcspn(tempEmail, "\n")] = 0;

    printf("Enter Initial Deposit: Rs. ");
    scanf("%f", &newAcc.balance);

    newAcc.name = strdup(tempName);
    newAcc.password = strdup(tempPass);
    newAcc.birthdate = strdup(tempBirth);
    newAcc.email = strdup(tempEmail);
    newAcc.isActive = 1;

    accounts = realloc(accounts, (accountCount + 1) * sizeof(struct Account));
    accounts[accountCount++] = newAcc;

    SaveAccounts();

    printf("Account created successfully!\n");
    printf("Account Number: %d\n", newAcc.accountNumber);
}

/* Check Balance */
void CheckBalance() {

    int accNum;
    char pass[1000];

    printf("Enter account number: ");
    scanf("%d", &accNum);

    printf("Enter password: ");
    scanf("%s", pass);

    if(VerifyPassword(accNum, pass)) {
        int index = FindAccount(accNum);
        printf("Current Balance: Rs. %.2f\n", accounts[index].balance);
    }
}

/* Deposit Money */
void DepositMoney() {

    int accNum;
    char pass[1000];
    float amount;

    printf("Enter account number: ");
    scanf("%d", &accNum);

    printf("Enter password: ");
    scanf("%s", pass);

    if(VerifyPassword(accNum, pass)) {

        printf("Enter deposit amount: Rs. ");
        scanf("%f", &amount);

        int index = FindAccount(accNum);

        if(amount > 0) {
            accounts[index].balance += amount;
            SaveAccounts();
            printf("Deposit successful!\n");
            printf("Updated Balance: Rs. %.2f\n", accounts[index].balance);
        } else {
            printf("Invalid amount!\n");
        }
    }
}

/* Withdraw Money */
void WithdrawMoney() {

    int accNum;
    char pass[1000];
    float amount;

    printf("Enter account number: ");
    scanf("%d", &accNum);

    printf("Enter password: ");
    scanf("%s", pass);

    if(VerifyPassword(accNum, pass)) {

        printf("Enter withdrawal amount: Rs. ");
        scanf("%f", &amount);

        int index = FindAccount(accNum);

        if(amount > 0 && amount <= accounts[index].balance) {
            accounts[index].balance -= amount;
            SaveAccounts();
            printf("Withdrawal successful!\n");
            printf("Updated Balance: Rs. %.2f\n", accounts[index].balance);  
        } else {
            printf("Insufficient balance!\n");
        }
    }
}

/* Transfer Money From One Account To Another */
void TransferMoney() {

    int fromAcc, toAcc;
    char pass[1000];
    float amount;

    printf("Enter source account number: ");
    scanf("%d", &fromAcc);

    printf("Enter password: ");
    scanf("%s", pass);

    if(!VerifyPassword(fromAcc, pass)) return;

    printf("Enter destination account number: ");
    scanf("%d", &toAcc);

    int toIndex = FindAccount(toAcc);
    if(toIndex == -1) {
        printf("Destination account not found!\n");
        return;
    }

    printf("Enter transfer amount: Rs. ");
    scanf("%f", &amount);

    int fromIndex = FindAccount(fromAcc);

    if(amount > 0 && amount <= accounts[fromIndex].balance) {
        accounts[fromIndex].balance -= amount;
        accounts[toIndex].balance += amount;
        SaveAccounts();
        printf("Transfer successful!\n");
    } else {
        printf("Insufficient balance!\n");
    }
}

/* Check Credentials */
void CheckCredentials() {

    int accNum;
    char pass[1000];

    printf("Enter account number: ");
    scanf("%d", &accNum);

    printf("Enter password: ");
    scanf("%s", pass);

    if(VerifyPassword(accNum, pass)) {
        int index = FindAccount(accNum);
        printf("Name: %s\n", accounts[index].name);
        printf("Birthdate: %s\n", accounts[index].birthdate);
        printf("Email: %s\n", accounts[index].email);
        printf("Account Number: %d\n", accounts[index].accountNumber);
        printf("Balance: Rs. %.2f\n", accounts[index].balance);
    }
}

/* Update Credentials */
void UpdateCredentials() {

    int accNum;
    char pass[1000];
    char newName[5], newPass[1000];
    char newBirth[1000], newEmail[1000];

    printf("Enter account number: ");
    scanf("%d", &accNum);

    printf("Enter current password: ");
    scanf("%s", pass);

    if(VerifyPassword(accNum, pass)) {

        int index = FindAccount(accNum);

        getchar();

        printf("Enter new name: ");
        fgets(newName, sizeof(newName), stdin);
        newName[strcspn(newName, "\n")] = 0;

        printf("Enter new password: ");
        fgets(newPass, sizeof(newPass), stdin);
        newPass[strcspn(newPass, "\n")] = 0;

        printf("Enter new birthdate: ");
        fgets(newBirth, sizeof(newBirth), stdin);
        newBirth[strcspn(newBirth, "\n")] = 0;

        printf("Enter new email: ");
        fgets(newEmail, sizeof(newEmail), stdin);
        newEmail[strcspn(newEmail, "\n")] = 0;

        free(accounts[index].name);
        free(accounts[index].password);
        free(accounts[index].birthdate);
        free(accounts[index].email);

        accounts[index].name = strdup(newName);
        accounts[index].password = strdup(newPass);
        accounts[index].birthdate = strdup(newBirth);
        accounts[index].email = strdup(newEmail);

        SaveAccounts();
        printf("Credentials updated successfully!\n");
    }
}

/* Delete Account */
void DeleteAccount() {

    int accNum;
    char pass[1000];

    printf("Enter account number to delete: ");
    scanf("%d", &accNum);

    printf("Enter password: ");
    scanf("%s", pass);

    if(VerifyPassword(accNum, pass)) {
        int index = FindAccount(accNum);
        accounts[index].isActive = 0;
        SaveAccounts();
        printf("Account deleted successfully!\n");
    }
}
