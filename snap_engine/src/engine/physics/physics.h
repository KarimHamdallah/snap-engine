#pragma once
#include <engine/defines.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

typedef struct aabb
{
	glm::vec2 position;
	glm::vec2 half_scale;
}AABB;

typedef struct body
{
	AABB aabb;
	glm::vec2 velocity;
	glm::vec2 acceleration;
}Body;

typedef struct hit
{
	bool is_hit;
	glm::vec2 position; // new position of aabb after collison resolution
	f32 time;
}Hit;

class physicsWorld
{
public:
	static void physics_init();
	static void physics_update(f32 delta_time);
	static void creatBody(glm::vec2 position, glm::vec2 scale);
	static Body* getBody(u32 index);

	static bool point_vs_aabb(glm::vec2 point, const AABB& aabb);
	static bool aabb_vs_aabb(const AABB& A, const AABB& B);
	static AABB minkowski_difference(const AABB& A, const AABB& B);
	static glm::vec2 get_closest_point_to_origin(const AABB& result);

	static void aabb_min_max(const AABB& aabb, glm::vec2& min, glm::vec2& max);
	static Hit ray_vs_aabb(glm::vec2 origin, glm::vec2 direction, const AABB& aabb);
private:
	static std::vector<Body*> m_physics_bodies;
	
};