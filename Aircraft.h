#ifndef __AIRCRAFT_H__
#define __AIRCRAFT_H__	
#include "Weapon.h"

class Aircraft
{
private:
	Ogre::SceneNode* mNode;
	Ogre::Real mAcc;			// 加速度
	Ogre::Real mSpeed;		// 速度
	Ogre::Vector3 mDirection;	// 前进方向
	Ogre::Vector3 mPosition;	// 当前位置
	Ogre::Real	  mHeight;		// 当前高度
	Ogre::Real	  mEnergy;		// 燃料量，为零时坠机
	Ogre::Real	  mLife;		// 生命值，为零时Game Over

	bool isup;
	bool isdown;
	bool isleft;
	bool isright;
	bool isspeeddown;
	bool isspeedup;

public:

	Weapon * weapon;

	// Constructors and Destructors
	//--------------------------------------------------------------------------
	Aircraft(Ogre::SceneNode* node, Ogre::Real acc, Ogre::Real speed, 
		Ogre::Vector3 direction, Ogre::Vector3 position, 
		Ogre::Real height, Ogre::Real energy, Ogre::Real life, Ogre::SceneManager* sceneMgr, int bulletNum)
	{
		mNode = node;
		mAcc = acc;
		mSpeed = speed;
		mDirection = direction;
		mPosition = position;
		mHeight = height;
		mEnergy = energy;
		mLife = life;

		isup = false;
		isdown = false;
		isleft = false;
		isright = false;
		isspeeddown = false;
		isspeedup = false;
		weapon = new Weapon(mNode, Ogre::Vector3(0,1,-1), sceneMgr);

	}
	~Aircraft()
	{
	//	delete weapon;
	}
	
	// Getters and Setters
	//--------------------------------------------------------------------------
	Ogre::SceneNode* Node() const { return mNode; }

	Ogre::Real Acc() const { return mAcc; }
	void Acc(Ogre::Real val) { mAcc = val; }
		
	Ogre::Real Speed() const { return mSpeed; }
	void Speed(Ogre::Real val) { mSpeed = val; }

	Ogre::Vector3 Direction() const { return mDirection; }
	void Direction(Ogre::Vector3 val) { mDirection = val; }

	Ogre::Vector3 Position() const { return mPosition; }
	void Position(Ogre::Vector3 val) { mPosition = val; }

	Ogre::Real Height() const { return mHeight; }
	void Height(Ogre::Real val) { mHeight = val; }

	Ogre::Real Energy() const { return mEnergy; }
	void Energy(Ogre::Real val) { mEnergy = val; }

	Ogre::Real Life() const { return mLife; }
	void Life(Ogre::Real val) { mLife = val; }
	
	//
	bool isUp() const { return isup; }

	bool isDown() const { return isdown; }

	bool isLeft() const { return isleft; }

	bool isRight() const { return isright; }
	// 控制飞机
	//--------------------------------------------------------------------------
	void pitch(Ogre::Real angle);
	void roll(Ogre::Real angle);
	void yaw(Ogre::Real angle);

	void speedUp();
	void speedDown();
	void RspeedUp();
	void RspeedDown();
	void speedRecover(bool isHitGround);

	void Up(Ogre::Real angle);
	void Down(Ogre::Real angle);
	void Left(Ogre::Real angle1,Ogre::Real angle2);
	void Right(Ogre::Real angle1,Ogre::Real angle2);

	void RUp();
	void RDown();
	void RLeft();
	void RRight();

	void move();

	void fire(Ogre::Ray & ray);


};

#endif