#include "RoomActor.h"
#include "Stone.h"
#include "RandomRoomManager.h"
#include "Door.h"
#include "Pooter.h"
#include "ContentsEnum.h"
#include "Fire.h"
#include "PassiveItem.h"
#include "Monstro.h"
#include "Trapdoor.h"
#include "ClearBox.h"
#include <GameEngineBase/GameEngineRandom.h>
#include "Player.h"
#include "KeyItem.h"
#include "BombItem.h"
#include "MoneyItem.h"
#include "BatteryItem.h"
#include "BoxItem.h"
#include "ShopItem.h"
#include "ShopKeeper.h"
#include "ContentsGlobal.h"
#include <GameEngineBase/GameEngineSound.h>
#include "ContentsGlobal.h"
#include "Poop.h"
#include "Gaper.h"
#include "Sucker.h"
#include "RoundWorm.h"
#include "BatteryItem.h"
#include "BombItem.h"
#include "BoxItem.h"
#include "HeartItem.h"
#include "KeyItem.h"
#include "MoneyItem.h"
#include "Clotty.h"
#include "Iblob.h"
#include "Clot.h"
#include "BoomFly.h"
#include "Mulliboom.h"
#include "Host.h"
#include "Fly.h"
#include "Loki.h"
#include "Mom.h"
#include "Thorn.h"
#include "FloatingKnight.h"
#include "Boil.h"
#include "Gut.h"
#include "Spit.h"
#include "Charger.h"
#include "Gusher.h"
#include <GameEngineBase/GameEngineInput.h>
#include "PlayLevel.h"

float StartX = -420.0f;
float StartY = -225.0f;
float ScaleX = 70.0f;
float ScaleY = 75.0f;

RoomActor::RoomActor()
	: MonsterCount_(0)
	, BossCount_(0)
	, Trapdoor_(nullptr)
{
}

RoomActor::~RoomActor()
{
}

void RoomActor::AddMonsterCount()
{
	MonsterCount_ += 1;
}

void RoomActor::MinusMonsterCount()
{
	
	MonsterCount_ -= 1;

	if (MonsterCount_ <= 0 && BossCount_ <= 0)
	{
		GameEngineSound::SoundPlayOneShotWithVolume("door heavy open.wav", 0, 0.1f * Option_SFX);
		OpenAllDoor();
		Player::MainPlayer->AddGaze(1);

		if (Data_.RoomType_ != RoomType::Boss)
		{
			MakeMapReward();
		}
		else
		{
			MakePassive();
			OpenNextStage();
		}
	}
}

void RoomActor::MinusBossCount()
{
	BossCount_ -= 1;

	if (MonsterCount_ <= 0 && BossCount_ <= 0)
	{
		GameEngineSound::SoundPlayOneShotWithVolume("door heavy open.wav", 0, 0.1f * Option_SFX);
		OpenAllDoor();
		Player::MainPlayer->AddGaze(1);

		if (Data_.RoomType_ != RoomType::Boss)
		{
			MakeMapReward();
		}
		else
		{
			MakePassive();
			OpenNextStage();
		}
	}
}

void RoomActor::OpenNextStage()
{
	PlayLevel* TmpLevel = dynamic_cast<PlayLevel*>(GetLevel());
	if (TmpLevel != nullptr)
	{
		TmpLevel->StopBossBGM();
	}

	if (3 == CurrentFloor)
	{
		ClearBox* ClearBox_ = GetLevel()->CreateActor<ClearBox>();
		ClearBox_->SetPosition(GetPosition() + float4(0, -150));
	}
	else
	{
		Trapdoor_ = GetLevel()->CreateActor<Trapdoor>();
		Trapdoor_->SetPosition(GetPosition() + float4(0, -150));
	}

	for (int i = 0; i < BossHpRendererVector_.size(); i++)
	{
		BossHpRendererVector_[i]->Off();
	}
}

