#include "JSystem/JKernel/JKRDvdAramRipper.h"
#include "Dolphin/os.h"
#include "JSystem/JKernel/JKRAram.h"
#include "JSystem/JKernel/JKRFile.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JSupport/JSUList.h"
#include "JSystem/JSupport/JSUStream.h"
#include "stl/limits.h"
#include "types.h"

/*
    Generated from dpostproc

    .section .ctors, "wa"  # 0x80472F00 - 0x804732C0
    .4byte __sinit_JKRDvdAramRipper_cpp

    .section .bss  # 0x804EFC20 - 0x8051467C
    .global sDvdAramAsyncList__16JKRDvdAramRipper
    sDvdAramAsyncList__16JKRDvdAramRipper:
        .skip 0xC

    .section .sdata, "wa"  # 0x80514680 - 0x80514D80
    .global errorRetry__16JKRDvdAramRipper
    errorRetry__16JKRDvdAramRipper:
        .4byte 0x01000000
    .global sSZSBufferSize__16JKRDvdAramRipper
    sSZSBufferSize__16JKRDvdAramRipper:
        .4byte 0x00000400

    .section .sbss # 0x80514D80 - 0x80516360
    .global szpBuf_1
    szpBuf_1:
        .skip 0x4
    .global szpEnd_1
    szpEnd_1:
        .skip 0x4
    .global refBuf_1
    refBuf_1:
        .skip 0x4
    .global refEnd_1
    refEnd_1:
        .skip 0x4
    .global refCurrent_1
    refCurrent_1:
        .skip 0x4
    .global dmaBuf
    dmaBuf:
        .skip 0x4
    .global dmaEnd
    dmaEnd:
        .skip 0x4
    .global dmaCurrent
    dmaCurrent:
        .skip 0x4
    .global srcOffset_1
    srcOffset_1:
        .skip 0x4
    .global transLeft_1
    transLeft_1:
        .skip 0x4
    .global srcLimit_1
    srcLimit_1:
        .skip 0x4
    .global srcFile
    srcFile:
        .skip 0x4
    .global fileOffset_1
    fileOffset_1:
        .skip 0x4
    .global readCount_1
    readCount_1:
        .skip 0x4
    .global maxDest_1
    maxDest_1:
        .skip 0x4
    .global isInitMutex_1
    isInitMutex_1:
        .skip 0x4
    .global tsPtr_1
    tsPtr_1:
        .skip 0x4
    .global tsArea_1
    tsArea_1:
        .skip 0x4
*/

/*
 * --INFO--
 * Address:	8001D6F0
 * Size:	00009C
 * loadToAram__16JKRDvdAramRipperFPCcUl15JKRExpandSwitchUlUlPUl
 */
JKRAramBlock* JKRDvdAramRipper::loadToAram(const char* p1, u32 p2, JKRExpandSwitch expandSwitch, u32 p4, u32 p5, u32* p6)
{
	JKRDvdFile file;
	if (file.open(p1) == 0) {
		return 0;
	}
	return loadToAram(&file, p2, expandSwitch, p4, p5, p6);
}

/*
 * --INFO--
 * Address:	8001D78C
 * Size:	00009C
 * loadToAram__16JKRDvdAramRipperFlUl15JKRExpandSwitchUlUlPUl
 */
JKRAramBlock* JKRDvdAramRipper::loadToAram(long p1, u32 p2, JKRExpandSwitch expandSwitch, u32 p4, u32 p5, u32* p6)
{
	JKRDvdFile file;
	if (file.open(p1) == 0) {
		return 0;
	}
	return loadToAram(&file, p2, expandSwitch, p4, p5, p6);
}

/*
 * --INFO--
 * Address:	8001D828
 * Size:	0000A4
 * loadToAram__16JKRDvdAramRipperFP10JKRDvdFileUl15JKRExpandSwitchUlUlPUl
 */
JKRAramBlock* JKRDvdAramRipper::loadToAram(JKRDvdFile* file, u32 p2, JKRExpandSwitch expandSwitch, u32 p4, u32 p5, u32* p6)
{
	JKRADCommand* command = loadToAram_Async(file, p2, expandSwitch, nullptr, p4, p5, p6);
	syncAram(command, 0);
	if (command->_48 < 0) {
		delete command;
		return 0;
	}
	if (p2 != 0) {
		delete command;
		return reinterpret_cast<JKRAramBlock*>(std::numeric_limits<size_t>::max());
	}
	JKRAramBlock* result = command->mBlock;
	delete command;
	return result;
}

