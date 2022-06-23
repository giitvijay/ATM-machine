#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

struct User
{
    int pin;
    float balance;
    int accountNumber;
    char name[40];
};

void loginMenu();
void signupMenu();
struct User getUserDetails(int);
void checkBalance(struct User);
void moneyWithdraw(struct User, float);
void moneyDeposit(struct User, float);
void updateUserDetails(struct User);
int noUser = 0;


int main()
{
    int mainMenu;
    int exit = 1;
    int anotherTransaction = 0;
    srand(time(0));

    while (exit)
    {
        system("CLS");
        printf("\nMain Menu\n1. Login to your account\n2. Create a new account\n3. Exit\n\n");
        scanf("%d", &mainMenu);

        switch (mainMenu)
        {
        case 1:
            loginMenu();
            break;
        case 2:
            signupMenu();
            break;

        case 3:
            exit = 0;
            break;

        default:
            break;
        }

        if (mainMenu != 3)
        {
            if (mainMenu == 2) {
                anotherTransaction = 2;
            } else {
                if (noUser) {
                    anotherTransaction = 2;
                } else {
                    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
                    printf("Exit?\n");
                    printf("< 1 > Yes\n");
                    printf("< 2 > No\n");
                    scanf("%d", &anotherTransaction);
                }
            }

            if (anotherTransaction == 1)
            {
                exit = 0;
            }
        }
    }
}



void loginMenu() {
    int accountNumber, optionsChoice, exitLoop=1, pin, actualPin, nextTransaction, depositAmount, withdrawAmount;
    system("CLS");
    printf("Welcome back! Enter your account number:\n");
    scanf("%d", &accountNumber);



    struct User userStructure;
    int proceed;

    userStructure = getUserDetails(accountNumber);

    if (userStructure.accountNumber == 0 && userStructure.pin == 0) {
        printf("\nUser does not exist : Create a new user, please enter any number to proceed to main menu\n");
        scanf("%d", &proceed);
        noUser = 1;
        return;
    }

    actualPin = userStructure.pin;




    printf("\nAccount Number: %d,Name:%s , Balance: %.2f\n",userStructure.accountNumber,userStructure.name, userStructure.balance);


    printf("Enter your PIN:\n");
    scanf("%d", &pin);

    if (pin != actualPin) {
        int tries = 4;
        printf("Invalid PIN entered, ");
        while(pin != actualPin && tries > 0) {
            printf("You have %d tries left, try again\n", tries);
            scanf("%d", &pin);
            tries--;
        }
        system("CLS");
        if (tries == 0) {
            printf("\n Maximum try limit reached, sending you back to the entry screen\n\n");
            return;
        }
    }

    while(exitLoop) {
        /*
            1. Check Balance 2. Deposit Money 3. Withdraw Money 4. Exit
        */
       userStructure = getUserDetails(accountNumber);
       system("CLS");
        printf("\n%s Choose what you want to do:\n1. Check Balance\n2. Deposit Money\n3. Withdraw Money\n4. Exit\n\n",userStructure.name);

        scanf("%d", &optionsChoice);
        switch (optionsChoice) {
            case 1:
             system("CLS");
                checkBalance(userStructure);
                break;
            case 2:
             system("CLS");
             printf("How much do you want to deposit?\n");
             scanf("%d", &depositAmount);
                moneyDeposit(userStructure, depositAmount);
                break;
            case 3:
             system("CLS");
             printf("How much do you want to withdraw?\n");
             scanf("%d", &withdrawAmount);
                moneyWithdraw(userStructure, withdrawAmount);
                break;

            case 4:
             system("CLS");
                exitLoop = 0;
                break;

            default:
                break;
        }

        if (optionsChoice != 4) {
            system("CLS");
            printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
            printf("Would you like to do another transaction?\n");
            printf("< 1 > Yes\n");
            printf("< 2 > No\n");
            scanf("%d", &nextTransaction);

            system("CLS");
        } else {
            nextTransaction = 2;
        }

        if (nextTransaction == 2) {
            exitLoop = 0;
            return;
        }
    }
}

struct User getUserDetails(int accountNumber) {


    FILE *createdFile;

    struct User tempuser;

    char tempStr[10];
    itoa(accountNumber, tempStr, 10);
    char *fileName = strcat(tempStr, ".txt");

    createdFile = fopen(fileName, "r");

    if(createdFile == NULL) {
        tempuser.accountNumber = 0;
        tempuser.balance = 0.0;
        tempuser.pin = 0;
        return tempuser;
    }

    while(fread(&tempuser, sizeof(struct User),1 , createdFile))

    fclose(createdFile);
    return tempuser;
}

void createUserAccount(struct User user) {
    FILE *file;

    char tempStr[10];
    itoa(user.accountNumber, tempStr, 10);
    char *fileName = strcat(tempStr, ".txt");

    file = fopen(fileName, "w");

    struct User tempUser = user;

    fwrite (&tempUser, sizeof(struct User), 1, file);

    if (fwrite != 0) {
        printf("User created succesfully! %d %.2f %d\n\n", tempUser.accountNumber, tempUser.balance,tempUser.pin);
        printf("Enter any number to proceed to main menu\n");
        int proceed;
        scanf("%d", &proceed);
    } else {
        printf("Error writing the file\n");
    }

    fclose(file);
}

void checkBalance(struct User user) {
    time_t timeNow;
    time(&timeNow);
    printf("Time: %s", ctime(&timeNow));
    printf("\nYour balance is: %.2f\n", user.balance);
    printf("Enter any number to proceed to the menu\n");
    int proceed;
    scanf("%d", &proceed);
}

void moneyDeposit(struct User user, float amount) {
    user.balance += amount;
    updateUserDetails(user);
    time_t timeNow;
    time(&timeNow);
    printf("Time: %s", ctime(&timeNow));
    printf("Money deposited. Your current balance: %.2f\nEnter any number to proceed to the menu\n", user.balance);
    int proceed;
    scanf("%d", &proceed);
}

void moneyWithdraw(struct User user, float amount) {
    if (user.balance >= amount) {
        user.balance -= amount;
        time_t timeNow;
        time(&timeNow);
        printf("Time: %s", ctime(&timeNow));
        printf("Money deducted from your account. Available balance: %.2f\nEnter any number to proceed to the menu\n", user.balance);
        int proceed;
        scanf("%d", &proceed);
    } else {
        printf("Insufficient balance in your account :(\n");
        int proceed;
        scanf("%d", &proceed);
    }
    updateUserDetails(user);

}

void updateUserDetails(struct User user) {

    FILE *file;

    struct User tempUser = user;

    char tempStr[10];
    itoa(user.accountNumber, tempStr, 10);
    char *fileName = strcat(tempStr, ".txt");

    file = fopen(fileName, "w");

    fwrite (&tempUser, sizeof(struct User), 1, file);

    if (fwrite != 0) {
        printf("User details updated succesfully!\n");
    } else {
        printf("Error writing the file");
    }

    fclose(file);
    return;
}

void signupMenu() {
    printf("\n=====Account Creation=====\n\n");

    time_t seconds;
    seconds = time(NULL) % 100000;

    struct User user;
    int pin;


    user.accountNumber = seconds;
    user.balance = 0.0;



    printf("Welcome! Your account number is %d\n", seconds);
    printf("Please choose a 4-digit PIN: \n");
    scanf("%d", &pin);





    if (pin<999 && pin>10000)
    {
        printf("please enter valid pin:");
        scanf("%d",&pin);
    }








    printf("Please enter your name without spaces:\n");
    scanf("%s",&user.name);

    user.pin = pin;

    createUserAccount(user);
}
