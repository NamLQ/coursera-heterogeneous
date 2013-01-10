// wb.h: Header file for Heterogeneous Parallel Programming course (Coursera)

#pragma once

////
// Headers
////

// C++
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <list>
#include <sstream>
#include <string>
#include <vector>

// CUDA
#include <cuda.h>
#include <cuda_runtime.h>

////
// Logging
////

enum wbLogLevel
{
    OFF,
    FATAL,
    ERROR,
    WARN,
    INFO,
    DEBUG,
    TRACE,
    wbLogLevelNum, // Keep this at the end
};

const char* _wbLogLevelStr[] =
{
    "Off",
    "Fatal",
    "Error",
    "Warn",
    "Info",
    "Debug",
    "Trace",
    "***InvalidLogLevel***", // Keep this at the end
};

const char* _wbLogLevelToStr(wbLogLevel level)
{
    assert(level >= OFF && level <= TRACE);
    return _wbLogLevelStr[level];
}

//-----------------------------------------------------------------------------
// Begin: Ugly C++03 hack
// NVCC does not support C++11 variadic template yet

template<typename T1>
inline void _wbLog(T1 const& p1)
{
    std::cout << p1;
}

template<typename T1, typename T2>
inline void _wbLog(T1 const& p1, T2 const& p2)
{
    std::cout << p1 << p2;
}

template<typename T1, typename T2, typename T3>
inline void _wbLog(T1 const& p1, T2 const& p2, T3 const& p3)
{
    std::cout << p1 << p2 << p3;
}

template<typename T1, typename T2, typename T3, typename T4>
inline void _wbLog(T1 const& p1, T2 const& p2, T3 const& p3, T4 const& p4)
{
    std::cout << p1 << p2 << p3 << p4;
}

template<typename T1, typename T2, typename T3, typename T4, typename T5>
inline void _wbLog(T1 const& p1, T2 const& p2, T3 const& p3, T4 const& p4, T5 const& p5)
{
    std::cout << p1 << p2 << p3 << p4 << p5;
}

template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
inline void _wbLog(T1 const& p1, T2 const& p2, T3 const& p3, T4 const& p4, T5 const& p5, T6 const& p6)
{
    std::cout << p1 << p2 << p3 << p4 << p5 << p6;
}

template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
inline void _wbLog(T1 const& p1, T2 const& p2, T3 const& p3, T4 const& p4, T5 const& p5, T6 const& p6, T7 const& p7)
{
    std::cout << p1 << p2 << p3 << p4 << p5 << p6 << p7;
}

template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
inline void _wbLog(T1 const& p1, T2 const& p2, T3 const& p3, T4 const& p4, T5 const& p5, T6 const& p6, T7 const& p7, T8 const& p8)
{
    std::cout << p1 << p2 << p3 << p4 << p5 << p6 << p7 << p8;
}

template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
inline void _wbLog(T1 const& p1, T2 const& p2, T3 const& p3, T4 const& p4, T5 const& p5, T6 const& p6, T7 const& p7, T8 const& p8, T9 const& p9)
{
    std::cout << p1 << p2 << p3 << p4 << p5 << p6 << p7 << p8 << p9;
}

template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
inline void _wbLog(T1 const& p1, T2 const& p2, T3 const& p3, T4 const& p4, T5 const& p5, T6 const& p6, T7 const& p7, T8 const& p8, T9 const& p9, T10 const& p10)
{
    std::cout << p1 << p2 << p3 << p4 << p5 << p6 << p7 << p8 << p9 << p10;
}

// End: Ugly C++03 hack
//-----------------------------------------------------------------------------

#define wbLog(level, ...)                                     \
    do                                                        \
    {                                                         \
        std::cout << _wbLogLevelToStr(level) << " ";          \
        std::cout << __FUNCTION__ << "::" << __LINE__ << " "; \
        _wbLog(__VA_ARGS__);                                  \
        std::cout << std::endl;                               \
    } while (0)

////
// Input arguments
////

struct wbArg_t
{
    int    argc;
    char** argv;
};

wbArg_t wbArg_read(int argc, char** argv)
{
    wbArg_t argInfo = { argc, argv };
    return argInfo;
}

char* wbArg_getInputFile(wbArg_t argInfo, int argNum)
{
    assert(argNum >= 0 && argNum < (argInfo.argc - 1));
    return argInfo.argv[argNum + 1];
}

