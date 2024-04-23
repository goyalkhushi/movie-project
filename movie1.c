#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MOVIES 10
#define MAX_CUSTOMERS 100

struct Movie
{
    char title[50];
    char director[50];
    char genre[20];
    int duration; // in minutes
    float price;
    int availableSeats;
};

struct Customer
{
    char name[50];
    char email[50];
    int bookings[10];
    int numBookings;
};

struct Movie movies[MAX_MOVIES];

int numMovies = 0;

struct Customer customers[MAX_CUSTOMERS];

int numCustomers = 0;

void displayMovies()
{
    printf("Current Movies:\n");
    for (int i = 0; i < numMovies; i++)
    {
        printf("%d. %s (%s, %d min) - %.2f\n", i + 1, movies[i].title,
        movies[i].genre, movies[i].duration, movies[i].price);
    }
}

void registerCustomer()
{
    FILE *fptr;
    fptr = fopen("User_data.txt", "a");
    char name[50];
    char email[50];
    printf("Enter your name: ");
    scanf("%s", name);
    fputs(name, fptr);
    fputs("\t", fptr);
    printf("Enter your email: ");
    scanf("%s", email);
    fputs(email, fptr);
    fputs("\n", fptr);
    struct Customer newCustomer = {.numBookings = 0};
    strcpy(newCustomer.name, name);
    strcpy(newCustomer.email, email);
    customers[numCustomers] = newCustomer;
    numCustomers++;
    printf("Registration successful!\n");
    fclose(fptr);
}

void bookTicket(int customerIndex)
{
    FILE *fptr;
    fptr = fopen("User_data.txt", "a");
    int movieIndex, numTickets;
    printf("Enter the movie number to book: ");
    scanf("%d", &movieIndex);
    if (movieIndex < 1 || movieIndex > numMovies)
    {
        printf("Invalid movie number.\n");
        return;
    }
    struct Movie *movie = &movies[movieIndex - 1];
    printf("Enter the number of tickets to book (1-%d): ",movie->availableSeats);
    scanf("%d", &numTickets);
    if (numTickets < 1 || numTickets > movie->availableSeats)
    {
        printf("Invalid number of tickets.\n");
        return;
    }
    float total = numTickets * movie->price;
    printf("Total price: %.2f\n", total);
    printf("Confirm booking? (y/n): ");
    char confirm;
    scanf(" %c", &confirm);
    if (confirm != 'y')
    {
        printf("Booking cancelled.\n");
        return;
    }
    else
    {
        char tickets[50];
        sprintf(tickets, "%d", numTickets);
        fputs(tickets, fptr);
    }
    struct Customer *customer = &customers[customerIndex];
    if (customer->numBookings >= 10)
    {
        printf("Maximum number of bookings reached.\n");
        return;
    }
    customer->bookings[customer->numBookings] = movieIndex;
    customer->numBookings++;
    movie->availableSeats -= numTickets;
    printf("Booking successful!\n");
    fclose(fptr);
}

void cancelTicket(int customerIndex)
{
    FILE *fptr;
    fptr = fopen("User_data.txt", "a");
    int bookingIndex;
    printf("Enter the booking number to cancel: ");
    scanf("%d", &bookingIndex);
    if (bookingIndex < 1 || bookingIndex > customers[customerIndex].numBookings)
    {
        printf("Invalid booking number.\n");
        return;
    }
    int movieIndex = customers[customerIndex].bookings[bookingIndex -1];
    struct Movie *movie = &movies[movieIndex - 1];
    printf("Movie: %s (%s, %d min)\n", movie->title, movie->genre,movie->duration);
    printf("Number of tickets: %d\n", 1); // assume only one ticket can be booked
    fclose(fptr);
}

void paymentOptions(float total)
{
    FILE *fptr;
    fptr = fopen("User_data.txt", "a");
    printf("Choose payment method:\n");
    printf("1. Credit card\n");
    printf("2. Debit card\n");
    printf("3. Net banking\n");
    int choice;
    scanf("%d", &choice);
    switch (choice)
    {
        case 1:
        printf("Enter credit card number: ");
        char creditCardNum[20];
        scanf("%s", creditCardNum);
        printf("Payment successful!\n");
        break;
        case 2:
        printf("Enter debit card number: ");
        char debitCardNum[20];
        scanf("%s", debitCardNum);
        printf("Payment successful!\n");
        break;
        case 3:
        printf("Enter net banking details: ");
        char netBankingDetails[50];
        scanf("%s", netBankingDetails);
        printf("Payment successful!\n");
        break;
        default:
        printf("Invalid option.\n");
        break;
    }
    fclose(fptr);
}

int main()
{
    FILE *fptr;
    fptr = fopen("User_data.txt", "r");
    struct Movie movie1 = {"The Kerala Story", "Sudipto Sen","Drama", 138, 150, 50};
    struct Movie movie2 = {"Guardians Of The Galaxy Vol. 3", "James Gunn","Action/Sci-fi", 150, 180, 50};
    struct Movie movie3 = {"Fast X", "Justin Lin","Action/Thriller", 134, 200, 50};
    movies[0] = movie1;
    movies[1] = movie2;
    movies[2] = movie3;
    numMovies = 3;
    int choice;
    int customerIndex = -1; // -1 means no customer is currently logged in
    while (1)
    {
        printf("Welcome to Movie Ticket Booking System!\n");
        printf("1. Display current movies\n");
        printf("2. Register\n");
        printf("3. Log in\n");
        printf("4. Book a ticket\n");
        printf("5. Cancel a booking\n");
        printf("6. Log out\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
        displayMovies();
        break;
        case 2:
        registerCustomer();
        break;
        case 3:
        if (customerIndex != -1)
        {
            printf("You are already logged in.\n");
        }
        else
        {
            char email[100];
            printf("Enter your email: ");
            scanf("%s", email);
            int line_num = 0;
            char line[100];
            if(fptr == NULL) 
            {
                printf("Error: Unable to open file.\n");
                return 1;
            }
    
            while(fgets(line, 100, fptr) != NULL) {
                line_num++;
                if(strstr(line, email) != NULL) {
                    customerIndex = 1;
                    printf("Logged in as %s.\n", customers[line_num].name);
                    break;
                }
            }
        }
        if (customerIndex == -1)
        {
            printf("Email not found. Please register or try again.\n");
        }
        break;
        case 4:
        if (customerIndex == -1)
        {
            printf("Please log in or register.\n");
        }
        else
        {
            bookTicket(customerIndex);
            paymentOptions(customerIndex);
        }
        break;
        case 5:
        if (customerIndex == -1)
        {
            printf("Please log in or register.\n");
        }
        else
        {
            cancelTicket(customerIndex);
        }
        break;
        case 6:
        if (customerIndex == -1)
        {
            printf("You are not logged in.\n");
        }
        else
        {
            printf("Logged out.\n");
            customerIndex = -1;
        }
        break;
        case 7:
        exit(0);
        default:
        printf("Invalid choice.\n");
        break;
        }
        printf("\n");
    }
    fclose(fptr);
}