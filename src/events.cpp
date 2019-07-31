#include <string>
#include <ctime>
#include <vector>

class Event {
private:
    tm *time;
    std::string name;
    bool completed;
    
public:
    Event(const unsigned int &day, const unsigned &month,
    const unsigned int &year, const std::string &name, const bool &comp) {
        this->name = name;
        (this->time)->tm_mday = day;
        (this->time)->tm_mon = month+1;
        (this->time)->tm_year = year-1900;
        this->completed = comp;
    }
    
};
