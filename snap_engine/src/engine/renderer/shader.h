#pragma once
#include "gl_base_elemnt.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <engine/defines.h>
#include <string>

class shader : public gl_base_element
{
public:
	shader() = default;
	shader(const std::string& vertexshader_filepath, const std::string& fragmentshader_filepath);
	~shader();

	virtual void bind() override;
	virtual void unbind() override;
	virtual void delete_id() override;

	void set_mat2(const std::string& name, const glm::mat2& value);
	void set_mat3(const std::string& name, const glm::mat3& value);
	void set_mat4(const std::string& name, const glm::mat4& value);

	void set_vec2(const std::string& name, const glm::vec2& value);
	void set_vec3(const std::string& name, const glm::vec3& value);
	void set_vec4(const std::string& name, const glm::vec4& value);
	void set_color(const std::string& name, const color& value);

	void set_int(const std::string& name, const i32& value);
	void set_float(const std::string& name, const f32& value);
private:
	gl_id compile_shader(u32 type, const std::string& shader_file_content);
	void link(gl_id vertex_shader, gl_id fragment_shader);
};