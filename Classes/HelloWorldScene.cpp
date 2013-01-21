#include "HelloWorldScene.h"
#include "AppMacros.h"
#include "SimpleAudioEngine.h"
#include <android/log.h>
using namespace CocosDenshion;
float factor = 1;
int level = 1;
int score = 0;
int lives = 3;
char points[32];
CCScene* HelloWorld::scene() {

	CCScene *scene = CCScene::create();
	HelloWorld *layer = HelloWorld::create();
	scene->addChild(layer);
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
	//////////////////////////////
	// 1. super init first
	if (!CCLayer::init()) {
		return false;
	}
	//lives = 10;
	//Game settings
	_nextEnemyChangePositionTime = 100000;

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();





	CCMenuItemImage *pCloseItem = CCMenuItemImage::create("CloseNormal.png",
			"CloseSelected.png", this,
			menu_selector(HelloWorld::menuCloseCallback));

	pCloseItem->setPosition(
			ccp(
					origin.x + visibleSize.width
							- pCloseItem->getContentSize().width / 2,
					origin.y + pCloseItem->getContentSize().height / 2));


	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label

	sprintf(points, "Lives: %d Level: %d Score: %d", lives, level, score);
	pScoreLabel = CCLabelTTF::create(points, "Arial", TITLE_FONT_SIZE);

	// position the label on the center of the screen
	pScoreLabel->setPosition(
			ccp(origin.x + visibleSize.width / 2,
					origin.y + visibleSize.height
							- pScoreLabel->getContentSize().height));

	// add the label as a child to this layer
	this->addChild(pScoreLabel, 1);

	_ship = CCSprite::create("ship.png");
	float shipNewSize = visibleSize.width / 6;
	__android_log_print(ANDROID_LOG_VERBOSE, "", "Nowa szerokosc statku:%f",
			visibleSize.height);
	__android_log_print(ANDROID_LOG_VERBOSE, "", "Wspolczynnik:%f",
			visibleSize.width);

	_scaleFactorX = shipNewSize / 50;

	shipNewSize = ((0.75 * visibleSize.height) / 4);
	_scaleFactorY = shipNewSize / 64;
	_scaleFactorX = _scaleFactorY;
	scaleObject(_ship);

	//Getting size of ship
	//_ship->setScale(0.5);
	float shipHeight = _ship->boundingBox().size.height / 2;

	// position the sprite on the center of the screen
	_ship->setPosition(ccp(visibleSize.width / 2 + origin.x, shipHeight));

	// add the sprite as a child to this layer
	this->addChild(_ship, 0);
	this->setAccelerometerEnabled(true);
	this->scheduleUpdate();

#define KNUMLASERS 5
	_shipLasers = new CCArray();
	_enemyLasers = new CCArray();
	for (int i = 0; i < KNUMLASERS; ++i) {
		CCSprite *shipLaser = CCSprite::create("shot.png");
		CCSprite *enemyLaser = CCSprite::create("shot.png");
		shipLaser->setVisible(false);
		enemyLaser->setVisible(false);
		this->addChild(shipLaser, 0);
		this->addChild(enemyLaser);
		_shipLasers->addObject(shipLaser);
		_enemyLasers->addObject(enemyLaser);
		scaleObject(enemyLaser);
		scaleObject(shipLaser);
	}

	this->setTouchEnabled(true);

	//Aliens
	_aliens = new CCArray();
	CCSprite *alien;
	for (int i = 1; i < 19; ++i) {

		alien = CCSprite::create("alien.png");
		this->addChild(alien);
		_aliens->addObject(alien);
		scaleObject(alien);
		/*
		 alien->setPosition(
		 ccp(alien->getContentSize().width * i,
		 visibleSize.height -  (i/7)* alien->getContentSize().height));
		 */
	}

	//Ustawiamy obcych na odpowiednich pozycjach
	setAliensPositions();
	__android_log_print(ANDROID_LOG_VERBOSE, "", "SzerokoscObcego:%f",
			alien->boundingBox().size.height);

	//Przygotowanie dzwiekow
	//
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sounds/music.wav",
			true);
	SimpleAudioEngine::sharedEngine()->preloadEffect(
			"Sounds/explosion_large.wav");
	SimpleAudioEngine::sharedEngine()->preloadEffect("Sounds/laser_ship.wav");
	_iEnemies = _aliens->count();
	_level_factor = 1;



	  _maxX = visibleSize.width/2 + 3 * alien->boundingBox().size.width;
	  _minX = visibleSize.width/2 - 3 * alien->boundingBox().size.width;




	return true;
}

