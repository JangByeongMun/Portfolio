#include "Accessory.h"
#include <GameEngine/GameEngineCollision.h>
#include "Player.h"

Accessory::Accessory() 
	: Collision_(nullptr)
	, Type_(AccessoryType::None)
{
}

Accessory::~Accessory() 
{
}

void Accessory::Start()
{
	RendererVector_.push_back(CreateRenderer("trinket_001_swallowedpenny.bmp"));
	RendererVector_.push_back(CreateRenderer("trinket_002_petrifiedpoop.bmp"));
	RendererVector_.push_back(CreateRenderer("trinket_003_aaabattery.bmp"));
	RendererVector_.push_back(CreateRenderer("trinket_004_brokenremote.bmp"));
	RendererVector_.push_back(CreateRenderer("trinket_005_purpleheart.bmp"));
	RendererVector_.push_back(CreateRenderer("trinket_006_brokenmagnet.bmp"));
	RendererVector_.push_back(CreateRenderer("trinket_008_cartridge.bmp"));
	RendererVector_.push_back(CreateRenderer("trinket_009_pulseworm.bmp"));

	for (int i = 0; i < RendererVector_.size(); i++)
	{
		RendererVector_[i]->Off();
	}

	Collision_ = CreateCollision("Accessory", { 50, 50 }, {0, 0});
}

void Accessory::Update()
{
	if (AccessoryType::None == Type_)
	{
		return;
	}

	if (true == Collision_->CollisionCheck("Player", CollisionType::Rect, CollisionType::Rect))
	{
		Player::MainPlayer->SetAccessory(Type_);
		Death();
	}
}