void RoomActor::Setting()
{
	float4 TmpRenderPos = { Pos_.x * 1280, Pos_.y * 740 };
	SetPosition(TmpRenderPos + GameEngineWindow::GetScale().Half());

	CreateRenderer("bgblack.bmp", RenderPivot::CENTER, float4::ZERO, -1);
	CreateRenderer(GetCurrentFloorName(), RenderPivot::CENTER, float4::ZERO);

	// 문닫혀있을때 벽 평평하게 만드는 컬리젼추가
	{
		GameEngineCollision* TmpCollision_ = CreateCollision("Wall", { 1000, 80 });
		TmpCollision_->SetPivot(float4(0, 300) + float4(0, -15));
		CloseCollisionVector_.push_back(TmpCollision_);
	}
	{
		GameEngineCollision* TmpCollision_ = CreateCollision("Wall", { 1000, 80 });
		TmpCollision_->SetPivot(float4(0, -300) + float4(0, -40));
		CloseCollisionVector_.push_back(TmpCollision_);
	}
	{
		GameEngineCollision* TmpCollision_ = CreateCollision("Wall", { 80, 700 });
		TmpCollision_->SetPivot(float4(500, 0) + float4(4, 0));
		CloseCollisionVector_.push_back(TmpCollision_);
	}
	{
		GameEngineCollision* TmpCollision_ = CreateCollision("Wall", { 80, 700 });
		TmpCollision_->SetPivot(float4(-500, 0) + float4(-8, 0));
		CloseCollisionVector_.push_back(TmpCollision_);
	}

	// 문 세팅
	DoorSetting();

	// 시작방에선 오브젝트 안만들고 가이드 UI 생성
	// 생각해보니 생성할때 0, 0엔 빈방을 만들고 나머지를 랜덤으로 채우는게 좋았을듯
	if (Pos_.CompareInt2D({ 0, 0 }))
	{
		CreateRenderer("StartGuide.bmp");
		return;
	}

	// 블럭 세팅
	std::vector<RoomData::BlockData> TmpTileVector = Data_.AllBlock_;
	for (int i = 0; i < TmpTileVector.size(); i++)
	{
		float4 TmpTilePos = { StartX + ScaleX * TmpTileVector[i].X_, StartY + ScaleY * TmpTileVector[i].Y_ };
		switch (TmpTileVector[i].Type_)
		{
		case BlockType::FIRE:
		{
			Fire* TmpFire = GetLevel()->CreateActor<Fire>();
			TmpFire->SetPosition(GetPosition() + TmpTilePos);
			TmpFire->SetRoom(this);
			break;
		}
		break;
		case BlockType::HOLE:
			break;
		case BlockType::POOP:
		{
			Poop* TmpActor = GetLevel()->CreateActor<Poop>();
			TmpActor->SetPosition(GetPosition() + TmpTilePos);
			//TmpActor->Create(0);
			break;
		}
		case BlockType::STONE:
		{
			Stone* TmpStone = GetLevel()->CreateActor<Stone>();
			TmpStone->SetPosition(GetPosition() + TmpTilePos);
			TmpStone->Create(0);
			break;
		}
		case BlockType::STONEBLACK:
		{
			Stone* TmpStone = GetLevel()->CreateActor<Stone>();
			TmpStone->SetPosition(GetPosition() + TmpTilePos);
			TmpStone->Create(1);
			break;
		}
		case BlockType::STONEURN:
		{
			Stone* TmpStone = GetLevel()->CreateActor<Stone>();
			TmpStone->SetPosition(GetPosition() + TmpTilePos);
			TmpStone->Create(2);
			break;
		}
		case BlockType::THORN:
		{
			Thorn* TmpActor = GetLevel()->CreateActor<Thorn>();
			TmpActor->SetPosition(GetPosition() + TmpTilePos);
			TmpActor->SetType(ThornType::Always);
			break;
		}
		default:
			break;
		}
	}

	// 몬스터 세팅
	std::vector<RoomData::MonsterData> TmpMonsterVector = Data_.AllMonster_;
	MonsterCount_ = static_cast<int>(TmpMonsterVector.size());
	for (int i = 0; i < TmpMonsterVector.size(); i++)
	{
		float4 TmpTilePos = { StartX + ScaleX * TmpMonsterVector[i].X_, StartY + ScaleY * TmpMonsterVector[i].Y_ };
		MonsterType TmpType = TmpMonsterVector[i].Type_;
		switch (TmpType)
		{
		case MonsterType::Default:
			break;

		case MonsterType::Pooter:
		{
			Pooter* TmpMonster = GetLevel()->CreateActor<Pooter>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
			MonsterVector_.push_back(TmpMonster);
			break;
		}
		case MonsterType::Gaper:
		{
			Gaper* TmpMonster = GetLevel()->CreateActor<Gaper>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
			MonsterVector_.push_back(TmpMonster);
			break;
		}
		case MonsterType::Gusher:
		{
			Gusher* TmpMonster = GetLevel()->CreateActor<Gusher>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
			MonsterVector_.push_back(TmpMonster);
			break;
		}
		case MonsterType::Sucker:
		{
			Sucker* TmpMonster = GetLevel()->CreateActor<Sucker>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
			MonsterVector_.push_back(TmpMonster);
			break;
		}
		case MonsterType::RoundWorm:
		{
			RoundWorm* TmpMonster = GetLevel()->CreateActor<RoundWorm>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
			MonsterVector_.push_back(TmpMonster);
			break;
		}
		case MonsterType::Clot:
		{
			Clot* TmpMonster = GetLevel()->CreateActor<Clot>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
			MonsterVector_.push_back(TmpMonster);
			break;
		}
		case MonsterType::Clotty:
		{
			Clotty* TmpMonster = GetLevel()->CreateActor<Clotty>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
			MonsterVector_.push_back(TmpMonster);
			break;
		}
		case MonsterType::Iblob:
		{
			Iblob* TmpMonster = GetLevel()->CreateActor<Iblob>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
			MonsterVector_.push_back(TmpMonster);
			break;
		}
		case MonsterType::BoomFlyNormal:
		{
			BoomFly* TmpMonster = GetLevel()->CreateActor<BoomFly>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
			TmpMonster->Setting(BoomFlyType::Normal);
			MonsterVector_.push_back(TmpMonster);
			break;
		}
		case MonsterType::BoomFlyRed:
		{
			BoomFly* TmpMonster = GetLevel()->CreateActor<BoomFly>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
			TmpMonster->Setting(BoomFlyType::Red);
			MonsterVector_.push_back(TmpMonster);
			break;
		}
		case MonsterType::Mulliboom:
		{
			Mulliboom* TmpMonster = GetLevel()->CreateActor<Mulliboom>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
			MonsterVector_.push_back(TmpMonster);
			break;
		}
		case MonsterType::HostNormal:
		{
			Host* TmpMonster = GetLevel()->CreateActor<Host>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
			TmpMonster->SetType(HostType::Normal);
			MonsterVector_.push_back(TmpMonster);
			break;
		}
		case MonsterType::HostRed:
		{
			Host* TmpMonster = GetLevel()->CreateActor<Host>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
			TmpMonster->SetType(HostType::Red);
			MonsterVector_.push_back(TmpMonster);
			break;
		}
		case MonsterType::FlyNormal:
		{
			Fly* TmpMonster = GetLevel()->CreateActor<Fly>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
			TmpMonster->SetType(FlyType::Normal);
			MonsterVector_.push_back(TmpMonster);
			break;
		}
		case MonsterType::FlyRed:
		{
			Fly* TmpMonster = GetLevel()->CreateActor<Fly>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
			TmpMonster->SetType(FlyType::Red);
			MonsterVector_.push_back(TmpMonster);
			break;
		}
		case MonsterType::FloatingKnight:
		{
			FloatingKnight* TmpMonster = GetLevel()->CreateActor<FloatingKnight>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
			MonsterVector_.push_back(TmpMonster);
			break;
		}
		case MonsterType::Boil:
		{
			Boil* TmpMonster = GetLevel()->CreateActor<Boil>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
			MonsterVector_.push_back(TmpMonster);
			break;
		}
		case MonsterType::Gut:
		{
			Gut* TmpMonster = GetLevel()->CreateActor<Gut>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
			MonsterVector_.push_back(TmpMonster);
			break;
		}
		case MonsterType::Spit:
		{
			Spit* TmpMonster = GetLevel()->CreateActor<Spit>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
			MonsterVector_.push_back(TmpMonster);
			break;
		}
		case MonsterType::Charger:
		{
			Charger* TmpMonster = GetLevel()->CreateActor<Charger>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
			MonsterVector_.push_back(TmpMonster);
			break;
		}
		case MonsterType::Max:
			break;

		default:
			break;
		}
	}

	// 패시브 아이템 세팅
	std::vector<RoomData::PassiveData> TmpPassiveVector = Data_.AllPassives_;
	for (int i = 0; i < TmpPassiveVector.size(); i++)
	{
		float4 TmpTilePos = { StartX + ScaleX * TmpPassiveVector[i].X_, StartY + ScaleY * TmpPassiveVector[i].Y_ };
		PassiveItem* TmpPassiveItem = GetLevel()->CreateActor<PassiveItem>();
		TmpPassiveItem->SetPosition(GetPosition() + TmpTilePos);

		PassiveType TmpType = static_cast<PassiveType>(GameEngineRandom::MainRandom->RandomInt(1, static_cast<int>(PassiveType::Max) - 1));
		TmpPassiveItem->Setting(TmpType);
	}

	// 보스몬스터 세팅
	std::vector<RoomData::BossData> TmpBossVector = Data_.AllBoss_;
	BossCount_ = static_cast<int>(TmpBossVector.size());
	for (int i = 0; i < TmpBossVector.size(); i++)
	{
		float4 TmpTilePos = { StartX + ScaleX * TmpBossVector[i].X_, StartY + ScaleY * TmpBossVector[i].Y_ };
		switch (TmpBossVector[i].Type_)
		{
		case BossType::Default:
			break;

		case BossType::Monstro:
		{
			Monstro* TmpBoss = GetLevel()->CreateActor<Monstro>();
			TmpBoss->SetPosition(GetPosition() + float4(0, -60));
			TmpBoss->SetRoom(*this);
			MonsterVector_.push_back(TmpBoss);
			break;
		}
		case BossType::Loki:
		{
			Loki* TmpBoss = GetLevel()->CreateActor<Loki>();
			TmpBoss->SetPosition(GetPosition());
			TmpBoss->SetRoom(*this);
			MonsterVector_.push_back(TmpBoss);
			break;
		}
		case BossType::Mom:
		{
			Mom* TmpBoss = GetLevel()->CreateActor<Mom>();
			TmpBoss->SetPosition(GetPosition());
			TmpBoss->SetRoom(*this);
			MonsterVector_.push_back(TmpBoss);
			break;
		}
		case BossType::Max:
			break;
		default:
			break;
		}
	}

	// 상점 아이템 세팅
	std::vector<RoomData::ShopData> TmpShopVector = Data_.AllShop_;
	for (int i = 0; i < TmpShopVector.size(); i++)
	{
		float4 TmpTilePos = { StartX + ScaleX * TmpShopVector[i].X_, StartY + ScaleY * TmpShopVector[i].Y_ };

		ShopItem* TmpItem = GetLevel()->CreateActor<ShopItem>();
		TmpItem->SetPosition(GetPosition() + TmpTilePos);
		TmpItem->SetType(TmpShopVector[i].Type_);
	}
	if (RoomType::Shop == Data_.RoomType_)
	{
		float4 TmpTilePos = { StartX + ScaleX * 6, StartY + ScaleY * 2 };
		ShopKeeper* TmpItem = GetLevel()->CreateActor<ShopKeeper>();
		TmpItem->SetPosition(GetPosition() + TmpTilePos);
	}

	// 픽업 아이템 세팅
	std::vector<RoomData::PickupData> TmpVector = Data_.AllPickUp_;
	for (int i = 0; i < TmpVector.size(); i++)
	{
		float4 TmpTilePos = { StartX + ScaleX * TmpVector[i].X_, StartY + ScaleY * TmpVector[i].Y_ };
		ItemType TmpType = TmpVector[i].Type_;

		switch (TmpType)
		{
		case ItemType::Bomb:
		{
			BombItem* TmpObject = GetLevel()->CreateActor<BombItem>();
			TmpObject->SetType(BombType::Normal);
			TmpObject->SetPosition(GetPosition() + TmpTilePos);
			break;
		}
		case ItemType::BombTwo:
		{
			BombItem* TmpObject = GetLevel()->CreateActor<BombItem>();
			TmpObject->SetType(BombType::Two);
			TmpObject->SetPosition(GetPosition() + TmpTilePos);
			break;
		}
		case ItemType::Key:
		{
			KeyItem* TmpObject = GetLevel()->CreateActor<KeyItem>();
			TmpObject->SetType(KeyType::Normal);
			TmpObject->SetPosition(GetPosition() + TmpTilePos);
			break;
		}
		case ItemType::KeyTwo:
		{
			KeyItem* TmpObject = GetLevel()->CreateActor<KeyItem>();
			TmpObject->SetType(KeyType::Two);
			TmpObject->SetPosition(GetPosition() + TmpTilePos);
			break;
		}
		case ItemType::keyMaster:
		{
			KeyItem* TmpObject = GetLevel()->CreateActor<KeyItem>();
			TmpObject->SetType(KeyType::Master);
			TmpObject->SetPosition(GetPosition() + TmpTilePos);
			break;
		}
		case ItemType::Money:
		{
			MoneyItem* TmpObject = GetLevel()->CreateActor<MoneyItem>();
			TmpObject->SetType(MoneyType::Normal, false);
			TmpObject->SetPosition(GetPosition() + TmpTilePos);
			break;
		}
		case ItemType::MoneyBlack:
		{
			MoneyItem* TmpObject = GetLevel()->CreateActor<MoneyItem>();
			TmpObject->SetType(MoneyType::Black, false);
			TmpObject->SetPosition(GetPosition() + TmpTilePos);
			break;
		}
		case ItemType::MoneySilver:
		{
			MoneyItem* TmpObject = GetLevel()->CreateActor<MoneyItem>();
			TmpObject->SetType(MoneyType::Silver, false);
			TmpObject->SetPosition(GetPosition() + TmpTilePos);
			break;
		}
		case ItemType::Battery:
		{
			BatteryItem* TmpObject = GetLevel()->CreateActor<BatteryItem>();
			TmpObject->SetPosition(GetPosition() + TmpTilePos);
			break;
		}
		default:
			break;
		}
	}

	if (BossCount_ != 0)
	{
		{
			GameEngineRenderer* TmpRenderer = CreateRenderer("ui_bosshealthbar_Shadow.bmp", static_cast<int>(ORDER::UI), RenderPivot::Left);
			TmpRenderer->SetPivot({ -230, -300 });
			TmpRenderer->SetAlpha(150);
			BossHpRendererVector_.push_back(TmpRenderer);
		}
		{
			GameEngineRenderer* TmpRenderer = CreateRenderer("ui_bosshealthbar.bmp", static_cast<int>(ORDER::UI), RenderPivot::Left);
			TmpRenderer->SetPivot({ -230, -300 });
			TmpRenderer->SetIndex(1);
			BossHpRendererVector_.push_back(TmpRenderer);
		}
		{
			GameEngineRenderer* TmpRenderer = CreateRenderer("ui_bosshealthbar_2.bmp", static_cast<int>(ORDER::UI), RenderPivot::Left);
			TmpRenderer->SetPivot({ -170, -300 });
			BossHpRendererVector_.push_back(TmpRenderer);
		}
	}
}

