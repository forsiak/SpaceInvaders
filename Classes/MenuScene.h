/*
 * MenuScene.h
 *
 *  Created on: Jan 16, 2013
 *      Author: damian
 */
#include "cocos2d.h"
USING_NS_CC ;

#ifndef MENUSCENE_H_
#define MENUSCENE_H_

class MenuScene : public cocos2d::CCLayer
{
public:

	virtual ~MenuScene();
    virtual bool init();
    void menuCallback1(CCObject* sender);
    void menuCallback2(CCObject* sender);
   // CREATE_FUNC(MenuScene);
    static cocos2d::CCScene* scene();

   // MenuScene():_label(NULL) {};

};

#endif /* MENUSCENE_H_ */
