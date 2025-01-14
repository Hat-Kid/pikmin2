#include "ebi/Screen/TTitleMenu.h"
#include "ebi/E2DGraph.h"
#include "Game/GameConfig.h"
#include "Graphics.h"
#include "PSSystem/PSSystemIF.h"
#include "SoundID.h"
#include "Dolphin/rand.h"

static const char name[] = "ebiScreenTitleMenu";

namespace ebi {
namespace Screen {

/*
 * --INFO--
 * Address:	803D9CE0
 * Size:	000928
 */
void TTitleMenu::doSetArchive(JKRArchive* arc)
{
	sys->heapStatusStart("TTitleMenu::setArchive", nullptr);

	mMainScreen = new P2DScreen::Mgr_tuning;
	mMainScreen->set("title_menu_6.blo", 0x1100000, arc);

	mCategoryPanes[0] = E2DScreen_searchAssert(mMainScreen, 'Ngame');
	mCategoryPanes[1] = E2DScreen_searchAssert(mMainScreen, 'Nvs');
	mCategoryPanes[2] = E2DScreen_searchAssert(mMainScreen, 'Nchallen');
	mCategoryPanes[3] = E2DScreen_searchAssert(mMainScreen, 'Noption');
	mCategoryPanes[4] = E2DScreen_searchAssert(mMainScreen, 'Nhiscore');
	mCategoryPanes[5] = E2DScreen_searchAssert(mMainScreen, 'Nomake');

	mPikiCounts[0] = mMainScreen->gather(mPikaPanes[0], 'Ppiga00', 'Ppiga99', 100);
	P2ASSERTLINE(44, mPikiCounts[0] < 100);

	mPikiCounts[1] = mMainScreen->gather(mPikaPanes[1], 'Ppivs00', 'Ppivs99', 100);
	P2ASSERTLINE(50, mPikiCounts[1] < 100);

	mPikiCounts[2] = mMainScreen->gather(mPikaPanes[2], 'Ppich00', 'Ppich99', 100);
	P2ASSERTLINE(56, mPikiCounts[2] < 100);

	mPikiCounts[3] = mMainScreen->gather(mPikaPanes[3], 'Ppiop00', 'Ppiop99', 100);
	P2ASSERTLINE(62, mPikiCounts[3] < 100);

	mPikiCounts[4] = mMainScreen->gather(mPikaPanes[4], 'Ppihs00', 'Ppihs99', 100);
	P2ASSERTLINE(68, mPikiCounts[4] < 100);

	mPikiCounts[5] = mMainScreen->gather(mPikaPanes[5], 'Ppiom00', 'Ppiom99', 100);
	P2ASSERTLINE(74, mPikiCounts[5] < 100);

	J2DPane* panelist1[6];
	J2DPane* panelist2[6];

	panelist1[0] = E2DScreen_searchAssert(mMainScreen, 'Pgail');
	panelist1[1] = E2DScreen_searchAssert(mMainScreen, 'Pvsil');
	panelist1[2] = E2DScreen_searchAssert(mMainScreen, 'Pchil');
	panelist1[3] = E2DScreen_searchAssert(mMainScreen, 'Popil');
	panelist1[4] = E2DScreen_searchAssert(mMainScreen, 'Phiil');
	panelist1[5] = E2DScreen_searchAssert(mMainScreen, 'Pomil');

	panelist2[0] = E2DScreen_searchAssert(mMainScreen, 'Pgair');
	panelist2[1] = E2DScreen_searchAssert(mMainScreen, 'Pvsir');
	panelist2[2] = E2DScreen_searchAssert(mMainScreen, 'Pchir');
	panelist2[3] = E2DScreen_searchAssert(mMainScreen, 'Popir');
	panelist2[4] = E2DScreen_searchAssert(mMainScreen, 'Phiir');
	panelist2[5] = E2DScreen_searchAssert(mMainScreen, 'Pomir');

	E2DPane_setTreeCallBackMessage(mMainScreen, mMainScreen);
	mMainScreen->addCallBackPane(mMainScreen, &mAnim6);
	mMainScreen->addCallBackPane(mMainScreen, &mAnim7);

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 6; j++) {
			mMainScreen->addCallBackPane(mCategoryPanes[j], &mAnims1[i][j]);
		}
	}

