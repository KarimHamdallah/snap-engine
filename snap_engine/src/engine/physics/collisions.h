#pragma once
#include <glm/glm.hpp>
#include <engine/defines.h>
#include <engine/math/math.h>

struct AABB
{
	glm::vec2 center;
	glm::vec2 half_scale;
};

struct RayCastHit
{
	bool hit;
	f32 t1;
	f32 t2;
	glm::vec2 enter_point;
	glm::vec2 exit_point;
};

class collisions
{
public:
	static bool point_vs_aabb(const glm::vec2& point, const AABB& aabb)
	{
		glm::vec2 min, max;
		aabb_min_max(aabb, min, max);
		
		return point.x >= min.x &&
			   point.x <= max.x &&
			   point.y >= min.y &&
			   point.y <= max.y;
	}
	static bool point_vs_aabb(const glm::vec2& point, const glm::vec2& min, const glm::vec2& max)
	{
		return point.x >= min.x &&
			point.x <= max.x &&
			point.y >= min.y &&
			point.y <= max.y;
	}
	static bool AABB_vs_AABB(const AABB& a, const AABB& b)
	{
		AABB minkowski_aabb;
		minkowski_difference(a, b, minkowski_aabb);

		return point_vs_aabb(glm::vec2(0.0f), minkowski_aabb);
	}

	// normal vector is in (a -> b) direction, to resolve subtract from (a) center or add to (b) center
	static bool AABB_vs_AABB(const AABB& a, const AABB& b, glm::vec2& normal, f32& depth)
	{
		normal = glm::vec2(0.0f);
		depth = 0.0f;

		AABB minkowski_aabb;
		minkowski_difference(a, b, minkowski_aabb);

		glm::vec2 min, max;
		aabb_min_max(minkowski_aabb, min, max);

		if (point_vs_aabb(glm::vec2(0.0f), min, max))
		{
			f32 min_dist = FLT_MAX;

			if (-min.x < min_dist)
			{
				// left
				min_dist = fabs(min.x);
				normal = glm::vec2(-1.0f, 0.0f);
			}
			if (max.x < min_dist)
			{
				// right
				min_dist = fabs(max.x);
				normal = glm::vec2(1.0f, 0.0f);
			}
			if (-min.y < min_dist)
			{
				// down
				min_dist = fabs(min.y);
				normal = glm::vec2(0.0f, -1.0f);
			}
			if (max.y < min_dist)
			{
				// up
				min_dist = fabs(max.y);
				normal = glm::vec2(0.0f, 1.0f);
			}

			depth = min_dist;
			return true;
		}

		return false;
	}

	static RayCastHit line_vs_AABB(const glm::vec2& line_start, const glm::vec2& line_end, AABB aabb)
	{
		RayCastHit result{ false, 0.0f, 0.0f, glm::vec2(0.0f), glm::vec2(0.0f) };

		glm::vec2 line_vector = line_end - line_start;
		glm::vec2 line_dir = glm::normalize(line_vector);

		glm::vec2 min, max;
		aabb_min_max(aabb, min, max);

		glm::vec2 max_vector = max - line_start; // vector from line start to max AABB point
		glm::vec2 min_vector = min - line_start; // vector from line start to min AABB point


		glm::vec2 temp;
		temp.x = (line_dir.x != 0.0f) ? 1.0f / line_dir.x : 0.0f;
		temp.y = (line_dir.y != 0.0f) ? 1.0f / line_dir.y : 0.0f;

		max_vector *= temp;  // max_vector.x / line_dir.x, max_vector.y / line_dir.y
		min_vector *= temp;  // min_vector.x / line_dir.x, min_vector.y / line_dir.y

		f32 tmin = math::max(math::min(min_vector.x, max_vector.x), math::min(min_vector.y, max_vector.y));
		f32 tmax = math::min(math::max(min_vector.x, max_vector.x), math::max(min_vector.y, max_vector.y));


		if (tmax < 0 || tmin > tmax)
			return result; // no collision and no data to return

		// there is a data to return
		result.t1 = tmin;
		result.t2 = tmax;
		result.enter_point = line_start + result.t1 * line_dir;
		result.exit_point = line_start + result.t2 * line_dir;

		f32 t = (tmin < 0.0f) ? tmax : tmin;
		result.hit = t > 0.0f && t * t < math::lengthSqrd(line_vector);

		return result;
	}

	static bool circle_vs_circle(const glm::vec2& centerA, f32 raduisA, const glm::vec2& centerB, f32 raduisB)
	{
		glm::vec2 dist_vector = centerB - centerA;
		f32 radii = raduisA + raduisB;
		
		return math::lengthSqrd(dist_vector) <= radii * radii;
	}

	static bool point_vs_circle(const glm::vec2& point, const glm::vec2& center, f32 raduis)
	{
		glm::vec2 dist_vector = point - center;

		return math::lengthSqrd(dist_vector) <= raduis * raduis;
	}

	static bool point_vs_polygon(const glm::vec2& point, const std::vector<glm::vec2>& vertices)
	{
		u32 count = 0;
		f32 x = point.x;
		f32 y = point.y;

		for (size_t i = 0; i < vertices.size(); i++)
		{
			glm::vec2 va = vertices[i];
			glm::vec2 vb = vertices[(i + 1) % vertices.size()];

			f32 x1 = va.x;
			f32 y1 = va.y;
			f32 x2 = vb.x;
			f32 y2 = vb.y;

			// derived from line equation
			f32 point_of_intersection_x = (x2 - x1) * (y - y1) / (y2 - y1) + x1;

			if (y < y1 != y < y2 && x < point_of_intersection_x)
				count++;
		}

		return count % 2 == 0 ? false : true;
	}

	static bool polygon_vs_polygon(std::vector<glm::vec2> polygonA_vertices, std::vector<glm::vec2> polygonB_vertices)
	{
		// check for each point in A is inside B or not
		for (size_t i = 0; i < polygonA_vertices.size(); i++)
		{
			glm::vec2& point = polygonA_vertices[i];
			if (point_vs_polygon(point, polygonB_vertices))
				return true;
		}
		// check for each point in B is inside A or not
		for (size_t i = 0; i < polygonB_vertices.size(); i++)
		{
			glm::vec2& point = polygonB_vertices[i];
			if (point_vs_polygon(point, polygonA_vertices))
				return true;
		}

		return false;
	}

private:
	static void aabb_min_max(const AABB& aabb, glm::vec2& min, glm::vec2& max)
	{
		min = aabb.center - aabb.half_scale;
		max = aabb.center + aabb.half_scale;
	}

	static void minkowski_difference(const AABB& a, const AABB& b, AABB& minkowski_difference)
	{
		minkowski_difference.center = a.center - b.center;
		minkowski_difference.half_scale = a.half_scale + b.half_scale;
	}
};