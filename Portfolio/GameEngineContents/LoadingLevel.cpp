#include "LoadingLevel.h"
#include "LoadingPage.h"
#include "LoadingCharacter.h"
#include "LoadingMap.h"
#include "LoadingBubble.h"

LoadingLevel::LoadingLevel() 
{
}

LoadingLevel::~LoadingLevel() 
{
}

void LoadingLevel::Loading()
{
	CreateActor<LoadingPage>(0);
	CreateActor<LoadingCharacter>(1);
	CreateActor<LoadingBubble>(1);
	CreateActor<LoadingMap>(2);
}

void LoadingLevel::Update()
{
}