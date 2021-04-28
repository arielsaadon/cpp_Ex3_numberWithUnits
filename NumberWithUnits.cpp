#include "NumberWithUnits.hpp"
#include <string>
#include <sstream>
#include <map>

using namespace std;
using namespace ariel;

const double conversion = 1.0;
const double TOLERANCE = 0.001;

map<string,map<string ,double >> NumberWithUnits::units;

void NumberWithUnits::addConversions(const string& firstUnit,const string& secondUnit,double num){
    for(auto const& x : units[firstUnit]){
        if((x.first != firstUnit && x.first != secondUnit) || units[x.first].count(secondUnit) == 0){
            double temp = units[x.first][firstUnit];
            units[x.first][secondUnit] = temp*num;
            units[secondUnit][x.first] = conversion/(temp*num);
        }
        for(auto const& j : units[secondUnit]){
            if(units[x.first].count(j.first) == 0) {
                double from = units[secondUnit][j.first];
                double to = units[x.first][secondUnit];
                units[x.first][j.first] = from*to;
                units[j.first][x.first] = conversion/(from*to);
            }
        }
    }
}

void NumberWithUnits::read_units(ifstream& file){
    string line;
    string firstUnit,secondUnit,space;
    double num = 0,num2 = 0;

    while(getline(file,line)){
        istringstream readLine(line);
        readLine >> num >> firstUnit >> space >> num2 >> secondUnit;
        units[firstUnit][firstUnit] = conversion;
        units[secondUnit][secondUnit] = conversion;
        units[firstUnit][secondUnit] = num2;
        units[secondUnit][firstUnit] = conversion/num2;
        addConversions(firstUnit,secondUnit,num2);
    }
}

double NumberWithUnits::convert(const string& firstUnit, const string& secondUnit,double value){
    if(firstUnit == secondUnit){
        return value;
    }
    if(units[firstUnit].count(secondUnit) != 0 || units[secondUnit].count(firstUnit) != 0){
        value = value * units[secondUnit][firstUnit];
    } else{
        throw invalid_argument{"Units do not match - [" +firstUnit +"] cannot be converted to [" +secondUnit +"]" };
    }
    return value;
}

NumberWithUnits NumberWithUnits::operator+(NumberWithUnits const& other){
    double converted = convert(this->unit,other.unit,other.number);
    return NumberWithUnits(this->number+converted, this->unit);;
}

NumberWithUnits& NumberWithUnits::operator+=(const NumberWithUnits& other) {
    double converted = convert(this->unit,other.unit,other.number);
    this->number += converted;
    return *this;
}

NumberWithUnits NumberWithUnits::operator+() const {
    return NumberWithUnits ((+1)*this->number, this->unit);
}

NumberWithUnits NumberWithUnits::operator-(NumberWithUnits const& other){
    double converted = convert(this->unit,other.unit,other.number);
    return NumberWithUnits (this->number-converted, this->unit);
}

NumberWithUnits &NumberWithUnits::operator-=(const NumberWithUnits& other) {
    double converted = convert(this->unit,other.unit,other.number);
    this->number -= converted;
    return *this;
}

NumberWithUnits NumberWithUnits::operator-() const {
    return NumberWithUnits ((-1)*this->number, this->unit);
}

bool NumberWithUnits::operator>(const NumberWithUnits& other) const{
    double converted = convert(this->unit,other.unit,other.number);
    return this->number > converted;
}

bool NumberWithUnits::operator>=(const NumberWithUnits& other) const{
    double converted = convert(this->unit,other.unit,other.number);
    return abs(this->number - converted) <= TOLERANCE || this->number > converted;
}

bool NumberWithUnits::operator<(const NumberWithUnits& other) const{
    double converted = convert(this->unit,other.unit,other.number);
    return this->number < converted;
}

bool NumberWithUnits::operator<=(const NumberWithUnits& other) const{
    double converted = convert(this->unit,other.unit,other.number);
    return abs(this->number - converted) <= TOLERANCE || this->number < converted;
}

bool NumberWithUnits::operator==(const NumberWithUnits& other) const{
    double converted = convert(this->unit,other.unit,other.number);
    return abs(this->number - converted) <= TOLERANCE;
}

bool NumberWithUnits::operator!=(const NumberWithUnits& other) const{
    double converted = convert(this->unit,other.unit,other.number);
    return this->number != converted;
}

NumberWithUnits& NumberWithUnits::operator++() {
    this->number++;
    return *this;
}

NumberWithUnits NumberWithUnits::operator++(int) {
    NumberWithUnits copy = *this;
    number++;
    return copy;
}

NumberWithUnits &NumberWithUnits::operator--() {
    this->number--;
    return *this;
}

NumberWithUnits NumberWithUnits::operator--(int) {
    NumberWithUnits copy = *this;
    number--;
    return copy;
}

NumberWithUnits ariel::operator*(double Num,NumberWithUnits const& Number){
    return NumberWithUnits(Num*Number.number, Number.unit);
}

NumberWithUnits ariel::operator*(NumberWithUnits const& Number,double Num){
    return NumberWithUnits(Number.number*Num, Number.unit);
}


ostream& ariel::operator<<(ostream &os, const NumberWithUnits& Number) {
    os << Number.number << "[" << Number.unit << "]";
    return os;
}

bool NumberWithUnits::CheckUnit(const string& unit){
    if(NumberWithUnits::units.count(unit) == 0){
        throw invalid_argument{"The unit is not on the list" };
    }
    return true;
}

istream& ariel::operator>>(istream& input, NumberWithUnits& Number) {
    double number = 0;
    char ch = 0;
    string unit;

    input >> number;
    input >> ch;
    input >> unit;
    if(unit[unit.size() - 1] == ']' && unit.size() > 1){
        if(NumberWithUnits::CheckUnit(unit.substr(0,unit.size()-1))){
            Number.number = number;
            Number.unit = unit.substr(0,unit.size()-1);
        }
    }else{
        input >> ch;
        if(ch == ']'){
            if(NumberWithUnits::CheckUnit(unit)){
                Number.number = number;
                Number.unit = unit;
            }
        }else{
            throw invalid_argument{"The unit is not on the list"};
        }
    }
    return input;
}