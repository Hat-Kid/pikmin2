#include "Dolphin/os.h"
#include "JSystem/JMath.h"
#include "JSystem/JKernel/JKRAram.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JKernel/JKRDecomp.h"
#include "JSystem/JKernel/JKRDvdAramRipper.h"
#include "JSystem/JKernel/JKRDvdRipper.h"
#include "types.h"
#include "stl/stdlib.h"
#include "stl/limits.h"
#include "stl/mem.h"

/*
 * --INFO--
 * Address:	........
 * Size:	00003C
 * UNUSED
 */
JKRAramArchive::JKRAramArchive()
    : JKRArchive()
{
}

/*
 * --INFO--
 * Address:	80018958
 * Size:	0000B0
 * __ct
 */
JKRAramArchive::JKRAramArchive(long p1, JKRArchive::EMountDirection mountDirection)
    : JKRArchive(p1, EMM_Aram)
    , mMountDirection(mountDirection)
{
	if (!open(p1)) {
		return;
	} else {
		mMagicWord  = 'RARC';
		mVolumeName = mStrTable + mDirectories->mOffset; // shouldn't this be SDirEntry->mName ?
		sVolumeList.prepend(&mFileLoaderLink);
		mIsMounted = true;
	}
}

/*
 * --INFO--
 * Address:	80018A08
 * Size:	000150
 */

JKRAramArchive::~JKRAramArchive()
{
	if (mIsMounted == true) {
		if (mDataInfo) {
			SDIFileEntry* fileEntries = mFileEntries;
			for (int i = 0; i < mDataInfo->mNumFileEntries; i++) {
				if (fileEntries->mData != nullptr) {
					JKRFreeToHeap(mHeap, fileEntries->mData);
				}
				fileEntries++;
			}
			JKRFreeToHeap(mHeap, mDataInfo);
			mDataInfo = nullptr;
		}

		if (mExpandSizes) {
			JKRFree(mExpandSizes);
			mExpandSizes = nullptr;
		}
		if (mDvdFile) {
			delete mDvdFile;
		}
		if (mBlock) {
			delete mBlock;
		}

		sVolumeList.remove(&mFileLoaderLink);
		mIsMounted = false;
	}
}
// TODO: get the JKRFile dtor to generate naturally

/*
 * --INFO--
 * Address:	80018BB8
 * Size:	000334
 */
