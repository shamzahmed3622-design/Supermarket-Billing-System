/* ============================================================================
                          PRODUCT & INVENTORY LOGIC
   ============================================================================ */
#include "header.h"

void initDefaultInventory() {
    productCount = 15;

    struct Product defaults[15] = {
        {101, "Fresh Whole Milk 1L", "Dairy", 2.80f, 40},
        {102, "Whole Wheat Bread 400g", "Bakery", 1.95f, 30},
        {103, "Farm Fresh Brown Eggs (12pk)", "Dairy", 3.40f, 25},
        {104, "Basmati Premium Rice 1kg", "Grocery", 4.50f, 50},
        {105, "Pure Cooking Olive Oil 500ml", "Cooking", 7.99f, 15},
        {106, "Coca-Cola Classic 1.5L", "Beverages", 2.20f, 60},
        {107, "Crispy Salted Potato Chips", "Snacks", 1.50f, 45},
        {108, "Swiss Dark Chocolate 100g", "Confectionery", 2.60f, 35},
        {109, "Instant Arabica Coffee 100g", "Beverages", 5.90f, 20},
        {110, "Green Tea Bags (Box of 25)", "Beverages", 3.25f, 28},
        {111, "Antibacterial Hand Wash 250ml", "Personal Care", 2.90f, 18},
        {112, "Total Defense Toothpaste 100ml", "Personal Care", 2.45f, 30},
        {113, "Lemon Fresh Dishwash Liquid", "Household", 3.10f, 12},
        {114, "Fine Refined White Sugar 1kg", "Grocery", 1.70f, 4},   // Low stock demo
        {115, "Crunchy Honey Almond Cereal", "Breakfast", 4.75f, 8}
    };

    for (int i = 0; i < productCount; i++) {
        inventory[i] = defaults[i];
    }

    saveInventory();
}
int findProductIndex(int id) {
    for (int i = 0; i < productCount; i++) {
        if (inventory[i].id == id) {
            return i;
        }
    }
    return -1;
}

void displaySingleProductRow(struct Product p) {
    char status[20];
    if (p.stock <= 0) {
        strcpy(status, "OUT OF STOCK");
    } else if (p.stock <= 5) {
        strcpy(status, "LOW STOCK (!)");
    } else {
        strcpy(status, "IN STOCK");
    }

    printf("  | %-5d | %-28s | %-13s | Rs.%6.2f | %-5d | %-13s |\n",
           p.id, p.name, p.category, p.price, p.stock, status);
}

void displayAllProducts() {
    printf("  +-------+------------------------------+---------------+---------+-------+---------------+\n");
    printf("  | ID    | PRODUCT NAME                 | CATEGORY      | PRICE   | STOCK | STATUS        |\n");
    printf("  +-------+------------------------------+---------------+---------+-------+---------------+\n");

    if (productCount == 0) {
        printf("  | No items found in the mart inventory!                                                 |\n");
    } else {
        for (int i = 0; i < productCount; i++) {
            displaySingleProductRow(inventory[i]);
        }
    }

    printf("  +-------+------------------------------+---------------+---------+-------+---------------+\n");
    printf("   Total Products Cataloged: %d\n", productCount);
}