/*
 * --INFO--
 * Address:	........
 * Size:	00015C
 */
void JKRDvdAramRipper::loadToAram_Async(const char*, u32, JKRExpandSwitch, LoadCallback, u32, u32, u32*)
{
	// UNUSED FUNCTION
}

/*
 * --INFO--
 * Address:	........
 * Size:	00006C
 */
// JGadget::TPointer_delete<JKRDVDFile>::~TPointer_delete()
// {
// 	// UNUSED FUNCTION
// }

/*
 * --INFO--
 * Address:	........
 * Size:	00003C
 */
// JGadget::TPointer<JKRDVDFile>::~TPointer()
// {
// 	// UNUSED FUNCTION
// }

/*
 * --INFO--
 * Address:	........
 * Size:	000110
 */
void JKRDvdAramRipper::loadToAram_Async(long, u32, JKRExpandSwitch, LoadCallback, u32, u32, u32*)
{
	// UNUSED FUNCTION
}

/*
 * --INFO--
 * Address:	8001D8CC
 * Size:	0000AC
 * loadToAram_Async__16JKRDvdAramRipperFP10JKRDvdFileUl15JKRExpandSwitchPFUl_vUlUlPUl
 */
JKRADCommand* JKRDvdAramRipper::loadToAram_Async(JKRDvdFile* file, u32 p2, JKRExpandSwitch expandSwitch, void (*p4)(u32), u32 p5, u32 p6,
                                                 u32* p7)
{
	JKRADCommand* command = new (JKRHeap::sSystemHeap, -4) JKRADCommand();
	command->mDvdFile     = file;
	command->_2C          = p2;
	command->mBlock       = nullptr;
	command->_34          = expandSwitch;
	command->mCallBack    = p4;
	command->_3C          = p5;
	command->_40          = p6;
	command->_44          = p7;
	if (callCommand_Async(command) == nullptr) {
		delete command;
		return nullptr;
	}
	return command;
}

/*
 * --INFO--
 * Address:	8001D978
 * Size:	000350
 */
