
//          Copyright Yamavol 2022 - 2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef DECIMAL3_H
#define DECIMAL3_H
#endif 

#include <cstdint>
#include <limits>
#include <stdexcept>

namespace Decimal3Params {

    /// Maximum long value that can be stored
    constexpr int64_t LongMax = std::numeric_limits<int64_t>::max();

    /// Minimum long value that can be stored
    constexpr int64_t LongMin = std::numeric_limits<int64_t>::min() + 1;

    /// Error representation
    constexpr int64_t ErrorValue = std::numeric_limits<int64_t>::min();

    /// Maximum long value that can be given.
    constexpr int64_t MaxValue = std::numeric_limits<int64_t>::max() / 1000;

    /// Maximum double value that can be given
    constexpr double  MaxValueD = static_cast<double>(MaxValue - 1);

    /// Maximum double value that can be given with correct integer
    constexpr double  MaxSafeNumD = 9007199254740991.0;

    /// Maximum double value that can be given with correct integer, in long
    constexpr int64_t MaxSafeNum = static_cast<int64_t>(MaxSafeNumD);

    /// Maximum double value that can be given, with 3 digit precisions.
    constexpr double  MaxAccurateNumD = MaxSafeNumD / 1000.0;

    /// Maximum double value that can be given, with 3 digit precisions.
    constexpr double  MaxRoundableAccurateNumD = MaxSafeNumD / 10000.0;

    /// Maximum long value which is safe to convert to double
    constexpr int64_t MaxAccurateNum = static_cast<int64_t>(MaxSafeNumD) / 1000;
}

class Decimal3 {
    int64_t _value;
public:
    Decimal3();
    /// @brief initialize with internal value. Use Decimal3::from() instead.
    Decimal3(int64_t x);
    Decimal3(const Decimal3& copy);
    
    /// @brief returns internal value stored
    int64_t value() const;

    /// @brief returns true if value is errorneous
    bool    error() const;

    /// @brief returns value in double.
    double  to_double() const;

    /// @brief returns value in long
    int64_t to_long() const;

    /// @brief returns value in type specified. value is wrapped in case of OutOfRange.
    int32_t to_int() const;
    int16_t to_short() const;
    uint8_t to_uchar() const;
    int8_t  to_char() const;

    Decimal3& operator=(const Decimal3& copy);

    Decimal3 operator+(const Decimal3& x) const;
    Decimal3 operator-(const Decimal3& x) const;
    Decimal3 operator*(const Decimal3& x) const;
    
    Decimal3 operator+(double x) const;
    Decimal3 operator-(double x) const;
    Decimal3 operator*(double x) const;
    Decimal3 operator/(double x) const;

    Decimal3& operator+=(const Decimal3& x);
    Decimal3& operator-=(const Decimal3& x);
    Decimal3& operator*=(const Decimal3& x);
    Decimal3& operator/=(const Decimal3& x);

    static const int64_t ErrorValue = Decimal3Params::ErrorValue;

    static Decimal3 from(int32_t x);
    static Decimal3 from(uint32_t x);
    static Decimal3 from(int64_t x);
    static Decimal3 from(double x);
    static Decimal3 from(const char* text);
    static Decimal3 from_internal(int64_t x);

    static int64_t safe_add(int64_t a, int64_t b);
    static int64_t safe_subtract(int64_t a, int64_t b);
    static int64_t safe_multiply(int64_t a, int64_t b);
    static int64_t safe_multiply(int64_t a, double b);
    static int64_t safe_divide(int64_t a,  double b);
    static int64_t safe_double_to_internal_long(double x);
    static int64_t parse_string_to_internal_long(const char* text);
};

Decimal3 Decimal3::from(int32_t x) {
    return Decimal3((int64_t)x * 1000);
}

Decimal3 Decimal3::from(uint32_t x) {
    return Decimal3((int64_t)x * 1000);
}

Decimal3 Decimal3::from(int64_t x) {
    if (x < -Decimal3Params::MaxValue || x > Decimal3Params::MaxValue) {
        return Decimal3(ErrorValue);
    }
    return Decimal3(x * 1000);
}

