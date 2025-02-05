#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <vector>
#include <unordered_map>

using namespace std;

const int INF = 1e9; // Large value representing infinity

// Data Module: Stores city names and travel costs
class datamodule {
public:
    static const int N = 15; // Number of cities
    string city[N] = { "Delhi", "Mumbai", "Bangalore", "Chennai", "Kolkata",
                       "Hyderabad", "Ahmedabad", "Pune", "Jaipur", "Lucknow",
                       "Chandigarh", "Indore", "Nagpur", "Patna", "Bhopal" };
    int cost[N][N]; // Matrix to store travel costs between cities
    unordered_map<string, int> cityIndex; // Mapping city names to indices

    datamodule() {
        // Initialize cost matrix with infinity, except diagonal elements as 0
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                cost[i][j] = (i == j) ? 0 : INF;
            }
            cityIndex[city[i]] = i; // Map city name to its index
        }
        initialize_cost(); // Initialize specific costs
    }

    void initialize_cost() {
        // Hardcoded travel costs between specific cities
        cost[0][1] = 500; cost[1][0] = 500;
        cost[0][2] = 700; cost[2][0] = 700;
        cost[1][2] = 300; cost[2][1] = 300;
    }
};

// Bellman-Ford Algorithm: Finds the shortest path between cities
class bellmanfordalgorithm : public datamodule {
public:
    int dist[N]; // Stores shortest distances from the source

    void algorithmic_implementation(int src) {
        // Initialize distances to infinity
        for (int i = 0; i < N; i++) dist[i] = INF;
        dist[src] = 0; // Distance to itself is 0
        
        // Bellman-Ford algorithm for finding shortest paths
        for (int iter = 0; iter < N - 1; iter++) {
            bool updated = false;
            for (int u = 0; u < N; u++) {
                for (int v = 0; v < N; v++) {
                    if (dist[u] != INF && dist[v] > dist[u] + cost[u][v]) {
                        dist[v] = dist[u] + cost[u][v];
                        updated = true;
                    }
                }
            }
            if (!updated) break; // If no updates, terminate early
        }
    }
};

// File Handling Module: Manages user authentication and booking history
class filehandlingmodule {
public:
    // Check if user credentials exist in login file
    bool check_user(string username, string password) {
        ifstream file("login.txt");
        if (!file) return false;
        string u, p;
        while (file >> u >> p) {
            if (u == username && p == password) return true;
        }
        return false;
    }

    // Register a new user
    void register_user(string username, string password) {
        ofstream file("login.txt", ios::app);
        file << username << " " << password << endl;
        cout << "User registered successfully!\n";
    }

    // Book a ticket and save the details to file
    void book_ticket(string username, string src, string dest, int price) {
        ofstream file("tickets.txt", ios::app);
        file << username << " " << src << " " << dest << " " << price << endl;
        cout << "Ticket booked successfully!\n";
    }

    // Display booking history of a user
    void view_ticket_history(string username) {
        ifstream file("tickets.txt");
        string u, src, dest;
        int price;
        bool found = false;
        while (file >> u >> src >> dest >> price) {
            if (u == username) {
                cout << "From " << src << " to " << dest << " - Rs. " << price << endl;
                found = true;
            }
        }
        if (!found) cout << "No booking history found.\n";
    }
};

// Input Module: Handles user authentication input
class inputmodule {
public:
    filehandlingmodule fh;
    
    string login() {
        string username, password;
        cout << "Enter Username: "; cin >> username;
        cout << "Enter Password: "; cin >> password;
        return (fh.check_user(username, password)) ? username : "";
    }

    void register_user() {
        string username, password;
        cout << "Choose a Username: "; cin >> username;
        cout << "Choose a Password: "; cin >> password;
        fh.register_user(username, password);
    }
};

// Output Module: Displays cities and calculates fares
class outputmodule {
public:
    void display_cities(datamodule& dm) {
        cout << "Available Cities:\n";
        for (int i = 0; i < dm.N; i++) {
            cout << i + 1 << ". " << dm.city[i] << "\n";
        }
    }

    void display_price(bellmanfordalgorithm& algo, string src, string dest, string username, filehandlingmodule& fh) {
        if (algo.cityIndex.find(src) == algo.cityIndex.end() || algo.cityIndex.find(dest) == algo.cityIndex.end()) {
            cout << "Invalid city names.\n";
            return;
        }

        int srcIdx = algo.cityIndex[src], destIdx = algo.cityIndex[dest];
        algo.algorithmic_implementation(srcIdx);

        if (algo.dist[destIdx] == INF) {
            cout << "No route available.\n";
        } else {
            cout << "Minimum fare from " << src << " to " << dest << " is Rs. " << algo.dist[destIdx] << "\n";
            fh.book_ticket(username, src, dest, algo.dist[destIdx]);
        }
    }
};

// Main Function: Entry point of the system
int main() {
    datamodule dm;
    bellmanfordalgorithm algo;
    filehandlingmodule fh;
    inputmodule im;
    outputmodule om;

    while (true) {
        cout << "\n=== Travel Management System ===\n1. Login\n2. Register\n3. Exit\nEnter choice: ";
        int choice; cin >> choice;
        
        string username;
        if (choice == 1) {
            username = im.login();
            if (username.empty()) continue;
        } else if (choice == 2) {
            im.register_user();
            continue;
        } else break;

        while (true) {
            cout << "\n=== Travel Options ===\n1. View Cities\n2. Book Ticket\n3. View Ticket History\n4. Logout\nEnter choice: ";
            int option; cin >> option;
            
            if (option == 1) om.display_cities(dm);
            else if (option == 2) {
                string src, dest;
                cout << "Enter Source City: "; cin >> src;
                cout << "Enter Destination City: "; cin >> dest;
                om.display_price(algo, src, dest, username, fh);
            } else if (option == 3) {
                fh.view_ticket_history(username);
            } else break;
        }
    }
    return 0;
}
