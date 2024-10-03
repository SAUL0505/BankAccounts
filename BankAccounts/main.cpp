
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cstring>

using namespace std;

class ClientData {
public:
    ClientData(int accNum = 0, const string& lname = "", const string& fname = "", double bal = 0.0)
        : accountNumber(accNum), balance(bal) {
        setLastName(lname);
        setFirstName(fname);
    }

    void setAccountNumber(int accNum) { accountNumber = accNum; }
    void setLastName(const string& lname) { strncpy(lastName, lname.c_str(), 15); }
    void setFirstName(const string& fname) { strncpy(firstName, fname.c_str(), 10); }
    void setBalance(double bal) { balance = bal; }

    int getAccountNumber() const { return accountNumber; }
    string getLastName() const { return lastName; }
    string getFirstName() const { return firstName; }
    double getBalance() const { return balance; }

private:
    int accountNumber;
    char lastName[15];
    char firstName[10];
    double balance;
};

int writeOption();
void printRecord(fstream&);
void updateRecord(fstream&);
void newRecord(fstream&);
void deleteRecord(fstream&);
void displayLine(ostream&, const ClientData&);
int getAccount(const char* const);

enum Options { PRINT = 1, UPDATE, NEW, DELETE, END };

int main() {
    fstream creditEntryExit("credit.dat", ios::in | ios::out | ios::binary);

    // Si el archivo no existe, crearlo con un registro inicial
    if (!creditEntryExit) {
        ofstream newFile("credit.dat", ios::out | ios::binary);
        if (!newFile) {
            cerr << "Could not create the file." << endl;
            exit(1);
        }

        // Crear un cliente inicial
        ClientData newClient(1234, "Doe", "John", 1000.50);
        newFile.write(reinterpret_cast<const char*>(&newClient), sizeof(ClientData));
        newFile.close();

        // Reabrir el archivo
        creditEntryExit.open("credit.dat", ios::in | ios::out | ios::binary);
    }

    if (!creditEntryExit) {
        cerr << "Could not open file." << endl;
        exit(1);
    }

    int option;

    while ((option = writeOption()) != END) {
        switch (option) {
            case PRINT:
                printRecord(creditEntryExit);
                break;
            case UPDATE:
                updateRecord(creditEntryExit);
                break;
            case NEW:
                newRecord(creditEntryExit);
                break;
            case DELETE:
                deleteRecord(creditEntryExit);
                break;
            default:
                cerr << "Incorrect option." << endl;
                break;
        }

        creditEntryExit.clear(); // Reset end-of-file flag
    }

    return 0;
}

int writeOption() {
    cout << "\nWrite your option" << endl
         << "1 - save a text file of the accounts with format" << endl
         << "2 - update an account" << endl
         << "3 - add a new account" << endl
         << "4 - delete an account" << endl
         << "5 - end the program\n? ";

    int menuOption;
    cin >> menuOption;
    return menuOption;
}

void printRecord(fstream& readFromFile) {
    ofstream printFileOutput("print.txt", ios::out);

    if (!printFileOutput) {
        cerr << "The file could not be created." << endl;
        exit(1);
    }

    printFileOutput << left << setw(10) << "Account" << setw(16)
                    << "Last Name" << setw(14) << "First Name" << right
                    << setw(10) << "Balance" << endl;

    readFromFile.seekg(0);
    ClientData client;
    while (readFromFile.read(reinterpret_cast<char*>(&client), sizeof(ClientData))) {
        if (client.getAccountNumber() != 0)
            displayLine(printFileOutput, client);
    }
}

void updateRecord(fstream& updateFile) {
    int accountNumber = getAccount("Enter the account number you want to update");

    updateFile.seekg((accountNumber - 1) * sizeof(ClientData));
    ClientData client;
    updateFile.read(reinterpret_cast<char*>(&client), sizeof(ClientData));

    if (client.getAccountNumber() != 0) {
        displayLine(cout, client);

        cout << "\nEnter charge (+) or credit (-): ";
        double transaction;
        cin >> transaction;

        double previousBalance = client.getBalance();
        client.setBalance(previousBalance + transaction);
        displayLine(cout, client);

        updateFile.seekp((accountNumber - 1) * sizeof(ClientData));
        updateFile.write(reinterpret_cast<const char*>(&client), sizeof(ClientData));
    } else {
        cerr << "Account #" << accountNumber << " contains no information." << endl;
    }
}

void newRecord(fstream& insertIntoFile) {
    int accountNumber = getAccount("Enter the new account number");

    insertIntoFile.seekg((accountNumber - 1) * sizeof(ClientData));
    ClientData client;
    insertIntoFile.read(reinterpret_cast<char*>(&client), sizeof(ClientData));

    if (client.getAccountNumber() == 0) {
        char lastName[15];
        char firstName[10];
        double balance;

        cout << "Enter last name, first name, and balance\n? ";
        cin >> setw(15) >> lastName;
        cin >> setw(10) >> firstName;
        cin >> balance;

        client.setLastName(lastName);
        client.setFirstName(firstName);
        client.setBalance(balance);
        client.setAccountNumber(accountNumber);

        insertIntoFile.seekp((accountNumber - 1) * sizeof(ClientData));
        insertIntoFile.write(reinterpret_cast<const char*>(&client), sizeof(ClientData));
    } else {
        cerr << "Account #" << accountNumber << " already contains information." << endl;
    }
}

void deleteRecord(fstream& deleteFromFile) {
    int accountNumber = getAccount("Enter the account number to delete");

    deleteFromFile.seekg((accountNumber - 1) * sizeof(ClientData));
    ClientData client;
    deleteFromFile.read(reinterpret_cast<char*>(&client), sizeof(ClientData));

    if (client.getAccountNumber() != 0) {
        ClientData blankClient;

        deleteFromFile.seekp((accountNumber - 1) * sizeof(ClientData));
        deleteFromFile.write(reinterpret_cast<const char*>(&blankClient), sizeof(ClientData));

        cout << "Account #" << accountNumber << " deleted.\n";
    } else {
        cerr << "Account #" << accountNumber << " is empty.\n";
    }
}

void displayLine(ostream& output, const ClientData& record) {
    output << left << setw(10) << record.getAccountNumber()
           << setw(16) << record.getLastName()
           << setw(10) << record.getFirstName()
           << setw(10) << setprecision(2) << fixed << showpoint << record.getBalance() << endl;
}

int getAccount(const char* const prompt) {
    int accountNumber;

    do {
        cout << prompt << " (1 - 100): ";
        cin >> accountNumber;
    } while (accountNumber < 1 || accountNumber > 100);

    return accountNumber;
}

