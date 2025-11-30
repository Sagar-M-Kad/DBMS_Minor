#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECORDS 100
#define DATA_FILE "students.dat"

// Structure for the Data Record
typedef struct {
    int id;
    char name[50];
    float cgpa;
    int deleted; // 0 = active, 1 = deleted
} Student;

// Structure for the Index Entry
typedef struct {
    int id;     // Key
    long offset; // Position in the file
} Index;

// Global Index Table
Index indexTable[MAX_RECORDS];
int indexCount = 0;

// Function to load index from file (if we were persisting it)
// For this demo, we rebuild index on startup or just keep in memory
void load_index() {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp) return; // File doesn't exist yet

    Student temp;
    indexCount = 0;

    while (fread(&temp, sizeof(Student), 1, fp)) {
        // Calculate offset: current position minus the size we just read
        long currentPos = ftell(fp);
        long recordOffset = currentPos - sizeof(Student);
        
        // Only index non-deleted records
        if (temp.deleted == 0) {
            indexTable[indexCount].id = temp.id;
            indexTable[indexCount].offset = recordOffset;
            indexCount++;
        }
    }
    fclose(fp);
    printf("[INFO] Index loaded. %d records found.\n", indexCount);
}

void add_record() {
    FILE *fp = fopen(DATA_FILE, "ab"); // Append Binary
    if (!fp) {
        perror("Error opening file");
        return;
    }

    Student s;
    printf("\nEnter Student ID: ");
    scanf("%d", &s.id);
    s.deleted = 0; // Mark as active
    
    // Check for duplicate ID in index
    for(int i=0; i<indexCount; i++) {
        if(indexTable[i].id == s.id) {
            printf("[ERROR] Duplicate ID! Record not added.\n");
            fclose(fp);
            return;
        }
    }

    printf("Enter Name: ");
    scanf(" %[^\n]s", s.name); // Read string with spaces
    printf("Enter CGPA: ");
    scanf("%f", &s.cgpa);

    // 1. Get current offset BEFORE writing
    long offset = ftell(fp);

    // 2. Write record to data file
    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);

    // 3. Update Index Table
    indexTable[indexCount].id = s.id;
    indexTable[indexCount].offset = offset;
    indexCount++;

    printf("[SUCCESS] Record added at offset %ld.\n", offset);
    printf("[DEBUG] Index now has %d records.\n", indexCount);
}

void search_record() {
    int searchId;
    printf("\nEnter ID to search: ");
    scanf("%d", &searchId);

    // 1. Search in Index Table (Binary Search could be used here for sorted index)
    long foundOffset = -1;
    for (int i = 0; i < indexCount; i++) {
        if (indexTable[i].id == searchId) {
            foundOffset = indexTable[i].offset;
            break;
        }
    }

    if (foundOffset == -1) {
        printf("[NOT FOUND] Student with ID %d does not exist.\n", searchId);
        return;
    }

    // 2. Retrieve from File using Offset
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp) {
        perror("Error opening file");
        return;
    }

    // JUMP directly to the record
    fseek(fp, foundOffset, SEEK_SET);

    Student s;
    fread(&s, sizeof(Student), 1, fp);
    fclose(fp);

    if (s.deleted == 1) {
        printf("[ERROR] This record has been deleted.\n");
        return;
    }

    printf("\n[FOUND] Record Retrieved:\n");
    printf("ID: %d\n", s.id);
    printf("Name: %s\n", s.name);
    printf("CGPA: %.2f\n", s.cgpa);
}

void delete_record() {
    int deleteId;
    printf("\nEnter ID to delete: ");
    scanf("%d", &deleteId);

    // 1. Search in Index Table
    long foundOffset = -1;
    int indexPos = -1;
    for (int i = 0; i < indexCount; i++) {
        if (indexTable[i].id == deleteId) {
            foundOffset = indexTable[i].offset;
            indexPos = i;
            break;
        }
    }

    if (foundOffset == -1) {
        printf("[NOT FOUND] Student with ID %d does not exist.\n", deleteId);
        return;
    }

    // 2. Mark as deleted in the file
    FILE *fp = fopen(DATA_FILE, "r+b"); // Read and Write
    if (!fp) {
        perror("Error opening file");
        return;
    }

    fseek(fp, foundOffset, SEEK_SET);
    Student s;
    fread(&s, sizeof(Student), 1, fp);
    
    s.deleted = 1; // Mark as deleted
    
    // Write back to the same position
    fseek(fp, foundOffset, SEEK_SET);
    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);

    // 3. Remove from index table
    for (int i = indexPos; i < indexCount - 1; i++) {
        indexTable[i] = indexTable[i + 1];
    }
    indexCount--;

    printf("[SUCCESS] Record with ID %d deleted.\n", deleteId);
}

void display_index() {
    printf("\n--- Current Index Table ---\n");
    printf("ID\t|\tOffset\n");
    printf("------------------------\n");
    for (int i = 0; i < indexCount; i++) {
        printf("%d\t|\t%ld\n", indexTable[i].id, indexTable[i].offset);
    }
    printf("------------------------\n");
    printf("Total active records: %d\n", indexCount);
}

void display_all_records() {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp) {
        printf("[ERROR] No data file found.\n");
        return;
    }

    Student s;
    int count = 0;
    long offset = 0;

    printf("\n--- All Records in File ---\n");
    printf("Offset\t| ID\t| Name\t\t| CGPA\t| Status\n");
    printf("--------------------------------------------------------\n");

    while (fread(&s, sizeof(Student), 1, fp)) {
        printf("%ld\t| %d\t| %-15s| %.2f\t| %s\n", 
               offset, s.id, s.name, s.cgpa, 
               s.deleted ? "DELETED" : "ACTIVE");
        count++;
        offset = ftell(fp);
    }
    
    fclose(fp);
    printf("--------------------------------------------------------\n");
    printf("Total records in file: %d\n", count);
}

int main() {
    int choice;
    
    // Load existing data to build index
    load_index();

    while (1) {
        printf("\n=== Indexing Demo Menu ===\n");
        printf("1. Add Record\n");
        printf("2. Search Record (using Index)\n");
        printf("3. Delete Record\n");
        printf("4. Display Index Table\n");
        printf("5. Display All Records (Debug)\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: add_record(); break;
            case 2: search_record(); break;
            case 3: delete_record(); break;
            case 4: display_index(); break;
            case 5: display_all_records(); break;
            case 6: exit(0);
            default: printf("Invalid choice.\n");
        }
    }
    return 0;
}
