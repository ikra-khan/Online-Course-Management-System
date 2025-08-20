#include "user.h"
#include "course.h"
int main() {
    User student("Lamia",337,"Lamia","01787788313","lamiarahmanorthy32@gmail.com");
    cout<<student.getName();
    cout << student.getId() << endl;
    cout<<student.getPassword();
    cout<<student.getPhoneNumber();
    cout<<student.getEmail();
    Course course1("Skill increment","112hr",6000,"10% for first month purchase","Academic","7 days",.9,"Got it");
     cout<<course1.getTitle();
    cout<<course1.getDuration();
    cout<<course1.getPrice();
    cout<<course1.getOffer();
    cout<<course1.getTopic();
    cout<<course1.getOutline();
    cout<<course1.getCertificate();
    return 0;

}
