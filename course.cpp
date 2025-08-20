#include "course.h"

Course::Course(const string &title, const string &duration, int price, const string &offer,
               const string &topic, const string &outline, const string &progress, bool certificate) {
    this->title = title;
    this->duration = duration;
    this->price = price;
    this->offer = offer;
    this->topic = topic;
    this->outline = outline;
    this->progress = progress;
    this->certificate = certificate;
}
void Course::setTitle(const string &t) {
    title = t;
}
string Course::getTitle() const {
    return title;
}
void Course::setDuration(const string &d) {
    duration = d;
}
string Course::getDuration() const {
    return duration;
}
void Course::setPrice(int p) {
    price = p;
}
int Course::getPrice() const {
    return price;
}
void Course::setOffer(const string &o) {
    offer = o;
}
string Course::getOffer() const {
    return offer;
}
void Course::setTopic(const string &t) {
    topic = t;
}
string Course::getTopic() const {
    return topic;
}
void Course::setOutline(const string &out) {
    outline = out;
}
string Course::getOutline() const {
    return outline;
}
void Course::setProgress(const string &pr) {
    progress = pr;
}
string Course::getProgress() const {
    return progress;
}
void Course::setCertificate(bool cer) {
    certificate = cer;
}
bool Course::getCertificate() const {
    return certificate;
}
