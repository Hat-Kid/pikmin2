#include "Dolphin/os.h"
#include "JSystem/JKernel/JKRDecomp.h"
#include "JSystem/JKernel/JKRDvdRipper.h"
#include "JSystem/JUtility/JUTException.h"
#include "types.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "stl/mem.h"

extern "C" {
int abs(int);
};

/*
 * --INFO--
 * Address:	8001E57C
 * Size:	0000B0
 * __ct__13JKRDvdArchiveFlQ210JKRArchive15EMountDirection
 */
JKRDvdArchive::JKRDvdArchive(long entryNum, JKRArchive::EMountDirection mountDirection)
    : JKRArchive(entryNum, EMM_Dvd)
{
	mMountDirection = mountDirection;
	if (!open(entryNum)) {
		return;
	} else {
		mMagicWord  = 'RARC';
		mVolumeName = &mStrTable[mDirectories->mOffset];
		sVolumeList.prepend(&mFileLoaderLink);
		mIsMounted = true;
	}
}

/*
 * --INFO--
 * Address:	8001E62C
 * Size:	00012C
 * __dt__13JKRDvdArchiveFv
 */
JKRDvdArchive::~JKRDvdArchive()
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

		sVolumeList.remove(&mFileLoaderLink);
		mIsMounted = false;
	}
}

/*
 * --INFO--
 * Address:	8001E758
 * Size:	0002AC
 */
bool JKRDvdArchive::open(s32 entryNum)
{
	mDataInfo    = nullptr;
	_64          = 0;
	mDirectories = nullptr;
	mFileEntries = nullptr;
	mStrTable    = nullptr;

	mDvdFile = new (JKRGetSystemHeap(), 0) JKRDvdFile(entryNum);
	if (mDvdFile == nullptr) {
		mMountMode = 0;
		return 0;
	}
	SDIFileEntry* mem = (SDIFileEntry*)JKRAllocFromSysHeap(32, 32); // NOTE: unconfirmed if this struct was used here
	if (mem == nullptr) {
		mMountMode = 0;
	} else {
		JKRDvdToMainRam(entryNum, (u8*)mem, Switch_1, 32, nullptr, JKRDvdRipper::ALLOC_DIR_TOP, 0, (int*)&mCompression, nullptr);
		DCInvalidateRange(mem, 32);
		int alignment = mMountDirection == EMD_Head ? 32 : -32;

		mDataInfo = (SArcDataInfo*)JKRAllocFromHeap(mHeap, mem->mSize, alignment);
		if (mDataInfo == nullptr) {
			mMountMode = 0;
		} else {
			JKRDvdToMainRam(entryNum, (u8*)mDataInfo, Switch_1, mem->mSize, nullptr, JKRDvdRipper::ALLOC_DIR_TOP, 32, nullptr, nullptr);
			DCInvalidateRange(mDataInfo, mem->mSize);

			mDirectories = (SDIDirEntry*)((u8*)mDataInfo + mDataInfo->mDirEntryOffset);
			mFileEntries = (SDIFileEntry*)((u8*)mDataInfo + mDataInfo->mFileEntryOffset);
			mStrTable    = (const char*)((u8*)mDataInfo + mDataInfo->mStrTableOffset);
			mExpandSizes = nullptr;

			u8 compressedFiles = 0; // maybe a check for if the last file is compressed?

			SDIFileEntry* fileEntry = mFileEntries;
			for (int i = 0; i < mDataInfo->mNumFileEntries; i++) {
				u8 flag = fileEntry->mFlag >> 24;
				if ((flag & 1)) {
					compressedFiles |= (flag & 0x4);
				}
				fileEntry++;
			}

			if (compressedFiles != 0) {
				mExpandSizes = (u32*)JKRAllocFromHeap(mHeap, mDataInfo->mNumFileEntries << 2, abs(alignment));
				if (mExpandSizes == nullptr) {
					JKRFreeToSysHeap(mDataInfo);
					mMountMode = 0;
					goto cleanup;
				}
				memset(mExpandSizes, 0, mDataInfo->mNumFileEntries << 2);
			}
			_64 = mem->mDataOffset + mem->mSize; // End of data offset?
		}
	}
cleanup:
	if (mem != nullptr) {
		JKRFreeToSysHeap(mem);
	}
	if (mMountMode == 0) {
		if (mDvdFile != nullptr) {
			delete mDvdFile;
		}
		return false;
	}
	return true;
}

/*
 * --INFO--
 * Address:	8001EA04
 * Size:	000128
 * fetchResource__13JKRDvdArchiveFPQ210JKRArchive12SDIFileEntryPUl
 */
