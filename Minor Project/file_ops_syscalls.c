#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

// Buffer size for reading
#define BUFFER_SIZE 1024

void print_menu(char *filename, int fd) {
    printf("\n--- File Operations Menu ---\n");
    if (fd != -1) {
        printf("[STATUS] Current Open File: '%s' (FD: %d)\n", filename, fd);
    } else {
        printf("[STATUS] No file currently open.\n");
    }
    printf("----------------------------\n");
    printf("1. Create/Open File (O_CREAT | O_RDWR)\n");
    printf("2. Write to File\n");
    printf("3. Read from File\n");
    printf("4. Lseek (Reposition Offset)\n");
    printf("5. Close File\n");
    printf("6. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    int fd = -1; // File descriptor, initialized to -1 (invalid)
    int choice;
    char filename[100];
    char buffer[BUFFER_SIZE];
    ssize_t bytes_processed;
    off_t offset;
    int whence_choice;
    int whence;

    while (1) {
        print_menu(filename, fd);
        if (scanf("%d", &choice) != 1) {
            // Clear input buffer if invalid input
            while(getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        // Consume newline left by scanf
        getchar();

        switch (choice) {
            case 1: // Open/Create
                if (fd != -1) {
                    printf("[WARNING] File is already open (fd: %d). Close it first.\n", fd);
                } else {
                    printf("Enter filename to open/create: ");
                    fgets(filename, sizeof(filename), stdin);
                    filename[strcspn(filename, "\n")] = 0; // Remove newline

                    // Open for Read/Write, Create if not exists, 0644 permissions
                    fd = open(filename, O_RDWR | O_CREAT, 0644);
                    if (fd == -1) {
                        perror("Error opening file");
                    } else {
                        printf("[SUCCESS] File opened. File Descriptor: %d\n", fd);
                    }
                }
                break;

            case 2: // Write
                if (fd == -1) {
                    printf("[ERROR] No file open. Please open a file first.\n");
                } else {
                    printf("Enter content to write: ");
                    fgets(buffer, sizeof(buffer), stdin);
                    // Note: fgets includes the newline character if there is space
                    bytes_processed = write(fd, buffer, strlen(buffer));
                    if (bytes_processed == -1) {
                        perror("Error writing to file");
                    } else {
                        printf("[SUCCESS] Written %zd bytes.\n", bytes_processed);
                    }
                }
                break;

            case 3: // Read
                if (fd == -1) {
                    printf("[ERROR] No file open. Please open a file first.\n");
                } else {
                    int bytes_to_read;
                    printf("Enter number of bytes to read: ");
                    scanf("%d", &bytes_to_read);
                    if (bytes_to_read >= BUFFER_SIZE) bytes_to_read = BUFFER_SIZE - 1;
                    
                    bytes_processed = read(fd, buffer, bytes_to_read);
                    if (bytes_processed == -1) {
                        perror("Error reading file");
                    } else if (bytes_processed == 0) {
                        printf("[INFO] End of File (EOF) reached.\n");
                    } else {
                        buffer[bytes_processed] = '\0'; // Null-terminate
                        printf("[SUCCESS] Read: \"%s\"\n", buffer);
                    }
                }
                break;

            case 4: // Lseek
                if (fd == -1) {
                    printf("[ERROR] No file open. Please open a file first.\n");
                } else {
                    printf("Select whence (0: SEEK_SET, 1: SEEK_CUR, 2: SEEK_END): ");
                    scanf("%d", &whence_choice);
                    
                    switch(whence_choice) {
                        case 0: whence = SEEK_SET; break;
                        case 1: whence = SEEK_CUR; break;
                        case 2: whence = SEEK_END; break;
                        default: whence = SEEK_SET; printf("[WARN] Invalid choice, defaulting to SEEK_SET\n"); break;
                    }

                    printf("Enter offset: ");
                    scanf("%ld", &offset);

                    off_t new_pos = lseek(fd, offset, whence);
                    if (new_pos == (off_t)-1) {
                        perror("Error seeking");
                    } else {
                        printf("[SUCCESS] New file offset: %ld\n", (long)new_pos);
                    }
                }
                break;

            case 5: // Close
                if (fd == -1) {
                    printf("[INFO] No file is currently open.\n");
                } else {
                    if (close(fd) == -1) {
                        perror("Error closing file");
                    } else {
                        printf("[SUCCESS] File closed.\n");
                        fd = -1;
                    }
                }
                break;

            case 6: // Exit
                if (fd != -1) {
                    close(fd);
                    printf("Closing open file...\n");
                }
                printf("Exiting program.\n");
                exit(0);

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}
