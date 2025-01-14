#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineRenderer.h>
#include <GameEngine/GameEngineCollision.h>

// ���� :
class Stone : public GameEngineActor
{
public:
	// constrcuter destructer
	Stone();
	~Stone();

	// delete Function
	Stone(const Stone& _Other) = delete;
	Stone(Stone&& _Other) noexcept = delete;
	Stone& operator=(const Stone& _Other) = delete;
	Stone& operator=(Stone&& _Other) noexcept = delete;

	void BombStone();
	void Create(int _StonType);

protected:

private:
	GameEngineRenderer* Renderer_;
	GameEngineRenderer* ShadowRenderer_;
	GameEngineCollision* Collision_;
	int StoneType_;

	void MakeRandomItemToBlackStone();
};