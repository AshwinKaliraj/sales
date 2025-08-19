#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
using namespace std;

struct Sale {
    string id, date, item;
    int Quantity;
    double price;
};

vector<Sale> loadCSV(const string &file) {
    ifstream fin(file);
    vector<Sale> data;
    string line;
    bool skipHeader = true;
    while (getline(fin, line)) {
        if (skipHeader) { skipHeader = false; continue; }
        stringstream ss(line);
        Sale s;
        string QuantityStr, priceStr;
        getline(ss, s.id, ',');
        getline(ss, s.date, ',');
        getline(ss, s.item, ',');
        getline(ss, QuantityStr, ',');
        getline(ss, priceStr, ',');
        if (!s.id.empty()) {
            s.Quantity = stoi(QuantityStr);
            s.price = stod(priceStr);
            data.push_back(s);
        }
    }
    return data;
}

void saveCSV(const string &file, const vector<Sale> &data) {
    ofstream fout(file);
    fout << "ID,Date,Item,Quantity,Price\n";
    for (auto &s : data)
        fout << s.id << "," << s.date << "," << s.item << "," << s.Quantity << "," << s.price << "\n";
}

void SortAndReport(const string &file, const string &tempfile) {
    auto data = loadCSV(file);


    sort(data.begin(), data.end(), [](Sale &a, Sale &b) { return a.date < b.date; });
    saveCSV(tempfile, data);


    ofstream fout("Report.txt");
    fout << "------------------- Sales Report -------------------\n";
    fout << left << setw(12) << "Date" << setw(10) << "SaleID" << setw(15) << "Item" << setw(10) << "Quantity"
         << setw(10) << "Price" << setw(12) << "Amount" << "\n"
         << "----------------------------------------------------\n";

    string prevDate = "";
    double dailyTotal = 0, grandTotal = 0;

    for (auto &s : data) {
        double amount = s.Quantity * s.price;
        if (!prevDate.empty() && prevDate != s.date) {
            fout << "Subtotal for " << prevDate << " : " << dailyTotal << "\n\n";
            dailyTotal = 0;
        }
        fout << left << setw(12) << s.date << setw(10) << s.id << setw(15) << s.item << setw(10) << s.Quantity
             << setw(10) << s.price << setw(12) << amount << "\n";

        dailyTotal += amount;
        grandTotal += amount;
        prevDate = s.date;
    }

    if (!prevDate.empty()) fout << "Subtotal for " << prevDate << " : " << dailyTotal << "\n";
    fout << "====================================================\n"
         << " GRAND TOTAL : " << grandTotal << "\n"
         << "====================================================\n";

    cout << " Data Sorted & Report.txt generated!\n";
}

int getNextId(const string &file) {
    auto data = loadCSV(file);
    int maxId = 0;
    for (auto &s : data) {
        
    }
    return maxId + 1;
}

void create(const string &file, const string &tempfile) {
    ifstream fin(file);
    bool fileEmpty = fin.peek() == ifstream::traits_type::eof();
    fin.close();

    ofstream fout(file, ios::app);
    Sale s;

    cout << "Enter Date (YYYY-MM-DD): "; cin >> s.date;
    cout << "Enter Item: "; cin >> s.item;
    cout << "Enter Quantity: "; cin >> s.Quantity;
    cout << "Enter Unit Price: "; cin >> s.price;

    
    s.id = to_string(getNextId(file));

    if (fileEmpty) fout << "ID,Date,Item,Quantity,Price\n";
    fout << s.id << "," << s.date << "," << s.item << "," << s.Quantity << "," << s.price << "\n";
    fout.close();

    cout << "Sale added successfully with ID " << s.id << "!\n";
    SortAndReport(file, tempfile);
}


void Read(const string &file) {
    auto data = loadCSV(file);
    cout << left << setw(10) << "ID" << setw(12) << "Date" << setw(15)
         << "Item" << setw(8) << "Quantity" << setw(10) << "Price" << "\n";
    for (auto &s : data)
        cout << left << setw(10) << s.id << setw(12) << s.date << setw(15)
             << s.item << setw(8) << s.Quantity << setw(10) << s.price << "\n";
}

void Update(const string &file, const string &tempfile) {
    auto data = loadCSV(file);
    string id; cout << "Enter Sale ID to update: "; cin >> id;
    for (auto &s : data) {
        if (s.id == id) {
            cout << "Current Item: " << s.item << " | Enter new Item: "; cin >> s.item;
            cout << "Current Quantity: " << s.Quantity << " | Enter new Quantity: "; cin >> s.Quantity;
            cout << "Current Price: " << s.price << " | Enter new Price: "; cin >> s.price;
            break;
        }
    }
    saveCSV(file, data);
    cout << "Sale updated!\n";
    SortAndReport(file, tempfile);
}

void Delete(const string &file, const string &tempfile) {
    auto data = loadCSV(file);
    string id; cout << "Enter Sale ID to delete: "; cin >> id;
    data.erase(remove_if(data.begin(), data.end(),
                         [&](Sale &s) { return s.id == id; }),
               data.end());
    saveCSV(file, data);
    cout << "Sale ID deleted!\n";
    SortAndReport(file, tempfile);
}

int main() {
    string file = "sales.csv", tempfile = "temp.csv";
    int choice;
    do {
        cout << "\nSales Report Menu \n";
        cout << "1. Do you want to Create data? \n2. Do you want to Read data? \n3. Do you want to Update data? \n4. Do you want to Delete data? \n5. Exit\nChoice: ";
        cin >> choice;
        switch (choice) {
            case 1: create(file, tempfile); break;
            case 2: Read(file); break;
            case 3: Update(file, tempfile); break;
            case 4: Delete(file, tempfile); break;
        }
    } while (choice != 5);


    SortAndReport(file, tempfile);
    cout << "Exiting... Final report generated.\n";
}
