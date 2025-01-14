#ifndef _GAME_ENTITIES_PELLETFRUIT_H
#define _GAME_ENTITIES_PELLETFRUIT_H

#include "types.h"
#include "Game/pelletMgr.h"
#include "Game/BasePelletMgr.h"

namespace Game {
namespace PelletFruit {
struct Object : public Pellet {
	Object()
	    : Pellet()
	{
	}

	virtual void do_onInit(CreatureInitArg*);      // _1CC
	virtual void do_update();                      // _1EC
	virtual u8 getKind() { return PELTYPE_BERRY; } // _1F4 (weak)
	virtual void changeMaterial();                 // _1F8

	// _00      = VTBL
	// _00-_458 = Pellet
	f32 mBurstTimer; // _458, seconds before rotting/bursting
};

struct Mgr : public FixedSizePelletMgr<Object> {
	Mgr();

	virtual void setupResources();                  // _44
	virtual void onCreateModel(SysShape::Model*);   // _54
	virtual char* getMgrName() { return "fruit"; }  // _58 (weak)
	virtual u8 getMgrID() { return PELTYPE_BERRY; } // _5C (weak)

	// _00      = VTABLE
	// _00-_A0  = FixedSizePelletMgr
};

extern Mgr* mgr;

} // namespace PelletFruit
} // namespace Game

#endif
