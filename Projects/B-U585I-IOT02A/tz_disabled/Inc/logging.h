/*
 * FreeRTOS STM32 Reference Integration
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#ifndef LOGGING_H
#define LOGGING_H

/* Standard Include. */
#include <stdio.h>

/* Include header for logging level macros. */
#include "logging_levels.h"
// #include "iot_uart.h"

/* Default logging config */
#if( !defined( LOGGING_OUTPUT_UART ) && !defined( LOGGING_OUTPUT_ITM ) && !defined( LOGGING_OUTPUT_NONE ) )
#define LOGGING_OUTPUT_UART
#endif

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_INFO
#endif

/* Get rid of extra C89 style parentheses generated by core FreeRTOS libraries */
#define REMOVE_PARENS(...) __VA_ARGS__

/* Function declarations */
void vLoggingPrintf( const char * const     pcLogLevel,
                     const char * const     pcFunctionName,
                     const unsigned long    ulLineNumber,
                     const char * const     pcFormat,
                     ... );
void vLoggingInit( void );
void vLoggingDeInit( void );
void vDyingGasp( void );
// IotUARTHandle_t xLoggingGetIOHandle( void );

/* task.h cannot be included here because this file is included by FreeRTOSConfig.h */
extern void vTaskSuspendAll( void );

static inline const char * pcPathToBasename( const char * pcFileName )
{
    const char * pcIter = pcFileName;
    const char * pcBasename = pcFileName;

    /* Extract basename from file name */
    while( *pcIter != '\0' )
    {
        if( *pcIter == '/' || *pcIter == '\\')
        {
            pcBasename = pcIter + 1;
        }
        pcIter++;
    }

    return pcBasename;
}

/* Get basename of file using gcc builtins. */
#define __NAME_ARG__ (__builtin_strrchr (__BASE_FILE__, '/') ? __builtin_strrchr (__BASE_FILE__, '/') + 1 : __BASE_FILE__)

/* Generic logging macros */
#define SdkLog( level, ... )        do { vLoggingPrintf( level, __NAME_ARG__, __LINE__, __VA_ARGS__ ); } while( 0 )

#define LogAssert( ... )            do { vTaskSuspendAll(); vDyingGasp(); SdkLog( "ASRT", __VA_ARGS__ ); } while( 0 )

#define LogSys( ... )               do { vLoggingPrintf( "SYS", "", 0, __VA_ARGS__ ); } while( 0 )

#define LogKernel( ... )            SdkLog( "KRN", __VA_ARGS__ )

#if !defined( LOG_LEVEL ) ||       \
    ( ( LOG_LEVEL != LOG_NONE ) && \
    ( LOG_LEVEL != LOG_ERROR ) &&  \
    ( LOG_LEVEL != LOG_WARN ) &&   \
    ( LOG_LEVEL != LOG_INFO ) &&   \
    ( LOG_LEVEL != LOG_DEBUG ) )

    #error "Please define LOG_LEVEL as either LOG_NONE, LOG_ERROR, LOG_WARN, LOG_INFO, or LOG_DEBUG."
#else

    #if ( LOG_LEVEL >= LOG_ERROR )
        #ifdef LOGGING_REMOVE_PARENS
            #define LogError( ARGS )        SdkLog( "ERR", REMOVE_PARENS ARGS  )
        #else
            #define LogError( ... )         SdkLog( "ERR", __VA_ARGS__ )
        #endif
    #else
        #define LogError( ... )
    #endif

    #if ( LOG_LEVEL >= LOG_WARN )
        #ifdef LOGGING_REMOVE_PARENS
            #define LogWarn( ARGS )         SdkLog( "WRN", REMOVE_PARENS ARGS  )
        #else
            #define LogWarn( ... )          SdkLog( "WRN", __VA_ARGS__ )
        #endif
    #else
        #define LogWarn( ... )
    #endif

    #if ( LOG_LEVEL >= LOG_INFO )
        #ifdef LOGGING_REMOVE_PARENS
            #define LogInfo( ARGS )         SdkLog( "INF", REMOVE_PARENS ARGS  )
        #else
            #define LogInfo( ... )          SdkLog( "INF", __VA_ARGS__ )
        #endif
    #else
        #define LogInfo( ... )
    #endif

    #if ( LOG_LEVEL >= LOG_DEBUG )
        #ifdef LOGGING_REMOVE_PARENS
            #define LogDebug( ARGS )        SdkLog( "DBG", REMOVE_PARENS ARGS  )
        #else
            #define LogDebug( ... )         SdkLog( "DBG", __VA_ARGS__ )
        #endif
    #else
        #define LogDebug( ... )
    #endif

#endif

#endif /* LOGGING_H */