void* JKRDvdArchive::fetchResource(JKRArchive::SDIFileEntry* entry, u32* outSize)
{
	u32 sizeptr;
	u32 size;
	u8* data;

	if (outSize == nullptr) {
		outSize = &sizeptr;
	}

	int compression = JKRConvertAttrToCompressionType((u8)(entry->mFlag >> 24));

	if (entry->mData == nullptr) {
		size = fetchResource_subroutine(mEntryNum, _64 + entry->mDataOffset, entry->mSize, mHeap, (int)compression, mCompression, &data);
		*outSize = size;
		if (size == 0) {
			return nullptr;
		}
		entry->mData = data;
		if (compression == COMPRESSION_YAZ0) {
			setExpandSize(entry, *outSize);
		}
	} else if (compression == COMPRESSION_YAZ0) {
		*outSize = getExpandSize(entry);
	} else {
		*outSize = entry->mSize;
	}

	return entry->mData;
}

/*
 * --INFO--
 * Address:	8001EB2C
 * Size:	0000FC
 * fetchResource__13JKRDvdArchiveFPvUlPQ210JKRArchive12SDIFileEntryPUl
 */
void* JKRDvdArchive::fetchResource(void* data, u32 compressedSize, JKRArchive::SDIFileEntry* entry, u32* outSize)
{
	u32 fileSize    = entry->mSize;
	u32 alignedSize = ALIGN_NEXT(fileSize, 32);
	u32 fileFlag    = entry->mFlag >> 0x18;
	int compression = JKRConvertAttrToCompressionType(fileFlag);

	if (entry->mData == nullptr) {
		fileSize = fetchResource_subroutine(mEntryNum, _64 + entry->mDataOffset, entry->mSize, (u8*)data, compressedSize & ~31, compression,
		                                    mCompression);
	} else {
		if (compression == COMPRESSION_YAZ0) {
			u32 expandSize = getExpandSize(entry);
			if (expandSize != 0) {
				fileSize = expandSize;
			}
		}

		if (fileSize > compressedSize) {
			fileSize = compressedSize;
		}

		JKRHeap::copyMemory(data, entry->mData, fileSize);
	}

	if (outSize != nullptr) {
		*outSize = fileSize;
	}
	return data;
}

/*
 * --INFO--
 * Address:	8001EC28
 * Size:	000200
 * fetchResource_subroutine__13JKRDvdArchiveFlUlUlPUcUlii
 */
u32 JKRDvdArchive::fetchResource_subroutine(s32 entryNum, u32 offset, u32 size, u8* data, u32 expandSize, int fileCompression,
                                            int archiveCompression)
{
	u32 prevAlignedSize, alignedSize;

	alignedSize     = ALIGN_NEXT(size, 32);
	prevAlignedSize = ALIGN_PREV(expandSize, 32);
	switch (archiveCompression) {
	case COMPRESSION_None: {
		switch (fileCompression) {
		case COMPRESSION_None:

			if (alignedSize > prevAlignedSize) {
				alignedSize = prevAlignedSize;
			}
			JKRDvdRipper::loadToMainRAM(entryNum, data, Switch_0, alignedSize, nullptr, JKRDvdRipper::ALLOC_DIR_TOP, offset, nullptr,
			                            nullptr);
			DCInvalidateRange(data, alignedSize);
			return alignedSize;

		case COMPRESSION_YAY0:
		case COMPRESSION_YAZ0:
			u8 buf[64];
			u8* bufPtr = (u8*)ALIGN_NEXT((u32)buf, 32);
			JKRDvdRipper::loadToMainRAM(entryNum, bufPtr, Switch_2, sizeof(buf) / 2, nullptr, JKRDvdRipper::ALLOC_DIR_TOP, offset, nullptr,
			                            nullptr);
			DCInvalidateRange(bufPtr, sizeof(buf) / 2);
			u32 expandFileSize = JKRDecompExpandSize(bufPtr);
			alignedSize        = ALIGN_NEXT(expandFileSize, 32);
			if (alignedSize > prevAlignedSize) {
				alignedSize = prevAlignedSize;
			}
			JKRDvdRipper::loadToMainRAM(entryNum, data, Switch_1, alignedSize, nullptr, JKRDvdRipper::ALLOC_DIR_TOP, offset, nullptr,
			                            nullptr);
			DCInvalidateRange(data, alignedSize);
			return expandFileSize;
		}
	}
	case COMPRESSION_YAZ0: {
		if (size > prevAlignedSize) {
			size = prevAlignedSize;
		}
		JKRDvdRipper::loadToMainRAM(entryNum, data, Switch_1, size, nullptr, JKRDvdRipper::ALLOC_DIR_TOP, offset, nullptr, nullptr);
		DCInvalidateRange(data, size);
		return size;
	}

	case COMPRESSION_YAY0: {
		OSErrorLine(649, "Sorry, not prepared for SZP archive.\n");
		return 0;
	}

	default: {
		OSErrorLine(655, ":::??? bad sequence\n");
	}
	}
	return 0;
}

