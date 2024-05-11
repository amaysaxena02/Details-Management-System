#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct student
{
    char nm[35];
    char cl[15];
    int rl;
    char dt[12];
};

FILE *fp;
long sz = sizeof(struct student);

// Function prototypes
void input();
void display();
void search();
void modify();
void deleteRecord();
void sort();
void fileInfo();
void pressAnyKeyToContinue();

int main()
{
    int ch;

    while (1)
    {
        system("cls");
        printf("<==== Class Management System ====>");
        printf("\n 1. Take Admission ");
        printf("\n 2. Student Info");
        printf("\n 3. Search");
        printf("\n 4. Modify");
        printf("\n 5. Delete");
        printf("\n 6. Sort");
        printf("\n 7. File Info");
        printf("\n 0. Exit");
        printf("\n\n Enter your choice: ");
        scanf("%d", &ch);

        switch (ch)
        {
        case 0:
            exit(0);

        case 1:
            input();
            pressAnyKeyToContinue();
            break;

        case 2:
            display();
            pressAnyKeyToContinue();
            break;

        case 3:
            search();
            pressAnyKeyToContinue();
            break;

        case 4:
            modify();
            pressAnyKeyToContinue();
            break;

        case 5:
            deleteRecord();
            pressAnyKeyToContinue();
            break;

        case 6:
            sort();
            pressAnyKeyToContinue();
            break;

        case 7:
            fileInfo();
            pressAnyKeyToContinue();
            break;

        default:
            printf("Invalid choice. Please try again.\n");
            pressAnyKeyToContinue();
            break;
        }
    }

    return 0;
}

