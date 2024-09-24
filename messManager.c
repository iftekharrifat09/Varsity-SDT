#include <stdio.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_STR_LEN 50

// Structure to store mess data
typedef struct {
    float totalDeposit;
    float totalMeal;
    float totalMealCost;
    float mealRate;
    float presentBalance;
    char message[MAX_STR_LEN];
} totalMessData;

// Structure to store user data
typedef struct {
    char name[MAX_STR_LEN];
    char email[MAX_STR_LEN];
    char password[MAX_STR_LEN];
    int user_id;
    int tag;  // 1 for Admin, 2 for Member
    float totalMeal;
    float totalDeposit;
    float mealCost;
    float balance;
    int isMember;
} UserData;

totalMessData messData = {0};                 // Global mess data
UserData registeredUsers[MAX_USERS];           // Array to store registered user data
UserData messMembers[MAX_USERS];               // Array to store mess members
int registeredUserCount = 0;                   // Number of registered users
int messMemberCount = 0;                       // Number of mess members
int creatingId = 1;                            // User ID counter
int currentUserIndex = -1;                     // Track the currently logged-in user


void updateUserData(UserData *user) {
    user->mealCost = user->totalMeal * messData.mealRate;
    user->balance = user->totalDeposit - user->mealCost;
}
// Function to calculate meal rate and balance
void updateMessData() {
    if (messData.totalMeal != 0) {
        messData.mealRate = messData.totalMealCost / messData.totalMeal;
    } else {
        messData.mealRate = 0;
    }
    messData.presentBalance = messData.totalDeposit - messData.totalMealCost;

    // Update the meal cost and balance for all mess members (including admin)
    for (int i = 0; i < messMemberCount; i++) {
        updateUserData(&messMembers[i]);
    }
}


// Function to display mess data
void displayFullMess() {
    printf("\n-------------------- Welcome to MessManager -----------------------\n\n");
    printf("* Total Deposit: %.2f\n", messData.totalDeposit);
    printf("* Total Meal: %.2f\n", messData.totalMeal);
    printf("* Total Meal Cost: %.2f\n", messData.totalMealCost);
    printf("* Meal Rate: %.2f\n", messData.mealRate);
    printf("* Present Balance: %.2f\n", messData.presentBalance);
    printf("* Message: %s\n", messData.message);
}

// Function to display mess members
void displayMessMembers() {
    printf("\n---------------------- Mess Member Details ----------------------\n\n");
    for (int i = 0; i < messMemberCount; i++) {

        const char* position = messMembers[i].tag == 1 ? "Admin" : "Member";
        printf("* Position: %s\n", position);
        printf("* Name: %s\n", messMembers[i].name);
        printf("* User ID: %d\n", messMembers[i].user_id);
        printf("* Total Meal: %.2f\n", messMembers[i].totalMeal);
        printf("* Total Deposit: %.2f\n", messMembers[i].totalDeposit);
        printf("* Meal Cost: %.2f\n", messMembers[i].mealCost);
        printf("* Balance: %.2f\n", messMembers[i].balance);
        printf("\n---------------------- *************** ----------------------\n");
    }
}
void viewMembersDetais(){
    printf("\nExisting Members: \n");
    for(int i = 0; i < messMemberCount; i++){
        printf("# Name : %s   -     User ID: %d\n",messMembers[i].name,messMembers[i].user_id); 
    }
}
// Function to check if email exists in registered users
int emailCheckingIntoRegisterData(const char *email) {
    for (int i = 0; i < registeredUserCount; i++) {
        if (strcmp(registeredUsers[i].email, email) == 0) {
            return 1;
        }
    }
    return 0;
}
// int emailCheckingIntoMessData(const char *email) {
//     for (int i = 0; i < messMemberCount; i++) {
//         if (strcmp(messMembers[i].email, email) == 0) {
//             return 1;
//         }
//     }
//     return 0;
// }

// Function to check if email exists in mess members
int isMember(const char *email) {
    for (int i = 0; i < messMemberCount; i++) {
        if (strcmp(messMembers[i].email, email) == 0) {
            return 1;
        }
    }
    return 0;
}

// Function to check if email and password match in registered users
int checkingData(const char *email, const char *password) {
    for (int i = 0; i < registeredUserCount; i++) {
        if (strcmp(registeredUsers[i].email, email) == 0 && strcmp(registeredUsers[i].password, password) == 0) {
            currentUserIndex = i;
            return 1;
        }
    }
    return 0;
}