bool JKRAramArchive::open(long entryNum)
{
	mDataInfo    = nullptr;
	mDirectories = nullptr;
	mFileEntries = nullptr;
	mStrTable    = nullptr;
	mBlock       = nullptr;

	mDvdFile = new (JKRGetSystemHeap(), mMountDirection == EMD_Head ? 4 : -4) JKRDvdFile(entryNum);
	if (mDvdFile == nullptr) {
		mMountMode = EMM_Unk0;
		return 0;
	}

	// NOTE: a different struct is used here for sure, unfortunately i can't get any hits on this address, so gonna leave it like this for
	// now

	SArcDataInfo* mem = (SArcDataInfo*)JKRAllocFromSysHeap(32, -32);
	if (mem == nullptr) {
		mMountMode = EMM_Unk0;
	} else {
		JKRDvdToMainRam(entryNum, (u8*)mem, Switch_1, 32, nullptr, JKRDvdRipper::ALLOC_DIR_TOP, 0, (int*)&mCompression, nullptr);
		DCInvalidateRange(mem, 32);
		int alignment      = mMountDirection == EMD_Head ? 32 : -32;
		size_t alignedSize = ALIGN_NEXT(mem->mFileEntryOffset, 32);
		mDataInfo          = (SArcDataInfo*)JKRAllocFromHeap(mHeap, alignedSize, alignment);
		if (mDataInfo == nullptr) {
			mMountMode = EMM_Unk0;
		} else {
			JKRDvdToMainRam(entryNum, (u8*)mDataInfo, Switch_1, alignedSize, nullptr, JKRDvdRipper::ALLOC_DIR_TOP, 32, nullptr, nullptr);
			DCInvalidateRange(mDataInfo, alignedSize);

			mDirectories = (SDIDirEntry*)((u8*)mDataInfo + mDataInfo->mDirEntryOffset);
			mFileEntries = (SDIFileEntry*)((u8*)mDataInfo + mDataInfo->mFileEntryOffset);
			mStrTable    = (const char*)((u8*)mDataInfo + mDataInfo->mStrTableOffset);
			mExpandSizes = nullptr;

			u8 compressedFiles = 0; // maybe a check for if the last file is compressed?

			SDIFileEntry* fileEntry = mFileEntries;
			for (int i = 0; i < mDataInfo->mNumFileEntries; i++) {
				u8 flag = fileEntry->getFlags();
				if ((flag & 1)) {
					compressedFiles |= (flag & 0x4); // JKRARCHIVE_ATTR_COMPRESSION
				}
				fileEntry++;
			}

			if (compressedFiles != 0) {
				mExpandSizes = (u32*)JKRAllocFromHeap(mHeap, mDataInfo->mNumFileEntries << 2, abs(alignment));
				if (mExpandSizes == nullptr) {
					JKRFree(mDataInfo);
					mMountMode = EMM_Unk0;
					goto cleanup;
				}
				memset(mExpandSizes, 0, mDataInfo->mNumFileEntries << 2);
			}

			size_t aramSize = ALIGN_NEXT(mem->mStrTableLength, 32);
			mBlock          = JKRAllocFromAram(aramSize, mMountDirection == EMD_Head ? JKRAramHeap::AM_Head : JKRAramHeap::AM_Tail);
			if (mBlock == nullptr) {
				if (mDataInfo) {
					JKRFree(mDataInfo);
				}
				if (mExpandSizes) {
					JKRFree(mExpandSizes);
				}
				mMountMode = EMM_Unk0;
			} else {
				JKRDvdToAram(entryNum, mBlock->getAddress(), Switch_1, mem->mNumFileEntries + mem->mFileEntryOffset, 0, nullptr);
			}
		}
	}
cleanup:
	if (mem != nullptr) {
		JKRFreeToSysHeap(mem);
	}
	if (mMountMode == EMM_Unk0) {
		if (mDvdFile != nullptr) {
			delete mDvdFile;
		}
		return false;
	}
	return true;
}

/*
 * --INFO--
 * Address:	80018EEC
 * Size:	000124
 */
void* JKRAramArchive::fetchResource(JKRArchive::SDIFileEntry* entry, unsigned long* p2)
{
	u32 standInForP2;
	if (p2 == nullptr) {
		p2 = &standInForP2;
	}
	int sequence;
	if (!entry->getFlag04()) {
		sequence = 0;
	} else if (entry->getFlag80()) {
		sequence = 2;
	} else {
		sequence = 1;
	}
	if (entry->mData == nullptr) {
		u8* v2;
		u32 v3 = fetchResource_subroutine(entry->mDataOffset + ((int*)mBlock)[5], entry->getSize(), mHeap, sequence, &v2);
		*p2    = v3;
		if (v3 == 0) {
			return nullptr;
		}
		entry->mData = v2;
		if (sequence == 2) {
			setExpandSize(entry, *p2);
		}
	} else if (sequence == 2) {
		*p2 = getExpandSize(entry);
	} else {
		*p2 = entry->getSize();
	}
	return entry->mData;
}

/*
 * --INFO--
 * Address:	80019010
 * Size:	0000F8
 */
void* JKRAramArchive::fetchResource(void* data, u32 compressedSize, SDIFileEntry* fileEntry, u32* pSize)
{
	u32 fileSize = fileEntry->mSize;
	if (fileSize > compressedSize) {
		fileSize = compressedSize;
	}

	int compression = JKRConvertAttrToCompressionType(fileEntry->getFlags());
	if (fileEntry->mData == nullptr) {
		fileSize = fetchResource_subroutine(fileEntry->mDataOffset + mBlock->getAddress(), fileSize, (u8*)data,
		                                    ALIGN_PREV(compressedSize, 32), compression);
	} else {
		if (compression == COMPRESSION_YAZ0) {
			u32 expandSize = getExpandSize(fileEntry);
			if (expandSize != 0) {
				fileSize = expandSize;
			}
		}

		if (fileSize > compressedSize) {
			fileSize = compressedSize;
		}

		JKRHeap::copyMemory(data, fileEntry->mData, fileSize);
	}
	if (pSize != nullptr) {
		*pSize = fileSize;
	}
	return data;
}

