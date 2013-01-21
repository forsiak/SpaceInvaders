/*
 * MenuScene.cpp
 *
 *  Created on: Jan 16, 2013
 *      Author: damian
 */

#include "MenuScene.h"

CCScene* MenuScene::scene() {
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	CCLayer *layer = MenuScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool MenuScene::init() {

	CCMenuItemImage* item1;
	CCMenuItemImage* item2;

	item1 = CCMenuItemImage::create("new_button.png", "exit_button_active.png",
			"exit_button_active.png", this, menu_selector(MenuScene::menuCallback1));

	item2 = CCMenuItemImage::create("exit_button.png", "button2Selected.png",
			"button2Disabled.png", this, menu_selector(MenuScene::menuCallback2));

	CCMenu* menu = CCMenu::create(item1, item2, NULL);
	menu->alignItemsVertically();
	this->addChild(menu);

}

void MenuScene::menuCallback1(CCObject* sender) {
	/*
	OtherScene* otherScene = OtherScene::create();

	CCLOG("Clicked on menu sprite item 1!");
	CCDirector::sharedDirector()->replaceScene(otherScene);
	*/
}

void MenuScene::menuCallback2(CCObject* sender) {
	/*
	AnScene* anScene = AnScene::create();

	CCLOG("Clicked on menu sprite item 2!");
	CCDirector::sharedDirector()->replaceScene(anScene);
	*/
}