// Function to check if the user is an admin
int isAdmin(const char *email) {
    for (int i = 0; i < registeredUserCount; i++) {
        if (strcmp(registeredUsers[i].email, email) == 0 && registeredUsers[i].tag == 1) {
            return 1;
        }
    }
    return 0;
}

// Function to add deposit for a mess member
void addDeposit(float amount, int userId) {
    for (int i = 0; i < messMemberCount; i++) {
        if (messMembers[i].user_id == userId) {
            printf("\nAdding deposit for %s (ID: %d)\n", messMembers[i].name, messMembers[i].user_id);
            messMembers[i].totalDeposit += amount;
            messData.totalDeposit += amount;
            updateUserData(&messMembers[i]);
            updateMessData();
            printf("\nDeposit Added Successfully\n");
            return;
        }
    }
    printf("User not found in the mess\n");
}

// Function to add meal cost
void addCost(float amount) {
    messData.totalMealCost += amount;
    updateMessData();
    for (int i = 0; i < messMemberCount; i++) {
        updateUserData(&messMembers[i]);
    }
    printf("\nMeal Cost Updated Successfully\n");
}

// Function to add meal for a mess member
// Function to add meal for a mess member (including the admin)
void addMeal(float mealAmount, int userId) {
    for (int i = 0; i < messMemberCount; i++) {
        if (messMembers[i].user_id == userId) {
            printf("\nAdding meal for %s (ID: %d)\n", messMembers[i].name, messMembers[i].user_id);
            messMembers[i].totalMeal += mealAmount;
            messData.totalMeal += mealAmount;
            updateUserData(&messMembers[i]);
            updateMessData();  // Update the global mess data after adding the meal
            printf("\nMeal Updated Successfully for %s\n", messMembers[i].name);
            return;
        }
    }
    printf("User not found in the mess\n");
}


// Function to create admin account
void createAdminAccount() {
    if (registeredUserCount > 0) {
        printf("\nAdmin Exists! Try to login.\n");
        return;
    }
    
    UserData user;
    printf("Enter Name: ");
    getchar();  // To consume newline left by previous input
    fgets(user.name, MAX_STR_LEN, stdin);
    user.name[strcspn(user.name, "\n")] = 0;  // Remove newline character

    while (1) {
        printf("Enter Email: ");
        fgets(user.email, MAX_STR_LEN, stdin);
        user.email[strcspn(user.email, "\n")] = 0;  // Remove newline character

        if (!emailCheckingIntoRegisterData(user.email)) {
            break;
        } else {
            printf("\nEmail Already Exists! Try Another Email.\n");
        }
    }

    printf("Enter Password: ");
    fgets(user.password, MAX_STR_LEN, stdin);
    user.password[strcspn(user.password, "\n")] = 0;  // Remove newline character

    user.user_id = creatingId++;
    user.tag = 1;  // Admin
    user.isMember = 1;  // Automatically a member of the mess
    user.totalMeal = 0;
    user.totalDeposit = 0;
    user.mealCost = 0;
    user.balance = 0;
    registeredUsers[registeredUserCount++] = user;
    messMembers[messMemberCount++] = user;  // Admin is also a mess member
    printf("\nAdmin Created Successfully\n");
}

int emailWithoutPassword(const char *email){
    for(int i=0; i<registeredUserCount; i++){
        if(strcmp(registeredUsers[i].email, email) == 0 && strlen(registeredUsers[i].password) == 1){
            return 1;
        }
    }
    return 0;
}

void updatingWithoutPasswordAcc(char *email) {
    char password[MAX_STR_LEN];
    printf("Create Password: ");
    fgets(password, MAX_STR_LEN, stdin); 
    password[strcspn(password, "\n")] = 0;  

    for (int i = 0; i < registeredUserCount; i++) {
        if (strcmp(registeredUsers[i].email, email) == 0) {
            strcpy(registeredUsers[i].password, password);
            printf("\nPassword updated successfully for user: %s\n", registeredUsers[i].name);
            break;  
        }
    }

    for (int i = 0; i < messMemberCount; i++) {
        if (strcmp(messMembers[i].email, email) == 0) {
            strcpy(messMembers[i].password, password);
            printf("\nPassword updated successfully for mess member: %s\n", messMembers[i].name);
            return;  // Assuming email is unique, exit the function after finding the match
        }
    }

    printf("\nUser with the email %s not found in mess members.\n", email);
}


