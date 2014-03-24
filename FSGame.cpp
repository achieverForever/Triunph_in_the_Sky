#include "FSGame.h"
#define TIAOZHENGSU 100

//--------------------------------------------------------------------------
FSGame::FSGame() : mTerrainGlobals(0), mTerrainGroup(0), mTerrainsImported(false), mInfoLabel(0)
{
	mContinue = true;
	mIsHitGround = false;
}
//--------------------------------------------------------------------------
FSGame::~FSGame()
{
	delete mAircraft;
}
//--------------------------------------------------------------------------
void FSGame::createScene()
{
	// 调整Camera的位置
	setupView();

	// 设置灯光
	Ogre::Vector3 lightdir(0.55, -0.3, 0.75);
	lightdir.normalise();

	Ogre::Light* dirlight = mSceneMgr->createLight("dirLight");
	dirlight->setType(Ogre::Light::LT_DIRECTIONAL);
	dirlight->setDirection(lightdir);
	dirlight->setDiffuseColour(Ogre::ColourValue::White);
	dirlight->setSpecularColour(Ogre::ColourValue(0.2, 0.2, 0.2));

	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.4, 0.4, 0.4));

	//--------------------------------------------------------------------------
	// 创建地形
	mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();
	mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(mSceneMgr, Ogre::Terrain::ALIGN_X_Z, TERRAIN_SIZE, TERRAIN_WORLD_SIZE);
	mTerrainGroup->setFilenameConvention(TERRAIN_FILE_PREFIX, TERRAIN_FILE_SUFFIX);
	mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

	configureTerrainDefaults(dirlight);

	// 定义每个Terrains的高度数据来源
	for (long x = 0; x <= 1; ++x)
		for (long y = 0; y <= 1; ++y)
			defineTerrain(x, y);	//ps：只要改变x,y的取值就可以生成多个Terrains，其中,0,0,表示在中间生成；0,1,表示再在其右边生成一个Terrain

	// 加载TerrainGroup里面所有的Terrains
	mTerrainGroup->loadAllTerrains(true);

	// 为每一个Terrains生成BlendMaps
	if (mTerrainsImported)
	{
		Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
		while(ti.hasMoreElements())
		{
			Ogre::Terrain* t = ti.getNext()->instance;
			initBlendMaps(t);
		}
	}

	// 释放临时资源
	mTerrainGroup->freeTemporaryResources();
	//--------------------------------------------------------------------------

	// 天空盒
	mSceneMgr->setSkyBox(true, "Examples/MorningSkyBox", 6000, false);

	// 飞机
	Ogre::Entity* ent = mSceneMgr->createEntity("Aircraft", "AircraftNew.mesh");
	
	Ogre::SceneNode* node = mSceneMgr->createSceneNode("Node1");
	node->attachObject(ent);
	node->setScale(5.0f, 5.0f, 5.0f);
	node->setPosition(Ogre::Vector3(1638, 50, 2116));
	mSceneMgr->getRootSceneNode()->addChild(node);

	// Weapon
	Ogre::Entity* ent2 = mSceneMgr->createEntity("Weapon", "sphere.mesh");
	Ogre::Entity* we = mSceneMgr->createEntity("We", "Weapon.mesh");

	Ogre::SceneNode* node2 = mSceneMgr->createSceneNode("Node2");
	node2->attachObject(ent2);
	node2->setScale(100.0f, 100.0f, 100.0f);
	node2->setPosition(Ogre::Vector3(1638, 80, 2116));
	mSceneMgr->getRootSceneNode()->addChild(node2);

	node2->attachObject(we);

	// 控制飞机
	mAircraft = new Aircraft(node, Ogre::Real(0.0f), Ogre::Real(0.0f), Ogre::Vector3::NEGATIVE_UNIT_Z, Ogre::Vector3(1638, 50, 2116), Ogre::Real(50), 100, 100, mSceneMgr, 10);

	mCamera->setFixedYawAxis(true);

	// 爆炸效果和着火效果的粒子系统
	mPartSystem = mSceneMgr->createParticleSystem("explosions", "explosionTemplate");
	mAircraft->Node()->attachObject(mPartSystem);
	mPartSystem->setEmitting(false);

	mAircraft->Node()->createChildSceneNode(Ogre::Vector3(0, 2, -2))->attachObject(mCamera);



}
//--------------------------------------------------------------------------
void FSGame::destroyScene()
{
	OGRE_DELETE mTerrainGroup;
	OGRE_DELETE mTerrainGlobals;

}
//--------------------------------------------------------------------------
void FSGame::setupView()
{
	mCamera->setNearClipDistance(0.1);
	mCamera->setFarClipDistance(50000);

	if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE));
	{
		mCamera->setFarClipDistance(0);		// 若硬件支持，使用无限远的远裁剪面
	}
}

void FSGame::createFrameListener()
{
	BaseApplication::createFrameListener();

	// 创建一个进度条显示地图生成进度
	mInfoLabel = mTrayMgr->createLabel(OgreBites::TrayLocation::TL_TOP, "TInfo", "", 350);

}