JKRADCommand* JKRDvdAramRipper::callCommand_Async(JKRADCommand*)
{
	/*
	stwu     r1, -0x70(r1)
	mflr     r0
	stw      r0, 0x74(r1)
	stmw     r25, 0x54(r1)
	mr       r31, r3
	li       r28, 1
	li       r26, 0
	lwz      r27, 0x28(r3)
	addi     r3, r27, 0x34
	bl       OSLockMutex
	lwz      r3, 0x44(r31)
	cmplwi   r3, 0
	beq      lbl_8001D9B4
	li       r0, 0
	stw      r0, 0(r3)

lbl_8001D9B4:
	lwz      r0, 0x50(r27)
	cmplwi   r0, 0
	beq      lbl_8001D9C8
	li       r28, 0
	b        lbl_8001DC94

lbl_8001D9C8:
	bl       OSGetCurrentThread
	stw      r3, 0x50(r27)
	li       r3, 0x10
	li       r5, -4
	lwz      r4, sSystemHeap__7JKRHeap@sda21(r13)
	bl       __nw__FUlP7JKRHeapi
	or.      r30, r3, r3
	beq      lbl_8001D9F4
	mr       r4, r27
	bl       __ct__18JSUFileInputStreamFP7JKRFile
	mr       r30, r3

lbl_8001D9F4:
	stw      r30, 0x54(r27)
	mr       r3, r27
	lwz      r12, 0(r27)
	lwz      r12, 0x1c(r12)
	mtctr    r12
	bctrl
	lwz      r0, 0x40(r31)
	cmplwi   r0, 0
	beq      lbl_8001DA24
	cmplw    r3, r0
	ble      lbl_8001DA24
	mr       r3, r0

lbl_8001DA24:
	lwz      r0, 0x34(r31)
	addi     r3, r3, 0x1f
	rlwinm   r29, r3, 0, 0, 0x1a
	cmpwi    r0, 1
	bne      lbl_8001DAF0
	addi     r0, r1, 0x27
	rlwinm   r25, r0, 0, 0, 0x1a

lbl_8001DA40:
	mr       r4, r25
	addi     r3, r27, 0x5c
	li       r5, 0x20
	li       r6, 0
	li       r7, 2
	bl       DVDReadPrio
	cmpwi    r3, 0
	bge      lbl_8001DA9C
	lbz      r0, errorRetry__16JKRDvdAramRipper@sda21(r13)
	cmplwi   r0, 0
	bne      lbl_8001DA94
	cmplwi   r30, 0
	beq      lbl_8001DA8C
	mr       r3, r30
	li       r4, 1
	lwz      r12, 0(r30)
	lwz      r12, 8(r12)
	mtctr    r12
	bctrl

lbl_8001DA8C:
	li       r3, 0
	b        lbl_8001DCB4

lbl_8001DA94:
	bl       VIWaitForRetrace
	b        lbl_8001DA40

lbl_8001DA9C:
	mr       r3, r25
	li       r4, 0x20
	bl       DCInvalidateRange
	mr       r3, r25
	bl       checkCompressed__9JKRDecompFPUc
	lbz      r0, 5(r25)
	mr       r26, r3
	lwz      r6, 0x40(r31)
	lbz      r4, 4(r25)
	slwi     r0, r0, 0x10
	lbz      r5, 6(r25)
	cmplwi   r6, 0
	rlwimi   r0, r4, 0x18, 0, 7
	lbz      r4, 7(r25)
	rlwimi   r0, r5, 8, 0x10, 0x17
	or       r0, r4, r0
	mr       r25, r0
	beq      lbl_8001DAF0
	cmplw    r0, r6
	ble      lbl_8001DAF0
	mr       r25, r6

lbl_8001DAF0:
	cmpwi    r26, 0
	bne      lbl_8001DB00
	li       r0, 0
	stw      r0, 0x34(r31)

lbl_8001DB00:
	lwz      r0, 0x34(r31)
	cmpwi    r0, 1
	bne      lbl_8001DB88
	lwz      r0, 0x2c(r31)
	cmplwi   r0, 0
	bne      lbl_8001DB58
	lwz      r0, 0x30(r31)
	cmplwi   r0, 0
	bne      lbl_8001DB58
	lwz      r3, sAramObject__7JKRAram@sda21(r13)
	mr       r4, r25
	li       r5, 0
	lwz      r3, 0x94(r3)
	bl       alloc__11JKRAramHeapFUlQ211JKRAramHeap10EAllocMode
	stw      r3, 0x30(r31)
	lwz      r3, 0x30(r31)
	cmplwi   r3, 0
	beq      lbl_8001DB50
	lwz      r0, 0x14(r3)
	stw      r0, 0x2c(r31)

lbl_8001DB50:
	lwz      r0, 0x30(r31)
	stw      r0, 0x4c(r27)

lbl_8001DB58:
	lwz      r3, 0x30(r31)
	cmplwi   r3, 0
	beq      lbl_8001DB6C
	lwz      r0, 0x14(r3)
	stw      r0, 0x2c(r31)

lbl_8001DB6C:
	lwz      r0, 0x2c(r31)
	cmplwi   r0, 0
	bne      lbl_8001DBE8
	li       r0, 0
	li       r3, 0
	stw      r0, 0x50(r27)
	b        lbl_8001DCB4

lbl_8001DB88:
	lwz      r0, 0x2c(r31)
	cmplwi   r0, 0
	bne      lbl_8001DBB8
	lwz      r0, 0x30(r31)
	cmplwi   r0, 0
	bne      lbl_8001DBB8
	lwz      r3, sAramObject__7JKRAram@sda21(r13)
	mr       r4, r29
	li       r5, 0
	lwz      r3, 0x94(r3)
	bl       alloc__11JKRAramHeapFUlQ211JKRAramHeap10EAllocMode
	stw      r3, 0x30(r31)

lbl_8001DBB8:
	lwz      r3, 0x30(r31)
	cmplwi   r3, 0
	beq      lbl_8001DBCC
	lwz      r0, 0x14(r3)
	stw      r0, 0x2c(r31)

lbl_8001DBCC:
	lwz      r0, 0x2c(r31)
	cmplwi   r0, 0
	bne      lbl_8001DBE8
	li       r0, 0
	li       r3, 0
	stw      r0, 0x50(r27)
	b        lbl_8001DCB4

lbl_8001DBE8:
	cmpwi    r26, 0
	bne      lbl_8001DC10
	lwz      r6, 0x3c(r31)
	mr       r3, r30
	lwz      r4, 0x2c(r31)
	lwz      r7, 0x44(r31)
	subf     r5, r6, r29
	bl write_StreamToAram_Async__13JKRAramStreamFP18JSUFileInputStreamUlUlUlPUl
	stw      r3, 0x50(r31)
	b        lbl_8001DC68

lbl_8001DC10:
	cmpwi    r26, 1
	bne      lbl_8001DC38
	lwz      r6, 0x3c(r31)
	mr       r3, r30
	lwz      r4, 0x2c(r31)
	lwz      r7, 0x44(r31)
	subf     r5, r6, r29
	bl write_StreamToAram_Async__13JKRAramStreamFP18JSUFileInputStreamUlUlUlPUl
	stw      r3, 0x50(r31)
	b        lbl_8001DC68

lbl_8001DC38:
	cmpwi    r26, 2
	bne      lbl_8001DC68
	li       r0, 0
	mr       r5, r29
	stw      r0, 0x50(r31)
	mr       r6, r25
	li       r8, 0
	lwz      r3, 0x28(r31)
	lwz      r4, 0x2c(r31)
	lwz      r7, 0x3c(r31)
	lwz      r9, 0x44(r31)
	bl       JKRDecompressFromDVDToAram__FP10JKRDvdFileUlUlUlUlUlPUl

lbl_8001DC68:
	lwz      r12, 0x38(r31)
	cmplwi   r12, 0
	bne      lbl_8001DC88
	lis      r3, sDvdAramAsyncList__16JKRDvdAramRipper@ha
	mr       r4, r31
	addi     r3, r3, sDvdAramAsyncList__16JKRDvdAramRipper@l
	bl       append__10JSUPtrListFP10JSUPtrLink
	b        lbl_8001DC94

lbl_8001DC88:
	mr       r3, r31
	mtctr    r12
	bctrl

lbl_8001DC94:
	addi     r3, r27, 0x34
	bl       OSUnlockMutex
	clrlwi   r0, r28, 0x18
	cmplwi   r0, 1
	bne      lbl_8001DCB0
	mr       r3, r31
	b        lbl_8001DCB4

lbl_8001DCB0:
	li       r3, 0

lbl_8001DCB4:
	lmw      r25, 0x54(r1)
	lwz      r0, 0x74(r1)
	mtlr     r0
	addi     r1, r1, 0x70
	blr
	*/
}

