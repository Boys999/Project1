#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Book {
    int id;
    char title[100];
    char author[100];
};

void addBook() {
    struct Book b;
    FILE *fp = fopen("library.dat", "ab");
    if (!fp) {
        printf("Unable to open file!\n");
        return;
    }

    printf("Enter Book ID: ");
    scanf("%d", &b.id);
    getchar(); // consume newline
    printf("Enter Book Title: ");
    fgets(b.title, sizeof(b.title), stdin);
    b.title[strcspn(b.title, "\n")] = 0; // remove newline
    printf("Enter Book Author: ");
    fgets(b.author, sizeof(b.author), stdin);
    b.author[strcspn(b.author, "\n")] = 0;

    fwrite(&b, sizeof(b), 1, fp);
    fclose(fp);
    printf("Book added successfully!\n");
}

void displayBooks() {
    struct Book b;
    FILE *fp = fopen("library.dat", "rb");
    if (!fp) {
        printf("No books found.\n");
        return;
    }

    printf("\n%-10s %-30s %-30s\n", "Book ID", "Title", "Author");
    while (fread(&b, sizeof(b), 1, fp)) {
        printf("%-10d %-30s %-30s\n", b.id, b.title, b.author);
    }

    fclose(fp);
}

void searchBook() {
    int id;
    struct Book b;
    FILE *fp = fopen("library.dat", "rb");
    if (!fp) {
        printf("No books found.\n");
        return;
    }

    printf("Enter Book ID to search: ");
    scanf("%d", &id);
    int found = 0;

    while (fread(&b, sizeof(b), 1, fp)) {
        if (b.id == id) {
            printf("Book found!\n");
            printf("ID: %d\nTitle: %s\nAuthor: %s\n", b.id, b.title, b.author);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Book not found.\n");

    fclose(fp);
}

void deleteBook() {
    int id;
    struct Book b;
    FILE *fp = fopen("library.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");

    if (!fp || !temp) {
        printf("File error.\n");
        return;
    }

    printf("Enter Book ID to delete: ");
    scanf("%d", &id);
    int deleted = 0;

    while (fread(&b, sizeof(b), 1, fp)) {
        if (b.id != id) {
            fwrite(&b, sizeof(b), 1, temp);
        } else {
            deleted = 1;
        }
    }

    fclose(fp);
    fclose(temp);
    remove("library.dat");
    rename("temp.dat", "library.dat");

    if (deleted)
        printf("Book deleted successfully.\n");
    else
        printf("Book ID not found.\n");
}

int main() {
    int choice;

    do {
        printf("\nLibrary Management System\n");
        printf("1. Add Book\n2. Display Books\n3. Search Book\n4. Delete Book\n5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // flush newline

        switch (choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: searchBook(); break;
            case 4: deleteBook(); break;
            case 5: printf("Exiting...\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 5);

    return 0;
}