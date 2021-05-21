#include"Character.h"
#include"Lib_and_const.h"
#include"Image_manager.h"
#include"Set_map.h"
#include"Imp_timer.h"
#include"Monster.h"
#include"Dead_monster.h"
#include"Text.h"
#include "Special.h"

TTF_Font* font_mark = NULL;

bool Init() {
	bool success = true;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Failed to initialize the SDL2 library\n";
		std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
		success = false;
	}
	else {
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
		//=>giúp hình ảnh đc mượt mà hơn (phải gọi trc khi tạo texture)

		g_window = SDL_CreateWindow("GameCPP SDL2.0", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if (g_window == NULL) {
			success = false;
			std::cout << "Failed to create window\n";
			std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
		}
		else {
			g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
			//-1 =>  máy tự định nghĩa
			//đường dẫn kết xuất Android 2D hỗ trợ tăng tốc phần cứng, 
			//nghĩa là tất cả các thao tác vẽ được thực hiện trên canvas của Chế độ xem đều sử dụng GPU.
			//Do cần tăng tài nguyên để kích hoạt tăng tốc phần cứng, ứng dụng của bạn sẽ ngốn nhiều RAM hơn

			if (g_screen == NULL) {
				std::cout << "Failed to create renderer\n";
				std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
				success = false;
			}
			else {
				SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
				// tạo màu nền cho vùng g_screen
				if (!(IMG_Init(IMG_INIT_PNG) && IMG_INIT_PNG)) {
					success = false;
					std::cout << "SDL_image could not initialize!\n;";
					std::cout << "SDL2 Error: " << SDL_GetError();
				}

				if (TTF_Init() == -1)
				{
					success = false;
				}
				font_mark = TTF_OpenFont("font//dlxfont_.ttf", 15);
				if (font_mark == NULL)
				{
					success = false;
				}

				if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) return false;
	
				g_sound_bullet = Mix_LoadWAV("mixer//Arrow+Swoosh+1.wav");
				g_sound_dead = Mix_LoadWAV("mixer//mixkit-boxer-getting-hit-2055.wav");
				// khởi tạo các file âm thanh
				if (g_sound_dead == NULL || g_sound_bullet == NULL)
				{
					success = false;
				}
			}
		}
	}
	return success;
}

void close() {

	SDL_DestroyRenderer(g_screen);
	g_screen = NULL;

	SDL_DestroyWindow(g_window);
	g_window = NULL;

	IMG_Quit();
	SDL_Quit();

}

void update(Image_manager& background, GameMap& game_map, Character& p_player, ImpTimer& fps_timer, std::vector<Monster*>& monsters_list
	, Dead_monster& dead_monster, int& die, Text mark_game, UINT& mark_value, Text& money_game, Player& i_player, bool& is_quit) {


	SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);

	SDL_RenderClear(g_screen);

	background.Render(g_screen, NULL);

	Map map_data = game_map.getMap();

	p_player.HandleBullet(g_screen);
	p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
	p_player.DoPlayer(map_data);
	p_player.Show(g_screen);

	game_map.SetMap(map_data);
	game_map.RenderMap(g_screen);

	i_player.Show(g_screen);

	for (int i = 0; i < monsters_list.size(); i++)
	{
		Monster* p_monster = monsters_list.at(i);
		if (p_monster != NULL)
		{
			p_monster->SetMapXY(map_data.start_x_, map_data.start_y_);
			p_monster->ImpMoveType(g_screen);
			p_monster->DoPlayer(map_data);
			// phát triển thêm khi quái di chuyển
			p_monster->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
			
			p_monster->Show(g_screen);

			// NHÂN VẬT TRÚNG ĐẠN =>
			SDL_Rect rect_player = p_player.GetRectFrame();
			// vị trí character để check vs đạn của quái
			bool bCol1 = false;
			std::vector<BulletObject*> tBullet_list = p_monster->get_bullet_list();
			for (int jj = 0; jj < tBullet_list.size(); jj++)
			{
				BulletObject* pt_bullet = tBullet_list.at(jj);
				if (pt_bullet)
				{
					bCol1 = SDLCommonFunc::CheckCollision(pt_bullet->GetRect(), rect_player);
					// check va chạm vs nhân vật
					if (bCol1)
					{
						Mix_PlayChannel(-1, g_sound_dead, 0);
						// âm thanh nhân vật trúng đạn
						p_monster->RemoveBullet(jj);
						// xóa đạn
						break;
					}
				}
			}
			if (bCol1)
			{
				die--;
				if (die > 0)
				{
					p_player.SetRect(0, 0);
					p_player.set_comeback_time(60);
					SDL_Delay(1000);
					i_player.Decrease();
					i_player.Render(g_screen, NULL);
					continue;
					// nếu còn mạng thì xuất hiện lại sau 1 tg
				}
				else
				{
					if (MessageBox(NULL, L"GAME OVER", L"Warning", MB_OK | MB_ICONSTOP) == IDOK)
					{
						p_monster->Free();
						close();
						SDL_Quit();
					}
					// hết mạng thì in ra thông báo và giải phóng bộ nhớ ...
				}
			}
		}
	}
	// thao tác với monster (.) list

	int frame_dead_width = dead_monster.get_frame_width();
	int frame_dead_height = dead_monster.get_frame_height();
	// chiều dài, rộng của tấm ảnh quái chết


	std::vector<BulletObject*> bullet_arr = p_player.get_bullet_list();
	for (int r = 0; r < bullet_arr.size(); r++)
	{
		BulletObject* p_bullet = bullet_arr.at(r);
		if (p_bullet != NULL)
		{
			for (int t = 0; t < monsters_list.size(); t++)
			{
				Monster* obj_monster_ = monsters_list.at(t);
				if (obj_monster_ != NULL)
				{
					SDL_Rect tRect;
					tRect.x = obj_monster_->GetRect().x;
					tRect.y = obj_monster_->GetRect().y;
					tRect.w = obj_monster_->get_width_frame();
					tRect.h = obj_monster_->get_height_frame();
					// vị trí, kích thước con quái
					SDL_Rect bRect = p_bullet->GetRect();
					// vị trí viên đạn
					
					bool bcol = SDLCommonFunc::CheckCollision(bRect, tRect);
					// va chạm giữa con quái và vũ khí nhân vật

					if (bcol)
					{
						mark_value++;
						for (int ex = 0; ex < NUM_FRAME_DEAD; ex++)
						{
							int x_pos = p_bullet->GetRect().x;
							int y_pos = p_bullet->GetRect().y - frame_dead_height * 0.5;

							dead_monster.set_frame(ex);
							dead_monster.SetRect(x_pos, y_pos);
							dead_monster.Show(g_screen);
						}
						Mix_PlayChannel(-1, g_sound_dead, 0);
						// tạo hoạt ảnh quái chết và tiếng khi giết quái

						p_player.RemoveBullet(r);
						obj_monster_->Free();
						monsters_list.erase(monsters_list.begin() + t);
						// xóa con quái đi
					}
				}
			}
		}
	}

	// => giết quái 

	std::string val_str_mark = std::to_string(mark_value);
	std::string strMark("Monster: ");
	strMark += val_str_mark;

	mark_game.SetText(strMark);
	mark_game.LoadFromRenderText(font_mark, g_screen);  //chung font
	mark_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 50, 15);

	int money_count = p_player.GetMoneyCount();
	std::string money_str = std::to_string(money_count);
	std::string strCoin("Coin: ");
	strCoin += money_str;

	money_game.SetText(strCoin);
	money_game.LoadFromRenderText(font_mark, g_screen);
	money_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 250, 15);

	// hiển thị text 


	if (mark_value >= 10)
	{
		if (MessageBox(NULL, L"You win", L"Warning", MB_OK | MB_ICONEXCLAMATION) == IDOK)
		{
			is_quit = true;

		}
	}


	SDL_RenderPresent(g_screen);

	int real_imp_time = fps_timer.get_ticks();// tg thực tế ta có đc
	int time_one_frame = 1000 / FRAME_PER_SECOND; //tg muốn chạy cho 1 frame 

	if (real_imp_time < time_one_frame)
	{
		int delay_time = time_one_frame - real_imp_time;
		if (delay_time >= 0) SDL_Delay(delay_time);
	}

	//tạo độ trễ để cân bằng chương trình nếu tg 1 frame chưa đủ
}

