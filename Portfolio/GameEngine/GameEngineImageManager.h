#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include <map>
#include <string>
#include "GameEngineImage.h"

// ���� :
class GameEngineImageManager
{
private:
	static GameEngineImageManager* inst_;
public:
	inline static GameEngineImageManager* GetInst() 
	{
		return inst_;
	}
	
	inline static void Destroy()
	{
		if (nullptr == inst_)
		{
			return;
		}

		delete inst_;
		inst_ = nullptr;
	}

public:
	GameEngineImage* Find(const std::string& _name);
	GameEngineImage* Create(const std::string& _name, const float4& _scale);
	GameEngineImage* Create(const std::string& _name, HDC _scale);
	GameEngineImage* Load(const std::string& _path);
	GameEngineImage* Load(const std::string& _path, const std::string& _name);

protected:

private:
	// constrcuter destructer
	GameEngineImageManager();
	~GameEngineImageManager();

	// delete Function
	GameEngineImageManager(const GameEngineImageManager& _Other) = delete;
	GameEngineImageManager(GameEngineImageManager&& _Other) noexcept = delete;
	GameEngineImageManager& operator=(const GameEngineImageManager& _Other) = delete;
	GameEngineImageManager& operator=(GameEngineImageManager&& _Other) noexcept = delete;

	std::map<std::string, GameEngineImage*> allRes;
};
