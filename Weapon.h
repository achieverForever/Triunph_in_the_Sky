#ifndef __BULLETMANAGER_H__
#define __BULLETMANAGER_H__

#include "Bullet.h"
#include "OgreBillboard.h"
#include "OgreBillboardSet.h"

class Weapon
{
	typedef std::vector<Bullet> BulletList;

public:
	Weapon(Ogre::SceneNode* parentNode, Ogre::Vector3 posRelative, Ogre::SceneManager* sm);
	~Weapon();

	bool fire(const Ogre::Ray & w_ray);	// 开火，从所有子弹列表中选择一个没有激活的子弹，调用其fire函数
	void update(Ogre::Real timeElapsed);		// 更新所有子弹
	void targetHit(std::string targetName);		// 若子弹击中目标，根据获得的Entity的名称Do whatever you want
private:
	BulletList mBullets;	
	Ogre::SceneNode* mNode;
	Ogre::BillboardSet* mBBS;
	Ogre::SceneManager* mSceneMgr;

};

#endif