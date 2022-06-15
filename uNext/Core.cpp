#include "header.h"
#include "Core.h"
#include "IMG.h"
#include "CFG.h"
#include "Text.h"
#include "SDL_mixer.h"

#include <psp2/kernel/processmgr.h>
#include <psp2/ctrl.h>
#include <psp2/touch.h>

/* ******************************************** */

Map* CCore::oMap = new Map();
bool CCore::mouseLeftPressed = false;
bool CCore::mouseRightPressed = false;
int CCore::mouseX = 0;
int CCore::mouseY = 0;
bool CCore::quitGame = false;

bool CCore::movePressed = false;
bool CCore::keyMenuPressed = false;
bool CCore::keyS = false;
bool CCore::keyW = false;
bool CCore::keyA = false;
bool CCore::keyD = false;
bool CCore::keyShift = false;
bool CCore::keyAPressed = false;
bool CCore::keyDPressed = false;

SceTouchData touchV[SCE_TOUCH_PORT_MAX_NUM];

CCore::CCore(void) {
	this->quitGame = false;
	this->iFPS = 0;
	this->iNumOfFPS = 0;
	this->lFPSTime = 0;

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO);
	
	window = SDL_CreateWindow("uMario port by Hammerill", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, CCFG::GAME_WIDTH, CCFG::GAME_HEIGHT, SDL_WINDOW_SHOWN);

	sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START);
	sceTouchEnableTouchForce(SCE_TOUCH_PORT_FRONT);

	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);

	if(window == NULL) {
		quitGame = true;
	}

	rR = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// ----- ICO
	std::string fileName = "files/images/ico.bmp";
	SDL_Surface* loadedSurface = SDL_LoadBMP(fileName.c_str());
	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 0, 255));

	SDL_SetWindowIcon(window, loadedSurface);
	SDL_FreeSurface(loadedSurface);

	mainEvent = new SDL_Event();
	// ----- ICO
	
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	
	oMap = new Map(rR);
	CCFG::getMM()->setActiveOption(rR);
	CCFG::getSMBLOGO()->setIMG("super_mario_bros", rR);

	CCFG::getMusic()->PlayMusic();

	this->keyMenuPressed = this->movePressed = this->keyS = this->keyW = this->keyA = this->keyD = this->keyShift = false;

	this->keyAPressed = this->keyDPressed = this->firstDir = false;

	this->mouseX = this->mouseY = 0;

	CCFG::keyIDA = SDLK_a;
	CCFG::keyIDS = SDLK_s;
	CCFG::keyIDD = SDLK_d;
	CCFG::keyIDSpace = SDLK_SPACE;
	CCFG::keyIDShift = SDLK_LSHIFT;
}

CCore::~CCore(void) {
	delete oMap;
	delete mainEvent;
	SDL_DestroyRenderer(rR);
	SDL_DestroyWindow(window);
}

/* ******************************************** */

void CCore::mainLoop() {
	lFPSTime = SDL_GetTicks();

	while(!quitGame && mainEvent->type != SDL_QUIT) {
		frameTime = SDL_GetTicks();
		SDL_PollEvent(mainEvent);
		SDL_RenderClear(rR);

		CCFG::getMM()->setBackgroundColor(rR);
		SDL_RenderFillRect(rR, NULL);

		Input();
		MouseInput();
		Update();
		Draw();

		/*CCFG::getText()->Draw(rR, "FPS:" + std::to_string(iNumOfFPS), CCFG::GAME_WIDTH - CCFG::getText()->getTextWidth("FPS:" + std::to_string(iNumOfFPS), 8) - 8, 5, 8);

		if(SDL_GetTicks() - 1000 >= lFPSTime) {
			lFPSTime = SDL_GetTicks();
			iNumOfFPS = iFPS;
			iFPS = 0;
		}

		++iFPS;*/

		SDL_RenderPresent(rR);
		
		if(SDL_GetTicks() - frameTime < MIN_FRAME_TIME) {
			SDL_Delay(MIN_FRAME_TIME - (SDL_GetTicks () - frameTime));
		}
	}
}

void CCore::Input() {
	switch(CCFG::getMM()->getViewID()) {
		case 2: case 7:
			if(!oMap->getInEvent()) {
				InputPlayer();
			} else {
				resetMove();
			}
			break;
		default:
			InputMenu();
			break;
	}
}

void CCore::InputMenu() {
	SceCtrlData ctrl;
	sceCtrlPeekBufferPositive(0, &ctrl, 1);

	if (!(ctrl.buttons & (	SCE_CTRL_DOWN | 
							SCE_CTRL_UP | 
							SCE_CTRL_CROSS | 
							SCE_CTRL_START | 
							SCE_CTRL_SELECT | 
							SCE_CTRL_CIRCLE | 
							SCE_CTRL_LEFT | 
							SCE_CTRL_RIGHT))) 
	{
		keyMenuPressed = false;
	}

	if (ctrl.buttons & SCE_CTRL_DOWN)
	{
		if(!keyMenuPressed) {
			CCFG::getMM()->keyPressed(2);
			keyMenuPressed = true;
		}
	}
	if (ctrl.buttons & SCE_CTRL_UP)
	{
		if(!keyMenuPressed) {
			CCFG::getMM()->keyPressed(0);
			keyMenuPressed = true;
		}
	}
	if (ctrl.buttons & SCE_CTRL_CROSS)
	{
		if(!keyMenuPressed) {
			CCFG::getMM()->enter();
			keyMenuPressed = true;
		}
	}
	if (ctrl.buttons & (SCE_CTRL_START | SCE_CTRL_SELECT | SCE_CTRL_CIRCLE))
	{
		if(!keyMenuPressed) {
			CCFG::getMM()->escape();
			keyMenuPressed = true;
		}
	}
	if (ctrl.buttons & SCE_CTRL_LEFT)
	{
		if(!keyMenuPressed) {
			CCFG::getMM()->keyPressed(3);
			keyMenuPressed = true;
		}
	}
	if (ctrl.buttons & SCE_CTRL_RIGHT)
	{
		if(!keyMenuPressed) {
			CCFG::getMM()->keyPressed(1);
			keyMenuPressed = true;
		}
	}
}

