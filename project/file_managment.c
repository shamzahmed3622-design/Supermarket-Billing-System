#include "header.h"
//file management functions for inventory and sales history


void loadInventory() {
    FILE *fp = fopen(INVENTORY_FILE, "r");
    if (fp == NULL) {
        // File does not exist yet: create sample default mart catalog
        initDefaultInventory();
        return;
    }

    productCount = 0;
    // Format in file: ID|Name|Category|Price|Stock
    while (productCount < MAX_PRODUCTS && 
           fscanf(fp, "%d|%49[^|]|%29[^|]|%f|%d\n", 
                  &inventory[productCount].id,
                  inventory[productCount].name,
                  inventory[productCount].category,
                  &inventory[productCount].price,
                  &inventory[productCount].stock) == 5) {
        productCount++;
    }

    fclose(fp);

    if (productCount == 0) {
        initDefaultInventory();
    }
}

void saveInventory() {
    FILE *fp = fopen(INVENTORY_FILE, "w");
    if (fp == NULL) {
        printf("  [!] Error: Unable to open '%s' for saving!\n", INVENTORY_FILE);
        return;
    }

    for (int i = 0; i < productCount; i++) {
        fprintf(fp, "%d|%s|%s|%.2f|%d\n",
                inventory[i].id,
                inventory[i].name,
                inventory[i].category,
                inventory[i].price,
                inventory[i].stock);
    }

    fclose(fp);
}

void logSaleToFile(struct SalesRecord record) {
    FILE *fp = fopen(SALES_FILE, "a");
    if (fp == NULL) {
        printf("  [!] Warning: Could not open sales history file for writing.\n");
        return;
    }

    // Format: BillNo|Customer|Phone|Date|Items|Subtotal|Discount|Tax|GrandTotal|PaymentMethod
    fprintf(fp, "%d|%s|%s|%s|%d|%.2f|%.2f|%.2f|%.2f|%s\n",
            record.billNo,
            record.customerName,
            record.customerPhone,
            record.dateStr,
            record.itemCount,
            record.subtotal,
            record.discountAmount,
            record.taxAmount,
            record.grandTotal,
            record.paymentMethod);

    fclose(fp);
}
void viewSalesHistory() {
    clearScreen();
    displayBanner();
    printf("  +-------------------------------------------------------------+\n");
    printf("  |                   TRANSACTION SALES LOG                     |\n");
    printf("  +-------------------------------------------------------------+\n");

    FILE *fp = fopen(SALES_FILE, "r");
    if (fp == NULL) {
        printf("  No sales records found yet.\n");
        pauseScreen();
        return;
    }

    struct SalesRecord rec;
    int count = 0;

    printf("  +-------+-----------------+---------------------+-------+---------+---------+----------+\n");
    printf("  | BILL# | CUSTOMER        | DATE / TIME         | ITEMS | SUBTOT  | TAX     | GRAND TOT|\n");
    printf("  +-------+-----------------+---------------------+-------+---------+---------+----------+\n");

    while (fscanf(fp, "%d|%49[^|]|%19[^|]|%29[^|]|%d|%f|%f|%f|%f|%19[^\n]\n",
                  &rec.billNo,
                  rec.customerName,
                  rec.customerPhone,
                  rec.dateStr,
                  &rec.itemCount,
                  &rec.subtotal,
                  &rec.discountAmount,
                  &rec.taxAmount,
                  &rec.grandTotal,
                  rec.paymentMethod) == 10) {
        printf("  | %-5d | %-15.15s | %-19s | %-5d | Rs.%6.2f | Rs.%6.2f | Rs.%8.2f |\n",
               rec.billNo, rec.customerName, rec.dateStr, rec.itemCount, rec.subtotal, rec.taxAmount, rec.grandTotal);
        count++;
    }

    printf("  +-------+-----------------+---------------------+-------+---------+---------+----------+\n");
    printf("   Total Completed Invoices: %d\n", count);

    fclose(fp);
    pauseScreen();
}

void viewFinancialSummary() {
    clearScreen();
    displayBanner();
    printf("  +-------------------------------------------------------------+\n");
    printf("  |                  MART FINANCIAL REVENUE REPORT              |\n");
    printf("  +-------------------------------------------------------------+\n");

    FILE *fp = fopen(SALES_FILE, "r");
    if (fp == NULL) {
        printf("  No sales history found to analyze.\n");
        pauseScreen();
        return;
    }

    struct SalesRecord rec;
    int totalBills = 0;
    int totalItemsSold = 0;
    float sumSubtotal = 0.0f;
    float sumDiscount = 0.0f;
    float sumTax = 0.0f;
    float sumRevenue = 0.0f;

    while (fscanf(fp, "%d|%49[^|]|%19[^|]|%29[^|]|%d|%f|%f|%f|%f|%19[^\n]\n",
                  &rec.billNo,
                  rec.customerName,
                  rec.customerPhone,
                  rec.dateStr,
                  &rec.itemCount,
                  &rec.subtotal,
                  &rec.discountAmount,
                  &rec.taxAmount,
                  &rec.grandTotal,
                  rec.paymentMethod) == 10) {
        totalBills++;
        totalItemsSold += rec.itemCount;
        sumSubtotal += rec.subtotal;
        sumDiscount += rec.discountAmount;
        sumTax += rec.taxAmount;
        sumRevenue += rec.grandTotal;
    }

    fclose(fp);

    printf("  ===============================================================\n");
    printf("  KEY PERFORMANCE METRICS\n");
    printf("  ===============================================================\n");
    printf("   Total Bills Issued         : %d\n", totalBills);
    printf("   Total Products / Units Sold: %d units\n", totalItemsSold);
    printf("   Gross Sales Subtotal       : Rs.%.2f\n", sumSubtotal);
    printf("   Total Discounts Awarded    : Rs.%.2f\n", sumDiscount);
    printf("   Total Sales Tax Collected  : Rs.%.2f\n", sumTax);
    printf("  ---------------------------------------------------------------\n");
    printf("   NET REVENUE COLLECTED      : Rs.%.2f\n", sumRevenue);
    printf("  ===============================================================\n");

    pauseScreen();
}
