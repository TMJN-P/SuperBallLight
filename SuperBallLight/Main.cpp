﻿#include <Siv3D.hpp> // OpenSiv3D v0.4.1

struct player_ball {
	Circle circle;
	Color color;
	Vec2 velocity;
	Vec2 before_pos;

	player_ball() {
		before_pos = circle.center = Vec2(325, 480);
		circle.r = 20;
		color = Palette::Orange;
	}

	void move(Array<Rect>& scaffold, double& camera_pos) {
		if (Cursor::PosF().x < circle.x - 5) {
			velocity.x -= 0.2;
		}
		else if (Cursor::PosF().x > circle.x + 5) {
			velocity.x += 0.2;
		}
		else {
			velocity.x *= 0.9;
		}
		if (circle.x < 70) {
			circle.x = 70;
			velocity.x *= -0.3;
		}
		if (circle.x > 580) {
			circle.x = 580;
			velocity.x *= -0.3;
		}
		velocity.x *= 0.95;
		gravity(0.45);
		for (Rect& r : scaffold) {
			if (r.intersects(circle) && before_pos.y <= r.y - circle.r && r.x <= circle.x && circle.x < r.x + r.w) {
				circle.center.y = r.y - circle.r;
				velocity = Vec2(0, 0);
				jump(20);
			}
		}
		if (camera_pos + circle.y < 300) {
			camera_pos = 300 - circle.y;
		}
		if (camera_pos + circle.y > 500) {
			camera_pos = 500 - circle.y;
		}
		before_pos = circle.center;
		velocity.y = Max(-30.0, velocity.y);
		velocity.y = Min(30.0, velocity.y);
		circle.center += velocity;
	}
	void draw(double& camera_pos) {
		circle.movedBy(0, camera_pos).draw(color);
	}
	void gravity(double d) {
		velocity.y += d;
	}
	void jump(double d) {
		velocity.y = Min(0.0, velocity.y);
		velocity.y -= d;
	}
};

