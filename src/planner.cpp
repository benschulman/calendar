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
#include <time.h>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>
#include <vector>


struct Event {
	time_t start;
	time_t end;
	
	int id;
	std::string title;
	std::string description;
};

std::vector<Event> events;

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
	std::cout << "\t-e <day-optional> <month-optional> <year-optional>: ";
	std::cout << "displays all events on month/day/year" << std::endl;
	std::cout << "\t-a \"title\" \"description\" <day> <month> <year> <start-time> <end-time>";
	std::cout << ": adds an event with the given title and description" << std::endl;
	std::cout << "\t-r <day> <month> <year> <id>: removes the event with the";
	std::cout << " specified day/month/year and id" << std::endl;
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
	printf("%d\t%s\n%s\n%d:%02d - %d:%02d\n", e.id, e.title.c_str(), e.description.c_str(),
			start_h, start_m, end_h, end_m);
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
		if(std::string(argv[i]) == "-h"){
			printUsage();
			exit(0);
		}
		else if(std::string(argv[i]) == "-a") {
			// events/day_month_year.events
			if(argc < i+7){
				printUsage();
				exit(1);
			}
			
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

			time_t start_t = mktime(&start);
			time_t end_t = mktime(&end);

			int id = 0;

			Event e = {start_t, end_t, id, title, desc};

			printEvent(e);
		}
		else if(std::string(argv[i]) == "-e") {
			if(events.empty()){
				std::cout << "No Events!" << std::endl;
			}
			//TODO: add functionality

			exit(0);
		} 
		// Pick your own month to display
		else if(std::string(argv[i]) == "-d") {
			if(argc <= i+2) {
				printUsage();
				exit(1);
			}
			else {
				// TODO: add catch
				month = atoi(argv[++i]);
				year  = atoi(argv[++i]);
			}
		}
	}	

	printCalendar(month, year);
}