bool FSGame::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	bool ret = BaseApplication::frameRenderingQueued(evt);

	// 显示地图生成的状态
	if (mTerrainGroup->isDerivedDataUpdateInProgress())
	{
		mTrayMgr->moveWidgetToTray(mInfoLabel, OgreBites::TL_TOP, 0);
		mInfoLabel->show();
		if (mTerrainsImported)
		{
			mInfoLabel->setCaption("Building terrain, please wait...wait...wait...");
		}
		else
		{
			mInfoLabel->setCaption("Updating textures, patience...patience...patience...");
		}
	}
	else
	{
		mTrayMgr->removeWidgetFromTray(mInfoLabel);
		mInfoLabel->hide();
		if (mTerrainsImported)
		{
			mTerrainGroup->saveAllTerrains(true);
			mTerrainsImported = false;
		}
	}

	// 控制飞机
	mKeyboard->capture();

	Ogre::Vector3 vec = Ogre::Vector3::ZERO;

	if(mAircraft->isUp())
	{
		vec += Ogre::Vector3(0,(-1*(1-Ogre::Math::Cos(evt.timeSinceLastFrame*TIAOZHENGSU*1.0f))),(-1*Ogre::Math::Sin(evt.timeSinceLastFrame*TIAOZHENGSU*1.0f)));//改变“1”可以调节速度
	}
	if(mAircraft->isDown())
	{
		vec += Ogre::Vector3(0,(1-Ogre::Math::Cos(evt.timeSinceLastFrame*TIAOZHENGSU*1.0f)),(-1*Ogre::Math::Sin(evt.timeSinceLastFrame*TIAOZHENGSU*1.0f)));//改变“1”可以调节速度
	}
	if(mAircraft->isLeft())
	{
		mAircraft->Left(-30.0f,0.0f);
		vec += Ogre::Vector3((-1*(1-Ogre::Math::Cos(evt.timeSinceLastFrame*TIAOZHENGSU*1.0f))),0,(-1*Ogre::Math::Sin(evt.timeSinceLastFrame*TIAOZHENGSU*1.0f)));//改变“1”可以调节速度
	}
	if(mAircraft->isRight())
	{
		mAircraft->Right(30.0f,0.0f);
		vec += Ogre::Vector3((1-Ogre::Math::Cos(evt.timeSinceLastFrame*TIAOZHENGSU*1.0f)),0,(-1*Ogre::Math::Sin(evt.timeSinceLastFrame*TIAOZHENGSU*1.0f)));//改变“1”可以调节速度
	}
	if(vec == Ogre::Vector3::ZERO)
	{
		vec = Ogre::Vector3::NEGATIVE_UNIT_Z;
	}
	vec *= evt.timeSinceLastFrame*TIAOZHENGSU;
	mAircraft->Direction(vec);
	mAircraft->speedRecover(mIsHitGround);
	mAircraft->Speed((mAircraft->Speed())+(mAircraft->Acc()));
	mAircraft->move();

	if(mAircraft->isUp())
	{
		mAircraft->Up(evt.timeSinceLastFrame*TIAOZHENGSU*1.0f);
	}
	if(mAircraft->isDown())
	{
		mAircraft->Down(-evt.timeSinceLastFrame*TIAOZHENGSU*1.0f);
	}
	if(mAircraft->isLeft())
	{
		mAircraft->Left(30.0f,evt.timeSinceLastFrame*TIAOZHENGSU*1.0f);
	}
	if(mAircraft->isRight())
	{
		mAircraft->Right(-30.0f,-evt.timeSinceLastFrame*TIAOZHENGSU*1.0f);
	}

	// 射线查询，保证飞机不穿越地图
	Ogre::Vector3 pos = mAircraft->Node()->getPosition();
	Ogre::Ray terRay;
	terRay.setOrigin(Ogre::Vector3(pos.x, 5000.0f, pos.z));
	terRay.setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);
	Ogre::TerrainGroup::RayResult terRayResult = mTerrainGroup->rayIntersects(terRay);
	if (terRayResult.hit)
	{
		if ( (terRayResult.position.y + 3.0f) > pos.y)
		{
			// 与地面碰撞，爆炸
			mIsHitGround = true;

			if (mIsHitGround)
			{
				mPartSystem->setEmitting(true);
				mAircraft->Speed(0.0f);
				mAircraft->Acc(0.0f);
				mAircraft->Node()->setPosition(terRayResult.position.x, terRayResult.position.y+5.0f, terRayResult.position.z);
			}
		}
	}

	// 更新所有子弹的状态
	mAircraft->weapon->update(evt.timeSinceLastFrame);

	static bool flag = true;
	if (flag)
	{
		Ogre::Quaternion r = mAircraft->Node()->_getDerivedOrientation();
		Ogre::Vector3 direction = r * Ogre::Vector3::NEGATIVE_UNIT_Z;
		direction.normalise();
		Ogre::Ray ray(mAircraft->Node()->_getDerivedPosition(), direction);
		mAircraft->fire(ray);
		flag = false;
	}
	

	return ret && mContinue;
}