void Main() {
	Scene::SetBackground(Color(20, 20, 30));
	Window::Resize(1000, 600);
	Window::SetTitle(U"Super Ball Light");
	Graphics::SetTargetFrameRateHz(60);

	const Rect ground(50, 500, 550, 100);
	const Rect left_wall(0, 0, 50, 600);
	const Rect right_wall(600, 0, 400, 600);
	const Rect timer_box(610, 330, 380, 100);
	const Rect instruction_box(50, 450, 400, 100);
	const Rect play_box(550, 450, 400, 100);
	const Rect time_attack_box(50, 50, 900, 180);
	const Rect score_attack_box(50, 280, 900, 180);
	const Rect return_box(50, 500, 140, 70);
	const Rect goal_line(50, -14530, 550, 10);

	const Array<Color>color_palette{
		Color(200, 0, 0), Color(0, 200, 0), Color(100, 100, 200), Color(200, 200, 0), Color(200, 0, 200), Color(0, 200, 200)
	};
	const Array<Shape2D> title_star{
		Shape2D::Star(13, Vec2(30, 80)), Shape2D::Star(13, Vec2(250, 340)), Shape2D::Star(13, Vec2(950, 400)), Shape2D::Star(13, Vec2(850, 100)), Shape2D::Star(13, Vec2(400, 390)), Shape2D::Star(13, Vec2(100, 300))
	};
	const Polygon logo_s{
		{Vec2(170, 30), Vec2(260, 30), Vec2(260, 60), Vec2(200, 60), Vec2(200, 90), Vec2(260, 90), Vec2(260, 180), Vec2(170, 180), Vec2(170, 150), Vec2(230, 150), Vec2(230, 120), Vec2(170, 120)}
	};
	const Polygon logo_u{
		{Vec2(290, 30), Vec2(320, 30), Vec2(320, 150), Vec2(350, 150), Vec2(350, 30), Vec2(380, 30), Vec2(380, 180), Vec2(290, 180)}
	};
	const Polygon logo_p{
		{Vec2(410, 30), Vec2(500, 30), Vec2(500, 120), Vec2(440, 120), Vec2(440, 180), Vec2(410, 180)},
		{{Vec2(440, 60), Vec2(440, 90), Vec2(470, 90), Vec2(470, 60)}}
	};
	const Polygon logo_e{
		{Vec2(530, 30), Vec2(620, 30), Vec2(620, 60), Vec2(560, 60), Vec2(560, 90), Vec2(620, 90), Vec2(620, 120), Vec2(560, 120), Vec2(560, 150), Vec2(620, 150), Vec2(620, 180), Vec2(530, 180)}
	};
	const Polygon logo_r{
		{Vec2(650, 30), Vec2(725, 30), Vec2(740, 45), Vec2(740, 90), Vec2(725, 105), Vec2(740, 120), Vec2(740, 180), Vec2(710, 180), Vec2(710, 120), Vec2(680, 120), Vec2(680, 180), Vec2(650, 180)},
		{{Vec2(680, 60), Vec2(680, 90), Vec2(710, 90), Vec2(710, 60)}}
	};
	const Polygon logo_b{
		{Vec2(410, 210), Vec2(485, 210), Vec2(500, 225), Vec2(500, 270), Vec2(485, 285), Vec2(500, 300), Vec2(500, 360), Vec2(410, 360)},
		{{Vec2(440, 240), Vec2(440, 270), Vec2(470, 270), Vec2(470, 240)}, {Vec2(440, 300), Vec2(440, 330), Vec2(470, 330), Vec2(470, 300)}}
	};
	const Polygon logo_a{
		{Vec2(560, 210), Vec2(590, 210), Vec2(620, 240), Vec2(620, 360), Vec2(590, 360), Vec2(590, 300), Vec2(560, 300), Vec2(560, 360), Vec2(530, 360), Vec2(530, 240)},
		{{Vec2(560, 240), Vec2(560, 270), Vec2(590, 270), Vec2(590, 240)}}
	};
	const Polygon logo_l{
		{Vec2(650, 210), Vec2(680, 210), Vec2(680, 330), Vec2(740, 330), Vec2(740, 360), Vec2(650, 360)}
	};
	const Polygon logo_l2 = logo_l.movedBy(120, 0);

	const Font font20_regular(20, Typeface::Regular);
	const Font font30_regular(30, Typeface::Regular);
	const Font font40_regular(40, Typeface::Regular);
	const Font font50_regular(50, Typeface::Regular);
	const Font font70_regular(70, Typeface::Regular);
	const Font font100_regular(100, Typeface::Regular);
	const Font font255_regular(255, Typeface::Regular);
	const Font font30_medium(25, Typeface::Medium);
	const Font font50_medium(45, Typeface::Medium);
	const Font font60_medium(60, Typeface::Medium);
	const Font font100_medium(100, Typeface::Medium);
	const Font font120_medium(120, Typeface::Medium);
	const Font font200_medium(200, Typeface::Medium);
	const Font font40_heavy(40, Typeface::Heavy);
	const Font font60_heavy(60, Typeface::Heavy);
	const Font font70_heavy(70, Typeface::Heavy);
	const Font font255_heavy(255, Typeface::Heavy);

	Array<Rect>scaffold;
	Array<std::pair<Polygon, Color>> star;
	Array<int>result;
	player_ball ball;
	int status = 0;//0:タイトル 1:せつめい 2:モード選択 3:難易度選択 10:ゲーム開始前 11:ゲーム本体 12:ゴール演出 13:時間切れ演出 14:結果発表
	int mode;//0:タイムアタック 1:スコアアタック
	int timer;
	int timer2 = 240;
	double camera_pos = 0;
	scaffold.push_back(Rect(50, 500, 550, 100));
	for (int i = -Random(400, 450); i <= 500000; i += Random(0, 200)) {
		star.push_back(std::make_pair(Shape2D::NStar(Random(5, 10), 7, 4, Vec2(Random(110, 590), -i)).asPolygon(), color_palette[Random(0, 5)]));
	}
	TextReader result_reader;
	bool is_text_correct = true;
	if (result_reader.open(U"result.txt")) {
		String line;
		while (result_reader.readLine(line)) {
			result.push_back(ParseOr<int>(line, -0xE869120));
			if (result.back() == -0xE869120) {
				is_text_correct = false;
				break;
			}
		}
	}
	else {
		is_text_correct = false;
	}
	result_reader.close();
	if (result.size() == 11) {
		int t = 0;
		for (int i = 0; i < 10; i++) {
			t ^= result[i];
		}
		if (t != result[10]) {
			is_text_correct = false;
		}
	}
	else {
		is_text_correct = false;
	}
	if (!is_text_correct) {
		result.clear();
		for (int i = 0; i < 5; i++) {
			result.push_back(0xE869120);
		}
		for (int i = 5; i < 11; i++) {
			result.push_back(-1);
		}
	}

	while (System::Update()) {
		if (status == 0) {
			logo_s.draw(Color(220, 20, 20));
			logo_u.draw(Color(220, 220, 20));
			logo_p.draw(Color(100, 220, 20));
			logo_e.draw(Color(100, 50, 220));
			logo_r.draw(Color(20, 100, 220));
			logo_b.draw(Color(230, 150, 40));
			logo_a.draw(Color(230, 150, 40));
			logo_l.draw(Color(230, 150, 40));
			logo_l2.draw(Color(230, 150, 40));
			for (int i = 0; i < 6; i++) {
				title_star[i].draw(color_palette[i]);
			}
			font50_medium(U"-Light Version-").draw(600, 370);
			if (instruction_box.intersects(Cursor::Pos())) {
				instruction_box.drawFrame(0, 10, Palette::Yellow);
				font70_heavy(U"あそびかた").drawAt(instruction_box.center());
				if (MouseL.down()) {
					status = 1;
				}
			}
			else {
				instruction_box.drawFrame(0, 5, Palette::Blue);
				font60_medium(U"あそびかた").drawAt(instruction_box.center());
			}
			if (play_box.intersects(Cursor::Pos())) {
				play_box.drawFrame(0, 10, Palette::Yellow);
				font70_heavy(U"あそぶ").drawAt(play_box.center());
				if (MouseL.down()) {
					status = 2;
				}
			}
			else {
				play_box.drawFrame(0, 5, Palette::Red);
				font60_medium(U"あそぶ").drawAt(play_box.center());
			}
		}
		else if (status == 1) {
			font50_regular(U"<ルール>\nとにかく高く登っていこう！\n\nマウス操作で動かします").drawAt(500, 250);
			if (return_box.intersects(Cursor::Pos())) {
				return_box.drawFrame(0, 10);
				font40_regular(U"もどる").drawAt(return_box.center());
				if (MouseL.down()) {
					status = 0;
				}
			}
			else {
				return_box.drawFrame(0, 5);
				font30_regular(U"もどる").drawAt(return_box.center());
			}
		}
		else if (status == 2) {
			if (time_attack_box.intersects(Cursor::Pos())) {
				time_attack_box.drawFrame(0, 10, Palette::Yellow);
				font40_heavy(U"ゴールまで早くたどり着け！").drawAt(time_attack_box.center().movedBy(0, -50));
				font60_heavy(U"タイムアタックモード").drawAt((time_attack_box.center().movedBy(0, 30)));
				if (MouseL.down()) {
					mode = 0;
					timer = 0;
					status = 10;
					for (int i = -Random(300, 450); (mode == 0 && i <= 14500) || (mode == 1 && i <= 100000); i += Random(80, 150)) {
						int tmp = Random(50, 600);
						int left = Max(50, tmp - Random(100, 180));
						int right = Min(600, tmp + Random(100, 180));
						scaffold.push_back(Rect(left, -i, right - left, 10));
					}
				}
			}
			else {
				time_attack_box.drawFrame(0, 5, Palette::Blue);
				font30_medium(U"ゴールまで早くたどり着け！").drawAt(time_attack_box.center().movedBy(0, -50));
				font50_medium(U"タイムアタックモード").drawAt((time_attack_box.center().movedBy(0, 30)));
			}
			if (score_attack_box.intersects(Cursor::Pos())) {
				score_attack_box.drawFrame(0, 10, Palette::Yellow);
				font40_heavy(U"1分間でどこまで登れるか？").drawAt(score_attack_box.center().movedBy(0, -50));
				font60_heavy(U"スコアアタックモード").drawAt((score_attack_box.center().movedBy(0, 30)));
				if (MouseL.down()) {
					mode = 1;
					timer = 3600;
					status = 10;
					for (int i = -Random(300, 450); (mode == 0 && i <= 14500) || (mode == 1 && i <= 100000); i += Random(80, 150)) {
						int tmp = Random(50, 600);
						int left = Max(50, tmp - Random(100, 180));
						int right = Min(600, tmp + Random(100, 180));
						scaffold.push_back(Rect(left, -i, right - left, 10));
					}
				}
			}
			else {
				font30_medium(U"1分間でどこまで登れるか？").drawAt(score_attack_box.center().movedBy(0, -50));
				font50_medium(U"スコアアタックモード").drawAt((score_attack_box.center().movedBy(0, 30)));
				score_attack_box.drawFrame(0, 5, Palette::Red);
			}
			if (return_box.intersects(Cursor::Pos())) {
				return_box.drawFrame(0, 10);
				font40_regular(U"もどる").drawAt(return_box.center());
				if (MouseL.down()) {
					status = 0;
				}
			}
			else {
				return_box.drawFrame(0, 5);
				font30_regular(U"もどる").drawAt(return_box.center());
			}
		}
		else if (status == 10) {
			for (std::pair<Polygon, Color>& p : star) {
				p.first.movedBy(0, camera_pos / 2).draw(p.second);
			}
			ball.draw(camera_pos);
			for (Rect& r : scaffold) {
				r.movedBy(0, camera_pos).draw(Color(255, 255, 200));
			}
			
			ground.movedBy(0, camera_pos).draw(Color(0, 100, 20));
			if (mode == 0) {
				goal_line.movedBy(0, camera_pos).draw(Palette::Red);
			}
			left_wall.draw(Color(0, 0, 10));
			right_wall.draw(Color(0, 0, 10));
			font50_medium(U"TIME").draw(610, 270);
			timer_box.drawFrame(3);
			font100_regular(U"{}:{:0>2}.{:0>2}"_fmt(timer / 60 / 60, timer / 60 % 60, timer % 60 * 100 / 60)).drawAt(timer_box.center());
			if (mode == 0) {
				font30_regular(U"ゴールまで あと").draw(620, 100);
				font100_medium(U"{:0>6.2f}"_fmt((ball.circle.y + 14520) / 100)).draw(620, 130);
				font30_regular(U"m").draw(965, 210);
			}
			else if (mode == 1) {
				font30_regular(U"いまの高さ").draw(620, 100);
				font100_medium(U"{:0>6.2f}"_fmt((480 - ball.circle.y) / 100)).draw(620, 130);
				font30_regular(U"m").draw(965, 210);
			}
			if (timer2 > 180) {
				Circle(325, 300, 150).drawArc(0, 1_pi * ((timer2 - 180) * 6) / 180, 0, 40, Palette::Yellow);
				font200_medium(3).drawAt(325, 300);
			}
			else if (timer2 > 120) {
				Circle(325, 300, 150).drawArc(0, 1_pi * ((timer2 - 120) * 6) / 180, 0, 40, Palette::Orange);
				font200_medium(2).drawAt(325, 300);
			}
			else if (timer2 > 60) {
				Circle(325, 300, 150).drawArc(0, 1_pi * ((timer2 - 60) * 6) / 180, 0, 40, Palette::Red);
				font200_medium(1).drawAt(325, 300);
			}
			else {
				font255_heavy(U"START!!").drawAt(timer2 * 160 / 6 - 700, 300, Palette::Yellow);
			}
			timer2--;
			if (timer2 == 0) {
				status = 11;
			}
		}
		else if (status == 11) {
			ball.move(scaffold, camera_pos);
			for (std::pair<Polygon, Color>& p : star) {
				p.first.movedBy(0, camera_pos / 2).draw(p.second);
			}
			ball.draw(camera_pos);
			for (Rect& r : scaffold) {
				r.movedBy(0, camera_pos).draw(Color(255, 255, 200));
			}
			ground.movedBy(0, camera_pos).draw(Color(0, 100, 20));
			if (mode == 0) {
				goal_line.movedBy(0, camera_pos).draw(Palette::Red);
			}
			left_wall.draw(Color(0, 0, 10));
			right_wall.draw(Color(0, 0, 10));
			font50_medium(U"TIME").draw(610, 270);
			timer_box.drawFrame(3);
			font100_regular(U"{}:{:0>2}.{:0>2}"_fmt(timer / 60 / 60, timer / 60 % 60, timer % 60 * 100 / 60)).drawAt(timer_box.center());
			if (mode == 0) {
				font30_regular(U"ゴールまで あと").draw(620, 100);
				font100_medium(U"{:0>6.2f}"_fmt((ball.circle.y + 14520) / 100)).draw(620, 130);
				font30_regular(U"m").draw(965, 210);
				timer++;
				if (timer == 36000) {
					status = 13;
					timer2 = 360;
				}
				else if (goal_line.intersects(ball.circle)) {
					status = 12;
					timer2 = 360;
					for (int i = 0; i < 5; i++) {
						if (result[i] >= timer) {
							for (int j = 4; j > i; j--) {
								result[j] = result[j - 1];
							}
							result[i] = timer;
							break;
						}
					}
					result[30] = 0;
					for (int i = 0; i < 10; i++) {
						result[10] ^= result[i];
					}
					TextWriter result_writer(U"result.txt");
					if (result_writer) {
						for (int i = 0; i < 11; i++) {
							result_writer.writeln(result[i]);
						}
					}
				}
			}
			else if (mode == 1) {
				font30_regular(U"いまの高さ").draw(620, 100);
				font100_medium(U"{:0>6.2f}"_fmt((480 - ball.circle.y) / 100)).draw(620, 130);
				font30_regular(U"m").draw(965, 210);
				timer--;
				if (timer == 0) {
					status = 13;
					timer2 = 360;
					for (int i = 5; i < 10; i++) {
						if (result[i] <= (480 - ball.circle.y)) {
							for (int j = 9; j > i; j--) {
								result[j] = result[j - 1];
							}
							result[i] = 480 - ball.circle.y;
							break;
						}
					}
					result[10] = 0;
					for (int i = 0; i < 10; i++) {
						result[10] ^= result[i];
					}
					TextWriter result_writer(U"result.txt");
					if (result_writer) {
						for (int i = 0; i < 11; i++) {
							result_writer.writeln(result[i]);
						}
					}
				}
			}
		}
		else if (status == 12) {
			for (std::pair<Polygon, Color>& p : star) {
				p.first.movedBy(0, camera_pos / 2).draw(p.second);
			}
			ball.draw(camera_pos);
			for (Rect& r : scaffold) {
				r.movedBy(0, camera_pos).draw(Color(255, 255, 200));
			}
			ground.movedBy(0, camera_pos).draw(Color(0, 100, 20));
			if (mode == 0) {
				goal_line.movedBy(0, camera_pos).draw(Palette::Yellow);
			}
			left_wall.draw(Color(0, 0, 10));
			right_wall.draw(Color(0, 0, 10));
			font50_medium(U"TIME").draw(610, 270);
			timer_box.drawFrame(3);
			font100_regular(U"{}:{:0>2}.{:0>2}"_fmt(timer / 60 / 60, timer / 60 % 60, timer % 60 * 100 / 60)).drawAt(timer_box.center());
			font30_regular(U"ゴールまで あと").draw(620, 100);
			font100_medium(U"{:0>6.2f}"_fmt(0.0)).draw(620, 130);
			font30_regular(U"m").draw(965, 210);
			font255_heavy(U"GOAL!!!").drawAt(timer2 * 8 - 1300, 300, Palette::Yellow);
			timer2--;
			if (timer2 == 0) {
				status = 14;
			}
		}
		else if (status == 13) {
			for (std::pair<Polygon, Color>& p : star) {
				p.first.movedBy(0, camera_pos / 2).draw(p.second);
			}
			ball.draw(camera_pos);
			for (Rect& r : scaffold) {
				r.movedBy(0, camera_pos).draw(Color(255, 255, 200));
			}
			ground.movedBy(0, camera_pos).draw(Color(0, 100, 20));
			if (mode == 0) {
				goal_line.movedBy(0, camera_pos).draw(Palette::Red);
			}
			left_wall.draw(Color(0, 0, 10));
			right_wall.draw(Color(0, 0, 10));
			font50_medium(U"TIME").draw(610, 270);
			timer_box.drawFrame(3);
			
			if (mode == 0) {
				font30_regular(U"ゴールまで あと").draw(620, 100);
				font100_medium(U"{:0>6.2f}"_fmt((ball.circle.y + 14520) / 100)).draw(620, 130);
				font30_regular(U"m").draw(965, 210);
				font100_regular(U"9:59.99").drawAt(timer_box.center(), Palette::Yellow);
			}
			else if (mode == 1) {
				font30_regular(U"いまの高さ").draw(620, 100);
				font100_medium(U"{:0>6.2f}"_fmt((480 - ball.circle.y) / 100)).draw(620, 130);
				font30_regular(U"m").draw(965, 210);
				font100_regular(U"0:00.00").drawAt(timer_box.center(), Palette::Yellow);
			}
			font255_heavy(U"Time's Up").drawAt(timer2 * 8 - 1300, 300, Palette::Yellow);
			timer2--;
			if (timer2 == 0) {
				status = 14;
			}
		}
		else if (status == 14) {
		font70_regular(U"ランキング").drawAt(500, 50);
			font20_regular(U"Enterキーでおわる").drawAt(500, 550);
			for (int i = 0; i < 6; i++) {
				title_star[i].draw(color_palette[i]);
			}
			if (mode == 0) {
				if (timer == 36000) {
					font120_medium(U"Time's UP!").drawAt(500, 180, Palette::Skyblue);
				}
				else {
					font120_medium(U"{}:{:0>2}.{:0>2}"_fmt(timer / 60 / 60, timer / 60 % 60, timer % 60 * 100 / 60)).drawAt(500, 180, Palette::Skyblue);
				}
				bool emphasized = false;
				for (int i = 0; i < 5; i++) {
					if (result[i] == 0xE869120) {
						font50_medium(U"{}位 : 9:59.99"_fmt(i + 1)).drawAt(500, 270 + i * 50);
					}
					else if (!emphasized && result[i] == timer) {
						font50_medium(U"{}位 : {}:{:0>2}.{:0>2}"_fmt(i + 1, result[i] / 60 / 60, result[i] / 60 % 60, result[i] % 60 * 100 / 60)).drawAt(500, 270 + i * 50, Palette::Yellow);
						emphasized = true;
					}
					else {
						font50_medium(U"{}位 : {}:{:0>2}.{:0>2}"_fmt(i + 1, result[i] / 60 / 60, result[i] / 60 % 60, result[i] % 60 * 100 / 60)).drawAt(500, 270 + i * 50);
					}
				}
			}
			else if (mode == 1) {
				font120_medium(U"{:0>6.2f}m"_fmt((480 - ball.circle.y) / 100)).drawAt(500, 180, Palette::Skyblue);
				bool emphasized = false;
				for (int i = 0; i < 5; i++) {
					if (result[5 + i] == -1) {
						font50_medium(U"{}位 : 000.00m"_fmt(i + 1)).drawAt(500, 270 + i * 50);
					}
					else if (!emphasized && result[5 + i] == 480 - ball.circle.y) {
						font50_medium(U"{}位 : {:0>6.2f}m"_fmt(i + 1, (double)result[5 + i] / 100)).drawAt(500, 270 + i * 50, Palette::Yellow);
						emphasized = true;
					}
					else {
						font50_medium(U"{}位 : {:0>6.2f}m"_fmt(i + 1, (double)result[5 + i] / 100)).drawAt(500, 270 + i * 50);
					}
				}
			}
			if (KeyEnter.down()) {
				return;
			}
		}
	}
}