/*
 * --INFO--
 * Address:	8001DCC8
 * Size:	000070
 * __dt__18JSUFileInputStreamFv
 */
// JSUFileInputStream::~JSUFileInputStream() { }

/*
 * --INFO--
 * Address:	8001DD38
 * Size:	0000E0
 */
bool JKRDvdAramRipper::syncAram(JKRADCommand* command, int p2)
{
	JKRDvdFile* file = command->mDvdFile;
	OSLockMutex(&file->mMutex2);
	JKRAramStreamCommand* streamCommand = command->mStreamCommand;
	if (streamCommand != nullptr) {
		streamCommand = JKRAramStream::sync(streamCommand, p2);
		command->_48  = -(streamCommand == nullptr);
		if (p2 != 0 && streamCommand == nullptr) {
			OSUnlockMutex(&file->mMutex2);
			return false;
		}
	}
	sDvdAramAsyncList.remove(command);
	if (command->mStreamCommand != nullptr) {
		delete command->mStreamCommand;
	}
	delete file->mInputStream;
	file->_50 = nullptr;
	OSUnlockMutex(&file->mMutex2);
	return true;
}

/*
 * --INFO--
 * Address:	........
 * Size:	00010C
 */
void JKRDvdAramRipper::syncAramAll(int)
{
	// UNUSED FUNCTION
}

/*
 * --INFO--
 * Address:	........
 * Size:	000034
 */
void JKRDvdAramRipper::countLeftSync()
{
	// UNUSED FUNCTION
}

/*
 * --INFO--
 * Address:	........
 * Size:	000040
 */
void JKRDvdAramRipper::afterAramAsync(JKRADCommand*)
{
	// UNUSED FUNCTION
}

/*
 * --INFO--
 * Address:	8001DE18
 * Size:	000040
 * __ct__12JKRADCommandFv
 */
