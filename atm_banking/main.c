//
//  main.c
//  atm_banking
//
//  Created by huseyin on 18.10.2019.
//  Copyright © 2019 huseyin. All rights reserved.
//

#include <stdio.h>
#include <ctype.h>

int main(int argc, const char * argv[]){
    char option, loweredOption;
    
    do {
        printf("Insert your card (y/n): \n");
        
        do {
            scanf("%c", &option);
        } while( option == '\n' );
        
        loweredOption = tolower(option);
        
        switch (loweredOption)
        {
            case 'y':
                // TODO: buraya menu gelcek @huso
                break;
                 
            case 'n':
                // TODO: programdan çıkıcak. @huso
                break;
        
            default:
                printf("wrong character");
                break;
        }
    } while (option != 'n');
    
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
