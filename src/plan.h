#ifndef PLAN_H
	struct Event;
	bool compareEvents(Event e1, Event e2);
	int dayNumber(int day, int month, int year);
	std::string getNameOfMonth(int month);
	int getNumDays(int month, int year);
	void printCalendar(int month, int year);
	void printUsage();
	void printEvent(const Event& e);
	void printEvents(const std::vector<Event>& events);
	void parseEvents(const std::string& filepath, std::vector<Event>* events);
	void removeEvent(const std::string& filepath, const std::string& title, std::vector<Event>* events);
	std::string getFilepath(const Event& e);
	std::string getFilepath(int day, int month, int year);
	std::vector<Event>addEvent(Event& e);
	void writeEvent(std::ofstream& out, const Event& e);
	void overwriteFile(const std::string& filepath, const std::vector<Event>& events);
#endif
