
#include <bits/stdc++.h>
using namespace std;
#define manager_id "manager"

class Car;
class Car_DB;
class User;
class Manager;
class Employee;
class Customer;
class User_DB;

class Car
{
private:
    string Company = "", Model = "", regNo = "", Condition = "",
           rented_to = manager_id;
    void Change_availability(string, int);

public:
    int due_date = 0;
    bool Check_availability();
    Car() { ; }
    Car(string, string, string, string);
    bool Car_Rent(string &, int);
    void Show_duedate();

    bool operator==(const Car &c) const;
    bool operator<(const Car &c) const;
    void Rented();
    void Display();
    friend Car_DB;
    friend Manager;
};

class Car_DB
{
protected:
    map<string, Car> database;
    void Add(Car);
    void Delete(Car);
    void Delete(string);
    bool Contains(string);

public:
    Car_DB() { ; }
    bool Rent_Car(string &, string, int);
    void Update(Car, string, int);
    void Update(Car c);
    Car Search(string);
    void Display();
    void Check_availability();
    friend Manager;
};

class User
{
protected:
    string ID = "", password = "";
    int fineamt = 0, max_cars = 0;
    set<Car> rented_cars;

public:
    string type = "", name = "";
    int rent_days = 0;
    User() { ; }
    virtual ~User() {}
    int Calculate_fine(int);
    void Clear_fine_amount(int);
    void Return_cars(int);
    bool operator==(const User &u);
    void Show_cars();
    bool Check_password(string);
    void Show_dues();
    void Rent_Car();
    void Logout();
    void Display();
    virtual void Help();
    friend User_DB;
    friend Manager;
    friend void Login();
};

class Employee : public User
{
public:
    Employee(string, string, string);
    void Help();
    friend void Login();
};

class Customer : public User
{
public:
    Customer(string, string, string);
    void Help();
    friend void Login();
};

class Manager : public User
{
private:
public:
    Manager();
    void Add_Car();
    void Delete_Car();
    void Add_User();
    void Delete_User();
    void Change_day();
    void Help();
    void Check_rent();
    void User_Cars();
    friend void Login();
};

class User_DB
{
public:
    map<string, User *> database;
    User_DB() { ; }
    void Add(User *);
    bool Delete(User *);
    bool Delete(string);
    User *Search(string);
    bool Contains(string);
    void Update(string, string);
    void Display();
    friend Manager;
};

void Login();
void Show_today();

Car_DB CAR_DB;
User_DB USER_DB;
Manager manager;
int TODAY = 0;
bool program_terminate = false;
User dummy;
User *logged_in = (&dummy);

void Car::Change_availability(string user_id, int day)
{
    rented_to = user_id;
    due_date = day;
}
bool Car::Check_availability()
{
    if (rented_to == manager_id)
    {
        cout << "This car is currently available!\n";
    }
    else
    {
        cout << "Sorry, this car is currently rented by someone else.\n";
    }
    return (rented_to == manager_id);
}
Car::Car(string C, string M, string R, string Co)
{
    Company = C, Model = M, regNo = R, Condition = Co;
}
bool Car::Car_Rent(string(&user), int day = 0)
{
    if (user == manager_id)
    {
        rented_to = manager_id, due_date = 0;
        return true;
    }
    if (Check_availability())
    {
        cout << "Car has been rented to you!\n";
        rented_to = user, due_date = day;
        return true;
    }
    else
    {
        cout << "Sorry, this car is currently rented to someone else.\n";
        return false;
    }
}
void Car::Show_duedate()
{
    cout << "The due date for this car is:\t" << due_date << '\n';
}

void Car::Display()
{
    cout << "Company of the Car:\t" << Company << '\n';
    cout << "Model of the Car:\t" << Model << '\n';
    cout << "Registration Number:\t" << regNo << '\n';
    cout << "Condition of the Car:\t" << Condition << "\n\n";
}

bool Car::operator<(const Car &c) const { return ((this->regNo) < c.regNo); }
void Car::Rented()
{
    if (rented_to == manager_id)
    {
        cout << "This car is available for rent";
    }
    else
    {
        cout << "This car is rented to the user with ID no. : " << rented_to;
    }
    cout << "\n";
}

