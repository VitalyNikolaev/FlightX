#include "GameScene.h"
#include "Bullet.h"


USING_NS_CC;

namespace 
{
	const int kPlaneZIndex = 1;
	const int kBulletZIndex = 2;
    Label *scoreLabel;
    int bulcount = 0;
    Vector<EnemyPlane*> vectorEnemyPlanes;
    DefaultPlane* plane;
    
    
};

Scene* GameScene::createScene()
{
    auto scene = Scene::createWithPhysics();  // For physics
    scene->getPhysicsWorld()->setDebugDrawMask( PhysicsWorld::DEBUGDRAW_NONE);
    auto layer = GameScene::create();
    layer->setPhysicsWorld(scene->getPhysicsWorld());
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool GameScene::init()
{
    if (!Layer::init())
    {
        return false;	
    }

    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	SetDefaulBackground();
    
    auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 2);
    auto edgeNode = Node::create();
    edgeNode ->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y ));
    edgeNode->setPhysicsBody(edgeBody);
    this->addChild(edgeNode);

   
    
    plane = DefaultPlane::create();
	this->addChild(plane);
    
   

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
 
	{
		auto listener = EventListenerKeyboard::create();
		listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, cocos2d::Event * event) 
		{
			Bullet *bullet;
			switch (keyCode)
			{
			case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
				plane->MovePlane(Vec2(-1.0f, .0f));
				break;
			case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
				plane->MovePlane(Vec2(1.0f, .0f));
				break;
			case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
				plane->MovePlane(Vec2(.0f, 1.0f));
				break;
			case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
				plane->MovePlane(Vec2(.0f, -1.0f));
				break;
            case cocos2d::EventKeyboard::KeyCode::KEY_K:
                {
                    EnemyPlane *en_plane = EnemyPlane::create();

                    this->addChild(en_plane);
                    vectorEnemyPlanes.pushBack(en_plane);
                    
                    break;

                }
                

			case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
			{
				bullet = Bullet::create(plane);
                bullet->setTag(bulcount);
                bulletsMas.insert(bulcount, bullet);
                bulcount++;
				this->addChild(bullet, kBulletZIndex);
                if (bulcount > 40) {
                    bulcount = 0;
                }

			}
            break;
			default:
            break;
			}
		};
		listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event)
		{

			switch (keyCode) 
			{
				// Plane Movement
			case cocos2d::EventKeyboard::KeyCode::KEY_NONE:
				break;

			case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
				plane->MovePlane(Vec2(1.0f, .0f));
				break;
			case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
				plane->MovePlane(Vec2(-1.0f, .0f));

				break;
			case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
				plane->MovePlane(Vec2(.0f, -1.0f));
				break;
			case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
				plane->MovePlane(Vec2(.0f, 1.0f));
                    
				break;
          
			default:
				break;
			};
		};

		this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, plane);
	}
	// Update everything in this scene.
	this->scheduleUpdate();
    return true;
}

bool GameScene::onContactBegin(cocos2d::PhysicsContact& contact) {
    PhysicsBody *a = contact.getShapeA()->getBody();
    PhysicsBody *b = contact.getShapeB()->getBody();
    if ((1 == a->getCollisionBitmask() && 2 == b->getCollisionBitmask()) || (2 == a->getCollisionBitmask() && 1 == b->getCollisionBitmask()))
    {
        ((EnemyPlane*) b->getNode())->ApplyDamage(100);
        ((DefaultPlane*) a->getNode())->ApplyDamage(100);
    }
    if ((3 == a->getCollisionBitmask() && 2 == b->getCollisionBitmask()))
    {
            ((EnemyPlane*) b->getNode())->ApplyDamage(20);
        

        for (int i = 0; i < bulletsMas.size(); i++) {
            if (bulletsMas.at(i)->getTag() == a->getNode()->getTag()) {
                bulletsMas.at(i)->unscheduleUpdateAndDelete();
            }
        }

        score++;
        scoreLabel->setString(std::to_string(score));
    }
    
    return true;
}

void GameScene::SetDefaulBackground()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto back = Sprite::create("background.jpg");
	back->setFlippedY(true);
	back->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    back->setScale(0.7);
	this->addChild(back, 0);
    
    auto gameLabel = Label::createWithTTF("fonts/arial.ttf", "Score:");
    gameLabel->setColor(Color3B(255, 255 , 255));
    gameLabel->setPosition(Vec2(origin.x + visibleSize.width - 40,
                                origin.y + visibleSize.height - 10));
    this->addChild(gameLabel);
    
    scoreLabel = Label::create();
    scoreLabel->setString(std::to_string(score));
    scoreLabel->setColor(Color3B(255, 0 , 0));
    scoreLabel->setPosition(Vec2(origin.x + visibleSize.width-15,
                                 origin.y + visibleSize.height-10));
    this->addChild(scoreLabel, 0);
}

void GameScene::update(float delta)
{
    

    
}