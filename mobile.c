#include "mobile.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MobilePhone phones[MAX_PHONES];
int phoneCount = 0;

void clearInputBuffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

static void trimNewline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

static void readLine(const char *prompt, char *buffer, int size) {
    printf("%s", prompt);
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    trimNewline(buffer);
}

static float readPrice(const char *prompt) {
    float price;
    char line[64];

    while (1) {
        printf("%s", prompt);
        if (fgets(line, sizeof(line), stdin) == NULL) {
            return 0.0f;
        }
        if (sscanf(line, "%f", &price) == 1 && price >= 0) {
            return price;
        }
        printf("Invalid price. Please enter a non-negative number.\n");
    }
}

int loadPhones(void) {
    FILE *file = fopen(FILENAME, "rb");

    phoneCount = 0;
    if (file == NULL) {
        return 0;
    }

    while (phoneCount < MAX_PHONES &&
           fread(&phones[phoneCount], sizeof(MobilePhone), 1, file) == 1) {
        phoneCount++;
    }

    fclose(file);
    return phoneCount;
}

int savePhones(void) {
    FILE *file = fopen(FILENAME, "wb");

    if (file == NULL) {
        printf("Error: Unable to save data to file.\n");
        return 0;
    }

    if (phoneCount > 0) {
        fwrite(phones, sizeof(MobilePhone), (size_t)phoneCount, file);
    }

    fclose(file);
    return 1;
}

void seedSampleData(void) {
    MobilePhone samples[] = {
        {"Samsung", "Galaxy S24", "Snapdragon 8 Gen 3", "8GB / 256GB",
         "50MP Main + 12MP Ultra Wide + 10MP Telephoto", "4000 mAh",
         "6.2 inches", "Android 14", 79999.0f},
        {"Apple", "iPhone 15", "A16 Bionic", "6GB / 128GB",
         "48MP Main + 12MP Ultra Wide", "3349 mAh", "6.1 inches", "iOS 17",
         89900.0f},
        {"OnePlus", "12R", "Snapdragon 8 Gen 2", "8GB / 128GB",
         "50MP Main + 8MP Ultra Wide + 2MP Macro", "5500 mAh", "6.78 inches",
         "Android 14", 39999.0f},
        {"Xiaomi", "Redmi Note 13 Pro", "Snapdragon 7s Gen 2", "8GB / 256GB",
         "200MP Main + 8MP Ultra Wide + 2MP Macro", "5100 mAh", "6.67 inches",
         "Android 13", 24999.0f},
        {"Google", "Pixel 8", "Google Tensor G3", "8GB / 128GB",
         "50MP Main + 12MP Ultra Wide", "4575 mAh", "6.2 inches", "Android 14",
         75999.0f}};

    int sampleCount = (int)(sizeof(samples) / sizeof(samples[0]));
    int i;

    phoneCount = 0;
    for (i = 0; i < sampleCount && phoneCount < MAX_PHONES; i++) {
        phones[phoneCount++] = samples[i];
    }

    savePhones();
    printf("Sample mobile phone records loaded.\n");
}

void displayPhone(const MobilePhone *phone) {
    printf("\n----------------------------------------\n");
    printf("Brand           : %s\n", phone->brand);
    printf("Model           : %s\n", phone->model);
    printf("Processor       : %s\n", phone->processor);
    printf("RAM & Storage   : %s\n", phone->ram_storage);
    printf("Camera          : %s\n", phone->camera);
    printf("Battery         : %s\n", phone->battery);
    printf("Display         : %s\n", phone->display);
    printf("Operating System: %s\n", phone->os);
    printf("Price           : Rs. %.2f\n", phone->price);
    printf("----------------------------------------\n");
}

void displayPhoneBrief(const MobilePhone *phone, int index) {
    printf("%2d. %s %s - Rs. %.2f\n", index + 1, phone->brand, phone->model,
           phone->price);
}

void displayAllPhones(void) {
    int i;

    if (phoneCount == 0) {
        printf("\nNo mobile phone records found.\n");
        return;
    }

    printf("\n========== ALL MOBILE PHONES ==========\n");
    for (i = 0; i < phoneCount; i++) {
        displayPhone(&phones[i]);
    }
}

void viewPhoneList(void) {
    int i;

    if (phoneCount == 0) {
        printf("\nNo mobile phones available.\n");
        return;
    }

    printf("\n========== AVAILABLE MOBILE PHONES ==========\n");
    for (i = 0; i < phoneCount; i++) {
        displayPhoneBrief(&phones[i], i);
    }
}

