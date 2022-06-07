#pragma once

#ifdef _DEBUG

#include <iostream>
#include <sstream>
#include <Windows.h>

#define ABR_STATE_FUNCTION(state, function) \
{ \
    std::wostringstream os_; \
    os_ << #state <<": "<<#function<<"\n";\
    OutputDebugStringW( os_.str().c_str() );  \
    function;\
} \

#define ABR_DEBUG_OUTPUT(output_any) \
{ \
    std::wostringstream os_; \
    os_ <<"["<<__FILE__<<": "<<__func__<<"(). L: "<<__LINE__<<"]: "<<##output_any##<<"\n";\
    OutputDebugStringW( os_.str().c_str() );  \
} \

#define ABR_RUN_WITH_CLOCK(codes_here)\
{ \
    std::wostringstream os_; \
    auto startTime = std::chrono::steady_clock::now();\
    sth;\
    os_ <<"Time Elapse: "<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count()<<" ms\n";\
    OutputDebugStringW( os_.str().c_str() );  \
} \

#else

#define ABR_STATE_FUNCTION(state, function) \
    function;\

#define ABR_RUN_TEST(function) \
    function;\

// do nothing
#define ABR_DEBUG_OUTPUT(output_any) ;

#define ABR_RUN_WITH_CLOCK(sth)\
    sth;\

#endif // DEBUG

#define KEY unsigned char
#define KB_K 0x4B