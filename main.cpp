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

int main() {
    LinkoNetwork network;
    string line;
    
    cout << "\nWelcome to Linko Social Network!" << endl;
    cout << "Type 'help' to see available commands.\n" << endl;
    
    printMenu();

    while (true) {
        if (network.isLoggedIn()) {
            cout << "[" << network.getCurrentUser() << "] > ";
        } else {
            cout << "[Guest] > ";
        }
        
        if (!getline(cin, line)) break;
        if (line.empty()) continue;
        
        stringstream ss(line);
        string cmd;
        ss >> cmd;

        if (cmd == "register") {
            string u, p, d, b;
            if (!(ss >> u >> p)) {
                cout << "Error: Invalid format. Usage: register <username> <password> \"<display name>\" \"<bio>\"" << endl;
                continue;
            }
            
            d = readQuotedString(ss);
            b = readQuotedString(ss);
            
            if (d == "") d = u;
            if (b == "") b = "Hello, I'm new to Linko!";
            
            if (network.registerUser(u, p, d, b)) {
                cout << "User '" << u << "' registered successfully!" << endl;
            } else {
                cout << "Error: Username already exists." << endl;
            }
        } 
        else if (cmd == "login") {
            string u, p;
            if (ss >> u >> p) {
                network.login(u, p);
            } else {
                cout << "Error: Invalid format. Usage: login <username> <password>" << endl;
            }
        } 
        else if (cmd == "logout") {
            network.logout();
        }
        else if (cmd == "post") {
            string content = readQuotedString(ss);
            if (content.empty()) {
                cout << "Error: Post content cannot be empty." << endl;
            } else {
                network.createPost(content);
            }
        } 
        else if (cmd == "feed") {
            string sortType;
            int sortMode = 0;
            
            if (ss >> sortType) {
                if (sortType == "time") {
                    sortMode = 1;
                    cout << "Sorting feed by time (newest first)" << endl;
                }
                else if (sortType == "like") {
                    sortMode = 2;
                    cout << "Sorting feed by likes (most first)" << endl;
                }
            }
            
            network.showFeed(sortMode);
        } 
        else if (cmd == "follow") {
            string target;
            if (ss >> target) {
                network.follow(target);
            } else {
                cout << "Error: Invalid format. Usage: follow <username>" << endl;
            }
        }
        else if (cmd == "unfollow") {
            string target;
            if (ss >> target) {
                network.unfollow(target);
            } else {
                cout << "Error: Invalid format. Usage: unfollow <username>" << endl;
            }
        } 
        else if (cmd == "like") {
            int id;
            if (ss >> id) {
                network.likePost(id);
            } else {
                cout << "Error: Invalid format. Usage: like <post_id>" << endl;
            }
        } 
        else if (cmd == "comment") {
            int id;
            string text;
            
            if (ss >> id) {
                text = readQuotedString(ss);
                if (text.empty()) {
                    cout << "Error: Comment text cannot be empty." << endl;
                } else {
                    network.addComment(id, text);
                }
            } else {
                cout << "Error: Invalid format. Usage: comment <post_id> \"<text>\"" << endl;
            }
        }
        else if (cmd == "delete") {
            int id;
            if (ss >> id) {
                network.deletePost(id);
            } else {
                cout << "Error: Invalid format. Usage: delete <post_id>" << endl;
            }
        }
        else if (cmd == "profile") {
            string displayName, bio;
            displayName = readQuotedString(ss);
            bio = readQuotedString(ss);
            
            if (displayName.empty() && bio.empty()) {
                cout << "Error: Please provide at least one field to update." << endl;
            } else {
                network.editProfile(displayName, bio);
            }
        }
        else if (cmd == "show") {
            string username;
            if (ss >> username) {
                network.showProfile(username);
            } else {
                network.showProfile();
            }
        }
        else if (cmd == "save") {
            network.saveData();
        } 
        else if (cmd == "exit") {
            cout << "\nGoodbye! See you soon on Linko." << endl;
            cout << "Data saved automatically." << endl;
            network.saveData();
            break;
        }
        else if (cmd == "help") {
            printMenu();
        }
        else {
            cout << "Unknown command. Type 'help' for available commands." << endl;
        }
    }
    
    return 0;
}
