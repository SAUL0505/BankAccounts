// dataClient.h
#ifndef DATACLIENT_H
#define DATACLIENT_H

#include <string>
#include <cstring>

class ClientData {
public:
    ClientData(int accNum = 0, const std::string& lname = "", const std::string& fname = "", double bal = 0.0)
        : accountNumber(accNum), balance(bal) {
        setLastName(lname);
        setFirstName(fname);
    }

    // Setters
    void setAccountNumber(int accNum) { accountNumber = accNum; }
    void setLastName(const std::string& lname) { strncpy(lastName, lname.c_str(), 15); }
    void setFirstName(const std::string& fname) { strncpy(firstName, fname.c_str(), 10); }
    void setBalance(double bal) { balance = bal; }

    // Getters
    int getAccountNumber() const { return accountNumber; }
    std::string getLastName() const { return lastName; }
    std::string getFirstName() const { return firstName; }
    double getBalance() const { return balance; }

private:
    int accountNumber;
    char lastName[15];
    char firstName[10];
    double balance;
};

#endif
