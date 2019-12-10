//
//  main.c
//  atm_banking
//
//  Created by huseyin on 18.10.2019.
//  Copyright © 2019 huseyin. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define CUSTOMERS_TXT "customers.txt"
#define TRANSACTIONS_TXT "transactions.txt"
#define TEMP_TXT "temp.txt"

enum transactionTypes {
    Withdrawal = 0,
    Deposit = 1,
    Transfer = 2,
};

struct customer {
    char *accountNumber;
    char *name;
    float balance;
    int pin;
    
    struct customer *next;
};

struct transaction {
    char *accountNumber;
    float amount;
    int type; // 0 is withdrawal, 1 is deposit
    char *createdAt;
    
    struct transaction *next;
};

void upsertDataFiles() {
    int numberOfFiles = 2;
    const char *files[numberOfFiles];
    files[0] = CUSTOMERS_TXT;
    files[1] = TRANSACTIONS_TXT;
    
    for (int i = 0; i < numberOfFiles; i++) {
        FILE *file;
        if((file = fopen(files[i], "r")) != NULL)
        {
            // file exists, nothing to do here
            fclose(file);
        }
        else
        {
            // file not found, so create one
            file = fopen(files[i], "w");
            fclose(file);
        }
    }
}

struct customer *getCustomers()
{
    FILE *fp = fopen(CUSTOMERS_TXT, "r");
    struct customer *current, *head;
    head = current = NULL;
    char line[1000];
    
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        char *accountNumber = strtok(line, ",");
        char *name = strtok(NULL, ",");
        char *balance = strtok(NULL, ",");
        char *pin = strtok(NULL, ",\n");
        
        struct customer *customer = malloc(sizeof(struct customer));
        customer->accountNumber = strdup(accountNumber);
        customer->name = strdup(name);
        customer->balance = atof(balance);
        customer->pin = atoi(pin);
        customer->next = NULL;
        
        if(head == NULL){
            current = head = customer;
        } else {
            current = current->next = customer;
        }
    }
    
    fclose(fp);
    
    
    return head;
}

bool checkIfAccountNumberExists(struct customer *head, int accountNumber)
{
    if (head == NULL)
        return false;
    
    if (atoi(head->accountNumber) == accountNumber)
        return true;
    
    return checkIfAccountNumberExists(head->next, accountNumber);
}

int getUniqueAccountNumber() {
    int accountNumber = 0;
    struct customer *head = getCustomers();
    
    do {
        accountNumber = arc4random() % 9000 + 1000;
    } while(checkIfAccountNumberExists(head, accountNumber));
    
    return accountNumber;
}

int createCustomer(char *name, int pin)
{
    FILE *file = fopen(CUSTOMERS_TXT, "a");
    
    if (file == NULL) {
        printf("\nUnable to open file.\n");
        exit(1);
    }
    
    char accountNumber[6];
    int uniqueAccountNumber = getUniqueAccountNumber();
    sprintf(accountNumber, "%d", uniqueAccountNumber);
    
    fprintf(file, "%s,%s,%f,%i\n", accountNumber, name, 0.0, pin);
    
    fclose(file);
    
    return uniqueAccountNumber;
}

struct transaction *getTransactions()
{
    FILE *fp = fopen(TRANSACTIONS_TXT, "r");
    struct transaction *current, *head;
    head = current = NULL;
    char line[1000];
    
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        char *accountNumber = strtok(line, ",");
        char *amount = strtok(NULL, ",");
        char *type = strtok(NULL, ",");
        char *createdAt = strtok(NULL, ",\n");
        
        struct transaction *transaction = malloc(sizeof(struct transaction));
        transaction->accountNumber = strdup(accountNumber);
        transaction->amount = atof(amount);
        transaction->type = atoi(type);
        transaction->createdAt = strdup(createdAt);
        transaction->next = NULL;
        
        if(head == NULL){
            current = head = transaction;
        } else {
            current = current->next = transaction;
        }
    }
    
    fclose(fp);
    
    return head;
}

