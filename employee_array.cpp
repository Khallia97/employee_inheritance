#include <iostream>
#include <iomanip>
#include <string>
#include <memory>
using namespace std;

// base class
class Employee {
protected:
    string fname;
    string lname;
    double hrlyPay;

public:
    Employee(string first, string last, double pay)
        : fname(first), lname(last), hrlyPay(pay) {}

    virtual ~Employee() {}

    string getFullName() const {
        return lname + ", " + fname;
    }

    double getHrlyPay() const {
        return hrlyPay;
    }

    void raiseWages(double percent) {
        hrlyPay += hrlyPay * (percent / 100.0);
    }

    virtual void computePay() const = 0;
    virtual void listInfo() const = 0;
};

// hourly employee
class HourlyEmployee : public Employee {
private:
    double hours;

public:
    HourlyEmployee(string first, string last, double pay, double hrs)
        : Employee(first, last, pay), hours(hrs) {}

    void computePay() const override {
        double weeklyPay;
        if (hours > 40)
            weeklyPay = (40 * hrlyPay) + ((hours - 40) * hrlyPay * 1.5);
        else
            weeklyPay = hours * hrlyPay;

        cout << getFullName() << endl;
        cout << "Weekly Pay: $" << fixed << setprecision(2)
             << weeklyPay << endl << endl;
    }

    void listInfo() const override {
        cout << left << setw(20) << getFullName()
             << setw(15) << fixed << setprecision(2)
             << hrlyPay << "/hour"
             << setw(10) << "-" << endl;
    }
};

// salaried employee
class SalariedEmployee : public Employee {
private:
    double bonusAmt;
    double annualSalary;

public:
    SalariedEmployee(string first, string last, double salary, double bonus)
        : Employee(first, last, (salary / 52.0 / 40.0)),
          bonusAmt(bonus), annualSalary(salary) {}

    void computePay() const override {
        double weeklyPay = hrlyPay * 40;
        cout << getFullName() << endl;
        cout << "Weekly Pay: $" << fixed << setprecision(2)
             << weeklyPay << endl << endl;
    }

    void raiseWages(double percent) {
        annualSalary += annualSalary * (percent / 100.0);
        hrlyPay = annualSalary / 52.0 / 40.0;
    }

    void listInfo() const override {
        cout << left << setw(20) << getFullName()
             << setw(15) << fixed << setprecision(2)
             << hrlyPay << "/year"
             << "$" << bonusAmt << endl;
    }

    double getAnnualSalary() const { return annualSalary; }
};

// display menu
void displayMenu() {
    cout << "N: New employee\n";
    cout << "C: Compute paychecks\n";
    cout << "R: Raise wages\n";
    cout << "L: List all employees\n";
    cout << "Q: Quit\n\n";
    cout << "Enter command: ";
}

// main 
int main() {
    Employee* employees[50];  // array of pointers to Employee
    int count = 0;             // keeps track of how many employees exist
    char command;

    do {
        displayMenu();
        cin >> command;
        command = tolower(command);
        cin.ignore();

        if (command == 'n') {
            if (count >= 50) {
                cout << "Employee list full.\n";
                continue;
            }

            string fullName;
            cout << "Enter name of new employee (Last, First): ";
            getline(cin, fullName);

            // split into last, first
            string lname, fname;
            size_t commaPos = fullName.find(',');
            if (commaPos != string::npos) {
                lname = fullName.substr(0, commaPos);
                fname = fullName.substr(commaPos + 1);
                if (fname.size() && fname[0] == ' ')
                    fname.erase(0, 1); // remove leading space
            } else {
                lname = fullName;
                fname = "";
            }

            char type;
            cout << "Hourly (h) or salaried (s): ";
            cin >> type;

           // create the appropriate object and store it in array
            if (tolower(type) == 'h') {
                double rate, hrs;
                cout << "Enter hourly wage: ";
                cin >> rate;
                cout << "Enter weekly hours: ";
                cin >> hrs;
                employees[count++] = new HourlyEmployee(fname, lname, rate, hrs);
            } else {
                double salary, bonus;
                cout << "Enter annual salary: ";
                cin >> salary;
                cout << "Enter yearly bonus amount: ";
                cin >> bonus;
                employees[count++] = new SalariedEmployee(fname, lname, salary, bonus);
            }
            cout << endl;
        }

        else if (command == 'c') {
            for (int i = 0; i < count; i++)
                employees[i]->computePay();
        }

        else if (command == 'r') {
            double percent;
            cout << "Enter percentage increase: ";
            cin >> percent;
            cout << "\nName\t\t\tNew Wages\n";
            cout << "------------------------------\n";

            // loop through and apply raise based on type
            for (int i = 0; i < count; i++) {
                SalariedEmployee* sEmp = dynamic_cast<SalariedEmployee*>(employees[i]);    // check if salaried employee using dynamic_cast
                if (sEmp) {
                    sEmp->raiseWages(percent);
                    cout << sEmp->getFullName() << "\t"
                         << fixed << setprecision(2)
                         << sEmp->getAnnualSalary() << "/year" << endl;
                } else {
                    employees[i]->raiseWages(percent);
                    cout << employees[i]->getFullName() << "\t"
                         << fixed << setprecision(2)
                         << employees[i]->getHrlyPay() << "/hour" << endl;
                }
            }
            cout << endl;
        }

        else if (command == 'l') {
            cout << left << setw(20) << "Name"
                 << setw(15) << "Hourly Wages"
                 << setw(10) << "Bonus" << endl;
            cout << "---------------------------------------------\n";

            for (int i = 0; i < count; i++)
                employees[i]->listInfo();

            cout << endl;
        }

    } while (command != 'q');    // repeat until user quits

    // cleanup
    for (int i = 0; i < count; i++)
        delete employees[i];

    cout << "Program terminated.\n";
    return 0;
}
