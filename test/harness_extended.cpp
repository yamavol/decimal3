#include "harness_extended.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdexcept>

static void handle_test_result(test_runner* runner, int cond, const char* msg, va_list args)
{
    runner->test_num++;
    if (cond) {
        runner->pass_count++;
    }
    else {
        runner->fail_count++;
        printf("FAILED test %d: ", runner->test_num);
        vprintf(msg, args);
        printf("\n");
    }
}


void ASSERT_THROW(test_runner* runner, std::function<void()> function, const char* msg, ...)
{
    int cond = 0;
    try {
        function();
    }
    catch(const std::exception&) {
        cond = 1;
    }
    catch(const std::string&) {
        cond = 1;
    }
    catch(const char*) {
        cond = 1;
    }
    va_list args;
    va_start(args, msg);
    handle_test_result(runner, cond, msg, args);
    va_end(args);

    if (!cond) {
        printf("  Expected function to throw, but it did not\n");
    }
}

void ASSERT_NO_THROW(test_runner* runner, std::function<void()> function, const char* msg, ...)
{
    int cond = 0;
    std::string emsg;
    try {
        function();
        cond = 1;
    }
    catch(const std::exception& e) {
        emsg = e.what();
    }
    catch(const std::string& text) {
        emsg = text;
    }
    catch(const char* text) {
        emsg = text;
    }
    va_list args;
    va_start(args, msg);
    handle_test_result(runner, cond, msg, args);
    va_end(args);

    if (!cond) {
        printf("  Expected function not to throw, but thrown\n");
        printf("  Hint: %s\n", emsg.c_str());
    }
}