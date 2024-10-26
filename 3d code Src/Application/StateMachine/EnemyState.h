#pragma once
#include"GameState.h"


class eAtkState : public GameState
{
public:
	eAtkState(){}
	~eAtkState() {}

	void Enter(KdGameObject* gameObj) override;
	void Update(KdGameObject* gameObj) override;
	void Exit(KdGameObject* gameObj) override;

private:

};

class eChaseState : public GameState
{
public:
	eChaseState(){}
	~eChaseState(){}

	void Enter(KdGameObject* gameObj) override;
	void Update(KdGameObject* gameObj) override;
	void Exit(KdGameObject* gameObj) override;

private:

};

class eIdleState : public GameState
{
public:
	eIdleState(){}
	~eIdleState(){}


	void Enter(KdGameObject* gameObj) override;
	void Update(KdGameObject* gameObj) override;
	void Exit(KdGameObject* gameObj) override;

private:

};

class eBlowAwayState : public GameState
{
public:
	eBlowAwayState() {}
	~eBlowAwayState() {}


	void Enter(KdGameObject* gameObj) override;
	void Update(KdGameObject* gameObj) override;
	void Exit(KdGameObject* gameObj) override;

private:

};

class eSpawnState : public GameState
{
public:
	eSpawnState() {}
	~eSpawnState() {}


	void Enter(KdGameObject* gameObj) override;
	void Update(KdGameObject* gameObj) override;
	void Exit(KdGameObject* gameObj) override;

private:

};

class eDeathState : public GameState
{
public:
	eDeathState() {}
	~eDeathState() {}


	void Enter(KdGameObject* gameObj) override;
	void Update(KdGameObject* gameObj) override;
	void Exit(KdGameObject* gameObj) override;

private:

};





//Wanderer Logic
class ePatrolState : public GameState
{
public:
	ePatrolState() {}
	~ePatrolState() {}


	void Enter(KdGameObject* gameObj) override;
	void Update(KdGameObject* gameObj) override;
	void Exit(KdGameObject* gameObj) override;

private:

};

class eFollowState : public GameState
{
public:
	eFollowState() {}
	~eFollowState() {}


	void Enter(KdGameObject* gameObj) override;
	void Update(KdGameObject* gameObj) override;
	void Exit(KdGameObject* gameObj) override;

private:

};

class eStunState : public GameState
{
public:
	eStunState() {}
	~eStunState() {}


	void Enter(KdGameObject* gameObj) override;
	void Update(KdGameObject* gameObj) override;
	void Exit(KdGameObject* gameObj) override;

private:

};

class eBackJumpState : public GameState
{
public:
	eBackJumpState() {}
	~eBackJumpState() {}


	void Enter(KdGameObject* gameObj) override;
	void Update(KdGameObject* gameObj) override;
	void Exit(KdGameObject* gameObj) override;

private:

};

class eDizzyState : public GameState
{
public:
	eDizzyState() {}
	~eDizzyState() {}


	void Enter(KdGameObject* gameObj) override;
	void Update(KdGameObject* gameObj) override;
	void Exit(KdGameObject* gameObj) override;

private:

};

class eMageAtkState : public GameState
{
public:
	eMageAtkState() {}
	~eMageAtkState() {}


	void Enter(KdGameObject* gameObj) override;
	void Update(KdGameObject* gameObj) override;
	void Exit(KdGameObject* gameObj) override;

private:

};

class eMageRangeAtkState : public GameState
{
public:
	eMageRangeAtkState() {}
	~eMageRangeAtkState() {}


	void Enter(KdGameObject* gameObj) override;
	void Update(KdGameObject* gameObj) override;
	void Exit(KdGameObject* gameObj) override;

private:

};

class eMageWalkState : public GameState
{
public:
	eMageWalkState() {}
	~eMageWalkState() {}


	void Enter(KdGameObject* gameObj) override;
	void Update(KdGameObject* gameObj) override;
	void Exit(KdGameObject* gameObj) override;

private:

};

class eBossJumpAtkState : public GameState
{
public:
	eBossJumpAtkState() {}
	~eBossJumpAtkState() {}


	void Enter(KdGameObject* gameObj) override;
	void Update(KdGameObject* gameObj) override;
	void Exit(KdGameObject* gameObj) override;

private:

};

class eBossSummonState : public GameState
{
public:
	eBossSummonState() {}
	~eBossSummonState() {}

	void Enter(KdGameObject* gameObj) override;
	void Update(KdGameObject* gameObj) override;
	void Exit(KdGameObject* gameObj) override;

private:

};

class eBossSlashWaveAtkState : public GameState
{
public:
	eBossSlashWaveAtkState() {}
	~eBossSlashWaveAtkState() {}

	void Enter(KdGameObject* gameObj) override;
	void Update(KdGameObject* gameObj) override;
	void Exit(KdGameObject* gameObj) override;



private:

};

class eWanderDeadState : public GameState
{
public:
	eWanderDeadState() {}
	~eWanderDeadState() {}


	void Enter(KdGameObject* gameObj) override;
	void Update(KdGameObject* gameObj) override;
	void Exit(KdGameObject* gameObj) override;

private:

};