Door* RoomActor::FindDoor(DoorDir _Dir)
{
	for (int i = 0; i < DoorVector_.size(); i++)
	{
		if (_Dir == DoorVector_[i]->GetDir())
		{
			return DoorVector_[i];
		}
	}
	return nullptr;
}

bool RoomActor::IsCurrentRoom()
{
	return RandomRoomManager::GetInst()->GetCurrentRoom() == this;
}

void RoomActor::OpenAllDoor()
{
	for (int i = 0; i < DoorVector_.size(); i++)
	{
		DoorVector_[i]->DoorOpen();
	}
}

void RoomActor::CloseAllDoor()
{
	for (int i = 0; i < DoorVector_.size(); i++)
	{
		DoorVector_[i]->DoorClose();
	}
}

void RoomActor::SetBossHPUI(float _Value)
{
	BossHpRendererVector_[2]->SetIndexWithValue(0, { -1.0f, -1.0f }	, _Value);;
}

void RoomActor::AllMonsterAttack(float _Damage)
{
	for (int i = 0; i < MonsterVector_.size(); i++)
	{
		if (MonsterVector_[i] == nullptr || MonsterVector_[i]->IsUpdate() == false)
		{
			continue;
		}

		MonsterVector_[i]->Damaged(_Damage);
	}
}