void addNewProduct() {
    clearScreen();
    displayBanner();
    printf("  +-------------------------------------------------------------+\n");
    printf("  |                     ADD NEW PRODUCT                         |\n");
    printf("  +-------------------------------------------------------------+\n");

    if (productCount >= MAX_PRODUCTS) {
        printf("  [!] Inventory catalog is at maximum capacity (%d items).\n", MAX_PRODUCTS);
        pauseScreen();
        return;
    }

    int id;
    do {
        printf("   Enter Unique Product ID (e.g. 116): ");
        id = safeGetInt();
        if (id < 0) {
            printf("  [!] Product ID cannot be negative. Please try again.\n");
        }
    } while (id < 0);

    if (findProductIndex(id) != -1) {
        printf("  [!] Error: Product ID %d already exists! Choose another ID.\n", id);
        pauseScreen();
        return;
    }

    struct Product newProd;
    newProd.id = id;

    do {
        printf("   Enter Product Name (up to 45 chars): ");
        safeGetString(newProd.name, sizeof(newProd.name));
        if (newProd.name[0] == '\0') {
            printf("  [!] Product name cannot be blank.\n");
        }
    } while (newProd.name[0] == '\0');

   
        printf("   Enter Category (Dairy, Bakery, Snacks, etc.): ");
        safeGetString(newProd.category, sizeof(newProd.category));

    printf("   Enter Price (Rs.): ");
    newProd.price = safeGetFloat();
    while (newProd.price <= 0.0f) {
        printf("   [!] Price must be greater than zero: ");
        newProd.price = safeGetFloat();
    }

    printf("   Enter Initial Stock Quantity: ");
    newProd.stock = safeGetInt();
    while (newProd.stock < 0) {
        printf("   [!] Stock cannot be negative: ");
        newProd.stock = safeGetInt();
    }

    inventory[productCount] = newProd;
    productCount++;

    saveInventory();
    printf("\n  [+] Success: Product '%s' (ID %d) added to inventory!\n", newProd.name, newProd.id);
    pauseScreen();
}

void updateProduct() {
    clearScreen();
    displayBanner();
    printf("  +-------------------------------------------------------------+\n");
    printf("  |                   UPDATE / RESTOCK PRODUCT                  |\n");
    printf("  +-------------------------------------------------------------+\n");

    printf("   Enter Product ID to update: ");
    int id = safeGetInt();

    int idx = findProductIndex(id);
    if (idx == -1) {
        printf("  [!] Product ID %d not found in inventory!\n", id);
        pauseScreen();
        return;
    }

    printf("\n  Current Product Details:\n");
    printf("  -----------------------------------------\n");
    printf("  ID       : %d\n", inventory[idx].id);
    printf("  Name     : %s\n", inventory[idx].name);
    printf("  Category : %s\n", inventory[idx].category);
    printf("  Price    : Rs.%.2f\n", inventory[idx].price);
    printf("  Stock    : %d units\n", inventory[idx].stock);
    printf("  -----------------------------------------\n");

    printf("\n  What would you like to update?\n");
    printf("  [1] Restock (Add more units to stock)\n");
    printf("  [2] Set New Stock Quantity\n");
    printf("  [3] Update Price\n");
    printf("  [4] Update Product Name\n");
    printf("  [5] Cancel\n");
    printf("  Select choice: ");

    int subChoice = safeGetInt();
    switch (subChoice) {
        case 1: {
            printf("   Enter number of units to ADD: ");
            int addQty = safeGetInt();
            if (addQty > 0) {
                inventory[idx].stock += addQty;
                saveInventory();
                printf("  [+] Stock updated! New stock: %d units\n", inventory[idx].stock);
            } else {
                printf("  [!] Quantity must be greater than 0.\n");
            }
            break;
        }
        case 2: {
            printf("   Enter NEW exact stock quantity: ");
            int newStock = safeGetInt();
            if (newStock >= 0) {
                inventory[idx].stock = newStock;
                saveInventory();
                printf("  [+] Stock updated to %d units.\n", inventory[idx].stock);
            } else {
                printf("  [!] Stock cannot be negative.\n");
            }
            break;
        }
        case 3: {
            printf("   Enter NEW price (Rs.): ");
            float newPrice = safeGetFloat();
            if (newPrice > 0.0f) {
                inventory[idx].price = newPrice;
                saveInventory();
                printf("  [+] Price updated to Rs.%.2f\n", inventory[idx].price);
            } else {
                printf("  [!] Price must be positive.\n");
            }
            break;
        }
        case 4: {
            do {
                printf("   Enter NEW product name: ");
                safeGetString(inventory[idx].name, sizeof(inventory[idx].name));
                if (inventory[idx].name[0] == '\0') {
                    printf("  [!] Product name cannot be blank.\n");
                }
            } while (inventory[idx].name[0] == '\0');
            saveInventory();
            printf("  [+] Product name updated to '%s'\n", inventory[idx].name);
            break;
        }
        case 5:
            printf("  Operation cancelled.\n");
            break;
        default:
            printf("  [!] Invalid choice.\n");
    }

    pauseScreen();
}

