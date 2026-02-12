#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <string>
#include <ctime>
#include <sstream>

using namespace std;

class Date {
private:
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;

public:
    Date() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        year = 1900 + ltm->tm_year;
        month = 1 + ltm->tm_mon;
        day = ltm->tm_mday;
        hour = ltm->tm_hour;
        minute = ltm->tm_min;
        second = ltm->tm_sec;
    }
    
    Date(int y, int m, int d) {
        year = y;
        month = m;
        day = d;
        hour = 0;
        minute = 0;
        second = 0;
    }
    
    string toString() const {
        string result = "";
        result += to_string(year) + "/";
        if (month < 10) result += "0";
        result += to_string(month) + "/";
        if (day < 10) result += "0";
        result += to_string(day) + " ";
        if (hour < 10) result += "0";
        result += to_string(hour) + ":";
        if (minute < 10) result += "0";
        result += to_string(minute) + ":";
        if (second < 10) result += "0";
        result += to_string(second);
        return result;
    }
    
    string serialize() const {
        return to_string(year) + " " + to_string(month) + " " + to_string(day) + " " +
               to_string(hour) + " " + to_string(minute) + " " + to_string(second);
    }
    
    static Date deserialize(const string& str) {
        int y, m, d, h, min, s;
        sscanf(str.c_str(), "%d %d %d %d %d %d", &y, &m, &d, &h, &min, &s);
        Date date(y, m, d);
        date.hour = h;
        date.minute = min;
        date.second = s;
        return date;
    }
    
    bool operator>(const Date& other) const {
        if (year != other.year) return year > other.year;
        if (month != other.month) return month > other.month;
        if (day != other.day) return day > other.day;
        if (hour != other.hour) return hour > other.hour;
        if (minute != other.minute) return minute > other.minute;
        return second > other.second;
    }

    bool operator<(const Date& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        if (day != other.day) return day < other.day;
        if (hour != other.hour) return hour < other.hour;
        if (minute != other.minute) return minute < other.minute;
        return second < other.second;
    }
};

string readQuotedString(stringstream& ss) {
    string result;
    char c;
    ss >> ws >> c;
    
    if (c == '"') {
        while (ss.get(c) && c != '"') {
            result += c;
        }
    } else {
        ss.putback(c);
        ss >> result;
    }
    
    return result;
}

#endif
