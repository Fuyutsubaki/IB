
# include <Siv3D.hpp>
#include<Windows.h>

#include"StgPart.h"
#include"stggame.h"
#include<boost\variant.hpp>
//Bombのデザイン
//UIの確定
//中身の実装



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

	::AllocConsole();
	FILE *console;
	::freopen_s(&console, "CONOUT$", "w", stdout);

	auto &stggame=StgGame::get();
	
	stggame.init();
	bool stop = false;
	while (System::Update())
	{
		if (Input::KeyR.clicked)stggame.init(), std::cout << "\n\n=========================\n\n";
		if (Input::Key0.clicked)stop = !stop;
		if (stop)continue;
		stggame.updata();
	}
	s3d::TransformedMesh s;
}
