/*
PROJECT TOPIC: 
Electricity Bill Generator(with Different slabs)

PROJECT OVERVIEW:
In this project, based on C programming Language, I am working on generating a realistic Electicity Monthly Bill,
as per the Current Tariff and all Other Charges applicable for Residential connection under UPCL in Dehradun, Uttarakhand.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
