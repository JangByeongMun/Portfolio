#include <GameEngine/GameEngine.h>
#include "TitleLevel.h"
#include "TitleBackGround.h"
#include "IntroPage.h"
#include "TitleForeGround.h"
#include "Intro01.h"
#include "Intro02.h"
#include "Intro03.h"
#include "Intro04.h"
#include "Intro05.h"
#include "Intro06.h"
#include "Intro07.h"
#include "Intro08.h"
#include "Intro09.h"
#include "Intro10.h"
#include "Intro11.h"
#include "Intro12.h"
#include "Intro13.h"
#include "Intro14.h"
#include "Intro15.h"
#include "Intro16.h"
#include "Intro17.h"
#include "Intro18.h"
#include "Intro19.h"
#include "Intro20.h"
#include "Intro21.h"
#include "Intro22.h"
#include "Intro23.h"
#include "Intro24.h"
#include "Intro25.h"
#include "IntroManager.h"

TitleLevel::TitleLevel() 
{
}

TitleLevel::~TitleLevel() 
{
}

void TitleLevel::Loading()
{
	CreateActor<TitleBackGround>(0);
	//CreateActor<IntroPage>(1);
	CreateActor<IntroManager>(1);

	//CreateActor<Intro01>(1);
	//CreateActor<Intro02>(1);
	//CreateActor<Intro03>(1);
	//CreateActor<Intro04>(1);
	//CreateActor<Intro05>(1);
	//CreateActor<Intro06>(1);
	//CreateActor<Intro07>(1);
	//CreateActor<Intro08>(1);
	//CreateActor<Intro09>(1);
	//CreateActor<Intro10>(1);
	//CreateActor<Intro11>(1);
	//CreateActor<Intro12>(1);
	//CreateActor<Intro13>(1);
	//CreateActor<Intro14>(1);
	//CreateActor<Intro15>(1);
	//CreateActor<Intro16>(1);
	//CreateActor<Intro17>(1);
	//CreateActor<Intro18>(1);
	//CreateActor<Intro19>(1);
	//CreateActor<Intro20>(1);
	//CreateActor<Intro21>(1);
	//CreateActor<Intro22>(1);
	//CreateActor<Intro23>(1);
	//CreateActor<Intro24>(1);
	//CreateActor<Intro25>(1);

	//CreateActor<TitleForeGround>(10);
}

void TitleLevel::Update()
{
}

void TitleLevel::LevelChangeStart()
{
}

void TitleLevel::LevelChangeEnd()
{
}