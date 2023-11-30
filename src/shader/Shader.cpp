#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const char* vertex_path, const char* fragment_path) {
	std::string vertex_code;
	std::string fragment_code;

	std::ifstream vertex_f;
	std::ifstream fragment_f;

	vertex_f.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragment_f.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vertex_f.open(vertex_path);
		fragment_f.open(fragment_path);
		std::stringstream vertex_stream, fragment_stream;

		vertex_stream << vertex_f.rdbuf();
		fragment_stream << fragment_f.rdbuf();

		vertex_f.close();
		fragment_f.close();

		vertex_code   = vertex_stream.str();
		fragment_code = fragment_stream.str();
	} catch (std::ifstream::failure e) {
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n" << e.what() << std::endl;
	}

	const char* vertex_code_c_str   = vertex_code.c_str();
	const char* fragment_code_c_str = fragment_code.c_str();

	GLuint vertex_shader, fragment_shader;
	int    success;
	char   info_log[512];

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_code_c_str, NULL);
	glCompileShader(vertex_shader);

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		std::cerr << "ERROR::VERTEX::SHADER::COMPILATION_FAILED\n" << info_log << "\n";
		return;
	}

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_code_c_str, NULL);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		std::cerr << "ERROR::FRAGMENT::SHADER::COMPILATION_FAILED\n" << info_log << "\n";
		return;
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertex_shader);
	glAttachShader(ID, fragment_shader);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, info_log);
		std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << info_log << "\n";
		return;
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::set_bool(const std::string& attr_name, bool value) {
	glUniform1i(glGetUniformLocation(ID, attr_name.c_str()), (int)value);
}

void Shader::set_int(const std::string& attr_name, int value) {
	glUniform1i(glGetUniformLocation(ID, attr_name.c_str()), value);
}

void Shader::set_float(const std::string& attr_name, float value) {
	glUniform1f(glGetUniformLocation(ID, attr_name.c_str()), value);
}

void Shader::set_vec(const std::string& attr_name, float* value, size_t size) {
	switch (size) {
		case 1:
			glUniform1f(glGetUniformLocation(ID, attr_name.c_str()), *value);
			break;
		case 2:
			glUniform2f(glGetUniformLocation(ID, attr_name.c_str()), *value, *(value + 1));
			break;
		case 3:
			glUniform3f(glGetUniformLocation(ID, attr_name.c_str()), *value, *(value + 1), *(value + 2));
			break;
		case 4:
			glUniform4f(glGetUniformLocation(ID, attr_name.c_str()), *value, *(value + 1), *(value + 2), *(value + 3));
			break;
		default:
			std::cerr << "Invalid vector size, should be 1, 2, 3 or 4; got = " << size << std::endl;
			break;
	}
}
