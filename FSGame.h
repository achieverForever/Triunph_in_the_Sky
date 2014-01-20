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
	Ogre::TerrainGlobalOptions* mTerrainGlobals;		// TerrainGroup里面的Terrains的基本设置
	Ogre::TerrainGroup* mTerrainGroup;					// TerrainGroup，维护多个Terrains
	bool mTerrainsImported;								// Terrain的高度数据是否已经导入
	OgreBites::Label* mInfoLabel;

	void defineTerrain(long x, long y);					// 定义了一个Terrain的高度数据来源
	void initBlendMaps(Ogre::Terrain* terrain);			// 初始化融合图
	void configureTerrainDefaults(Ogre::Light* light);	// 配置Terrains的基本设置
	void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img);		// 为一个Terrain的.dat地形数据文件指定高度数据来源(img)

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