	for (int i = 0; i < 6; i++) {
		mMainScreen->addCallBackPane(panelist1[i], &mAnims2[i]);
		mMainScreen->addCallBackPane(panelist2[i], &mAnims3[i]);
		mMainScreen->addCallBackPane(panelist1[i], &mAnims4[i]);
		mMainScreen->addCallBackPane(panelist2[i], &mAnims5[i]);
	}

	mMainScreen->addCallBackPane(mMainScreen, &mAnim8);

	mAnims1[0][0].loadAnm("title_menu_5.bck", arc, 100, 221);
	mAnims1[0][1].loadAnm("title_menu_5.bck", arc, 295, 421);
	mAnims1[0][2].loadAnm("title_menu_5.bck", arc, 495, 621);
	mAnims1[0][3].loadAnm("title_menu_5.bck", arc, 695, 821);
	mAnims1[0][4].loadAnm("title_menu_5.bck", arc, 895, 1021);
	mAnims1[0][5].loadAnm("title_menu_5.bck", arc, 1095, 1221);
	mAnims1[1][0].loadAnm("title_menu_6.bck", arc, 100, 221);
	mAnims1[1][1].loadAnm("title_menu_6.bck", arc, 295, 421);
	mAnims1[1][2].loadAnm("title_menu_6.bck", arc, 495, 621);
	mAnims1[1][3].loadAnm("title_menu_6.bck", arc, 695, 821);
	mAnims1[1][4].loadAnm("title_menu_6.bck", arc, 895, 1021);
	mAnims1[1][5].loadAnm("title_menu_6.bck", arc, 1095, 1221);

	mAnims2[0].loadAnm("title_menu_6.bck", arc, 95, 109);
	mAnims4[0].loadAnm("title_menu_6.bck", arc, 110, 229);
	mAnims2[1].loadAnm("title_menu_6.bck", arc, 295, 309);
	mAnims4[1].loadAnm("title_menu_6.bck", arc, 310, 429);
	mAnims2[2].loadAnm("title_menu_6.bck", arc, 494, 509);
	mAnims4[2].loadAnm("title_menu_6.bck", arc, 510, 629);
	mAnims2[3].loadAnm("title_menu_6.bck", arc, 694, 709);
	mAnims4[3].loadAnm("title_menu_6.bck", arc, 710, 829);
	mAnims2[4].loadAnm("title_menu_6.bck", arc, 894, 909);
	mAnims4[4].loadAnm("title_menu_6.bck", arc, 910, 1029);
	mAnims2[5].loadAnm("title_menu_6.bck", arc, 1094, 1109);
	mAnims4[5].loadAnm("title_menu_6.bck", arc, 1110, 1229);

	mAnims3[0].loadAnm("title_menu_6.bck", arc, 95, 109);
	mAnims5[0].loadAnm("title_menu_6.bck", arc, 110, 229);
	mAnims3[1].loadAnm("title_menu_6.bck", arc, 295, 309);
	mAnims5[1].loadAnm("title_menu_6.bck", arc, 310, 429);
	mAnims3[2].loadAnm("title_menu_6.bck", arc, 494, 509);
	mAnims5[2].loadAnm("title_menu_6.bck", arc, 510, 629);
	mAnims3[3].loadAnm("title_menu_6.bck", arc, 694, 709);
	mAnims5[3].loadAnm("title_menu_6.bck", arc, 710, 829);
	mAnims3[4].loadAnm("title_menu_6.bck", arc, 894, 909);
	mAnims5[4].loadAnm("title_menu_6.bck", arc, 910, 1029);
	mAnims3[5].loadAnm("title_menu_6.bck", arc, 1094, 1109);
	mAnims5[5].loadAnm("title_menu_6.bck", arc, 1110, 1229);

	mAnim6.loadAnm("title_menu_6.bck", arc, 0, 100);
	mAnim7.loadAnm("title_menu_5.bck", arc, 0, 100);
	E2DPane_setTreeInfluencedAlpha(mMainScreen, true);

	for (int i = 0; i < 6; i++) {
		mObjIcon[i].mAnimA  = &mAnims2[i];
		mObjIcon[i].mAnimB  = &mAnims4[i];
		mObjIcon[i].mStatus = 0;

		mObjIcon2[i].mAnimA  = &mAnims3[i];
		mObjIcon2[i].mAnimB  = &mAnims5[i];
		mObjIcon2[i].mStatus = 0;
	}

