/* ============================================================================
                          UTILITY & UI FUNCTIONS
   ============================================================================ */
 #include "header.h"

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pauseScreen() {
    printf("\n  Press Enter to continue...");
    getchar();
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void removeNewline(char *buffer) {
    buffer[strcspn(buffer, "\r\n")] = '\0';
}

void safeGetString(char *buffer, int maxLen) {
    if (fgets(buffer, maxLen, stdin) != NULL) {
        if (strchr(buffer, '\n') == NULL && !feof(stdin)) {
            clearInputBuffer();
        }
        removeNewline(buffer);
    }
}

int safeGetInt() {
    int value;
    while (scanf("%d", &value) != 1) {
        printf("   [!] Invalid input. Please enter a valid number: ");
        clearInputBuffer();
    }
    clearInputBuffer();
    return value;
}

float safeGetFloat() {
    float value;
    while (scanf("%f", &value) != 1) {
        printf("   [!] Invalid input. Please enter a valid decimal number: ");
        clearInputBuffer();
    }
    clearInputBuffer();
    return value;
}

void getCurrentDateTime(char *buffer, int maxLen) {
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, maxLen, "%Y-%m-%d %H:%M:%S", timeinfo);
}

void displayBanner() {
    printf("=================================================================\n");
    printf("            *** SUPER MART BILLING & RETAIL SYSTEM ***           \n");
    printf("                  Automated Point-Of-Sale (POS)                  \n");
    printf("=================================================================\n\n");
}

/* ============================================================================
                          FILE HANDLING & PERSISTENCE
   ============================================================================ */

void loadAdminPassword() {
    FILE *fp = fopen(CONFIG_FILE, "r");
    if (fp != NULL) {
        if (fgets(adminPassword, sizeof(adminPassword), fp) != NULL) {
            size_t len = strlen(adminPassword);
            if (len > 0 && adminPassword[len - 1] == '\n') {
                adminPassword[len - 1] = '\0';
            }
        }
        fclose(fp);
    }
}

void saveAdminPassword() {
    FILE *fp = fopen(CONFIG_FILE, "w");
    if (fp != NULL) {
        fprintf(fp, "%s\n", adminPassword);
        fclose(fp);
    }
}
