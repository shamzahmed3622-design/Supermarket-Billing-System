//BILLING & POINT OF SALE (POS)
#include "header.h"
float applyDiscount(float subtotal, float *discountPercent) {
    printf("\n  +-------------------------------------------------------------+\n");
    printf("  |                     DISCOUNT & PROMOTIONS                   |\n");
    printf("  +-------------------------------------------------------------+\n");
    printf("  [1] No Discount (0%%)\n");
    printf("  [2] Super Mart Member Club (5%% OFF)\n");
    printf("  [3] Enter Promo / Voucher Code (SAVE10, MEGA20, STUDENT15)\n");
    printf("  [4] Cashier Custom Percentage Discount\n");
    printf("  Select discount option (1-4): ");

    int discChoice = safeGetInt();
    *discountPercent = 0.0f;

    switch (discChoice) {
        case 1:
            *discountPercent = 0.0f;
            break;
        case 2:
            *discountPercent = 5.0f;
            printf("  [+] Super Mart Membership Discount applied (5%% OFF)!\n");
            break;
        case 3: {
            char code[30];
            printf("   Enter Promo Code: ");
            safeGetString(code, sizeof(code));

            // Convert to uppercase
            for (int i = 0; code[i]; i++) code[i] = toupper(code[i]);

            if (strcmp(code, "SAVE10") == 0) {
                *discountPercent = 10.0f;
                printf("  [+] Promo Valid! 10%% Discount applied.\n");
            } else if (strcmp(code, "MEGA20") == 0) {
                *discountPercent = 20.0f;
                printf("  [+] Mega Deal Valid! 20%% Discount applied.\n");
            } else if (strcmp(code, "STUDENT15") == 0) {
                *discountPercent = 15.0f;
                printf("  [+] Student Discount Valid! 15%% Discount applied.\n");
            } else {
                printf("  [!] Invalid or expired promo code. No discount applied.\n");
                *discountPercent = 0.0f;
            }
            break;
        }
        case 4: {
            printf("   Enter custom discount percentage (0 to 50%%): ");
            float customD = safeGetFloat();
            if (customD >= 0.0f && customD <= 50.0f) {
                *discountPercent = customD;
                printf("  [+] Custom %.1f%% discount applied.\n", customD);
            } else {
                printf("  [!] Invalid percentage. Limit is 50%% max.\n");
                *discountPercent = 0.0f;
            }
            break;
        }
        default:
            printf("  No discount applied.\n");
            *discountPercent = 0.0f;
    }

    return (subtotal * (*discountPercent) / 100.0f);
}

