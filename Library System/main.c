#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILENAME "books.txt"
#define MAX_NAME_LENGTH 50
#define MAX_CURRENCY_LENGTH 20

typedef struct
{
    char bookName[MAX_NAME_LENGTH];
    char writerName[MAX_NAME_LENGTH];
    char currency[MAX_CURRENCY_LENGTH];
    int releaseYear;
    float price;
} Book;

int sortOrder;//Specify sort side to sort books by price

int calculateBookCount(void);
Book* changeArraySize(Book* books, int);
void menuOfProgram(Book* books, int);
void addArchiveToArray(Book* books, int*);
void addBook(Book* books, int, int*);
void listBooks(Book* books, int, int, int);
void deleteBook(Book* books, char*, int*);
void searchBook(Book* books, int, int);
void deleteAllBooks(Book* books, int*);
void sortBooks(Book* books, int, int);
int compareByPrice(const void*, const void*);
int compareByBooks(const void*, const void*);
void clearInputBuffer(void);
void clearStringBuffer(char*);
void clearStructBuffer(Book* books, int);
void toUpperCase(char*);

int main()
{
    int bookCount = calculateBookCount();

    Book* books = (Book*)malloc((bookCount + 1) * sizeof(Book));

    if (books == NULL)
    {
        perror("Failed to allocate memory for books");
        exit(1);
    }

    addArchiveToArray(books, &bookCount);

    menuOfProgram(books, bookCount);

    free(books);

    return 0;
}

int calculateBookCount(void)
{
    FILE* file = fopen(FILENAME, "r");

    if (file == NULL)
        return 0;

    int numberOfLines = 0;
    char lines[MAX_NAME_LENGTH];

    while (fgets(lines, sizeof(lines), file) != NULL)
        numberOfLines++;

    return numberOfLines / 5;
}

Book* changeArraySize(Book* books, int bookCount)
{
    Book* temp = (Book*)realloc(books, (bookCount + 1) * sizeof(Book));

    if (temp == NULL)
    {
        perror("Failed to allocate memory for temp");

        free(books);

        exit(1);
    }

    return temp;
}

void menuOfProgram(Book* books, int bookCount)
{
    int choice;
    int choice2;

    do
    {
        printf("--- Library System ---\n");
        printf("1. Add book\n2. List all books\n3. Delete book\n4. Search book\n5. Sort books\n6. Delete all books\n7. Exit");

        printf("\n\nPlease make a choice: ");
        scanf("%d", &choice);

        clearInputBuffer();

        switch (choice)
        {
            case 1:
            {
                printf("\nAdd new book: \n");
                printf("--------------------------\n");

                int bookWarning = 0;

                addBook(books, bookCount, &bookWarning);

                if (bookWarning == 0)
                {
                    bookCount++;

                    books = changeArraySize(books, bookCount);
                }

                break;
            }
            case 2:
            {
                printf("\nAll Books: \n");
                printf("--------------------------\n");

                listBooks(books, bookCount, 0, 0);

                break;
            }
            case 3:
            {
                char bookName[MAX_NAME_LENGTH];

                printf("\nEnter the name of the book you want to delete: ");
                fgets(bookName, sizeof(bookName), stdin);

                clearStringBuffer(bookName);
                toUpperCase(bookName);

                deleteBook(books, bookName, &bookCount);

                books = changeArraySize(books, bookCount);

                break;
            }
            case 4:
            {
                do
                {
                    printf("\nDo you want to search with book name,author,release year or price?\n");

                    printf("\nPress 1 (Book name)\nPress 2 (Writer name)\nPress 3 (Release year)\nPress 4 (Price): ");
                    scanf("%d", &choice2);

                    clearInputBuffer();
                    printf("\n");

                    if (choice2 > 4 || choice2 < 1)
                        printf("Invalid value! Please try again.\n");

                } while (choice2 > 4 || choice2 < 1);

                searchBook(books, bookCount, choice2);

                break;
            }
            case 5:
            {
                do
                {
                    printf("\nDo you want to sort by alphabetically or by price?\n");

                    printf("\nPress 1 (Price)\nPress 2 (Alphabetically): ");
                    scanf("%d", &choice2);

                    clearInputBuffer();
                    printf("\n");

                    if (choice2 > 2 || choice2 < 1)
                        printf("Invalid value! Please try again.\n");

                } while(choice2 > 2 || choice2 < 1);

                sortBooks(books, bookCount, choice2);

                break;
            }
            case 6:
            {
                do
                {
                    printf("\nAre you completely sure that you want to delete ALL BOOKS?\n");

                    printf("\nPress 1 (Delete)\nPress 2 (Back to menu): ");
                    scanf("%d", &choice2);

                    clearInputBuffer();
                    printf("\n");

                    if (choice2 > 2 || choice2 < 1)
                        printf("Invalid value! Please try again.\n");

                } while(choice2 > 2 || choice2 < 1);

                if (choice2 == 1)
                    deleteAllBooks(books, &bookCount);

                books = changeArraySize(books, bookCount);

                break;
            }
            case 7:
            {
                printf("\nProgram is closed.\n");

                break;
            }
            default:
            {
                printf("\nInvalid choice! Please try again.\n\n");

                break;
            }
        }

    } while (choice != 7);
}

