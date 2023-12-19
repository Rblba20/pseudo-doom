#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <list>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>

using namespace sf;
using namespace std;;


float offsetX = 0, offsetY = 0;


const int H = 67;
const int W = 150;//21 == 1x ; 42 ==x2 ; 63 == x3
const int siz = 32;
const int vr = 17;

String Map[H] = {
"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
"ZzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzZ",
"z                                        KKK                                                                                                        z0",
"z                                        KKK                                                                                                   B    z0",
"z                     BBb                kKK                                                                                                  kk    z0",
"z                     kkkkkkkkkkkkkkkkkkkkkk                                                kk kk                                                   z0",
"z                  kk                                                                              kk                                               z0",
"z          kkk                                   kkkkkkkk                             kk                 rrrr                                       z0",
"z                                                                                                          rr                             kk        z0",
"z                                                                                                          rr   cc                                  z0",
"z                                                                                                          rr                        kk             z0",
"zkkk                w                                  w                   w                               rr                                       z0",
"z                                      w                                       kk                          rr              cc                       z0",
"z                                                                             k  kk                        rr     kk                                z0",
"z                      c                                            rr        k                w           rr                                       z0",
"z B                                                               rrrrrrr                                  rr        zzzzzzzzzzzzzzzzzz             z0",
"zzzzzzzzzzzz                                                      rr   rr                                  rr        zz              zz             z0",
"z           z        b   b           ckckkk                                                                rr        zzbbb           zz             z0",
"z           z   c    kckck                                                                                 rrrr      zzzzz           zz     cc      z0",
"z           z                         e                             rr                               V     rr        zz              zz             z0",
"z                                                e                rrrrrr          e                  p     rr        zz              zz             z0",
"z  s                           bb b            b bb       b       rrrrrrr              rr                  rr  rr    zz Bbbbb     BB zz        BB   z0",
"zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz   zzzzzzzzzzzzzzzzzzzzz",
"zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz   zzzzzzzzzzzzzzzzzzzzz",
"zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz   zzzzzzzzzzzzzzzzzzzzz",
"z                                                                                                                                                   z0",
"z                                                                                                                                                   z0",
"z                                                                                                                                                   z0",
"z                      BBb                                                                                                                          z0",
"z                     kkkkk                                                   w                                                                     z0",
"z                                                                                                                                                   z0",
"z           w                                                                                                 w                                     z0",
"z                               kk       kk         kk                                                                                              z0",
"z                                                     k                                                                                             z0",
"z                                                               2                                                                                   z0",
"z                                                               1                                                                                   z0",
"z                                                                    2                                                     w                        z0",
"z                                                               1    1                                                                              z0",
"z                                                                          2                                                                        z0",
"z        V                                                      1    1     1                                                                        z0",
"z                                  E                  E                          2                 E           E                                    z0",
"z        p                                                      1    1     1     1                   P                                              z0",
"z                                                               B          b          r                                     z      z                z0",//42
"zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz",
"zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz",
"z                                                                                                                                                   z0",
"z                                                                                                                                                   z0",
"z                                                                                                                                                   z0",
"z                                                                                                                                                   z0",
"z                                                                                                                                                   z0",
"z                                                                                                                                                   z0",
"z                                                                                                             w                                     z0",
"z                                                                                                                                                   z0",
"z                                                                                                                                                   z0",
"z                                                                                                                                                   z0",
"z                               w                                                                                                                   z0",
"z                                                                                             w                                                     z0",
"z                                                                                                                                                   z0",
"z                                                                                                                                                   z0",
"z                                       w                                                                                                           z0",
"z                                                                        BB                                                                         z0",
"z                                                11       11             11                                                        kkkkk            z0",
"z                              e                                           e                                           e           2   2            z0",
"z           P                                    11       11             11                                                        1 9 1            z0",
"z                 U                   bbbb      U                         BB             B        r        bbb                                      z0",//63
"zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz0",
"zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz0",
};


