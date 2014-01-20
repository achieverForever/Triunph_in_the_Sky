#include "Bullet.h"

Bullet::Bullet(Ogre::SceneManager* sm)
{
	mTimeToLive = 0.0f;
	mHit = false;
	mActive = false;
	mHitTargetName = "";
	mQuery = sm->createSphereQuery(Ogre::Sphere(), 0x1);	// 设置SceneQueryMask为0x1
	mQuery->setQueryTypeMask(Ogre::SceneManager::ENTITY_TYPE_MASK);	// 只查询Entities
}

Bullet::~Bullet()
{
}
bool Bullet::hitQuery()
{
	// 设置一个以子弹坐标为圆心，半径极小的球作为子弹的碰撞体
	mQuery->setSphere(Ogre::Sphere(m_wPos, 0.01f));
	mQuery->setQueryMask(0x1);
	// 执行球形碰撞查询，并返回结果
	Ogre::SceneQueryResult res = mQuery->execute();
	if (!res.movables.empty())
	{
		// 取出第一个碰撞物体的Name
		Ogre::SceneQueryResultMovableList::iterator it = res.movables.begin();

		mHitTargetName = ( *it )->getName();
		// 碰撞测试成功，返回true
		return true;
	}
	// 碰撞测试失败，返回false
	return false;
}

