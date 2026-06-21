#ifndef MOBILE_H
#define MOBILE_H

#define MAX_PHONES 100
#define FILENAME "phones.dat"

#define BRAND_LEN 50
#define MODEL_LEN 50
#define PROC_LEN 80
#define RAM_LEN 50
#define CAMERA_LEN 100
#define BATTERY_LEN 30
#define DISPLAY_LEN 30
#define OS_LEN 30

typedef struct {
    char brand[BRAND_LEN];
    char model[MODEL_LEN];
    char processor[PROC_LEN];
    char ram_storage[RAM_LEN];
    char camera[CAMERA_LEN];
    char battery[BATTERY_LEN];
    char display[DISPLAY_LEN];
    char os[OS_LEN];
    float price;
} MobilePhone;

extern MobilePhone phones[MAX_PHONES];
extern int phoneCount;

int loadPhones(void);
int savePhones(void);
void seedSampleData(void);

void displayPhone(const MobilePhone *phone);
void displayPhoneBrief(const MobilePhone *phone, int index);
void displayAllPhones(void);
void viewPhoneList(void);
void viewPhonePrices(void);

int findByModel(const char *model);
int findByBrand(const char *brand, int results[], int maxResults);

void displayPhoneSpecs(void);
void searchByModel(void);
void searchByBrand(void);
void comparePhones(void);

void addPhone(void);
void updatePhone(void);
void deletePhone(void);

void clearInputBuffer(void);

#endif