void viewPhonePrices(void) {
    int i;

    if (phoneCount == 0) {
        printf("\nNo mobile phones available.\n");
        return;
    }

    printf("\n========== MOBILE PHONE PRICES ==========\n");
    printf("%-4s %-15s %-25s %12s\n", "No.", "Brand", "Model", "Price (Rs.)");
    printf("------------------------------------------------------------\n");
    for (i = 0; i < phoneCount; i++) {
        printf("%-4d %-15s %-25s %12.2f\n", i + 1, phones[i].brand,
               phones[i].model, phones[i].price);
    }
}

int findByModel(const char *model) {
    int i;

    for (i = 0; i < phoneCount; i++) {
        if (strcmp(phones[i].model, model) == 0) {
            return i;
        }
    }
    return -1;
}

int findByBrand(const char *brand, int results[], int maxResults) {
    int i;
    int count = 0;

    for (i = 0; i < phoneCount && count < maxResults; i++) {
        if (strcmp(phones[i].brand, brand) == 0) {
            results[count++] = i;
        }
    }
    return count;
}

void displayPhoneSpecs(void) {
    char model[MODEL_LEN];
    int index;

    if (phoneCount == 0) {
        printf("\nNo mobile phones available.\n");
        return;
    }

    viewPhoneList();
    readLine("\nEnter model name: ", model, sizeof(model));

    index = findByModel(model);
    if (index == -1) {
        printf("\nPhone with model '%s' not found.\n", model);
        return;
    }

    printf("\n========== MOBILE SPECIFICATIONS ==========\n");
    displayPhone(&phones[index]);
}

void searchByModel(void) {
    char model[MODEL_LEN];
    int index;

    if (phoneCount == 0) {
        printf("\nNo mobile phones available.\n");
        return;
    }

    readLine("\nEnter model name to search: ", model, sizeof(model));
    index = findByModel(model);

    if (index == -1) {
        printf("\nNo phone found with model name '%s'.\n", model);
        return;
    }

    printf("\nSearch result:\n");
    displayPhone(&phones[index]);
}

void searchByBrand(void) {
    char brand[BRAND_LEN];
    int results[MAX_PHONES];
    int count;
    int i;

    if (phoneCount == 0) {
        printf("\nNo mobile phones available.\n");
        return;
    }

    readLine("\nEnter brand name to search: ", brand, sizeof(brand));
    count = findByBrand(brand, results, MAX_PHONES);

    if (count == 0) {
        printf("\nNo phones found for brand '%s'.\n", brand);
        return;
    }

    printf("\nFound %d phone(s) for brand '%s':\n", count, brand);
    for (i = 0; i < count; i++) {
        displayPhone(&phones[results[i]]);
    }
}

void comparePhones(void) {
    char model1[MODEL_LEN];
    char model2[MODEL_LEN];
    int index1;
    int index2;
    MobilePhone *p1;
    MobilePhone *p2;

    if (phoneCount < 2) {
        printf("\nAt least two phones are required for comparison.\n");
        return;
    }

    viewPhoneList();
    readLine("\nEnter first model name: ", model1, sizeof(model1));
    readLine("Enter second model name: ", model2, sizeof(model2));

    index1 = findByModel(model1);
    index2 = findByModel(model2);

    if (index1 == -1) {
        printf("\nPhone with model '%s' not found.\n", model1);
        return;
    }
    if (index2 == -1) {
        printf("\nPhone with model '%s' not found.\n", model2);
        return;
    }

    p1 = &phones[index1];
    p2 = &phones[index2];

    printf("\n================ PHONE COMPARISON ================\n");
    printf("%-22s | %-28s | %-28s\n", "Specification", p1->model, p2->model);
    printf("--------------------------------------------------------------------\n");
    printf("%-22s | %-28s | %-28s\n", "Brand", p1->brand, p2->brand);
    printf("%-22s | %-28s | %-28s\n", "Processor", p1->processor, p2->processor);
    printf("%-22s | %-28s | %-28s\n", "RAM & Storage", p1->ram_storage,
           p2->ram_storage);
    printf("%-22s | %-28s | %-28s\n", "Camera", p1->camera, p2->camera);
    printf("%-22s | %-28s | %-28s\n", "Battery", p1->battery, p2->battery);
    printf("%-22s | %-28s | %-28s\n", "Display", p1->display, p2->display);
    printf("%-22s | %-28s | %-28s\n", "Operating System", p1->os, p2->os);
    printf("%-22s | Rs. %-24.2f | Rs. %-24.2f\n", "Price", p1->price, p2->price);
    printf("--------------------------------------------------------------------\n");

    if (p1->price < p2->price) {
        printf("\n%s is cheaper by Rs. %.2f\n", p1->model, p2->price - p1->price);
    } else if (p2->price < p1->price) {
        printf("\n%s is cheaper by Rs. %.2f\n", p2->model, p1->price - p2->price);
    } else {
        printf("\nBoth phones have the same price.\n");
    }
}

