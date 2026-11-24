/*
 ============================================================================
  PROJECT NAME : SUPER MART BILLING & INVENTORY MANAGEMENT SYSTEM
  LANGUAGE     : C (Standard C99 / ANSI C)
  PURPOSE      : Semester Project for Programming Fundamentals / Structured C
  DESIGN       : Simple, flat structures (no complex pointers, no linked lists)
                 High compatibility with MinGW, GCC, Dev-C++, Code::Blocks
 ============================================================================
*/

#include "header.h"

struct Product inventory[MAX_PRODUCTS];
int productCount = 0;
char adminPassword[30] = "admin123";

 // MAIN FUNCTION

int main() {
    int choice;

    // Load admin password and inventory from disk
    loadAdminPassword();
    loadInventory();

    while (1) {
        clearScreen();
        displayBanner();

        printf("  +-------------------------------------------------------------+\n");
        printf("  |                      MAIN CONTROL PANEL                     |\n");
        printf("  +-------------------------------------------------------------+\n");
        printf("  |  [1] POS Billing Counter (Cashier / New Sale)               |\n");
        printf("  |  [2] Customer Price Checker & Item Search                   |\n");
        printf("  |  [3] View All Available Products in Mart                    |\n");
        printf("  |  [4] Admin & Inventory Management (Password Protected)      |\n");
        printf("  |  [5] Exit Application                                       |\n");
        printf("  +-------------------------------------------------------------+\n");
        printf("   Enter your choice (1-5): ");

        choice = safeGetInt();

        switch (choice) {
            case 1:
                startBillingPOS();
                break;
            case 2:
                priceChecker();
                break;
            case 3:
                clearScreen();
                displayBanner();
                displayAllProducts();
                pauseScreen();
                break;
            case 4:
                if (adminLogin()) {
                    adminMenu();
                } else {
                    printf("\n  [!] Access Denied: Incorrect Password.\n");
                    pauseScreen();
                }
                break;
            case 5:
                clearScreen();
                displayBanner();
                printf("  Saving current inventory to database...\n");
                saveInventory();
                printf("  Thank you for using Super Mart Billing System!\n");
                printf("  Have a great day!\n\n");
                return 0;
            default:
                printf("\n  [!] Invalid choice! Please select between 1 and 5.\n");
                pauseScreen();
        }
    }

}
//LOGIN & MENU
int adminLogin() {
    clearScreen();
    displayBanner();
    printf("  +-------------------------------------------------------------+\n");
    printf("  |                     ADMIN AUTHENTICATION                    |\n");
    printf("  +-------------------------------------------------------------+\n");
    printf("   Enter Admin Password: ");

    char enteredPass[30];
    safeGetString(enteredPass, sizeof(enteredPass));

    if (strcmp(enteredPass, adminPassword) == 0) {
        return 1;
    }
    return 0;
}

void changeAdminPassword() {
    clearScreen();
    displayBanner();
    printf("  +-------------------------------------------------------------+\n");
    printf("  |                    CHANGE ADMIN PASSWORD                    |\n");
    printf("  +-------------------------------------------------------------+\n");

    printf("   Enter CURRENT password: ");
    char currPass[30];
    safeGetString(currPass, sizeof(currPass));

    if (strcmp(currPass, adminPassword) != 0) {
        printf("  [!] Current password incorrect!\n");
        pauseScreen();
        return;
    }

    printf("   Enter NEW password (up to 25 characters): ");
    char newPass[30];
    safeGetString(newPass, sizeof(newPass));

    printf("   Confirm NEW password: ");
    char confPass[30];
    safeGetString(confPass, sizeof(confPass));

    if (strcmp(newPass, confPass) == 0 && strlen(newPass) > 0) {
        strcpy(adminPassword, newPass);
        saveAdminPassword();
        printf("  [+] Success: Admin password updated and saved.\n");
    } else {
        printf("  [!] Passwords do not match or empty. Update failed.\n");
    }

    pauseScreen();
}


void adminMenu() {
    int choice;
    while (1) {
        clearScreen();
        displayBanner();
        printf("  +-------------------------------------------------------------+\n");
        printf("  |                   ADMINISTRATIVE DASHBOARD                  |\n");
        printf("  +-------------------------------------------------------------+\n");
        printf("  |  [1] View Complete Product Inventory                        |\n");
        printf("  |  [2] Add New Product to Mart                                |\n");
        printf("  |  [3] Update / Restock Existing Product                      |\n");
        printf("  |  [4] Delete Product from Inventory                          |\n");
        printf("  |  [5] Check Low Stock Warning Alerts (< 5 units)             |\n");
        printf("  |  [6] Search Product Database                                |\n");
        printf("  |  [7] View Sales Transaction Log                             |\n");
        printf("  |  [8] View Financial Revenue Summary                         |\n");
        printf("  |  [9] Change Admin Password                                  |\n");
        printf("  |  [0] Return to Main Menu                                    |\n");
        printf("  +-------------------------------------------------------------+\n");
        printf("   Enter Admin Choice (0-9): ");

        choice = safeGetInt();

        switch (choice) {
            case 1:
                clearScreen();
                displayBanner();
                displayAllProducts();
                pauseScreen();
                break;
            case 2:
                addNewProduct();
                break;
            case 3:
                updateProduct();
                break;
            case 4:
                deleteProduct();
                break;
            case 5:
                checkLowStockAlerts();
                break;
            case 6:
                searchProduct();
                break;
            case 7:
                viewSalesHistory();
                break;
            case 8:
                viewFinancialSummary();
                break;
            case 9:
                changeAdminPassword();
                break;
            case 0:
                return;
            default:
                printf("\n  [!] Invalid choice! Enter 0 to 9.\n");
                pauseScreen();
        }
    }
}








