#include<bits/stdc++.h>
using namespace std;
const size_t NONE = string::npos;

int creatingId = 1;
class registeredAccount{
    public:
    string name;
    string gmail;
    string password;
};
vector<registeredAccount> registeredAccounts;
class totalMessData {
public:
    float totalDeposit = 0;
    float totalMeal = 0;
    float totalMealCost = 0;
    float mealRate = 0;  
    float presentBalance = 0; 
    string message;

    void updateMessData() {
        if (totalMeal != 0) {
            mealRate = totalMealCost / totalMeal;
        } else {
            mealRate = 0;
        }
        presentBalance = totalDeposit - totalMealCost;
    }
};

class UserData {
public:
    totalMessData* messData;
    string name;
    string email;
    string password;
    int tag = 0;
    int user_id;
    float totalMeal = 0;
    float totalDeposit = 0;
    float mealCost = 0;  
    float balance = 0;  

    void updateUserData() {
        if (messData) {
            mealCost = totalMeal * messData->mealRate;
            balance = totalDeposit - mealCost;
        }
    }
};

vector<UserData> users;

class displayData {
public:
    void displayFullMess(totalMessData& totalMess) {
        cout << "\n-------------------- Welcome to MessManager -----------------------\n\n";
        cout << "* Total Deposit: " << totalMess.totalDeposit << endl;
        cout << "* Total Meal: " << totalMess.totalMeal << endl;
        cout << "* Total Meal Cost: " << totalMess.totalMealCost << endl;
        cout << "* Meal Rate: " << totalMess.mealRate << endl;
        cout << "* Present Balance: " << totalMess.presentBalance << endl;
        cout << "* Message: " << totalMess.message << endl;
        cout << "\n\n";
    }

    void displayUser() {
        cout << "\n---------------------- User Details ----------------------\n\n";
        for (UserData& user : users) {
            cout << "* Name: " << user.name << endl;
            cout<< "* Email: " << user.email << endl;
            cout << "* User ID: " << user.user_id << endl;
            cout << "* Total Meal: " << user.totalMeal << endl;
            cout << "* Total Deposit: " << user.totalDeposit << endl;
            cout << "* Meal Cost: " << user.mealCost << endl;
            cout << "* Balance: " << user.balance << endl;
            cout << "\n\n------------------- ******** --------------------\n\n";
        }
    }
};

class AdminWorks {
public:
    totalMessData& tmd;
    AdminWorks(totalMessData& tmd_ref) : tmd(tmd_ref) {}

    void allMembers() {
        int index = 1;
        cout<<"\nList of members: "<<endl;
        for (const UserData& user : users) {
            cout << index << ". " << user.name << endl;
            index++;
        }
    }

    void addDeposit(float amount, int command) {
        for (UserData& user : users) {
            if (user.user_id == command) {
                user.totalDeposit += amount;
                tmd.totalDeposit += amount;
                user.updateUserData();  
                tmd.updateMessData(); 
                cout << "\nDeposit Added Successfully" << endl;
                return;
            }
        }
        cout << "User not found" << endl;
    }

    void addCost(float amount) {
        tmd.totalMealCost += amount;
        tmd.updateMessData();  
        for (UserData& user : users) {
            user.updateUserData();  
        }
        cout << "\nMeal Cost Updated Successfully\n" << endl;
    }

    void addMeal(float mealAmount, int command) {
        for (UserData& user : users) {
            if (user.user_id == command) {
                user.totalMeal += mealAmount;
                tmd.totalMeal += mealAmount;
                tmd.updateMessData(); 
                user.updateUserData();  
                cout << "\nMeal Updated Successfully" << endl;
                return;
            }
        }
        cout << "User not found" << endl;
    }

    bool changeManager() {
        allMembers();
        int command;
        cout << "Enter User ID: ";
        cin >> command;
        bool found = false;
        for (UserData& user : users) {
            if (user.user_id == command) {
                cout << "\nUser Found. Updating Manager......\n" << endl;
                found = true;
                break;
            }
        }
        if (found) {
            for (UserData& user : users) {
                user.tag = 2;
                if (user.user_id == command) {
                    user.tag = 1;
                }
            }
            return true;
        } else {
            cout << "\nUser not found. Error!!!\n" << endl;
            return false;
        }
    }