void RoomActor::AddMonster(Monster* _Monster)
{
	MonsterVector_.push_back(_Monster);
}

float4 RoomActor::GetRandomPos()
{
	float4 TmpTilePos = { StartX + ScaleX * GameEngineRandom::MainRandom->RandomInt(0, 12), StartY + ScaleY * GameEngineRandom::MainRandom->RandomInt(0, 6) };
	return GetPosition() + TmpTilePos;
}

void RoomActor::Start()
{
	SetPosition(GameEngineWindow::GetScale().Half());

	////////////////// 디버깅용 키입력
	if (false == GameEngineInput::GetInst()->IsKey("TestH"))
	{
		GameEngineInput::GetInst()->CreateKey("TestH", 'H');
		GameEngineInput::GetInst()->CreateKey("TestJ", 'J');
		GameEngineInput::GetInst()->CreateKey("TestK", 'K');
		GameEngineInput::GetInst()->CreateKey("TestL", 'L');
		GameEngineInput::GetInst()->CreateKey("TestN", 'N');
		GameEngineInput::GetInst()->CreateKey("TestM", 'M');
		GameEngineInput::GetInst()->CreateKey("Test,", VK_OEM_COMMA);
		GameEngineInput::GetInst()->CreateKey("Test.", VK_OEM_PERIOD);
		GameEngineInput::GetInst()->CreateKey("ctrl", VK_CONTROL);
		GameEngineInput::GetInst()->CreateKey("shift", VK_SHIFT);
	}
}

