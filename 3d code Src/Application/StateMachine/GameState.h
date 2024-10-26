#pragma once

class GameState
{
public:
	GameState(){}
	~GameState(){}

	virtual void Enter(KdGameObject* gameObj) = 0;
	virtual void Update(KdGameObject* gameObj) = 0;
	virtual void Exit(KdGameObject* gameObj) = 0;



private:

};


