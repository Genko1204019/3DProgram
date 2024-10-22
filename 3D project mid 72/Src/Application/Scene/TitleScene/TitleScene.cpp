#include "TitleScene.h"
#include "../SceneManager.h"

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


void TitleScene::Event()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SceneManager::Instance().SetNextScene
		(
			SceneManager::SceneType::Game
		);
	}
}

void TitleScene::Init()
{
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
