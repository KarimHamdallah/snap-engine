#include "Time.h"
#include <SDL2/SDL.h>
#include <random>

f32 Time::m_deltaTime;
f32 Time::m_currentTime;
f32 Time::m_lastTime;

f32 Time::m_frameLast;
f32 Time::m_targetFrameTime;

f32 Time::m_frameTime;
u32 Time::m_frameRate;
u32 Time::m_frameCount;

void Time::time_init(u32 frame_rate)
{
	m_frameRate = frame_rate;
	m_targetFrameTime = 1000.f / frame_rate; // frame target time in ms

	srand(SDL_GetTicks());
}

void Time::time_update(void)
{
	m_currentTime = (f32)SDL_GetTicks();
	m_deltaTime = (m_currentTime - m_lastTime) / 1000.f;
	m_lastTime = m_currentTime;
	m_frameCount++;

	// run this block once each seconed
	if (m_currentTime - m_frameLast >= 1000.f) {
		m_frameRate = m_frameCount;
		m_frameCount = 0;
		m_frameLast = m_currentTime;
	}
}

void Time::time_update_late(void)
{
	// calculate frame time lasting between calling time-update and time_update_late
	m_frameTime = (f32)SDL_GetTicks() - m_currentTime;

	if (m_targetFrameTime > m_frameTime)
	{
		SDL_Delay(m_targetFrameTime - m_frameTime);
	}
}