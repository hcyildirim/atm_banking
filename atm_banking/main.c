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

struct customer {
    int id;
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
    char line[255];
    
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        char *id = strtok(line, ",");
        char *accountNumber = strtok(NULL, ",");
        char *name = strtok(NULL, ",");
        char *balance = strtok(NULL, ",");
        char *pin = strtok(NULL, ",\n");
        
        struct customer *customer = malloc(sizeof(struct customer));
        customer->id = atoi(id);
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

struct transaction *getTransactions()
{
    FILE *fp = fopen(TRANSACTIONS_TXT, "r");
    struct transaction *current, *head;
    head = current = NULL;
    char line[255];
    
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

void updateTransactionData(struct transaction *head) {
    FILE *file = fopen(TRANSACTIONS_TXT, "r");
    FILE *temp = fopen(TEMP_TXT, "w");
    
    if (file == NULL || temp == NULL)
    {
        printf("\nUnable to open file.\n");
        exit(1);
    }
    
    while (head != NULL)
    {
        fprintf(temp, "%s,%f,%i,%s\n", head->accountNumber, head->amount,head->type, head->createdAt);
        head = head->next;
    }
    
    /* Close all files to release resource */
    fclose(file);
    fclose(temp);
    
    /* Delete original source file */
    remove(TRANSACTIONS_TXT);
    
    /* Rename temp file as original file */
    rename(TEMP_TXT, TRANSACTIONS_TXT);
}

void createTransaction(struct transaction **head_ref, struct customer *customer, float amount, int type) {
    char createdAt[255];
    time_t now = time(0);
    strftime(createdAt, 255, "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    struct transaction *last = *head_ref;
    
    struct transaction *transaction = malloc(sizeof(struct transaction));
    transaction->accountNumber = customer->accountNumber;
    transaction->amount = amount;
    transaction->type = type;
    transaction->createdAt = createdAt;
    transaction->next = NULL;
    
    if (*head_ref == NULL)
    {
        *head_ref = transaction;
        updateTransactionData(transaction);
        return;
    }
    
    while (last->next != NULL){
        last = last->next;
    }
    
    last->next = transaction;
    
    updateTransactionData(*head_ref);
    
    return;
}

void updateCustomerData(struct customer *head) {
    FILE *file = fopen(CUSTOMERS_TXT, "r");
    FILE *temp = fopen(TEMP_TXT, "w");
    
    if (file == NULL || temp == NULL)
    {
        printf("\nUnable to open file.\n");
        exit(1);
    }
    
    while (head != NULL)
    {
        fprintf(temp, "%i,%s,%s,%f,%d\n", head->id, head->accountNumber, head->name, head->balance, head->pin);
        head = head->next;
    }
    
    /* Close all files to release resource */
    fclose(file);
    fclose(temp);
    
    /* Delete original source file */
    remove(CUSTOMERS_TXT);
    
    /* Rename temp file as original file */
    rename(TEMP_TXT, CUSTOMERS_TXT);
}

void createCustomer(struct customer **head_ref, char *name, int pin) {
    struct customer *last = *head_ref;
    
    char accountNumber[6];
    sprintf(accountNumber, "%d", arc4random() % 9000 + 1000);

    struct customer *customer = malloc(sizeof(struct customer));
    customer->accountNumber = accountNumber;
    customer->name = name;
    customer->balance = 0;
    customer->pin = pin;
    customer->next = NULL;
    
    if (*head_ref == NULL)
    {
        *head_ref = customer;
        customer->id = 1;
        updateCustomerData(customer);
        return;
    }
    
    while (last->next != NULL){
        last = last->next;
    }
    
    customer->id = last->id + 1;
    last->next = customer;
    
    updateCustomerData(*head_ref);
    
    return;

}

struct customer *authenticate(struct customer *head, char *accountNumber, int pin) {
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
    printf("Your balance is: %f\n", customer->balance);
}

void showBalance(struct customer *customer){
    printf("Your balance is: %f\n", customer->balance);
}

void deposit(struct customer *customer, float amount) {
    if (amount > 0) {
        customer->balance += amount;
    }
    else {
        printf("Amount should more than 0\n");
    }
}

void withdrawal(struct customer *customer, float amount) {
    if (customer->balance >= amount) {
        customer->balance -= amount;
    } else {
        printf("Insufficient balance.Your balance is: %f", customer->balance);
    }
}

void changePin(struct customer *customer,int oldPin, int newPin){
    if (customer->pin == oldPin) {
        customer->pin = newPin;
    } else {
        printf("Wrong pin.\n");
    }
}

void showTransactionsByCustomer(struct customer *customer){
    struct transaction *transactions = getTransactions();
    
    while (transactions != NULL) {
        if (strcmp(transactions->accountNumber, customer->accountNumber) == 0) {
            printf("Amount: %f ",transactions->amount);
            if (transactions->type == 0) {
                printf("Type: Withrawal ");
            }
            else{
                printf("Type: Deposit ");
            }
            printf("Date: %s",transactions->createdAt);
        }
        printf("\n");
        
        transactions = transactions->next;
    }
    
}

void showOperationsMenu(struct customer *currentUser) {
    int selection;
    float amount;
    int oldPin, newPin;
    
    do {
        
        printf("1- Account information\n");
        printf("2- Transactions\n");
        printf("3- Deposit\n");
        printf("4- Pin change\n");
        printf("5- Balance\n");
        printf("6- Withdrawal\n");
        printf("7- Bill payment\n");
        
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
                printf("7 selected. \n");
             
                break;
                
            default:
                printf("only use numbers between 1 and 7. \n");
                break;
        }
    } while(selection >= 1 && selection <= 7);
}

void authorizeOperationsMenu() {
    struct customer *customers = getCustomers();
    
    char accountNumber[255];
    int pin;
    
    printf("Please enter your account number: ");
    scanf("%s", accountNumber);
    printf("Please enter your pin: ");
    scanf("%i", &pin);
    struct customer *customer = authenticate(customers, accountNumber, pin);
    
    if (customer != NULL) {
        //        struct transaction *transactions = getTransactions();
        //        createTransaction(&transactions, customers, 20, 1);
        
        //        while (transactions != NULL) {
        //            if(strcmp(transactions->accountNumber, customer->accountNumber) == 0) {
        //                printf("Your transaction");
        //            }
        //
        //            transactions = transactions->next;
        //        }
        
        //        createCustomer(&customers, "kasfkaksfaksf", 1234);
        
        printf("\n----------------------------\nWelcome %s!\n----------------------------\n\n", customer->name);
        showOperationsMenu(customer);
    } else {
        printf("\nThis user is not exist.\n");
    }
}

int main(int argc, const char * argv[]){
    upsertDataFiles();
    
    authorizeOperationsMenu();
    
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
