#include "Weapon.h"

Weapon::Weapon(Ogre::SceneNode* parentNode, Ogre::Vector3 posRelative, Ogre::SceneManager* sm)
{
	mSceneMgr = sm;
	mNode = parentNode->createChildSceneNode(posRelative);
	mBBS = sm->createBillboardSet();
	mBBS->setBillboardOrigin(Ogre::BillboardOrigin::BBO_CENTER);
	mBBS->setBillboardsInWorldSpace(true);
	mBBS->setBillboardType(Ogre::BBT_POINT);
	mBBS->setDefaultDimensions(5.0f, 5.0f);
	mBBS->setMaterialName("Examples/Flare");
	mNode->attachObject(mBBS);
}

Weapon::~Weapon()
{

}

bool Weapon::fire(const Ogre::Ray & w_ray)
 {
	Bullet bullet(mSceneMgr);
	bullet.mActive = true;
	bullet.mBB = mBBS->createBillboard(w_ray.getOrigin(), Ogre::ColourValue(1.0f,0.5098f,0.0f));
	bullet.m_wPos = w_ray.getOrigin();
	bullet.m_wVelocity= w_ray.getDirection();
	bullet.m_wVelocity.normalise();
	mBullets.push_back(bullet);
	mBBS->_updateBounds();

	return true;
}

void Weapon::update(Ogre::Real timeElapsed)
{
	// 更新所有子弹的状态
	int i = 0;
	for (BulletList::iterator it = mBullets.begin(); it != mBullets.end(); it++)
	{
		Bullet* bullet = &(*it);

		if ( bullet->mActive )
		{
			if (bullet->mTimeToLive<=5.0f)
			{
				bullet->m_wPos = bullet->m_wPos + bullet->m_wVelocity * timeElapsed*100.0f;
				mBBS->getBillboard(i)->setPosition(bullet->m_wPos);

				if ( bullet->hitQuery())	// 子弹碰撞测试
				{
					bullet->mActive = false;
					targetHit( bullet->mHitTargetName );
				}
				bullet->mTimeToLive += timeElapsed;
			}else{
				bullet->mActive = false;
			}
		}else		// Bullet inactive, kill it
		{
			mBBS->removeBillboard(i);
			it = mBullets.erase(it);
		}

		if (it == mBullets.end())
		{
			break;
		}
		i++;

	}

}

void Weapon::targetHit(std::string targetName)
{
	// 根据MovableObject的Name，减少该Object的生命值(???)
	if (targetName != "")
	{
		Ogre::Entity* ent = mSceneMgr->getEntity(targetName);
		Ogre::SceneNode* node = ent->getParentSceneNode();
		node->scale(1.2f,1.2f,1.2f);

	}
}
