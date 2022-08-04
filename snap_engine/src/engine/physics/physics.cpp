#include "physics.h"

std::vector<Body*> physicsWorld::m_physics_bodies;

void physicsWorld::physics_init()
{
}

void physicsWorld::physics_update(f32 delta_time)
{
	for (size_t i = 0; i < m_physics_bodies.size(); i++)
	{
		Body* physics_body = m_physics_bodies[i];

		physics_body->velocity += physics_body->acceleration * delta_time;
		physics_body->aabb.position += physics_body->velocity * delta_time;
	}
}

void physicsWorld::creatBody(glm::vec2 position, glm::vec2 scale)
{
	Body* b = new Body;

	b->aabb.position = position;
	b->aabb.half_scale = (scale * 0.5f);
	b->velocity = glm::vec2(0.0f);
	b->acceleration = glm::vec2(0.0f);

	m_physics_bodies.push_back(b);
}

Body* physicsWorld::getBody(u32 index)
{
	return m_physics_bodies[index];
}

bool physicsWorld::aabb_vs_aabb(const AABB & A, const AABB & B)
{
	AABB difference = minkowski_difference(A, B);

	return point_vs_aabb(glm::vec2(0.0f), difference);
}

AABB physicsWorld::minkowski_difference(const AABB & A, const AABB & B)
{
	AABB result;
	result.position = A.position - B.position;
	result.half_scale = A.half_scale + B.half_scale;

	return result;
}

glm::vec2 physicsWorld::get_closest_point_to_origin(const AABB & result)
{
	glm::vec2 min = result.position - result.half_scale;
	glm::vec2 max = result.position + result.half_scale;

	f32 min_dist = fabsf(min.x);
	glm::vec2 bounds_point = glm::vec2(min.x, 0.0f);
	if (fabsf(max.x) < min_dist)
	{
		min_dist = fabsf(max.x);
		bounds_point = glm::vec2(max.x, 0.0f);
	}
	if (fabsf(max.y) < min_dist)
	{
		min_dist = fabs(max.y);
		bounds_point = glm::vec2(0.0f, max.y);
	}
	if (fabsf(min.y) < min_dist)
	{
		min_dist = fabsf(min.y);
		bounds_point = glm::vec2(0.0f, min.y);
	}


	return bounds_point;
}

void physicsWorld::aabb_min_max(const AABB & aabb, glm::vec2 & min, glm::vec2 & max)
{
	min = aabb.position - aabb.half_scale;
	max = aabb.position + aabb.half_scale;
}

Hit physicsWorld::ray_vs_aabb(glm::vec2 origin, glm::vec2 direction, const AABB& aabb)
{
	Hit hit = { 0 };
	glm::vec2 min, max;
	aabb_min_max(aabb, min, max);

	f32 last_entry = -INFINITY;
	f32 first_exit = INFINITY;

	for (u8 i = 0; i < 2; ++i) {
		if (direction[i] != 0) {
			f32 t1 = (min[i] - origin[i]) / direction[i];
			f32 t2 = (max[i] - origin[i]) / direction[i];

			last_entry = fmaxf(last_entry, fminf(t1, t2));
			first_exit = fminf(first_exit, fmaxf(t1, t2));
		}
		else if (origin[i] <= min[i] || origin[i] >= max[i]) {
			return hit;
		}
	}

	if (first_exit > last_entry && first_exit > 0 && last_entry < 1) {
		hit.position[0] = origin[0] + direction[0] * last_entry;
		hit.position[1] = origin[1] + direction[1] * last_entry;

		hit.is_hit = true;
		hit.time = last_entry;
	}

	return hit;
}

bool physicsWorld::point_vs_aabb(glm::vec2 point, const AABB& aabb)
{
	glm::vec2 min = aabb.position - aabb.half_scale;
	glm::vec2 max = aabb.position + aabb.half_scale;


	return point.x >= min.x &&
		   point.x <= max.x &&
		   point.y >= min.y &&
		   point.y <= max.y;
}