void RoomActor::Update()
{
	if (this == RandomRoomManager::GetInst()->GetCurrentRoom())
	{
		DebugMonster();
	}
}

void RoomActor::DoorSetting()
{
	DoorType DefaultDoorType = DoorType::Default;
	// 자기방이 황금방이면 황금문
	if (RoomType::Treasure == Data_.RoomType_)
	{
		DefaultDoorType = DoorType::Treasure;
	}
	else if (RoomType::Boss == Data_.RoomType_)
	{
		DefaultDoorType = DoorType::Boss;
	}
	else if (RoomType::Shop == Data_.RoomType_)
	{
		DefaultDoorType = DoorType::Shop;
	}

	if (true == RandomRoomManager::GetInst()->ExistPos(Pos_ + float4(0, 1)))
	{
		Door* TmpDoor = GetLevel()->CreateActor<Door>(0);
		TmpDoor->SetPosition(GetPosition() + float4(0, 300));

		switch (RandomRoomManager::GetInst()->FindRoom(Pos_ + float4(0, 1))->Data_.RoomType_)
		{
		case RoomType::Treasure:
			TmpDoor->Setting(DoorType::Treasure, DoorDir::Down);
			if (1 != CurrentFloor)
			{
				TmpDoor->SetLock(true);
			}
			break;
		case RoomType::Boss:
			TmpDoor->Setting(DoorType::Boss, DoorDir::Down);
			break;
		case RoomType::Shop:
			TmpDoor->Setting(DoorType::Shop, DoorDir::Down);
			TmpDoor->SetLock(true);
			break;
		default:
			TmpDoor->Setting(DefaultDoorType, DoorDir::Down);
			break;
		}

		TmpDoor->SetCloseCollision(CloseCollisionVector_[0]);
		DoorVector_.push_back(TmpDoor);
	}
	if (true == RandomRoomManager::GetInst()->ExistPos(Pos_ + float4(0, -1)))
	{
		Door* TmpDoor = GetLevel()->CreateActor<Door>(0);
		TmpDoor->SetPosition(GetPosition() + float4(0, -300));

		switch (RandomRoomManager::GetInst()->FindRoom(Pos_ + float4(0, -1))->Data_.RoomType_)
		{
		case RoomType::Treasure:
			TmpDoor->Setting(DoorType::Treasure, DoorDir::Up);
			if (1 != CurrentFloor)
			{
				TmpDoor->SetLock(true);
			}
			break;
		case RoomType::Boss:
			TmpDoor->Setting(DoorType::Boss, DoorDir::Up);
			break;
		case RoomType::Shop:
			TmpDoor->Setting(DoorType::Shop, DoorDir::Up);
			TmpDoor->SetLock(true);
			break;
		default:
			TmpDoor->Setting(DefaultDoorType, DoorDir::Up);
			break;
		}

		TmpDoor->SetCloseCollision(CloseCollisionVector_[1]);
		DoorVector_.push_back(TmpDoor);
	}
	if (true == RandomRoomManager::GetInst()->ExistPos(Pos_ + float4(1, 0)))
	{
		Door* TmpDoor = GetLevel()->CreateActor<Door>(0);
		TmpDoor->SetPosition(GetPosition() + float4(500, 0));

		switch (RandomRoomManager::GetInst()->FindRoom(Pos_ + float4(1, 0))->Data_.RoomType_)
		{
		case RoomType::Treasure:
			TmpDoor->Setting(DoorType::Treasure, DoorDir::Right);
			if (1 != CurrentFloor)
			{
				TmpDoor->SetLock(true);
			}
			break;
		case RoomType::Boss:
			TmpDoor->Setting(DoorType::Boss, DoorDir::Right);
			break;
		case RoomType::Shop:
			TmpDoor->Setting(DoorType::Shop, DoorDir::Right);
			TmpDoor->SetLock(true);
			break;
		default:
			TmpDoor->Setting(DefaultDoorType, DoorDir::Right);
			break;
		}

		TmpDoor->SetCloseCollision(CloseCollisionVector_[2]);
		DoorVector_.push_back(TmpDoor);
	}
	if (true == RandomRoomManager::GetInst()->ExistPos(Pos_ + float4(-1, 0)))
	{
		Door* TmpDoor = GetLevel()->CreateActor<Door>(0);
		TmpDoor->SetPosition(GetPosition() + float4(-500, 0));

		switch (RandomRoomManager::GetInst()->FindRoom(Pos_ + float4(-1, 0))->Data_.RoomType_)
		{
		case RoomType::Treasure:
			TmpDoor->Setting(DoorType::Treasure, DoorDir::Left);
			if (1 != CurrentFloor)
			{
				TmpDoor->SetLock(true);
			}
			break;
		case RoomType::Boss:
			TmpDoor->Setting(DoorType::Boss, DoorDir::Left);
			break;
		case RoomType::Shop:
			TmpDoor->Setting(DoorType::Shop, DoorDir::Left);
			TmpDoor->SetLock(true);
			break;
		default:
			TmpDoor->Setting(DefaultDoorType, DoorDir::Left);
			break;
		}

		TmpDoor->SetCloseCollision(CloseCollisionVector_[3]);
		DoorVector_.push_back(TmpDoor);
	}

	OpenAllDoor();
}

