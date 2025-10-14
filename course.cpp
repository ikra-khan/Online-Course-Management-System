#include <iostream>
#include <string>
using namespace std;

// ======================
// Course Class Definition
// ======================
class Course {
private:
    string title;
    string duration;
    int price;
    string offer;
    string topic;
    string outline;
    string progress;
    bool certificate;

public:
    // Constructor
    Course(const string &title = "", const string &duration = "", int price = 0,
           const string &offer = "", const string &topic = "",
           const string &outline = "", const string &progress = "", bool certificate = false)
        : title(title), duration(duration), price(price), offer(offer),
          topic(topic), outline(outline), progress(progress), certificate(certificate) {}

    // Setters and Getters
    void setTitle(const string &t) { title = t; }
    string getTitle() const { return title; }

    void setDuration(const string &d) { duration = d; }
    string getDuration() const { return duration; }

    void setPrice(int p) { price = p; }
    int getPrice() const { return price; }

    void setOffer(const string &o) { offer = o; }
    string getOffer() const { return offer; }

    void setTopic(const string &t) { topic = t; }
    string getTopic() const { return topic; }

    void setOutline(const string &out) { outline = out; }
    string getOutline() const { return outline; }

    void setProgress(const string &pr) { progress = pr; }
    string getProgress() const { return progress; }

    void setCertificate(bool cer) { certificate = cer; }
    bool getCertificate() const { return certificate; }

    // Display Function
    void display() const {
        cout << "\n========== Course Information ==========\n";
        cout << "Title: " << title << endl;
        cout << "Duration: " << duration << endl;
        cout << "Price: $" << price << endl;
        cout << "Offer: " << offer << endl;
        cout << "Topic: " << topic << endl;
        cout << "Outline: " << outline << endl;
        cout << "Progress: " << progress << endl;
        cout << "Certificate: " << (certificate ? "Available ✅" : "Not Available ❌") << endl;
        cout << "========================================\n";
    }
};

// ======================
// Main Function
// ======================
int main() {
    Course course;
    int choice;
    string input;
    int num;
    bool cer;

    do {
        cout << "\n========= Course Menu =========\n";
        cout << "1. Set Course Title\n";
        cout << "2. Set Duration\n";
        cout << "3. Set Price\n";
        cout << "4. Set Offer\n";
        cout << "5. Set Topic\n";
        cout << "6. Set Outline\n";
        cout << "7. Set Progress\n";
        cout << "8. Set Certificate Availability\n";
        cout << "9. Display Course Information\n";
        cout << "0. Exit\n";
        cout << "--------------------------------\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // clear newline

        switch (choice) {
            case 1:
                cout << "Enter course title: ";
                getline(cin, input);
                course.setTitle(input);
                break;
            case 2:
                cout << "Enter duration: ";
                getline(cin, input);
                course.setDuration(input);
                break;
            case 3:
                cout << "Enter price: ";
                cin >> num;
                course.setPrice(num);
                cin.ignore();
                break;
            case 4:
                cout << "Enter offer details: ";
                getline(cin, input);
                course.setOffer(input);
                break;
            case 5:
                cout << "Enter topic: ";
                getline(cin, input);
                course.setTopic(input);
                break;
            case 6:
                cout << "Enter course outline: ";
                getline(cin, input);
                course.setOutline(input);
                break;
            case 7:
                cout << "Enter progress details: ";
                getline(cin, input);
                course.setProgress(input);
                break;
            case 8:
                cout << "Is certificate available? (1 = Yes, 0 = No): ";
                cin >> cer;
                course.setCertificate(cer);
                cin.ignore();
                break;
            case 9:
                course.display();
                break;
            case 0:
                cout << "Exiting program...\n";
                break;
            default:
                cout << "Invalid choice. Try again!\n";
        }

    } while (choice != 0);

    return 0;
}