void deleteProduct() {
    clearScreen();
    displayBanner();
    printf("  +-------------------------------------------------------------+\n");
    printf("  |                     DELETE PRODUCT                          |\n");
    printf("  +-------------------------------------------------------------+\n");

    printf("   Enter Product ID to delete: ");
    int id = safeGetInt();

    int idx = findProductIndex(id);
    if (idx == -1) {
        printf("  [!] Product ID %d not found!\n", id);
        pauseScreen();
        return;
    }

    printf("\n  Selected Product: %s (Category: %s, Price: Rs.%.2f, Stock: %d)\n",
           inventory[idx].name, inventory[idx].category, inventory[idx].price, inventory[idx].stock);
    printf("  Are you sure you want to permanently delete this item? (y/n): ");

    char confirm[10];
    safeGetString(confirm, sizeof(confirm));

    if (confirm[0] == 'y' || confirm[0] == 'Y') {
        // Shift elements to the left to delete without gaps
        for (int i = idx; i < productCount - 1; i++) {
            inventory[i] = inventory[i + 1];
        }
        productCount--;
        saveInventory();
        printf("  [+] Product successfully removed from database.\n");
    } else {
        printf("  Deletion aborted.\n");
    }

    pauseScreen();
}

void checkLowStockAlerts() {
    clearScreen();
    displayBanner();
    printf("  +-------------------------------------------------------------+\n");
    printf("  |                   LOW STOCK ALERT MONITOR                   |\n");
    printf("  |              (Items with 5 or fewer units left)             |\n");
    printf("  +-------------------------------------------------------------+\n");

    int lowCount = 0;
    printf("  +-------+------------------------------+---------------+---------+-------+---------------+\n");
    printf("  | ID    | PRODUCT NAME                 | CATEGORY      | PRICE   | STOCK | STATUS        |\n");
    printf("  +-------+------------------------------+---------------+---------+-------+---------------+\n");

    for (int i = 0; i < productCount; i++) {
        if (inventory[i].stock <= 5) {
            displaySingleProductRow(inventory[i]);
            lowCount++;
        }
    }
    printf("  +-------+------------------------------+---------------+---------+-------+---------------+\n");

    if (lowCount == 0) {
        printf("  [OK] Good news! All inventory levels are above safety threshold (> 5 units).\n");
    } else {
        printf("  [!] ATTENTION: %d item(s) are running critically low! Please restock soon.\n", lowCount);
    }

    pauseScreen();
}