void RoomActor::MakeMapReward()
{
	// 박스 확정 테스트
	//BoxItem* TmpObject = GetLevel()->CreateActor<BoxItem>();
	//TmpObject->SetType(BoxType::Gold);
	//TmpObject->SetPosition(GetPosition());
	//return;

	float RandomFloat = GameEngineRandom::MainRandom->RandomFloat(0.0f, 1.0f);
	if (0.0f <= RandomFloat && RandomFloat < 0.15f)
	{
		// 열쇠 1개
		KeyItem* TmpObject = GetLevel()->CreateActor<KeyItem>();
		TmpObject->SetType(KeyType::Normal);
		TmpObject->SetPosition(GetPosition());
		TmpObject->SetRoom(GetPos());
	}
	else if (0.15f <= RandomFloat && RandomFloat < 0.3f)
	{
		// 폭탄 1개
		BombItem* TmpObject = GetLevel()->CreateActor<BombItem>();
		TmpObject->SetType(BombType::Normal);
		TmpObject->SetPosition(GetPosition());
		TmpObject->SetRoom(GetPos());
	}
	else if (0.3f <= RandomFloat && RandomFloat < 0.45f)
	{
		// 1원
		MoneyItem* TmpObject = GetLevel()->CreateActor<MoneyItem>();
		TmpObject->SetType(MoneyType::Normal);
		TmpObject->SetPosition(GetPosition());
		TmpObject->SetRoom(GetPos());
	}
	else if (0.45f <= RandomFloat && RandomFloat < 0.5f)
	{
		// 열쇠 2개
		KeyItem* TmpObject = GetLevel()->CreateActor<KeyItem>();
		TmpObject->SetType(KeyType::Two);
		TmpObject->SetPosition(GetPosition());
		TmpObject->SetRoom(GetPos());
	}
	else if (0.5f <= RandomFloat && RandomFloat < 0.55f)
	{
		// 폭탄 2개
		BombItem* TmpObject = GetLevel()->CreateActor<BombItem>();
		TmpObject->SetType(BombType::Two);
		TmpObject->SetPosition(GetPosition());
		TmpObject->SetRoom(GetPos());
	}
	else if (0.55f <= RandomFloat && RandomFloat < 0.6f)
	{
		// 5원
		MoneyItem* TmpObject = GetLevel()->CreateActor<MoneyItem>();
		TmpObject->SetType(MoneyType::Black);
		TmpObject->SetPosition(GetPosition());
		TmpObject->SetRoom(GetPos());
	}
	else if (0.6f <= RandomFloat && RandomFloat < 0.65f)
	{
		// 그냥상자
		BoxItem* TmpObject = GetLevel()->CreateActor<BoxItem>();
		TmpObject->SetType(BoxType::Normal);
		TmpObject->SetPosition(GetPosition());
		TmpObject->SetRoom(GetPos());
	}
	else if (0.65f <= RandomFloat && RandomFloat < 0.7f)
	{
		// 황금상자
		BoxItem* TmpObject = GetLevel()->CreateActor<BoxItem>();
		TmpObject->SetType(BoxType::Gold);
		TmpObject->SetPosition(GetPosition());
		TmpObject->SetRoom(GetPos());
	}
	else if (0.7f <= RandomFloat && RandomFloat < 0.75f)
	{
		// 배터리
		BatteryItem* TmpObject = GetLevel()->CreateActor<BatteryItem>();
		TmpObject->SetPosition(GetPosition());
		TmpObject->SetRoom(GetPos());
	}
	else if (0.75f <= RandomFloat && RandomFloat < 0.775f)
	{
		// 마스터키
		KeyItem* TmpObject = GetLevel()->CreateActor<KeyItem>();
		TmpObject->SetType(KeyType::Master);
		TmpObject->SetPosition(GetPosition());
		TmpObject->SetRoom(GetPos());
	}
	else if (0.775f <= RandomFloat && RandomFloat < 0.8f)
	{
		// 10원
		MoneyItem* TmpObject = GetLevel()->CreateActor<MoneyItem>();
		TmpObject->SetType(MoneyType::Silver);
		TmpObject->SetPosition(GetPosition());
		TmpObject->SetRoom(GetPos());
	}
}

