#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <vector>
#include <memory>
#include <istream>
#include <map>

// Simple user-made template for optional points
template <typename T1, typename T2>
struct SimplePair {
    T1 first;
    T2 second;
    SimplePair() = default;
    SimplePair(const T1 &a, const T2 &b) : first(a), second(b) {}
};

// Segment polymorphic hierarchy
class Segment {
protected:
    std::string title;
    int durationMinutes;
public:
    Segment(const std::string &t = "", int d = 0);
    virtual ~Segment();
    virtual void display() const;
    virtual std::string serialize() const;
    static std::unique_ptr<Segment> deserialize(const std::string &line);
};

class VideoSegment : public Segment {
    std::string videoUrl;
public:
    VideoSegment(const std::string &t = "", int d = 0, const std::string &url = "");
    void display() const override;
    std::string serialize() const override;
};

class QuizSegment : public Segment {
    int questions;
public:
    QuizSegment(const std::string &t = "", int d = 0, int q = 0);
    void display() const override;
    std::string serialize() const override;
};

// Course class
class Course {
    std::string id;
    std::string title;
    std::string duration;
    int price;
    std::string offer;
    std::string topic;
    std::string outline;
    std::string progress;
    bool certificate;
    std::vector<std::unique_ptr<Segment>> segments;
public:
    Course(const std::string &id_ = "", const std::string &title_ = "", const std::string &duration_ = "",
           int price_ = 0, const std::string &offer_ = "", const std::string &topic_ = "",
           const std::string &outline_ = "", const std::string &progress_ = "", bool certificate_ = false);

    // non-copyable (unique_ptr), but movable
    Course(const Course &other) = delete;
    Course& operator=(const Course &other) = delete;
    Course(Course &&) = default;
    Course& operator=(Course &&) = default;

    // setters/getters
    void setId(const std::string &i); std::string getId() const;
    void setTitle(const std::string &t); std::string getTitle() const;
    void setDurationStr(const std::string &d); std::string getDurationStr() const;
    void setPrice(int p); int getPrice() const;
    void setOffer(const std::string &o); std::string getOffer() const;
    void setTopic(const std::string &t); std::string getTopic() const;
    void setOutline(const std::string &o); std::string getOutline() const;
    void setProgress(const std::string &p); std::string getProgress() const;
    void setCertificate(bool c); bool hasCertificate() const;

    // segments
    void addSegment(std::unique_ptr<Segment> seg);
    void display() const;

    // serialization
    std::string serialize() const;
    static Course deserialize(std::istream &in);
};

// CourseManager + friend function
class CourseManager {
    std::map<std::string, Course> courses;
public:
    CourseManager() = default;
    ~CourseManager() = default;
    void addCourse(Course &&c);
    CourseManager& operator+=(Course &&c); // operator overloading (optional)
    bool hasCourse(const std::string &id) const;
    Course* getCoursePtr(const std::string &id);
    void displayAll() const;
    bool saveToFile(const std::string &filename) const;
    bool loadFromFile(const std::string &filename);
    friend void printSummary(const CourseManager &mgr);
};

void printSummary(const CourseManager &mgr);

#endif // COURSE_H
