#ifndef _PSSYSTEM_SEQTRACK_H
#define _PSSYSTEM_SEQTRACK_H

#include "types.h"
#include "PSSystem/Task.h"
#include "PSSystem/Director.h"

struct JASTrack;
namespace JADUtility {
struct DataMgrNode;
} // namespace JADUtility

namespace PSSystem {

/**
 * @size 0x1
 */
struct BeatMgr {
	void proc();

	u8 _00; // _00 - unknown, but size 0x1 (or 0x2?)
}

/**
 * @size 0x2C
 */
struct SeqTrackBase {
	virtual void update();        // _08
	virtual void init(JASTrack*); // _0C
	virtual void onStopSeq() = 0; // _10

	void getTaskEntryList();

	TaskEntryMgr m_taskEntryMgr; // _04
};

/**
 * @size 0x2C8
 */
struct SeqTrackRoot : public SeqTrackBase {
	SeqTrackRoot();

	virtual void update();        // _08 (weak)
	virtual void init(JASTrack*); // _0C
	virtual void onStopSeq();     // _10
	virtual void beatUpdate();    // _14
	virtual void onBeatTop();     // _18 (weak)

	void initSwingRatio();
	void pitchModulation(float, float, u32, DirectorBase*);
	void tempoChange(float, u32, DirectorBase*);

	// _00      = VTABLE
	// _04-_2C  = SeqBase
	u16 _2C;                   // _2C
	short _2E;                 // _2E
	u16 _30;                   // _30
	u16 _32;                   // _32
	float _34;                 // _34
	u32 _38;                   // _38 - unknown
	BeatMgr m_beatMgr;         // _3C
	u8 _3D;                    // _3D - possibly padding or part of BeatMgr
	short _3E;                 // _3E
	TaskEntry_Tempo _40;       // _40
	FlagWaitTask _AC;          // _AC
	OuterParamTask _CC;        // _CC
	TaskEntry_OuterParam _100; // _100 - probably
	TaskEntry_PitMod _16C;     // _16C
	TaskEntry _20C;            // _20C - probably
	BankRandTask _244;         // _244
	FlagWaitTask _274;         // _274
	BankRandTask _294;         // _294
	DataMgrNode* _2C4;         // _2C4
};

struct SeqTrackRoot_JumpBgm : public SeqTrackRoot {
	virtual void onBeatTop(); // _18
};

/**
 * @size 0x27C
 */
struct SeqTrackChild : public SeqTrackBase {
	SeqTrackChild(const SeqTrackRoot&);

	virtual void onStopSeq(); // _10

	void muteOffAndFadeIn(float, u32, DirectorBase*);
	void fadeoutAndMute(u32, DirectorBase*);
	void fade(float, u32, DirectorBase*);
	void setIdMask(u8, DirectorBase*);

	// _00      = VTABLE
	// _04-_2C  = SeqBase
	TaskEntry_MuteVolume _2C;    // _2C
	FlagWaitTask _B8;            // _B8
	OuterParamTask _D8;          // _D8
	MuteTask _10C;               // _10C
	TaskEntry_MuteOnVolume _12C; // _12C
	TaskEntry_OuterParam _1B8;   // _1B8
	TaskEntry_IdMask _224;       // _224
	u8 _27[0x3];                 // _279 - maybe padding
};

} // namespace PSSystem

#endif