JKRADCommand::JKRADCommand()
    : JSULink<JKRADCommand>(this)
    , _48(0)
    , _4C(0)
{
}

/*
 * --INFO--
 * Address:	........
 * Size:	000054
 */
// JSULink<JKRADCommand>::~JSULink()
// {
// 	// UNUSED FUNCTION
// }

/*
 * --INFO--
 * Address:	8001DE58
 * Size:	00008C
 * __dt__12JKRADCommandFv
 */
JKRADCommand::~JKRADCommand()
{
	if (_4C == 1) {
		delete mDvdFile;
	}
}

/*
 * --INFO--
 * Address:	8001DEE4
 * Size:	000178
 */
void JKRDecompressFromDVDToAram(JKRDvdFile*, u32, u32, u32, u32, u32, u32*)
{
	/*
	.loc_0x0:
	  stwu      r1, -0x30(r1)
	  mflr      r0
	  stw       r0, 0x34(r1)
	  stmw      r24, 0x10(r1)
	  mr        r25, r3
	  mr        r26, r4
	  mr        r27, r5
	  mr        r28, r6
	  mr        r29, r7
	  mr        r30, r8
	  mr        r31, r9
	  bl        0xD0D28
	  lbz       r0, -0x7844(r13)
	  mr        r24, r3
	  cmplwi    r0, 0
	  bne-      .loc_0x54
	  lis       r3, 0x804F
	  subi      r3, r3, 0xA4
	  bl        0xD1C04
	  li        r0, 0x1
	  stb       r0, -0x7844(r13)

	.loc_0x54:
	  mr        r3, r24
	  bl        0xD0D24
	  lis       r3, 0x804F
	  subi      r3, r3, 0xA4
	  bl        0xD1C20
	  lwz       r24, -0x7FF4(r13)
	  li        r5, 0x20
	  lwz       r3, -0x77D8(r13)
	  mr        r4, r24
	  bl        0x56E4
	  mr        r4, r3
	  lwz       r3, -0x77D8(r13)
	  add       r0, r4, r24
	  stw       r4, -0x7880(r13)
	  li        r4, 0x1120
	  li        r5, 0
	  stw       r0, -0x787C(r13)
	  bl        0x56C4
	  mr        r6, r3
	  lwz       r3, -0x77D8(r13)
	  addi      r0, r6, 0x1120
	  stw       r6, -0x7878(r13)
	  li        r4, 0x100
	  li        r5, 0x20
	  stw       r0, -0x7874(r13)
	  stw       r6, -0x7870(r13)
	  bl        0x56A0
	  sub       r4, r27, r30
	  addi      r5, r3, 0x100
	  li        r0, 0
	  cmplwi    r31, 0
	  stw       r3, -0x786C(r13)
	  stw       r5, -0x7868(r13)
	  stw       r3, -0x7864(r13)
	  stw       r25, -0x7854(r13)
	  stw       r30, -0x7860(r13)
	  stw       r4, -0x785C(r13)
	  stw       r29, -0x7850(r13)
	  stw       r0, -0x784C(r13)
	  stw       r28, -0x7848(r13)
	  beq-      .loc_0x100
	  mr        r3, r31
	  b         .loc_0x104

	.loc_0x100:
	  subi      r3, r13, 0x783C

	.loc_0x104:
	  stw       r3, -0x7840(r13)
	  li        r0, 0
	  stw       r0, 0x0(r3)
	  bl        0x2E8
	  cmplwi    r3, 0
	  beq-      .loc_0x12C
	  mr        r4, r26
	  bl        .loc_0x178
	  mr        r25, r3
	  b         .loc_0x130

	.loc_0x12C:
	  li        r25, -0x1

	.loc_0x130:
	  lwz       r3, -0x7880(r13)
	  li        r4, 0
	  bl        0x5650
	  lwz       r3, -0x7878(r13)
	  li        r4, 0
	  bl        0x5644
	  lwz       r3, -0x786C(r13)
	  li        r4, 0
	  bl        0x5638
	  lis       r3, 0x804F
	  subi      r3, r3, 0xA4
	  bl        0xD1C04
	  mr        r3, r25
	  lmw       r24, 0x10(r1)
	  lwz       r0, 0x34(r1)
	  mtlr      r0
	  addi      r1, r1, 0x30
	  blr

	.loc_0x178:
	*/
}