void CCore::InputPlayer() {
	SceCtrlData ctrl;
	sceCtrlPeekBufferPositive(0, &ctrl, 1);

	if(mainEvent->type == SDL_WINDOWEVENT) {
		switch(mainEvent->window.event) {
			case SDL_WINDOWEVENT_FOCUS_LOST:
				CCFG::getMM()->resetActiveOptionID(CCFG::getMM()->ePasue);
				CCFG::getMM()->setViewID(CCFG::getMM()->ePasue);
				CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cPASUE);
				CCFG::getMusic()->PauseMusic();
				break;
		}
	}

	if (ctrl.buttons & SCE_CTRL_RIGHT)
	{
		keyDPressed = true;
		if (!keyAPressed) firstDir = true;
	}
	else
	{
		keyDPressed = false;
		if (firstDir) firstDir = false;
	}

	if (ctrl.buttons & SCE_CTRL_DOWN)
	{
		if(!keyS)
		{
			keyS = true;
			if(!oMap->getUnderWater() && !oMap->getPlayer()->getInLevelAnimation()) oMap->getPlayer()->setSquat(true);
		}
	}
	else
	{
		oMap->getPlayer()->setSquat(false);
		keyS = false;
	}

	if (ctrl.buttons & SCE_CTRL_LEFT)
	{
		keyAPressed = true;
		if (!firstDir) firstDir = false;
	}
	else
	{
		keyAPressed = false;
		if (!keyDPressed) firstDir = true;
	}

	if (ctrl.buttons & SCE_CTRL_CROSS)
	{
		if(!CCFG::keySpace) 
		{
			oMap->getPlayer()->jump();
			CCFG::keySpace = true;
		}
	}
	else
	{
		CCFG::keySpace = false;
	}

	if (ctrl.buttons & SCE_CTRL_SQUARE)
	{
		if(!keyShift) 
		{
			oMap->getPlayer()->startRun();
			keyShift = true;
		}
	}
	else
	{
		if(keyShift) 
		{
			oMap->getPlayer()->resetRun();
			keyShift = false;
		}
	}

	if (ctrl.buttons & (SCE_CTRL_START | SCE_CTRL_SELECT))
	{
		if(!keyMenuPressed && CCFG::getMM()->getViewID() == CCFG::getMM()->eGame)
		{
				CCFG::getMM()->resetActiveOptionID(CCFG::getMM()->ePasue);
				CCFG::getMM()->setViewID(CCFG::getMM()->ePasue);
				CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cPASUE);
				CCFG::getMusic()->PauseMusic();
				keyMenuPressed = true;
		}
	}
	else
	{
		keyMenuPressed = false;
	}

	if(keyAPressed) {
		if(!oMap->getPlayer()->getMove() && firstDir == false && !oMap->getPlayer()->getChangeMoveDirection() && !oMap->getPlayer()->getSquat()) {
			oMap->getPlayer()->startMove();
			oMap->getPlayer()->setMoveDirection(false);
		} else if(!keyDPressed && oMap->getPlayer()->getMoveSpeed() > 0 && firstDir != oMap->getPlayer()->getMoveDirection()) {
			oMap->getPlayer()->setChangeMoveDirection();
		}
	}

	if(keyDPressed) {
		if(!oMap->getPlayer()->getMove() && firstDir == true && !oMap->getPlayer()->getChangeMoveDirection() && !oMap->getPlayer()->getSquat()) {
			oMap->getPlayer()->startMove();
			oMap->getPlayer()->setMoveDirection(true);
		} else if(!keyAPressed && oMap->getPlayer()->getMoveSpeed() > 0 && firstDir != oMap->getPlayer()->getMoveDirection()) {
			oMap->getPlayer()->setChangeMoveDirection();
		}
	}

	if(oMap->getPlayer()->getMove() && !keyAPressed && !keyDPressed) {
		oMap->getPlayer()->resetMove();
	}
}

void CCore::MouseInput() {
	sceTouchPeek(0, &touchV[0], 1);
	if (touchV[0].reportNum > 0)
	{
		mouseLeftPressed = true;

		mouseX = touchV[0].report[0].x/2;
		mouseY = touchV[0].report[0].y/2;
	}
	else
	{
		mouseLeftPressed = false;
	}
}

void CCore::resetKeys() {
	movePressed = keyMenuPressed = keyS = keyW = keyA = keyD = CCFG::keySpace = keyShift = keyAPressed = keyDPressed = false;
}

void CCore::Update() {
	CCFG::getMM()->Update();
}


void CCore::Draw() {
	CCFG::getMM()->Draw(rR);
}

/* ******************************************** */

void CCore::resetMove() {
	this->keyAPressed = this->keyDPressed = false;
}

Map* CCore::getMap() {
	return oMap;
}