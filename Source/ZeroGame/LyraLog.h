#pragma once

#include "Logging/LogMacros.h"


class UObject;


DECLARE_LOG_CATEGORY_EXTERN(LogLyra, Log, All);


FString LyraGetClientServerContextString(UObject* ContextObject = nullptr);


#define BOOL2TEXT(b)  (b) ? *TrueString : *FalseString

EXTERN_C const FString FalseString;
EXTERN_C const FString TrueString;


/**
 * Write a log of the specified level to the LogLyra log
 * @param Level UE_LOG Level
 * @param fmt FString::Printf format string
 */
#define Lyra_LEVEL_LOG(Level, fmt, ...) \
        UE_LOG(LogLyra, Level, TEXT("%s"), \
            *FString::Printf(TEXT("%05llu %s:%i %s"), \
            GFrameCounter, *FString(__FUNCTION__), __LINE__, \
            *FString::Printf(fmt, ##__VA_ARGS__)))


#define Lyra_LOG(fmt, ...)             Lyra_LEVEL_LOG(Log, fmt, ##__VA_ARGS__)
#define Lyra_DISPLAY_LOG(fmt, ...)     Lyra_LEVEL_LOG(Display, fmt, ##__VA_ARGS__)
#define Lyra_ERROR_LOG(fmt, ...)       Lyra_LEVEL_LOG(Error, fmt, ##__VA_ARGS__)
#define Lyra_WARNING_LOG(fmt, ...)     Lyra_LEVEL_LOG(Warning, fmt, ##__VA_ARGS__)
#define Lyra_VERBOSE_LOG(fmt, ...)     Lyra_LEVEL_LOG(Verbose, fmt, ##__VA_ARGS__)


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
#define Lyra_DEBUG_LOG(fmt, ...)       Lyra_LEVEL_LOG(VeryVerbose, fmt, ##__VA_ARGS__)
#else
#define Lyra_DEBUG_LOG(fmt, ...)       {}
#endif