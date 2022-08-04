#pragma once

#include <engine/defines.h>

class Time{
public:
	static void time_init(u32 frame_rate);
	static void time_update(void);
	static void time_update_late(void);

	static f32 m_deltaTime;
	static f32 m_currentTime;
	static f32 m_lastTime;

	static f32 m_frameLast;
	static f32 m_targetFrameTime;
	static f32 m_frameTime;
	
	static u32 m_frameRate;
	static u32 m_frameCount;
};