	sys->heapStatusEnd("TTitleMenu::setArchive");
}

/*
 * --INFO--
 * Address:	803DA608
 * Size:	00021C
 */
void TTitleMenu::doOpenScreen(ArgOpen* arg)
{

	P2ASSERTLINE(197, arg);

	ArgOpenTitleMenu* sarg = static_cast<ArgOpenTitleMenu*>(arg);
	mState                 = sarg->_04;
	mSelectID              = sarg->mSelectID;
	P2ASSERTBOUNDSLINE(203, 0, mState, 2);

	E2DPane_setTreeShow(mMainScreen);

	// When E3 mode is enabled, only two menu options exist, vs and challenge mode (I think)
	if (Game::gGameConfig.mParms.mE3version.mData) {
		for (int i = 0; i < 6; i++) {
			if (i != 1 && i != 2) {
				mCategoryPanes[i]->setAlpha(0);
			}
		}
	}
	if (Game::gGameConfig.mParms.mE3version.mData) {
		mPad.init(mController, 1, 2, &mSelectID, EUTPadInterface_countNum::MODE_DOWNUP, 0.66f, 0.15f);
	} else {
		mPad.init(mController, 0, 5, &mSelectID, EUTPadInterface_countNum::MODE_DOWNUP, 0.66f, 0.15f);
	}

	mMainScreen->clearAnmTransform();

	switch (mState) {
	case 0:
		mCategoryPanes[2]->hide();
		mAnim6.stop();
		mAnim7.play(sys->mDeltaTime * 60.0f, J3DAA_UNKNOWN_0, true);
		break;
	case 1:
		mAnim6.play(sys->mDeltaTime * 60.0f, J3DAA_UNKNOWN_0, true);
		mAnim7.stop();
		break;
	}

	mDecidedMenuOption = false;
	mDoCloseMenu       = false;

	mMainScreen->setAlpha(255);
}

/*
 * --INFO--
 * Address:	803DA824
 * Size:	0001C4
 */
void TTitleMenu::doInitWaitState()
{
	switch (mState) {
	case 0:
		mAnim7.setEndFrame();
		for (int i = 0; i < 6; i++) {
			mAnims1[1][i].stop();
		}
		break;
	case 1:
		mAnim6.setEndFrame();
		for (int i = 0; i < 6; i++) {
			mAnims1[0][i].stop();
		}
		break;
	}

	mAnims1[mState][mSelectID].play(sys->mDeltaTime * 60.0f, J3DAA_UNKNOWN_0, true);
	showPika_(mSelectID);

	for (int i = 0; i < 6; i++) {
		mObjIcon[i].mStatus = 0;
		mObjIcon[i].mAnimA->setStartFrame();
		mObjIcon2[i].mStatus = 0;
		mObjIcon2[i].mAnimA->setStartFrame();
	}

	mObjIcon[mSelectID].start();
	mObjIcon2[mSelectID].start();

	u32 count            = 30.0f / sys->mDeltaTime;
	mMenuCloseCounter    = count;
	mMenuCloseCounterMax = count;
}

/*
 * --INFO--
 * Address:	803DA9E8
 * Size:	000070
 */
void TTitleMenu::doCloseScreen(ArgClose*)
{
	if (mDoCloseMenu) {
		u32 count            = 1.0f / sys->mDeltaTime;
		mMenuCloseCounter    = count;
		mMenuCloseCounterMax = count;
	} else {
		u32 count            = 0.2f / sys->mDeltaTime;
		mMenuCloseCounter    = count;
		mMenuCloseCounterMax = count;
	}
}

/*
 * --INFO--
 * Address:	803DAA58
 * Size:	000084
 */
bool TTitleMenu::doUpdateStateOpen()
{
	mMainScreen->update();
	E2DCallBack_AnmBase* anm;
	switch (mState) {
	case 0:
		anm = &mAnim7;
		break;
	case 1:
		anm = &mAnim6;
		break;
	}
	return u8(anm->isFinish() != 0);
}

/*
 * --INFO--
 * Address:	803DAADC
 * Size:	000470
 */