class PLAYER {

public:

	float speed_x, speed_y;
	FloatRect rect;
	bool onGround;
	Sprite sprite;
	float currentFrame;
	bool life, nap;
	int plCount, health;
	int niz, pos  , textt , startpos, ene, cot;
	vector<int> cout_enem;

	PLAYER(Texture& image)
	{
		sprite.setTexture(image);
		startpos = 0;
		
		rect = FloatRect(6 * siz, 20 * siz, 40, 50);
			
		
		
		speed_x = 0.6;
		speed_y = 0.2;
		currentFrame = 0;
		life = true;
		plCount = 0;
		health = 3;
		niz = 0;
		pos,textt = 0;
		
	}


	void update(float time)
	{
		if (pos == 1) {
			rect.top = 1326;
			pos = 2;
		}
		if (pos == 3) {
			rect.top = 2030;
			pos = 4;
		}
		if (pos == 9) {
			rect.top = 2030;
			speed_x = 0;
		}

		cot = count(cout_enem.begin(), cout_enem.end(), 1);

		rect.left += speed_x * time;
		Collision(0);
		

		if (!onGround) speed_y += 0.0005 * time;
		rect.top += speed_y * time;
		onGround = false;
		Collision(1);
		if (health == 0) {
			life = false;
		}
		currentFrame += 0.005 * time;
		if (life) {
			if (speed_x) {
				if (currentFrame > 6) currentFrame -= 6;

				if (speed_x > 0) {
					sprite.setTextureRect(IntRect(40 * int(currentFrame), 244, 40, 50));
					niz = 1;
				}
				if (speed_x < 0) {
					sprite.setTextureRect(IntRect(40 * int(currentFrame) + 40, 244, -40, 50));
					niz = 0;
				}
			}
			if (speed_y == 0 && speed_x == 0) {
				if (niz == 0) {
					sprite.setTextureRect(IntRect(0, 300, 40, 50));
				}
				else {
					sprite.setTextureRect(IntRect(40, 300, -40, 50));
				}
			}
			if (speed_y) {
				if (speed_x > 0) {
					sprite.setTextureRect(IntRect(0, 360, 40, 50));
					niz = 1;
				}
				if (speed_x < 0) {
					sprite.setTextureRect(IntRect(40, 360, -40, 50));
					niz = 0;
				}
			}

		}
		else {
			/*for (int i = 0; i < 4; i += 0.0005 * time) {*/
			sprite.setTextureRect(IntRect(0, 184, 40, 50));



		}

		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
		speed_x = 0;
	}



	void Collision(int dir)
	{
		for (int i = rect.top / siz; i < (rect.top + rect.height) / siz; i++) {
			for (int j = rect.left / siz; j < (rect.left + rect.width) / siz; j++)
			{
				if ((Map[i][j] == 'z') || (Map[i][j] == 'k') || (Map[i][j] == '0') || (Map[i][j] == 'r') || (Map[i][j] == 'c') || (Map[i][j] == 'C') || (Map[i][j] == '2'))

				{

					if ((speed_x > 0) && (dir == 0)) rect.left = j * siz - rect.width;
					if ((speed_x < 0) && (dir == 0)) rect.left = j * siz + siz;
					if ((speed_y > 0) && (dir == 1)) { rect.top = i * siz - rect.height;  speed_y = 0;   onGround = true; }
					if ((speed_y < 0) && (dir == 1)) { rect.top = i * siz + siz;   speed_y = 0; }
				}

				if (Map[i][j] == 'b' || Map[i][j] == 'B') {
					if (Map[i][j] == 'b'){
						plCount += 1;
					}
					if (Map[i][j] == 'B') {
						plCount += 3;
					}
					Map[i][j] = ' ';				
				}
				if (Map[i][j] == 'p') {
					if (cot > 2) {
						pos = 1;
					}

					if (cot > 6) {
						pos = 3;

					}
				
					
				}
				if (Map[i][j] == ' ') {
					if (((Map[i - 1][j] == 'p') || (Map[i - 1][j] == '9')&&(pos == 0 || pos == 2 || pos ==4))) {
						textt = 1;

					}
					else {
						textt = 0;
					}
				
				}

				if (onGround == false && dir == 1) {
					if (Map[i][j] == 'c') {
						Map[i - 1][j] = 'h';
						Map[i][j] = 'C';
					}
				}
				if (Map[i][j] == 'h') {
					health += 1;
					Map[i][j] = ' ';
				}
				
				if (Map[i][j] == '9') {
					if (cot > 12) {
						pos = 9;
					}
					
				}

			}
		}
	}
};


