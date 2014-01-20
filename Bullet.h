#ifndef __BULLET_H__
#define __BULLET_H__
#include "Ogre.h"

class Bullet
{
public:
	Bullet(Ogre::SceneManager* sm);
	~Bullet();
	bool hitQuery();	// 执行球形查询，检测是否与目标发生了碰撞

	Ogre::Billboard* mBB;
	Ogre::Vector3 m_wVelocity;	// 世界坐标系内的速度
	Ogre::Vector3 m_wPos;		// 世界坐标系内的坐标
	Ogre::Real mTimeToLive;		// 子弹的年龄
	bool mHit;
	bool mActive;	// 子弹在飞行时，视为Active状态；否则为Inactive	
	std::string mHitTargetName;	// 保存击中目标的名字
	Ogre::SphereSceneQuery*  mQuery;	// 用于查询子弹是否与目标发生了碰撞
	
};

#endif