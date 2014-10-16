
# include <Siv3D.hpp>

#include"StgPart.h"
#include"stggame.h"
class testBt
	:public stgpart::Bullet
{
	int x = 30;
	int y = 30;
	bool alive = true;
public:
	void updata(stgpart::TaskMediator &)
	{
		x += 1;
		y += 1;
		Circle(x, y, 5).draw();
	}
	bool isAlive()const override
	{
		return alive;
	}
	void onHitFlag()override
	{
		alive = false;
	}
	Circle getSharp()override
	{
		return Circle(x, y, 5);
	}
};


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
	

	auto &stggame=StgGame::get();
	stggame.init();

	while (System::Update())
	{
		if (Input::KeyR.clicked)stggame.init();
		stggame.updata();
	}
}
