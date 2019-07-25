#include <string>
#include <iostream>
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