/*
 * --INFO--
 * Address:	80019108
 * Size:	0000BC
 */
u32 JKRAramArchive::fetchResource_subroutine(u32 p1, u32 p2, u8* p3, u32 p4, int p5)
{
	u32 v1 = ALIGN_PREV(p4, 0x20);
	u32 v2 = ALIGN_NEXT(p2, 0x20);
	u32 v3;
	switch (p5) {
	case COMPRESSION_None:
		if (v2 > v1) {
			v2 = v1;
		}
		JKRAram::aramToMainRam(p1, p3, v2, Switch_0, v1, nullptr, -1, &v3);
		return v3;
	case COMPRESSION_YAY0:
	case COMPRESSION_YAZ0:
		JKRAram::aramToMainRam(p1, p3, v2, Switch_1, v1, nullptr, -1, &v3);
		return v3;
	}
	OSErrorLine(655, ":::??? bad sequence\n");
	return 0;
}

/*
 * --INFO--
 * Address:	800191C4
 * Size:	00014C
 */
u32 JKRAramArchive::fetchResource_subroutine(u32 srcAram, u32 size, JKRHeap* heap, int compression, u8** pBuf)
{
	u32 resSize;
	u32 alignedSize = ALIGN_NEXT(size, 32);

	u8* buffer;
	switch (compression) {
	case COMPRESSION_None:
		buffer = (u8*)JKRAllocFromHeap(heap, alignedSize, 32);
		JKRAramToMainRam(srcAram, buffer, alignedSize, Switch_0, alignedSize, nullptr, -1, nullptr);
		*pBuf = buffer;
		return size;
	case COMPRESSION_YAY0:
	case COMPRESSION_YAZ0:
		u8 decompBuf[64];
		u8* bufptr = (u8*)ALIGN_NEXT((u32)decompBuf, 32);
		JKRAramToMainRam(srcAram, bufptr, sizeof(decompBuf) / 2, Switch_0, 0, nullptr, -1, nullptr);

		u32 expandSize = ALIGN_NEXT(JKRDecompExpandSize(bufptr), 32);
		buffer         = (u8*)JKRAllocFromHeap(heap, expandSize, 32);
		JKRAramToMainRam(srcAram, buffer, alignedSize, Switch_1, expandSize, heap, -1, &resSize);
		*pBuf = buffer;
		return resSize;
	default:
		OSErrorLine(713, ":::??? bad sequence\n");
		return 0;
	}
}

/*
 * --INFO--
 * Address:	80019310
 * Size:	000134
 */
size_t JKRAramArchive::getExpandedResSize(const void* resource) const
{
	if (mExpandSizes == nullptr) {
		return getResSize(resource);
	}
	SDIFileEntry* fileEntry = findPtrResource(resource);
	if (fileEntry == nullptr) {
		return std::numeric_limits<size_t>::max();
	}
	if (!fileEntry->getFlag04()) {
		return getResSize(resource);
	}
	size_t expandSize = getExpandSize(fileEntry);
	if (expandSize != 0) {
		return expandSize;
	}

	u8 buf[64];
	u8* bufPtr = (u8*)ALIGN_NEXT((u32)buf, 32);

	JKRAramToMainRam(fileEntry->mDataOffset + mBlock->getAddress(), bufPtr, sizeof(buf) / 2, Switch_0, 0, nullptr, -1, nullptr);

	size_t decompExpandSize = JKRDecompExpandSize(bufPtr);
	// TODO: uhhhhh this is a const function. Why is it calling a non-const function???
	const_cast<JKRAramArchive*>(this)->setExpandSize(fileEntry, decompExpandSize);
	return decompExpandSize;
}