    void addMessage() {
        cout << "Enter Message: \n";
        getchar();  
        getline(cin, tmd.message);
        cout<<"\nMessage Added Successfully\n";
    }
};

bool emailChekign(const string& email) {
    for (const UserData& user : users) {
        if (user.email == email) {
            return true;
        }
    }
    return false;
}

bool isAdminExist() {
    for (const UserData& user : users) {
        if (user.tag == 1) {
            return true;
        }
    }
    return false;
}

bool chekcingData(const string& gmail, const string& password) {
    for (const UserData& user : users) {
        if (user.email == gmail && user.password == password) {
            return true;
        }
    }
    return false;
}

int checkingManagerOrMember(const string& gmail) {
    for (const UserData& user : users) {
        if (user.email == gmail && user.tag == 1) {
            return 1;
        } else if (user.email == gmail && user.tag == 2) {
            return 2;
        }
    }
    return 0;
}

bool isValidGmail(const string& email) {
    size_t atPos = email.find('@');
    size_t dotPos = email.rfind('.');

    if (atPos == string::npos || dotPos == string::npos || atPos > dotPos) {
        return false;
    }

    if (email.size() - dotPos - 1 < 2 || email.size() - dotPos - 1 > 3) {
        return false;
    }

    if (!isalpha(email[0]) && email[0] != '_') {
        return false;
    }

    for (size_t i = 0; i < atPos; ++i) {
        if (!isalnum(email[i]) && email[i] != '_') {
            return false;
        }
    }

    if (email.find("..") != string::npos || email.find("@@") != string::npos) {
        return false;
    }
    
    return true;
}
void createAdminAccount(totalMessData& tmd) {
    UserData user;
    cout << "Enter Name: ";
    cin.ignore();  
    getline(cin, user.name);

    while (true) {
        cout << "Enter Email: ";
        string email;
        getline(cin, email);
        if(!isValidGmail(email)){
            cout << "\nInvalid Email! Email should be in the format:  example@example.com\n\n";
            continue;
        }
        if (!emailChekign(email)) {
            user.email = email;
            break;
        } else {
            cout << "\nEmail Already Exists! Try Another Email.\n";
        }
    }

    cout << "Enter Password: ";
    getline(cin, user.password);

    if (isAdminExist()) {
        cout << "\nAdmin/Manager Exists! Try Again.\n";
    } else {
        cout << "\nAdmin/Manager Created Successfully.\n";
        user.tag = 1;
        user.messData = &tmd;  // Assign the messData pointer
        user.user_id = creatingId;
        users.push_back(user);
        creatingId++;
    }
        
}
void addMember(totalMessData& tmd) {
    UserData user;
    string name, email;

        cout << "Enter Name: ";
        cin.ignore();  
        getline(cin, name);
        cout << "Enter Email: ";
        getline(cin, email);
        if(!isValidGmail(email)){
            cout << "\nInvalid Email! Email should be in the format:  example@example.com\n\n";
            return;
        }
        if (!emailChekign(email)) {
            user.email = email;
            user.name = name;
            for(registeredAccount account : registeredAccounts) {
                if(account.gmail == email) {
                    user.name = account.name;
                    user.password = account.password;
                    cout<<"\nExisting Account Added into this Mess Successfully\n"<<endl;
                }
            }
            cout<<"\nAdding Member Successful\n"<<endl;
        } else {
            cout << "\nEmail Already Exists! Try Another Email.\n";
        }

    user.messData = &tmd;  // Assign the messData pointer
    user.tag = 2;
    user.user_id = creatingId;
    users.push_back(user);
    creatingId++;
}