// For assignments MP1, MP4 & MP5
float* wbImport(char* fname, int* itemNum)
{
    // Open file

    std::ifstream inFile(fname);

    if (!inFile)
    {
        std::cout << "Error opening input file: " << fname << " !\n";
        exit(1);
    }

    // Read from file

    inFile >> *itemNum;

    float* fBuf = (float*) malloc( *itemNum * sizeof(float) );

    std::string sval;
    int idx = 0;

    while (inFile >> sval)
    {
        std::istringstream iss(sval);
        iss >> fBuf[ idx++ ];
    }

    return fBuf;
}

// For assignments MP2 & MP3
float* wbImport(char* fname, int* numRows, int* numCols)
{
    // Open file

    std::ifstream inFile(fname);

    if (!inFile)
    {
        std::cout << "Error opening input file: " << fname << " !\n";
        exit(1);
    }

    // Read file to vector

    std::string sval;
    float fval;
    std::vector<float> fVec;
    int itemNum = 0;

    // Read in matrix dimensions

    inFile >> *numRows;
    inFile >> *numCols;

    while (inFile >> sval)
    {
        std::istringstream iss(sval);
        iss >> fval;
        fVec.push_back(fval );
    }

    // Vector to malloc memory

    if (fVec.size() != (*numRows * *numCols))
    {
        std::cout << "Error reading matrix content for a " << *numRows << " * " << *numCols << "matrix!\n";
        exit(1);
    }

    itemNum = *numRows * *numCols;

    float* fBuf = (float*) malloc(itemNum * sizeof(float));

    for (int i = 0; i < itemNum; ++i)
    {
        fBuf[i] = fVec[i];
    }

    return fBuf;
}

////
// Timer
////

// Namespace because windows.h causes errors
namespace CudaTimerNS
{
#if defined (_WIN32)
    #include <Windows.h>

    // CudaTimer class from: https://bitbucket.org/ashwin/cudatimer
    class CudaTimer
    {
    private:
        double        timerResolution;
        LARGE_INTEGER startTime;
        LARGE_INTEGER endTime;

    public:
        CudaTimer::CudaTimer()
        {
            LARGE_INTEGER freq;
            QueryPerformanceFrequency(&freq);
            timerResolution = 1.0 / freq.QuadPart;
            return;
        }

        void start()
        {
            cudaDeviceSynchronize();
            QueryPerformanceCounter(&startTime);
            return;
        }

        void stop()
        {
            cudaDeviceSynchronize();
            QueryPerformanceCounter(&endTime);
            return;
        }

        double value()
        {
            return (endTime.QuadPart - startTime.QuadPart) * timerResolution * 1000;
        }
    };
#elif defined (__APPLE__)
    #include <mach/mach_time.h>

    class CudaTimer
    {
    private:
        uint64_t startTime;
        uint64_t endTime;

    public:
        void start()
        {
            cudaDeviceSynchronize();
            startTime = mach_absolute_time();
        }

        void stop()
        {
            cudaDeviceSynchronize();
            endTime = mach_absolute_time();
        }

        double value()
        {
            static mach_timebase_info_data_t tb;

            if (0 == tb.denom)
                (void) mach_timebase_info(&tb); // Calculate ratio of mach_absolute_time ticks to nanoseconds

            return ((double) endTime - startTime) * (tb.numer / tb.denom) / 1000000000ULL;
        }
    };
#else
    #if defined(_POSIX_TIMERS) && _POSIX_TIMERS > 0
        #include<time.h>
    #else
        #include<sys/time.h>
    #endif

    class CudaTimer
    {
    private:
        long long startTime;
        long long endTime;

        long long getTime()
        {
            long long time;
        #if defined(_POSIX_TIMERS) && _POSIX_TIMERS > 0

            struct timespec ts;

            if ( 0 == clock_gettime(CLOCK_REALTIME, &ts) )
            {
                time  = 1000000000LL; // seconds->nanonseconds
                time *= ts.tv_sec;
                time += ts.tv_nsec;
            }
        #else
            struct timeval tv;

            if ( 0 == gettimeofday(&tv, NULL) )
            {
                time  = 1000000000LL; // seconds->nanonseconds
                time *= tv.tv_sec;
                time += tv.tv_usec * 1000; // ms->ns
            }
        #endif

            return time;
        }

    public:
        void start()
        {
            startTime = getTime();
        }

        void stop()
        {
            endTime = getTime();
        }

