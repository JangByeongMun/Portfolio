#include "BindingOfIsaac.h"
#include "TitleLevel.h"
#include "PlayLevel.h"
#include "LoadingLevel.h"
#include "EndingLevel.h"
#include "MenuLevel.h"
#include <conio.h>
#include <GameEngineBase/GameEngineWindow.h>
#include <GameEngine/GameEngineImageManager.h>
#include <string>
#include <time.h>

std::string home = "D:\\AssortRock\\ApiPortfolio\\Portfolio\\Resources\\Image";
std::string notebook = "C:\\CppProject\\ApiPortfolio\\Portfolio\\Resources\\Image";

BindingOfIsaac::BindingOfIsaac() 
{
}

BindingOfIsaac::~BindingOfIsaac() 
{
}

time_t startTime = 0;
void BindingOfIsaac::GameInit()
{
	GameEngineWindow::GetInst().SetWindowScaleAndPosition({ 100, 100 }, { 1280, 720 });
	GameEngineImageManager::GetInst()->Load(notebook + "\\Player\\Player.bmp", "Player.bmp");
	GameEngineImageManager::GetInst()->Load(notebook + "\\UI\\Title.bmp", "Title.bmp");
	GameEngineImageManager::GetInst()->Load(notebook + "\\UI\\TitleLogo.bmp", "TitleLogo.bmp");
	GameEngineImageManager::GetInst()->Load(notebook + "\\UI\\TitleLogoText.bmp", "TitleLogoText.bmp");
	GameEngineImageManager::GetInst()->Load(notebook + "\\UI\\MakeBy.bmp", "MakeBy.bmp");
	GameEngineImageManager::GetInst()->Load(notebook + "\\UI\\Lobby.bmp", "Lobby.bmp");

	CreateLevel<TitleLevel>("Title");
	CreateLevel<MenuLevel>("Menu");
	CreateLevel<PlayLevel>("Play");
	CreateLevel<EndingLevel>("Ending");
	CreateLevel<LoadingLevel>("Loading");
	ChangeLevel("Title");

	startTime = time(NULL);
}

void BindingOfIsaac::GameLoop()
{
	time_t currentTime = time(NULL);
	if (currentTime - startTime >= 5)
	{
		if (currentTime - startTime >= 10)
		{
			ChangeLevel("Play");
		}
		else
		{
			ChangeLevel("Menu");
		}
	}
	
}

void BindingOfIsaac::GameEnd()
{
	int a = 0;
}