void AdminCalls(totalMessData& tmd) {
    AdminWorks aw(tmd);

    while (true) {
        cout << "\n1. See All Members Names\n2. Add Member\n3. Add Deposit\n4. Add Meal Cost\n5. Add Meal\n6. Change Manager\n7. Add Message\n8. Display Mess\n9. Exit\n";
        cout << "Enter Choice: ";
        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                aw.allMembers();
                break;
            case 2:
                addMember(tmd);
                break;
            case 3:
                aw.allMembers();
                int command;
                cout << "Enter User ID: ";
                cin >> command;
                float amount;
                cout << "Enter Amount: ";
                cin >> amount;
                aw.addDeposit(amount, command);
                break;
            case 4:
                float mealCost;
                cout << "Enter Total Meal Cost: ";
                cin >> mealCost;
                aw.addCost(mealCost);
                break;
            case 5:
                aw.allMembers();
                cout << "Enter User ID: ";
                cin >> command;
                float mealAmount;
                cout << "Enter Total Meals: ";
                cin >> mealAmount;
                aw.addMeal(mealAmount, command);
                break;
            case 6:
                if(aw.changeManager()){
                    return;
                }
                break;
            case 7:
                aw.addMessage();
                break;
            case 8:
                displayData dd;
                dd.displayFullMess(tmd);
                dd.displayUser();
                break;
            case 9:
                return;
            default:
                cout << "\nInvalid Choice! Try Again.\n";
        }
    }
}

bool checkingRegisteredAccounts(const string& gmail, const string& password) {
    for (const registeredAccount& account : registeredAccounts) {
        if (account.gmail == gmail && account.password == password) {
            return true;
        }
    }
    return false;
}

bool registeredEmailCheck(const string& email) {
    for (const registeredAccount& account : registeredAccounts) {
        if (account.gmail == email) {
            return true;
        }
    }
    return false;
}

bool handleDuplicateAccounts(const string& gmail){
    for(UserData& user: users){
        if(user.email == gmail && user.password.length() != 0){
            return true;
        }
    }
    return false;
}


void accountRegistration(){
    registeredAccount rga;
    cout << "Enter Name: ";
    cin.ignore();  
    getline(cin, rga.name);

    while (true) {
        cout << "Enter Email: ";
        string email;
        getline(cin, email);
        if(!isValidGmail(email)){
            cout << "\nInvalid Email! Email should be in the format:  example@example.com\n\n";
            continue;
        }
        if (!registeredEmailCheck(email) && !handleDuplicateAccounts(email)) {
            rga.gmail = email;
            break;
        } else {
            cout << "\nEmail Already Exists! Try to login.\n\n";
            return;
        }
    }

    cout << "Enter Password: ";
    getline(cin, rga.password);
    
    registeredAccounts.push_back(rga);
    cout<<"\nAccount Creation Successful"<<endl;
    for(UserData& user : users){
        if (user.email == rga.gmail){
            user.name  = rga.name;
            user.password = rga.password;
            cout<<"\nRegistration Successful. You are now a Member of the Mess.\n";
        }
    }

}

void login( totalMessData& tmd) {

    string gmail, password;
    cout << "Enter Gmail: ";
    cin >> gmail;
    cout << "Enter Password: ";
    cin >> password;

    if (chekcingData(gmail, password) && checkingManagerOrMember(gmail) == 1) {
        cout << "\nAdmin Login Successfully.\n";
        AdminCalls(tmd);
    }else if (chekcingData(gmail, password) && checkingManagerOrMember(gmail) == 2) {
        cout << "\nMember Login Successfully.\n";
        displayData dd;
        dd.displayFullMess(tmd);  
        dd.displayUser();
    } else if(checkingRegisteredAccounts(gmail,password)){
        cout<<"\nLogin Successfully. But You are not a Member of any Mess!!!\n";
    } else {
        cout << "\nInvalid Gmail or Password! Try Again.\n";
    }
        
}
int main() {
    cout<<"\n\n************     Welcome to the Mess Management Application      *************\n\n"<<endl;
    totalMessData tmd;
    UserData UD;
    while (true) {
        cout << "\n1. Create Mess as Admin\n2. Create Account\n3. Login\n4. Exit\n";
        cout << "Enter Choice: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            if (users.size() == 0) {
                cout << "\nNo User Exists in this Mess! Create an Admin Account First.\n\n"; 
                createAdminAccount(tmd);
            } else {
                cout << "\nAdmin Exists in this Mess! Try Login First.\n";
            }
        } else if(choice == 2) {
            accountRegistration();
        } else if (choice == 3) {
            login(tmd);
        } else if (choice == 4) {
            break;
        } else {
            cout << "\nInvalid Choice! Try Again.\n";
        }
    }

    return 0;
}