bool FSGame::keyPressed( const OIS::KeyEvent &e )
{
	switch(e.key)
	{
		case OIS::KC_ESCAPE:
			mContinue = false;
			break;
		case OIS::KC_LCONTROL:
			mAircraft->speedUp();
			break;
		case OIS::KC_LSHIFT:
			mAircraft->speedDown();
			break;
		case OIS::KC_W:
			mAircraft->Down(0.0f);
			break;
		case OIS::KC_S:
			mAircraft->Up(0.0f);
			break;
		case OIS::KC_A:
			mAircraft->Left(30.0f,0.0f);
			break;
		case OIS::KC_D:
			mAircraft->Right(-30.0f,0.0f);
			break;
		case OIS::KC_SPACE:
			Ogre::Quaternion r = mAircraft->Node()->_getDerivedOrientation();
			Ogre::Vector3 direction = r * Ogre::Vector3::NEGATIVE_UNIT_Z;
			direction.normalise();
			Ogre::Ray ray(mAircraft->Node()->_getDerivedPosition(), direction);
			mAircraft->fire(ray);
			break;
			
	}
	return true;
}

bool FSGame::keyReleased( const OIS::KeyEvent &e )
{	
	switch(e.key)
	{
	case OIS::KC_LCONTROL:
		mAircraft->RspeedUp();
		break;
	case OIS::KC_LSHIFT:
		mAircraft->RspeedDown();
		break;
	case OIS::KC_W:
		mAircraft->RDown();
		break;
	case  OIS::KC_S:
		mAircraft->RUp();
		break;
	case OIS::KC_A:
		mAircraft->RLeft();
		break;
	case OIS::KC_D:
		mAircraft->RRight();
		break;
	}
	return true;
}

void FSGame::defineTerrain(long x, long y)
{
	Ogre::String filename = mTerrainGroup->generateFilename(x, y);	// 每一个Terrain都会生成一个.dat文件
	// 若.dat文件存在，则使用它作为该Terrain的高度数据， 若不存在，则利用load进来的terrain.png高度图生成一个.dat地图数据文件
	if (Ogre::ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(), filename))
	{
		mTerrainGroup->defineTerrain(x, y);
	}else
	{
		Ogre::Image img;
		getTerrainImage(x%2 != 0, y%2 != 0, img);
		mTerrainGroup->defineTerrain(x, y, &img);
		mTerrainsImported = true;
	}

}

void FSGame::getTerrainImage(bool flipX, bool flipY, Ogre::Image& img)
{
	img.load("terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	if (flipX)
	{
		img.flipAroundX();
	}
	if (flipY)
	{
		img.flipAroundY();
	}
}

void FSGame::initBlendMaps(Ogre::Terrain* terrain)
{
	// 根据地形某点的高度计算该点处的融合因子（ps：我们用了三层layer，每层一个texture，所以有两个BlendMap）
	Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
	Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
	Ogre::Real minHeight0 = 70;
	Ogre::Real fadeDist0 = 40;
	Ogre::Real minHeight1 = 70;
	Ogre::Real fadeDist1 = 15;
	float* pBlend0 = blendMap0->getBlendPointer();
	float* pBlend1 = blendMap1->getBlendPointer();
	for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
	{
		for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
		{
			Ogre::Real tx, ty;

			blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
			Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
			Ogre::Real val = (height - minHeight0) / fadeDist0;
			val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
			*pBlend0++ = val;

			val = (height - minHeight1) / fadeDist1;
			val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
			*pBlend1++ = val;
		}
	}
	blendMap0->dirty();
	blendMap1->dirty();
	blendMap0->update();
	blendMap1->update();
}

void FSGame::configureTerrainDefaults(Ogre::Light* light)
{
	// 配置Terrains的全局设置
	mTerrainGlobals->setMaxPixelError(5);
	mTerrainGlobals->setCompositeMapDistance(3000);
	mTerrainGlobals->setSkirtSize(50);
	mTerrainGlobals->setCastsDynamicShadows(false);
	mTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
	mTerrainGlobals->setCompositeMapAmbient(mSceneMgr->getAmbientLight());
	mTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());
	

	// 配置默认的import设置，以便在使用imported image作为高度数据的时候使用
	Ogre::Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
	defaultimp.terrainSize = TERRAIN_SIZE;
	defaultimp.worldSize = TERRAIN_WORLD_SIZE;
	defaultimp.inputScale = 600;	// 因为terrain.png is 8 bpp???
	defaultimp.minBatchSize = 33;
	defaultimp.maxBatchSize = 65;
	// 设置纹理（这里使用了3层纹理）
	defaultimp.layerList.resize(3);
	defaultimp.layerList[0].worldSize = 100;
	defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
	defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
	defaultimp.layerList[1].worldSize = 30;
	defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
	defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
	defaultimp.layerList[2].worldSize = 200;
	defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
	defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");

}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	FSGame app;
	try {
		app.go();
	}catch ( Ogre::Exception& e){
		 MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
	}
	return 0;
}