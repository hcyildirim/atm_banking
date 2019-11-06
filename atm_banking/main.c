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

#define CUSTOMERS_TXT "customers.txt"
#define ACCOUNTS_TXT "accounts.txt"
#define TEMP_TXT "temp.txt"

struct customer {
    int id;
    char *accountNumber;
    char *name;
    float balance;
    int pin;
};

void showOperationsMenu() {
    int selection;
    
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
                       printf("1 selected. \n");
                       break;
                       
                   case 2:
                       printf("2 selected. \n");
                       break;
                       
                  case 3:
                       printf("3 selected. \n");
                       break;
                       
                    case 4:
                       printf("4 selected. \n");
                       break;
                       
                    case 5:
                       printf("5 selected. \n");
                       break;
                       
                   case 6:
                       printf("6 selected. \n");
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

void upsertDataFiles() {
    int numberOfFiles = 2;
    const char *files[numberOfFiles];
    files[0] = CUSTOMERS_TXT;
    files[1] = ACCOUNTS_TXT;
    
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

void updateCustomerData(struct customer *customers) {
    FILE *file = fopen(CUSTOMERS_TXT, "r");
    FILE *temp = fopen(TEMP_TXT, "w");
    int lineCount = 0;
    char line[255];
    
    if (file == NULL || temp == NULL)
    {
        printf("\nUnable to open file.\n");
        exit(1);
    }
    
    while (fgets(line, sizeof(line), file) != NULL)
    {
        lineCount++;
    }
    
    for(int i = 0; i < lineCount; i++) {
        fprintf(temp, "%i,%s,%s,%f,%d\n", customers[i].id, customers[i].accountNumber, customers[i].name, customers[i].balance, customers[i].pin);
    }
    
    /* Close all files to release resource */
    fclose(file);
    fclose(temp);

    /* Delete original source file */
    remove(CUSTOMERS_TXT);

    /* Rename temp file as original file */
    rename(TEMP_TXT, CUSTOMERS_TXT);
}

struct customer *getCustomers(int k)
{
    FILE *fp = fopen(CUSTOMERS_TXT, "r");
    int i = 0;
    struct customer *customers = (struct customer*)calloc(k, sizeof(struct customer));
    char line[255];

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        char *id = strtok(line, ",");
        char *accountNumber = strtok(NULL, ",");
        char *name = strtok(NULL, ",");
        char *balance = strtok(NULL, ",");
        char *pin = strtok(NULL, ",\n");
        
        struct customer input = { atoi(id), strdup(accountNumber), strdup(name), atof(balance), atoi(pin) };
        
        customers[i] = input;
        
        i++;
    }
    
    fclose(fp);
        
    return customers;
}

int authenticate(struct customer *customers, int k, char *accountNumber, int pin) {
    int result = 0;
    
    for(int i = 0; i < k; i++)
    {
        if(strcmp(customers[i].accountNumber, accountNumber) == 0 && customers[i].pin == pin)
        {
            result = 1;
            break;
        }
    }
    
    return result;
}

void showAccountInfo(struct customer *customer){
    printf("Your account number is: %s\n", customer->accountNumber);
    printf("Your balance is: %f\n", customer->balance);
}

void deposit(struct customer *customer, float amount){
    customer->balance = customer->balance + amount;
}

void authorizeOperationsMenu() {
    struct customer *customers = getCustomers(2);

    char accountNumber[255];
    int pin;
    
    printf("Please enter your account number: ");
    scanf("%s", accountNumber);
    printf("Please enter your pin: ");
    scanf("%i",&pin);
    int result = authenticate(customers, 2, accountNumber, pin);
    
    
    if (result == 1) {
        showOperationsMenu();
    } else {
        printf("\nThis user is not exist");
    }
    
    free(customers);
}

int main(int argc, const char * argv[]){
    upsertDataFiles();
    
    
    //    updateCustomerData(customers);
    
    //    int result = authenticate(customers, 2, "0956", 1234);
    
    
    char option, loweredOption;
    bool cardInserted = false;
    
    do {
        printf("Insert your card (y/n): \n");

        do {
            scanf("%c", &option);
        } while( option == '\n' );
        
        loweredOption = tolower(option);
        
        switch (loweredOption)
        {
            case 'y':
                cardInserted = true;
                system("clear");
                authorizeOperationsMenu();
                break;
                 
            case 'n':
                exit(0);
                break;
        
            default:
                printf("only use y and n chars. \n");
                break;
        }
    } while (cardInserted != true);
             
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
            + Kullanıcıya hangi çeşit hesabına para yatırmak istediği sorulur. (Dolar, TL, Euro)
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
