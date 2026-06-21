#include "mobile.h"

#include <stdio.h>
#include <stdlib.h>

static int readMenuChoice(int min, int max) {
    int choice;
    char line[32];

    while (1) {
        printf("Enter choice (%d-%d): ", min, max);
        if (fgets(line, sizeof(line), stdin) == NULL) {
            return -1;
        }
        if (sscanf(line, "%d", &choice) == 1 && choice >= min && choice <= max) {
            return choice;
        }
        printf("Invalid choice. Please try again.\n");
    }
}

static void pauseScreen(void) {
    printf("\nPress Enter to continue...");
    getchar();
}

static void userModuleMenu(void) {
    int choice;

    while (1) {
        printf("\n========== USER MODULE ==========\n");
        printf("1. View list of available mobile phones\n");
        printf("2. Search mobile phone by model name\n");
        printf("3. Search mobile phone by brand\n");
        printf("4. Display mobile specifications\n");
        printf("5. Compare two mobile phones\n");
        printf("6. View phone prices\n");
        printf("7. Back to Main Menu\n");

        choice = readMenuChoice(1, 7);
        if (choice == -1) {
            return;
        }

        switch (choice) {
            case 1:
                viewPhoneList();
                break;
            case 2:
                searchByModel();
                break;
            case 3:
                searchByBrand();
                break;
            case 4:
                displayPhoneSpecs();
                break;
            case 5:
                comparePhones();
                break;
            case 6:
                viewPhonePrices();
                break;
            case 7:
                return;
        }

        if (choice != 7) {
            pauseScreen();
        }
    }
}

static void dataManagementMenu(void) {
    int choice;

    while (1) {
        printf("\n========== DATA MANAGEMENT ==========\n");
        printf("1. Add new mobile phone record\n");
        printf("2. Update existing phone details\n");
        printf("3. Delete phone record\n");
        printf("4. Display all phone information\n");
        printf("5. Back to Main Menu\n");

        choice = readMenuChoice(1, 5);
        if (choice == -1) {
            return;
        }

        switch (choice) {
            case 1:
                addPhone();
                break;
            case 2:
                updatePhone();
                break;
            case 3:
                deletePhone();
                break;
            case 4:
                displayAllPhones();
                break;
            case 5:
                return;
        }

        if (choice != 5) {
            pauseScreen();
        }
    }
}

int main(void) {
    int choice;

    printf("============================================\n");
    printf("   MOBILE PHONE REFERENCE APPLICATION\n");
    printf("============================================\n");

    if (loadPhones() == 0) {
        printf("\nNo existing data found. Loading sample records...\n");
        seedSampleData();
    } else {
        printf("\nLoaded %d mobile phone record(s) from file.\n", phoneCount);
    }

    while (1) {
        printf("\n============== MAIN MENU ==============\n");
        printf("1. User Module\n");
        printf("2. Data Management\n");
        printf("3. Exit\n");

        choice = readMenuChoice(1, 3);
        if (choice == -1) {
            break;
        }

        switch (choice) {
            case 1:
                userModuleMenu();
                break;
            case 2:
                dataManagementMenu();
                break;
            case 3:
                savePhones();
                printf("\nThank you for using Mobile Phone Reference Application!\n");
                return 0;
        }
    }

    savePhones();
    return 0;
}
