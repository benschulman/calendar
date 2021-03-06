///////////////////////////////////////////////////////////////////////////////
// File Name: planner.cpp
//
// Author: Ben Schulman and Jacob Latts
// CS email: bgschulman@wisc.edu and latts@wisc.edu
//
// Description: Source File for the final project of CS368. This project is a
// 				command-line calendar application. It allows you to display a 
// 				calendar via the command line as well as add events to days.
///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm>

#include "plan.h"

struct Event {
	time_t start;
	time_t end;
	
	std::string title;
	std::string description;
};

/*
 * Compares two events for STL sort algorithm
 */
bool compareEvents(Event e1, Event e2) {
	return e1.start < e2.start;
}

/*
 * A Function that returns the index of the day of the date- day/month/year
 *
 * Index     Day
 * 0         Sunday
 * 1         Monday
 * 2         Tuesday
 * 3         Wednesday
 * 4         Thursday
 * 5         Friday
 * 6         Saturday
 *
 * Function uses Sakamoto's algorithm to determine the day of the week. 
 * Source: 
*/
int dayNumber(int day, int month, int year) {

    static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1,
                       4, 6, 2, 4 };
    year -= month < 3;
    return ( year + year/4 - year/100 +
             year/400 + t[month-1] + day) % 7;
}

/*
 * A Function that returns a the name of the month given the number
 *
 * Number	Name
 * 1		January
 * 2		February
 * 3		March
 * 4		April
 * 5		May
 * 6		June
 * 7		July
 * 8		August
 * 9		September
 * 10		October
 * 11		November
 * 12		December
 */
std::string getNameOfMonth(int month) {
	if(month == 1) return "January";
	else if(month == 2) return "February";
	else if(month == 3) return "March";
	else if(month == 4) return "April";
	else if(month == 5) return "May";
	else if(month == 6) return "June";
	else if(month == 7) return "July";
	else if(month == 8) return "August";
	else if(month == 9) return "September";
	else if(month == 10) return "October";
	else if(month == 11) return "November";
	else if(month == 12) return "December";
	else {perror("Invalid Month"); exit(1);}
}

/*
 * A function that returns the number of days given a month (number) and year
 *
 * Month	Days
 * 1		31
 * 2		28/29 If the year is divisible by 400 or if the year is divisible
 * 				  by 4 and not 100 then it is a leap year otherwise it is not
 * 3		31
 * 4		30
 * 5		31
 * 6		30
 * 7		31
 * 8		31
 * 9		30
 * 10		31
 * 11		30
 * 12		31
 *
 * If an invalid month is entered then 0 is returned
 */
