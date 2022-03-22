#pragma once
#include "GameEngineActor.h"
// ���� :
class GameEngineActorSubObject
{
	friend GameEngineActor;
public:
	// constrcuter destructer
	GameEngineActorSubObject();
	~GameEngineActorSubObject();

	// delete Function
	GameEngineActorSubObject(const GameEngineActorSubObject& _Other) = delete;
	GameEngineActorSubObject(GameEngineActorSubObject&& _Other) noexcept = delete;
	GameEngineActorSubObject& operator=(const GameEngineActorSubObject& _Other) = delete;
	GameEngineActorSubObject& operator=(GameEngineActorSubObject&& _Other) noexcept = delete;

	inline GameEngineActor* GetActor()
	{
		return actor_;
	}
protected:
	inline void SetActor(GameEngineActor* _actor)
	{
		actor_ = _actor;
	}

private:
	GameEngineActor* actor_;
};

