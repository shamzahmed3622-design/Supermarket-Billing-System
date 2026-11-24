
#ifndef MART_BILLING_SYSTEM_HEADER_H
#define MART_BILLING_SYSTEM_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_PRODUCTS 100
#define MAX_CART 50
#define INVENTORY_FILE "inventory.txt"
#define SALES_FILE "sales_history.txt"
#define CONFIG_FILE "admin_config.txt"
#define TAX_RATE 0.05f  // 5% Sales Tax / GST

/* ============================================================================
                            DATA STRUCTURES
   (Kept intentionally simple & flat for semester project evaluation)
   ============================================================================ */

// 1. Structure for Mart Product
struct Product{
    int id;
    char name[50];
    char category[30];
    float price;
    int stock;
};

// 2. Structure for Billing Cart Item
struct CartItem {
    int id;
    char name[50];
    float price;
    int quantity;
    float subtotal;
};

// 3. Structure for Sales Record

struct SalesRecord {
    int billNo;
    char customerName[50];
    char customerPhone[20];
    char dateStr[30];
    int itemCount;
    float subtotal;
    float discountAmount;
    float taxAmount;
    float grandTotal;
    char paymentMethod[20];
};

/* ============================================================================
                            GLOBAL VARIABLES
   ============================================================================ */
extern struct Product inventory[MAX_PRODUCTS];
extern int productCount;
extern char adminPassword[30];

/* ============================================================================
                          FUNCTION DECLARATIONS
   ============================================================================ */
// Utility & UI Helpers
void clearScreen();
void pauseScreen();
void displayBanner();
void clearInputBuffer();
void removeNewline(char *buffer);
void safeGetString(char *buffer, int maxLen);
int safeGetInt();
float safeGetFloat();
void getCurrentDateTime(char *buffer, int maxLen);

// Data Persistence (File Handling)
void loadAdminPassword();
void saveAdminPassword();
void loadInventory();
void saveInventory();
void initDefaultInventory();
void logSaleToFile(struct SalesRecord record);

// Product & Inventory Functions
int findProductIndex(int id);
void displayAllProducts();
void displaySingleProductRow(struct Product p);
void addNewProduct();
void updateProduct();
void deleteProduct();
void checkLowStockAlerts();
void searchProduct();
void priceChecker();

// Billing & Point of Sale (POS)
void startBillingPOS();
float applyDiscount(float subtotal, float *discountPercent);
void printReceipt(struct SalesRecord record, struct CartItem cart[], int cartCount, float cashTendered, float balance);

// Admin & Reports
int adminLogin();
void changeAdminPassword();
void viewSalesHistory();
void viewFinancialSummary();
void adminMenu();

#endif