int getNumDays(int month, int year) {
	if(month == 1) return 31;
	else if(month == 2) {
		if(year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
			return 29;
		else
			return 28;
	}
	else if(month == 3) return 31;
	else if(month == 4) return 30;
	else if(month == 5) return 31;
	else if(month == 6) return 30;
	else if(month == 7) return 31;
	else if(month == 8) return 31;
	else if(month == 9) return 30;
	else if(month == 10) return 31;
	else if(month == 11) return 30;
	else if(month == 12) return 31;
	else return 0;
	std::cout << "Not a valid month!" << std::endl;
}

/*
 * A function that prints the calendar for a given month and year
 */
void printCalendar(int month, int year) {
	// Print the header of the calendar with month and year centered
	std::string str_m = getNameOfMonth(month) + " - " + std::to_string(year);
	int remaining = 26 - str_m.length();
	int spacesRight = remaining / 2;
	int spacesLeft = remaining - spacesRight;
	printf("%*s%s%*s\n", spacesLeft, "", str_m.c_str(), spacesRight, "");
	
	// Print days of the week
	std::cout << "Su  Mo  Tu  We  Th  Fr  Sa" << std::endl;
	// find the current day of the week
	int currDay = dayNumber(1, month, year); 
	// print out spaces until that day
	for(int i = 0; i < currDay; i++){
		std::cout << "    ";
	}
    
	// get number of days in that month
	int numDays = getNumDays(month, year);
	// print out number and newline on saturday
	for(int j = 1; j <= numDays; j++){
		if(currDay == 6) {
			currDay = 0;
			printf("%2d\n", j);
		}
		else {
			currDay++;
			printf("%2d  ", j);
		}
	}

	std::cout << std::endl;
}

/*
 * A function to print out the usage message for the executable file
 */
void printUsage() {
	std::cout << "Usage: plan <optional-arguments>" << std::endl;
	std::cout << "\t-d <month> <year>: displays the calendar ";
	std::cout << "for the given month and year" << std::endl;
	std::cout << "\t-e <day> <month> <year>: ";
	std::cout << "displays all events on month/day/year" << std::endl;
	std::cout << "\t-a \"title\" \"description\" <day> <month> <year> <start-time> <end-time>";
	std::cout << ": adds an event with the given title and description. <start-time> and "; 
	std::cout << "<end-time> should be written in the form 4:00 or 13:00. Only supports military time" << std::endl;
	std::cout << "\t-r \"title\" <day> <month> <year>: removes the first event with the";
	std::cout << " specified day/month/year and title" << std::endl;
	std::cout << "\t-h: displays this help page" << std::endl;
}

/*
 * This function prints the contents of an event.
 */
void printEvent(const Event& e) {
	// Convert time_t to hours and minutes
	int start_h = std::localtime(&(e.start))->tm_hour % 12;
	int end_h = std::localtime(&(e.end))->tm_hour % 12;
	int start_m = std::localtime(&(e.start))->tm_min; 
	int end_m = std::localtime(&(e.end))->tm_min; 

	// make 0 o'clock be 12 o'clock
	start_h = start_h == 0 ? 12 : start_h;
	end_h = end_h == 0 ? 12 : end_h;
	// print formatted
	printf("%s\n%s\n%d:%02d - %d:%02d\n", e.title.c_str(), e.description.c_str(),
			start_h, start_m, end_h, end_m);
}

/*
 * This function prints the Event structs in the events vector
 */
void printEvents(const std::vector<Event>& events){
	for(auto it = events.begin(); it != events.end(); it++){
		printEvent(*it);
		std::cout << std::endl;
	}
}

/*
 * A Function that parses a string of the form "hour:min" and stores hour in
 * hour and minutes in min. Returns 0 on success. 1 on failure.
 */
int parseTime(const std::string& str, int *hour, int *min) {
	*hour = atoi(str.substr(0, str.find(":")).c_str());
	*min = atoi(str.substr(str.find(":")+1, str.length()).c_str());
	return 0;
}

/*
 * This function takes in a filepath and a pointer to a vector and
 * populates the vector with initialized event variables from the events
 * file.
 *
 * Event files are of the form:
 * Title1
 * Description1
 * start time 1 (as time_t)
 * end time 1 (as time_t)
 *
 * Title2
 *  ...
 *
 *
 */
void parseEvents(const std::string& filepath, std::vector<Event>* events){
	// open streams and declare variables
	std::ifstream in;
	in.open(filepath);
	std::string line;
	
	// if file does not exist
	if(!in.good())
		return;

	time_t start;
	time_t end;
	std::string title;
	std::string description;
	
	getline(in, line);

	// parse file
	while(!in.eof()){
		for(int i = 0; i < 5; i++){
			// title
			if(i == 0){
				title = line;
			}
			// description
			else if(i == 1){
				getline(in, line);
				description = line;
			}
			// start time
			else if(i == 2){
				getline(in, line);
				start = atoi(line.c_str());
			}
			// end time
			else if(i == 3){
				getline(in, line);
				end = atoi(line.c_str());
			}
			else {
				getline(in, line);
				getline(in, line);
			}
		}
		Event e = {start, end, title, description};
		(*events).push_back(e);
	}

	in.close();
}

/*
 * This function takes in a filepath, a title and a poitner to a vecotr of
 * events. The function removes the first event with the given title from
 * the events vector and then overwrites the file with the updated events
 * vector.
 */
void removeEvent(const std::string& filepath, const std::string& title, 
		std::vector<Event>* events){
	// loop through events finds one with matching title and removes
	for(auto it = events->begin(); it != events->end(); it++){
		if(title == it->title){
			events->erase(it);
			break;
		}
	}
	
	// overwirteFile with updated events
	overwriteFile(filepath, *events);
}

/*
 * This function returns the filepath for the coresponding event
 */
std::string getFilepath(const Event& e){
	// retrieve day, month, year from event
	// used to name evnt file
	struct tm *t = localtime(&e.start);
	int day = t->tm_mday;
	int month = t->tm_mon + 1;
	int year = t->tm_year + 1900;

	// filepath to the .evnt file used for storing all events
	// on that day
	return "events/" + std::to_string(day)+
		std::to_string(month) + std::to_string(year) + ".evnt";
}

/*
 * Ths function returns the filepath for the given date
 */
std::string getFilepath(int day, int month, int year){
	return std::string("events/") 
				+ std::to_string(day)
				+ std::to_string(month) 
				+ std::to_string(year) 
				+ std::string(".evnt");
}

/*
 * This function adds an event to the correct event file and then returns
 * a vector with the events from the corresponding day.
 */
std::vector<Event> addEvent(Event& e) {
	// create directory for events if it does not already exist
	std::ofstream out;
	mkdir("events", S_IRWXU);
	
	std::string filepath = getFilepath(e);

	// create events vector to store events and read in events
	// from file
	std::vector<Event> events;
	parseEvents(filepath, &events);

	// Add event and sort events vector
	events.push_back(e);
	std::sort(events.begin(), events.end(), compareEvents);

	// write events vector to file
	overwriteFile(filepath, events);
	return events;
}

/* 
 * This function writes a single event to the ofstream passed
 * 
 * The event is written in the form:
 *
 * Title
 * Description
 * start time (time_t)
 * end time (time_t)
 *
 * Note: writes an empty line following end time
 */
void writeEvent(std::ofstream& out, const Event& e) {
	out << e.title << std::endl;
	out << e.description << std::endl;
	out << e.start << std::endl;
	out << e.end << std::endl;
	out << std::endl;	
}

/*
 * This function overwrites the file at filepath with the events in the events
 * vector.
 *
 * Format for how events are written is described in documentation for writeEvent
 */
void overwriteFile(const std::string& filepath, const std::vector<Event>& events) {
	std::ofstream out;
	out.open(filepath);
	
	// error with file
	if(out.is_open() == false){
		std::cout << "Error creating events file" << std::endl;
		exit(1);
	}
	
	// for each event write the event to the file
	for(Event e:events){
		writeEvent(out, e);
	}

	out.close();
}

/*
 * Function will run on startup.
 */
void startUp() {
	// Check for .planrc folder in $HOME 
	// Read settings
	// Check for ~/.plan/ directory
	
	// Create if does not exist
	 
}

/*
 * The main function of this program
 */
int main(int argc, char* argv[]) {
	// Get current time and create tm struct

	std::time_t rawtime = time(nullptr);
	struct tm* lt = localtime(&rawtime);
	
	// Current day month and year
	int day = lt->tm_mday;
	int month = lt->tm_mon + 1;
	int year = lt->tm_year + 1900; 

	// Deal with command line args
	for(int i = 1; i < argc; i++) {
		// help arg
		if(std::string(argv[i]) == "-h"){
			printUsage();
			exit(0);
		}
		// add event arg
		else if(std::string(argv[i]) == "-a") {
			// events/day_month_year.events
			if(argc < i+7){
				printUsage();
				exit(1);
			}
			
			//parse command line arguments
			std::string title = argv[i+1];
			std::string desc = argv[i+2];

			day = atoi(argv[i+3]);
			month = atoi(argv[i+4]);
			year = atoi(argv[i+5]);

			// time args should have the form 12:00 or 1:46
			int start_h;
			int start_m;
			parseTime(std::string(argv[i+6]), &start_h, &start_m);

			int end_h;
			int end_m;
			parseTime(std::string(argv[i+7]), &end_h, &end_m);

			// create a tm struct for start and end to convert to time_t
			struct tm start;
			struct tm end;

			start.tm_sec = 0;
			end.tm_sec = 0;
			start.tm_min = start_m;
			start.tm_hour = start_h - 1;
			end.tm_min = end_m;
			end.tm_hour = end_h -1;
			start.tm_mday = day;
			start.tm_mon = month - 1;
			start.tm_year = year - 1900;
			end.tm_mday = day;
			end.tm_mon = month -1;
			end.tm_year = year - 1900;

			// create time_t vars for storing in file
			time_t start_t = mktime(&start);
			time_t end_t = mktime(&end);

			// create event and add it to the proper file
			Event e = {start_t, end_t, title, desc};
			std::vector<Event> events = addEvent(e);
			// print events for that day
			printEvents(events);
		}
		// display events arg
		else if(std::string(argv[i]) == "-e") {
			// check correct number of args
			if(argc - i+1 < 3){
				printUsage();
				exit(1);
			}
			
			// parse args
			day = atoi(argv[i+1]);
			month = atoi(argv[i+2]);
			year = atoi(argv[i+3]);

			mkdir("events", S_IRWXU);
			std::string filepath = getFilepath(day, month, year);

			// display events
			std::vector<Event> events;
			parseEvents(filepath, &events);
			printEvents(events);
		} 
		// Pick your own month to display arg
		else if(std::string(argv[i]) == "-d") {
			// checks for correct number of args
			if(argc <= i+2) {
				printUsage();
				exit(1);
			}
			else {
				// parses args
				month = atoi(argv[++i]);
				year  = atoi(argv[++i]);
			}
		}
		// remove arg
		else if(std::string(argv[i]) == "-r"){
			// check for correct number of args
			if(argc - i + 1 < 4){
				printUsage();
				exit(1);
			}
			else{
				// parse remaining args
				day = atoi(argv[i+2]);
				month = atoi(argv[i+3]);
				year = atoi(argv[i+4]);

				std::string title = argv[i+1];
				mkdir("events", S_IRWXU);
				
				std::string filepath = getFilepath(day, month, year);

				// parse events from file
				std::vector<Event> events;
				parseEvents(filepath, &events);

				// remove correct event
				removeEvent(filepath, title, &events);
				printEvents(events);
			}
		}
	}	

	printCalendar(month, year);
}