void Car_DB::Add(Car c) { database[c.regNo] = c; }
void Car_DB::Delete(Car c)
{
    if (Contains(c.regNo))
    {
        database.erase(c.regNo);
    }
}
void Car_DB::Delete(string regNo)
{
    if (Contains(regNo))
    {
        database.erase(regNo);
    }
}
void Car_DB::Update(Car c, string user, int new_date)
{
    if (!Contains(c.regNo))
    {
        return;
    }
    c.rented_to = user;
    c.due_date = new_date;
    database[c.regNo] = c;
}
void Car_DB::Update(Car c) { database[c.regNo] = c; }
Car Car_DB::Search(string regNo) { return database[regNo]; }
bool Car_DB::Contains(string regNo)
{
    return (database.find(regNo) != database.end());
}
bool Car_DB::Rent_Car(string(&user), string regNo, int today)
{
    cout << "\n";
    bool rent;
    if (Contains(regNo))
    {
        Car c = database[regNo];
        rent = c.Car_Rent(user, today);
        Update(c);
    }
    else
    {
        cout << "Car does not exist in the store, please check the reg number you "
                "entered.\n";
        rent = false;
    }
    cout << "\n";
    return rent;
}
void Car_DB::Display()
{
    cout << "\n\n";
    if (database.size() == 0)
    {
        cout << "The store is currently empty!\n";
        return;
    }
    cout << "The store contains:\n\n";
    for (auto i : database)
    {
        Car c = i.second;
        c.Display();
    }
    cout << "\n";
}
void Car_DB::Check_availability()
{
    cout << "\nEnter the reg number of the car you want to check availability: ";
    string i;
    cin >> i;
    if (Contains(i))
    {
        Car c = database[i];
        c.Check_availability();
    }
    else
    {
        cout << "Car does not exist in the store, please check the reg number you "
                "entered.\n";
    }
    cout << "\n";
}

int User::Calculate_fine(int today = TODAY)
{
    if (this->ID == manager_id)
    {
        cout << "\nThis is Manager, he can't have fine.\n";
        return 0;
    }
    int fine = 0;
    for (auto c : rented_cars)
    {
        fine += fineamt * max(today - c.due_date, 0);
    }
    if (fine)
    {
        cout << "Your current outstanding fines are: " << fine << ".\n";
    }
    else
    {
        cout << "You currently have no pending fines.\n";
    }
    return fine;
}

void User::Return_cars(int today = TODAY)
{
    if (this->ID == manager_id)
    {
        cout << "\nThis is Manager.\n";
        return;
    }
    int fine = Calculate_fine(today);
    cout << "\nClearing a fine of:\t" << fine << '\n';
    cout << "Returning all cars that you currently have.\n...";
    for (auto c : rented_cars)
    {
        CAR_DB.Update(c, manager_id, 0);
    }
    rented_cars.clear();
    cout << "Returned successfully.\n";
}

bool User::operator==(const User &u)
{
    return (this->ID == u.ID) && (this->password == u.password);
}

bool User::Check_password(string pwd) { return password == pwd; }
void User::Show_cars()
{
    if (rented_cars.size() == 0)
    {
        cout << "No cars issued yet.\n\n";
        return;
    }
    cout << "The following cars have been issued:\n\n";
    for (auto c : rented_cars)
    {
        c.Display();
    }
}

void User::Rent_Car()
{
    if (max_cars && rented_cars.size() == max_cars)
    {
        cout << "Sorry, you cannot rent more cars!\n";
        return;
    }
    cout << "Please enter the reg number of the car you want to rent: ";
    string regNo;
    cin >> regNo;
    if (CAR_DB.Rent_Car(this->ID, regNo, TODAY + rent_days))
    {
        rented_cars.insert(CAR_DB.Search(regNo));
    }
}

void User::Clear_fine_amount(int today = TODAY)
{
    cout << "\nReturning the cars which are due...\n\n";

    set<Car> to_return;
    for (auto c : rented_cars)
    {
        if (c.due_date >= today)
        {
            to_return.insert(c);
        }
    }
    for (auto c : to_return)
    {
        rented_cars.erase(c);
    }
}

