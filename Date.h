#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <string>

using namespace std;

struct Date {
    int year;
    int month;
    int day;
    Date(int y = 1402, int m = 1, int d = 1) : year(y), month(m), day(d) {}
    
    string toString() const {
        return to_string(year) + "/" + to_string(month) + "/" + to_string(day);
    }
    
    string serialize() const {
        return to_string(year) + " " + to_string(month) + " " + to_string(day);
    }
    
    static Date deserialize(const string& str) {
        int y, m, d;
        sscanf(str.c_str(), "%d %d %d", &y, &m, &d);
        return Date(y, m, d);
    }
};

#endif
