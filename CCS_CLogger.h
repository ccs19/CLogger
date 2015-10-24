/**
Copyright 2015 Christopher Schneider

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
**/

/**
 * Author's note: Suitable for small projects that don't want the huge overhead of installing
 * third party logging libraries (e.g. Apache Log4cpp)
 *
 * Capabilities are limited intentionally.
 *
 * Call just like printf
 * e.g. LogE("Oh noes, call tech support! Their number is %s", "unknown");
 *
 * LogLevel | Definition | Description
 *    4         LogI         Info
 *    3         LogD         Debug
 *    2         LogW         Warn
 *    1         LogE         Error
 *
 * Log level 4 enables all logs, log level 1 only enables error logs
 * log level 0 disables all logs.
 *
 */

#ifndef CCS_CLOGGER_H
#define CCS_CLOGGER_H


/**Settings**/
#define CCS_LOGS_LEVEL 0


#include <string.h>
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */
#include <sys/time.h>

static long long CCS_CLogger_programStartTime;
static int CCS_CLogger_initialized = 0;
//Max log entry size. If you exceed this, you either need to shorten your logs
//or you should be using a more robust logging system :-)
static const int CCS_CLogger_BUFFER_SIZE = 1024;

static const char* CCS_CLogger_LOGGER_FORMAT = "[%ld] %s:%d [%s] %s\n";

static long long getTimeMilliseconds(){
    struct timeval te;
    gettimeofday(&te, NULL);
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000;
    return milliseconds;
}

static void initStartTime(void){
    if(!CCS_CLogger_initialized){
        CCS_CLogger_programStartTime = getTimeMilliseconds();
        CCS_CLogger_initialized = 1;
    }
}

static void makeMessage(FILE* output, const char *tag, const char* func,  const int line, const char* format, ...){
    char arr[CCS_CLogger_BUFFER_SIZE];
    va_list args;
    va_start(args, format);
    long long now = getTimeMilliseconds() - CCS_CLogger_programStartTime;
    const char* loggerFormat = "[%ld] %s:%d [%s] %s\n"; //[time] func:line [tag] <msg> (msg exists from format and va_list args)
    snprintf(arr, CCS_CLogger_BUFFER_SIZE, loggerFormat, now, func, line, tag, format);
    vfprintf(output, arr, args);
    va_end(args);
    fflush(output);
}




#if(CCS_LOGS_LEVEL >= 1)
#define LogE(fmt, ...)  makeMessage(stderr, "ERROR", __func__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define LogE(fmt, ...)
#endif
#if(CCS_LOGS_LEVEL >= 2)
#define LogW(fmt, ...)  makeMessage(stderr, "WARN", __func__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define LogW(fmt, ...)
#endif
#if(CCS_LOGS_LEVEL >= 3)
#define LogD(fmt, ...) makeMessage(stdout, "DEBUG", __func__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define LogD(fmt, ...)
#endif
#if(CCS_LOGS_LEVEL >= 4)
#define LogI(fmt, ...)  makeMessage(stdout, "INFO", __func__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define LogI(fmt, ...)
#endif

#endif //CCS_CLOGGER_H

