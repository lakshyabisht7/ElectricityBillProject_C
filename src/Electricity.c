/*
PROJECT TOPIC: 
Electricity Bill Generator(with Different slabs)

PROJECT OVERVIEW:
In this project, based on C programming Language, I am working on generating a realistic Electicity Monthly Bill, as per the Current Tariff 
and all Other Charges applicable for Residential connection under UPCL in Dehradun, Uttarakhand. Apart from calculating slab-wise energy charges,
the program also stores customer details, allows searching, and displays a formatted bill. File handling is used so that customer records 
remain saved even after the program is closed and opened again. Basic search options (by ID and by name) are also provided for ease of usage.

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
int count=0;           

//Save customer details and bill records to file
void saveToFile()
{
    FILE *fp=fopen("customers.dat","wb");
    if(fp == NULL)
    {
        printf("Error while saving data.\n");
        return;
    }
    fwrite(&count, sizeof(int), 1, fp);
    fwrite(customers, sizeof(Customer), count, fp);

    fclose(fp);
}

// Load data from file when program starts
void loadFromFile()
{
    FILE *fp = fopen("customers.dat","rb");
    if(fp == NULL)
    {
        return;
    }
    fread(&count, sizeof(int), 1, fp);
    fread(customers, sizeof(Customer), count, fp);

    fclose(fp);
}

//Calculates bill breakdown and total bill
float calculateBill(float units, float load, int latePayment, Customer *c) 
{
    float energyCharge=0, totalBill=0, surcharges=0, fixedCharge=0, duty=0;

    //Fixed charge based on load capacity
    if(load <= 1)
    fixedCharge = 75.0 * load;
    else if(load <= 4)
    fixedCharge = 85.0 * load; 
    else 
    fixedCharge = 100 * load;

    //Slab-wise energy charge calculation
    if(units <= 100) 
    {
        energyCharge = units * 3.65;
    } 
    else if(units <= 200) 
    {
        energyCharge = (100 * 3.65) + ((units - 100) * 5.25);
    } 
    else if(units <= 400) 
    {
        energyCharge = (100 * 3.65) + (100 * 5.25) + ((units - 200) * 7.15);
    } 
    else 
    {
        energyCharge = (100 * 3.65) + (100 * 5.25) + (200 * 7.15) + ((units - 400) * 7.80);
    }

    //Duty charges 15% of energy charges
    duty = energyCharge * 0.15; 

    //Surcharges  Rs. 0.10 per unit
    surcharges = units * 0.10; 

    //Sum of all components = Total bill 
    totalBill = energyCharge + fixedCharge + duty + surcharges ;

    //Late payment fee 1.25% (if applicable)
    if(latePayment == 1)
    totalBill += totalBill * 0.0125;

    //Store all calculated components back in structure
    c->energyCharge = energyCharge;
    c->fixedCharge = fixedCharge;
    c->duty = duty;
    c->surcharge = surcharges;
    c->totalBill = totalBill;

    return totalBill;
}

//Input new customer details and billing details
void addCustomer() 
{
    Customer c;
    int latePayment;

    if(count >= 100) //Check storage limit
    {
        printf("Adding customer unsuccessful. Customer limit reached.\n");
        return;
    }
    
    printf("Enter Customer ID: ");
    scanf("%d", &c.id);
    for(int i=0; i<count; i++)
    {   
        //Duplicate ID check
        if(customers[i].id == c.id)
        {
            printf("Customer ID already exists.\n");
            return;
        }
    }
    
    printf("Enter Customer Name: ");
    getchar(); 
    fgets(c.name, sizeof(c.name), stdin);
    c.name[strcspn(c.name,"\n")]='\0';

    printf("Enter Units Consumed: ");
    scanf("%f", &c.units);
    if(c.units < 0) 
    {
        printf("Invalid input. Units cannot be less than zero.\n");
        return;
    }

    printf("Enter Load Capacity (in kW): ");
    scanf("%f", &c.load);
    if(c.load <= 0) 
    {
        printf("Invalid input. Load capacity must be greater than zero.\n");
        return;
    }

    printf("Is it late payment? (1=yes, 0=no): ");
    scanf("%d", &latePayment);
    if(latePayment != 0 && latePayment != 1) 
    {
        printf("Invalid input. Please enter 1(late payment) or not late(0).\n");
        return;
    }

    calculateBill(c.units, c.load, latePayment, &c);

    customers[count]=c; 
    count++;

    saveToFile(); //Save input data to file

    printf("Customer added successfully.\n");
}

//Search customer records by ID
void searchByID()
{
    int id, f=0;
    printf("Enter Customer ID to search: ");
    scanf("%d", &id);
    
    for(int i=0; i<count; i++)
    {
        if(customers[i].id == id)
        {
            printf("\nCustomer Found:\n");
            printf("ID: %d\n", customers[i].id);
            printf("Name: %s\n", customers[i].name);
            printf("Units: %.2f\n", customers[i].units);
            printf("Load: %.2f kW\n", customers[i].load);
            printf("Total Bill: INR %.2f\n", customers[i].totalBill);
            found=1;
            break;
        }
    }

    if(f == 0)
    printf("Customer not found.\n");
}

//Search customer records by name 
void searchByName()
{
    char name[50];
    int f=0;

    printf("Enter Customer Name to search: ");
    getchar();
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")]='\0';

    for(int i=0; i<count; i++)
    {
        if(strcmp(customers[i].name, name) == 0)
        {
            printf("\nCustomer Found:\n");
            printf("ID: %d\n", customers[i].id);
            printf("Name: %s\n", customers[i].name);
            printf("Units: %.2f\n", customers[i].units);
            printf("Load: %.2f kW\n", customers[i].load);
            printf("Total Bill: INR %.2f\n", customers[i].totalBill);

            f=1;
        }
    }
    
    if(f == 0)
    printf("No customer found with the entered name.\n");
}

//Display formatted electricity bill
void generateReceipt()
{
    int id, f=0;
    printf("Enter Customer ID to generate receipt:\n");
    scanf("%d", &id);

    for (int i=0; i<count; i++)
    {
        if (customers[i].id == id)
        {
            printf("---------------------------------------------\n");
            printf("  UTTARAKHAND POWER CORPORATION LTD. (UPCL) \n\n");
            printf("Customer ID   : %d\n", customers[i].id);
            printf("Customer Name : %s\n", customers[i].name);
            printf("Connected Load: %.2f kW\n", customers[i].load);
            printf("Units Consumed: %.2f units\n", customers[i].units);
            printf("\n  Bill Breakdown\n");
            printf("Energy Charge : INR %.2f\n", customers[i].energyCharge);
            printf("Fixed Charge  : INR %.2f\n", customers[i].fixedCharge);
            printf("Duty (15%%)    : INR %.2f\n", customers[i].duty);
            printf("Surcharge     : INR %.2f\n", customers[i].surcharge);
            printf("---------------------------------------------\n");
            printf("TOTAL BILL    : INR %.2f\n", customers[i].totalBill);
            printf("---------------------------------------------\n");

            found=1;
            break;
        }
    }
}

// Main menu for users to interact with available options 
int main() 
{
    loadFromFile(); // Load customer data from file

    int choice;
}
