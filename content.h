#ifndef CONTENT_H
#define CONTENT_H

#include <string>
#include <vector>

class Content {
    std::vector<std::string> lectures;
    std::vector<std::string> videos;
    std::vector<std::string> notes;
    std::vector<std::string> slides;
    std::vector<std::string> books;
    std::vector<std::string> assignments;
public:
    Content() = default;
    void addLecture(const std::string &lecture);
    void addVideo(const std::string &video);
    void addNote(const std::string &note);
    void addSlide(const std::string &slide);
    void addBook(const std::string &book);
    void addAssignment(const std::string &assignment);
    void displayAll() const;
    bool saveToFile(const std::string &filename) const;
    bool loadFromFile(const std::string &filename);
};

#endif // CONTENT_H
