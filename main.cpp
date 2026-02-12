#include "LinkoNetwork.h"
#include <iostream>
#include <sstream>

using namespace std;

void printMenu() {
    cout << "\n=============== Linko Social Network ===============" << endl;
    cout << "Available commands:" << endl;
    cout << "  register <username> <password> \"<display name>\" \"<bio>\"" << endl;
    cout << "  login <username> <password>" << endl;
    cout << "  logout" << endl;
    cout << "  post \"<content>\"" << endl;
    cout << "  feed [time|like]" << endl;
    cout << "  follow <username>" << endl;
    cout << "  unfollow <username>" << endl;
    cout << "  like <post_id>" << endl;
    cout << "  comment <post_id> \"<text>\"" << endl;
    cout << "  delete <post_id>" << endl;
    cout << "  profile \"<display name>\" \"<bio>\"" << endl;
    cout << "  show [username]" << endl;
    cout << "  save" << endl;
    cout << "  exit" << endl;
    cout << "  help" << endl;
    cout << "==================================================\n" << endl;
}
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
