#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<string> split(string str, char del) {
    vector<string> result;
    // declaring temp string to store the curr "word" upto del
    string temp = "";
    for (int i = 0; i < (int)str.size(); i++) {
        // If current char is not ',', then append it to the "word", otherwise you have completed the word, and start the next word
        if (str[i] != del) {
            temp += str[i];
        }
        else {
            result.push_back(temp);
            temp = "";
        }
    }
    // push the last substring after the last ',' to the result
    result.push_back(temp);
    return result;
}

class cvsdata {
    string date;
    string country;
    int cases;
    int deaths;
public:
    cvsdata(string date, string country, int cases, int deaths) {
        this->date = date;
        this->country = country;
        this->cases = cases;
        this->deaths = deaths;
    }

    string getDate() {
        return date;
    }

    string getCountry() {
        return country;
    }

    int getCases() {
        return cases;
    }

    int getDeaths() {
        return deaths;
    }
};

class DataEntry
{
public:
    DataEntry();
    string getDate();
    string getCountry();
    int getCases();
    int getDeaths();
    void setDate(string newDate);
    void setCountry(string newCountry);
    void setCases(int newCases);
    void setDeaths(int newDeaths);

private:
    string date;
    string country;
    int c_cases;
    int c_deaths;
};

DataEntry::DataEntry()
{
}

string DataEntry::getDate() {
    return date;
}

string DataEntry::getCountry() {
    return country;
}

int DataEntry::getCases() {
    return c_cases;
}

int DataEntry::getDeaths() {
    return c_deaths;
}

void DataEntry::setDate(string newDate) {
    date = newDate;
}

void DataEntry::setCountry(string newCountry) {
    country = newCountry;
}

void DataEntry::setCases(int newCases) {
    c_cases = newCases;
}

void DataEntry::setDeaths(int newDeaths) {
    c_deaths = newDeaths;
}

class CovidDB
{
public:
    CovidDB();
    DataEntry get(string country);
    int hash(string country);
    //int checkItem(string country, int index);
    void makeHash(vector<cvsdata> hashtable);
    void removeItem(string country);
    void DBinit(vector<cvsdata> dataInit);
    void display();

    bool compareDate(vector<string>date_1, vector<string> date_2);
    bool addItem(DataEntry item);

private:
    int tableSize = 17;
    vector<vector<DataEntry>> database;
};

CovidDB::CovidDB()
{
    database = vector<vector<DataEntry>>(tableSize);
}

DataEntry CovidDB::get(string country) {
    int getHash = hash(country);
    vector<DataEntry>& bucket = database[getHash];
    for (int i = 0; i < bucket.size(); i++)
    {
        if (bucket[i].getCountry() == country)
        {
            return bucket[i];
        }
    }
    DataEntry null;
    return null;
}

int CovidDB::hash(string country) {
    int sum = 0;
    for (int i = 0; i < country.length(); i++)
    {
        sum += (i + 1) * int(country[i]);
    }
    return sum % tableSize;
}

//int CovidDB::checkItem(string country, int index)
//{
//    for (int i = 0; i < database[index].size(); i++)
//    {
//        if (database[index][i].getCountry() == country)
//        {
//            return i;
//        }
//    }
//    return -1;
//}

void CovidDB::makeHash(vector<cvsdata> hashtable)
{
    for (int i = 0; i < hashtable.size(); i++)
    {
        DataEntry item;
    }
}

void CovidDB::removeItem(string country)
{
    int deleteHash = hash(country);
    vector<DataEntry>& bucket = database[deleteHash];
    for (int i = 0; i < bucket.size(); i++)
    {
        if (bucket[i].getCountry() == country)
        {
            bucket.erase(bucket.begin() + i);
            return;
        }
    }
    return;
}

void CovidDB::DBinit(vector<cvsdata> dataInit)
{
    for (int i = 0; i < dataInit.size(); i++)
    {
        DataEntry addi;
        addi.setCases(dataInit[i].getCases());
        addi.setCountry(dataInit[i].getCountry());
        addi.setDate(dataInit[i].getDate());
        addi.setDeaths(dataInit[i].getDeaths());

        addItem(addi);
    }
}

void CovidDB::display()
{
    cout << "WHO COVID Data" << endl;
    for (int i = 0; i < database.size(); i++)
    {
        vector<DataEntry>& bucket = database[i];
        for (int j = 0; j < bucket.size(); j++)
        {
            cout << bucket[j].getDate() << "\t" << bucket[j].getCountry() << "\t" << bucket[j].getCases() << "\t" << bucket[j].getDeaths() << endl;
        }
    }
}

