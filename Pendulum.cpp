#define _USE_MATH_DEFINES
#include "glm/glm.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <functional>

class Particle
{
public:
	sf::Vector2f pos;
	sf::Clock cl;
};

float correctRot(float rot)
{
	if (rot > 270)
		rot = rot - 360;
	return rot;
}

long long findFactorial(int n)
{
	return n == 0 ? 1 : n * findFactorial(n - 1);
}

float getLegendre(float rad)
{
	int amt = 26;

	double badboy = 0.0;
	for(int n=0;n<=amt;++n)
	{
		auto tf = findFactorial(2 * n);
		auto lr = findFactorial(n);

		double lp = pow(2.0, n);
		double lb = pow(lp * lr, 2.0);

		double fl = pow(tf / lb, 2.0);
		double sin2n = sin(rad/2);
		double rs = pow(sin2n, 2 * n);

		badboy += fl * rs;
	}

	return badboy;
}

int main()
{
	const unsigned int W = 800;
	const unsigned int H = 800;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(W, H), "SFML works!", sf::Style::Default, settings);
	window.setFramerateLimit(60.f); //look up later

	float bar_len = 20.0f;
	float bar_y = 250.0f;

	sf::RectangleShape line(sf::Vector2f(bar_len,300));
	line.setFillColor(sf::Color::White);
	line.setOrigin(line.getSize().x/2,0);
	line.setPosition(sf::Vector2f(W/2,bar_y));

	sf::RectangleShape ceiling(sf::Vector2f(W, 12));
	ceiling.setFillColor(sf::Color(128, 64, 0));
	ceiling.setPosition(sf::Vector2f(0, bar_y));

	sf::CircleShape ball(1.5f * line.getSize().x);
	ball.setFillColor(sf::Color::Black);
	ball.setOrigin(sf::Vector2f(ball.getRadius(), ball.getRadius()));
	ball.setPosition(sf::Vector2f(line.getGlobalBounds().left + line.getGlobalBounds().width, line.getGlobalBounds().top + line.getGlobalBounds().height));

	sf::CircleShape tparticle(5.0f);
	tparticle.setFillColor(sf::Color::Green);
	tparticle.setOrigin(sf::Vector2f(tparticle.getRadius(), tparticle.getRadius()));
	tparticle.setOutlineColor(sf::Color::Black);
	tparticle.setOutlineThickness(4.0f);

	sf::RectangleShape debug(sf::Vector2f(10, 10));
	debug.setFillColor(sf::Color::Green);

	sf::Font def_font;

	if (!def_font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) 
		std::cout << "Font failed laul\n";
	else
		std::cout << "Font loaded properly\n";

	float length = 2.0f, kg = 23.5f, g = 9.81f;

	sf::Text text;
	text.setFont(def_font);
	text.setCharacterSize(1.45f * line.getSize().x);	
	text.setString(std::to_string(static_cast<int>(length)) + " m");
	text.setFillColor(sf::Color::Black);

	sf::Text md;
	md.setFont(def_font);
	md.setCharacterSize(0.7f * ball.getRadius());
	md.setString(std::to_string(static_cast<int>(kg)) + " kg");
	md.setFillColor(sf::Color::White);

	sf::Text meta;
	meta.setString("G = 9.81");
	meta.setFont(def_font);
	meta.setFillColor(sf::Color::Black);
	meta.setCharacterSize(30.0f);
	meta.setPosition(sf::Vector2f(0, line.getPosition().y /2));

	bool isheld = false;
	float ampl = 0.0f;
	
	sf::Clock cl;
	sf::Clock fc;

	int pi = 0;
	std::vector<Particle> ps(100);

	for (int i = 0; i < ps.size(); ++i)
		ps[i].pos = sf::Vector2f(ball.getPosition());

	while (window.isOpen())
	{
		window.clear(sf::Color(128, 128, 128));

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
					window.close();
			}
			if (isheld && event.type == sf::Event::MouseButtonReleased)
			{
				ampl = correctRot(line.getRotation());
				cl.restart();
			}
		}
		sf::Vector2i mp = sf::Mouse::getPosition(window);

		const sf::FloatRect& rect = line.getGlobalBounds();
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			float dist = glm::distance(glm::vec2(ball.getPosition().x, ball.getPosition().y), glm::vec2(mp.x, mp.y));

			if (dist < ball.getRadius())
				isheld = true;

			float angle_rad = std::atan((mp.y - line.getPosition().y)/(mp.x - line.getPosition().x));
			float to_deg = glm::degrees(angle_rad);

			if (isheld)
			{
				if (angle_rad < 0)
					line.setRotation(1 * (90 + to_deg));
				else
					line.setRotation(-1 * (90 - to_deg));
			}
		}

		float t = cl.getElapsedTime().asSeconds();
		
		if(!isheld)
		{
			float ampr = glm::radians(ampl);
			float al = getLegendre(ampr);

			float w = sqrt(g / length) * (1.0f / al);

			float rad = glm::radians(ampl) * cos(w * t);
			float dd = glm::degrees(rad);

			line.setRotation(dd);
		}

		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			isheld = false;
		}
		float tr = correctRot(line.getRotation());

		line.setRotation(std::max(std::min(tr, 85.0f), -85.0f));
		ball.setPosition(rect.left + !(line.getRotation() < 90) * rect.width, rect.top + rect.height);

		if (fc.getElapsedTime().asSeconds() > 0.05f)
		{
			fc.restart();

			ps[pi].pos = ball.getPosition();
			ps[pi++].cl.restart();
		}

		pi %= ps.size();

		for (int i = 0; i < ps.size(); ++i)
		{
			if (ps[i].cl.getElapsedTime().asSeconds() < 1.0f)
			{
				tparticle.setPosition(ps[i].pos);
				window.draw(tparticle);
			}
		}

		text.setRotation( ((line.getRotation() < 90)? 270: 90) + line.getRotation());

		md.setOrigin((md.getLocalBounds().left + md.getLocalBounds().width / 2), (md.getLocalBounds().top + md.getLocalBounds().height / 2));
		md.setPosition(ball.getPosition().x, ball.getPosition().y);

		text.setOrigin((text.getLocalBounds().left + text.getLocalBounds().width / 2), (text.getLocalBounds().top + text.getLocalBounds().height / 2));
		text.setPosition((rect.left  + rect.width/2), (rect.top + rect.height/2));

		window.draw(meta);
		window.draw(line);
		window.draw(text);
		window.draw(ceiling);
		window.draw(ball);
		window.draw(md);
		
		window.display();
	}
	return 0;
}