void HelloWorld::menuCloseCallback(CCObject* pSender) {
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void HelloWorld::didAccelerate(CCAcceleration* pAccelerationValue) {

#define KSHIPMAXPOINTSPERSEC (winSize.width*1.25)

	pAccelerationValue->y = pAccelerationValue->x;
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	if (pAccelerationValue->y > 0.25) {
		_shipPointsPerSecX = KSHIPMAXPOINTSPERSEC * pAccelerationValue->y;

	} else if (pAccelerationValue->y < -0.25) {
		_shipPointsPerSecX = KSHIPMAXPOINTSPERSEC * pAccelerationValue->y;

	} else {
		_shipPointsPerSecX = 0;
	}

}

void HelloWorld::update(float dt) {

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	float maxX = winSize.width - _ship->getContentSize().width / 2;
	float minX = _ship->getContentSize().width / 2;

	float diff = (_shipPointsPerSecX * dt);
	float newX = _ship->getPosition().x + diff;

	if (_iEnemies == 0) {
		endScene(NEXT);
	}

	newX = MIN(MAX(newX, _minX), _maxX);
	_ship->setPosition(ccp(newX, _ship->getPosition().y));

	// Enemies collision
	CCArray * itAster, itLaser;
	CCObject* item = NULL;
	CCObject* laser_item = NULL;
	CCObject* enemy_laser_item = NULL;
	CCARRAY_FOREACH(_aliens, item)
	{
		CCSprite *enemy = dynamic_cast<CCSprite *>(item);
		if (!enemy->isVisible())
			continue;

		CCARRAY_FOREACH(_shipLasers, laser_item)
		{
			CCSprite *laser = dynamic_cast<CCSprite *>(laser_item);
			if (!laser->isVisible())
				continue;
			CCRect rect = laser->boundingBox();
			if (rect.intersectsRect(enemy->boundingBox())) {
				laser->setVisible(false);
				enemy->setVisible(false);
				_iEnemies--;
				score += level * 10;
				sprintf(points, "Lives: %d Level: %d Score: %d", lives, level,
						score);
				pScoreLabel->setString(points);
				SimpleAudioEngine::sharedEngine()->playEffect(
						"explosion_large.wav");
				//_aliens->removeObject(enemy, false);
				continue;
			}

		}

		if (_ship->boundingBox().intersectsRect(enemy->boundingBox())) {
			lives = 0;
			continue;

		}

		if (_ship->numberOfRunningActions() == 0) {
			CCARRAY_FOREACH(_enemyLasers, enemy_laser_item)
			{
				CCSprite *enemy_laser =
						dynamic_cast<CCSprite *>(enemy_laser_item);
				if (_ship->boundingBox().intersectsRect(
						enemy_laser->boundingBox())) {
					//enemy->setVisible(false);

					_ship->runAction(CCBlink::create(1.0, 9));
					lives--;
					sprintf(points, "Lives: %d Level: %d Score: %d", lives,
							level, score);
					pScoreLabel->setString(points);
					continue;
				}
			}
		}
	}

	//Wrogowie tez moga strzelac...
	float curTimeMillis = getTimeTick();
	if (curTimeMillis > _nextEnemyAttackTime) {
		if (_aliens->count() != 0) {

			float randMillisecs = randomValueBetween(0.20, 1.0) * 1000;
			_nextEnemyAttackTime = randMillisecs + curTimeMillis;

			//Losujemy ktory wrog powinien oddac strzal...
			//float randY = randomValueBetween(0.0,winSize.height);
			//float randDuration = randomValueBetween(2.0,10.0);

			CCSprite *enemy = dynamic_cast<CCSprite *>(_aliens->randomObject());
			if (enemy->isVisible()) {
				CCSprite *enemy_laser =
						dynamic_cast<CCSprite *>(_enemyLasers->objectAtIndex(
								_nextEnemyLaser));
				_nextEnemyLaser++;

				if (_nextEnemyLaser >= _enemyLasers->count())
					_nextEnemyLaser = 0;

				enemy_laser->setPosition(
						ccpAdd(enemy->getPosition(),
								ccp(enemy->boundingBox().size.width / 2, 0)));
				enemy_laser->setVisible(true);

				enemy_laser->runAction(
						CCSequence::create(
								CCMoveBy::create(0.5, ccp(0, -winSize.height)),
								CCCallFuncN::create(this,
										callfuncN_selector(
												HelloWorld::setInvisible)), NULL // DO NOT FORGET TO TERMINATE WITH NULL
								));
			}
		}
	}

	if (curTimeMillis > _nextEnemyChangePositionTime) {

		CCARRAY_FOREACH(_aliens, item)
		{
			_nextEnemyChangePositionTime = curTimeMillis + (10000 * factor);
			CCSprite *enemy = dynamic_cast<CCSprite *>(item);
			enemy->setPosition(ccpAdd(enemy->getPosition(), ccp(0, -25)));
		}

	}

	if (lives <= 0) {
		_ship->stopAllActions();
		_ship->setVisible(false);
		this->endScene(LOSE);
	}

}

void HelloWorld::ccTouchesBegan(cocos2d::CCSet* touches,
		cocos2d::CCEvent* event) {
	if (_ship->isVisible() && _iEnemies != 0) {
		SimpleAudioEngine::sharedEngine()->playEffect("laser_ship.wav");
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();

		CCSprite *shipLaser = (cocos2d::CCSprite*) _shipLasers->objectAtIndex(
				_nextShipLaser++);
		if (_nextShipLaser >= _shipLasers->count())
			_nextShipLaser = 0;
		shipLaser->setPosition(
				ccpAdd(_ship->getPosition(),
						ccp(shipLaser->getContentSize().width / 2, 0)));
		shipLaser->setVisible(true);
		shipLaser->stopAllActions();
		shipLaser->runAction(
				CCSequence::create(
						CCMoveBy::create(0.5, ccp(0, winSize.height)),
						CCCallFuncN::create(this,
								callfuncN_selector(HelloWorld::setInvisible)),
						NULL // DO NOT FORGET TO TERMINATE WITH NULL
						));
	}
}

void HelloWorld::setInvisible(CCNode * node) {
	node->setVisible(false);
}

float HelloWorld::randomValueBetween(float low, float high) {
	return (((float) arc4random() / 0xFFFFFFFFu) * (high - low)) + low;
}

float HelloWorld::getTimeTick() {
	timeval time;
	gettimeofday(&time, NULL);
	unsigned long millisecs = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (float) millisecs;
}

void HelloWorld::scaleObject(CCNode * node) {
	node->setScaleX(_scaleFactorX);
	node->setScaleY(_scaleFactorY);
}

void HelloWorld::setAliensPositions() {
	//Wiem,wiem...
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	float offsetX;
	int k = 0;
	for (int i = 0; i < 6; i++)
		for (int j = 0; j < 3; j++) {
			CCSprite *alien =
					dynamic_cast<CCSprite *>(_aliens->objectAtIndex(k));
			offsetX = (visibleSize.width - 6 * alien->boundingBox().size.width)
					/ 2;
			k++;
			alien->setAnchorPoint(ccp(0, 0));
			alien->setPosition(
					ccp(offsetX + alien->boundingBox().size.width * i,
							visibleSize.height
									- (1 + j)
											* alien->boundingBox().size.height));

		}

}

//Zatrzymujemy glowna pentel(update)

void HelloWorld::endScene(EndReason endReason) {
	if (_gameOver)
		return;
	_gameOver = true;

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	char message[10] = "You Win";
	char option[8] = "RESTART";
	if (endReason == LOSE)
		strcpy(message, "You Lose");
	else if (endReason = NEXT) {
		strcpy(message, "Next Level");
		strcpy(option, "Start");
	}
	CCLabelTTF * label;

	//label = CCLabelBMFont::create(message, "Arial.fnt");
	label = CCLabelTTF::create(message, "Arial", TITLE_FONT_SIZE);
	label->setScale(0.1);
	label->setPosition(ccp(winSize.width / 2, winSize.height * 0.6));
	this->addChild(label);

	CCLabelTTF * restartLabel;
	restartLabel = CCLabelTTF::create(option, "Arial", TITLE_FONT_SIZE);
	CCMenuItemLabel *restartItem;
	if (endReason == NEXT) {
		restartItem = CCMenuItemLabel::create(restartLabel, this,
				menu_selector(HelloWorld::nextLevel));

	} else {
		restartItem = CCMenuItemLabel::create(restartLabel, this,
				menu_selector(HelloWorld::restartTapped));
	}
	restartItem->setScale(0.1);
	restartItem->setPosition(ccp(winSize.width / 2, winSize.height * 0.4));

	CCMenu *menu = CCMenu::create(restartItem, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu);

	// clear label and menu
	restartItem->runAction(CCScaleTo::create(0.5, 1.0));
	label->runAction(CCScaleTo::create(0.5, 1.0));
	// Terminate update callback
	this->unscheduleUpdate();
}

void HelloWorld::restartTapped() {
	CCDirector::sharedDirector()->replaceScene(
			CCTransitionZoomFlipX::create(0.5, this->scene()));
	// reschedule
	_iEnemies = _aliens->count();
	level = 1;
	factor = 1;
	this->scheduleUpdate();
	lives = 3;
	score = 0;
	sprintf(points, "Lives: %d Level: %d Score: %d", lives, level, score);
	pScoreLabel->setString(points);
}

void HelloWorld::nextLevel() {
	CCDirector::sharedDirector()->replaceScene(
			CCTransitionZoomFlipX::create(0.5, this->scene()));
	// reschedule
	level++;
	factor = factor / 2;
	_iEnemies = _aliens->count();
	this->scheduleUpdate();
}
