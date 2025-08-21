//
// Created by Asus on 8/21/2025.
//

#ifndef UNTITLED_CONTENT_H
#define UNTITLED_CONTENT_H
#include <string>
#include <vector>
using namespace std;
class content {
    vector<string>lecture;
    vector<string>video;
    vector<string>note;
    vector<string>slide;
    vector<string>books;
    vector<string>assignment;
public:
   void addlecture(const string &lec) {
    }
   void addvideo(const string &vid) {
    }
    void addnote(const string &n) {
   }
    void addslide(const string &s) {
   }
 void addbook(const string &b) {
   }
 void addassignment(const string &a) {
   }
    void display() {
    for (auto lec: lecture){}
    for (auto vid: video){}
    for (auto note: note){}
    for (auto slide: slide){}
    for (auto books: books){}
    for (auto assignment: assignment){}

   }
};
#endif //UNTITLED_CONTENT_H