void printReceipt(struct SalesRecord record, struct CartItem cart[], int cartCount, float cashTendered, float balance) {
    char filename[50];
    sprintf(filename, "receipt_%d.txt", record.billNo);

    // Save detailed customer receipt to individual text file
    FILE *rfp = fopen(filename, "w");
    if (!rfp) {
        perror("Unable to create receipt file");
        printf("  [!] Receipt was displayed but not saved as '%s'.\n", filename);
    }

    // macro to print to both screen and file cleanly
    #define PRINT_BOTH(...) do { printf(__VA_ARGS__); if(rfp) fprintf(rfp, __VA_ARGS__); } while(0)

    clearScreen();
    PRINT_BOTH("\n");
    PRINT_BOTH("  *****************************************************************\n");
    PRINT_BOTH("                   *** SUPER MART SUPERMARKET ***                 \n");
    PRINT_BOTH("                 Main Commercial Avenue, Block 4                   \n");
    PRINT_BOTH("                    Phone: +1 (800) 555-MART                      \n");
    PRINT_BOTH("  *****************************************************************\n");
    PRINT_BOTH("  Invoice No   : %-15d Date/Time: %s\n", record.billNo, record.dateStr);
    PRINT_BOTH("  Customer Name: %-15s Phone    : %s\n", record.customerName, record.customerPhone);
    PRINT_BOTH("  -----------------------------------------------------------------\n");
    PRINT_BOTH("  %-4s %-26s %-8s %-10s %-10s\n", "No.", "Item Description", "Qty", "Price", "Total");
    PRINT_BOTH("  -----------------------------------------------------------------\n");

    for (int i = 0; i < cartCount; i++) {
        PRINT_BOTH("  %-4d %-26.26s %-8d Rs.%-9.2f Rs.%-9.2f\n",
                   i + 1, cart[i].name, cart[i].quantity, cart[i].price, cart[i].subtotal);
    }

    PRINT_BOTH("  -----------------------------------------------------------------\n");
    PRINT_BOTH("  Total Items Purchased     : %d\n", record.itemCount);
    PRINT_BOTH("  Cart Subtotal             : Rs.%.2f\n", record.subtotal);
    PRINT_BOTH("  Discount Applied          : -Rs.%.2f\n", record.discountAmount);
    PRINT_BOTH("  Sales Tax / GST (5.0%%)    : +Rs.%.2f\n", record.taxAmount);
    PRINT_BOTH("  =================================================================\n");
    PRINT_BOTH("  FINAL GRAND TOTAL         : Rs.%.2f\n", record.grandTotal);
    PRINT_BOTH("  =================================================================\n");
    PRINT_BOTH("  Payment Method            : %s\n", record.paymentMethod);

    if (strcmp(record.paymentMethod, "Cash") == 0) {
        PRINT_BOTH("  Cash Tendered             : Rs.%.2f\n", cashTendered);
        PRINT_BOTH("  Change Returned to Buyer  : Rs.%.2f\n", balance);
    }

    PRINT_BOTH("  -----------------------------------------------------------------\n");
    PRINT_BOTH("             Thank you for shopping with Super Mart!               \n");
    PRINT_BOTH("                 Please visit us again soon!                      \n");
    PRINT_BOTH("  *****************************************************************\n\n");

    #undef PRINT_BOTH

    if (rfp) {
        int writeFailed = ferror(rfp);
        if (fclose(rfp) != 0) {
            writeFailed = 1;
            perror("Error closing receipt file");
        }

        if (writeFailed) {
            printf("  [!] Receipt may be incomplete: '%s'.\n", filename);
        } else {
            printf("  [i] Printable invoice saved to disk as: '%s'\n", filename);
        }
    }
}

