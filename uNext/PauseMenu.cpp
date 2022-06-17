#include "PauseMenu.h"
#include "CFG.h"
#include "Core.h"

/* ******************************************** */

PauseMenu::PauseMenu(void) {
	rPause.x = 220 + 80; //+vita width/2
	rPause.y = 140 + (544-448)/2; //+vita height/2
	rPause.w = 360;
	rPause.h = 142;

	this->lMO.push_back(new MenuOption("RESUME", 0, 156 + (544-448)/2));
	this->lMO.push_back(new MenuOption("OPTIONS", 0, 180 + (544-448)/2));

	this->lMO.push_back(new MenuOption("QUIT TO MENU", 0, 228 + (544-448)/2));
	this->lMO.push_back(new MenuOption("QUIT TO LIVEAREA", 0, 252 + (544-448)/2));

	this->numOfMenuOptions = lMO.size();
}

PauseMenu::~PauseMenu(void) {

}

/* ******************************************** */

void PauseMenu::Update() {

}

void PauseMenu::Draw(SDL_Renderer* rR) {
	SDL_SetRenderDrawBlendMode(rR, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(rR, 4, 4, 4, 235);
	SDL_RenderFillRect(rR, &rPause);
	SDL_SetRenderDrawColor(rR, 255, 255, 255, 255);
	rPause.x += 1;
	rPause.y += 1;
	rPause.h -= 2;
	rPause.w -= 2;
	SDL_RenderDrawRect(rR, &rPause);
	rPause.x -= 1;
	rPause.y -= 1;
	rPause.h += 2;
	rPause.w += 2;

	for(unsigned int i = 0; i < lMO.size(); i++) {
		if(i == activeMenuOption) {
			CCFG::getText()->DrawCenterX(rR, lMO[i]->getText(), lMO[i]->getYPos(), 16, 255, 255, 255);
		} else {
			CCFG::getText()->DrawCenterX(rR, lMO[i]->getText(), lMO[i]->getYPos(), 16, 90, 90, 90);
		}
	}

	SDL_SetRenderDrawBlendMode(rR, SDL_BLENDMODE_NONE);
	CCore::getMap()->setBackgroundColor(rR);
}

/* ******************************************** */

void PauseMenu::enter() {
	switch(activeMenuOption) {
		case 0:
			CCFG::getMM()->setViewID(CCFG::getMM()->eGame);
			CCFG::getMusic()->PlayMusic();
			break;
		case 1:
			CCFG::getMM()->getOptions()->setEscapeToMainMenu(false);
			CCFG::getMM()->resetActiveOptionID(CCFG::getMM()->eOptions);
			CCFG::getMM()->getOptions()->updateVolumeRect();
			CCFG::getMM()->setViewID(CCFG::getMM()->eOptions);
			break;
		case 2:
			CCore::getMap()->resetGameData();
			CCFG::getMM()->setViewID(CCFG::getMM()->eMainMenu);
			break;
		case 3:
			CCore::quitGame = true;
			break;
	}
}

void PauseMenu::escape() {
	CCFG::getMM()->setViewID(CCFG::getMM()->eGame);
	CCFG::getMusic()->PauseMusic();
}

void PauseMenu::updateActiveButton(int iDir) {
	Menu::updateActiveButton(iDir);
}