class MusicPlayer {
public:
	bool loadMusic(const string& filename) {
		if (!m_music.openFromFile(filename)) {
			return false;
		}
		return true;
	}

	void play() {
		m_music.play();
	}

	void pause() {
		m_music.pause();
	}

private:
	sf::Music m_music;
};


class ENEMY
{

public:
	float speed_x, speed_y;
	FloatRect rect;
	Sprite sprite;
	float currentFrame;
	bool life, draw;
	int t,hp,sizX;
	bool drawSprite;
	Clock deathTime;


	void set(Texture& image, int x, int y,int siz_x , int siz_y , int h)
	{
		sprite.setTexture(image);
		rect = FloatRect(x, y, siz_x,siz_y);
		
		if (siz_x == 64) {
			speed_x = 0.095;
		}
		else {
			speed_x = 0.075;
		}
		currentFrame = 0;
		life = true;
		hp = h;
		t = h;
		sizX = siz_x;
		drawSprite = true;
		deathTime.restart();
	}

	void update(float time)
	{
		rect.left += speed_x * time;
		Collision();
		currentFrame += time * 0.005;
		if (sizX == 32) {
			
			if (currentFrame > 2) currentFrame -= 2;
			sprite.setTextureRect(IntRect(32 * int(currentFrame), 0, siz, siz));
			if (!life) {
				sprite.setTextureRect(IntRect(64, 0, siz, siz));
				sprite.setTextureRect(IntRect(96, 0, siz, siz));
			}
		}
		if (sizX == 64 && t == 4 ) {
			if (currentFrame > 2) currentFrame -= 2;
			sprite.setTextureRect(IntRect(64 * int(currentFrame), 32, siz * 2, siz * 2));
			if (!life) {
				sprite.setTextureRect(IntRect(128, 32, 64, 64));
				sprite.setTextureRect(IntRect(192, 32, 64, 64));
			}
		}
		if (sizX == 64 && t == 7) {
			if (currentFrame > 4) currentFrame -= 4;
			if (speed_x > 0) {
				sprite.setTextureRect(IntRect(64 * int(currentFrame) +64, 0, -siz * 2, siz * 2));
			}
			if (speed_x < 0) {
				sprite.setTextureRect(IntRect(64 * int(currentFrame), 0, siz * 2, siz * 2));
			}
			if (!life) {
				sprite.setTextureRect(IntRect(0, 64, 64, 64));
			}
		}

		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);

	}


	void Collision()
	{
		
		for (int i = rect.top / 32; i < (rect.top + rect.height) / 32; i++)
			for (int j = rect.left / 32; j < (rect.left + rect.width) / 32; j++)
				if ((Map[i][j] == 'z') || (Map[i][j] == '0') || (Map[i][j] == 'r'))
				{
					if (speed_x > 0)
					{
						rect.left = j * 32 - rect.width; speed_x *= -1;
					}
					else if (speed_x < 0)
					{
						rect.left = j * 32 + 32;  speed_x *= -1;
					}

				}
	}

};

int getRandomNumber(int min, int max) {
	return rand() % (max - min + 1) + min; // генерация случайного числа между min и max
}

