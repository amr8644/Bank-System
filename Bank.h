#if !defined(BANK_H)
#define BANK_H
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>

#include "Accounts.h"
#include "SQL.h"

class Bank {
public:
  void bank_login(sqlite3 &db);
  void bank_menu(sqlite3 &db, std::string acc_no, double balance);
  void bank_withdraw(sqlite3 &db, std::string acc_no, double balance);
  void bank_deposit(sqlite3 &db, std::string acc_no);
  void bank_transfer(sqlite3 &db, std::string acc_no);
};

void Bank::bank_menu(sqlite3 &db, std::string acc_no, double balance) {

  int choice;

  std::cout << "Menu"
            << "\n";

  std::cout << "[1] Withdraw"
            << "\n";
  std::cout << "[2] Deposit"
            << "\n";
  std::cout << "[3] Transfer"
            << "\n";

  std::cout << "Your choice: ";
  std::cin >> choice;
  switch (choice) {

  case 1:
    bank_withdraw(db, acc_no, balance);
    break;
  case 2:
    std::cout << "X";
    break;
  case 3:
    std::cout << "aaa";
    break;
  }
}
void Bank::bank_login(sqlite3 &db) {

  std::string acc_no, pin;

  double balance;
  char *err_msg;
  std::cout << "[*] Enter yout account number: ";

  std::getline(std::cin >> std::ws, acc_no);
  std::cout << "[*] Enter your pin number: ";
  std::cin >> pin;

  sqlite3_stmt *stmt;
  int rc;
  std::string sql = "SELECT *  FROM ACCOUNTS WHERE name = ? AND pin = ?;";

  sqlite3_prepare_v2(&db, sql.c_str(), -1, &stmt, NULL);

  sqlite3_bind_text(stmt, 1, acc_no.c_str(), acc_no.length(), NULL);
  sqlite3_bind_text(stmt, 2, pin.c_str(), pin.length(), NULL);
  if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    balance = sqlite3_column_double(stmt, 4);
    bank_menu(db, acc_no, balance);
  }
}

void Bank::bank_withdraw(sqlite3 &db, std::string acc_no, double balance) {

  double amount;
  sqlite3_stmt *stmt;
  int rc;
  std::cout << "[*] How much do you want to withdraw?";
  std::cin >> amount;
  if (amount > balance) {
    std::cout << "[!] Not enought balance"
              << "\n";
    return;
  }
  std::string sql = "UPDATE Accounts set balance = ? where name = ?;";

  sqlite3_prepare_v2(&db, sql.c_str(), -1, &stmt, NULL);

  if (stmt != NULL) {
    sqlite3_bind_text(stmt, 1, acc_no.c_str(), acc_no.length(),
                      SQLITE_TRANSIENT);

    sqlite3_bind_double(stmt, 2, balance - amount);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
  } else
    std::cout << "\n" << sqlite3_errmsg(&db) << "\n";
}

#endif
