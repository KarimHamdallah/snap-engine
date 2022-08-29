#pragma once
#include <engine/physics/collisions.h>
#include <vector>
#include <memory>

typedef struct body
{
	AABB aabb;
	glm::vec2 velocity;
	glm::vec2 acceleration;
}Body;

typedef struct static_body
{
	AABB aabb;
}StaticBody;

typedef struct hit
{
	bool is_hit = false;
	glm::vec2 position = glm::vec2(0.0f); // new position of aabb after collison resolution
	f32 time = 0.0f;
	glm::vec2 normal = glm::vec2(0.0f);
}Hit;

class physicsWorld
{
public:
	static void physics_init();
	static void physics_update(f32 delta_time);
	static u32 pushBody(glm::vec2 position, glm::vec2 scale);
	static Body* getBody(u32 index);
	static u32 pushStaticBody(glm::vec2 position, glm::vec2 scale);
	static StaticBody* getStaticBody(u32 index);
	static void addForce(Body* body, glm::vec2 force);

	static bool point_vs_aabb(glm::vec2 point, const AABB& aabb);
	static bool aabb_vs_aabb(const AABB& A, const AABB& B);
	static AABB minkowski_difference(const AABB& A, const AABB& B);
	static glm::vec2 get_closest_point_to_origin(const AABB& result);

	static void aabb_min_max(const AABB& aabb, glm::vec2& min, glm::vec2& max);
	static Hit ray_vs_aabb(glm::vec2 origin, glm::vec2 direction, const AABB& aabb);

	static Hit sweep_static_bodies(AABB aabb, glm::vec2 velocity);
	static void sweep_response(Body* b, glm::vec2 velocity);
	static void stationary_response(Body* b);

	static u32 getBodies_count();
	static u32 getStaticBodies_count();


	static void free_memory();
private:
	static std::vector<Body*> m_physics_bodies;
	static std::vector<StaticBody*> m_static_bodies;
	static f32 gravity;
	static f32 terminal_velocity;
	static u32 iterations;
	static f32 tick_rate;
};