#pragma once
#include "GameEngineNameObject.h"
#include "ThirdParty/inc/fmod.hpp"
#include <map>

class GameEngineSoundPlayer;
// 설명 :
class GameEngineSound : public GameEngineNameObject
{
public:
	// constrcuter destructer
	GameEngineSound();
	~GameEngineSound();

	// delete Function
	GameEngineSound(const GameEngineSound& _Other) = delete;
	GameEngineSound(GameEngineSound&& _Other) noexcept = delete;
	GameEngineSound& operator=(const GameEngineSound& _Other) = delete;
	GameEngineSound& operator=(GameEngineSound&& _Other) noexcept = delete;

protected:
	bool Load(const std::string& _Path);

private:
	FMOD::Sound* Sound;

	///////////////////////////////////////// 매니지먼트 사운드 기능
public:
	static GameEngineSoundPlayer SoundPlayControl(const std::string& _Name);

	static void SoundPlayOneShot(const std::string& _Name);
	static void Update();

////////////////////////////////////////// 리소스 매니지먼트
public:
	static GameEngineSound* FindRes(const std::string& _Name);
	static GameEngineSound* LoadRes(const std::string& _Path);
	static GameEngineSound* LoadRes(const std::string& _Path, const std::string& _Name);

	static void AllResourcesDestroy();

private:
	static std::map<std::string, GameEngineSound*> AllRes;

};

// 사운드 재생을 제어할수 있는 기능들을 넣을겁니다.
class GameEngineSound;
class GameEngineSoundPlayer 
{
	friend GameEngineSound;

public:
	void Play();
	void Stop();
	void SetVolume(float _Volume);

	GameEngineSoundPlayer();
	GameEngineSoundPlayer(const GameEngineSoundPlayer& _Other);
	~GameEngineSoundPlayer();

private:
	GameEngineSound* Sound_;
	FMOD::Channel* ControlHandle_;

	GameEngineSoundPlayer(GameEngineSound* Sound, FMOD::Channel* ControlHandle);
};