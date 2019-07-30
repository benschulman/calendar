#include <string>

class Event {
private:
    std::string filepath;
    unsigned int day;
    unsigned int month;
    unsigned int year;
    unsigned int id;

    std::string name;
    
public:
    Event(const unsigned int day, const unsigned month, const unsigned int year) {

    }
};