#include "game_play.h"
#include <snap_engine.h>
#include "asteroid.h"
#include "main_menu.h"

#define ASTEROIDS_COUNT 5

void game_play::init()
{
	// hide cursor
	Input::show_cursor(false);
	// creat player
	player = std::make_shared<space_ship>(WindowCenter, 20.0f, 0.0f);
	ship_color = BLUE;
	bullet_color = BLUE;

	// window dimensions
	f32 width = window::getInstance()->getWidth();
	f32 height = window::getInstance()->getHeight();

	glm::vec2 corners[]{ 
		glm::vec2(0.0f, height),
		glm::vec2(width, height),
		glm::vec2(width, 0.0f),
		glm::vec2(0.0f, 0.0f)
	};
	// reserve asteroids count
	stars.reserve(ASTEROIDS_COUNT);
	for (size_t i = 0; i < ASTEROIDS_COUNT; i++)
		stars.emplace_back(std::make_shared<star>(
			glm::vec2(random(-1.0f, 5.0f), random(-1.0f, 5.0f)), random(50.0f, 70.0f), random(0.0f, 360.0f) )
		);
}

void game_play::update()
{
	// update player
	player->update(200.0f);

	// update stars
	for (size_t i = 0; i < stars.size(); i++)
		stars[i]->update();
	
	// collsions
	for (size_t i = 0; i < player->Bullets.size(); i++)
	{
		auto bullet = player->Bullets[i];
		for (size_t j = 0; j < stars.size(); j++)
		{
			auto star = stars[j];

			// outer collsion
			if (collisions::point_vs_circle(bullet->pos, star->position, star->scale.x))
			{
				// inner collison
				if (collisions::point_vs_polygon(bullet->pos, star->transformed_vertices))
				{
					// remove bullet
					player->Bullets.erase(player->Bullets.begin() + i);
					// remove star
					destroy_star(j);
					break;
				}
			}
		}
	}

	if (!player->is_respawining)
	{
		player_vs_asteroids();
	}

	vfx.update();

	if (Input::isKeyPressed(Key::EscapeKey))
		GameStateManger.set_state(new main_menu());
}

void game_play::render()
{
	player->render(ship_color, bullet_color);

	for (size_t i = 0; i < stars.size(); i++)
		stars[i]->render();

	vfx.render();
}

void game_play::shutdown()
{
	LOG_CRITICAL("game_play shutdown");
	// remove stars and free memory
	stars.clear();
	stars.shrink_to_fit();
}

game_play::~game_play()
{
}

void game_play::destroy_star(u32 index)
{
	glm::vec2 pos = stars[index]->position;
	f32 scale = stars[index]->scale.x;
	
	// break into two
	if (scale > 40.0f)
	{
		stars.push_back(std::make_shared<star>
			(pos, scale - 10.0f, random(0.0f, 360.0f))
		);

		stars.push_back(std::make_shared<star>
			(pos, scale - 10.0f, random(0.0f, 360.0f))
		);
	}

	vfx.push(smoke_vfx(stars[index]->position), 20);

	stars.erase(stars.begin() + index);
}

void game_play::player_vs_asteroids()
{
	for (size_t i = 0; i < stars.size(); i++)
	{
		if (collisions::circle_vs_circle(player->polygon->getpos(), player->polygon->getraduis() * 2.3f, stars[i]->position, stars[i]->scale.x))
		{
			if (collisions::polygon_vs_polygon(player->polygon->getTransformedVertices(), stars[i]->transformed_vertices))
			{
				player->Kill();
				// add particles
				if (!player->shield_on && player->is_respawining)
				{
					vfx.push(blue_vfx(player->polygon->getpos()), 100);
				}
				break;
			}
		}
	}
}