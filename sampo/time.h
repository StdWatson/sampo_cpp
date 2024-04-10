#pragma once
//см файл time.cpp - пояснени там

#ifndef TIME_H
#define TIME_H

class Time {
private:
    int value;
public:
    explicit Time(int value);

    static Time inf();

    bool is_inf() const;

    int get_val();

    Time operator+(Time& other) const;

    Time operator+(int other) const;

    Time operator-(Time& other) const;

    Time operator-(int other) const;

    Time operator*(Time& other) const;

    Time operator*(int other) const;

    Time operator/(Time& other) const;

    Time operator/(int other) const;

    bool operator<(Time& other) const;

    bool operator<(int other) const;

    bool operator>(Time& other) const;

    bool operator>(int other) const;

    bool operator<=(Time& other) const;

    bool operator<=(int other) const;

    bool operator>=(Time& other) const;

    bool operator>=(int other) const;

    bool operator==(Time& other) const;

    bool operator==(const Time& other) const;

    bool operator==(int other) const;

    bool operator!=(Time& other) const;

    bool operator!=(int other) const;
};


//#define TIME_INF 2000000000
//
////Time::Time(int value) {
////    if (value > TIME_INF) {
////        value = TIME_INF;
////    }
////    else if (value < 0) {
////        value = 0;
////    }
////    this->value = value;
////}
//
//class Time {
//private:
//    int value;
//public:
//    explicit Time(int value) {
//        if (value > TIME_INF) {
//            value = TIME_INF;
//        }
//        else if (value < 0) {
//            value = 0;
//        }
//        this->value = value;
//    }
//
//    static inf() {
//        static Time inf(TIME_INF); // caching the inf instance
//        return inf;
//    }
//
//    bool is_inf() const;
//
//    int get_val();
//
//    Time operator+(Time& other) const;
//
//    Time operator+(int other) const;
//
//    Time operator-(Time& other) const;
//
//    Time operator-(int other) const;
//
//    Time operator*(Time& other) const;
//
//    Time operator*(int other) const;
//
//    Time operator/(Time& other) const;
//
//    Time operator/(int other) const;
//
//    bool operator<(Time& other) const;
//
//    bool operator<(int other) const;
//
//    bool operator>(Time& other) const;
//
//    bool operator>(int other) const;
//
//    bool operator<=(Time& other) const;
//
//    bool operator<=(int other) const;
//
//    bool operator>=(Time& other) const;
//
//    bool operator>=(int other) const;
//
//    bool operator==(Time& other) const;
//
//    bool operator==(int other) const;
//};
//
//Time::Time(int value) {
//    if (value > TIME_INF) {
//        value = TIME_INF;
//    }
//    else if (value < 0) {
//        value = 0;
//    }
//    this->value = value;
//}
//Time Time::inf() {
//    static Time inf(TIME_INF); // caching the inf instance
//    return inf;
//}
//bool Time::is_inf() const {
//    return this->value == TIME_INF;
//}
//
//int Time::get_val() {
//    return value;
//}
//
//// Copy-paste body between functions with different arg type
//// is here to avoid functional call overhead. Time instances
//// should be as fast as possible.
//
//Time Time::operator+(Time& other) const {
//    return Time(this->value + other.value);
//}
//
//Time Time::operator+(int other) const {
//    return Time(this->value + other);
//}
//
//Time Time::operator-(Time& other) const {
//    return Time(this->value - other.value);
//}
//
//Time Time::operator-(int other) const {
//    return Time(this->value - other);
//}
//
//Time Time::operator*(Time& other) const {
//    return Time(this->value * other.value);
//}
//
//Time Time::operator*(int other) const {
//    return Time(this->value * other);
//}
//
//Time Time::operator/(Time& other) const {
//    return Time(this->value / other.value);
//}
//
//Time Time::operator/(int other) const {
//    return Time(this->value / other);
//}
//
//bool Time::operator<(Time& other) const {
//    return this->value < other.value;
//}
//
//bool Time::operator<(int other) const {
//    return this->value < other;
//}
//
//bool Time::operator>(Time& other) const {
//    return this->value > other.value;
//}
//
//bool Time::operator>(int other) const {
//    return this->value > other;
//}
//
//bool Time::operator<=(Time& other) const {
//    return this->value <= other.value;
//}
//
//bool Time::operator<=(int other) const {
//    return this->value <= other;
//}
//
//bool Time::operator>=(Time& other) const {
//    return this->value >= other.value;
//}
//
//bool Time::operator>=(int other) const {
//    return this->value >= other;
//}
//
//bool Time::operator==(Time& other) const {
//    return this->value == other.value;
//}
//
//bool Time::operator==(int other) const {
//    return this->value == other;
//}

#endif //NATIVE_TIME_H