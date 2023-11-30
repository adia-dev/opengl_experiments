#pragma once

#include <glad/glad.h>
#include <string>

struct Shader {
	Shader(const char* vertex_path, const char* fragment_path);

	void use();

	void set_bool(const std::string& attr_name, bool value);
	void set_int(const std::string& attr_name, int value);
	void set_float(const std::string& attr_name, float value);
	void set_vec(const std::string& attr_name, float* value, size_t size);

	GLuint ID;
};