Decimal3 Decimal3::from(double x) {
    return Decimal3(safe_double_to_internal_long(x));
}

Decimal3 Decimal3::from(const char* text) {
    return Decimal3(parse_string_to_internal_long(text));
}

Decimal3 Decimal3::from_internal(int64_t x) {
    return Decimal3(x);
}

int64_t Decimal3::safe_double_to_internal_long(double x) {
    if (!isfinite(x))
        return ErrorValue;
        
    const double absx = std::abs(x);
    if (absx > Decimal3Params::MaxValueD) {
        return ErrorValue;
    }
    if (absx <= Decimal3Params::MaxRoundableAccurateNumD) {
        int64_t tmp = static_cast<int64_t>(absx * 10000.0);
        if (tmp % 10 >= 5)
            tmp = (tmp + 10) / 10;
        else
            tmp = tmp / 10;
        
        return x >= 0 ? tmp : -tmp;
    }
    if (absx <= Decimal3Params::MaxAccurateNumD) {
        return static_cast<int64_t>(x * 1000.0);
    }
    if (absx > Decimal3Params::MaxAccurateNumD) {
        // accept precision loss and convert
        if (absx > Decimal3Params::MaxSafeNumD / 10.0)
            return static_cast<int64_t>(x) * 1000;
        if (absx > Decimal3Params::MaxSafeNumD / 100.0)
            return static_cast<int64_t>(x * 10.0) * 100;
        else
            return static_cast<int64_t>(x * 100) * 10;
    }
    return ErrorValue;
}

int64_t Decimal3::parse_string_to_internal_long(const char* text) {
    if (text == nullptr)
        return ErrorValue;
    if (strlen(text) == 0)
        return 0LL;
    
    char* next = nullptr;
    int64_t integer = strtoll(text, &next, 10);

    if (integer < -Decimal3Params::MaxValue || integer > Decimal3Params::MaxValue)
        return ErrorValue;

    if (*next == '.') {
        char buf[5] = "0000";
        int x = 0;
        for (next = next + 1, x = 0; next != '\0' && x < 4; next++, x++) {
            char c = *next;
            if (c >= '0' && c <= '9')
                buf[x] = c;
            else 
                break;
        }
        int32_t decimal = (buf[0] - '0') * 1000
                        + (buf[1] - '0') * 100
                        + (buf[2] - '0') * 10
                        + (buf[3] - '0');

        if (decimal % 10 >= 5)
            decimal = (decimal + 10) / 10;
        else
            decimal = decimal / 10;

        if (integer == -Decimal3Params::MaxValue || integer == Decimal3Params::MaxValue) {
            constexpr int max_decimal = Decimal3Params::LongMax - Decimal3Params::MaxValue * 1000;
            if (decimal < -max_decimal || decimal > max_decimal) {
                return ErrorValue;
            } 
        }

        int sign = integer >= 0 ? 1 : -1;
        return integer * 1000 + decimal * sign;
    }
    else {
        return integer * 1000;
    }
}


Decimal3::Decimal3() {
    _value = 0LL;
}

Decimal3::Decimal3(int64_t x) {
    _value = x;
}

Decimal3::Decimal3(const Decimal3& copy) {
    _value = copy._value;
}

int64_t Decimal3::value() const {
    return _value;
}

bool Decimal3::error() const {
    return _value == Decimal3::ErrorValue;
}

uint8_t Decimal3::to_uchar() const {
    int64_t x = _value / 1000;
    return static_cast<uint8_t>(x);
}

int8_t  Decimal3::to_char() const {
    int64_t x = _value / 1000;
    return static_cast<int8_t >(x);
}

int16_t Decimal3::to_short() const {
    int64_t x = _value / 1000;
    return static_cast<int16_t>(x);
}

int32_t Decimal3::to_int() const {
    int64_t x = _value / 1000;
    return static_cast<int32_t>(x);
}

int64_t Decimal3::to_long() const {
    return _value / 1000; 
}

