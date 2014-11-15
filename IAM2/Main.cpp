
# include <Siv3D.hpp>
#include<Windows.h>

#include"StgPart.h"
#include"stggame.h"
#include<boost\variant.hpp>
//UI‚ÌŠm’è
//’†g‚ÌÀ‘•
//score 


void Main()
{
	/*auto test = stgpart::BulletManeger{};
	auto player = std::make_shared<stgpart::PlayerShipManeger>();
	auto tasks = std::make_shared<stgpart::TaskMediator>();
	auto input = std::make_shared<InputKey>();
	auto ch = std::make_shared<stgpart::CheckHit>();
	auto chAB = std::make_shared<stgpart::CheckHitAtkBt>();
	auto patk = std::make_shared<stgpart::PlayerAtackManeger>();
	test.init(tasks);
	player->init(tasks);
	tasks->input = input;
	tasks->playerAtkmane = patk.get();
	player->add(std::make_shared<testPlayer>());


	while (System::Update())
	{
	if (Input::KeyR.clicked)test.init(tasks);
	if (Input::KeyP.clicked)player->add(std::make_shared<testPlayer>());
	if (Input::KeySpace.clicked)test.add(std::make_shared<testBt>());
	test.updata(*tasks);
	player->updata(*tasks);
	ch->updata(*tasks);
	chAB->updata(*tasks);
	patk->updata(*tasks);
	}*/
	/*::AllocConsole();
	FILE *console;
	::freopen_s(&console, "CONOUT$", "w", stdout);*/
	Window::SetTitle(L"‚Í‚é‚Í‚ ‚¯‚Ú‚ÌA‰ä‚Í’eŠÛ");
	Window::Resize(960, 600);
	StgGame stggame ;
	
	stggame.init();
	bool stop = false;
	while (System::Update())
	{
		stggame.updata();
	}

}
