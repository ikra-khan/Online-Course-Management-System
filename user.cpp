#include "user.h"
#include <bits/stdc++.h>
using namespace std;

User::User(const string &n, const int idNo, const string &pass, const long long int phone, const string &emailNo) {
    name = n;
    Id = idNo;
    password = pass;
    phoneNumber = phone;
    email = emailNo;
}

void User::setName(const string &n) {
    name = n;
}
void User::setId(const int idNo) {
    Id = idNo;
}
void User::setPassword(const string &pass) {
    password = pass;
}
void User::setPhoneNumber(const string &phone) {
    phoneNumber = phone;
}
void User::setEmail(const string &emailNo) {
    email = emailNo;
}

string User::getName() {
    return name;
}
int User::getId() const {
    return Id;
}
string User::getPassword() {
    return password;
}
string User::getPhoneNumber() const {
    return phoneNumber;
}
string User::getEmail() {
    return email;
}
