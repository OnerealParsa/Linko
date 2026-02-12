#ifndef LINKONETWORK_H
#define LINKONETWORK_H

#include "Models.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

class LinkoNetwork {
private:
    vector<User> users;
    vector<Post> posts;
    string currentUser;
    int nextPostId;

    User* findUser(const string& username) {
        for (int i = 0; i < (int)users.size(); i++) {
            if (users[i].getUsername() == username) {
                return &users[i];
            }
        }
        return NULL;
    }
    
    const User* findUser(const string& username) const {
        for (int i = 0; i < (int)users.size(); i++) {
            if (users[i].getUsername() == username) {
                return &users[i];
            }
        }
        return NULL;
    }
    
    Post* findPost(int id) {
        for (int i = 0; i < (int)posts.size(); i++) {
            if (posts[i].getId() == id) {
                return &posts[i];
            }
        }
        return NULL;
    }

    int getUserPostCount(const string& username) const {
        int count = 0;
        for (int i = 0; i < (int)posts.size(); i++) {
            if (posts[i].getAuthor() == username) {
                count++;
            }
        }
        return count;
    }
