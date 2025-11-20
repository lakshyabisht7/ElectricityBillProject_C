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

// Bill Calculation Logic
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

    //Energy charges
    if (units <= 100) 
    {
        energyCharge = units * 3.65;
    } 
    else if (units <= 200) 
    {
        energyCharge = (100 * 3.65) + ((units - 100) * 5.25);
    } 
    else if (units <= 400) 
    {
        energyCharge = (100 * 3.65) + (100 * 5.25) + ((units - 200) * 7.15);
    } 
    else 
    {
        energyCharge = (100 * 3.65) + (100 * 5.25) + (200 * 7.15) + ((units - 400) * 7.80);
    }

    // Duty charges 15% of energy charges
    duty = energyCharge * 0.15; 

    // Surcharges  Rs. 0.10 per unit
    surcharges = units * 0.10; 

    // Total bill 
    totalBill = energyCharge + fixedCharge + duty + surcharges ;

    // Late payment fee 1.25%
    if (latePayment == 1)
    totalBill += totalBill * 0.0125;

    //Store in struct
    c->energyCharge = energyCharge;
    c->fixedCharge = fixedCharge;
    c->duty = duty;
    c->surcharge = surcharges;
    c->totalBill = totalBill;

    return totalBill;
}