/*
 * --INFO--
 * Address:	8001EE28
 * Size:	00021C
 * fetchResource_subroutine__13JKRDvdArchiveFlUlUlP7JKRHeapiiPPUc
 */
u32 JKRDvdArchive::fetchResource_subroutine(s32 entryNum, u32 offset, u32 size, JKRHeap* heap, int fileCompression, int archiveCompression,
                                            u8** pBuf)
{
	u32 alignedSize = ALIGN_NEXT(size, 32);

	u8* buffer;
	switch (archiveCompression) {
	case COMPRESSION_None: {
		switch (fileCompression) {
		case COMPRESSION_None:
			buffer = (u8*)JKRAllocFromHeap(heap, alignedSize, 32);

			JKRDvdToMainRam(entryNum, buffer, Switch_0, alignedSize, nullptr, JKRDvdRipper::ALLOC_DIR_TOP, offset, nullptr, nullptr);
			DCInvalidateRange(buffer, alignedSize);
			*pBuf = buffer;
			return alignedSize;

		case COMPRESSION_YAY0:
		case COMPRESSION_YAZ0:
			u8 decompBuf[64];
			u8* bufptr = (u8*)ALIGN_NEXT((u32)decompBuf, 32);
			JKRDvdToMainRam(entryNum, bufptr, Switch_2, sizeof(decompBuf) / 2, nullptr, JKRDvdRipper::ALLOC_DIR_TOP, offset, nullptr,
			                nullptr);
			DCInvalidateRange(bufptr, 0x20);

			alignedSize = JKRDecompExpandSize(bufptr);

			buffer = (u8*)JKRAllocFromHeap(heap, alignedSize, 32);

			JKRDvdToMainRam(entryNum, buffer, Switch_1, alignedSize, nullptr, JKRDvdRipper::ALLOC_DIR_TOP, offset, nullptr, nullptr);
			DCInvalidateRange(buffer, alignedSize);
			*pBuf = buffer;
			return alignedSize;
		}
	}
	case COMPRESSION_YAZ0: {
		buffer = (u8*)JKRAllocFromHeap(heap, alignedSize, 32);
		JKRDvdToMainRam(entryNum, buffer, Switch_1, size, nullptr, JKRDvdRipper::ALLOC_DIR_TOP, offset, nullptr, nullptr);
		DCInvalidateRange(buffer, size);
		*pBuf = buffer;
		return alignedSize;
	}

	case COMPRESSION_YAY0: {
		OSErrorLine(756, "Sorry, not prepared for SZP archive.\n");
		return 0;
	}

	default: {
		OSErrorLine(761, ":::??? bad sequence\n");
	}
	}
	return 0;
}

/*
 * --INFO--
 * Address:	8001F044
 * Size:	000144
 * getExpandedResSize__13JKRDvdArchiveCFPCv
 */
u32 JKRDvdArchive::getExpandedResSize(const void* resource) const
{
	if (mExpandSizes == 0) {
		return getResSize(resource);
	}

	SDIFileEntry* fileEntry = findPtrResource(resource);
	if (fileEntry == nullptr) {
		return 0xffffffff;
	}

	u8 flags = (fileEntry->mFlag >> 0x18);
	if ((flags & 4) == 0) { // not compressed
		return getResSize(resource);
	}

	u32 expandSize = getExpandSize(fileEntry);
	if (expandSize != 0) {
		return expandSize;
	}

	u8 buf[64];
	u8* bufPtr = (u8*)ALIGN_NEXT((u32)buf, 32);

	JKRDvdToMainRam(mEntryNum, bufPtr, Switch_2, sizeof(buf) / 2, nullptr, JKRDvdRipper::ALLOC_DIR_TOP, _64 + fileEntry->mDataOffset,
	                nullptr, nullptr);
	DCInvalidateRange(bufPtr, sizeof(buf) / 2);

	u32 decompExpandSize = JKRDecompExpandSize(bufPtr);
	const_cast<JKRDvdArchive*>(this)->setExpandSize(fileEntry, decompExpandSize);
	return decompExpandSize;
}
