#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
USING_NS_CC ;


typedef enum {
   WIN,
   LOSE,
   NEXT
} EndReason;



class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

	//
	void setInvisible(CCNode * node);
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
	   static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
    //acelerometr
    virtual void didAccelerate(CCAcceleration* pAccelerationValue);
    //So we can shoot
    virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    //Random numbers
    float randomValueBetween( float low , float high );
    float getTimeTick();
    //Set scale ratio to every object
    void scaleObject(CCNode * node);




private:

    int _level;
    float _level_factor;
    float _scaleFactorX;
    float _scaleFactorY;
    float _nextEnemyAttackTime;
    float _shipPointsPerSecX;
    float  _nextEnemyChangePositionTime;
    int _nextShipLaser;
    int _nextEnemyLaser;
    int _iEnemies;
    CCArray *_shipLasers ;
    CCArray * _enemyLasers;
    CCArray *  _aliens;
    cocos2d::CCSprite * _ship;
    void update(float dt);
    void setAliensPositions();

    //Do zarzadzania stanem gry
    int _lives;
    double _gameOverTime ;
    bool _gameOver ;
    void endScene( EndReason endReason ) ;
    void restartTapped();
    void nextLevel();
    CCLabelTTF* pScoreLabel;

    int _maxX;
    int _minX;


};

#endif // __HELLOWORLD_SCENE_H__