void toUpperCase(char* str)//Makes string's all letters to capital letters.
{
    for (int i = 0;str[i] != '\0';i++)
        str[i] = toupper(str[i]);
}

void clearInputBuffer(void)
{
    while ((getchar()) != '\n');
}

void clearStringBuffer(char* buffer)
{
    int size = strlen(buffer);

    buffer[size - 1] = '\0';
}

void clearStructBuffer(Book* book, int option)
{
    if (option == 1)
    {
        int size = strlen(book->bookName);

        if (book->bookName[size - 1] == '\n')
            book->bookName[size - 1] = '\0';
    }
    else if (option == 2)
    {
        int size = strlen(book->writerName);

        if (book->writerName[size - 1] == '\n')
            book->writerName[size - 1] = '\0';
    }
    else
    {
        int size = strlen(book->currency);

        if (book->currency[size - 1] == '\n')
            book->currency[size - 1] = '\0';
    }
}

void addArchiveToArray(Book* books, int* bookCount)
{
    int arrayIndex = 0;

    FILE* file = fopen(FILENAME, "r");

    if (file == NULL)
    {
        printf("There is no any book.Please add a book: \n");

        addBook(books, arrayIndex, 0);

        (*bookCount) = 1;

        books = changeArraySize(books, 1);

        return;
    }

    char buf[MAX_NAME_LENGTH];
    int check = 0;

    while (fgets(buf, sizeof(buf), file) != NULL)
    {
        if (check == 0)
        {
            strcpy(books[arrayIndex].bookName, buf + 6);

            clearStructBuffer(&books[arrayIndex], 1);
        }

        else if (check == 1)
        {
            strcpy(books[arrayIndex].writerName, buf + 8);

            clearStructBuffer(&books[arrayIndex], 2);
        }

        else if (check == 2)
            sscanf(buf, "Release Year: %d", &books[arrayIndex].releaseYear);

        else if (check == 3)
            sscanf(buf, "Price: %f %[^\n]", &books[arrayIndex].price, books[arrayIndex].currency);

        check++;

        if (buf[0] == '-')
        {
            arrayIndex++;
            check = 0;
        }
    }

    fclose(file);
}

void addBook(Book* books, int bookCount, int* bookWarning)
{
    printf("\nBook Name: ");
    fgets(books[bookCount].bookName, sizeof(books[bookCount].bookName), stdin);

    clearStructBuffer(&books[bookCount], 1);
    toUpperCase(books[bookCount].bookName);

    printf("Writer Name: ");
    fgets(books[bookCount].writerName, sizeof(books[bookCount].writerName), stdin);

    clearStructBuffer(&books[bookCount], 2);
    toUpperCase(books[bookCount].writerName);

    for (int i = 0;i < bookCount;i++)
    {
        if (strcmp(books[i].bookName, books[bookCount].bookName) == 0 && strcmp(books[i].writerName, books[bookCount].writerName) == 0)
        {
            printf("\nThis book is already exist!.If you want to add new one please try again.\n\n");

            (*bookWarning) = 1;

            return;
        }
    }

    printf("Release Year: ");
    scanf("%d", &books[bookCount].releaseYear);

    clearInputBuffer();

    if (bookCount == 0)
    {
        printf("[Please write a currency type for all books]: ");
        fgets(books[bookCount].currency, sizeof(books[bookCount].currency), stdin);

        clearStructBuffer(&books[bookCount], 3);
        toUpperCase(books[bookCount].currency);
    }
    else
        strcpy(books[bookCount].currency, books[0].currency);

    printf("Price: ");
    scanf("%f", &books[bookCount].price);

    while (books[bookCount].price < 0)
    {
        printf("Wrong price!Please try again.\n");

        printf("Price: ");
        scanf("%f", &books[bookCount].price);
    }

    clearInputBuffer();

    printf("\nThe book added successfully!\n\n");

    FILE* file = fopen(FILENAME, "a");

    if (file == NULL)
    {
        printf("\nNo such file or directory!\n");

        free(books);

        exit(1);
    }

    fprintf(file, "Book: %s\nWriter: %s\n", books[bookCount].bookName, books[bookCount].writerName);
    fprintf(file, "Release Year: %d\nPrice: %.2f %s\n", books[bookCount].releaseYear, books[bookCount].price, books[bookCount].currency);
    fprintf(file, "--------------------------\n");

    fclose(file);
}

