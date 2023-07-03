#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <limits>


using namespace std;

// Helper functions
void clrscr() {
    cout << "\033[2J\033[1;1H";
}

vector<string> splitString(string str, string delimiter) {
    vector<string> tokens;
    int start = 0;
    int end = str.find(delimiter);
    while (end != -1) {
        tokens.push_back(str.substr(start, end - start));
        start = end + delimiter.size();
        end = str.find(delimiter, start);
    }
    tokens.push_back(str.substr(start, end - start));
    return tokens;
}


class Item {
    private:
    string name;
    double price;
    int calories;

    public:
    // Parameterized constructor
    Item(string name, double price, int calories) {
        this->name = name;
        this->price = price;
        this->calories = calories;
    }

    string getName() {
        return name;
    }

    double getPrice() {
        return price;
    }

    int getCalories() {
        return calories;
    }

    string toString() {
        stringstream buffer;
        buffer << setw(20) << name << setw(10) << calories << setw(10) << price;
        return buffer.str();
    }
};

class OrderItem {
    private:
    string item_name;
    int quanity;
    double item_price;
    double line_total;

    public:
    OrderItem(string item_name, int quanity, double item_price, double line_total) {
        this->item_name = item_name;
        this->quanity = quanity;
        this->item_price = item_price;
        this->line_total = line_total;
    }

    string toString() {
        stringstream buffer;
        buffer << setw(20) << item_name << setw(15) << "$" + to_string(item_price) << setw(10) << quanity << setw(15) << "$" + to_string(line_total);
        return buffer.str();
    }
};

class Order {
    private:
    vector<OrderItem> order_items;
    double order_total;

    public:
    void addToOrder(string item_name, double item_price, int quanity) {
        double line_total = quanity * item_price;
        OrderItem new_item(item_name, quanity, item_price, line_total);
        order_items.push_back(new_item);
        order_total += line_total;
    }

    bool isValidOrder() {
        return order_items.size();
    }

    double getOrderTotal() {
        return order_total;
    }

    string toString() {
        stringstream buffer;
        for (int i=0; i<order_items.size(); i++) {
            buffer << setw(5) << i+1 << order_items[i].toString() << endl;
        }
        buffer << setw(65) << "Total = $" + to_string(order_total) << endl;
        return buffer.str();
    }
};

class Shop {
    private:
    string items_file;
    vector<Item> items;
    vector<Order> orders;
    double day_total;

    public:
    Shop(string items_file) {
        this->items_file = items_file;
        this->day_total = 0;
        this->getItemsFromFile();
        this->printHomePage();
    }

    void printHomePage() {
        bool flag = true;
        while(flag) {
            cout << "\n\nWelcome to the Shop!" << endl;
            cout << "1. Add Item to Menu" << endl;
            cout << "2. Print Menu" << endl;
            cout << "3. Take Order" << endl;
            cout << "4. Print Orders" << endl;
            cout << "5. Close Shop " << endl;
            cout << "Enter your choice: ";
            int choice;
            cin >> choice;
            switch (choice) {
                case 1:
                    clrscr();
                    addItem();
                    break;
                case 2: 
                    clrscr();
                    printMenu();
                    break;
                case 3:
                    clrscr();
                    takeOrder();
                    break;
                case 4:
                    clrscr();
                    printOrders();
                    break;
                case 5:
                    clrscr();
                    flag = false;
                    break;
                default:
                    clrscr();
                    cout << "Invalid Choice" << endl;
            }
        }
    }

    void addItem() {
        string name;
        int calories;
        double price;
        cout <<"Enter details of new item: \n";
        cout <<"Enter name: ";
        getline(cin >> ws, name);
        cout <<"Enter calories: ";
        cin >> calories;
        cout <<"Enter price: $";
        cin >> price;
        Item new_item(name, price, calories);
        items.push_back(new_item);
        cout << "Added Item!" << endl;
    }

    void getItemsFromFile() {
        // The items are saved in the following format: name;price;calories
        ifstream file (this->items_file);
        string line;
        while(getline(file, line)) {
            // split line at ';'
            string delimiter = ";";
            vector<string> tokens = splitString(line, delimiter);
            // Create Item object and add to items vector
            if (tokens.size() != 3) {
                cout<< "Error! Items file is not in the right format!";
                exit(EXIT_FAILURE);
            }
            Item item(tokens[0], stod(tokens[1]), stoi(tokens[2]));
            this->items.push_back(item);
        }
    }

    void saveItemsToFile() {
        // Saving to file
        ofstream file;
        file.open(this->items_file);
        for (int i=0; i<items.size(); i++) {
            Item currItem = items[i];
            // Each line 
            // name;price;calories
            file << currItem.getName() << ";" << currItem.getPrice() << ";" << currItem.getCalories() << endl;
        }
        file.close();
    }

    void printMenu() {
        cout << "Menu: \n";
        cout << setw(5) << "S.no" << setw(20) << "Name" << setw(10) << "Calories" << setw(10) << "Price" << endl;
        for (int i=0; i<items.size(); i++) {
            cout << setw(5) << i + 1 << items[i].toString() << endl;
        }
    }

    void takeOrder() {
        bool add_items = true;
        Order order;
        while (add_items) {
            int choice = -1;
            int quantity = -1;
            printMenu();
            cout << "Enter your choice: ";
            cin >> choice;
            cout << "Enter quantity: ";
            cin >> quantity;
            // Validate the choice, if incorrect print error and continue.
            // choice can be in the range [1, items.size()].
            // quantity has to be more than 0.
            if (choice >= 1 && choice <= items.size() && quantity > 0) { 
                order.addToOrder(items[choice-1].getName(), items[choice-1].getPrice(), quantity);
                cout << "Added item to order!"<<endl;
            }
            else {
                cout << "Incorrect value for choice or quantity!" << endl;
            }
            string more_choice;
            cout << "Would you like to add more items? (y or n) ";
            cin >> more_choice;
            if (more_choice != "y") 
                add_items = false;
        }
        if (order.isValidOrder()) {
            cout << "Order Complete!" << endl;
            orders.push_back(order);
            day_total += order.getOrderTotal();
        }
        else
            cout << "Invalid order!" << endl;
    }

    void printOrders() {
        cout <<"Orders: \n";
        for (int i=0; i<orders.size(); i++) {
            cout << "Order "<< setw(5) << i + 1 << ": \n";
            cout << orders[i].toString() << endl;
            cout <<endl;
        }
    }

    ~Shop() {
        cout << "Closing the shop for the day!" <<endl;
        cout << "Total for the day was: $" << day_total <<endl;
        saveItemsToFile();

    }
};


int main(int argc, char** argv) {
    // Getting the command line arguments
    string items_file = argv[1];
    Shop shop(items_file);
    return 0; 
}