void input()
{
    struct student s;

    char mydate[12];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(mydate, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    strcpy(s.dt, mydate);

    fp = fopen("stud.txt", "ab+");
    if (fp == NULL)
    {
        perror("Error opening file");
        return;
    }

    printf("Enter Roll: ");
    scanf("%d", &s.rl);

    // Check if the roll number already exists
    fseek(fp, 0, SEEK_SET);
    struct student existing;
    int found = 0;

    while (fread(&existing, sz, 1, fp) == 1)
    {
        if (existing.rl == s.rl)
        {
            found = 1;
            printf("A student with the same Roll number already exists. Admission not allowed.\n");
            pressAnyKeyToContinue();
            fclose(fp);
            return;
        }
    }

    if (!found)
    {
        printf("Enter Student name: ");
        fflush(stdin);
        fgets(s.nm, sizeof(s.nm), stdin);
        s.nm[strcspn(s.nm, "\n")] = '\0';

        printf("Enter Class: ");
        fgets(s.cl, sizeof(s.cl), stdin);
        s.cl[strcspn(s.cl, "\n")] = '\0';

        fseek(fp, 0, SEEK_END);
        fwrite(&s, sz, 1, fp);
        printf("Admission successful. Your record has been saved.\n");
    }

    fclose(fp);
    pressAnyKeyToContinue();
}

void display()
{
    printf("\nDisplaying Student Information:\n");

    // Open the file in read mode
    fp = fopen("stud.txt", "rb");
    if (fp == NULL)
    {
        perror("Error opening file");
        return;
    }

    struct student s;

    // Read and display each student's information
    while (fread(&s, sz, 1, fp) == 1)
    {
        printf("Name: %s\n", s.nm);
        printf("Class: %s\n", s.cl);
        printf("Roll: %d\n", s.rl);
        printf("Date: %s\n\n", s.dt);
    }

    fclose(fp);
    pressAnyKeyToContinue();
}

void search()
{
    printf("\nEnter Roll number to search for a student: ");
    int rollToSearch;
    scanf("%d", &rollToSearch);

    // Open the file in read mode
    fp = fopen("stud.txt", "rb");
    if (fp == NULL)
    {
        perror("Error opening file");
        return;
    }

    struct student s;
    int found = 0;

    while (fread(&s, sz, 1, fp) == 1)
    {
        if (s.rl == rollToSearch)
        {
            printf("Student Found:\n");
            printf("Name: %s\n", s.nm);
            printf("Class: %s\n", s.cl);
            printf("Roll: %d\n", s.rl);
            printf("Date: %s\n\n", s.dt);
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("Student with Roll number %d not found.\n", rollToSearch);
    }

    fclose(fp);
    pressAnyKeyToContinue();
}

void modify()
{
    printf("\nEnter Roll number to modify a student's record: ");
    int rollToModify;
    scanf("%d", &rollToModify);

    // Open the file in read mode
    fp = fopen("stud.txt", "r+b");
    if (fp == NULL)
    {
        perror("Error opening file");
        return;
    }

    struct student s;
    int found = 0;

    while (fread(&s, sz, 1, fp) == 1)
    {
        if (s.rl == rollToModify)
        {
            printf("Enter new Student name: ");
            fflush(stdin);
            fgets(s.nm, sizeof(s.nm), stdin);
            s.nm[strcspn(s.nm, "\n")] = '\0';

            printf("Enter new Class: ");
            fgets(s.cl, sizeof(s.cl), stdin);
            s.cl[strcspn(s.cl, "\n")] = '\0';

            printf("Enter new Roll: ");
            scanf("%d", &s.rl);

            fseek(fp, -sz, SEEK_CUR);
            fwrite(&s, sz, 1, fp);
            found = 1;
            printf("Student record modified successfully.\n");
            break;
        }
    }

    if (!found)
    {
        printf("Student with Roll number %d not found.\n", rollToModify);
    }

    fclose(fp);
    pressAnyKeyToContinue();
}

void deleteRecord()
{
    printf("\nEnter Roll number to delete a student's record: ");
    int rollToDelete;
    scanf("%d", &rollToDelete);

    // Open the file in read mode
    fp = fopen("stud.txt", "rb");
    if (fp == NULL)
    {
        perror("Error opening file");
        return;
    }

    FILE *tempFile = fopen("temp.txt", "wb");
    if (tempFile == NULL)
    {
        perror("Error creating temporary file");
        fclose(fp);
        return;
    }

    struct student s;
    int found = 0;

    while (fread(&s, sz, 1, fp) == 1)
    {
        if (s.rl == rollToDelete)
        {
            found = 1;
            printf("Student record with Roll number %d deleted.\n", rollToDelete);
        }
        else
        {
            fwrite(&s, sz, 1, tempFile);
        }
    }

    fclose(fp);
    fclose(tempFile);

    // Remove the original file and rename the temporary file
    remove("stud.txt");
    rename("temp.txt", "stud.txt");
    pressAnyKeyToContinue();
}

void sort()
{
    printf("\nSorting student records by Roll number...\n");

    fp = fopen("stud.txt", "r+b");
    if (fp == NULL)
    {
        perror("Error opening file");
        return;
    }

    fseek(fp, 0, SEEK_END);
    long numRecords = ftell(fp) / sz;

    if (numRecords <= 1)
    {
        printf("No need to sort; there are 0 or 1 records.\n");
        fclose(fp);
        pressAnyKeyToContinue();
        return;
    }

    struct student s1, s2;
    int swapped;

    do
    {
        swapped = 0;
        fseek(fp, 0, SEEK_SET);

        for (int i = 0; i < numRecords - 1; i++)
        {
            fread(&s1, sz, 1, fp);
            fread(&s2, sz, 1, fp);

            if (s1.rl > s2.rl)
            {
                fseek(fp, -2 * sz, SEEK_CUR);
                fwrite(&s2, sz, 1, fp);
                fseek(fp, sz, SEEK_CUR);
                fwrite(&s1, sz, 1, fp);
                swapped = 1;
            }
            else
            {
                fseek(fp, -sz, SEEK_CUR);
            }
        }
    } while (swapped);

    printf("Student records sorted by Roll number.\n");

    fseek(fp, 0, SEEK_SET);
    while (fread(&s1, sz, 1, fp) == 1)
    {
        printf("Name: %s\n", s1.nm);
        printf("Class: %s\n", s1.cl);
        printf("Roll: %d\n", s1.rl);
        printf("Date: %s\n\n", s1.dt);
    }

    fclose(fp);
    pressAnyKeyToContinue();
}

void fileInfo()
{
    printf("\nFile Information:\n");

    fp = fopen("stud.txt", "rb");
    if (fp == NULL)
    {
        perror("Error opening file");
        return;
    }

    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    printf("File Size: %ld bytes\n", fileSize);

    fclose(fp);
    pressAnyKeyToContinue();
}

void pressAnyKeyToContinue()
{
    printf("Press Enter to continue...");
    getchar();
}
