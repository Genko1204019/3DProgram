#pragma once
#include"GameState.h"

class cFollowState : public GameState
{
public:
	cFollowState() {}
	~cFollowState() {}

	void Enter(KdGameObject* gameObj) override;
	void Update(KdGameObject* gameObj) override;
	void Exit(KdGameObject* gameObj) override;

private:

};