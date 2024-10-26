#pragma once

class GameState;

class StateMachine
{
public:
	StateMachine(){}
	~StateMachine() {}

	void ChangeState(KdGameObject *gameObj, GameState *newState);
	void Update(KdGameObject *gameObj);

private:

	GameState* currentState = nullptr;

};

