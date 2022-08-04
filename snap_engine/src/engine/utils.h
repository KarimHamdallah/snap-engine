#pragma once
#include <engine/logger/asserts.h>
#include <fstream>
#include <sstream>
#include <string>

inline std::string get_file_content(const std::string& file_path)
{
	std::string ret = "";
	std::stringstream buff;
	std::ifstream file;

	file.open(file_path);

	if (file.is_open())
	{
		buff << file.rdbuf();
		ret = buff.str();
	}
	else
	{
		LOG_ERROR("Failed to open file at location {}!", file_path);
	}

	return ret;
}