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
