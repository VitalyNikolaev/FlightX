#include "Bullet.h"
#include "math.h"
#include <iostream>
USING_NS_CC;

namespace
{
	static float kMaximumBulletSpeed = 200.0f;
	static int kDefaultDamage = 10;
	float DegreesToRadians(float degrees)
	{
		return degrees * (M_PI / 180);
	}
};

Bullet::Bullet() 
{
	//CCLOG("Bullet created!");
}

Bullet::~Bullet() 
{
	//CCLOG("Bullet destroyed!");
}

int Bullet::GetDamage()
{
    if (this != NULL)
    {
        return _damage;
    }
    return NULL;
    
}

Bullet*  Bullet::create(const DefaultPlane *plane)
{
	Bullet* bullet = new Bullet();
    
	if (bullet->initWithFile("fast_star.png"))
    {
		bullet->autorelease();
		bullet->initOptions(plane);
		bullet->addEvents();
        
		// Should be called after all work is done.
		return bullet;
    }
    
	CC_SAFE_DELETE(bullet);
    return NULL;

}

float Bullet::GetRotation() const
{
	return _currentSpeed.getAngle();
}

void Bullet::unscheduleUpdateAndDelete()
{
	// Obviously, we don't want all update() functions called after object was destroyed.
	this->unscheduleUpdate();
	removeFromParent();
	
	// TODO: Fix enemy bullets.
	//if (!this->_isEnemyBullet)
	//	this->~Bullet();
}

void Bullet::applySpeed(float deltaTime)
{
	auto currentPosition = getPosition();
	currentPosition += _currentSpeed * deltaTime;

	// There may be some more complex logic.
	setPosition(currentPosition);
}

bool Bullet::isStillOnScreen()
{
	auto boundings = getParent()->getBoundingBox();
	bool bIsOnScreen = this->getPositionX() < boundings.getMaxX() && this->getPositionX() > boundings.getMinX() &&
						this->getPositionY() < boundings.getMaxY() && this->getPositionY() > boundings.getMinY();
    
    // There may be some more complex logic.
	return bIsOnScreen;
}

void Bullet::update(float delta)
{
	applySpeed(delta);
	// If bullet is offscreen, destroy it.
	if (!isStillOnScreen())
		unscheduleUpdateAndDelete();
	
}

// Probably we should pass SimplePlane instance here to get all the values: plane type, isEnemy bool and initial rotation...
void Bullet::initOptions(const DefaultPlane *plane)
{
    auto bullet_body = PhysicsBody::createCircle(1,PhysicsMaterial(0,0,0));
    bullet_body->setCollisionBitmask(3);
    if (plane->IsEnemy()) {
        bullet_body->setCollisionBitmask(4);
    }
    this->setScaleY(0.25);
    this->setScaleX(0.5);
    bullet_body->setContactTestBitmask(true);
    bullet_body->setDynamic(true);
    this->setPhysicsBody(bullet_body);

    
	setPosition(plane->getPosition());

	_max_speed = kMaximumBulletSpeed;	// Should depend on bullet type.
	_damage = kDefaultDamage;

	_isEnemyBullet = plane->IsEnemy();
	_isEnemyBullet ? this->setColor(Color3B(255, 0, 0)) : this->setColor(Color3B(0, 255, 0)); 
	// I hate Maths.
	_currentSpeed = Vec2(-1.0f, 0.0f).rotateByAngle(Vec2(), -DegreesToRadians(plane->getRotation() + 90)).getNormalized() * kMaximumBulletSpeed;
}

// Some undescribable horror.
void Bullet::addEvents() 
{
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
//    listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event) {
//        cocos2d::Vec2 p = touch->getLocation();
//        cocos2d::Rect rect = this->getBoundingBox();
//        
//        if(rect.containsPoint(p)) {
//            return true; // to indicate that we have consumed it.
//        }
//        
//        return false; // we did not consume this event, pass thru.
//    };
//    
//    listener->onTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Event* event) {
//        touchEvent(touch);
//    };
//    
//    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 30);

	scheduleUpdate();

}

void Bullet::touchEvent(cocos2d::Touch* touch)
{
    //CCLOG("Sprite touched");
    //CCLOG("%s", this->_isEnemyBullet? "Enemy bullet!" : "Keep calm, it's my bullet.");

}