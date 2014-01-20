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

	bool fire(const Ogre::Ray & w_ray);	// ���𣬴������ӵ��б���ѡ��һ��û�м�����ӵ���������fire����
	void update(Ogre::Real timeElapsed);		// ���������ӵ�
	void targetHit(std::string targetName);		// ���ӵ�����Ŀ�꣬���ݻ�õ�Entity������Do whatever you want
private:
	BulletList mBullets;	
	Ogre::SceneNode* mNode;
	Ogre::BillboardSet* mBBS;
	Ogre::SceneManager* mSceneMgr;

};

#endif