void RoomActor::MakePassive()
{
	// 패시브 아이템 세팅
	float4 TmpTilePos = { StartX + ScaleX * 6, StartY + ScaleY * 4 };
	PassiveItem* TmpPassiveItem = GetLevel()->CreateActor<PassiveItem>();
	TmpPassiveItem->SetPosition(GetPosition() + TmpTilePos);

	PassiveType TmpType;
	int count = 0;
	do
	{
		count++;
		TmpType = static_cast<PassiveType>(GameEngineRandom::MainRandom->RandomInt(1, static_cast<int>(PassiveType::Max) - 1));

	} while (Player::MainPlayer->IsAlreadyHave(TmpType) == true && count <= 100); // 이미 가지고있는지 확인해서 이미 가지고 있는 아이템이면 또 랜덤 돌리기

	TmpPassiveItem->Setting(TmpType);
}

std::string RoomActor::GetCurrentFloorName()
{
	std::string ReturnString = "";

	if (CurrentFloor == 1)
	{
		ReturnString += "01_basement";
	}
	else if (CurrentFloor == 2)
	{
		ReturnString += "03_caves";
	}
	else if (CurrentFloor == 3)
	{
		ReturnString += "05_depths";
	}

	if (RoomType::Boss == Data_.RoomType_)
	{
		ReturnString += "_Boss";
	}

	ReturnString += ".bmp";
	return ReturnString;
}

