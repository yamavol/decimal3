#include <iostream>
#include "harness.h"
#include "harness_extended.h"
#include "decimal3.h"

namespace P = Decimal3Params;

template <class T>
static inline Decimal3 d3(T value) {
    return Decimal3::from(value);
}

template <class T>
static inline int64_t d3value_from(T value) {
    return Decimal3::from(value).value();
}

void decimal3_initialize(test_runner* t)
{
    int count = 0;
    const char* test_title = "";

    count = 0;
    test_title = "construction test %d";
    LONG_EQ(t, Decimal3(0).value(), 0LL, test_title, ++count);
    LONG_EQ(t, Decimal3(5).value(), 5LL, test_title, ++count);
    LONG_EQ(t, d3value_from( 1),  1000LL, test_title, ++count);
    LONG_EQ(t, d3value_from(-1), -1000LL, test_title, ++count);
    LONG_EQ(t, d3value_from(0.001), 1LL, test_title, ++count);
    LONG_EQ(t, d3value_from(0.010), 10LL, test_title, ++count);
    LONG_EQ(t, d3value_from(0.100), 100LL, test_title, ++count);

    count = 0;
    test_title = "construction error test %d";
    IS_FALSE(t, Decimal3( LLONG_MAX).error(), test_title, ++count);
    IS_FALSE(t, Decimal3(-LLONG_MAX).error(), test_title, ++count);
    IS_FALSE(t, Decimal3( P::MaxValue).error(), test_title, ++count);
    IS_FALSE(t, Decimal3(-P::MaxValue).error(), test_title, ++count);
    IS_TRUE (t, Decimal3( LLONG_MIN).error(), test_title, ++count);
    IS_TRUE (t, Decimal3(-LLONG_MIN).error(), test_title, ++count);
    
    IS_TRUE (t, Decimal3::from( LLONG_MAX).error(), test_title, ++count);
    IS_TRUE (t, Decimal3::from(-LLONG_MAX).error(), test_title, ++count);
    IS_FALSE(t, Decimal3::from( P::MaxValue).error(), test_title, ++count);
    IS_FALSE(t, Decimal3::from(-P::MaxValue).error(), test_title, ++count);
}

void decimal3_initialize_from_double(test_runner* t)
{
    int count = 0;
    const char* test_title = "";

    count = 0;
    test_title = "construction test %d";
    LONG_EQ(t, d3value_from(0.001), 1LL, test_title, ++count);
    LONG_EQ(t, d3value_from(0.010), 10LL, test_title, ++count);
    LONG_EQ(t, d3value_from(0.100), 100LL, test_title, ++count);

    LONG_EQ(t, d3value_from(0.1110), 111LL, test_title, ++count);
    LONG_EQ(t, d3value_from(0.1115), 112LL, test_title, ++count);

    count = 0;
    test_title = "construct from large double test %d";
    LONG_EQ(t, d3value_from(P::MaxSafeNumD), P::MaxSafeNum * 1000, test_title, ++count);
    LONG_EQ(t, d3value_from(P::MaxSafeNumD + 1.0), (P::MaxSafeNum +1) * 1000, test_title, ++count);
    LONG_EQ(t, d3value_from(P::MaxSafeNumD + 2.0), (P::MaxSafeNum +1) * 1000, test_title, ++count);
    
    LONG_EQ(t, d3value_from(P::MaxSafeNumD / 10.0), P::MaxSafeNum * 100, test_title, ++count);
    LONG_EQ(t, d3value_from(P::MaxSafeNumD / 100.0), P::MaxSafeNum * 10, test_title, ++count);
    LONG_EQ(t, d3value_from(P::MaxSafeNumD / 1000.0), d3value_from(P::MaxAccurateNumD), test_title, ++count);

    IS_TRUE (t, Decimal3::from(nan("")).error(), "initialize by nan");
    IS_TRUE (t, Decimal3::from(INFINITY).error(), "initialize by inf");
    IS_TRUE (t, Decimal3::from( DBL_MAX).error(), "initialize by DBL_MAX");
    IS_TRUE (t, Decimal3::from(-DBL_MAX).error(), "initialize by DBL_MAX");
}