void listBooks(Book* books, int bookCount, int i, int control)
{
    if (control == 0)
    {
        int k;

        for (k = 0;k < bookCount;k++)
        {
            printf("\n%d.Book: %s\n\n", k + 1, books[k].bookName);
            printf("Writer: %s\n", books[k].writerName);
            printf("Release Year: %d\n", books[k].releaseYear);
            printf("Price: %.2f %s\n", books[k].price, books[k].currency);
            printf("--------------------------\n\n");
        }

        if (k == 0)
            printf("There are no any books.\n\n");
    }
    else
    {
        printf("\n%d.Book: %s\n\n", i + 1, books[i].bookName);
        printf("Writer: %s\n", books[i].writerName);
        printf("Release Year: %d\n", books[i].releaseYear);
        printf("Price: %.2f %s\n", books[i].price, books[i].currency);
        printf("--------------------------\n\n");
    }

}

void sortBooks(Book* books, int bookCount, int choice)
{
    if(choice == 1)
    {
        printf("\nDo you want to sort by ascending or by descending?\n");

        printf("\nPress 1 (Sort by price ascending)\n");
        printf("Press 2 (Sort by price descending): ");
        scanf("%d", &sortOrder);

        while (sortOrder != 1 && sortOrder != 2)
        {
            printf("\nInvalid choice!.Please choose a sort type.\n");

            printf("\nPress 1 (Sort by price ascending)\n");
            printf("Press 2 (Sort by price descending): ");
            scanf("%d", &sortOrder);
        }

        qsort(books, bookCount, sizeof(Book), compareByPrice);
    }

    else
        qsort(books, bookCount, sizeof(Book), compareByBooks);

    printf("--------------------------\n\n");
    listBooks(books, bookCount, 0, 0);
}

int compareByPrice(const void *a, const void *b)
{
    const Book *bookA = (const Book *)a;
    const Book *bookB = (const Book *)b;

    if (bookA->price > bookB->price)
        return 1 * sortOrder;
    if (bookA->price < bookB->price)
        return -1 * sortOrder;

    return 0;
}

int compareByBooks(const void *a, const void *b)
{
    const Book *bookA = (const Book *)a;
    const Book *bookB = (const Book *)b;

    return strcmp(bookA->bookName, bookB->bookName);
}

void deleteBook(Book* books, char* bookName2, int* bookCount)
{
    int control = 0;

    for (int k = 0;k < *bookCount;k++)
    {
        if (strcmp(books[k].bookName, bookName2) == 0)
        {
            control = 1;
            break;
        }
    }

    if (control == 1)
    {
        int count = 0;
        int tempBookCount = *bookCount;

        for (int k = 0;k < tempBookCount;k++)
        {
            if (strcmp(books[k].bookName, bookName2) == 0)
            {
                count++;

                listBooks(books, tempBookCount, k, 1);
            }
        }

        char writerName2[MAX_NAME_LENGTH];//we use this string and count variable if there are books with same names.

        if (count > 1)
        {
            printf("\nPlease enter the name of the author for delete: ");
            fgets(writerName2, sizeof(writerName2), stdin);

            clearStringBuffer(writerName2);
            toUpperCase(writerName2);

            printf("--------------------------\n");
        }

        if (count > 1)
            printf("\nAre you sure you want to delete \"%s/%s\" book?", bookName2, writerName2);
        else
            printf("\nAre you sure you want to delete \"%s\" book?", bookName2);


        int choice;

        printf("\n\nPress 1 (Delete)\nPress 2 (Back to menu): ");
        scanf("%d", &choice);

        clearInputBuffer();
        printf("\n");

        if (choice == 2)
            return;

        while (choice != 1)
        {
            printf("Invalid choice.Please try again.");

            printf("\n\nPress 1 (Delete)\nPress 2 (Back to menu): ");
            scanf("%d", &choice);

            clearInputBuffer();
            printf("\n");
        }

        FILE* file = fopen(FILENAME, "r");
        FILE* tempFile = fopen("temp.txt", "w");

        if (file == NULL || tempFile == NULL)
        {
            printf("\nNo such file or directory!\n");

            free(books);

            exit(1);
        }

        char buf[MAX_NAME_LENGTH];
        char buf2[MAX_NAME_LENGTH];
        char currentBuf[MAX_NAME_LENGTH];
        int book_check = 0;
        int check2 = 0;
        int index = 0;
        int index_check = 0;
        int index_check2 = 0;
        int count2 = 0;

        while (fgets(buf, sizeof(buf), file) != NULL)//Deletes from file.
        {
            if (strstr(buf, "Book: ") != NULL)
            {
                strcpy(currentBuf, buf + 6);

                clearStringBuffer(currentBuf);

                if (strcmp(currentBuf, bookName2) == 0)
                {
                    book_check = 1;
                    index_check2 = 1;
                }

                else
                    book_check = 0;
            }

            if (count > 1 && book_check == 1)//Deletes from file if there are multiple books with same names.
            {
                strcpy(buf2, buf);

                if (check2 == 0)
                    fgets(buf, sizeof(buf), file);

                if (strstr(buf, "Writer: ") != NULL && strstr(buf, writerName2) != NULL)
                {
                    check2 = 1;
                    index_check = 1;
                }

                else if (check2 == 1)
                    count2++;
                else
                {
                    book_check = 0;
                    fprintf(tempFile, "%s", buf2);
                }

                if (count2 == 3)
                {
                    check2 = 0;
                    count2 = 0;
                }
            }

            if (buf[0] == '-' && count > 1 && index_check == 0)//Index for struct array.If there are multiple books with same names.
                index++;

            if (buf[0] == '-' && count == 1 && index_check2 == 0)//Index for struct array.If there is one book.
                index++;

            if (book_check == 0)
                fprintf(tempFile, "%s", buf);
        }

        fclose(file);
        fclose(tempFile);

        char line[MAX_NAME_LENGTH];

        file = fopen(FILENAME, "w");
        tempFile = fopen("temp.txt", "r");

        if (file == NULL || tempFile == NULL)
        {
            printf("\nNo such file or directory!\n");

            free(books);

            exit(1);
        }

        while (fgets(line, sizeof(line), tempFile))
            fputs(line, file);

        fclose(file);
        fclose(tempFile);

        for (int k = index;k < (*bookCount) - 1;k++)//Deletes from struct array
            books[k] = books[k + 1];

        (*bookCount)--;

        printf("\nThe book deleted successfully.\n\n");
    }

    else
        printf("\nThere is no such book.\n\n");
}

