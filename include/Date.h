#ifndef DATE_H
#define DATE_H

class Date
{
public:
    Date(int y, int m, int d)
    : year(y), month(m), day(d)
    {}

    bool operator< (const Date& other) const {
        return year < other.year ||
               (year == other.year && month < other.month ||
                (month == other.month && day < other.day));
    }

private:
    int year;
    int month;
    int day;
};

#endif // DATE_H