std::vector<Monster*> MakeMonstersList() {
	std::vector<Monster*> list_monsters;
	int rand_x;

	Monster* monsters_objs = new Monster[MONSTER_NUM];
	for (int i = 0; i < MONSTER_NUM; i++) {
		rand_x = rand() % (500 - 200 + 1) + 200;

		Monster* p_monster = monsters_objs + i;
		// duyệt từng monster
		if (p_monster != NULL) {
			p_monster->LoadImg("img/agent.png", g_screen);
			p_monster->set_clips();
			p_monster->set_x_pos(700 + i * rand_x);
			// tạo vị trí quái => random khoảng cách 200 - 500
			p_monster->set_y_pos(250);
			// chọn vị trí quái rơi
			p_monster->set_type_move(Monster::STATIC_MONSTER);
			// chọn kiểu quái( di chuyển hay không => phát triển thêm)

			BulletObject* p_bullet = new BulletObject();
			p_monster->InitBullet(p_bullet, g_screen);
			// tạo đạn cho quái

			list_monsters.push_back(p_monster);
			// cho vào list
		}
	}
	return list_monsters;
}
// tạo danh sách monster


int main(int argc, char* args[]) {

	ImpTimer fps_timer;

	if (Init() == false)
		return 1;

	Image_manager background;
	background.loadImg("img/background.png", g_screen);

	GameMap game_map;
	game_map.LoadMap("map/map01.dat", g_screen);;

	Character p_player;
	p_player.LoadImg("img//player_right.png", g_screen);
	p_player.set_clips();

	Player i_player;
	i_player.Init(g_screen);

	std::vector<Monster*> monster_list = MakeMonstersList();
	// tạo 1 danh sách quái

	Dead_monster dead_monster;
	bool tRet = dead_monster.loadImg("img//dead.png", g_screen);
	if (!tRet) return -1;
	dead_monster.set_clip();
	//monster

	Text mark_game;
	mark_game.SetColor(Text::WHITE_TEXT);
	// chọn màu trắng
	UINT mark_value = 0;

	Text money_game;
	money_game.SetColor(Text::WHITE_TEXT);

	

	bool is_quit = false;
	int die = HEARTS;



	while (!is_quit) {
		fps_timer.start();
		// tính thời điểm bắt đầu (start_tick_)
		while (SDL_PollEvent(&g_event) != 0) {
			if (g_event.type == SDL_QUIT) {
				is_quit = true;
			}
			p_player.HandelInputAction(g_event, g_screen, g_sound_bullet);
		}
		update(background, game_map, p_player, fps_timer, monster_list, dead_monster, die
			  , mark_game, mark_value, money_game, i_player, is_quit);
	}

	close();
	return 0;
}

