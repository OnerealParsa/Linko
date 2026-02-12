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

public:
    LinkoNetwork() {
        currentUser = "";
        nextPostId = 1;
        loadData();
    }
    
    bool isLoggedIn() const { return currentUser != ""; }
    string getCurrentUser() const { return currentUser; }

    bool registerUser(const string& un, const string& pw, const string& dn, const string& b) {
        if (findUser(un) != NULL) return false;
        users.push_back(User(un, pw, dn, b));
        saveData();
        return true;
    }

    bool login(const string& un, const string& pw) {
        User* user = findUser(un);
        if (user != NULL && user->checkPassword(pw)) {
            currentUser = un;
            cout << "Login successful. Welcome, " << user->getDisplayName() << "!" << endl;
            return true;
        }
        cout << "Login failed. Invalid username or password." << endl;
        return false;
    }

    void logout() {
        if (isLoggedIn()) {
            cout << "Logged out successfully." << endl;
            currentUser = "";
        }
    }

    void createPost(const string& content) {
        if (!isLoggedIn()) {
            cout << "Please login first." << endl;
            return;
        }
        
        if (content == "") {
            cout << "Post content cannot be empty." << endl;
            return;
        }
        
        posts.push_back(Post(nextPostId++, currentUser, content));
        cout << "Post created successfully. Post ID: " << (nextPostId - 1) << endl;
        saveData();
    }

    bool follow(const string& target) {
        if (!isLoggedIn()) {
            cout << "Please login first." << endl;
            return false;
        }
        
        if (currentUser == target) {
            cout << "You cannot follow yourself." << endl;
            return false;
        }
        
        User* targetUser = findUser(target);
        if (targetUser == NULL) {
            cout << "User '" << target << "' not found." << endl;
            return false;
        }
        
        User* current = findUser(currentUser);
        
        if (current->isFollowing(target)) {
            cout << "You are already following '" << target << "'." << endl;
            return false;
        }
        
        current->addFollowing(target);
        targetUser->addFollower(currentUser);
        
        cout << "You are now following '" << target << "'." << endl;
        saveData();
        return true;
    }

    bool unfollow(const string& target) {
        if (!isLoggedIn()) {
            cout << "Please login first." << endl;
            return false;
        }
        
        if (currentUser == target) {
            cout << "You cannot unfollow yourself." << endl;
            return false;
        }
 
        User* targetUser = findUser(target);
        if (targetUser == NULL) {
            cout << "User '" << target << "' not found." << endl;
            return false;
        }
        
        User* current = findUser(currentUser);
        
        if (!current->isFollowing(target)) {
            cout << "You are not following '" << target << "'." << endl;
            return false;
        }
        
        current->removeFollowing(target);
        targetUser->removeFollower(currentUser);
        
        cout << "You have unfollowed '" << target << "'." << endl;
        saveData();
        return true;
    }

    void showFeed(int sortMode = 0) {
        if (!isLoggedIn()) {
            cout << "Please login first." << endl;
            return;
        }
        
        User* current = findUser(currentUser);
        vector<Post> feedPosts;
        
        for (int i = 0; i < (int)posts.size(); i++) {
            string author = posts[i].getAuthor();
            
            if (author == currentUser || (current && current->isFollowing(author))) {
                feedPosts.push_back(posts[i]);
            }
        }
        
        if (sortMode == 1) {
            for (int i = 0; i < (int)feedPosts.size() - 1; i++) {
                for (int j = 0; j < (int)feedPosts.size() - i - 1; j++) {
                    if (feedPosts[j].getDate() < feedPosts[j + 1].getDate()) {
                        swap(feedPosts[j], feedPosts[j + 1]);
                    }
                }
            }
        }
        else if (sortMode == 2) {
            for (int i = 0; i < (int)feedPosts.size() - 1; i++) {
                for (int j = 0; j < (int)feedPosts.size() - i - 1; j++) {
                    if (feedPosts[j].getLikes() < feedPosts[j + 1].getLikes()) {
                        swap(feedPosts[j], feedPosts[j + 1]);
                    }
                }
            }
        }
        
        if (feedPosts.empty()) {
            cout << "No posts to show. Follow some users!" << endl;
            return;
        }
        
        cout << "\n=============== YOUR FEED ===============" << endl;
        
        for (int i = 0; i < (int)feedPosts.size(); i++) {
            const Post& post = feedPosts[i];
            const User* author = findUser(post.getAuthor());
            
            cout << "[" << post.getId() << "] ";
            cout << post.getAuthor();
            if (author != NULL) {
                cout << " (" << author->getDisplayName() << ")";
            }
            cout << ":" << endl;
            cout << "\"" << post.getContent() << "\"" << endl;
            cout << "Date: " << post.getDate().toString() << endl;
            cout << "Likes: " << post.getLikes();
            if (post.hasLiked(currentUser)) {
                cout << " (you liked this)";
            }
            cout << " | Comments: " << post.getCommentsCount() << endl;
            
            if (post.getCommentsCount() > 0) {
                cout << "Comments:" << endl;
                vector<Comment> comments = post.getComments();
                for (int j = 0; j < (int)comments.size(); j++) {
                    cout << "  - " << comments[j].getAuthor() << ": \"" 
                         << comments[j].getText() << "\" (" 
                         << comments[j].getDate().toString() << ")" << endl;
                }
            }
            
            cout << "----------------------------------------" << endl;
        }
    }

    void likePost(int id) {
        if (!isLoggedIn()) {
            cout << "Please login first." << endl;
            return;
        }
        
        Post* post = findPost(id);
        if (post == NULL) {
            cout << "Post with ID " << id << " not found." << endl;
            return;
        }
        
        post->addLike(currentUser);
        cout << "Post " << id << " liked successfully." << endl;
        saveData();
    }
