#include "EnemyState.h"
#include"Application/GameObject/Enemy/Enemy.h"
#include"Application/GameObject/Player/Player.h"

#include"Application/GameObject/Enemy/BattleEnemy/BattleEnemy.h"
#include"Application/GameObject/Enemy/WanderEnemy/WanderEnemy.h"

void eAtkState::Enter(KdGameObject* gameObj)
{
	
}

void eAtkState::Update(KdGameObject* gameObj)
{
	BattleEnemy* battleEnemy = static_cast<BattleEnemy*>(gameObj);
	battleEnemy->EnemyAttack();
}

void eAtkState::Exit(KdGameObject* gameObj)
{


}

void eChaseState::Enter(KdGameObject* gameObj)
{
}

void eChaseState::Update(KdGameObject* gameObj)
{
	BattleEnemy* battleEnemy = static_cast<BattleEnemy*>(gameObj);
	battleEnemy->EnemyChase();
}

void eChaseState::Exit(KdGameObject* gameObj)
{
}

void eIdleState::Enter(KdGameObject* gameObj)
{
}

void eIdleState::Update(KdGameObject* gameObj)
{
	BattleEnemy* battleEnemy = static_cast<BattleEnemy*>(gameObj);
	battleEnemy->EnemyIdle();
}

void eIdleState::Exit(KdGameObject* gameObj)
{
}

void eSpawnState::Enter(KdGameObject* gameObj)
{
}

void eSpawnState::Update(KdGameObject* gameObj)
{
	BattleEnemy* battleEnemy = static_cast<BattleEnemy*>(gameObj);
	battleEnemy->EnemySpawn();
}

void eSpawnState::Exit(KdGameObject* gameObj)
{
	BattleEnemy* battleEnemy = static_cast<BattleEnemy*>(gameObj);
	battleEnemy->ExitSpawn();
}

void eDeathState::Enter(KdGameObject* gameObj)
{
}

void eDeathState::Update(KdGameObject* gameObj)
{
	BattleEnemy* battleEnemy = static_cast<BattleEnemy*>(gameObj);
	battleEnemy->HandleDeath();
}

void eDeathState::Exit(KdGameObject* gameObj)
{
}

void ePatrolState::Enter(KdGameObject* gameObj)
{
	
}

void ePatrolState::Update(KdGameObject* gameObj)
{
	
	WanderEnemy* wanderEnemy = static_cast<WanderEnemy*>(gameObj);
	wanderEnemy->EnemyPatrol();
}

void ePatrolState::Exit(KdGameObject* gameObj)
{
}

void eFollowState::Enter(KdGameObject* gameObj)
{
	
}

void eFollowState::Update(KdGameObject* gameObj)
{
	
	WanderEnemy* wanderEnemy = static_cast<WanderEnemy*>(gameObj);
	wanderEnemy->EnemyFollow();

}

void eFollowState::Exit(KdGameObject* gameObj)
{
}

void eDizzyState::Enter(KdGameObject* gameObj)
{
}

void eDizzyState::Update(KdGameObject* gameObj)
{
	
	WanderEnemy* wanderEnemy = static_cast<WanderEnemy*>(gameObj);
	wanderEnemy->EnemyDizzy();

}

void eDizzyState::Exit(KdGameObject* gameObj)
{
	WanderEnemy* wanderEnemy = static_cast<WanderEnemy*>(gameObj);
	wanderEnemy->ExitDizzy();
}

void eStunState::Enter(KdGameObject* gameObj)
{
}

void eStunState::Update(KdGameObject* gameObj)
{

	BattleEnemy* battleEnemy = static_cast<BattleEnemy*>(gameObj);
	battleEnemy->EnemyStun();
}

void eStunState::Exit(KdGameObject* gameObj)
{
}

void eMageAtkState::Enter(KdGameObject* gameObj)
{
}

void eMageAtkState::Update(KdGameObject* gameObj)
{
	BattleEnemy* battleEnemy = static_cast<BattleEnemy*>(gameObj);
	battleEnemy->MageAtk();
}

void eMageAtkState::Exit(KdGameObject* gameObj)
{
	BattleEnemy* battleEnemy = static_cast<BattleEnemy*>(gameObj);
	battleEnemy->ExitMageAtk();
}

void eMageWalkState::Enter(KdGameObject* gameObj)
{
}

void eMageWalkState::Update(KdGameObject* gameObj)
{
	BattleEnemy* battleEnemy = static_cast<BattleEnemy*>(gameObj);
	battleEnemy->MageWalk();
}

void eMageWalkState::Exit(KdGameObject* gameObj)
{
}

void eMageRangeAtkState::Enter(KdGameObject* gameObj)
{
}

void eMageRangeAtkState::Update(KdGameObject* gameObj)
{
	BattleEnemy* battleEnemy = static_cast<BattleEnemy*>(gameObj);
	battleEnemy->MageRangeAtk();
}

void eMageRangeAtkState::Exit(KdGameObject* gameObj)
{
}

void eBackJumpState::Enter(KdGameObject* gameObj)
{
}

void eBackJumpState::Update(KdGameObject* gameObj)
{
	BattleEnemy* battleEnemy = static_cast<BattleEnemy*>(gameObj);
	battleEnemy->EnemyBackJump();
}

void eBackJumpState::Exit(KdGameObject* gameObj)
{
}

void eBossJumpAtkState::Enter(KdGameObject* gameObj)
{
}

void eBossJumpAtkState::Update(KdGameObject* gameObj)
{
	BattleEnemy* battleEnemy = static_cast<BattleEnemy*>(gameObj);
	battleEnemy->BossJumpAtk();

}

void eBossJumpAtkState::Exit(KdGameObject* gameObj)
{
}

void eBossSummonState::Enter(KdGameObject* gameObj)
{
}

void eBossSummonState::Update(KdGameObject* gameObj)
{
	BattleEnemy* battleEnemy = static_cast<BattleEnemy*>(gameObj);
	battleEnemy->BossSummon();
}

void eBossSummonState::Exit(KdGameObject* gameObj)
{
	BattleEnemy* battleEnemy = static_cast<BattleEnemy*>(gameObj);
	battleEnemy->ExitSummon();
}

void eBossSlashWaveAtkState::Enter(KdGameObject* gameObj)
{
}

void eBossSlashWaveAtkState::Update(KdGameObject* gameObj)
{
}

void eBossSlashWaveAtkState::Exit(KdGameObject* gameObj)
{
}

void eBlowAwayState::Enter(KdGameObject* gameObj)
{
}

void eBlowAwayState::Update(KdGameObject* gameObj)
{
	BattleEnemy* battleEnemy = static_cast<BattleEnemy*>(gameObj);
	battleEnemy->EnemyBlowAway();
}

void eBlowAwayState::Exit(KdGameObject* gameObj)
{
}

void eWanderDeadState::Enter(KdGameObject* gameObj)
{
}

void eWanderDeadState::Update(KdGameObject* gameObj)
{
	WanderEnemy* wanderEnemy = static_cast<WanderEnemy*>(gameObj);
	wanderEnemy->WanderDead();
}

void eWanderDeadState::Exit(KdGameObject* gameObj)
{
}