void startBillingPOS() {
    struct CartItem cart[MAX_CART];
    int cartCount = 0;
    float runningSubtotal = 0.0f;

    clearScreen();
    displayBanner();
    printf("  +-------------------------------------------------------------+\n");
    printf("  |                   POS BILLING & CHECKOUT                    |\n");
    printf("  +-------------------------------------------------------------+\n");

    char custName[50];
    char custPhone[20];

    printf("   Enter Customer Name (or 'Guest'): ");
    safeGetString(custName, sizeof(custName));
    if (strlen(custName) == 0) strcpy(custName, "Guest Customer");

    printf("   Enter Customer Phone Number (or '-': ");
    safeGetString(custPhone, sizeof(custPhone));
    if (strlen(custPhone) == 0) strcpy(custPhone, "N/A");

    // Shopping cart item addition loop
    while (1) {
        clearScreen();
        displayBanner();
        printf("  Customer: %s | Phone: %s\n", custName, custPhone);
        printf("  Current Items in Cart: %d | Running Subtotal: Rs.%.2f\n\n", cartCount, runningSubtotal);

        printf("  [1] Add Product to Cart by ID\n");
        printf("  [2] View Current Cart\n");
        printf("  [3] Remove an Item from Cart\n");
        printf("  [4] Quick View Product Catalog / Price List\n");
        printf("  [5] Proceed to Checkout & Payment\n");
        printf("  [6] Cancel Bill and Return to Main Menu\n");
        printf("  Select POS action (1-6): ");

        int action = safeGetInt();

        if (action == 1) {
            if (cartCount >= MAX_CART) {
                printf("  [!] Cart is full! Cannot add more items.\n");
                pauseScreen();
                continue;
            }

            printf("\n   Enter Product ID: ");
            int id = safeGetInt();

            int idx = findProductIndex(id);
            if (idx == -1) {
                printf("  [!] Product ID %d not found in inventory!\n", id);
                pauseScreen();
                continue;
            }

            printf("   Selected: %s | Unit Price: Rs.%.2f | Stock Available: %d\n",
                   inventory[idx].name, inventory[idx].price, inventory[idx].stock);

            if (inventory[idx].stock <= 0) {
                printf("  [!] Sorry, this item is OUT OF STOCK!\n");
                pauseScreen();
                continue;
            }

            printf("   Enter Quantity to purchase: ");
            int qty = safeGetInt();

            if (qty <= 0) {
                printf("  [!] Quantity must be at least 1.\n");
                pauseScreen();
                continue;
            }

            // Check if user already has some in cart
            int alreadyInCart = 0;
            for (int i = 0; i < cartCount; i++) {
                if (cart[i].id == id) {
                    alreadyInCart += cart[i].quantity;
                }
            }

            if ((qty + alreadyInCart) > inventory[idx].stock) {
                printf("  [!] Insufficient Stock! Only %d available (%d already in cart).\n",
                       inventory[idx].stock, alreadyInCart);
                pauseScreen();
                continue;
            }

            // If item already in cart, increment quantity
            int foundInCart = 0;
            for (int i = 0; i < cartCount; i++) {
                if (cart[i].id == id) {
                    cart[i].quantity += qty;
                    cart[i].subtotal = cart[i].quantity * cart[i].price;
                    foundInCart = 1;
                    break;
                }
            }

            if (!foundInCart) {
                cart[cartCount].id = inventory[idx].id;
                strcpy(cart[cartCount].name, inventory[idx].name);
                cart[cartCount].price = inventory[idx].price;
                cart[cartCount].quantity = qty;
                cart[cartCount].subtotal = qty * inventory[idx].price;
                cartCount++;
            }

            runningSubtotal = 0.0f;
            for (int i = 0; i < cartCount; i++) {
                runningSubtotal += cart[i].subtotal;
            }

            printf("  [+] Added %d x '%s' to cart successfully!\n", qty, inventory[idx].name);
            pauseScreen();

        } else if (action == 2) {
            clearScreen();
            displayBanner();
            printf("  +-------------------------------------------------------------+\n");
            printf("  |                      CURRENT SHOPPING CART                  |\n");
            printf("  +-------------------------------------------------------------+\n");

            if (cartCount == 0) {
                printf("  Cart is currently empty!\n");
            } else {
                printf("  +----+------------------------------+-------+---------+----------+\n");
                printf("  | No | Item Name                    | Qty   | Price   | Subtotal |\n");
                printf("  +----+------------------------------+-------+---------+----------+\n");
                for (int i = 0; i < cartCount; i++) {
                    printf("  | %-2d | %-28s | %-5d | Rs.%6.2f | Rs.%7.2f |\n",
                           i + 1, cart[i].name, cart[i].quantity, cart[i].price, cart[i].subtotal);
                }
                printf("  +----+------------------------------+-------+---------+----------+\n");
                printf("  Cart Subtotal: Rs.%.2f\n", runningSubtotal);
            }
            pauseScreen();

        } else if (action == 3) {
            if (cartCount == 0) {
                printf("  [!] Cart is already empty.\n");
                pauseScreen();
                continue;
            }

            printf("   Enter Item Number (1-%d) to remove: ", cartCount);
            int itemNum = safeGetInt();

            if (itemNum >= 1 && itemNum <= cartCount) {
                int removeIdx = itemNum - 1;
                printf("  [-] Removed '%s' from cart.\n", cart[removeIdx].name);

                for (int i = removeIdx; i < cartCount - 1; i++) {
                    cart[i] = cart[i + 1];
                }
                cartCount--;

                runningSubtotal = 0.0f;
                for (int i = 0; i < cartCount; i++) {
                    runningSubtotal += cart[i].subtotal;
                }
            } else {
                printf("  [!] Invalid item number.\n");
            }
            pauseScreen();

        } else if (action == 4) {
            clearScreen();
            displayBanner();
            displayAllProducts();
            pauseScreen();

        } else if (action == 5) {
            if (cartCount == 0) {
                printf("  [!] Cannot checkout with an empty cart! Add at least 1 item.\n");
                pauseScreen();
                continue;
            }
            // Break from cart loop to proceed to checkout calculation
            break;

        } else if (action == 6) {
            printf("  Are you sure you want to cancel this entire bill? (y/n): ");
            char confirm[10];
            safeGetString(confirm, sizeof(confirm));
            if (confirm[0] == 'y' || confirm[0] == 'Y') {
                printf("  Bill discarded.\n");
                pauseScreen();
                return;
            }
        } else {
            printf("  [!] Invalid choice.\n");
            pauseScreen();
        }
    }

    // ----------------- CHECKOUT & PAYMENT STAGE -----------------
    clearScreen();
    displayBanner();
    printf("  +-------------------------------------------------------------+\n");
    printf("  |                     CHECKOUT & INVOICING                    |\n");
    printf("  +-------------------------------------------------------------+\n");

    int totalItems = 0;
    for (int i = 0; i < cartCount; i++) {
        totalItems += cart[i].quantity;
    }

    float discountPercent = 0.0f;
    float discountAmount = applyDiscount(runningSubtotal, &discountPercent);
    float taxableAmount = runningSubtotal - discountAmount;
    float taxAmount = taxableAmount * TAX_RATE;
    float grandTotal = taxableAmount + taxAmount;

    printf("\n  Financial Summary:\n");
    printf("  --------------------------------------\n");
    printf("  Subtotal (%d units)   : Rs.%.2f\n", totalItems, runningSubtotal);
    printf("  Discount (%.1f%%)       : -Rs.%.2f\n", discountPercent, discountAmount);
    printf("  Sales Tax (5.0%%)      : +Rs.%.2f\n", taxAmount);
    printf("  --------------------------------------\n");
    printf("  FINAL PAYABLE TOTAL   : Rs.%.2f\n", grandTotal);
    printf("  --------------------------------------\n");

    // Select Payment Method
    printf("\n  Select Payment Method:\n");
    printf("  [1] Cash\n");
    printf("  [2] Credit / Debit Card\n");
    printf("  [3] Mobile Wallet (JazzCash / EasyPaisa / UPI / ApplePay)\n");
    printf("  Select (1-3): ");

    int payChoice = safeGetInt();
    char paymentMethod[20];
    float cashTendered = 0.0f;
    float balance = 0.0f;

    if (payChoice == 1) {
        strcpy(paymentMethod, "Cash");
        while (1) {
            printf("   Enter Cash Received from Customer (Rs.): ");
            cashTendered = safeGetFloat();
            if (cashTendered >= grandTotal) {
                balance = cashTendered - grandTotal;
                printf("  [+] Change to return to customer: Rs.%.2f\n", balance);
                break;
            } else {
                printf("  [!] Insufficient cash! Grand total is Rs.%.2f (Short by Rs.%.2f)\n",
                       grandTotal, grandTotal - cashTendered);
            }
        }
    } else if (payChoice == 2) {
        strcpy(paymentMethod, "Card");
        printf("   Enter last 4 digits of Card: ");
        char cardLast[10];
        safeGetString(cardLast, sizeof(cardLast));
        printf("  [+] Card Authorization Approved. Transaction #TXN%ld\n", (long)time(NULL) % 100000);
        cashTendered = grandTotal;
        balance = 0.0f;
    } else {
        strcpy(paymentMethod, "Mobile Wallet");
        printf("   Enter Mobile Wallet Transaction Reference ID: ");
        char refId[30];
        safeGetString(refId, sizeof(refId));
        printf("  [+] Mobile payment verified and confirmed!\n");
        cashTendered = grandTotal;
        balance = 0.0f;
    }

    // Deduct stock from inventory
    for (int i = 0; i < cartCount; i++) {
        int idx = findProductIndex(cart[i].id);
        if (idx != -1) {
            inventory[idx].stock -= cart[i].quantity;
            if (inventory[idx].stock < 0) inventory[idx].stock = 0;
        }
    }
    saveInventory();

    // Prepare Sales Record
    struct SalesRecord record;
    record.billNo = (int)(time(NULL) % 90000) + 10000; // Unique 5-digit bill number
    strcpy(record.customerName, custName);
    strcpy(record.customerPhone, custPhone);
    getCurrentDateTime(record.dateStr, sizeof(record.dateStr));
    record.itemCount = totalItems;
    record.subtotal = runningSubtotal;
    record.discountAmount = discountAmount;
    record.taxAmount = taxAmount;
    record.grandTotal = grandTotal;
    strcpy(record.paymentMethod, paymentMethod);

    // Save sale to persistent sales history
    logSaleToFile(record);

    // Print Receipt to Screen and File
    printReceipt(record, cart, cartCount, cashTendered, balance);

    pauseScreen();
}
