#ifndef _JSYSTEM_J3D_J3DMATERIALFACTORY_H
#define _JSYSTEM_J3D_J3DMATERIALFACTORY_H

#include "Dolphin/gx.h"
#include "JSystem/J3D/J3DFileBlock.h"
#include "JSystem/J3D/J3DMaterial.h"
#include "types.h"

struct J3DAlphaCompInfo {
	u8 _00[0x5]; // _00
};

struct J3DBlendInfo {
	u8 _00; // _00
	u8 _01; // _01
	u8 _02; // _02
	u8 _03; // _03
};

struct J3DColorChanInfo {
	u8 _00[0x8]; // _00
};

struct J3DCurrentMtxInfo { };

struct J3DDisplayListInit { };

struct J3DFogInfo {
	u8 _00;       // _00
	u8 _01;       // _01
	u16 _02;      // _02
	float _04;    // _04
	float _08;    // _08
	float _0C;    // _0C
	float _10;    // _10
	u8 _14;       // _14
	u8 _15;       // _15
	u8 _16;       // _16
	u16 _18[0xA]; // _18
};

struct J3DIndInitData {
	u8 _00[0x88]; // _00
};

struct J3DLightInfo { };

struct J3DPatchingInfo { };

struct J3DTevOrderInfo {
	u8 m_texCoordID; // _00
	u8 m_texMapID;   // _01
	u8 m_channelID;  // _02
};

struct J3DTevStageInfo {
	u8 _00[0x13]; // _00
};

struct J3DTevSwapModeInfo {
	u32 _00;
};

struct J3DTevSwapModeTableInfo { };

struct J3DTexCoordInfo {
	u8 _00; // _00
	u8 _01; // _01
	u8 _02; // _02
};

struct J3DTexCoord2Info { };

struct J3DTexMtxInfo {
	u8 _00;    // _00
	u8 _01;    // _01
	float _04; // _04
	float _08; // _08
	float _0C; // _0C
	float _10; // _10
	float _14; // _14
	u16 _18;   // _18
	float _1C; // _1C
	float _20; // _20
	float _24; // _24
	float _28; // _28
	float _2C; // _2C
	float _30; // _30
	float _34; // _34
	float _38; // _38
	float _3C; // _3C
	float _40; // _40
	float _44; // _44
	float _48; // _48
	float _4C; // _4C
	float _50; // _50
	float _54; // _54
	float _58; // _58
	float _5C; // _5C
	float _60; // _60
};

struct J3DZModeInfo {
	u8 _00; // _00
	u8 _01; // _01
	u8 _02; // _02
};

/**
 * @size{0x14C}
 */
struct J3DMaterialInitData {
	u8 _00[0x14C]; // _00
};

struct J3DMaterialFactory {
	enum MaterialType { NORMAL = 0, LOCKED, PATCHED };

	J3DMaterialFactory(const J3DMaterialBlock& block);
	J3DMaterialFactory(const J3DMaterialDLBlock& block);

	u32 calcSize(J3DMaterial*, MaterialType, int, u32) const;
	u32 calcSizeNormalMaterial(J3DMaterial*, int, u32) const;
	u32 calcSizePatchedMaterial(J3DMaterial*, int, u32) const;
	u32 calcSizeLockedMaterial(J3DMaterial*, int, u32) const;
	u32 countUniqueMaterials();
	J3DMaterial* create(J3DMaterial*, MaterialType, int, u32) const;
	J3DMaterial* createNormalMaterial(J3DMaterial*, int, u32) const;
	J3DLockedMaterial* createLockedMaterial(J3DMaterial*, int, u32) const;
	J3DPatchedMaterial* createPatchedMaterial(J3DMaterial*, int, unsigned long) const;
	void newAlphaComp(int) const;
	void newAmbColor(int, int) const;
	void newBlend(int) const;
	void newColorChan(int, int) const;
	void newColorChanNum(int) const;
	void newCullMode(int) const;
	void newDither(int) const;
	void newFog(int) const;
	void newIndTexCoordScale(int, int) const;
	void newIndTexMtx(int, int) const;
	void newIndTexOrder(int, int) const;
	void newIndTexStageNum(int) const;
	void newIndTevStage(int, int) const;
	void newMatColor(int, int) const;
	void newNBTScale(int) const;
	void newTevColor(int, int) const;
	void newTevKColor(int, int) const;
	void newTevOrder(int, int) const;
	void newTevStage(int, int) const;
	void newTevStageNum(int) const;
	void newTevSwapModeTable(int, int) const;
	void newTexCoord(int, int) const;
	void newTexGenNum(int) const;
	void newTexMtx(int, int) const;
	void newTexNo(int, int) const;
	void newZCompLoc(int) const;
	void newZMode(int) const;
	void modifyPatchedCurrentMtx(J3DMaterial*, int) const;

	u8 _00[4]; // _00

	J3DMaterialInitData* _04;     // _04
	u16* _08;                     // _08
	J3DIndInitData* _0C;          // _0C
	GXColor* _10;                 // _10
	u8* _14;                      // _14
	J3DColorChanInfo* _18;        // _18
	GXColor* _1C;                 // _1C
	J3DLightInfo* _20;            // _20
	u8* _24;                      // _24
	J3DTexCoordInfo* _28;         // _28
	J3DTexCoord2Info* _2C;        // _2C
	J3DTexMtxInfo* _30;           // _30
	J3DTexMtxInfo* _34;           // _34
	u16* _38;                     // _38
	_GXCullMode* _3C;             // _3C
	J3DTevOrderInfo* _40;         // _40
	GXColorS10* _44;              // _44
	GXColor* _48;                 // _48
	u8* _4C;                      // _4C
	J3DTevStageInfo* _50;         // _50
	J3DTevSwapModeInfo* _54;      // _54
	J3DTevSwapModeTableInfo* _58; // _58
	J3DFogInfo* _5C;              // _5C
	J3DAlphaCompInfo* _60;        // _60
	J3DBlendInfo* _64;            // _64
	J3DZModeInfo* _68;            // _68
	u8* _6C;                      // _6C
	u8* _70;                      // _70
	J3DNBTScaleInfo* _74;         // _74
	J3DDisplayListInit* _78;      // _78
	J3DPatchingInfo* _7C;         // _7C
	J3DCurrentMtxInfo* _80;       // _80
	u8* _84;                      // _84
};

/**
 * @size{0x138}
 */
struct J3DMaterialInitData_v21 {
	u8 _00[0x138]; // _00
};

struct J3DMaterialFactory_v21 {
	u8 _00[4]; // _00

	J3DMaterialInitData_v21* _04; // _04
	void* _08;                    // _08
	void* _0C;                    // _0C
	void* _10;                    // _10
	void* _14;                    // _14
	void* _18;                    // _18
	void* _1C;                    // _1C
	void* _20;                    // _20
	void* _24;                    // _24
	void* _28;                    // _28
	void* _2C;                    // _2C
	void* _30;                    // _30
	void* _34;                    // _34
	void* _38;                    // _38
	void* _3C;                    // _3C
	void* _40;                    // _40
	void* _44;                    // _44
	void* _48;                    // _48
	void* _4C;                    // _4C
	void* _50;                    // _50
	void* _54;                    // _54
	void* _58;                    // _58
	void* _5C;                    // _5C
	void* _60;                    // _60
	void* _64;                    // _64
	void* _68;                    // _68
	void* _6C;                    // _6C
	void* _70;                    // _70
	void* _74;                    // _74
	void* _78;                    // _78
	void* _7C;                    // _7C
	void* _80;                    // _80
	void* _84;                    // _84
};

#endif