int convert(int q, int min, int max, vector<int>& vec)
{
	int a;
	if (q == 0) {
		a = getRandomNumber(min, max);
	}
	else {
		a = getRandomNumber(min, max);
		if (a == q || find(vec.begin(), vec.end(), a) != vec.end()) {
			return convert(q, min, max, vec);
		}
	}
	switch (a) {
	case 57:
	case 58:
	case 60:
	case 68:
	case 69:
	case 70:
	case 71:
	case 72:
	case 73:
	case 74:
	case 75:
	case 90:
	case 91:
		return convert(q, min, max, vec);
	default:
		return a;
	}
}


int main()
{
	RenderWindow window(VideoMode(1200, 700), "Test!");


	MusicPlayer music;
	(music.loadMusic("last.mp3"));
	


	Texture background;
	background.loadFromFile("back.png");
	Sprite back;
	back.setTexture(background);

	back.setScale(
		static_cast<float>(window.getSize().x) / background.getSize().x,
		static_cast<float>(window.getSize().y) / background.getSize().y
	);
	back.setColor(Color(255, 255, 255, 200));

	int z, v;
	z = 0;
	v = 1;
	Texture hero;
	hero.loadFromFile("hero1.png");
	Texture tile;
	if (z == 1) {
		tile.loadFromFile("Ztep.png");
	}
	if (v == 1) {
		tile.loadFromFile("тер.png");
	}

	Sprite till(tile);
	float currentFrame = 0;
	Font fot;
	fot.loadFromFile("COld.ttf");//передаем файл шрифта
	Text text("", fot, 40);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях)
	Text text1("", fot, 40);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях)
	text1.setFillColor(Color::Red);//покрасили текст в красный
	text1.setStyle(Text::Bold);//жирный и подчеркнутый текст. 
	Text text2("", fot, 40);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях)
	text2.setFillColor(Color::Red);//покрасили текст в красный
	Text textmenu, textmenu2, textmenu3;

	textmenu.setFont(fot); // Установка шрифта для первого текстового элемента
	textmenu.setString("Start Game"); // Установка текста "Start Game"
	textmenu.setCharacterSize(36); // Установка размера шрифта
	textmenu.setFillColor(Color::Red); // Установка цвета текста
	textmenu.setOutlineColor(Color::Black);
	textmenu.setOutlineThickness(2);
	textmenu.setPosition(150, 200); // Установка позиции текста

	textmenu2.setFont(fot);
	textmenu2.setString("Sound:");
	textmenu2.setCharacterSize(36);
	textmenu2.setFillColor(Color::White);
	textmenu2.setOutlineColor(Color::Black);
	textmenu2.setOutlineThickness(2);
	textmenu2.setPosition(150, 250);

	textmenu3.setFont(fot);
	textmenu3.setString("Quit");
	textmenu3.setCharacterSize(36);
	textmenu3.setFillColor(Color::White);
	textmenu3.setOutlineColor(Color::Black);
	textmenu3.setOutlineThickness(2);
	textmenu3.setPosition(150, 300);


	PLAYER pl(hero);
	Sprite plh(hero);
	if (pl.life == true) {
		plh.setTextureRect(IntRect(0, 300, 40, 50));
	}
	if (pl.life == false) {
		plh.setTextureRect(IntRect(0, 184, 40, 50));
	}
	plh.setPosition(3 * siz, 0);
	plh.setScale(0.9, 0.9);


	Sprite monet(tile);
	monet.setPosition(32 * siz, 0);
	monet.setScale(1.3, 1.3);
	monet.setTextureRect(IntRect(0, 64, siz, siz));

	Texture ene;
	ene.loadFromFile("ene.png");

	Texture enE;
	enE.loadFromFile("dimon.png");

	vector<int> temp1;

	int r, temp = 0;
	int count = 0;

	srand(time(0));  // Инициализация генератора случайных чисел на основе текущего времени

	while (temp1.size() < 20) {
		r = convert(temp, 18, 85, temp1);
		temp = r;
		temp1.push_back(r);
		count++;	
	}
	
	
	ENEMY enemy[vr];

	{	
		enemy[0].set(ene, temp1[0] * siz, 21 * siz,siz,siz,1);
		enemy[1].set(ene, temp1[1] * siz, 21 * siz, siz, siz, 1);
		enemy[2].set(ene, temp1[2] * siz, 21 * siz, siz, siz, 1);
		enemy[3].set(ene, temp1[3] * siz, 42 * siz, siz, siz, 2);
		enemy[4].set(ene, temp1[4] * siz, 42 * siz, siz, siz, 2);
		enemy[5].set(ene, 130 * siz, 42 * siz, siz, siz, 10);
		enemy[6].set(ene, temp1[5] * siz, 41 * siz, 64, siz * 2 , 4);
		enemy[7].set(ene, temp1[6] * siz, 41 * siz, 64, siz * 2 , 4);
		enemy[8].set(enE, temp1[7] * siz, 63 * siz, 64, siz * 2 , 7);
		enemy[9].set(enE, temp1[8] * siz, 63 * siz, 64, siz * 2, 7);
		enemy[10].set(enE, temp1[9] * siz, 63 * siz, 64, siz * 2, 7);
		enemy[11].set(enE, temp1[10] * siz, 63 * siz, 64, siz * 2, 7);
		enemy[12].set(enE, 60 * siz, 63 * siz, 64, siz * 2, 7);
		enemy[13].set(enE, temp1[11] * siz, 63 * siz, 64, siz * 2, 7);
		enemy[14].set(enE, 75 * siz, 63 * siz, 64, siz * 2, 7);
		
	}
	Clock clock;
	Clock gameTimeCl;
	Clock gameTimeE;

	int gameTime = 0;
	int EnTime,ElTime = 0;

	int variable = 0;
	bool isPlaying = false;

	bool Mous_nasat = false;

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();

		time = time / 600;

		if (time > 15) time = 15;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		if (variable == 0) {

			if (event.type == Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == Mouse::Left && !Mous_nasat)
				{
					Mous_nasat = true;
					if (event.mouseButton.button == Mouse::Left)
					{
						if (textmenu.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
						{
							// Запуск игры
							cout << "Starting the game..." << endl;
							variable = 1;
							pl.startpos = 1;

						}
						else if (textmenu2.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
						{
							
								if (isPlaying) {
									cout << "Sound is now Off..." << endl;
									textmenu2.setString("Sound: Off");
									isPlaying = false;
									music.pause();
									
								}
								else {
									cout << "Sound is now On..." << endl;
									textmenu2.setString("Sound: On");
									isPlaying = true;
									music.play();

								}
							
						}
						else if (textmenu3.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
						{
							// Закрытие игры
							window.close();
						}
					}
				}
			}

			else if (event.type == Event::MouseButtonReleased)
			{
				if (event.mouseButton.button == Mouse::Left)
				{
					Mous_nasat = false;  // Update the previous mouse button state
				}
			}
			if (event.type == sf::Event::MouseMoved)
			{
				if (textmenu.getGlobalBounds().contains(sf::Vector2f(event.mouseMove.x, event.mouseMove.y)))
				{
					textmenu.setFillColor(sf::Color::Yellow); // Изменение цвета при наведении курсора
				}
				else
				{
					textmenu.setFillColor(sf::Color::Red); // Возврат исходного цвета при уходе курсора
				}
				if (textmenu2.getGlobalBounds().contains(sf::Vector2f(event.mouseMove.x, event.mouseMove.y)))
				{
					textmenu2.setFillColor(sf::Color::Yellow); // Изменение цвета при наведении курсора
				}
				else
				{
					textmenu2.setFillColor(sf::Color::White); // Возврат исходного цвета при уходе курсора
				}
				if (textmenu3.getGlobalBounds().contains(sf::Vector2f(event.mouseMove.x, event.mouseMove.y)))
				{
					textmenu3.setFillColor(sf::Color::Yellow); // Изменение цвета при наведении курсора
				}
				else
				{
					textmenu3.setFillColor(sf::Color::White); // Возврат исходного цвета при уходе курсора
				}
			}

		}
		if (variable == 1) {
			if (pl.life == true) {
				gameTime = gameTimeCl.getElapsedTime().asSeconds();

				if ((Keyboard::isKeyPressed(Keyboard::Left)) || (Keyboard::isKeyPressed(Keyboard::A))) {
					pl.speed_x = -0.195;;

				}

				if (((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D))))) {
					pl.speed_x = 0.195;

				}

				if ((Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W)))) {
					if (pl.onGround) {
						pl.speed_y = -0.425;
						pl.onGround = false;


					}
				}

			}
			if ((Keyboard::isKeyPressed(Keyboard::Escape))) {

				variable = 0;
			}

			pl.update(time);

			for (int i = 0; i < vr; i++) {
				enemy[i].update(time);
			}


			for (int i = 0; i < vr - 1; i++) {
				if (pl.rect.intersects(enemy[i].rect))
				{
					if (enemy[i].life) {
						
						if (pl.speed_y > 0) {
							if (i == 5) {
								pl.speed_y = -3;
							}
							else {
								pl.speed_y = -0.4;
							}
							enemy[i].hp -= 1;
							EnTime = gameTimeE.getElapsedTime().asSeconds();

							if (enemy[i].hp == 0) {
								enemy[i].life = false;
								enemy[i].speed_x = 0;
								pl.cout_enem.push_back(1);
							}
						}

						else if (pl.health > 0 && abs(EnTime - gameTimeE.getElapsedTime().asSeconds()) >= 2) {
							if (abs(ElTime - gameTimeE.getElapsedTime().asSeconds()) >= 1) {
								pl.health--;
								ElTime = gameTimeE.getElapsedTime().asSeconds();

								if (pl.health > 0) {
									if (pl.niz == 1) {
										pl.rect.left -= 120;
									}
									else {
										pl.rect.left += 120;
									}
								}
							}
						}
					}
				}
			}

			if (pl.rect.left > 600) offsetX = pl.rect.left - 600;

			if (pl.rect.top > 200) offsetY = pl.rect.top - 400;


			/*for (int i = 0; i < 2; i++) {
				cout << pl.cot << endl;
			}*/
		      }

		window.clear(Color(47, 79, 79));

		int siz = 32;
		for (int i = 0; i < H; i++) {
			for (int j = 0; j < W; j++) {
				char symbol = Map[i][j];

				switch (symbol) {
				case 'k':
					till.setTextureRect(IntRect(0, 0, siz, siz));
					break;
				case 'z':
					till.setTextureRect(IntRect(96, 0, siz, siz));
					break;
				case 'c':
					till.setTextureRect(IntRect(64, 0, siz, siz));
					break;
				case 'r':
					till.setTextureRect(IntRect(32, 0, siz, siz));
					break;
				case 'w':
					till.setTextureRect(IntRect(0, 32, siz*2, siz));
					break;
				case 's':
					till.setTextureRect(IntRect(64, 32, siz * 2, siz));
					break;
				case 'b':
					till.setTextureRect(IntRect(0, 64, siz, siz));
					break;
				case 'B':
					till.setTextureRect(IntRect(96, 128, siz, siz));
					break;
				case 'C':
					till.setTextureRect(IntRect(32, 64, siz, siz));
					break;
				case 'h':
					till.setTextureRect(IntRect(64, 64, siz, siz));
					break;
				case 'p':
					till.setTextureRect(IntRect(64, 96, siz, siz*2));
					break;
				case 'V':
					till.setTextureRect(IntRect(0, 96, siz , siz));
					break;
				case 'e':
					till.setTextureRect(IntRect(0, 128, siz, siz));
					break;
				case 'E':
					till.setTextureRect(IntRect(32, 128, -siz, siz));
					break;
				case '1':
					till.setTextureRect(IntRect(32, 96, siz, siz*2));
					break;
				case '2':
					till.setTextureRect(IntRect(96, 64, siz, siz));
					break;
				case 'P':
					till.setTextureRect(IntRect(64, 96, siz, siz * 2));
					break;
				case '9':
					till.setTextureRect(IntRect(64, 160, siz, siz * 2));
					break;
				case 'U':
					till.setTextureRect(IntRect(128, 96, -siz, siz));
					break;

				}

				if ((Map[i][j] == ' ') || (Map[i][j] == '0')) continue;
				till.setPosition(j * siz - offsetX, i * siz - offsetY);
				window.draw(till);

			}
		}
		ostringstream playerScore;    // объявили переменную
		playerScore << pl.plCount;

		int slov = pl.plCount;
		if ((slov == 12) or (slov == 11) or (slov == 13) or (slov == 14))
		{
			slov = 0;
		}
		else {
			slov = pl.plCount % 10;
		}
		
		string word;
		if (slov == 1) {
			word = "пуля";
		}
		else if (slov >= 2 && slov <= 4) {
			word = "пули";
		}
		else {
			word = "пуль";
		}

		if (pl.pos != 9) {
			text.setString(playerScore.str() + " - " + word);//задает строку тексту
			text.setPosition(1050, 0);
		}
		ostringstream playerHealth;    // объявили переменную
		if (pl.pos != 9) {
		text1.setPosition(0, 0);
		playerHealth << pl.health;	//занесли в нее число здоровья, то есть формируем строку
		text1.setString("ХВ:" + playerHealth.str());	//задаем строку тексту и вызываем сформированную выше строку методом .str() 
		
		if (pl.life == false) {
				playerHealth << gameTime;
				text1.setCharacterSize(80);
				text1.setPosition(400, 200);
				text1.setString("Время в секундах:" + playerHealth.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 
		}
		}
		if (pl.pos == 9) {
				playerHealth << gameTime;
				text1.setCharacterSize(80);
				text.setCharacterSize(80);
				text.setPosition(450, 100);
				text1.setPosition(450, 200);
				text1.setString(playerScore.str() + " - " + word);//задаем строку тексту и вызываем сформированную выше строку методом .str() 
				text.setString( " КОНЕЦ ");
		}	//задаем строку тексту и вызываем сформированную выше строку методом .str() 	
		
		for (int i = 0; i < vr - 1; i++) {
			if(!enemy[i].life) {
				if (enemy[i].deathTime.getElapsedTime().asSeconds() > 20) {
					enemy[i].drawSprite = false; 
				}
				else {
					window.draw(enemy[i].sprite);
				}
			}
			else {
				enemy[i].deathTime.restart();
				if (enemy[i].drawSprite) {
					window.draw(enemy[i].sprite);
				}
			}
		}
		if (pl.textt == 1) {
			text2.setString(" Нужно убить всех врагов на уровне ");
			text2.setPosition(200, 150);
		}
		else {
			text2.setString("  ");
		}
		if (variable == 0) {
			window.clear();
			window.draw(back);
			window.draw(textmenu);
			window.draw(textmenu2);
			window.draw(textmenu3);
		}
		if (variable == 1) {
			window.draw(text2);
			window.draw(text1);//рисую этот текст
			window.draw(text);
			window.draw(monet);
			window.draw(plh);
			window.draw(pl.sprite);
		}
		ofstream outfile("bullets.txt");
		double coeff = 1 + 0.1 * pl.health;
		cout << "Coeff " << coeff << endl;
		cout << "PLCount" << pl.plCount << endl;
		cout << int(pl.plCount * coeff) << endl;
		cout << pl.plCount * coeff << endl;
		int total = pl.plCount * coeff;
		outfile << total << endl;
		outfile.close();

		window.display();
	}

	return 0;
}