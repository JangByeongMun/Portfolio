#pragma once
#include <GameEngine/GameEngineActor.h>
#include "ContentsEnum.h"

// 설명 : 투사체
class Projectile : public GameEngineActor
{
public:
	// constrcuter destructer
	Projectile();
	~Projectile();

	// delete Function
	Projectile(const Projectile& _Other) = delete;
	Projectile(Projectile&& _Other) noexcept = delete;
	Projectile& operator=(const Projectile& _Other) = delete;
	Projectile& operator=(Projectile&& _Other) noexcept = delete;

	inline void SetVector(const float4& _value)
	{
		vec_ = _value;
	}
	inline void	SetSpeed(const float& _value)
	{
		speed_ = _value;
	}

protected:

private:
	ProjectileType _type;
	float4 vec_;
	float speed_;

	void Start() override;
	void Update() override;
};

