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
#include <getopt.h>
#include <cstring>

#include "plan.h"
#include "cal.h"

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
 * A function to print out the usage message for the executable file
 */
void printUsage() {
	std::cout << "Usage: plan <optional-arguments>" << std::endl;
	std::cout << "\t-a, --add [Event]:\tAdd Event to calendar" << std::endl;
	std::cout << "\t-t, --today:\tDisplay all events for today" << std::endl;
	std::cout << "\t-w, --week:\tDisplay all events for this week" << std::endl;
	std::cout << "\t-h, --help:\tShow this help page" << std::endl;
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
 * Function will run on start up.
 */
void startUp() {
	// Check for .planrc folder in $HOME 
	// Read settings
	// make ~/.plan directory if it does not exist
	char home[100] = "";
	std::strcat(home, std::getenv("HOME"));
	std::strcat(home, "/.plan");
	mkdir(home, S_IRWXU);
}

/* 
 * Function for processing the command line arguments
 */
void processArgs(int argc, char **argv) {
	const char* const shortOpts = "a:h:t:w";
	const option longOpts[] = {
		{"add", no_argument, nullptr, 'a'},
		{"help", no_argument, nullptr, 'h'},
		{"today", no_argument, nullptr, 't'},
		{"week", no_argument, nullptr, 'w'},
		{0}
	};
	
	while(true) {
		const auto opt = getopt_long(argc, argv, shortOpts, longOpts, nullptr);
		if(opt == -1)
			break;
		switch (opt)
		{
		case 'a':
			// TODO: Set add flag to add events
			break;
		case 'h':
			printUsage();
			exit(0);
		case 't':
			//TODO: Show today's events 
			break;
		case 'w':
			//TODO: Show week's events
			break;
		case '?':
		default:
			printUsage();
			exit(1);
		}
	}
}

/*
 * The main function of this program
 */
int main(int argc, char **argv) {
	// Parse command line arguments
	startUp();
	processArgs(argc, argv);
	printCalendar(10, 2019);
}
