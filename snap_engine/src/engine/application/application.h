#pragma once
#include <memory>
#include <snap_engine.h>

class application
{
public:
	application();
	~application();


	virtual void start() = 0;
	virtual void update() = 0;
	virtual void render() = 0;

protected:
	std::shared_ptr<window> win;
};