        double value()
        {
            return ((double) endTime - startTime) / 1000000000LL;
        }
    };
#endif
}

enum wbTimeType
{
    Generic,
    GPU,
    Compute,
    Copy,
    wbTimeTypeNum, // Keep this at the end
};

const char* wbTimeTypeStr[] =
{
    "Generic",
    "GPU    ",
    "Compute",
    "Copy   ",
    "***InvalidTimeType***", // Keep this at the end
};

const char* wbTimeTypeToStr(wbTimeType t)
{
    assert(t >= Generic && t < wbTimeTypeNum);
    return wbTimeTypeStr[t];
}

struct wbTimerInfo
{
    wbTimeType             type;
    std::string            name;
    CudaTimerNS::CudaTimer timer;

    bool operator == (const wbTimerInfo& t2) const
    {
        return (type == t2.type && (0 == name.compare(t2.name)));
    }
};

typedef std::list< wbTimerInfo> wbTimerInfoList;
wbTimerInfoList gTimerInfoList;

void wbTime_start(wbTimeType timeType, const std::string timeStar)
{
    CudaTimerNS::CudaTimer timer;
    timer.start();

    wbTimerInfo tInfo = { timeType, timeStar, timer };

    gTimerInfoList.push_front(tInfo);

    return;
}

void wbTime_stop(wbTimeType timeType, const std::string timeStar)
{
    // Find timer

    const wbTimerInfo searchInfo         = { timeType, timeStar };
    const wbTimerInfoList::iterator iter = std::find( gTimerInfoList.begin(), gTimerInfoList.end(), searchInfo );

    // Stop timer and print time

    wbTimerInfo& timerInfo = *iter;

    timerInfo.timer.stop();

    std::cout << "[" << wbTimeTypeToStr( timerInfo.type ) << "] ";
    std::cout << std::fixed << std::setprecision(9) << timerInfo.timer.value() << " ";
    std::cout << timerInfo.name << std::endl;

    // Delete timer from list
    gTimerInfoList.erase(iter);

    return;
}

////
// Solutions
////

// For assignments MP1, MP4 & MP5
template < typename T, typename S >
void wbSolution(wbArg_t args, const T& t, const S& s)
{
    int solnItems;
    float *soln = (float *) wbImport(wbArg_getInputFile(args, args.argc - 2), &solnItems);

    if (solnItems != s)
    {
        std::cout << "Number of elements in solution file does not match. ";
        std::cout << "Expecting " << s << " but got " << solnItems << ".\n";

        free(soln);

        return;
    }
    
    // Check solution

    int errCnt = 0;

    for (int item = 0; item < solnItems; item++)
    {
        if (fabs((soln[item] - t[item]) / (t[item] == 0.0f ? 1.0f : t[item])) > 0.005f)
        {
            std::cout << "The solution did not match the expected result at element " << item << ". ";
            std::cout << "Expecting " << soln[item] << " but got " << t[item] << ".\n";
            errCnt++;
        }
    }

    if (!errCnt)
        std::cout << "Solution is correct.\n";
    else
        std::cout << errCnt << " tests failed!\n";
    
    free(soln);

    return;
}

// For assignments MP2 & MP3
template < typename T, typename S, typename U >
void wbSolution(wbArg_t args, const T& t, const S& s, const U& u)
{
    int solnRows, solnColumns;
    float *soln = (float *) wbImport(wbArg_getInputFile(args, 2), &solnRows, &solnColumns);

    if (solnRows != s || solnColumns != u)
    {
        std::cout << "Size of solution file does not match. ";
        std::cout << "Expecting " << solnRows << " x " << solnColumns << " but got " << s << " x " << u << ".\n";

        free(soln);

        return;
    }
    
    // Check solution

    int errCnt = 0;

    for (int row = 0; row < solnRows; row++)
    {
        for (int col = 0; col < solnColumns; col++)
        {
            float expected = *(soln + row * solnColumns + col);
            float result = *(t + row * solnColumns + col);

            if (fabs((soln[item] - t[item]) / (t[item] == 0.0f ? 1.0f : t[item])) > 0.005f)
            {
                std::cout << "The solution did not match the expected results at column " << col << " and row " << row << "). ";
                std::cout << "Expecting " << expected << " but got " << result << ".\n";
                errCnt++;
            }
        }
    }

    if (!errCnt)
        std::cout << "Solution is correct.\n";
    else
        std::cout << errCnt << " tests failed!\n";
    
    free(soln);

    return;
}