/*
 * --INFO--
 * Address:	8001E05C
 * Size:	000280
 */
static void decompSZS_subroutine(u8*, u32)
{
	/*
	stwu     r1, -0x30(r1)
	mflr     r0
	stw      r0, 0x34(r1)
	stmw     r23, 0xc(r1)
	mr       r24, r4
	mr       r25, r24
	li       r29, 0
	li       r28, 0
	li       r26, 0
	lbz      r0, 0(r3)
	cmpwi    r0, 0x59
	bne      lbl_8001E0B0
	lbz      r0, 1(r3)
	cmpwi    r0, 0x61
	bne      lbl_8001E0B0
	lbz      r0, 2(r3)
	cmpwi    r0, 0x7a
	bne      lbl_8001E0B0
	lbz      r0, 3(r3)
	cmpwi    r0, 0x30
	beq      lbl_8001E0B8

lbl_8001E0B0:
	li       r3, -1
	b        lbl_8001E2C8

lbl_8001E0B8:
	lwz      r5, fileOffset_1@sda21(r13)
	lwz      r4, 4(r3)
	lwz      r0, maxDest_1@sda21(r13)
	subf     r4, r5, r4
	add      r31, r24, r4
	add      r0, r24, r0
	cmplw    r31, r0
	ble      lbl_8001E0DC
	mr       r31, r0

lbl_8001E0DC:
	addi     r23, r3, 0x10

lbl_8001E0E0:
	cmpwi    r29, 0
	bne      lbl_8001E118
	lwz      r0, srcLimit_1@sda21(r13)
	cmplw    r23, r0
	ble      lbl_8001E10C
	lwz      r0, transLeft_1@sda21(r13)
	cmplwi   r0, 0
	beq      lbl_8001E10C
	mr       r3, r23
	bl       nextSrcData__FPUc1
	mr       r23, r3

lbl_8001E10C:
	lbz      r28, 0(r23)
	li       r29, 8
	addi     r23, r23, 1

lbl_8001E118:
	rlwinm.  r0, r28, 0, 0x18, 0x18
	beq      lbl_8001E1B0
	lwz      r3, readCount_1@sda21(r13)
	lwz      r0, fileOffset_1@sda21(r13)
	cmplw    r3, r0
	blt      lbl_8001E170
	lwz      r3, dmaCurrent@sda21(r13)
	addi     r24, r24, 1
	lbz      r4, 0(r23)
	addi     r26, r26, 1
	addi     r0, r3, 1
	stw      r0, dmaCurrent@sda21(r13)
	stb      r4, 0(r3)
	lwz      r3, dmaCurrent@sda21(r13)
	lwz      r0, dmaEnd@sda21(r13)
	cmplw    r3, r0
	bne      lbl_8001E168
	mr       r3, r25
	bl       dmaBufferFlush__FUl
	add      r25, r25, r3

lbl_8001E168:
	cmplw    r24, r31
	beq      lbl_8001E2B4

lbl_8001E170:
	lwz      r3, refCurrent_1@sda21(r13)
	lbz      r4, 0(r23)
	addi     r0, r3, 1
	stw      r0, refCurrent_1@sda21(r13)
	stb      r4, 0(r3)
	lwz      r3, refCurrent_1@sda21(r13)
	lwz      r0, refEnd_1@sda21(r13)
	cmplw    r3, r0
	bne      lbl_8001E19C
	lwz      r0, refBuf_1@sda21(r13)
	stw      r0, refCurrent_1@sda21(r13)

lbl_8001E19C:
	lwz      r3, readCount_1@sda21(r13)
	addi     r23, r23, 1
	addi     r0, r3, 1
	stw      r0, readCount_1@sda21(r13)
	b        lbl_8001E2A4

lbl_8001E1B0:
	lbz      r5, 0(r23)
	lbz      r3, 1(r23)
	addi     r23, r23, 2
	lwz      r0, refCurrent_1@sda21(r13)
	rlwimi   r3, r5, 8, 0x14, 0x17
	lwz      r4, refBuf_1@sda21(r13)
	srawi    r27, r5, 4
	subf     r3, r3, r0
	addi     r30, r3, -1
	cmplw    r30, r4
	bge      lbl_8001E1E8
	lwz      r0, refEnd_1@sda21(r13)
	subf     r0, r4, r0
	add      r30, r30, r0

lbl_8001E1E8:
	cmpwi    r27, 0
	bne      lbl_8001E200
	lbz      r3, 0(r23)
	addi     r23, r23, 1
	addi     r27, r3, 0x12
	b        lbl_8001E204

lbl_8001E200:
	addi     r27, r27, 2

lbl_8001E204:
	lwz      r3, readCount_1@sda21(r13)
	lwz      r0, fileOffset_1@sda21(r13)
	cmplw    r3, r0
	blt      lbl_8001E254
	lwz      r3, dmaCurrent@sda21(r13)
	addi     r24, r24, 1
	lbz      r4, 0(r30)
	addi     r26, r26, 1
	addi     r0, r3, 1
	stw      r0, dmaCurrent@sda21(r13)
	stb      r4, 0(r3)
	lwz      r3, dmaCurrent@sda21(r13)
	lwz      r0, dmaEnd@sda21(r13)
	cmplw    r3, r0
	bne      lbl_8001E24C
	mr       r3, r25
	bl       dmaBufferFlush__FUl
	add      r25, r25, r3

lbl_8001E24C:
	cmplw    r24, r31
	beq      lbl_8001E2A4

lbl_8001E254:
	lwz      r3, refCurrent_1@sda21(r13)
	lbz      r4, 0(r30)
	addi     r0, r3, 1
	stw      r0, refCurrent_1@sda21(r13)
	stb      r4, 0(r3)
	lwz      r0, refCurrent_1@sda21(r13)
	lwz      r3, refEnd_1@sda21(r13)
	cmplw    r0, r3
	bne      lbl_8001E280
	lwz      r0, refBuf_1@sda21(r13)
	stw      r0, refCurrent_1@sda21(r13)

lbl_8001E280:
	addi     r30, r30, 1
	cmplw    r30, r3
	bne      lbl_8001E290
	lwz      r30, refBuf_1@sda21(r13)

lbl_8001E290:
	lwz      r3, readCount_1@sda21(r13)
	addic.   r27, r27, -1
	addi     r0, r3, 1
	stw      r0, readCount_1@sda21(r13)
	bne      lbl_8001E204

lbl_8001E2A4:
	cmplw    r24, r31
	slwi     r28, r28, 1
	addi     r29, r29, -1
	blt      lbl_8001E0E0

lbl_8001E2B4:
	mr       r3, r25
	bl       dmaBufferFlush__FUl
	lwz      r4, tsPtr_1@sda21(r13)
	li       r3, 0
	stw      r26, 0(r4)

lbl_8001E2C8:
	lmw      r23, 0xc(r1)
	lwz      r0, 0x34(r1)
	mtlr     r0
	addi     r1, r1, 0x30
	blr
	*/
}