// Function to register an account
void registerAccount() {
    if(messMemberCount==0){
        printf("\nNo mess members found. Create admin account first.\n");
        return;
    }
    UserData user;
    printf("Enter Name: ");
    getchar();  // To consume newline left by previous input
    fgets(user.name, MAX_STR_LEN, stdin);
    user.name[strcspn(user.name, "\n")] = 0;  // Remove newline character

    while (1) {
        printf("Enter Email: ");
        fgets(user.email, MAX_STR_LEN, stdin);
        user.email[strcspn(user.email, "\n")] = 0;  // Remove newline character
        if (!emailCheckingIntoRegisterData(user.email)) {
            break;
        }else if(emailWithoutPassword(user.email)) {
            printf("\nAdmin already add this gmail, Create account to Login.\n");
            updatingWithoutPasswordAcc(user.email);
            return;
        }else {
            printf("\nEmail Already Exists! Try Another Email.\n");
            return;
        }
    }

    printf("Enter Password: ");
    fgets(user.password, MAX_STR_LEN, stdin);
    user.password[strcspn(user.password, "\n")] = 0;  // Remove newline character

    user.user_id = creatingId++;
    user.tag = 2;  // Default to regular member
    user.isMember = 0;  // Not a mess member yet
    user.totalMeal = 0;
    user.totalDeposit = 0;
    user.mealCost = 0;
    user.balance = 0;
    registeredUsers[registeredUserCount++] = user;
    printf("\nAccount Created Successfully\n");
}

void changeManager(char* email) {
    int old = 0, new = 0;  // Initialize old and new to 0

    for (int i = 0; i < messMemberCount; i++) {
        if (strcmp(messMembers[i].email, email) == 0 && messMembers[i].tag == 1) {
            printf("\n%s is Already Admin of this Mess\n", messMembers[i].name);
            return;
        }
        if (strcmp(messMembers[i].email, email) == 0) { 
            messMembers[i].tag = 1;  
            new = 1; 
        } else if (messMembers[i].tag == 1) { 
            messMembers[i].tag = 2;  
            old = 1;  
        }
    }
    for (int i = 0; i < registeredUserCount; i++) {
        if (strcmp(registeredUsers[i].email, email) == 0 && registeredUsers[i].tag == 1) {
            printf("\n%s is Already Admin of this Mess\n", messMembers[i].name);
            return;
        }
        if (strcmp(registeredUsers[i].email, email) == 0) { 
            registeredUsers[i].tag = 1;  
        } else if (registeredUsers[i].tag == 1) {  
            registeredUsers[i].tag = 2;  
            old = 1;  
        }
    }

    if (old == 1 && new == 1) {
        printf("\nManager Changed Successfully\n");
    } else if (new == 0) {
        printf("\nUser with email %s not found in the mess.\n", email);
    }
}

// Function to login to the system
int login() {
    char email[MAX_STR_LEN], password[MAX_STR_LEN];
    getchar();  // To consume newline left by previous input
    printf("Enter Email: ");
    fgets(email, MAX_STR_LEN, stdin);
    email[strcspn(email, "\n")] = 0;  // Remove newline character

    printf("Enter Password: ");
    fgets(password, MAX_STR_LEN, stdin);
    password[strcspn(password, "\n")] = 0;  // Remove newline character

    if (checkingData(email, password)) {
        printf("\nLogin Successful\n");
        if (isAdmin(email)) {
            printf("\nLogged in as Admin.\n");
            return 1;  // Logged in as admin
        } else if (isMember(email)) {
            printf("\nLogged in as Member.\n");
            return 2;  // Logged in as member
        } else {
            printf("\nYou are not a mess member. Contact with the Admin.\n");
            return 0;
        }
    } else {
        printf("\nInvalid Email or Password\n");
        return 0;
    }
}

