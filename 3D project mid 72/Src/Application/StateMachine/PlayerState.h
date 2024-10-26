#pragma once
#include"GameState.h"

class Player;

class pAttackState : public GameState
{
public:
	pAttackState() {}
	~pAttackState() {}

	void Enter(KdGameObject* gameObj) override;
	void Update(KdGameObject* gameObj) override;
	void Exit(KdGameObject* gameObj) override;

private:

};

class pMoveState : public GameState
{
public:
	pMoveState() {}
	~pMoveState() {}

	void Enter(KdGameObject* gameObj) override;
	void Update(KdGameObject* gameObj) override;
	void Exit(KdGameObject* gameObj) override;

private:

};

class pIdleState : public GameState
{
public:
	pIdleState() {}
	~pIdleState() {}

	void Enter(KdGameObject* gameObj) override;
	void Update(KdGameObject* gameObj) override;
	void Exit(KdGameObject* gameObj) override;

private:

};

class pDashState : public GameState
{
public:
	pDashState() {}
	~pDashState() {}

	void Enter(KdGameObject* gameObj) override;
	void Update(KdGameObject* gameObj) override;
	void Exit(KdGameObject* gameObj) override;

private:

};

class pDefenseState : public GameState
{
public:
	pDefenseState() {}
	~pDefenseState() {}

	void Enter(KdGameObject* gameObj) override;
	void Update(KdGameObject* gameObj) override;
	void Exit(KdGameObject* gameObj) override;

private:

};

class pStrikeKillState : public GameState
{
public:
	pStrikeKillState() {}
	~pStrikeKillState() {}

	void Enter(KdGameObject* gameObj) override;
	void Update(KdGameObject* gameObj) override;
	void Exit(KdGameObject* gameObj) override;

private:

};

class pSkillState : public GameState
{
public:
	pSkillState() {}
	~pSkillState() {}

	void Enter(KdGameObject* gameObj) override;
	void Update(KdGameObject* gameObj) override;
	void Exit(KdGameObject* gameObj) override;

private:

};

class pStunState : public GameState
{
public:
	pStunState() {}
	~pStunState() {}

	void Enter(KdGameObject* gameObj) override;
	void Update(KdGameObject* gameObj) override;
	void Exit(KdGameObject* gameObj) override;

private:

};

class pBlowAwayState : public GameState
{
public:
	pBlowAwayState() {}
	~pBlowAwayState() {}

	void Enter(KdGameObject* gameObj) override;
	void Update(KdGameObject* gameObj) override;
	void Exit(KdGameObject* gameObj) override;

private:

};
