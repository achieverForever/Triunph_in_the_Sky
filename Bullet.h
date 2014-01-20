#ifndef __BULLET_H__
#define __BULLET_H__
#include "Ogre.h"

class Bullet
{
public:
	Bullet(Ogre::SceneManager* sm);
	~Bullet();
	bool hitQuery();	// ִ�����β�ѯ������Ƿ���Ŀ�귢������ײ

	Ogre::Billboard* mBB;
	Ogre::Vector3 m_wVelocity;	// ��������ϵ�ڵ��ٶ�
	Ogre::Vector3 m_wPos;		// ��������ϵ�ڵ�����
	Ogre::Real mTimeToLive;		// �ӵ�������
	bool mHit;
	bool mActive;	// �ӵ��ڷ���ʱ����ΪActive״̬������ΪInactive	
	std::string mHitTargetName;	// �������Ŀ�������
	Ogre::SphereSceneQuery*  mQuery;	// ���ڲ�ѯ�ӵ��Ƿ���Ŀ�귢������ײ
	
};

#endif