bool CovidDB::compareDate(vector<string> date_1, vector<string> date_2)
{
    if (stoi(date_1[2]) > stoi(date_2[2]))
    {
        return true;
    }
    else if (stoi(date_1[2]) < stoi(date_2[2]))
    {
        return false;
    }
    else
    {
        if (stoi(date_1[0]) > stoi(date_2[0])) {
            return true;
        }
        else if (stoi(date_1[0]) < stoi(date_2[0])) {
            return false;
        }
        else {
            if (stoi(date_1[1]) > stoi(date_2[1])) {
                return true;
            }
            else {
                return false;
            }
        }
    }
}

bool CovidDB::addItem(DataEntry item)
{
    int hashItem = hash(item.getCountry());
    vector<DataEntry>& bucket = database[hashItem];

    for (int i = 0; i < bucket.size(); i++) {
        if (bucket[i].getCountry() == item.getCountry()) {
            // Merge the countries
            // Compare date
            vector<string> entry_date = split(item.getDate(), '/');
            vector<string> current_date = split(bucket[i].getDate(), '/');
            if (compareDate(entry_date, current_date) == true) {
                bucket[i].setDate(item.getDate());
                bucket[i].setCases(bucket[i].getCases() + item.getCases());
                bucket[i].setDeaths(bucket[i].getDeaths() + item.getDeaths());
                return true;
            }
            else {
                return false;
            }
        }
    }
    bucket.push_back(item);
    return true;
}

void menu() {
    cout << "WHO COVID DATA" << endl;
    cout << "1. Construct hash table.\n" << "2. Create new entry. \n" << "3. Get entry. \n" << "4. Delete entry.\n" << "5. Show hash table.\n" << "0. Exit.\n";
    cout << "Please enter your input: " << endl;
}





int main()
{
    CovidDB CovidDB;
    int input;
    bool init = false;
    vector<cvsdata> recordList;

    // open the file
    ifstream file("WHO-COVID-Data.csv");

    if (file.is_open()) {

        cout << "Open File Success" << endl;

        string line;

        while (getline(file, line)) {

            // split the content in each line
            vector<string> result = split(line, ',');

            // wrap up all the data in the string to a cvsdata type and push it to the vector
            cvsdata oneitem = cvsdata(result[0], result[1], std::stoi(result[2]), std::stoi(result[3]));
            recordList.push_back(oneitem);
        }
        cout << "There are " << recordList.size() << " records in total." << endl;
        cout << "==============================================" << endl;

    }
    else {
        cout << "Open file failed" << endl;
    }

    menu();
    cin >> input;
    while (input != 0)
    {
        if (input == 1)
        {
            cout << "Processing, please wait...\n";
            if (!init)
            {
                CovidDB.DBinit(recordList);
                cout << "Database initialization complete." << endl;
                init = true;
            }
            else
            {
                cout << "Database initialization already completed." << endl;
            }
        }
        else if (input == 2)
        {
            DataEntry newEntry;
            string date;
            string country;
            int cases;
            int deaths;

            cout << "Enter entry date:\n";
            cin >> date;
            newEntry.setDate(date);
            cout << endl;

            cout << "Enter entry country:\n";
            cin >> country;
            newEntry.setCountry(country);
            cout << endl;

            cout << "Enter entry cases:\n";
            cin >> cases;
            newEntry.setCases(cases);
            cout << endl;

            cout << "Enter entry deaths:\n";
            cin >> deaths;
            newEntry.setDeaths(deaths);
            cout << endl;

            CovidDB.addItem(newEntry);
        }
        else if (input == 3)
        {
            string country;
            cout << "Which country's data would you like to retreive: \n";
            cin >> country;
            DataEntry entry = CovidDB.get(country);
            cout << entry.getDate() << "\t-\t" << entry.getCountry() << "\t-\t" << entry.getCases() << "\t-\t" << entry.getDeaths() << endl;
        }
        else if (input == 4)
        {
            string country;
            cout << "Which country's data would you like to DELETE: \n";
            cin >> country;
            CovidDB.removeItem(country);
            cout << "Entry has been removed." << endl;
        }
        else if (input == 5)
        {
            CovidDB.display();
        }
        menu();
        cin >> input;
    }
    return 0;
}