bool TTitleMenu::doUpdateStateWait()
{
	if (mMenuCloseCounter) {
		mMenuCloseCounter--;
	}
	mMainScreen->update();

	for (int i = 0; i < 6; i++) {
		mObjIcon[i].update();
		mObjIcon2[i].update();
	}

	mPad.update();
	// change main menu selection
	if (mPad._0D) {
		int id = mPad.mLastIndex;
		// skip over challenge mode if its not unlocked
		if (mState == 0 && mSelectID == 2) {
			if (id < 2) {
				mSelectID++;
			} else {
				mSelectID--;
			}
		}
		mAnims1[mState][mSelectID].play(sys->mDeltaTime * 60.0f, J3DAA_UNKNOWN_0, true);
		showPika_(mSelectID);
		mObjIcon[mSelectID].start();
		mObjIcon2[mSelectID].start();

		mObjIcon[id].stop();
		mObjIcon2[id].stop();
		PSSystem::spSysIF->playSystemSe(PSSE_SY_MENU_CURSOR, 0);
		u32 count            = 30.0f / sys->mDeltaTime;
		mMenuCloseCounter    = count;
		mMenuCloseCounterMax = count;
	}
	if (mAnims1[mState][mSelectID].isFinish()) {
		if (randFloat() < 0.2f) {
			showPika_(mSelectID);
		} else {
			hidePika_(mSelectID);
		}
		mAnims1[mState][mSelectID].play(sys->mDeltaTime * 60.0f, J3DAA_UNKNOWN_0, true);
	}

	if (mController->mButton.mButtonDown & Controller::PRESS_A || mController->mButton.mButtonDown & Controller::PRESS_START) {
		mDecidedMenuOption = true;
		PSSystem::spSysIF->playSystemSe(PSSE_SY_MENU_DECIDE, 0);
		return true;
	} else if (mController->mButton.mButtonDown & Controller::PRESS_B) {
		mDoCloseMenu = true;
		PSSystem::spSysIF->playSystemSe(PSSE_SY_MENU_CANCEL, 0);
		return true;
	} else {
		// force close menu after 30 seconds
		if (!mMenuCloseCounter) {
			mDoCloseMenu = true;
			PSSystem::spSysIF->playSystemSe(PSSE_SY_MENU_CANCEL, 0);
			return true;
		} else {
			return false;
		}
	}
}

/*
 * --INFO--
 * Address:	803DAF4C
 * Size:	0000D4
 */
bool TTitleMenu::doUpdateStateClose()
{
	mMainScreen->update();

	if (mMenuCloseCounter) {
		mMenuCloseCounter--;
	}

	f32 alpha;
	if (mMenuCloseCounterMax) {
		alpha = (f32)mMenuCloseCounter / (f32)mMenuCloseCounterMax;
	} else {
		alpha = 0.0f;
	}

	mMainScreen->setAlpha(alpha * 255.0f);

	if (!mMenuCloseCounter) {
		return true;
	}
	return false;
}

/*
 * --INFO--
 * Address:	803DB020
 * Size:	000074
 */
void TTitleMenu::doDraw()
{
	Graphics* gfx       = sys->mGfx;
	J2DPerspGraph* graf = &gfx->mPerspGraph;

	graf->setPort();
	mMainScreen->draw(*gfx, *graf);
}

/*
 * --INFO--
 * Address:	803DB094
 * Size:	000008
 */
void TTitleMenu::setController(Controller* a1) { mController = a1; }

/*
 * --INFO--
 * Address:	803DB09C
 * Size:	000060
 */
bool TTitleMenu::openMenuSet(ArgOpen* arg)
{
	if (openScreen(arg)) {
		doInitWaitState();
		return true;
	}
	return false;
}

/*
 * --INFO--
 * Address:	803DB0FC
 * Size:	000008
 */
bool TTitleMenu::isDecide() { return mDecidedMenuOption; }

/*
 * --INFO--
 * Address:	803DB104
 * Size:	000008
 */
bool TTitleMenu::isCancel() { return mDoCloseMenu; }

/*
 * --INFO--
 * Address:	803DB10C
 * Size:	00003C
 */
void TTitleMenu::showPika_(long id)
{
	for (int i = 0; i < mPikiCounts[id]; i++) {
		mPikaPanes[id][i]->show();
	}
}

/*
 * --INFO--
 * Address:	803DB148
 * Size:	00003C
 */
void TTitleMenu::hidePika_(long id)
{
	for (int i = 0; i < mPikiCounts[id]; i++) {
		mPikaPanes[id][i]->hide();
	}
}

} // namespace Screen
} // namespace ebi
