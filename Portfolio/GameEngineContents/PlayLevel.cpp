#include "PlayLevel.h"
#include "Player.h"
#include "Projectile.h"
#include "HpUI.h"
#include "TestMap.h"
#include "PlayBackGround.h"
#include "SpacebarUI.h"
#include "Accessory.h"
#include "ContentsEnum.h"

#include <GameEngineBase/GameEngineInput.h>
#include <GameEngine/GameEngine.h>
#include <GameEngineBase/GameEngineWindow.h>

PlayLevel::PlayLevel() 
	: PlayerInst_(nullptr)
{
}

PlayLevel::~PlayLevel() 
{
}

void PlayLevel::Loading()
{
	CreateActor<PlayBackGround>((int)ORDER::BACKGROUND);
	GameEngineActor* Actor = CreateActor<TestMap>((int)ORDER::BACKGROUND);
	PlayerInst_ = CreateActor<Player>((int)ORDER::PLAYER);
	CreateActor<HPUI>((int)ORDER::UI);
	CreateActor<SpacebarUI>((int)ORDER::UI);

	//Actor->CreateCollision("Wall", {100, 100}, { 300, 200 });

	if (false == GameEngineInput::GetInst()->IsKey("PlayESC"))
	{
		GameEngineInput::GetInst()->CreateKey("PlayESC", VK_ESCAPE);
		GameEngineInput::GetInst()->CreateKey("PlayP", 'P');
	}
}

void PlayLevel::Update()
{
	if (true == GameEngineInput::GetInst()->IsDown("PlayESC"))
	{
		Accessory* TEST = CreateActor<Accessory>();
		TEST->SetPosition(GameEngineWindow::GetScale().Half());
		TEST->SetType(AccessoryType::brokenmagnet);
	}

	if (true == GameEngineInput::GetInst()->IsDown("PlayP"))
	{
		GameEngine::ChangeDebugRender();
	}
}

void PlayLevel::LevelChangeStart()
{
	PlayerInst_->GetPlayerInfo();
	BgmPlayer_ = GameEngineSound::SoundPlayControl("BasementBGM.ogg");
	BgmPlayer_.SetVolume(0.05f);
}

void PlayLevel::LevelChangeEnd()
{
	BgmPlayer_.Stop();
}
