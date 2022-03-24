#pragma once
#include "GameEnginePath.h"

class GameEngineFile;
class GameEngineDirectory : public GameEnginePath
{
public:
	// constrcuter destructer
	GameEngineDirectory();
	~GameEngineDirectory();

	// delete Function
	GameEngineDirectory(const GameEngineDirectory& _Other) = delete;
	GameEngineDirectory(GameEngineDirectory&& _Other) noexcept = delete;
	GameEngineDirectory& operator=(const GameEngineDirectory& _Other) = delete;
	GameEngineDirectory& operator=(GameEngineDirectory&& _Other) noexcept = delete;

	void Move(const std::string& _name);
	void MoveParent();
	void MoveParent(const std::string& _name);
	bool IsRoot();

	// Ȯ���ڸ��� �޾Ƽ� ������ġ�� �ش� Ȯ���ڸ� ���� ���ϵ� ���� ���������Լ�
	std::vector<GameEngineFile> GetAllFile(const std::string& _Ext = "");

protected:

private:

};
