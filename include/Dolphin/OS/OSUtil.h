#ifndef _DOLPHIN_OS_OSUTIL_H
#define _DOLPHIN_OS_OSUTIL_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

///// USEFUL MACROS/DEFINES //////
// Macro for making clear things are addresses.
#define AT_ADDRESS(addr) : (addr)

// Defines for cached and uncached memory.
#define OS_BASE_CACHED   (0x80000000)
#define OS_BASE_UNCACHED (0xC0000000)

//////////////////////////////////

////////// CLOCK UTILS ///////////
// Time and tick typedefs for convenience.
typedef s64 OSTime;
typedef u32 OSTick;

// Clock speeds.
u32 __OSBusClock AT_ADDRESS(OS_BASE_CACHED | 0x00F8);
u32 __OSCoreClock AT_ADDRESS(OS_BASE_CACHED | 0x00FC);

#define OS_BUS_CLOCK   __OSBusClock
#define OS_CORE_CLOCK  __OSCoreClock
#define OS_TIMER_CLOCK (OS_BUS_CLOCK / 4)

// Tick conversions.
#define OSTicksToCycles(ticks)       (((ticks) * ((OS_CORE_CLOCK * 2) / OS_TIMER_CLOCK)) / 2)
#define OSTicksToSeconds(ticks)      ((ticks) / OS_TIMER_CLOCK)
#define OSTicksToMilliseconds(ticks) ((ticks) / (OS_TIMER_CLOCK / 1000))
#define OSTicksToMicroseconds(ticks) (((ticks)*8) / (OS_TIMER_CLOCK / 125000))
#define OSTicksToNanoseconds(ticks)  (((ticks)*8000) / (OS_TIMER_CLOCK / 125000))
#define OSSecondsToTicks(sec)        ((sec)*OS_TIMER_CLOCK)
#define OSMillisecondsToTicks(msec)  ((msec) * (OS_TIMER_CLOCK / 1000))
#define OSMicrosecondsToTicks(usec)  (((usec) * (OS_TIMER_CLOCK / 125000)) / 8)
#define OSNanosecondsToTicks(nsec)   (((nsec) * (OS_TIMER_CLOCK / 125000)) / 8000)

#define OSDiffTick(tick1, tick0) ((s32)(tick1) - (s32)(tick0))

// Time-related getters.
OSTick OSGetTick();
OSTime OSGetTime();

// Struct for 'calendar time'.
typedef struct OSCalendarTime {
	int sec;  // _00, secs after minute
	int min;  // _04, mins after hour
	int hour; // _08, hours since midnight
	int mday; // _0C, day of month
	int mon;  // _10, month since Jan
	int year; // _14, years since 0000
	int wday; // _18, days since Sunday
	int yday; // _1C, days since Jan 1
	int msec; // _20, millisecs after sec
	int usec; // _24, microsecs after millisec
} OSCalendarTime;

// Calendar time functions.
OSTime OSCalendarTimeToTicks(OSCalendarTime* timeDate);
void OSTicksToCalendarTime(OSTime ticks, OSCalendarTime* timeDate);

//////////////////////////////////

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif