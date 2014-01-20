#ifndef __FSGame_h__
#define __FSGame_h__

#include "StdAfx.h"
#include "BaseApplication.h"
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>
#include <OgreString.h>
#include "Aircraft.h"

#define TERRAIN_FILE_PREFIX Ogre::String("FSGameTerrain")
#define TERRAIN_FILE_SUFFIX Ogre::String("dat")
#define TERRAIN_WORLD_SIZE 12000.0f
#define TERRAIN_SIZE 513

class FSGame : public BaseApplication
{
private:
	// Terrains
	Ogre::TerrainGlobalOptions* mTerrainGlobals;		// TerrainGroup�����Terrains�Ļ�������
	Ogre::TerrainGroup* mTerrainGroup;					// TerrainGroup��ά�����Terrains
	bool mTerrainsImported;								// Terrain�ĸ߶������Ƿ��Ѿ�����
	OgreBites::Label* mInfoLabel;

	void defineTerrain(long x, long y);					// ������һ��Terrain�ĸ߶�������Դ
	void initBlendMaps(Ogre::Terrain* terrain);			// ��ʼ���ں�ͼ
	void configureTerrainDefaults(Ogre::Light* light);	// ����Terrains�Ļ�������
	void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img);		// Ϊһ��Terrain��.dat���������ļ�ָ���߶�������Դ(img)

	Aircraft* mAircraft;

	bool mContinue;
	bool mIsHitGround;

	Ogre::ParticleSystem* mPartSystem;

protected:
	virtual void createScene();
	virtual void createFrameListener();
	virtual void destroyScene();
	void setupView();

	// FrameListener
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	// KeyListener
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );

public:
	FSGame();
	virtual ~FSGame();

};

#endif