//
// Created by Asus on 8/21/2025.
//
#include<bits/stdc++.h>
#include "content.h"
using namespace std;
void  addlecture(const string &lec) {
    lecture.push_back(lec);
}
void addvideo(const string &vid) {
    video.push_back(vid);
}
void addnote(const string &n) {
    note.push_back(n);
}
void addslide(const string &s) {
    slide.push_back(s);
}
void addbook(const string &b) {
    books.push_back(b);
}
void addassignment(const string &a) {
    assignment.push_back(a);
}
void display() {
    for (auto lec: lecture) {
        cout<<lec<<" "<<endl;
    }
    for (auto vid: video) {
        cout<<vid<<" "<<endl;
    }
    for (auto note: note) {
        cout<<note<<" "<<endl;
    }
    for (auto slide: slide) {
        cout<<slide<<" "<<endl;
    }
    for (auto books: books) {
        cout<<books<<" "<<endl;
    }
    for (auto assignment: assignment) {
        cout<<assignment<<" "<<endl;
    }
}