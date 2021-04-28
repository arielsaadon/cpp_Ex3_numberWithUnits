#pragma once
#include <fstream>
#include <map>

namespace ariel{
    class NumberWithUnits{
        static std::map<std::string,std::map<std::string ,double >> units;
        double number = 0;
        std::string unit;

        static void addConversions(const std::string& firstUnit, const std::string& secondUnit,double num);
        static double convert(const std::string& firstUnit, const std::string& secondUnit,double val);
        static bool CheckUnit(const std::string& unit);
    public:
        NumberWithUnits() = default;
        NumberWithUnits(double number,const std::string& unit){
            if(units.count(unit) == 0){
                throw std::invalid_argument{"The unit is not on the list"};
            }else{
                this->number = number;
                this->unit = unit;
            }
        }
        static void read_units(std::ifstream& file);

        NumberWithUnits operator+(NumberWithUnits const& other);
        NumberWithUnits& operator+=(const NumberWithUnits& other);
        NumberWithUnits operator+() const;

        NumberWithUnits operator-(NumberWithUnits const& other);
        NumberWithUnits& operator-=(const NumberWithUnits& other);
        NumberWithUnits operator-() const;

        bool operator>(const NumberWithUnits& other) const;
        bool operator>=(const NumberWithUnits& other) const;
        bool operator<(const NumberWithUnits& other) const;
        bool operator<=(const NumberWithUnits& other) const;
        bool operator==(const NumberWithUnits& other) const;
        bool operator!=(const NumberWithUnits& other) const;

        NumberWithUnits& operator++();
        NumberWithUnits operator++(int);
        NumberWithUnits& operator--();
        NumberWithUnits operator--(int);

        friend NumberWithUnits operator*(double Num,NumberWithUnits const& Number);
        friend NumberWithUnits operator*(NumberWithUnits const& Number,double Num);

        friend std::ostream& operator<< (std::ostream& os, const NumberWithUnits& Number);
        friend std::istream& operator>> (std::istream& input, NumberWithUnits& Number);
    };
}