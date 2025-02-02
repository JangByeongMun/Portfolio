#pragma once
#include <Windows.h>
#include <string>
#include "GameEngineMath.h"

// ���� :
class GameEngineWindow
{
private:
	static GameEngineWindow* Inst_;
public:
	inline static GameEngineWindow& GetInst()
	{
		return *Inst_;
	}
	inline static void Destroy()
	{
		if (nullptr != Inst_)
		{
			delete Inst_;
			Inst_ = nullptr;
		}
	}

public:
	void RegClass(HINSTANCE _hInst);
	void CreateGameWindow(HINSTANCE _hInst, const std::string& _Title);
	void ShowGameWindow();
	void MessageLoop(void(*_InitFunction)(), void(*_LoopFunction)());
	void SetWindowScaleAndPosition(float4 _pos, float4 _scale);
	void Off();

	inline static HDC GetHDC()
	{
		return Inst_->hDC_;
	}
	inline static float4 GetScale()
	{
		return Inst_->Scale_;
	}

protected:

private:
	std::string Title_;
	bool WindowOn_;
	HINSTANCE hInst_;
	HWND hWnd_;
	HDC hDC_;
	float4 Scale_;
	
	// constrcuter destructer
	GameEngineWindow();
	~GameEngineWindow();

	// delete Function
	GameEngineWindow(const GameEngineWindow& _Other) = delete;
	GameEngineWindow(GameEngineWindow&& _Other) noexcept = delete;
	GameEngineWindow& operator=(const GameEngineWindow& _Other) = delete;
	GameEngineWindow& operator=(GameEngineWindow&& _Other) noexcept = delete;

	static LRESULT CALLBACK MessageProcess(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
}; 