void User::Logout()
{
    logged_in = &dummy;
    TODAY++;
    cout << "\nLogged out successfully!\n";
}

void User::Display()
{
    cout << "\n";
    cout << "Name of the user:\t" << name;
    cout << "\nID of the user:\t\t" << ID;
    cout << "\n";
}

void User::Help()
{
    cout << "\n\nEnter:\n";
    cout << "\t1:\t To log-in.\n";
    cout << "\t2:\t To exit and terminate the program.\n";
    cout << "Option: \t";
    int instruction;
    cin >> instruction;
    switch (instruction)
    {
    case 1:
        Login();
        break;
    case 2:
        program_terminate = true;
        break;
    default:
        cout << "Invalid input.\n";
        break;
    }
    cout << "\n\n";
}

void User::Show_dues()
{
    bool due = false;

    for (auto c : rented_cars)
    {
        due = true;
        c.Show_duedate();
    }
    if (!due)
    {
        cout << "\nNo cars are on due right now.\n";
    }
}

Employee::Employee(string n, string id, string pwd)
{
    name = n, ID = id, password = pwd;
    fineamt = 10;
    type = "e";
    rent_days = 25;
}

void Employee::Help()
{
    cout << "\nEnter:\n";
    cout << "\t1:\t Display all the cars in store.\n";
    cout << "\t2:\t Display all the cars issued to you.\n";
    cout << "\t3:\t Show the cars which are due.\n";
    cout << "\t4:\t Check availability of a car.\n";
    cout << "\t5:\t Rent a car.\n";
    cout << "\t6:\t See your outstanding fines.\n";
    cout << "\t7:\t Clear your fines and return the cars which are due.\n";
    cout << "\t8:\t Return all your cars.\n";
    cout << "\t9:\t Log-out.\n";
    cout << "\t10:\t Exit and terminate the program.\n";
    cout << "Option: \t";

    int instruction;
    cin >> instruction;
    switch (instruction)
    {
    case 1:
        CAR_DB.Display();
        break;
    case 2:
        Show_cars();
        break;
    case 3:
        Show_dues();
        break;
    case 4:
        CAR_DB.Check_availability();
        break;
    case 5:
        Rent_Car();
        break;
    case 6:
        Calculate_fine();
        break;
    case 7:
        Clear_fine_amount();
        break;
    case 8:
        Return_cars();
        break;
    case 9:
        (*logged_in).Logout();
        break;
    case 10:
        program_terminate = true;
        break;
    default:
        cout << "Invalid input.\n";
        break;
    }
    cout << "\n";
}

Customer::Customer(string n, string id, string p)
{
    name = n, ID = id, password = p;
    fineamt = 10, type = "c";
    rent_days = 25;
}

void Customer::Help()
{
    cout << "\nEnter:\n";
    cout << "\t1:\t Display all the cars in the store.\n";
    cout << "\t2:\t Display all the cars issued to you.\n";
    cout << "\t3:\t Show the cars which are on due.\n";
    cout << "\t4:\t Check availability of a car.\n";
    cout << "\t5:\t Rent a car.\n";
    cout << "\t6:\t See your outstanding fines.\n";
    cout << "\t7:\t Clear your fines and return the cars which are on due.\n";
    cout << "\t8:\t Return all your cars.\n";
    cout << "\t9:\t Logout.\n";
    cout << "\t10:\t Exit and terminate the program.\n";
    cout << "Option: \t";

    int instruction;
    cin >> instruction;
    switch (instruction)
    {
    case 1:
        CAR_DB.Display();
        break;
    case 2:
        Show_cars();
        break;
    case 3:
        Show_dues();
        break;
    case 4:
        CAR_DB.Check_availability();
        break;
    case 5:
        Rent_Car();
        break;
    case 6:
        Calculate_fine();
        break;
    case 7:
        Clear_fine_amount();
        break;
    case 8:
        Return_cars();
        break;
    case 9:
        Logout();
        break;
    case 10:
        program_terminate = true;
        break;
    default:
        cout << "Invalid input.\n";
        break;
    }
    cout << "\n";
}

Manager::Manager()
{
    name = "Manager", ID = "manager", password = "cars";
    type = "m";
    USER_DB.Add(this);
}

