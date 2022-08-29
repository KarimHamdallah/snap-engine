#include "physics.h"

std::vector<Body*> physicsWorld::m_physics_bodies;
std::vector<StaticBody*> physicsWorld::m_static_bodies;
f32 physicsWorld::gravity;
f32 physicsWorld::terminal_velocity;
u32 physicsWorld::iterations;
f32 physicsWorld::tick_rate;

void physicsWorld::physics_init()
{
	gravity = -50.0f;
	terminal_velocity = -10000.0f;

	iterations = 2;
	tick_rate = 1.0f / iterations;
}

void physicsWorld::physics_update(f32 delta_time)
{
	Body* physics_body;

	for (size_t i = 0; i < m_physics_bodies.size(); i++)
	{
		// integration
		physics_body = m_physics_bodies[i];

		physics_body->velocity.y += gravity;
		// calmp y velocity in down direction
		if (terminal_velocity > physics_body->velocity.y)
			physics_body->velocity.y = terminal_velocity;

		physics_body->velocity += physics_body->acceleration;

		glm::vec2 scaled_velocity = physics_body->velocity * (delta_time * tick_rate); // scale velocity to fractions reltaed to iterations

		
		for (size_t j = 0; j < iterations; j++)
		{
			sweep_response(physics_body, scaled_velocity); // collsion detection and resolution for moving baodies
			stationary_response(physics_body); // collsion detection and resolution for passed body vs all static baodies in the phsyics world
		}

		// TODO:: REMOVE
		// reset acceleration
		physics_body->acceleration = glm::vec2(0.0f);
	}
}

u32 physicsWorld::pushBody(glm::vec2 position, glm::vec2 scale)
{
	Body* b = new Body;

	b->aabb.center = position;
	b->aabb.half_scale = (scale * 0.5f);
	b->velocity = glm::vec2(0.0f);
	b->acceleration = glm::vec2(0.0f);

	m_physics_bodies.push_back(b);
	
	// return body index
	return m_physics_bodies.size() - 1;
}

Body* physicsWorld::getBody(u32 index)
{
	return m_physics_bodies[index];
}

u32 physicsWorld::pushStaticBody(glm::vec2 position, glm::vec2 scale)
{
	StaticBody* b = new StaticBody;

	b->aabb.center = position;
	b->aabb.half_scale = (scale * 0.5f);

	m_static_bodies.push_back(b);
	// return static body index
	return m_static_bodies.size() - 1;
}

StaticBody * physicsWorld::getStaticBody(u32 index)
{
	return m_static_bodies[index];
}

void physicsWorld::addForce(Body * body, glm::vec2 force)
{
	body->acceleration += force;
}

bool physicsWorld::aabb_vs_aabb(const AABB & A, const AABB & B)
{
	AABB difference = minkowski_difference(A, B);

	return point_vs_aabb(glm::vec2(0.0f), difference);
}

AABB physicsWorld::minkowski_difference(const AABB & A, const AABB & B)
{
	AABB result;
	result.center = A.center - B.center;
	result.half_scale = A.half_scale + B.half_scale;

	return result;
}

glm::vec2 physicsWorld::get_closest_point_to_origin(const AABB & result)
{
	glm::vec2 min = result.center - result.half_scale;
	glm::vec2 max = result.center + result.half_scale;

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
	min = aabb.center - aabb.half_scale;
	max = aabb.center + aabb.half_scale;
}

Hit physicsWorld::ray_vs_aabb(glm::vec2 origin, glm::vec2 direction, const AABB& aabb)
{
	Hit hit;
	hit.is_hit = false;
	hit.normal = glm::vec2(0.0f);
	hit.position = glm::vec2(0.0f);
	hit.time = 0.0f;

	
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

		glm::vec2 d = hit.position - aabb.center;
		glm::vec2 p;
		p.x = aabb.half_scale.x - glm::abs(d.x);
		p.y = aabb.half_scale.y - glm::abs(d.y);

		if (p.x < p.y)
		{
			hit.normal.x = (d.x > 0.0f) - (d.x < 0.0f);
			hit.normal.y = 0.0f;
		}
		else
		{
			hit.normal.x = 0.0f;
			hit.normal.y = (d.y > 0.0f) - (d.y < 0.0f);
		}
	}

	return hit;
}

Hit physicsWorld::sweep_static_bodies(AABB aabb, glm::vec2 velocity)
{
	Hit result;
	result.time = 0xBEEF;

	for (u32 i = 0; i < m_static_bodies.size(); ++i) {
		StaticBody *static_body = m_static_bodies[i];

		AABB sum_aabb = static_body->aabb;
		sum_aabb.half_scale += aabb.half_scale;

		Hit hit = ray_vs_aabb(aabb.center, velocity, sum_aabb);
		if (!hit.is_hit)
		{
			continue;
		}
		if (hit.time < result.time) {
			result = hit;
		}
		else if (hit.time == result.time)
		{
			// Solve highest velocity axis first.
			if (fabsf(velocity[0]) > fabsf(velocity[1]) && hit.normal[0] != 0)
			{
				result = hit;
			}
			else if (fabsf(velocity[1]) > fabsf(velocity[0]) && hit.normal[1] != 0)
			{
				result = hit;
			}
		}
	}

	return result;
}

void physicsWorld::sweep_response(Body * b, glm::vec2 velocity)
{
	Hit hit = sweep_static_bodies(b->aabb, velocity);

	if (hit.is_hit) {
		b->aabb.center[0] = hit.position[0];
		b->aabb.center[1] = hit.position[1];

		if (hit.normal[0] != 0) {
			b->aabb.center[1] += velocity[1];
			b->velocity[0] = 0;
		}
		else if (hit.normal[1] != 0) {
			b->aabb.center[0] += velocity[0];
			b->velocity[1] = 0;
		}
	}
	else
	{
		b->aabb.center += velocity;
	}
}

void physicsWorld::stationary_response(Body * b)
{
	for (size_t i = 0; i < m_static_bodies.size(); i++)
	{
		StaticBody* staticbody = m_static_bodies[i];

		// minkowski collsion detection
		AABB minkowski_difference_aabb = minkowski_difference(staticbody->aabb, b->aabb);
		if (point_vs_aabb(glm::vec2(0.0f), minkowski_difference_aabb))
		{
			// get penteration vector
			glm::vec2 penteration_vector = get_closest_point_to_origin(minkowski_difference_aabb);
			// separate moving body away from static body
			b->aabb.center += penteration_vector;
		}
	}
}

u32 physicsWorld::getBodies_count()
{
	return m_physics_bodies.size();
}

u32 physicsWorld::getStaticBodies_count()
{
	return m_static_bodies.size();
}

bool physicsWorld::point_vs_aabb(glm::vec2 point, const AABB& aabb)
{
	glm::vec2 min = aabb.center - aabb.half_scale;
	glm::vec2 max = aabb.center + aabb.half_scale;


	return point.x >= min.x &&
		   point.x <= max.x &&
		   point.y >= min.y &&
		   point.y <= max.y;
}

void physicsWorld::free_memory()
{
	for (size_t i = 0; i < m_physics_bodies.size(); i++)
	{
		delete m_physics_bodies[i];
	}
	for (size_t i = 0; i < m_static_bodies.size(); i++)
	{
		delete m_static_bodies[i];
	}
}