void decimal3_initialize_from_string(test_runner* t)
{
    int count = 0;
    const char* test_title = "string parse test %d";
    LONG_EQ(t, d3value_from("123.456"), 123456LL, test_title, ++count);
    LONG_EQ(t, d3value_from("123.456789"), 123457LL, test_title, ++count);

    count = 0;
    test_title = "string parse boundary test %d";
    LONG_EQ(t, d3value_from("9223372036854775"),     9223372036854775000LL, test_title, ++count);
    LONG_EQ(t, d3value_from("9223372036854775.806"), 9223372036854775806LL, test_title, ++count);
    LONG_EQ(t, d3value_from("9223372036854775.807"), 9223372036854775807LL, test_title, ++count);
    LONG_EQ(t, d3value_from("9223372036854775.808"), Decimal3::ErrorValue,  test_title, ++count);
    LONG_EQ(t, d3value_from("9223372036854776"),     Decimal3::ErrorValue,  test_title, ++count);
    LONG_EQ(t, d3value_from("9999999999999999"),     Decimal3::ErrorValue,  test_title, ++count);

    LONG_EQ(t, d3value_from("-9223372036854775"),     -9223372036854775000LL, test_title, ++count);
    LONG_EQ(t, d3value_from("-9223372036854775.806"), -9223372036854775806LL, test_title, ++count);
    LONG_EQ(t, d3value_from("-9223372036854775.807"), -9223372036854775807LL, test_title, ++count);
    LONG_EQ(t, d3value_from("-9223372036854775.808"), Decimal3::ErrorValue,   test_title, ++count);
    LONG_EQ(t, d3value_from("-9223372036854776"),     Decimal3::ErrorValue,   test_title, ++count);
    
    LONG_EQ(t, d3value_from("9223372036854775"),     9223372036854775000LL, test_title, ++count);
    LONG_EQ(t, d3value_from("9223372036854775.805"), 9223372036854775805LL, test_title, ++count);
}

void decimal3_arithmetic(test_runner* t)
{
    LONG_EQ(t, (d3( 1.111) + d3( 2.222)).value(),  3333LL, "add two positive decimal");
    LONG_EQ(t, (d3(-1.111) + d3(-2.222)).value(), -3333LL, "add two negative decimal");
    LONG_EQ(t, (d3(-1.111) - d3(-2.222)).value(),  1111LL, "subtract two negative decimal");
    LONG_EQ(t, (d3( 1.111) * d3( 2.222)).value(),  2469LL, "add two negative decimal"); // 2.468642
    
    
    LONG_EQ(t, (d3( 1.111) + 1.23456).value(),  2346, "add decimal and double"); // 2.34556
    LONG_EQ(t, (d3( 1.111) - 1.23456).value(), -124LL, "sub decimal and double"); // -0.12356
    LONG_EQ(t, (d3( 1.111) * d3( 2.222)).value(),  2469LL, "add two negative decimal"); 
    LONG_EQ(t, (d3( 1.111) * d3( 2.222)).value(),  2469LL, "add two negative decimal");
    
    IS_FALSE(t, (d3(9e12) + 1.0).error(), "");
    IS_FALSE(t, (d3(9e12) - 1.0).error(), "");

    IS_FALSE(t, (d3(9e10) *  1.0).error(), "");
    IS_FALSE(t, (d3(9e11) *  1.0).error(), "");
    IS_FALSE(t, (d3(9e12) *  1.0).error(), "");
    IS_FALSE(t, (d3(9e12) *  1.0001).error(), ""); // probably meaningless
    IS_TRUE (t, (d3(9e12) *  1.001).error(), "");
    IS_TRUE (t, (d3(9e12) *  1.010).error(), "");
    IS_TRUE (t, (d3(9e12) *  1.100).error(), "");
    IS_TRUE (t, (d3(9e12) * 10.0).error(), "");
    
    LONG_EQ(t, (Decimal3( LLONG_MAX) + 1.0).value(),  P::ErrorValue, "");
    LONG_EQ(t, (Decimal3(-LLONG_MAX) - 1.0).value(),  P::ErrorValue, "");
    
    LONG_EQ(t, (d3(P::MaxAccurateNumD) * 1.5).value(), P::ErrorValue, "");
}



int main()
{
    auto t = new_test_runner();

    decimal3_initialize(t);
    decimal3_initialize_from_double(t);
    decimal3_initialize_from_string(t);
    decimal3_arithmetic(t);

    int testok = is_test_ok(t);
    print_test_summary(t);
    free_test_runner(t);
    return testok ? 0 : 1;
}