void RoomActor::DebugMonster()
{
	float4 TmpTilePos = { StartX + ScaleX * 1, StartY + ScaleY * 1 };
	if (true == GameEngineInput::GetInst()->IsPress("ctrl"))
	{
		if (true == GameEngineInput::GetInst()->IsDown("TestH"))
		{
			BoomFly* TmpMonster = GetLevel()->CreateActor<BoomFly>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
			TmpMonster->Setting(BoomFlyType::Normal);
		}

		if (true == GameEngineInput::GetInst()->IsDown("TestJ"))
		{
			BoomFly* TmpMonster = GetLevel()->CreateActor<BoomFly>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
			TmpMonster->Setting(BoomFlyType::Red);
		}

		if (true == GameEngineInput::GetInst()->IsDown("TestK"))
		{
			Mulliboom* TmpMonster = GetLevel()->CreateActor<Mulliboom>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
		}

		if (true == GameEngineInput::GetInst()->IsDown("TestL"))
		{
			Host* TmpMonster = GetLevel()->CreateActor<Host>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
			TmpMonster->SetType(HostType::Normal);
		}

		if (true == GameEngineInput::GetInst()->IsDown("TestN"))
		{
			Host* TmpMonster = GetLevel()->CreateActor<Host>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
			TmpMonster->SetType(HostType::Red);
		}

		if (true == GameEngineInput::GetInst()->IsDown("TestM"))
		{
			Fly* TmpMonster = GetLevel()->CreateActor<Fly>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
			TmpMonster->SetType(FlyType::Normal);
		}

		if (true == GameEngineInput::GetInst()->IsDown("Test,"))
		{
			Fly* TmpMonster = GetLevel()->CreateActor<Fly>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
			TmpMonster->SetType(FlyType::Red);
		}
		if (true == GameEngineInput::GetInst()->IsDown("Test."))
		{
			FloatingKnight* TmpMonster = GetLevel()->CreateActor<FloatingKnight>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
		}
	}
	else if (true == GameEngineInput::GetInst()->IsPress("shift"))
	{
		if (true == GameEngineInput::GetInst()->IsDown("TestH"))
		{
			Boil* TmpMonster = GetLevel()->CreateActor<Boil>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
		}

		if (true == GameEngineInput::GetInst()->IsDown("TestJ"))
		{
			Gut* TmpMonster = GetLevel()->CreateActor<Gut>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
		}

		if (true == GameEngineInput::GetInst()->IsDown("TestK"))
		{
			Spit* TmpMonster = GetLevel()->CreateActor<Spit>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
		}

		if (true == GameEngineInput::GetInst()->IsDown("TestL"))
		{
			Charger* TmpMonster = GetLevel()->CreateActor<Charger>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
		}
	}
	else
	{
		if (true == GameEngineInput::GetInst()->IsDown("TestH"))
		{
			Pooter* TmpMonster = GetLevel()->CreateActor<Pooter>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
		}

		if (true == GameEngineInput::GetInst()->IsDown("TestJ"))
		{
			Gaper* TmpMonster = GetLevel()->CreateActor<Gaper>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
		}

		if (true == GameEngineInput::GetInst()->IsDown("TestK"))
		{
			Gusher* TmpMonster = GetLevel()->CreateActor<Gusher>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
		}

		if (true == GameEngineInput::GetInst()->IsDown("TestL"))
		{
			Sucker* TmpMonster = GetLevel()->CreateActor<Sucker>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
		}
		
		if (true == GameEngineInput::GetInst()->IsDown("TestN"))
		{
			RoundWorm* TmpMonster = GetLevel()->CreateActor<RoundWorm>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
		}

		if (true == GameEngineInput::GetInst()->IsDown("TestM"))
		{
			Clot* TmpMonster = GetLevel()->CreateActor<Clot>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
		}

		if (true == GameEngineInput::GetInst()->IsDown("Test,"))
		{
			Clotty* TmpMonster = GetLevel()->CreateActor<Clotty>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
		}
		if (true == GameEngineInput::GetInst()->IsDown("Test."))
		{
			Iblob* TmpMonster = GetLevel()->CreateActor<Iblob>();
			TmpMonster->SetPosition(GetPosition() + TmpTilePos);
			TmpMonster->SetRoom(*this);
		}
	}
}
