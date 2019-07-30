#ifndef _CAL_H

#include <string>
#define _CAL_H

int dayNumber(int day, int month, int year);
std::string getNameOfMonth(int month);
int getNumDays(int month, int year);
void printCalendar(int month, int year);

#endif