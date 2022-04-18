#pragma once
#include <map>
#include <vector>
#include "MapPos.h"
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineLevel.h>

// ���� :
union MapPos;
class RoomData;
class RoomActor;
class RandomRoomManager : public GameEngineActor
{
private:
	static RandomRoomManager* Inst_;
public:
	inline static RandomRoomManager* GetInst()
	{
		return Inst_;
	}
	inline static void SetInst(RandomRoomManager* _Value)
	{
		Inst_ = _Value;
	}
	inline static void Destroy()
	{
		if (nullptr == Inst_)
		{
			return;
		}

		delete Inst_;
		Inst_ = nullptr;
	}

	bool ChangeFloor(const int& _floor);
	float4 RandomPos();
	RoomData RandomData();
	bool ExistPos(float4 _Pos);
	RoomActor* FindRoom(float4 _Pos);

protected:

private:
	friend GameEngineLevel;

	std::map<int, std::vector<RoomData>> AllMaps_;
	std::vector<RoomActor*> CurrentRooms_;
	MapPos CurrentMapPos_;
	int CurrentFloor_;
	int CurrentMapCount_;

	// constrcuter destructer
	RandomRoomManager();
	~RandomRoomManager();

	// delete Function
	RandomRoomManager(const RandomRoomManager& _Other) = delete;
	RandomRoomManager(RandomRoomManager&& _Other) noexcept = delete;
	RandomRoomManager& operator=(const RandomRoomManager& _Other) = delete;
	RandomRoomManager& operator=(RandomRoomManager&& _Other) noexcept = delete;

	void Start() override;
};