void searchBook(Book* books, int bookCount, int choice)
{
    int check = 0;

    if (choice == 1)
    {
        char bookName1[MAX_NAME_LENGTH];

        printf("\nEnter the name of the book you want to search: ");
        fgets(bookName1, sizeof(bookName1), stdin);

        clearStringBuffer(bookName1);
        toUpperCase(bookName1);

        printf("--------------------------\n");

        for (int k = 0;k < bookCount;k++)
        {
            if (strstr(books[k].bookName, bookName1) != NULL)
            {
                listBooks(books, bookCount, k, 1);

                check = 1;
            }
        }

        if (check == 0)
            printf("\nThere is no such book.\n\n");
    }

    else if (choice == 2)
    {
        char writerName1[MAX_NAME_LENGTH];

        printf("\nEnter the name of the author of the book you want to search: ");
        fgets(writerName1, sizeof(writerName1), stdin);

        clearStringBuffer(writerName1);
        toUpperCase(writerName1);

        printf("--------------------------\n");

        for (int k = 0;k < bookCount;k++)
        {
            if (strstr(books[k].writerName, writerName1) != NULL)
            {
                listBooks(books, bookCount, k, 1);

                check = 1;
            }
        }

        if (check == 0)
            printf("\nThere is no such book for this author.\n\n");
    }

    else if (choice == 3)
    {
        int firstY, secondY;

        printf("\nPlease write a release year range: ");

        printf("\n\nLittle year: ");
        scanf("%d", &firstY);

        printf("Big year: ");
        scanf("%d", &secondY);

        printf("--------------------------\n");

        for (int k = 0;k < bookCount;k++)
        {
            if (books[k].releaseYear >= firstY && books[k].releaseYear <= secondY)
            {
                listBooks(books, bookCount, k, 1);

                check = 1;
            }
        }

        if (check == 0)
            printf("\nThere is no such book for this price range.\n\n");
    }

    else
    {
        float firstP, secondP;

        printf("\nPlease write a price range: ");

        printf("\n\nSmall price: ");
        scanf("%f", &firstP);

        printf("Big price: ");
        scanf("%f", &secondP);

        printf("--------------------------\n");

        for (int k = 0;k < bookCount;k++)
        {
            if (books[k].price >= firstP && books[k].price <= secondP)
            {
                listBooks(books, bookCount, k, 1);

                check = 1;
            }
        }

        if (check == 0)
            printf("\nThere is no such book for this release year range.\n\n");

    }
}

void deleteAllBooks(Book* books, int* bookCount)
{
    FILE* file = fopen(FILENAME, "w");
    FILE* tempFile = fopen("temp.txt", "w");

    if (file == NULL || tempFile == NULL)
    {
        printf("\nNo such file or directory!\n");
    }

    fclose(file);
    fclose(tempFile);

    memset(books, 0, (*bookCount) * sizeof(Book));

    (*bookCount) = 0;

    printf("All books deleted succesfully.\n\n");
}
