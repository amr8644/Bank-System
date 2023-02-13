#include <iostream>
#include <conio.h>
#include <ctime>
#include <random>
#include <fstream>
#include <cstring>
#include "Account.h"
#include "Bank.h"

using namespace std;

void menu()
{
    Account account;
    cout << "<<<<<\tBanking System\t>>>>>" << endl;
    cout << "<<<\tMain menu\t>>>" << endl;
    int choice;

    cout << "1 - Create an account" << endl;
    cout << "2 - Login to an account" << endl;
    cout << "3 - Show all accounts" << endl;
    cout << "4 - Search account by ID" << endl;
    cout << "5 - Delete account" << endl;
    cout << "Choose a number: ";
    cin >> choice;
   
     switch (choice)
     {
      case 1:
         account_create();
         break;
      case 2:
         account_login();
         break;
      case 3:
         account_view_all();
         break;
      case 4:
         account_search();
         break;
      case 5:
         account_deletion();
         break;
     default:
         std::cout << "Invalid choice..." << std::endl;
         break;
     }
}

void account_create()
{
   Account person;
            
   std::ofstream fp("Accounts.dat",std::ios::binary | std::ios::app);

   std::random_device dev;
   std::mt19937 rng(dev());
   std::uniform_int_distribution<std::mt19937::result_type> dist6(1,10000); 

   time_t now = time(0);
   char* dt = ctime(&now);
   tm *gmtm = gmtime(&now);
   dt = asctime(gmtm);

   person.id = dist6(rng);
   std::cout << "ID: " << person.id << std::endl;

   std::cin.ignore(1);
   std::cout << "Name: ";
   std::cin.getline(person.name,100);

   std::cout << "Pin: ";
   std::cin >> person.pin;

   std::cout << "Deposit: ";
   std::cin >> person.balance;

   strcpy(person.created_at,dt);
   std::cout << "Created at: " << person.created_at;

   fp.write((char*)&person,sizeof(person));
   fp.close();

}

void account_view_all()
{
   Account person;
   std::ifstream fp("Accounts.dat");
   while (fp.read((char *)&person,sizeof(person)))
   {
      std::cout << "*******************************************" << std::endl;
      std::cout << "Account's ID: " << person.id << std::endl
      << "Account's name: " << person.name << std::endl
      << "Account's balance: $" << person.balance << std::endl
      << "Created At: " << person.created_at << std::endl;  
   }

   fp.close();
}

void account_search()
{
   Account person;
   std::ifstream fp("Accounts.dat");
   int choice;
   std::cout << "Enter ID number :";
   std::cin >> choice;

   while (fp.read((char *)&person,sizeof(person)))
   {
      if (choice == person.id)
      {
         std::cout << "*******************************************" << std::endl;
         std::cout << "Account found" << std::endl;
         std::cout << "Account's ID: " << person.id << std::endl
         << "Account's name: " << person.name << std::endl
         << "Account's balance: $" << person.balance << std::endl
         << "Created At: " << person.created_at << std::endl;  
      }
      
   }
   fp.close();
}

void account_deletion()
{
   Account person;
   std::ifstream fp("Accounts.dat");
   std::ofstream tmp("tmp.dat",std::ios::binary | std::ios::app);
   int choice;
   std::cout << "Enter ID number :";
   std::cin >> choice;

   while (fp.read((char *)&person,sizeof(person)))
   {
      if (choice != person.id) tmp.write((char*)&person,sizeof(person));

      if (choice == person.id)
      {
         std::cout << "Account found and deleted" << std::endl;
         std::cout << "*******************************************" << std::endl;
         std::cout << "Account's ID: " << person.id << std::endl
         << "Account's name: " << person.name << std::endl
         << "Account's balance: $" << person.balance << std::endl
         << "Created At: " << person.created_at << std::endl; 
      }
   }

   fp.close();
   tmp.close();

   int status = remove("Accounts.dat");
   if(status == 0) cout <<"\nFile Deleted Successfully!" << std::endl;
   else cout<<"\nError Occurred!";

   if (rename("tmp.dat", "Accounts.dat") != 0) perror("Error renaming file\n");
	else cout << "File renamed successfully";
}

void account_login()
{
   int choice;
   int found = 0;
   Account person;
   std::ifstream fp("Accounts.dat");
   std::cout << "Enter ID number :";
   std::cin >> choice;

   while (fp.read((char *)&person,sizeof(person))) if (choice == person.id) found = 1;

   if (found)
   {
      int pin;
      std::cout << "Account found" << std::endl;
      std:: cout << "Enter pin number" << std::endl;
      std::cin >> pin;
      std::cout << "Checking...." << std::endl;
      if (pin == person.pin)
      {
         std::cout << "Pin correct" << std::endl;
         long double balance = person.menu();
         fp.close();
         account_update(choice,balance);
      }

      else std::cout << "Wrong pin number. Please try again..." << std::endl;
   }
   else std::cout << "Account not found..." << std::endl;
      
}

void account_update(int id,long double balance)
{
   Account person;
   std::ifstream fp("Accounts.dat");
   std::ofstream tmp("tmp.dat",std::ios::binary | std::ios::app);
   int choice = id;

   while (fp.read((char *)&person,sizeof(person)))
   {
      if (choice == person.id) 
      {
         person.balance = balance;
         tmp.write((char*)&person,sizeof(person));
      }
   }

   fp.close();
   tmp.close();

   int status = remove("Accounts.dat");
   if(status == 0) cout<<"\nFile Deleted Successfully!" << std::endl;
   else cout<<"\nError Occurred!";

   if (rename("tmp.dat", "Accounts.dat") != 0) perror("Error renaming file\n");
	else cout << "File renamed successfully";
}