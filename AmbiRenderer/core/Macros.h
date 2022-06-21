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
    codes_here;\
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

#define MS_PER_SEC float(1000)
#define KeyCode unsigned char
#define KeyCode_K 0x4B

#define GEN_STATIC_DYN_CALL_0(function, class_name) \
namespace _##class_name { \
	template <typename U> \
	struct has_##function { template <typename T, void(T::*)(void) = &T::##function##> \
		static constexpr bool check(T*) { return true; }; \
		static constexpr bool check(...) { return false; }; \
		static constexpr bool ret = check(static_cast<U*>(0));}; \
	template<typename T, typename std::enable_if<has_##function##<T>::ret, T>::type* = nullptr > \
	void function##(T* t) {t->##function##();} \
	template<typename T, typename std::enable_if<!has_##function##<T>::ret, T>::type* = nullptr> \
	void function##(T* t) {;} \
}

#define GEN_STATIC_DYN_CALL_1(function, fp_1, class_name) \
namespace _##class_name { \
	template <typename U> \
	struct has_##function { template <typename T, void(T::*)(##fp_1##) = &T::##function##> \
		static constexpr bool check(T*) { return true; }; \
		static constexpr bool check(...) { return false; }; \
		static constexpr bool ret = check(static_cast<U*>(0));}; \
	template<typename T, typename std::enable_if<has_##function##<T>::ret, T>::type* = nullptr > \
	void function##(T* t, fp_1##) {t->##function##(_1);} \
	template<typename T, typename std::enable_if<!has_##function##<T>::ret, T>::type* = nullptr> \
	void function##(T* t, fp_1##) {;} \
}

#define GEN_STATIC_DYN_CALL_2(function, fp_1, fp_2, class_name) \
namespace _##class_name { \
	template <typename U> \
	struct has_##function { template <typename T, void(T::*)(##fp_1##, fp_2##) = &T::##function##> \
		static constexpr bool check(T*) { return true; }; \
		static constexpr bool check(...) { return false; }; \
		static constexpr bool ret = check(static_cast<U*>(0));}; \
	template<typename T, typename std::enable_if<has_##function##<T>::ret, T>::type* = nullptr > \
	void function##(T* t, fp_1##, fp_2##) {t->##function##(_1, _2);} \
	template<typename T, typename std::enable_if<!has_##function##<T>::ret, T>::type* = nullptr> \
	void function##(T* t, fp_1##, fp_2##) {;} \
}
