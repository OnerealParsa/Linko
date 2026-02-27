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
                        Post temp = feedPosts[j];
                        feedPosts[j] = feedPosts[j + 1];
                        feedPosts[j + 1] = temp;
                    }
                }
            }
        }
        else if (sortMode == 2) {
            for (int i = 0; i < (int)feedPosts.size() - 1; i++) {
                for (int j = 0; j < (int)feedPosts.size() - i - 1; j++) {
                    if (feedPosts[j].getLikes() < feedPosts[j + 1].getLikes()) {
                        Post temp = feedPosts[j];
                        feedPosts[j] = feedPosts[j + 1];
                        feedPosts[j + 1] = temp;
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

    void addComment(int id, const string& text) {
        if (!isLoggedIn()) {
            cout << "Please login first." << endl;
            return;
        }
        
        Post* post = findPost(id);
        if (post == NULL) {
            cout << "Post with ID " << id << " not found." << endl;
            return;
        }
        
        post->addComment(Comment(text, currentUser));
        cout << "Comment added successfully." << endl;
        saveData();
    }
    
    void deletePost(int id) {
        if (!isLoggedIn()) {
            cout << "Please login first." << endl;
            return;
        }
        
        for (int i = 0; i < (int)posts.size(); i++) {
            if (posts[i].getId() == id) {
                if (posts[i].getAuthor() != currentUser) {
                    cout << "You can only delete your own posts." << endl;
                    return;
                }
                
                posts.erase(posts.begin() + i);
                cout << "Post " << id << " deleted successfully." << endl;
                saveData();
                return;
            }
        }
        
        cout << "Post with ID " << id << " not found." << endl;
    }
    
    void editProfile(const string& newDisplayName, const string& newBio) {
        if (!isLoggedIn()) {
            cout << "Please login first." << endl;
            return;
        }
        
        User* user = findUser(currentUser);
        if (newDisplayName != "") {
            user->setDisplayName(newDisplayName);
        }
        if (newBio != "") {
            user->setBio(newBio);
        }
        
        cout << "Profile updated successfully." << endl;
        saveData();
    }
    
    void showProfile(const string& username = "") {
        string target = (username == "") ? currentUser : username;
        
        if (target == "" && !isLoggedIn()) {
            cout << "Please login first or specify a username." << endl;
            return;
        }
        
        const User* user = findUser(target);
        if (user == NULL) {
            cout << "User not found." << endl;
            return;
        }
        
        cout << "\n=============== PROFILE ===============" << endl;
        cout << "Display Name: " << user->getDisplayName() << " (@" << user->getUsername() << ")" << endl;
        cout << "Bio: " << user->getBio() << endl;
        cout << "Posts: " << getUserPostCount(target) << endl;
        cout << "Following: " << user->getFollowingCount() << " users" << endl;
        cout << "Followers: " << user->getFollowersCount() << " users" << endl;
        
        if (user->getFollowingCount() > 0) {
            cout << "Following list: ";
            vector<string> following = user->getFollowing();
            for (int i = 0; i < (int)following.size(); i++) {
                cout << following[i];
                if (i < (int)following.size() - 1) cout << ", ";
            }
            cout << endl;
        }
        
        if (user->getFollowersCount() > 0) {
            cout << "Followers list: ";
            vector<string> followers = user->getFollowers();
            for (int i = 0; i < (int)followers.size(); i++) {
                cout << followers[i];
                if (i < (int)followers.size() - 1) cout << ", ";
            }
            cout << endl;
        }
        
        cout << "========================================" << endl;
    }

    void saveData() {
        ofstream fout("linko_data.txt");
        if (!fout) {
            cout << "Error: Could not save data." << endl;
            return;
        }
        
        fout << users.size() << endl;
        for (int i = 0; i < (int)users.size(); i++) {
            fout << users[i].serialize() << endl;
        }
        
        fout << posts.size() << endl;
        for (int i = 0; i < (int)posts.size(); i++) {
            fout << posts[i].serialize() << endl;
        }
        
        fout << nextPostId << endl;
        fout.close();
        cout << "Data saved successfully." << endl;
    }

    void loadData() {
        ifstream fin("linko_data.txt");
        if (!fin) {
            nextPostId = 1;
            return;
        }
        
        users.clear();
        posts.clear();
        
        try {
            int userCount = 0;
            string line;
            
            if (!getline(fin, line)) return;
            userCount = stoi(line);
            
            for (int i = 0; i < userCount; i++) {
                if (!getline(fin, line)) break;
                if (!line.empty()) {
                    User u = User::deserialize(line);
                    if (u.getUsername() != "") {
                        users.push_back(u);
                    }
                }
            }
            
            int postCount = 0;
            if (!getline(fin, line)) {
                nextPostId = 1;
                fin.close();
                return;
            }
            postCount = stoi(line);
            
            int maxId = 0;
            for (int i = 0; i < postCount; i++) {
                if (!getline(fin, line)) break;
                if (!line.empty()) {
                    Post p = Post::deserialize(line);
                    if (p.getId() != 0) {
                        posts.push_back(p);
                        if (p.getId() > maxId) maxId = p.getId();
                    }
                }
            }
            
            if (getline(fin, line)) {
                nextPostId = stoi(line);
                if (nextPostId <= maxId) nextPostId = maxId + 1;
            } else {
                nextPostId = maxId + 1;
            }
            
        } catch (const exception& e) {
            cout << "Error loading data: " << e.what() << endl;
            nextPostId = 1;
        }
        
        fin.close();
    }
    
    const vector<Post>& getAllPosts() const { 
        return posts; 
    }
    
    const User* getUserByUsername(const string& username) const { 
        return findUser(username); 
    }
};

#endif
