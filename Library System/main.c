#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "books.txt"
#define MAX_LENGTH 100
#define MAX_NAME_LENGTH 50
#define MAX_CURRENCY_LENGTH 20

struct Book
{
    char bookName[MAX_NAME_LENGTH];
    char writerName[MAX_NAME_LENGTH];
    char currency[MAX_CURRENCY_LENGTH];
    int releaseYear;
    float price;
};

int addArchiveToArray(struct Book books[]);
void addBook(struct Book books[],int);
void listBooks(struct Book books[],int);
void deleteBook(struct Book books[],char *,int *);
void searchBook(struct Book books[],int,int);
void clearInputBuffer(void);
void clearStringBuffer(char *);
void clearStructBuffer(struct Book *,int);

int main()
{
    struct Book books[MAX_LENGTH];
    int choice;
    int bookCount = 0;

    bookCount = addArchiveToArray(books);

    do
    {
        printf("--- Library System ---\n");
        printf("1. Add book\n2. List all books\n3. Delete book\n4. Search book\n5. Exit");

        printf("\n\nPlease make a choice: ");
        scanf("%d", &choice);

        clearInputBuffer();

        switch(choice)
        {
            case 1:
            {
                printf("\nAdd new book: \n");
                printf("--------------------------\n");

                addBook(books, bookCount);
                bookCount++;

                break;
            }
            case 2:
            {
                printf("\nAll Books: \n");
                printf("--------------------------\n\n");

                listBooks(books, bookCount);

                break;
            }
            case 3:
            {
                int choice2;
                char bookName[MAX_NAME_LENGTH];

                do
                {
                    printf("\nEnter the name of the book you want to delete: ");

                    fgets(bookName,sizeof(bookName),stdin);

                    clearStringBuffer(bookName);

                    printf("\nAre you sure to delete \"%s\" book?",bookName);

                    printf("\n\nPress 1 (Delete)\nPress 2 (Back to menu): ");
                    scanf("%d",&choice2);

                    clearInputBuffer();
                    printf("\n");

                    if(choice2 > 2 || choice2 < 1)
                        printf("Invalid value! Please try again.\n");

                } while (choice2 > 2 || choice2 < 1);

                if(choice2 == 1)
                    deleteBook(books, bookName, &bookCount);

                break;
            }
            case 4:
            {
                int choice3;

                do
                {
                    printf("\nDo you want to search with writer,book name or release year?\n");

                    printf("\nPress 1 (Book name)\nPress 2 (Writer name)\nPress 3 (Release year): ");
                    scanf("%d",&choice3);

                    clearInputBuffer();
                    printf("\n");

                    if(choice3 > 3 || choice3 < 1)
                        printf("Invalid value! Please try again.\n");

                } while (choice3 > 3 || choice3 < 1);

                searchBook(books, bookCount, choice3);

                break;
            }
            case 5:
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

    } while (choice != 5);

    return 0;
}

void clearInputBuffer(void)
{
    while ((getchar()) != '\n');
}

void clearStringBuffer(char *buffer)
{
    int size = strlen(buffer);

    buffer[size - 1] = '\0';
}

void clearStructBuffer(struct Book *book,int option)
{
    if(option == 1)
    {
        int size = strlen(book->bookName);

        if(book->bookName[size - 1] == '\n')
            book->bookName[size - 1] = '\0';
    }
    else if(option == 2)
    {
        int size = strlen(book->writerName);

        if(book->writerName[size - 1] == '\n')
            book->writerName[size - 1] = '\0';
    }
    else
    {
        int size = strlen(book->currency);

        if(book->currency[size - 1] == '\n')
            book->currency[size - 1] = '\0';
    }
}

int addArchiveToArray(struct Book books[MAX_LENGTH])
{
    int bookCount = 0;

    FILE *file = fopen(FILENAME,"r");

    if(file == NULL)
    {
        printf("There is no any book.Please add a book: \n");

        addBook(books, bookCount);

        return 1;
    }

    char buf[MAX_NAME_LENGTH];
    int check = 0;

    while(fgets(buf,sizeof(buf),file) != NULL)
    {
        if(check == 0)
        {
            strcpy(books[bookCount].bookName,buf + 6);

            clearStructBuffer(&books[bookCount], 1);
        }

        else if(check == 1)
        {
            strcpy(books[bookCount].writerName,buf + 8);

            clearStructBuffer(&books[bookCount], 2);
        }

        else if(check == 2)
            sscanf(buf,"Release Year: %d",&books[bookCount].releaseYear);

        else if(check == 3)
            sscanf(buf,"Price: %f %[^\n]",&books[bookCount].price, books[bookCount].currency);

        check++;

        if(buf[0] == '-')
        {
            bookCount++;
            check = 0;
        }
    }

    fclose(file);

    return bookCount;
}

void addBook(struct Book books[MAX_LENGTH], int bookCount)
{
    printf("\nBook Name: ");
    fgets(books[bookCount].bookName, sizeof(books[bookCount].bookName), stdin);

    clearStructBuffer(&books[bookCount], 1);

    printf("Writer Name: ");
    fgets(books[bookCount].writerName, sizeof(books[bookCount].writerName), stdin);

    clearStructBuffer(&books[bookCount], 2);

    printf("Release Year: ");
    scanf("%d", &books[bookCount].releaseYear);

    printf("Price: ");
    scanf("%f", &books[bookCount].price);

    while(books[bookCount].price < 0)
    {
        printf("Wrong price!Please try again.\n");

        printf("Price: ");
        scanf("%f", &books[bookCount].price);
    }

    clearInputBuffer();

    printf("Currency type: ");
    fgets(books[bookCount].currency, sizeof(books[bookCount].currency), stdin);

    clearStructBuffer(&books[bookCount], 3);

    printf("\nThe book added successfully!\n\n");

    FILE *file = fopen(FILENAME,"a");

    if(file == NULL)
    {
        printf("\nNo such file or directory!\n");
        exit(1);
    }

    fprintf(file,"Book: %s\nWriter: %s\n", books[bookCount].bookName,books[bookCount].writerName);
    fprintf(file,"Release Year: %d\nPrice: %.2f %s\n", books[bookCount].releaseYear, books[bookCount].price, books[bookCount].currency);
    fprintf(file,"--------------------------\n");

    fclose(file);
}

void listBooks(struct Book books[MAX_LENGTH], int bookCount)
{
    for(int k = 0;k < bookCount;k++)
    {
        printf("%d.Book: %s\n\n",k + 1, books[k].bookName);
        printf("Writer: %s\n",books[k].writerName);
        printf("Release Year: %d\n",books[k].releaseYear);
        printf("Price: %.2f %s\n",books[k].price, books[k].currency);
        printf("--------------------------\n\n");
    }
}

void deleteBook(struct Book books[MAX_LENGTH], char *bookName2, int *bookCount)
{
    int control = 0;

    for(int k = 0;k < *bookName2;k++)
    {
        if(strstr(books[k].bookName,bookName2) != NULL)
        {
            control = 1;
            break;
        }
    }

    if(control == 1)
    {
        FILE *file = fopen(FILENAME,"r");
        FILE *tempFile = fopen("temp.txt","w");

        if(file == NULL || tempFile == NULL)
        {
            printf("\nNo such file or directory!\n");
            exit(1);
        }

        char buf[MAX_NAME_LENGTH];
        int check = 0;
        int check2 = 0;
        int index = 0;

        while(fgets(buf,sizeof(buf),file) != NULL)//Deletes from file.
        {
            if(strstr(buf,"Book: ") != NULL)
            {
                if(strstr(buf,bookName2) != NULL)
                {
                    check = 1;
                    check2 = 1;
                }

                else
                    check = 0;
            }

            if(buf[0] == '-' && check2 == 0)//Index for struct array.
                index++;

            if(check == 0)
                fprintf(tempFile,"%s",buf);

            else
            {
                if(buf[0] == '-')
                    check = 0;
            }
        }

        fclose(file);
        fclose(tempFile);

        remove(FILENAME);

        rename("temp.txt",FILENAME);

        for(int k = index;k < *bookCount - 1;k++)//Deletes from struct array
            books[k] = books[k + 1];

        (*bookCount)--;

        printf("\nThe book deleted successfully.\n\n");
    }

    else
        printf("There is no such book.\n\n");
}

void searchBook(struct Book books[MAX_LENGTH], int bookCount, int choice)
{
    int check = 0;

    if(choice == 1)
    {
        char bookName1[MAX_NAME_LENGTH];

        printf("\nEnter the name of the book you want to search: ");

        fgets(bookName1,sizeof(bookName1),stdin);

        clearStringBuffer(bookName1);

        printf("--------------------------\n");

        for(int k = 0;k < bookCount;k++)
        {
            if(strstr(books[k].bookName,bookName1) != NULL)
            {
                printf("\n%d.Book: %s\n\n",k + 1, books[k].bookName);
                printf("Writer: %s\n\n",books[k].writerName);
                printf("Release Year: %d\n",books[k].releaseYear);
                printf("Price: %.2f %s\n",books[k].price, books[k].currency);
                printf("--------------------------\n\n");

                check = 1;
            }
        }

        if(check == 0)
            printf("\nThere is no such book.\n\n");
    }

    else if(choice == 2)
    {
        char writerName1[MAX_NAME_LENGTH];

        printf("\nEnter the name of the author of the book you want to search: ");

        fgets(writerName1,sizeof(writerName1),stdin);

        clearStringBuffer(writerName1);

        printf("--------------------------\n");

        for(int k = 0;k < bookCount;k++)
        {
            if(strstr(books[k].writerName,writerName1) != NULL)
            {
                printf("\n%d.Book: %s\n\n",k + 1, books[k].bookName);
                printf("Writer: %s\n\n",books[k].writerName);
                printf("Release Year: %d\n",books[k].releaseYear);
                printf("Price: %.2f %s\n",books[k].price, books[k].currency);
                printf("--------------------------\n\n");

                check = 1;
            }
        }

        if(check == 0)
            printf("\nThere is no such book for this author.\n\n");
    }

    else
    {
        int firstY, secondY;

        printf("\nPlease write a release year range: ");

        printf("\n\nFirst year: ");
        scanf("%d",&firstY);

        printf("Second year: ");
        scanf("%d",&secondY);

        printf("--------------------------\n");

        for(int k = 0;k < bookCount;k++)
        {
            if(books[k].releaseYear >= firstY && books[k].releaseYear <= secondY)
            {
                printf("\n%d.Book: %s\n\n",k + 1, books[k].bookName);
                printf("Writer: %s\n\n",books[k].writerName);
                printf("Release Year: %d\n",books[k].releaseYear);
                printf("Price: %.2f %s\n",books[k].price, books[k].currency);
                printf("--------------------------\n\n");

                check = 1;
            }
        }

        if(check == 0)
            printf("\nThere is no such book for this release year range.\n\n");
    }
}