void addPhone(void) {
    MobilePhone phone;

    if (phoneCount >= MAX_PHONES) {
        printf("\nCannot add more phones. Maximum limit reached.\n");
        return;
    }

    printf("\n========== ADD NEW MOBILE PHONE ==========\n");
    readLine("Brand Name: ", phone.brand, sizeof(phone.brand));
    readLine("Model Name: ", phone.model, sizeof(phone.model));

    if (findByModel(phone.model) != -1) {
        printf("\nA phone with model '%s' already exists.\n", phone.model);
        return;
    }

    readLine("Processor Details: ", phone.processor, sizeof(phone.processor));
    readLine("RAM & Storage: ", phone.ram_storage, sizeof(phone.ram_storage));
    readLine("Camera Specifications: ", phone.camera, sizeof(phone.camera));
    readLine("Battery Capacity: ", phone.battery, sizeof(phone.battery));
    readLine("Display Size: ", phone.display, sizeof(phone.display));
    readLine("Operating System: ", phone.os, sizeof(phone.os));
    phone.price = readPrice("Price (Rs.): ");

    phones[phoneCount++] = phone;

    if (savePhones()) {
        printf("\nMobile phone added successfully.\n");
    }
}

void updatePhone(void) {
    char model[MODEL_LEN];
    int index;

    if (phoneCount == 0) {
        printf("\nNo mobile phones available to update.\n");
        return;
    }

    viewPhoneList();
    readLine("\nEnter model name to update: ", model, sizeof(model));
    index = findByModel(model);

    if (index == -1) {
        printf("\nPhone with model '%s' not found.\n", model);
        return;
    }

    printf("\nUpdating phone: %s %s\n", phones[index].brand, phones[index].model);
    printf("Press Enter to keep the current value.\n");

    readLine("Brand Name: ", model, sizeof(model));
    if (strlen(model) > 0) {
        strncpy(phones[index].brand, model, BRAND_LEN - 1);
        phones[index].brand[BRAND_LEN - 1] = '\0';
    }

    readLine("Processor Details: ", model, sizeof(model));
    if (strlen(model) > 0) {
        strncpy(phones[index].processor, model, PROC_LEN - 1);
        phones[index].processor[PROC_LEN - 1] = '\0';
    }

    readLine("RAM & Storage: ", model, sizeof(model));
    if (strlen(model) > 0) {
        strncpy(phones[index].ram_storage, model, RAM_LEN - 1);
        phones[index].ram_storage[RAM_LEN - 1] = '\0';
    }

    readLine("Camera Specifications: ", model, sizeof(model));
    if (strlen(model) > 0) {
        strncpy(phones[index].camera, model, CAMERA_LEN - 1);
        phones[index].camera[CAMERA_LEN - 1] = '\0';
    }

    readLine("Battery Capacity: ", model, sizeof(model));
    if (strlen(model) > 0) {
        strncpy(phones[index].battery, model, BATTERY_LEN - 1);
        phones[index].battery[BATTERY_LEN - 1] = '\0';
    }

    readLine("Display Size: ", model, sizeof(model));
    if (strlen(model) > 0) {
        strncpy(phones[index].display, model, DISPLAY_LEN - 1);
        phones[index].display[DISPLAY_LEN - 1] = '\0';
    }

    readLine("Operating System: ", model, sizeof(model));
    if (strlen(model) > 0) {
        strncpy(phones[index].os, model, OS_LEN - 1);
        phones[index].os[OS_LEN - 1] = '\0';
    }

    readLine("Price (Rs.) [leave blank to keep current]: ", model, sizeof(model));
    if (strlen(model) > 0) {
        float price;
        if (sscanf(model, "%f", &price) == 1 && price >= 0) {
            phones[index].price = price;
        } else {
            printf("Invalid price entered. Keeping previous price.\n");
        }
    }

    if (savePhones()) {
        printf("\nPhone details updated successfully.\n");
    }
}

void deletePhone(void) {
    char model[MODEL_LEN];
    char confirm[10];
    int index;
    int i;

    if (phoneCount == 0) {
        printf("\nNo mobile phones available to delete.\n");
        return;
    }

    viewPhoneList();
    readLine("\nEnter model name to delete: ", model, sizeof(model));
    index = findByModel(model);

    if (index == -1) {
        printf("\nPhone with model '%s' not found.\n", model);
        return;
    }

    displayPhone(&phones[index]);
    readLine("Are you sure you want to delete this record? (y/n): ", confirm,
             sizeof(confirm));

    if (confirm[0] != 'y' && confirm[0] != 'Y') {
        printf("\nDeletion cancelled.\n");
        return;
    }

    for (i = index; i < phoneCount - 1; i++) {
        phones[i] = phones[i + 1];
    }
    phoneCount--;

    if (savePhones()) {
        printf("\nPhone record deleted successfully.\n");
    }
}
