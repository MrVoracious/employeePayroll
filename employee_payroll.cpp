// Employee Payroll Management System (CIC-211, Mini Project 4)
// Saksham Shreyansh, Vandit Kad

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
using namespace std;

// salary rates
const double HRA_RATE    = 0.20;
const double DA_RATE     = 0.10;
const double PF_RATE     = 0.12;
const double PROF_TAX    = 200.0;
const double TRAVEL_RATE = 0.05;
const double TDS_RATE    = 0.10;
const double MIN_BASIC   = 1000.0;
const double MAX_BASIC   = 1000000.0;

// input helpers
string readLine(const string& prompt)
{
    string line;
    cout << prompt;
    if (!getline(cin, line))
    {
        cout << "\nInput ended. Exiting program.\n";
        exit(0);
    }
    return line;
}

string trim(const string& s)
{
    size_t start = s.find_first_not_of(" \t");
    if (start == string::npos)
        return "";
    size_t end = s.find_last_not_of(" \t");
    return s.substr(start, end - start + 1);
}

int readInt(const string& prompt, int minValue, int maxValue)
{
    while (true)
    {
        istringstream iss(readLine(prompt));
        int value;
        if (iss >> value && (iss >> ws).eof() && value >= minValue && value <= maxValue)
            return value;
        cout << "Invalid input. Enter a whole number from " << minValue
             << " to " << maxValue << ".\n";
    }
}

double readDouble(const string& prompt, double minValue, double maxValue)
{
    while (true)
    {
        istringstream iss(readLine(prompt));
        double value;
        if (iss >> value && (iss >> ws).eof() && value >= minValue && value <= maxValue)
            return value;
        cout << "Invalid input. Enter a number from " << fixed << setprecision(0)
             << minValue << " to " << maxValue << ".\n";
    }
}

string readText(const string& prompt)
{
    while (true)
    {
        string text = trim(readLine(prompt));
        if (!text.empty())
            return text;
        cout << "This field cannot be empty.\n";
    }
}

// base class
class Employee
{
private:
    int id;
    string name;
    string designation;
    double basicSalary;

protected:
    // salary slip parts
    void printHeader() const
    {
        cout << "\n------------------------------------------\n";
        cout << "             SALARY SLIP\n";
        cout << "------------------------------------------\n";
        cout << left << setw(16) << "Employee ID"  << ": " << id << "\n";
        cout << left << setw(16) << "Name"         << ": " << name << "\n";
        cout << left << setw(16) << "Designation"  << ": " << designation << "\n";
        cout << left << setw(16) << "Category"     << ": " << getCategory() << "\n";
        cout << "------------------------------------------\n";
        printLine("Basic Salary", basicSalary);
    }

    void printTotals() const
    {
        cout << "------------------------------------------\n";
        printLine("Gross Salary", calculateGross());
        printLine("Total Deductions", calculateDeductions());
        cout << "------------------------------------------\n";
        printLine("NET SALARY", calculateNet());
        cout << "------------------------------------------\n";
    }

    static void printLine(const string& label, double amount)
    {
        cout << left << setw(26) << label << ": Rs. "
             << right << setw(10) << fixed << setprecision(2) << amount << "\n";
    }

public:
    Employee(int empId, const string& empName, const string& empDesignation, double basic)
        : id(empId), name(empName), designation(empDesignation), basicSalary(basic)
    {
    }

    virtual ~Employee() {}

    int getId() const                  { return id; }
    string getName() const             { return name; }
    string getDesignation() const      { return designation; }
    double getBasicSalary() const      { return basicSalary; }
    void setBasicSalary(double basic)  { basicSalary = basic; }

    // overridden by each category
    virtual string getCategory() const = 0;
    virtual double calculateAllowances() const = 0;
    virtual double calculateDeductions() const = 0;
    virtual void displaySalarySlip() const = 0;

    double calculateGross() const { return basicSalary + calculateAllowances(); }
    double calculateNet() const   { return calculateGross() - calculateDeductions(); }
};

// permanent employee
class PermanentEmployee : public Employee
{
public:
    PermanentEmployee(int empId, const string& empName, const string& empDesignation, double basic)
        : Employee(empId, empName, empDesignation, basic)
    {
    }

    string getCategory() const { return "Permanent"; }

    double calculateAllowances() const
    {
        return getBasicSalary() * (HRA_RATE + DA_RATE);
    }

    double calculateDeductions() const
    {
        return getBasicSalary() * PF_RATE + PROF_TAX;
    }

    void displaySalarySlip() const
    {
        printHeader();
        printLine("HRA (20% of basic)", getBasicSalary() * HRA_RATE);
        printLine("DA (10% of basic)", getBasicSalary() * DA_RATE);
        cout << "------------------------------------------\n";
        printLine("PF (12% of basic)", getBasicSalary() * PF_RATE);
        printLine("Professional Tax", PROF_TAX);
        printTotals();
    }
};

// contract employee
class ContractEmployee : public Employee
{
public:
    ContractEmployee(int empId, const string& empName, const string& empDesignation, double basic)
        : Employee(empId, empName, empDesignation, basic)
    {
    }

    string getCategory() const { return "Contract"; }

    double calculateAllowances() const
    {
        return getBasicSalary() * TRAVEL_RATE;
    }

    double calculateDeductions() const
    {
        return calculateGross() * TDS_RATE;
    }

