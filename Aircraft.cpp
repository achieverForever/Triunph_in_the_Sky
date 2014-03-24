#include "Aircraft.h"

void Aircraft::pitch(Ogre::Real angle)
{
	assert(mNode);
	mNode->pitch(Ogre::Degree(angle));
	Ogre::Quaternion rotation = mNode->getOrientation();
	mDirection = rotation * Ogre::Vector3::NEGATIVE_UNIT_Z;
	
}

void Aircraft::roll(Ogre::Real angle)
{
	assert(mNode);
	mNode->roll(Ogre::Degree(angle));
	Ogre::Quaternion rotation = mNode->getOrientation();
	mDirection = rotation * Ogre::Vector3::NEGATIVE_UNIT_Z;
}

void Aircraft::yaw(Ogre::Real angle)
{
	assert(mNode);
	mNode->pitch(Ogre::Degree(angle));
	Ogre::Quaternion rotation = mNode->getOrientation();
	mDirection = rotation * Ogre::Vector3::NEGATIVE_UNIT_Z;
}
void Aircraft::speedUp()
{
	isspeedup = true;
	mAcc += 0.04f;
}

void Aircraft::speedDown()
{
	isspeeddown = true;
	mAcc -= 0.02f;
}
void Aircraft::RspeedUp()
{
	isspeedup = false;
	mAcc = 0.0f;
}

void Aircraft::RspeedDown()
{
	isspeeddown = false;
	mAcc = 0.0f;
}
void Aircraft::speedRecover(bool isHitGround)
{
	if (isHitGround)
	{
		mSpeed = 0.0f;
		mAcc =0.0f;
	}else
	{
		if((mSpeed < 0.01f) && isspeeddown )
		{
			mSpeed = 0.0f;
			//mAcc = 0.0f;
		}
		else if(! isspeedup && ! isspeeddown)
		{
			if (mSpeed>2.0f)
			{
				mAcc = -0.01f;
			}
			else if(mSpeed<2.0f)
			{
				mAcc = 0.01f;
			}
		}
	}
	
}

//·ÉÐÐ×ËÌ¬µ÷Õû
//--------------------------------------
void Aircraft::Up(Ogre::Real angle)
{
	isup = true;
	assert(mNode);
	mNode->pitch(Ogre::Degree(angle));
}
void Aircraft::Down(Ogre::Real angle)
{
	isdown = true;
	assert(mNode);
	mNode->pitch(Ogre::Degree(angle));
}
void Aircraft::Left(Ogre::Real angle1,Ogre::Real angle2)
{
	isleft = true;
	assert(mNode);
	mNode->yaw(Ogre::Degree(angle2));
	mNode->roll(Ogre::Degree(angle1));
}
void Aircraft::Right(Ogre::Real angle1,Ogre::Real angle2)
{
	isright = true;
	assert(mNode);
	mNode->yaw(Ogre::Degree(angle2));
	mNode->roll(Ogre::Degree(angle1));
}

//·ÉÐÐ×ËÌ¬»Ö¸´
//----------------------------------------
void Aircraft::RUp()
{
	isup = false;
}
void Aircraft::RDown()
{
	isdown = false;
}
void Aircraft::RLeft()
{
	isleft = false;
	assert(mNode);
	mNode->roll(Ogre::Degree(-30.0f));
}
void Aircraft::RRight()
{
	isright = false;
	assert(mNode);
	mNode->roll(Ogre::Degree(30.0f));
}
//--------------------------------------------------
void Aircraft::move()
{
	assert(mNode);
	mNode->translate(mDirection * mSpeed, Ogre::Node::TS_LOCAL);
	mPosition = mNode->getPosition();
}

void Aircraft::fire(Ogre::Ray & ray)
{
	weapon->fire(ray);
}
