#ifndef UNTITLED_OCMS_H
#define UNTITLED_OCMS_H
#include<bits/stdc++.h>
using namespace std;
class User {
private:
    string name;
    int Id;
    string password;
    string phoneNumber;
    string email;
public:
    User(const string &n, int idNo, const string &pass, string phone, const string &emailNo);

    void setName(const string &n);
    string getName();
    void setId(int idNo);
    int getId() const;
    void setPassword(const string &pass);
    string getPassword();
    void setPhoneNumber(const string &phone);
    string getPhoneNumber() const;
    void setEmail(const string &emailNo);
    string getEmail();
};
#endif //UNTITLED_OCMS_H