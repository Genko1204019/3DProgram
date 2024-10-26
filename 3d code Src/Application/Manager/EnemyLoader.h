#pragma once

using namespace std;
using namespace DirectX::SimpleMath;

class Player;

class EnemyLoader : public KdGameObject
{
public:
	
	void SetPlayer(const shared_ptr<Player>& player) { wpPlayer = player; }

	void Init() override;
	void Update() override;

	void LoadWanderEnemy();
	void LoadBattleEnemy();

	void CallImgui() override;


private:

	weak_ptr<Player> wpPlayer;

	int battleEnemyToLoad = 0;
	int FighterToLoad = 4;
	int MageToLoad	= 2;
	int ArcherToLoad = 0;
	int HealerToLoad = 0;

	Vector3 enemySpawnPos = { 7.5 ,-14,28 };



private:
	EnemyLoader() {}
	~EnemyLoader() override {}

public:
	static EnemyLoader& Instance()
	{
		static EnemyLoader instance;
		return instance;
	}
};