void createTransaction(struct customer *customer, float amount, int type) {
    FILE *file = fopen(TRANSACTIONS_TXT, "a");
    
    if (file == NULL) {
        printf("\nUnable to open file.\n");
        exit(1);
    }
    
    char createdAt[255];
    time_t now = time(0);
    strftime(createdAt, 255, "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    fprintf(file, "%s,%f,%i,%s\n", customer->accountNumber, amount, type, createdAt);
    
    fclose(file);
    
    return;
}

void updateCustomer(struct customer *currentUser) {
    FILE *file = fopen(CUSTOMERS_TXT, "r");
    FILE *temp = fopen(TEMP_TXT, "w");
    char line[1000];
    
    if (file == NULL || temp == NULL)
    {
        printf("\nUnable to open file.\n");
        exit(1);
    }
    
    while (fgets(line, sizeof(line), file) != NULL) {
        char *accountNumber = strtok(line, ",");
        char *name = strtok(NULL, ",");
        float balance = atof(strtok(NULL, ","));
        int pin = atoi(strtok(NULL, ",\n"));
        
        if (accountNumber == currentUser->accountNumber) {
            fprintf(temp, "%s,%s,%f,%d\n", currentUser->accountNumber, currentUser->name, currentUser->balance, currentUser->pin);
        } else {
            fprintf(temp, "%s,%s,%f,%d\n", accountNumber, name, balance, pin);
        }
    }
    
    /* Close all files to release resource */
    fclose(file);
    fclose(temp);
    
    /* Delete original source file */
    remove(CUSTOMERS_TXT);
    
    /* Rename temp file as original file */
    rename(TEMP_TXT, CUSTOMERS_TXT);
}

struct customer *findUserByAccountNumber(char *accountNumber) {
    struct customer *head = getCustomers();
    struct customer *customer = NULL;
    
    while (head != NULL) {
        if(strcmp(head->accountNumber, accountNumber) == 0)
        {
            customer = head;
            break;
        }
        
        head = head->next;
    }
    
    return customer;
}


struct customer *authenticate(char *accountNumber, int pin) {
    struct customer *head = getCustomers();
    struct customer *customer = NULL;
    
    while (head != NULL) {
        if(strcmp(head->accountNumber, accountNumber) == 0 && head->pin == pin)
        {
            customer = head;
            break;
        }
        
        head = head->next;
    }
    
    return customer;
}

void showAccountInfo(struct customer *customer){
    printf("Your account number is: %s\n", customer->accountNumber);
    printf("Your balance is: %.2f\n", customer->balance);
}

void showBalance(struct customer *customer){
    printf("Your balance is: %.2f\n", customer->balance);
}

void deposit(struct customer *customer, float amount) {
    if (amount > 0) {
        customer->balance += amount;
        updateCustomer(customer);
        createTransaction(customer, amount, Deposit);
        showBalance(customer);
    }
    else {
        printf("Amount should more than 0\n");
    }
}

void withdrawal(struct customer *customer, float amount) {
    if (customer->balance >= amount) {
        customer->balance -= amount;
        updateCustomer(customer);
        createTransaction(customer, amount, Withdrawal);
        showBalance(customer);
    } else {
        printf("Insufficient balance. Your balance is: %.2f \n", customer->balance);
    }
}

void changePin(struct customer *customer,int oldPin, int newPin){
    if (customer->pin == oldPin) {
        customer->pin = newPin;
        updateCustomer(customer);
    } else {
        printf("Wrong pin.\n");
    }
}

void showTransactionsByCustomer(struct customer *customer){
    struct transaction *head = getTransactions();
    
    while (head != NULL) {
        if (strcmp(head->accountNumber, customer->accountNumber) == 0) {
            printf("Amount: %.2f ",head->amount);
            if (head->type == Withdrawal) {
                printf("Type: Withrawal ");
            }
            else if (head->type == Transfer){
                printf("Type: Transfer ");
            } else {
                printf("Type: Deposit ");
            }
            printf("Date: %s",head->createdAt);
            printf("\n");
        }
        
        head = head->next;
    }
}

void transfer(struct customer *from, struct customer *to, float amount) {
    if (strcmp(from->accountNumber, to->accountNumber) != 0) {
        if (from->balance >= amount) {
            // decrement user's balance who is sending the money.
            from->balance -= amount;
            updateCustomer(from);
            createTransaction(from, amount, Transfer);
            
            // increment user's balance who is receiving the money.
            to->balance += amount;
            updateCustomer(to);
            createTransaction(to, amount, Deposit);
            
            // show info
            printf("%.2f has been sent to %s successfully! \n", amount, to->name);
            
            // show user's balance who is sending the money after everything's done.
            showBalance(from);
        } else {
            printf("Insufficient balance. Your balance is: %.2f \n", from->balance);
        }
    } else {
        printf("You can't transfer money to yourself. \n");
    }
}

void showOperationsMenu(struct customer *currentUser) {
    int selection;
    float amount;
    int oldPin, newPin;
    char accountNumber[6];
    
    do {
        
        printf("1- Account information\n");
        printf("2- Transactions\n");
        printf("3- Deposit\n");
        printf("4- Pin change\n");
        printf("5- Balance\n");
        printf("6- Withdrawal\n");
        printf("7- Transfer\n");
        
        printf("Select an operation: ");
        do {
            scanf("%i", &selection);
        } while( selection == '\n' );
        
        switch (selection)
        {
            case 1:
                showAccountInfo(currentUser);
                break;
                
            case 2:
                showTransactionsByCustomer(currentUser);
                break;
                
            case 3:
                printf("Enter the amount: ");
                scanf("%f",&amount);
                deposit(currentUser, amount);
                break;
                
            case 4:
                printf("Please enter your current pin: ");
                scanf("%i", &oldPin);
                printf("Please enter your new pin: ");
                scanf("%i", &newPin);
                changePin(currentUser, oldPin, newPin);
                break;
                
            case 5:
                showBalance(currentUser);
                break;
                
            case 6:
                printf("Please enter an amount: ");
                scanf("%f",&amount);
                withdrawal(currentUser, amount);
                break;
                
            case 7:
                printf("Please enter the user's account number: ");
                scanf("%s", accountNumber);
                struct customer *to = findUserByAccountNumber(accountNumber);
                if (to != NULL) {
                    printf("Please enter the amount: ");
                    scanf("%f", &amount);
                    
                    transfer(currentUser, to, amount);
                }
                else{
                    printf("This user is not exist.\n");
                }
                break;
                
            default:
                printf("only use numbers between 1 and 7. \n");
                break;
        }
    } while(selection >= 1 && selection <= 7);
}

void authorizeOperationsMenu() {
    char accountNumber[6];
    int pin;
    
    printf("Please enter your account number: ");
    scanf("%s", accountNumber);
    printf("Please enter your pin: ");
    scanf("%i", &pin);
    struct customer *customer = authenticate(accountNumber, pin);
    
    if (customer != NULL) {
        printf("\n----------------------------\nWelcome %s!\n----------------------------\n\n", customer->name);
        showOperationsMenu(customer);
    } else {
        printf("\nThis user is not exist.\n");
    }
}

void welcomeMenu(){
    int option;
    char name[1000];
    char temp;
    int pin;
    int pinConfirmation;
    printf("1- Sign in\n2- Sign up\n");
    printf("Please select an option: ");
    scanf("%d",&option);
    switch (option) {
        case 1:
            authorizeOperationsMenu();
            break;
        case 2:
            printf("Name Surname: ");
            scanf("%c", &temp); // temp statement to clear buffer
            scanf("%[^\n]",name);
            printf("Pin: ");
            scanf("%i", &pin);
            printf("Please enter your pin again: ");
            scanf("%i", &pinConfirmation);
            if (pin == pinConfirmation) {
                int accountNumber = createCustomer(name, pin);
                printf("Your account has been created successfully.\n");
                printf("Your account number is: %i \n", accountNumber);
                authorizeOperationsMenu();
            }
            else{
                printf("Pins couldn't match!");
            }
        default:
            break;
    }
}

int main(int argc, const char * argv[]){
    upsertDataFiles();
    welcomeMenu();
    
    /*
     authenticate olabilmek icin 1 fonksiyon'a ihtiyacımız var, 1 numaralı menu o işe yarıyor
     
     Insert your card (Y/N)
     
     Kullanıcıya iki seçenek gösterilir. Yes/No (Y/N)
     + Yes derse;
     + Kullanıcıya müşteri numarası sorulur ve şifresi sorulur.
     + Doğru giriş yaparsa diğer menü gösterilir.
     + Yanlış giriş yaparsa, bankamatik içerisine ne kadar yanlış giriş yaptığı sayısı log olarak yazılır.
     + 3 kere yanlış giriş yaparsa, kartı bloke olur.
     + No derse;
     + Program kapanır.
     
     oluşturulcak method
     
     - int authenticate(account_number, password)
     
     eğer başarılı giriş yaparsa authenticate methodu geriye pozitif bir sayı döncek.
     eğer başarılı giriş yapamazsa authenticate methodu geriye negatif bir sayı döncek.
     */
    
    /*
     authenticate olduktan sonra kullanıcıya yapabileceği işlemleri göstermeli.
     
     1- Account information
     2- Transactions
     3- Deposit
     4- Pin change
     5- Balance
     6- Withdrawal
     7- Bill payment
     
     Kullanıcı 1. seçeneği seçerse;
     + Hesap numarası
     + Bakiyesi
     
     Kullanıcı 2. seçeneği seçerse;
     + Kullanıcının bizim atm'yi kullanarak yaptığı işlemler kayıt altına alıncak.
     + Bu seçenek altında yapılan işlemler gösterilcek.
     
     Kullanıcı 3. seçeneği seçerse;
     + Ne kadar miktar para yatıracağı sorulur.
     + Dekont gösterilir.
     + Hesap bilgileri gösterilir.
     + Ne kadar miktar para yatırdığı gösterilir.
     
     Kullanıcı 4. seçeneği seçerse;
     + Kullanıcıdan değiştirmek istediği pin numarası alınır.
     + Pin numarasını doğrulamak için tekrardan aynı pin numarasını girmesi istenir.
     + Pin numaraları birbirlerine eşitse pin değiştirilir.
     
     Kullanıcı 5. seçeneği seçerse;
     + Kullanıcının bakiyesi gösterilir.
     
     Kullanıcı 6. seçeneği seçerse;
     + Kullanıcıya önceden belirlenmiş miktarlar gösterilir. 10 hariç 10,un katları. (20, 50, 100)
     + Seçeneklerden birini seçmezse kendisi giriş yapabilir. Yine 10'un katı olmak zorunda.
     
     Kullanıcı 7. seçeneği seçerse;
     + Hangi tip faturayı ödemek istediği sorulur.
     + Tutar random belirlenir.
     + Kullanıcı onay verdikten sonra, hesabından fatura tutarı düşülür.
     + Dekont verilir.
     + Faturanın adı, tipi.
     + Hesap bilgileri.
     + Ne kadar ödediği.
     
     */
    return 0;
}
