#include "GameScene.h"
#include"../SceneManager.h"
#include"Application/main.h"
#include"Application/GameObject/Camera/GameCamera/GameCamera.h"
#include"Application/GameObject/Player/Player.h"
#include"Application/GameObject/Enemy/Enemy.h"
#include"Application/GameObject/MapObj/Ground.h"
#include"Application/Manager/EnemyLoader.h"
#include"Application/Manager/MapLoader.h"
#include"Application/Manager/GameManager.h"
#include"Application/GameObject/MapObj/MapObj.h"
#include"Application/Manager/UiManager.h"
#include"Application/Manager/InventoryManager.h"
#include"Application/GameObject/Enemy/WanderEnemy/WanderEnemy.h"



void GameScene::Event()
{
	/*static float test = 0.0f;
	test += 0.3f;
	Application::Instance().m_log.AddLog("test : %f \n", test);
	*/


	KdEffekseerManager::GetInstance().Update();

	

}

void GameScene::Init()
{

	KdEffekseerManager::GetInstance().Create(1280, 720);

	//ShowCursor(false);

	// プレイヤーの作成
	std::shared_ptr<Player> spPlayer = std::make_shared<Player>();
	spPlayer->Init();
	m_objList.push_back(spPlayer);


	//カメラの作成
	std::shared_ptr<GameCamera> spCamera = std::make_shared<GameCamera>();
	spCamera->Init();
	m_objList.push_back(spCamera);

	spCamera->SetPlayer(spPlayer);
	spPlayer->SetCamera(spCamera);

	MapLoader::Instance().SetCamera(spCamera);
	MapLoader::Instance().SetPlayer(spPlayer);
	MapLoader::Instance().SetKdCamera(spCamera->WorkCamera());

	EnemyLoader::Instance().SetCamera(spCamera);
	EnemyLoader::Instance().SetPlayer(spPlayer);
	EnemyLoader::Instance().Init(); //!!

	KdEffekseerManager::GetInstance().SetCamera(spCamera->WorkCamera());

	GameManager::Instance().SetPlayer(spPlayer);
	GameManager::Instance().SetCamera(spCamera);

	UiManager::Instance().SetPlayer(spPlayer);


	InventoryManager::Instance().SetPlayer(spPlayer);


	

}