double  Decimal3::to_double() const {
    if (_value > Decimal3Params::MaxSafeNum) {} //
    if (_value > Decimal3Params::MaxAccurateNum) {
        // result may be imprecise
    }
    return static_cast<double >(_value) / 1000; 
}

Decimal3& Decimal3::operator=(const Decimal3& copy) {
    _value = copy._value;
    return *this;
}

Decimal3 Decimal3::operator+(const Decimal3& other) const {
    return Decimal3(_value) += other;
}

Decimal3 Decimal3::operator-(const Decimal3& other) const {
    return Decimal3(_value) -= other;
}

Decimal3 Decimal3::operator*(const Decimal3& other) const {
    return Decimal3(safe_multiply(_value, other._value));
}

Decimal3 Decimal3::operator+(double x) const {
    return Decimal3(_value) += Decimal3::from(x);
}

Decimal3 Decimal3::operator-(double x) const {
    return Decimal3(_value) -= Decimal3::from(x);
}

Decimal3 Decimal3::operator*(double x) const {
    return Decimal3(safe_multiply(_value, x));
}

Decimal3 Decimal3::operator/(double x) const {
    return Decimal3(safe_divide(_value, x));
}

Decimal3& Decimal3::operator+=(const Decimal3& other) {
    _value = safe_add(_value, other._value);
    return *this;
}
Decimal3& Decimal3::operator-=(const Decimal3& other) {
    _value = safe_subtract(_value, other._value);
    return *this;
}




int64_t Decimal3::safe_add(int64_t a, int64_t b) {
    bool a_positive = a >= 0;
    bool b_positive = b >= 0;
    if (a_positive != b_positive) {
        // never overflows
        return a + b;
    } 
    else if (a_positive) { 
        // both positive pattern
        if (LLONG_MAX - a < b)
            return ErrorValue;
        else
            return a + b;
    }
    else { 
        // both negative pattern
        if (a == LLONG_MIN || b == LLONG_MIN)
            return ErrorValue;
        else
            return a + b;
    }
}

int64_t Decimal3::safe_subtract(int64_t a, int64_t b) {
    bool a_positive = a >= 0;
    bool b_positive = b >= 0;
    if (a_positive == b_positive) {
        // never overflows (negative overflow results correct answer)
        return a - b;
    } 
    else if (a_positive) { // a is positive, b is negative
        if (b == LLONG_MIN)
            return ErrorValue;
        else if (LLONG_MAX - a < -b)
            return ErrorValue;
        else
            return a - b;
    }
    else { // a is negative, b is positive
        if (a == LLONG_MIN)
            return ErrorValue;
        else if (LLONG_MIN - a < b)
            return ErrorValue;
        else
            return a - b;
    }
}

int64_t Decimal3::safe_multiply(int64_t a, int64_t b) {
    auto c = a * b;
    if (c == 0LL)
        return 0LL;
    else if (a > 0 && c / a != b)
        return ErrorValue;
    else if (c % 1000 >= 500)
        return c / 1000 + 1;
    else
        return c / 1000;
}

int64_t Decimal3::safe_multiply(int64_t a, double b) {
    if (!isfinite(b))
        return ErrorValue;

    double c = a * b;
    if (c > Decimal3Params::LongMax) {
        return ErrorValue;
    }
    if (c > Decimal3Params::MaxSafeNumD) {
        // result is inaccurate (integer part)
        return ErrorValue;
    }
    return static_cast<int64_t>(c);
}

int64_t Decimal3::safe_divide(int64_t a, double b) {
    if (!isfinite(b))
        throw std::invalid_argument("argument is not finite");

    double c = a / b;

    if (isinf(c))
        return ErrorValue;
    if (c > Decimal3Params::MaxSafeNumD) {
        // result is inaccurate (integer part)
        return ErrorValue;
    }
    if (c > Decimal3Params::MaxAccurateNumD) {
        // result is inaccurate (decimal part)
        return ErrorValue;
    }
    return static_cast<int64_t>(c);
}