/*
 * --INFO--
 * Address:	8001E2DC
 * Size:	0000B4
 */
static void firstSrcData()
{
	/*
	stwu     r1, -0x10(r1)
	mflr     r0
	stw      r0, 0x14(r1)
	stw      r31, 0xc(r1)
	stw      r30, 8(r1)
	lwz      r4, szpEnd_1@sda21(r13)
	lwz      r3, szpBuf_1@sda21(r13)
	addi     r0, r4, -25
	lwz      r5, transLeft_1@sda21(r13)
	subf     r4, r3, r4
	stw      r0, srcLimit_1@sda21(r13)
	cmplw    r5, r4
	mr       r30, r3
	mr       r31, r4
	bge      lbl_8001E31C
	mr       r31, r5

lbl_8001E31C:
	lwz      r3, srcFile@sda21(r13)
	mr       r4, r30
	mr       r5, r31
	li       r6, 0
	addi     r3, r3, 0x5c
	li       r7, 2
	bl       DVDReadPrio
	cmpwi    r3, 0
	bge      lbl_8001E35C
	lbz      r0, errorRetry__16JKRDvdAramRipper@sda21(r13)
	cmplwi   r0, 0
	bne      lbl_8001E354
	li       r3, 0
	b        lbl_8001E378

lbl_8001E354:
	bl       VIWaitForRetrace
	b        lbl_8001E31C

lbl_8001E35C:
	lwz      r4, srcOffset_1@sda21(r13)
	mr       r3, r30
	lwz      r0, transLeft_1@sda21(r13)
	add      r4, r4, r31
	subf     r0, r31, r0
	stw      r4, srcOffset_1@sda21(r13)
	stw      r0, transLeft_1@sda21(r13)

lbl_8001E378:
	lwz      r0, 0x14(r1)
	lwz      r31, 0xc(r1)
	lwz      r30, 8(r1)
	mtlr     r0
	addi     r1, r1, 0x10
	blr
	*/
}

