/*
PROJECT TOPIC: 
Electricity Bill Generator(with Different slabs)

PROJECT OVERVIEW:
In this project, based on C programming Language, I am working on generating a realistic Electicity Monthly Bill, as per the Current Tariff 
and all Other Charges applicable for Residential connection under UPCL in Dehradun, Uttarakhand. Apart from calculating slab-wise energy charges,
the program also stores customer details, allows searching, and displays a formatted bill. File handling is used so that customer records 
remain saved even after the program is closed and opened again. Basic search options (by ID and by name) are also provided for ease of usage.
Concepts Used: Structures, File Handling, Dynamic Memory allocation, Functions, Pointers, Searching.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Structure to store customer information
typedef struct 
{
    int id;
    char name[50];
    float units;
    float load; //Connected load in kW
    float energyCharge;
    float fixedCharge;
    float duty; //Electricity duty
    float surcharge;
    float totalBill;
} Customer;

Customer *customers = NULL;
int count=0;           

//Function to save data to file
void saveToFile()
{
    if(customers == NULL)
    return;

    FILE *fp=fopen("customers.dat","wb");
    if(fp == NULL)
    {
        printf("Error: Cannot save file.\n");
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
    return;

    fread(&count, sizeof(int), 1, fp);

    customers = (Customer *)malloc(count * sizeof(Customer));
    if (customers == NULL)
    {
        printf("Memory allocation failed\n");
        fclose(fp);
        exit(1);
    }

    fread(customers, sizeof(Customer), count, fp);

    fclose(fp);
}

//Calculates bill breakdown and total bill
void calculateBill(float units, float load, int latePayment, Customer *c) 
{
    float energyCharge=0, totalBill=0, surcharges=0, fixedCharge=0, duty=0;

    //Fixed charge based on load capacity
    if(load <= 1)
    fixedCharge = 75.0 * load; //75/kW for upto 1kW
    else if(load <= 4)
    fixedCharge = 85.0 * load; //85/kW for upto 4kW
    else 
    fixedCharge = 100 * load; //100/kW for greater than 4kW

    //Slab-wise energy charge calculation
    if(units <= 100) //First 100 units @3.65/unit
    {
        energyCharge = units * 3.65;
    } 
    else if(units <= 200) //Next 100 units @5.25/unit, including fixed charge for first 100 units
    {
        energyCharge = (100 * 3.65) + ((units - 100) * 5.25);
    } 
    else if(units <= 400) //Next 200 units @7.15/unit, including fixed charges for first 200 units
    {
        energyCharge = (100 * 3.65) + (100 * 5.25) + ((units - 200) * 7.15);
    } 
    else //After 400 units @7.80/unit, including fixed charges for first 400 units
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
}

//Function to add a new customer
void addCustomer() 
{
    Customer c;
    int latePayment;
    int tempChar;

    customers = (Customer *)realloc(customers, (count + 1) * sizeof(Customer));
    if (customers == NULL)
    {
        printf("Memory error\n");
        exit(1);
    }
    
    printf("\n ADD NEW CUSTOMER \n");

    printf("Enter Customer ID: ");
    scanf("%d", &c.id);
    for(int i=0; i<count; i++)
    {   
        //Duplicate ID check
        if(customers[i].id == c.id)
        {
            printf("Error: Customer ID already exists.\n");
            return;
        }
    }

    //Clear the "Enter" key left by scanf so fgets works
    while ((tempChar=getchar()) != '\n' && tempChar != EOF);

    printf("Enter Customer Name: ");
    fgets(c.name, sizeof(c.name), stdin);
    //Removes new line at the end of name
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

    //Calculate bill and save it to structure
    calculateBill(c.units, c.load, latePayment, &c);

    customers[count]=c; 
    count++;

    saveToFile(); //Save input data to file

    printf("Customer added successfully.\n");
}

//Function to search customer records by ID
void searchByID()
{
    int id, found=0;
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

    if(found == 0)
    printf("Customer not found.\n");
}

//Function to search customer records by name 
void searchByName()
{
    char searchName[50];
    int found=0;
    int tempChar;

    while ((tempChar=getchar()) != '\n' && tempChar != EOF);

    printf("Enter Customer Name to search: ");
    fgets(searchName, sizeof(searchName), stdin);
    searchName[strcspn(searchName, "\n")]='\0';

    for(int i=0; i<count; i++)
    {
        if(strcmp(customers[i].name, searchName) == 0)
        {
            printf("\nCustomer Found:\n");
            printf("ID: %d\n", customers[i].id);
            printf("Name: %s\n", customers[i].name);
            printf("Units: %.2f\n", customers[i].units);
            printf("Load: %.2f kW\n", customers[i].load);
            printf("Total Bill: INR %.2f\n", customers[i].totalBill);

            found=1;
        }
    }
    
    if(found == 0)
    printf("No customer found with the entered name.\n");
}

//Display formatted electricity bill
void generateReceipt()
{
    int id, found=0;
    printf("Enter Customer ID to generate receipt: ");
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
            printf("\n      Bill Breakdown\n");
            printf("Energy Charge : INR %.2f\n", customers[i].energyCharge);
            printf("Fixed Charge  : INR %.2f\n", customers[i].fixedCharge);
            printf("Duty (15%%)    : INR %.2f\n", customers[i].duty);
            printf("Surcharge     : INR %.2f\n", customers[i].surcharge);
            printf("---------------------------------------------\n");
            printf("TOTAL BILL    : INR %.2f\n", customers[i].totalBill);
            printf("---------------------------------------------\n");
            printf("\nThank you for using UPCL Billing System\n");

            found=1;
            break;
        }
    }

    if(found == 0)
    printf("Customer data not found\n");
}

//Main menu for users to interact with available options 
int main() 
{
    loadFromFile(); //Load customer data from file

    int choice;
    while(1)
    {
        printf("\n\nELECTRICITY BILL SYSTEM MENU:\n");
        printf("1. Add New Customer\n");
        printf("2. Generate Electricity Bill\n");
        printf("3. Search customer records by ID\n");
        printf("4. Search customer records by Name\n");
        printf("5. Exit\n");

        printf("\nPlease select an option: ");
        if(scanf("%d", &choice) != 1) //If customer did not entered a number
        {
            printf("Invalid input. Please enter number (1-5)\n");
            while (getchar() != '\n'); 
            continue; //Go back to the start of the while loop
        }

        //switch case to call functions based on user's choice
        switch(choice) 
        {
            case 1:
                addCustomer();
                break;

            case 2:
                generateReceipt();
                break;

            case 3:
                searchByID();
                break;

            case 4:
                searchByName();
                break;

            case 5:
                saveToFile();
                free(customers);
                printf("Exiting program...\n");
                exit(0);

            default:
                printf("Invalid choice...Try again\n");
        }
    }

    return 0;
}