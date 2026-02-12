#ifndef MODELS_H
#define MODELS_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "Date.h"

using namespace std;

class Comment {
private:
    string text;
    string author;
    Date commentDate;

public:
    Comment() {}
    
    Comment(const string& t, const string& a) {
        text = t;
        author = a;
        commentDate = Date();
    }
    
    string getText() const { return text; }
    string getAuthor() const { return author; }
    Date getDate() const { return commentDate; }
    
    string serialize() const {
        string safeText = text;
        for (int i = 0; i < (int)safeText.length(); i++) {
            if (safeText[i] == '|') safeText[i] = 0x7F;
            if (safeText[i] == ',') safeText[i] = 0x7E;
        }
        return safeText + "|" + author + "|" + commentDate.serialize();
    }
    
    static Comment deserialize(const string& str) {
        Comment c;
        int pos1 = -1, pos2 = -1;
        
        for (int i = 0; i < (int)str.length(); i++) {
            if (str[i] == '|') {
                if (pos1 == -1) pos1 = i;
                else if (pos2 == -1) pos2 = i;
            }
        }
        
        if (pos1 == -1 || pos2 == -1) return Comment();
        
        c.text = str.substr(0, pos1);
        for (int i = 0; i < (int)c.text.length(); i++) {
            if (c.text[i] == 0x7F) c.text[i] = '|';
            if (c.text[i] == 0x7E) c.text[i] = ',';
        }
        c.author = str.substr(pos1 + 1, pos2 - pos1 - 1);
        c.commentDate = Date::deserialize(str.substr(pos2 + 1));
        
        return c;
    }
};
class Post {
private:
    int id;
    string author;
    string content;
    int likes;
    Date postDate;
    vector<Comment> comments;
    vector<string> likedBy;

public:
    Post() {
        id = 0;
        likes = 0;
        author = "";
        content = "";
        postDate = Date();
    }
    
    Post(int _id, const string& _author, const string& _content) {
        id = _id;
        author = _author;
        content = _content;
        likes = 0;
        postDate = Date();
    }
    
    int getId() const { return id; }
    string getAuthor() const { return author; }
    string getContent() const { return content; }
    int getLikes() const { return (int)likedBy.size(); }
    Date getDate() const { return postDate; }
    int getCommentsCount() const { return (int)comments.size(); }
    vector<Comment> getComments() const { return comments; }
    
    bool hasLiked(const string& username) const {
        for (int i = 0; i < (int)likedBy.size(); i++) {
            if (likedBy[i] == username) return true;
        }
        return false;
    }
    
    void addLike(const string& username) {
        for (int i = 0; i < (int)likedBy.size(); i++) {
            if (likedBy[i] == username) return;
        }
        likedBy.push_back(username);
        likes = (int)likedBy.size();
    }
    
    void addComment(const Comment& c) {
        comments.push_back(c);
    }
    
    string serialize() const {
        stringstream ss;
        
        ss << id << "|" << author << "|";
        
        string safeContent = content;
        for (int i = 0; i < (int)safeContent.length(); i++) {
            if (safeContent[i] == '|') safeContent[i] = 0x7F;
            if (safeContent[i] == ',') safeContent[i] = 0x7E;
        }
        ss << safeContent << "|";
        
        ss << getLikes() << "|" << postDate.serialize() << "|";
        
        ss << likedBy.size() << "|";
        for (int i = 0; i < (int)likedBy.size(); i++) {
            ss << likedBy[i];
            if (i < (int)likedBy.size() - 1) ss << ",";
        }
        ss << "|";
        
        ss << comments.size() << "|";
        for (int i = 0; i < (int)comments.size(); i++) {
            ss << comments[i].serialize();
            if (i < (int)comments.size() - 1) ss << "|";
        }
        ss << "|";
        
        return ss.str();
    }
    
    static Post deserialize(const string& str) {
        Post p;
        vector<string> parts;
        string current = "";
        
        for (int i = 0; i < (int)str.length(); i++) {
            if (str[i] == '|') {
                parts.push_back(current);
                current = "";
            } else {
                current += str[i];
            }
        }
        if (!current.empty()) parts.push_back(current);
        
        if (parts.size() < 7) {
            cout << "Error: Not enough parts in post data" << endl;
            return Post();
        }
        
        try {
            int index = 0;
            
            p.id = stoi(parts[index++]);
            p.author = parts[index++];
            p.content = parts[index++];
            
            for (int i = 0; i < (int)p.content.length(); i++) {
                if (p.content[i] == 0x7F) p.content[i] = '|';
                if (p.content[i] == 0x7E) p.content[i] = ',';
            }
            
            p.likes = stoi(parts[index++]);
            p.postDate = Date::deserialize(parts[index++]);
            
           
            if (index < (int)parts.size()) {
                int likedCount = stoi(parts[index++]);
                if (index < (int)parts.size()) {
                    string likedStr = parts[index++];
                    stringstream likedStream(likedStr);
                    string username;
                    for (int i = 0; i < likedCount; i++) {
                        if (getline(likedStream, username, ',')) {
                            if (!username.empty()) p.likedBy.push_back(username);
                        }
                    }
                }
            }
            
           
            if (index < (int)parts.size()) {
                int commentCount = stoi(parts[index++]);
                
                for (int i = 0; i < commentCount; i++) {
                   
                    if (index + 2 < (int)parts.size()) {
                        
                        string commentStr = parts[index] + "|" + 
                                           parts[index + 1] + "|" + 
                                           parts[index + 2];
                        
                        Comment c = Comment::deserialize(commentStr);
                        if (c.getAuthor() != "") {
                            p.comments.push_back(c);
                        }
                        
                        index += 3; 
                    } else {
                        break;
                    }
                }
            }
            
        } catch (const exception& e) {
            cout << "Exception in Post deserialize: " << e.what() << endl;
            return Post();
        }
        
        return p;
    }
};

