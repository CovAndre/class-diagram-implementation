#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Product {
public:
    string productID;
    string name;
    double price;
    int stockQuantity;

    Product(string id, string n, double p, int s) : productID(id), name(n), price(p), stockQuantity(s) {}

    void updateStock(int quantity) {
        stockQuantity += quantity;
    }

    void displayProduct() {
        cout << productID << " | " << name << " | " << price << " | " << stockQuantity << endl;
    }
};

class ShoppingCart {
public:
    unordered_map<string, pair<Product*, int>> items;

    void addProduct(Product* product, int quantity) {
        if (product->stockQuantity >= quantity) {
            items[product->productID] = {product, items[product->productID].second + quantity};
            product->updateStock(-quantity);
            cout << "Product added successfully!" << endl;
        } else {
            cout << "Insufficient stock." << endl;
        }
    }

    void displayCart() {
        cout << "Product ID | Name | Price | Quantity" << endl;
        for (auto& item : items) {
            cout << item.first << " | " << item.second.first->name << " | " << item.second.first->price << " | " << item.second.second << endl;
        }
    }
};

class Order {
public:
    static int orderCounter;
    int orderID;
    unordered_map<string, pair<Product*, int>> cart;
    double totalAmount;
    string orderStatus;

    Order(ShoppingCart* shoppingCart, double total) {
        orderID = ++orderCounter;
        cart = shoppingCart->items;
        totalAmount = total;
        orderStatus = "Placed";
    }

    void displayOrder() {
        cout << "\nOrder ID: " << orderID << "\nTotal Amount: " << totalAmount << "\nStatus: " << orderStatus << endl;
        cout << "Product ID | Name | Price | Quantity" << endl;
        for (auto& item : cart) {
            cout << item.first << " | " << item.second.first->name << " | " << item.second.first->price << " | " << item.second.second << endl;
        }
    }
};

int Order::orderCounter = 0;

int main() {
    vector<Product> products = {
        {"ABC", "Paper", 20, 100},
        {"CDE", "Pencil", 10, 200},
        {"QWE", "Paper", 20, 150},
        {"TRE", "Pencil", 10, 300},
        {"ASD", "Paper", 20, 120},
        {"ZXC", "Pencil", 10, 250},
    };

    ShoppingCart cart;
    vector<Order> orders;

    while (true) {
        cout << "\n1. View Products\n2. View Shopping Cart\n3. View Orders\n4. Exit\nEnter choice: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            cout << "\nAvailable Products:" << endl;
            cout << "Product ID | Name | Price | Stock" << endl;
            for (auto& product : products) {
                product.displayProduct();
            }
            while (true) {
                string prodID;
                cout << "Enter Product ID to add (or 'exit' to return): ";
                cin >> prodID;
                if (prodID == "exit") break;
                int qty;
                cout << "Enter quantity: ";
                cin >> qty;
                auto it = find_if(products.begin(), products.end(), [&](Product& p) { return p.productID == prodID; });
                if (it != products.end()) {
                    cart.addProduct(&(*it), qty);
                } else {
                    cout << "Invalid Product ID." << endl;
                }
            }
        } else if (choice == 2) {
            cart.displayCart();
            cout << "Do you want to check out? (1 for Yes, 0 for No): ";
            int checkout;
            cin >> checkout;
            if (checkout) {
                double total = 0;
                for (auto& item : cart.items) {
                    total += item.second.first->price * item.second.second;
                }
                orders.emplace_back(&cart, total);
                cart.items.clear();
                cout << "You have successfully checked out!" << endl;
            }
        } else if (choice == 3) {
            if (orders.empty()) cout << "No orders placed yet." << endl;
            else for (auto& order : orders) order.displayOrder();
        } else if (choice == 4) {
            cout << "Exiting..." << endl;
            break;
        } else {
            cout << "Invalid choice. Try again." << endl;
        }
    }
    return 0;
}