/*
 * --INFO--
 * Address:	8001E390
 * Size:	0000EC
 */
static void nextSrcData(u8*)
{
	/*
	stwu     r1, -0x20(r1)
	mflr     r0
	mr       r4, r3
	stw      r0, 0x24(r1)
	stw      r31, 0x1c(r1)
	stw      r30, 0x18(r1)
	stw      r29, 0x14(r1)
	lwz      r0, szpEnd_1@sda21(r13)
	subf     r30, r4, r0
	clrlwi.  r5, r30, 0x1b
	beq      lbl_8001E3CC
	lwz      r3, szpBuf_1@sda21(r13)
	addi     r0, r3, 0x20
	subf     r29, r5, r0
	b        lbl_8001E3D0

lbl_8001E3CC:
	lwz      r29, szpBuf_1@sda21(r13)

lbl_8001E3D0:
	mr       r3, r29
	mr       r5, r30
	bl       memcpy
	lwz      r0, szpEnd_1@sda21(r13)
	add      r30, r29, r30
	lwz      r3, transLeft_1@sda21(r13)
	subf     r31, r30, r0
	cmplw    r31, r3
	ble      lbl_8001E3F8
	mr       r31, r3

lbl_8001E3F8:
	lwz      r3, srcFile@sda21(r13)
	mr       r4, r30
	lwz      r6, srcOffset_1@sda21(r13)
	mr       r5, r31
	addi     r3, r3, 0x5c
	li       r7, 2
	bl       DVDReadPrio
	cmpwi    r3, 0
	bge      lbl_8001E438
	lbz      r0, errorRetry__16JKRDvdAramRipper@sda21(r13)
	cmplwi   r0, 0
	bne      lbl_8001E430
	li       r3, 0
	b        lbl_8001E460

lbl_8001E430:
	bl       VIWaitForRetrace
	b        lbl_8001E3F8

lbl_8001E438:
	lwz      r0, transLeft_1@sda21(r13)
	lwz      r3, srcOffset_1@sda21(r13)
	subf.    r0, r31, r0
	add      r3, r3, r31
	stw      r3, srcOffset_1@sda21(r13)
	stw      r0, transLeft_1@sda21(r13)
	bne      lbl_8001E45C
	add      r0, r30, r31
	stw      r0, srcLimit_1@sda21(r13)

lbl_8001E45C:
	mr       r3, r29

lbl_8001E460:
	lwz      r0, 0x24(r1)
	lwz      r31, 0x1c(r1)
	lwz      r30, 0x18(r1)
	lwz      r29, 0x14(r1)
	mtlr     r0
	addi     r1, r1, 0x20
	blr
	*/
}

/*
 * --INFO--
 * Address:	8001E47C
 * Size:	000068
 */
static void dmaBufferFlush(u32)
{
	/*
	stwu     r1, -0x10(r1)
	mflr     r0
	stw      r0, 0x14(r1)
	stw      r31, 0xc(r1)
	lwz      r0, dmaCurrent@sda21(r13)
	lwz      r4, dmaBuf@sda21(r13)
	cmplw    r0, r4
	bne      lbl_8001E4A4
	li       r3, 0
	b        lbl_8001E4D0

lbl_8001E4A4:
	subf     r5, r4, r0
	li       r7, 0
	addi     r0, r5, 0x1f
	rlwinm   r31, r0, 0, 0, 0x1a
	mr       r5, r3
	li       r3, 0
	mr       r6, r31
	bl       orderSync__12JKRAramPieceFiUlUlUlP12JKRAramBlock
	lwz      r0, dmaBuf@sda21(r13)
	mr       r3, r31
	stw      r0, dmaCurrent@sda21(r13)

lbl_8001E4D0:
	lwz      r0, 0x14(r1)
	lwz      r31, 0xc(r1)
	mtlr     r0
	addi     r1, r1, 0x10
	blr
	*/
}