void Manager::Add_Car()
{
    string c, m, regNo, con;
    cout << "\n\nPlease enter the reg number of the car: ";
    cin >> regNo;
    if (CAR_DB.Contains(regNo))
    {
        cout << "Car with this regNo number already exists! Cannot add another "
                "car.\n";
        return;
    }
    cout << "Please enter company of the car: ";
    cin >> c;
    cout << "Please enter model of the car: ";
    cin >> m;
    CAR_DB.Add(Car(c, m, regNo, "good"));
    cout << "Car added to the store successfully!\n";
}

void Manager::Add_User()
{
    string n, i, p, t;
    cout << "\n\nEnter the ID of the user: ";
    cin >> i;
    if (USER_DB.Contains(i))
    {
        cout << "User with this ID already exists! Cannot add another User.\n";
        return;
    }
    cout << "Enter the name of the user: ";
    cin >> n;
    cout << "Enter the password of the user: ";
    cin >> p;
    cout << "Enter the type of user (e/c): ";
    cin >> t;

    User *u;
    if (t[0] == 'e' || t[0] == 'E')
    {
        u = new Employee(n, i, p);
        USER_DB.Add(u);
        cout << "Employee added successfully!\n";
        return;
    }
    else if (t[0] == 'c' || t[0] == 'C')
    {
        User *u = new Customer(n, i, p);
        USER_DB.Add(u);
        cout << "Customer added successfully!\n";
        return;
    }
    else
    {
        cout << "Invalid input.\n";
        return;
    }
}

void Manager::Delete_Car()
{
    string regNo;
    cout << "\nEnter the reg number of the car to delete: ";
    cin >> regNo;
    cout << "\n";
    if (CAR_DB.Contains(regNo))
    {
        CAR_DB.Delete(regNo);
        cout << "Car deleted successfully!\n";
    }
    else
    {
        cout << "Car is not present in the store!\n";
    }
    cout << "\n";
}

void Manager::Delete_User()
{
    string id;
    cout << "\nEnter the ID of the user to delete: ";
    cin >> id;
    cout << "\n";
    if (id == manager_id)
    {
        cout << "Cannot delete the Manager!\n";
        return;
    }
    if (USER_DB.Contains(id))
    {
        User *u = USER_DB.Search(id);
        u->Clear_fine_amount();
        u->Return_cars();
        USER_DB.Delete(id);
        cout << "User deleted successfully.\n";
    }
    else
    {
        cout << "User is not present in the database!\n";
    }
    cout << "\n";
}

void Manager::Check_rent()
{
    cout << "\nEnter the reg number of the car: ";
    string regNo;
    cin >> regNo;
    if (CAR_DB.Contains(regNo))
    {
        Car car = CAR_DB.Search(regNo);
        car.Rented();
    }
    else
    {
        cout << "Car does not exist in the store, please check the reg number you "
                "entered.\n";
    }
}

void Manager::Change_day()
{
    cout << "\nEnter the new day (integer): ";
    int t;
    cin >> t;
    if (t < TODAY)
    {
        cout << "New day cannot be smaller than today!\n";
    }
    else if (t == TODAY)
    {
        cout << "This is already the day today.\n";
    }
    else
    {
        cout << "Day has been changed from " << TODAY << " to " << t << ".\n";
        TODAY = t;
    }
}

void Manager::User_Cars()
{
    cout << "\nEnter the ID of the user: ";
    string id;
    cin >> id;
    cout << "\n";
    if (USER_DB.Contains(id))
    {
        auto user = USER_DB.Search(id);
        user->Show_cars();
    }
    else
    {
        cout << "User with this ID does not exist.\n";
    }
}

