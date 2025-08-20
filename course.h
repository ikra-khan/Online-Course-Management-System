#ifndef UNTITLED_COURSE_H
#define UNTITLED_COURSE_H

#include <string>
using namespace std;

class Course {
private:
    string title;
    string duration;
    int price;
    string offer;
    string topic;
    string outline;
    string progress;
    bool certificate;   // use bool for true/false

public:
    Course(const string &title, const string &duration, int price, const string &offer,
           const string &topic, const string &outline, const string &progress, bool certificate);

    void setTitle(const string &t);
    string getTitle() const;

    void setDuration(const string &d);
    string getDuration() const;

    void setPrice(int p);
    int getPrice() const;

    void setOffer(const string &o);
    string getOffer() const;

    void setTopic(const string &t);
    string getTopic() const;

    void setOutline(const string &out);
    string getOutline() const;

    void setProgress(const string &pr);
    string getProgress() const;

    void setCertificate(bool cer);
    bool getCertificate() const;
};

#endif