    void displaySalarySlip() const
    {
        printHeader();
        printLine("Travel Allow. (5% basic)", getBasicSalary() * TRAVEL_RATE);
        cout << "------------------------------------------\n";
        printLine("TDS (10% of gross)", calculateGross() * TDS_RATE);
        printTotals();
    }
};

// employee list and menu actions
class PayrollManager
{
private:
    vector<Employee*> employees;

    int findIndex(int id) const
    {
        for (size_t i = 0; i < employees.size(); i++)
        {
            if (employees[i]->getId() == id)
                return static_cast<int>(i);
        }
        return -1;
    }

public:
    PayrollManager() {}

    ~PayrollManager()
    {
        for (size_t i = 0; i < employees.size(); i++)
            delete employees[i];
    }

    // no copying
    PayrollManager(const PayrollManager&) = delete;
    PayrollManager& operator=(const PayrollManager&) = delete;

    void addEmployee(bool permanent)
    {
        cout << "\n--- Add " << (permanent ? "Permanent" : "Contract") << " Employee ---\n";

        int id = readInt("Enter employee ID (positive number): ", 1, 999999);
        if (findIndex(id) != -1)
        {
            cout << "Error: an employee with ID " << id << " already exists.\n";
            return;
        }

        string name        = readText("Enter name: ");
        string designation = readText("Enter designation: ");
        double basic       = readDouble("Enter basic salary (Rs. 1000 to 1000000): ",
                                        MIN_BASIC, MAX_BASIC);

        if (permanent)
            employees.push_back(new PermanentEmployee(id, name, designation, basic));
        else
            employees.push_back(new ContractEmployee(id, name, designation, basic));

        cout << "Employee added successfully.\n";
    }

    void displayAll() const
    {
        if (employees.empty())
        {
            cout << "\nNo employee records available.\n";
            return;
        }

        cout << "\n" << left
             << setw(7)  << "ID"
             << setw(18) << "Name"
             << setw(11) << "Category"
             << right
             << setw(11) << "Basic"
             << setw(11) << "Allowance"
             << setw(11) << "Gross"
             << setw(11) << "Deduction"
             << setw(11) << "Net" << "\n";
        cout << string(91, '-') << "\n";

        double totalNet = 0;
        for (size_t i = 0; i < employees.size(); i++)
        {
            const Employee* e = employees[i];
            string shortName = e->getName().substr(0, 17);
            cout << left
                 << setw(7)  << e->getId()
                 << setw(18) << shortName
                 << setw(11) << e->getCategory()
                 << right << fixed << setprecision(2)
                 << setw(11) << e->getBasicSalary()
                 << setw(11) << e->calculateAllowances()
                 << setw(11) << e->calculateGross()
                 << setw(11) << e->calculateDeductions()
                 << setw(11) << e->calculateNet() << "\n";
            totalNet += e->calculateNet();
        }
        cout << string(91, '-') << "\n";
        cout << "Total employees: " << employees.size()
             << "    Total net payout: Rs. " << fixed << setprecision(2) << totalNet << "\n";
    }

    void searchEmployee() const
    {
        int id = readInt("\nEnter employee ID to search: ", 1, 999999);
        int index = findIndex(id);
        if (index == -1)
        {
            cout << "Employee with ID " << id << " not found.\n";
            return;
        }
        employees[index]->displaySalarySlip();
    }

    void updateBasicSalary()
    {
        int id = readInt("\nEnter employee ID to update: ", 1, 999999);
        int index = findIndex(id);
        if (index == -1)
        {
            cout << "Employee with ID " << id << " not found.\n";
            return;
        }
        cout << "Current basic salary: Rs. " << fixed << setprecision(2)
             << employees[index]->getBasicSalary() << "\n";
        double basic = readDouble("Enter new basic salary (Rs. 1000 to 1000000): ",
                                  MIN_BASIC, MAX_BASIC);
        employees[index]->setBasicSalary(basic);
        cout << "Basic salary updated. New net salary: Rs. " << fixed << setprecision(2)
             << employees[index]->calculateNet() << "\n";
    }

    void removeEmployee()
    {
        int id = readInt("\nEnter employee ID to remove: ", 1, 999999);
        int index = findIndex(id);
        if (index == -1)
        {
            cout << "Employee with ID " << id << " not found.\n";
            return;
        }
        delete employees[index];
        employees.erase(employees.begin() + index);
        cout << "Employee record removed.\n";
    }
};

void showMenu()
{
    cout << "\n========================================\n";
    cout << "   EMPLOYEE PAYROLL MANAGEMENT SYSTEM\n";
    cout << "========================================\n";
    cout << "1. Add permanent employee\n";
    cout << "2. Add contract employee\n";
    cout << "3. Display all employees\n";
    cout << "4. Search employee and show salary slip\n";
    cout << "5. Update basic salary\n";
    cout << "6. Remove employee\n";
    cout << "0. Exit\n";
}

int main()
{
    PayrollManager manager;
    int choice;

    do
    {
        showMenu();
        choice = readInt("Enter your choice: ", 0, 6);

        switch (choice)
        {
            case 1: manager.addEmployee(true);   break;
            case 2: manager.addEmployee(false);  break;
            case 3: manager.displayAll();        break;
            case 4: manager.searchEmployee();    break;
            case 5: manager.updateBasicSalary(); break;
            case 6: manager.removeEmployee();    break;
            case 0: cout << "\nThank you. Exiting program.\n"; break;
        }
    } while (choice != 0);

    return 0;
}
