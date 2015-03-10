#pragma once

#include "d3dApp.h"
#include "Geometry.h"
#include "Actor.h"
#include "Player.h"
#include "Cube.h"
#include "Wall.h"
#include "Quad.h"
#include "Bullet.h"
#include "Line.h"
#include "EnemyLight.h"
#include "EnemyHeavy.h"
#include "EnemySplit.h"
#include "Air.h"
#include "Power.h"
#include "Points.h"
#include "Origin.h"
#include "Coin.h"
#include "EnemyBoat.h"
#include <d3dx9math.h>

namespace NL{

	const int MAX_PLAYER_BULLETS = 500;
	const int MAX_ENEMY_BULLETS = 5000;
	const int MAX_WALLS = 4000;
	const int MAX_LIGHT_ENEMIES = 500;
	const int MAX_HEAVY_ENEMIES = 500;
	const int MAX_SPLIT_ENEMEIS = 500;
	const int MAX_BOAT_ENEMEIS = 500;
	const int MAX_DROPS = 500;
	const float MAX_PLAYER_CENTER_DISTANCE = 15;
	const float PRECEIVED_SCREEN_WIDTH = 80;
	const float PRECEIVED_SCREEN_HEIGHT = 60;
	const float MIN_SCROLL_SPEED = 3;
	const int MAX_BACK = 3;
	const int NUM_BKGD_IMGS = 4;
	const int NUM_MENU_ITEMS = 7;//title, play, imFeelingLucky, quit, ___, howto
};

enum GameState{
	MENU,
	L1,
	L2,
	L3
};

class NuclearLiberation : public D3DApp{

public:
	NuclearLiberation(HINSTANCE hInstance);
	~NuclearLiberation();

	void initApp();
	void initBackground();
	void onResize();
	void updateScene(float dt);
	void drawScene(); 
	void collisions();

	void menuLoad();
	void menuUpdate(float dt,bool reset = false);
	void menuDraw();

	void clearLevel();
	void levelsUpdate(float dt);
	void levelsDraw();
	void loadLevel1();

	GameState state;
	
	void spawnBullet(Vector3 pos, Vector3 vel,float scale = 0.5);
	void spawnAir(Vector3 pos, Vector3 vel);
	void spawnPower(Vector3 pos, Vector3 vel);
	void spawnPoints(Vector3 pos, Vector3 vel);
	void spawnEnemyBullet(Vector3 pos, Vector3 vel);
	void spawnLightEnemy(Vector3 pos);
	void spawnHeavyEnemy(Vector3 pos);
	void spawnSplitEnemy(Vector3 pos, int gen);
	void spawnWall(Vector3 pos);
	void spawnEnemyBoat(float x);//boat will always spawn at surface

	void onPlayerDeath();

	Vector3 worldSize;
	
	Player player;

	ID3D10Device* getDevice(){return md3dDevice;}

	float invisibleWallLocation;
	float getCeiling(float x);
	float getFloor(float x);


private:
	void buildFX();
	void buildVertexLayouts();

	Origin origin;
	bool test;

protected:

	

	ID3D10Effect* mFX;
	ID3D10EffectTechnique* mTech;
	ID3D10InputLayout* mVertexLayout;
	ID3D10EffectMatrixVariable* mfxWVPVar;

	D3DXMATRIX mView;
	D3DXMATRIX mProj;

	Vector3 cameraDisplacement;
	Vector3 cameraTarget, cameraPositon, cameraUp;

	//geometry
	Cube cubeG,cubeR,cubeY,cubeW, cubeGLD, cubeLGRY,cubeSub, purpleCube;
	Line lineX, lineY, lineZ;
	Quad bgQuad[NL::NUM_BKGD_IMGS];
	Quad menuQuad, goldQuad, quadLtBlue;
	Coin whiteCoin, goldCoin, cyanCoin, redCoin;

	//game objects
	Bullet* playerBullets;
	Air* air;
	Power* power;
	Points* points;
	Wall* walls;
	
	Bullet* enemyBullets;
	EnemyLight * enemyLight;
	EnemyHeavy * enemyHeavy;
	EnemySplit * enemySplit;
	EnemyBoat * enemyBoat;

	Actor finishLine;
	Actor airBar;
	Actor bgImg[NL::NUM_BKGD_IMGS];
	std::wstring menuText[NL::NUM_MENU_ITEMS];
	int menuChoice;
	void spawnAllWallsOnMap();
	void placeFinishLine();
	void placeEnemyBoats(int numBoats);
};