class User {
private:
    string username;
    string password;
    string displayName;
    string bio;
    vector<string> following;
    vector<string> followers;

public:
    User() {
        username = "";
        password = "";
        displayName = "";
        bio = "";
    }
    
    User(const string& un, const string& pw, const string& dn, const string& b) {
        username = un;
        password = pw;
        displayName = dn;
        bio = b;
    }
    
    string getUsername() const { return username; }
    string getPassword() const { return password; }
    string getDisplayName() const { return displayName; }
    string getBio() const { return bio; }
    
    void setBio(const string& newBio) { bio = newBio; }
    void setDisplayName(const string& newName) { displayName = newName; }
    
    int getFollowingCount() const { return (int)following.size(); }
    int getFollowersCount() const { return (int)followers.size(); }
    vector<string> getFollowing() const { return following; }
    vector<string> getFollowers() const { return followers; }
    
    bool isFollowing(const string& target) const {
        for (int i = 0; i < (int)following.size(); i++) {
            if (following[i] == target) return true;
        }
        return false;
    }
    
    void addFollowing(const string& target) {
        for (int i = 0; i < (int)following.size(); i++) {
            if (following[i] == target) return;
        }
        following.push_back(target);
    }
    
    void removeFollowing(const string& target) {
        for (int i = 0; i < (int)following.size(); i++) {
            if (following[i] == target) {
                following.erase(following.begin() + i);
                return;
            }
        }
    }
    
    void addFollower(const string& follower) {
        for (int i = 0; i < (int)followers.size(); i++) {
            if (followers[i] == follower) return;
        }
        followers.push_back(follower);
    }
    
    void removeFollower(const string& follower) {
        for (int i = 0; i < (int)followers.size(); i++) {
            if (followers[i] == follower) {
                followers.erase(followers.begin() + i);
                return;
            }
        }
    }
    
    bool checkPassword(const string& pw) const { return password == pw; }
    
    string serialize() const {
        stringstream ss;
        
        ss << username << "|" << password << "|" << displayName << "|";
        
        string safeBio = bio;
        for (int i = 0; i < (int)safeBio.length(); i++) {
            if (safeBio[i] == '|') safeBio[i] = 0x7F;
            if (safeBio[i] == ',') safeBio[i] = 0x7E;
        }
        ss << safeBio << "|";
        
        ss << following.size() << "|";
        for (int i = 0; i < (int)following.size(); i++) {
            ss << following[i];
            if (i < (int)following.size() - 1) ss << ",";
        }
        ss << "|";
        
        ss << followers.size() << "|";
        for (int i = 0; i < (int)followers.size(); i++) {
            ss << followers[i];
            if (i < (int)followers.size() - 1) ss << ",";
        }
        ss << "|";
        
        return ss.str();
    }
    
    static User deserialize(const string& str) {
        User u;
        vector<string> parts;
        string current = "";
        
        for (int i = 0; i < (int)str.length(); i++) {
            if (str[i] == '|') {
                parts.push_back(current);
                current = "";
            } else {
                current += str[i];
            }
        }
        if (!current.empty()) parts.push_back(current);
        
        if (parts.size() < 5) return User();
        
        try {
            int index = 0;
            
            u.username = parts[index++];
            u.password = parts[index++];
            u.displayName = parts[index++];
            u.bio = parts[index++];
            
            for (int i = 0; i < (int)u.bio.length(); i++) {
                if (u.bio[i] == 0x7F) u.bio[i] = '|';
                if (u.bio[i] == 0x7E) u.bio[i] = ',';
            }
            
           
            if (index < (int)parts.size()) {
                int followingCount = stoi(parts[index++]);
                if (index < (int)parts.size()) {
                    string followingStr = parts[index++];
                    stringstream followingStream(followingStr);
                    string username;
                    for (int i = 0; i < followingCount; i++) {
                        if (getline(followingStream, username, ',')) {
                            if (!username.empty()) u.following.push_back(username);
                        }
                    }
                }
            }
            
           
            if (index < (int)parts.size()) {
                int followersCount = stoi(parts[index++]);
                if (index < (int)parts.size()) {
                    string followersStr = parts[index++];
                    stringstream followersStream(followersStr);
                    string username;
                    for (int i = 0; i < followersCount; i++) {
                        if (getline(followersStream, username, ',')) {
                            if (!username.empty()) u.followers.push_back(username);
                        }
                    }
                }
            }
            
        } catch (const exception& e) {
            return User();
        }
        
        return u;
    }
};

#endif