void Manager::Help()
{
    cout << "\nEnter:\n";
    cout << "\t1:\t Display all the cars in the store.\n";
    cout << "\t2:\t Display all the current users.\n";
    cout << "\t3:\t Show cars rented to a particular user.\n";
    cout << "\t4:\t Check availability of a car.\n";
    cout << "\t5:\t Add a car.\n";
    cout << "\t6:\t Add a user.\n";
    cout << "\t7:\t Delete a car.\n";
    cout << "\t8:\t Delete a user.\n";
    cout << "\t9:\t Check user who is currently using a particular car.\n";
    cout << "\t10:\t Show the day today.\n";
    cout << "\t11:\t Change the day today.\n";
    cout << "\t12:\t Log-out.\n";
    cout << "\t13:\t Exit and terminate the program.\n";
    cout << "Option: \t";
    int instruction;
    cin >> instruction;
    switch (instruction)
    {
    case 1:
        CAR_DB.Display();
        break;
    case 2:
        USER_DB.Display();
        break;
    case 3:
        User_Cars();
        break;
    case 4:
        CAR_DB.Check_availability();
        break;
    case 5:
        Add_Car();
        break;
    case 6:
        Add_User();
        break;
    case 7:
        Delete_Car();
        break;
    case 8:
        Delete_User();
        break;
    case 9:
        Check_rent();
        break;
    case 10:
        Show_today();
        break;
    case 11:
        Change_day();
        break;
    case 12:
        (*logged_in).Logout();
        break;
    case 13:
        program_terminate = true;
        break;
    default:
        cout << "Invalid input.\n";
        break;
    }
    cout << "\n";
}

void User_DB::Add(User *u) { database[u->ID] = u; }
bool User_DB::Delete(User *u)
{
    if (u->ID == manager_id)
    {
        cout << "Cannot remove the manager!\n";
        return false;
    }
    else
    {
        if (database.find(u->ID) != database.end())
        {
            cout << "User not in database.\n";
            return false;
        }
        else
        {
            database.erase(u->ID);
            delete u;
            return true;
        }
    }
}
bool User_DB::Delete(string ID)
{
    if (ID == manager_id)
    {
        cout << "Cannot remove the manager!\n";
        return false;
    }
    else
    {
        if (Contains(ID))
        {
            auto u = database[ID];
            database.erase(ID);
            delete u;
            return true;
        }
        else
        {
            cout << "User not in database.\n";
            return false;
        }
    }
}
User *User_DB::Search(string ID) { return database[ID]; }
bool User_DB::Contains(string ID)
{
    return database.find(ID) != database.end();
}
void User_DB::Display()
{
    cout << "\n";
    for (auto u : database)
    {
        auto user = *(u.second);
        user.Display();
    }
    cout << "\n";
}

void Login()
{
    cout << "\n\n";
    string id, pwd;
    cout << "Enter your ID: ";
    cin >> id;
    if (USER_DB.Contains(id))
    {
        cout << "Enter your password: ";
        cin >> pwd;
        auto u = USER_DB.Search(id);
        if (u->Check_password(pwd))
        {
            logged_in = USER_DB.Search(id);
        }
        else
        {
            cout << "Incorrect password!\n";
        }
    }
    else
    {
        cout << "\nUser with this ID does not exist.\n";
        cout << "Please ask the manager to add the ID to the database.\n";
    }
}

void Show_today() { cout << "\nToday is " << TODAY << ".\n"; }

void deploy()
{

    while (!program_terminate)
    {
        logged_in->Help();
    }

    cout << "\nClosing the program...\n";
    cout << "     _____                 _   _        \n";
    cout << "   / ____|               | | | |         \n ";
    cout << "  | |  __  ___   ___   __| | | |__  _   _  ___ \n";
    cout << "   | | |_ |/ _ \\ / _ \\ / _` | | '_ \\| | | |/ _ \\ \n";
    cout << "   | |__| | (_) | (_) | (_| | | |_) | |_| |  __/ \n";
    cout << "    \\_____|\\___/ \\___/ \\__,_| |_.__/ \\__, |\\___| \n";
    cout << "                                      __/ |    \n ";
    cout << "                                     |___/   ";
    return;
}

int main()
{

    cout << "\n\
     __      __       .__                                \n\
    /  \\    /  \\ ____ |  |   ____  ____   _____   ____   \n\
    \\   \\/\\/   // __ \\|  | _/ ___\\/  _ \\ /     \\_/ __ \\  \n\
     \\        /\\  ___/|  |_\\  \\__(  <_> )  Y Y  \\  ___/  \n\
      \\__/\\__/  \\____ >____/\\____ >____/|__|_|__/\\____ > \n\
    ";
    deploy();

    return 0;
}
// manager help
// user added sucesfully