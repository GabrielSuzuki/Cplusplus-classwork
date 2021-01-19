#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>

using namespace std;

class UserLogin {
private:
    
    unordered_map< string, string > table;
    

public:
    UserLogin(); // default constructor

    void readIn(const string& filename); // add data from file to hash table

    size_t numberOfUsers(); // return the number of users

    string passWordCheck(const string& userName); // returns the passWord of the given user

    size_t wordBucketIdMethod1(const string& userName); // return the bucket number where the given user is located

    size_t wordBucketIdMethod2(const string& userName); // another way to find the bucket number where the given user is located

    bool validateUser(const string& userName); // look up given user

    bool authenticateUser(const string& userName, const string& passWord);// authenticate given user and password
       
};

UserLogin::UserLogin() {
}

// COMPLETE THE FOLLOWING FUNCTIONS

void UserLogin::readIn(const string& filename) {
//read in data, and fill the table
//file has userName and passWord on each line 
//userName is the key and the passWord is the hashtable
    ifstream myfile;
    string userName;
    string passWord;
    myfile.open(filename);
    while (!myfile.eof())
    {
        myfile >> userName;
        myfile >> passWord;
        table.insert(pair<string, string>(userName, passWord));
    }
    myfile.close();
  
}

size_t UserLogin::numberOfUsers() {

    // returns the numbers of users
    return table.size();
}

string UserLogin::passWordCheck(const string& userName) {

    // return passWord of the userName
    //return "Non-existent User" if it does not exist
    unordered_map< string, string >::const_iterator got = table.find(userName);
    if (got == table.end()) {
        return "Non-existent User";
    }
    else {
        return got->second;
    }

}

size_t UserLogin::wordBucketIdMethod1(const string& userName) {

    // return bucket number
    unordered_map< string, string >::const_iterator got;
    for (got = table.begin(); got != table.end(); ++got)
    {
        if (userName == got->first)
        {
            return table.bucket(got->first);
        }
        }

}

size_t UserLogin::wordBucketIdMethod2(const string& userName) {
    
    // return bucket number
    unsigned n = table.bucket_count();
    for (unsigned i = 0; i < n; i++) {
        for (auto it = table.begin(i); it != table.end(i); it++)
            if (userName == it->first) {
                return i;
            }
    }
    return 0;
    
}

bool UserLogin::validateUser(const string& userName) {
    
    // return true if the given userName exists in  the table, otherwise false
    unordered_map< string, string >::const_iterator got = table.find(userName);
    if (got == table.end()) {
        return false;
    }
    else {
        return true;
    }
     
}

bool UserLogin::authenticateUser(const string& userName, const string& passWord) { 
    
    // return true if given userName, passWord combination exists in the table
    unsigned n = table.bucket_count();

    for (unsigned i = 0; i < n; i++) {
        for (auto it = table.begin(i); it != table.end(i); it++)
            if (userName == it->first && passWord == it->second) {
                return true;
            }
            
    }
    return false;
 }