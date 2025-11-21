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

