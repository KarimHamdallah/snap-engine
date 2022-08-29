#include "particle_system.h"
#include <snap_engine.h>

particle_system::particle_system()
{
	m_ParticlePool.resize(1000);
}

void particle_system::update()
{
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
			continue;

		if (particle.LifeRemaining <= 0.0f)
		{
			particle.Active = false;
			continue;
		}

		particle.LifeRemaining -= Time::m_deltaTime;
		particle.Position += particle.Velocity * Time::m_deltaTime;
		particle.Rotation += 0.01f * Time::m_deltaTime;
	}
}

void particle_system::render()
{
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
			continue;

		// Fade away particles
		f32 life = particle.LifeRemaining / particle.LifeTime;
		color color = math::lerp(particle.ColorEnd, particle.ColorBegin, life);
		//color.a = color.a * life;

		glm::vec2 size = math::lerp(particle.SizeEnd, particle.SizeBegin, life);

		// Render
		//renderer::render_quad(particle.Position, size, particle.Rotation, color);
		renderer::render_aabb({ particle.Position, size * 0.5f }, color);
	}
}

void particle_system::push(const particel_properties & particleProps, u32 particles_count)
{
	for (size_t i = 0; i < particles_count; i++)
	{
		Particle& particle = m_ParticlePool[m_PoolIndex];
		particle.Active = true;
		particle.Position = particleProps.position;
		particle.Rotation = random(0.0f, 360.0f);

		// Velocity
		particle.Velocity = particleProps.velocity;
		particle.Velocity.x += particleProps.velocityVariation.x * random(-0.5f, 0.5f);
		particle.Velocity.y += particleProps.velocityVariation.y * random(-0.5f, 0.5f);

		// Color
		particle.ColorBegin = particleProps.startColor;
		particle.ColorEnd = particleProps.endColor;

		particle.LifeTime = particleProps.lifeTime;
		particle.LifeRemaining = particleProps.lifeTime;
		particle.SizeBegin = particleProps.startScale + particleProps.scaleVariation * random(-0.5f, 0.5f);
		particle.SizeEnd = particleProps.endScale;

		m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
	}
}