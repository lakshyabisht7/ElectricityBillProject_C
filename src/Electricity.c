/*
PROJECT TOPIC: 
Electricity Bill Generator(with Different slabs)

PROJECT OVERVIEW:
In this project, based on C programming Language, I am working on generating a realistic Electicity Monthly Bill,as per 
the Current Tariff and all Other Charges applicable for Residential connection under UPCL in Dehradun, Uttarakhand.
Apart from calculating slab-wise energy charges, the program also stores customer details, allows searching, and displays 
a formatted bill. File handling is used so that customer records remain saved even after the program is closed.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Structure to store customer details and bill components
typedef struct 
{
    int id;
    char name[50];
    float units;
    float load;
    float energyCharge;
    float fixedCharge;
    float duty;
    float surcharge;
    float totalBill;
} Customer;

Customer customers[100]; 
int count = 0;           

//Calculates bill breakdown and total bill
float calculateBill(float units, float load, int latePayment, Customer *c) 
{
    float energyCharge = 0, totalBill = 0, surcharges = 0, fixedCharge = 0, duty=0;

    // Fixed charge based on load capacity
    if (load <= 1)
    fixedCharge = 75.0 * load;
    else if (load <= 4)
    fixedCharge = 85.0 * load; 
    else 
    fixedCharge = 100 * load;
