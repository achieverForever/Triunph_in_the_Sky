#include "Bullet.h"

Bullet::Bullet(Ogre::SceneManager* sm)
{
	mTimeToLive = 0.0f;
	mHit = false;
	mActive = false;
	mHitTargetName = "";
	mQuery = sm->createSphereQuery(Ogre::Sphere(), 0x1);	// ����SceneQueryMaskΪ0x1
	mQuery->setQueryTypeMask(Ogre::SceneManager::ENTITY_TYPE_MASK);	// ֻ��ѯEntities
}

Bullet::~Bullet()
{
}
bool Bullet::hitQuery()
{
	// ����һ�����ӵ�����ΪԲ�ģ��뾶��С������Ϊ�ӵ�����ײ��
	mQuery->setSphere(Ogre::Sphere(m_wPos, 0.01f));
	mQuery->setQueryMask(0x1);
	// ִ��������ײ��ѯ�������ؽ��
	Ogre::SceneQueryResult res = mQuery->execute();
	if (!res.movables.empty())
	{
		// ȡ����һ����ײ�����Name
		Ogre::SceneQueryResultMovableList::iterator it = res.movables.begin();

		mHitTargetName = ( *it )->getName();
		// ��ײ���Գɹ�������true
		return true;
	}
	// ��ײ����ʧ�ܣ�����false
	return false;
}