// Function to add a mess member by admin
void addMessMember() {
    char email[MAX_STR_LEN];
    getchar();  // To consume newline left by previous input
    printf("Enter Email of the member to add: ");
    fgets(email, MAX_STR_LEN, stdin);
    email[strcspn(email, "\n")] = 0;  // Remove newline character

    if (!emailCheckingIntoRegisterData(email)) {
        printf("\nEmail not registered yet. Adding to the system without password.\n");
        UserData user;

        printf("Enter Name: ");
        fgets(user.name, MAX_STR_LEN, stdin);
        user.name[strcspn(user.name, "\n")] = 0;

        strcpy(user.email, email);
        user.user_id = creatingId++;
        user.tag = 2;  // Default to regular member
        user.isMember = 1;  // New member of the mess
        user.totalMeal = 0;
        user.totalDeposit = 0;
        user.mealCost = 0;
        user.balance = 0;
        registeredUsers[registeredUserCount++] = user;
        messMembers[messMemberCount++] = user;
        printf("\nMember Added Successfully. User must complete registration later.\n");
    } else if (!isMember(email)) {
        for (int i = 0; i < registeredUserCount; i++) {
            if (strcmp(registeredUsers[i].email, email) == 0) {
                registeredUsers[i].isMember = 1;
                messMembers[messMemberCount++] = registeredUsers[i];
                printf("\nA Registered User added to the mess successfully.\n");
                return;
            }
        }
    } else {
        printf("\nUser is already a mess member.\n");
    }
}

// Function to show main menu
void mainMenu() {
    while (1) {
        printf("\n--------------- Main Menu ------------------\n");
        printf("1. Create Admin Account\n");
        printf("2. Register Account\n");
        printf("3. Login\n");
        printf("4. Exit\n");
        printf("\nEnter your choice: ");
        
        int choice, loginType;
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                createAdminAccount();
                break;
            case 2:
                registerAccount();
                break;
            case 3:
                loginType = login();
                if (loginType == 1) {  // Admin logged in
                    int condition = 1;
                    while (condition) {
                        printf("\n---- Admin Menu ----\n");
                        printf("1. Add Deposit\n");
                        printf("2. Add Meal\n");
                        printf("3. Add Mess Member\n");
                        printf("4. Add Cost\n");
                        printf("5. View Mess Data\n");
                        printf("6. View Mess Members Details\n");
                        printf("7. Add Messages\n");
                        printf("8. Change Manager\n");
                        printf("9. Logout\n");
                        printf("Enter your choice: ");
                        
                        int adminChoice, userId;
                        float amount;
                        scanf("%d", &adminChoice);
                        
                        switch (adminChoice) {
                            case 1:
                                viewMembersDetais();
                                printf("Enter User ID: ");
                                scanf("%d", &userId);
                                printf("\nEnter Deposit Amount: ");
                                scanf("%f", &amount);
                                addDeposit(amount, userId);
                                break;
                            case 2:
                                viewMembersDetais();
                                printf("Enter User ID: ");
                                scanf("%d", &userId);
                                printf("\nEnter Meal Amount: ");
                                scanf("%f", &amount);
                                addMeal(amount, userId);
                                break;
                            case 3:
                                addMessMember();
                                break;
                            case 4:
                                printf("\nEnter Cost Amount: ");
                                scanf("%f", &amount);
                                addCost(amount);
                                break;
                            case 5:
                                displayFullMess();
                                displayMessMembers();
                                break;
                            case 6:
                                viewMembersDetais();
                                break;
                            case 7:
                                getchar();  // Clear any leftover input from previous scanf
                                printf("Write a Message: ");
                                fgets(messData.message, MAX_STR_LEN, stdin); 
                                messData.message[strcspn(messData.message, "\n")] = 0; 
                                printf("\nMessage added successfully.\n");
                                break;
                            case 8:
                                viewMembersDetais();
                                int uid;
                                printf("Enter User ID: ");
                                scanf("%d", &uid);
                                char email[MAX_STR_LEN];
                                int done = 0;
                                for(int i = 0; i < messMemberCount; i++){
                                    if(messMembers[i].user_id == uid){
                                        strcpy(email, messMembers[i].email);
                                        changeManager(email);
                                        condition = 0;
                                        done = 1;
                                        break;
                                    }
                                }
                                if(done==0){
                                    printf("\nUser with the given ID not found.\n");
                                }
                                break;
                            case 9:
                                currentUserIndex = -1;
                                printf("\nLogged out successfully.\n");
                                condition = 0;
                                break;
                            default:
                                printf("\nInvalid Choice.\n");
                        }
                    }
                } else if (loginType == 2) {  // Member logged in
                    printf("\n---- Member Menu ----\n");
                    displayFullMess();
                    displayMessMembers();
                }
                break;
            case 4:
                return;
            default:
                printf("\nInvalid Choice.\n");
        }
    }
}

int main() {
    mainMenu();
    return 0;
}
