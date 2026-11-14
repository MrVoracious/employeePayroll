# Employee Payroll Management System

Mini Project 4, OOP using C++ (CIC-211)

By Saksham Shreyansh and Vandit Kad

A console program that stores employee details and calculates gross and net salary. Permanent and contract employees have different salary rules.

## Features

- Add permanent or contract employees (duplicate IDs not allowed)
- Display all employees
- Search by ID and show a salary slip
- Update basic salary
- Remove an employee

## Salary rules

- Permanent: HRA 20% and DA 10% of basic. PF 12% of basic and Rs. 200 professional tax deducted.
- Contract: travel allowance 5% of basic. TDS 10% of gross deducted.

## Classes

- Employee (abstract base class)
- PermanentEmployee and ContractEmployee (derived classes)
- PayrollManager (stores employees and handles the menu)

## Run

Open `employee_payroll.cpp` in VS Code and run it.