void searchProduct() {
    clearScreen();
    displayBanner();
    printf("  +-------------------------------------------------------------+\n");
    printf("  |                       SEARCH PRODUCT                        |\n");
    printf("  +-------------------------------------------------------------+\n");
    printf("  [1] Search by Product ID\n");
    printf("  [2] Search by Name keyword\n");
    printf("  [3] Filter by Category\n");
    printf("  Enter search method (1-3): ");

    int method = safeGetInt();
    if (method == 1) {
        printf("   Enter Product ID: ");
        int id = safeGetInt();
        int idx = findProductIndex(id);
        if (idx != -1) {
            printf("\n  Product Found:\n");
            printf("  +-------+------------------------------+---------------+---------+-------+---------------+\n");
            displaySingleProductRow(inventory[idx]);
            printf("  +-------+------------------------------+---------------+---------+-------+---------------+\n");
        } else {
            printf("  [!] No product found with ID %d.\n", id);
        }
    } else if (method == 2) {
        char keyword[50];
        printf("   Enter search keyword (e.g. Milk, Rice, Tea): ");
        safeGetString(keyword, sizeof(keyword));

        // Convert keyword to lowercase for case-insensitive search
        char lowerKey[50];
        for (int i = 0; keyword[i]; i++) lowerKey[i] = tolower(keyword[i]);
        lowerKey[strlen(keyword)] = '\0';

        int matchCount = 0;
        printf("\n  Search Results for '%s':\n", keyword);
        printf("  +-------+------------------------------+---------------+---------+-------+---------------+\n");
        printf("  | ID    | PRODUCT NAME                 | CATEGORY      | PRICE   | STOCK | STATUS        |\n");
        printf("  +-------+------------------------------+---------------+---------+-------+---------------+\n");

        for (int i = 0; i < productCount; i++) {
            char lowerName[50];
            for (int j = 0; inventory[i].name[j]; j++) lowerName[j] = tolower(inventory[i].name[j]);
            lowerName[strlen(inventory[i].name)] = '\0';

            if (strstr(lowerName, lowerKey) != NULL) {
                displaySingleProductRow(inventory[i]);
                matchCount++;
            }
        }
        printf("  +-------+------------------------------+---------------+---------+-------+---------------+\n");
        printf("   Matches Found: %d\n", matchCount);
    } else if (method == 3) {
        char cat[30];
        printf("   Enter Category name: ");
        safeGetString(cat, sizeof(cat));

        char lowerCat[30];
        for (int i = 0; cat[i]; i++) lowerCat[i] = tolower(cat[i]);
        lowerCat[strlen(cat)] = '\0';

        int matchCount = 0;
        printf("\n  Products in Category '%s':\n", cat);
        printf("  +-------+------------------------------+---------------+---------+-------+---------------+\n");
        printf("  | ID    | PRODUCT NAME                 | CATEGORY      | PRICE   | STOCK | STATUS        |\n");
        printf("  +-------+------------------------------+---------------+---------+-------+---------------+\n");

        for (int i = 0; i < productCount; i++) {
            char currentCat[30];
            for (int j = 0; inventory[i].category[j]; j++) currentCat[j] = tolower(inventory[i].category[j]);
            currentCat[strlen(inventory[i].category)] = '\0';

            if (strstr(currentCat, lowerCat) != NULL) {
                displaySingleProductRow(inventory[i]);
                matchCount++;
            }
        }
        printf("  +-------+------------------------------+---------------+---------+-------+---------------+\n");
        printf("   Matches Found: %d\n", matchCount);
    } else {
        printf("  [!] Invalid search option.\n");
    }

    pauseScreen();
}

void priceChecker() {
    while (1) {
        clearScreen();
        displayBanner();
        printf("  +-------------------------------------------------------------+\n");
        printf("  |                 CUSTOMER QUICK PRICE CHECKER                |\n");
        printf("  +-------------------------------------------------------------+\n");
        printf("   Enter Product ID to check price (or 0 to return to Main Menu): ");

        int id = safeGetInt();
        if (id == 0) break;

        int idx = findProductIndex(id);
        if (idx == -1) {
            printf("\n  [!] Product ID %d was not found in our system.\n", id);
        } else {
            printf("\n  =========================================\n");
            printf("  Item Name     : %s\n", inventory[idx].name);
            printf("  Category      : %s\n", inventory[idx].category);
            printf("  Unit Price    : Rs.%.2f\n", inventory[idx].price);
            if (inventory[idx].stock > 0) {
                printf("  Availability  : AVAILABLE (%d units in stock)\n", inventory[idx].stock);
            } else {
                printf("  Availability  : CURRENTLY OUT OF STOCK\n");
            }
            printf("  =========================================\n");
        }

        printf("\n  Would you like to check another item? (y/n): ");
        char ans[10];
        safeGetString(ans, sizeof(ans));
        if (ans[0] != 'y' && ans[0] != 'Y') {
            break;
        }
    }
}
