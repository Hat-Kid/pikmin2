#include "types.h"
#include "Dolphin/OS/OSInterrupt.h"

/*
 * --INFO--
 * Address:	800C0C48
 * Size:	000008
 */
int udp_cc_initialize(void* flagOut, __OSInterruptHandler handler) { return -1; }

/*
 * --INFO--
 * Address:	800C0C40
 * Size:	000008
 */
int udp_cc_shutdown(void) { return -1; }

/*
 * --INFO--
 * Address:	800C0C38
 * Size:	000008
 */
int udp_cc_open(void) { return -1; }

/*
 * --INFO--
 * Address:	800C0C30
 * Size:	000008
 */
int udp_cc_close(void) { return -1; }

/*
 * --INFO--
 * Address:	800C0C28
 * Size:	000008
 */
int udp_cc_read(u8* dest, int size) { return 0; }

/*
 * --INFO--
 * Address:	800C0C20
 * Size:	000008
 */
int udp_cc_write(const u8* src, int size) { return 0; }

/*
 * --INFO--
 * Address:	800C0C18
 * Size:	000008
 */
int udp_cc_peek(void) { return 0; }

/*
 * --INFO--
 * Address:	800C0C10
 * Size:	000008
 */
int udp_cc_pre_continue(void) { return -1; }

/*
 * --INFO--
 * Address:	800C0C08
 * Size:	000008
 */
int udp_cc_post_stop(void) { return -1; }
