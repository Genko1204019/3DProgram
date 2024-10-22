#include"StateMachine.h"
#include"Application/StateMachine/GameState.h"

void StateMachine::ChangeState(KdGameObject* gameObj, GameState* newState)
{
	if(currentState) currentState->Exit(gameObj);
	currentState = newState;
	currentState->Enter(gameObj);

}

void StateMachine::Update(KdGameObject* gameObj)
{
	if (currentState) currentState->Update(gameObj);
}
