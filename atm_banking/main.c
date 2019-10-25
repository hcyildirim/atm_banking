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
               
                   default:
                       printf("only use numbers between 1 and 7. \n");
                       break;
               }
    } while(selection >= 1 && selection <= 7);
}

int main(int argc, const char * argv[]){
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
                showOperationsMenu();
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
            + Şube bilgisi
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
