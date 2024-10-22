#include"EnemyLoader.h"
#include"Application/Manager/GameManager.h"
#include"Application/GameObject/Enemy/Enemy.h"
#include"Application/Scene/SceneManager.h"
#include"Application/GameObject/Enemy/BattleEnemy/BattleEnemy.h"

void EnemyLoader::Init()
{
}

void EnemyLoader::Update()
{
	LoadBattleEnemy();

}

void EnemyLoader::LoadWanderEnemy()
{


}

void EnemyLoader::LoadBattleEnemy()
{
	if (GameManager::Instance().GetCanLoadBattleEnemy())
	{
		GameManager::Instance().SetCanLoadBattleEnemy(false);
		FighterToLoad = 5;
		MageToLoad = 2;


		if (GameManager::Instance().GetFloorNum() != 0) {
			Vector3 fighterPositions[5] = {
			enemySpawnPos + Vector3(-8, 0, 3),  // Leftmost fighter
			enemySpawnPos + Vector3(8, 0, 3),   // Rightmost fighter

			enemySpawnPos + Vector3(0, 0, 9)     // Center fighter
			};

			Vector3 magePositions[2] = {
				enemySpawnPos + Vector3(-4, 0, 6),  // Closer to center (left)
				enemySpawnPos + Vector3(4, 0, 6),   // Closer to center (right)
			};

			for (int i = 0; i < 3; i++) { //3
				shared_ptr<BattleEnemy> fighter = make_shared<BattleEnemy>();
				fighter->SetType(EnemyType::eFighter);
				fighter->SetPlayer(wpPlayer.lock());
				fighter->SetCamera(wpCamera.lock());
				fighter->Init();
				fighter->SetPosition(fighterPositions[i]);
				SceneManager::Instance().AddObject(fighter);
				GameManager::Instance().SetBattleEnemyNum(GameManager::Instance().GetBattleEnemyNum() + 1);
			}

			for (int i = 0; i < 2; i++) { //2
				shared_ptr<BattleEnemy> mage = make_shared<BattleEnemy>();
				mage->SetType(EnemyType::eMage);
				mage->SetPlayer(wpPlayer.lock());
				mage->SetCamera(wpCamera.lock());
				mage->Init();
				mage->SetPosition(magePositions[i]);
				SceneManager::Instance().AddObject(mage);
				GameManager::Instance().SetBattleEnemyNum(GameManager::Instance().GetBattleEnemyNum() + 1);
			}
		}
		else {
			shared_ptr<BattleEnemy> Boss = make_shared<BattleEnemy>();
			Boss->SetType(EnemyType::eBoss);
			Boss->SetPlayer(wpPlayer.lock());
			Boss->SetCamera(wpCamera.lock());
			Boss->Init();
			Boss->SetPosition(enemySpawnPos);
			GameManager::Instance().SetBattleEnemyNum(GameManager::Instance().GetBattleEnemyNum() + 1);
			SceneManager::Instance().AddObject(Boss);
		}
		

		
		


	}


}

void EnemyLoader::LoadBossEnemy()
{
	if (GameManager::Instance().GetCanLoadBoss()) {

		GameManager::Instance().SetCanLoadBoss(false);
		GameManager::Instance().SetBattleEnemyNum(1);

		shared_ptr<BattleEnemy> boss = make_shared<BattleEnemy>();
		boss->SetType(EnemyType::eBoss);
		boss->SetPlayer(wpPlayer.lock());
		boss->SetCamera(wpCamera.lock());
		boss->Init();
		boss->SetPosition(enemySpawnPos + Vector3(0, 0, 9));
		SceneManager::Instance().AddObject(boss);
	}
}

void EnemyLoader::CallImgui()
{
}
