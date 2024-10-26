#include"PlayerState.h"
#include"Application/GameObject/Player/Player.h"

void pAttackState::Enter(KdGameObject* gameObj)
{
	Player* player = static_cast<Player*>(gameObj);
	player->EnterAtk();
}

void pAttackState::Update(KdGameObject* gameObj)
{
	Player* player = static_cast<Player*>(gameObj);
	player->PlayerAttack();

}

void pAttackState::Exit(KdGameObject* gameObj)
{
	Player* player = static_cast<Player*>(gameObj);
	player->ExitAtk();
}

void pMoveState::Enter(KdGameObject* gameObj)
{
	Player* player = static_cast<Player*>(gameObj);
	player->EnterMove();


}

void pMoveState::Update(KdGameObject* gameObj)
{
	Player* player = static_cast<Player*>(gameObj);
	player->PlayerMove();
}

void pMoveState::Exit(KdGameObject* gameObj)
{
	Player* player = static_cast<Player*>(gameObj);
	player->ExitMove();

}

void pIdleState::Enter(KdGameObject* gameObj)
{
	Player* player = static_cast<Player*>(gameObj);
	player->EnterIdle();
}

void pIdleState::Update(KdGameObject* gameObj)
{
	Player* player = static_cast<Player*>(gameObj);
	player->PlayerIdle();
}

void pIdleState::Exit(KdGameObject* gameObj)
{
}

void pDashState::Enter(KdGameObject* gameObj)
{
	Player* player = static_cast<Player*>(gameObj);
	player->EnterDash();
}

void pDashState::Update(KdGameObject* gameObj)
{
	Player* player = static_cast<Player*>(gameObj);
	player->PlayerDash();
}

void pDashState::Exit(KdGameObject* gameObj)
{
	Player* player = static_cast<Player*>(gameObj);
	player->ExitDash();
}

void pDefenseState::Enter(KdGameObject* gameObj)
{
	Player* player = static_cast<Player*>(gameObj);
	player->EnterDefense();
}

void pDefenseState::Update(KdGameObject* gameObj)
{
	Player* player = static_cast<Player*>(gameObj);
	player->PlayerDefense();
}

void pDefenseState::Exit(KdGameObject* gameObj)
{
}

void pStrikeKillState::Enter(KdGameObject* gameObj)
{
}

void pStrikeKillState::Update(KdGameObject* gameObj)
{
	Player* player = static_cast<Player*>(gameObj);
	player->PlayerStrikeKill();
}

void pStrikeKillState::Exit(KdGameObject* gameObj)
{
}

void pStunState::Enter(KdGameObject* gameObj)
{
}

void pStunState::Update(KdGameObject* gameObj)
{
	Player* player = static_cast<Player*>(gameObj);
	player->PlayerStun();
}

void pStunState::Exit(KdGameObject* gameObj)
{
}

void pSkillState::Enter(KdGameObject* gameObj)
{
	Player* player = static_cast<Player*>(gameObj);
	player->EnterSkill();
}

void pSkillState::Update(KdGameObject* gameObj)
{
	Player* player = static_cast<Player*>(gameObj);
	player->PlayerSkill();

}

void pSkillState::Exit(KdGameObject* gameObj)
{
	Player* player = static_cast<Player*>(gameObj);
	player->ExitSkill();

}

void pBlowAwayState::Enter(KdGameObject* gameObj)
{
	Player* player = static_cast<Player*>(gameObj);
	player->EnterBlowAway();
}

void pBlowAwayState::Update(KdGameObject* gameObj)
{
	Player* player = static_cast<Player*>(gameObj);
	player->PlayerBlowAway();
}

void pBlowAwayState::Exit(KdGameObject* gameObj)
{

}
