#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <GL3/gl3w.h>
#include "LoadShaders.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

static const GLchar* ReadShader(const char* filename)
{
	FILE* f = fopen(filename, "rb");
	if (!f) {
		fprintf(stderr, "Unable to open file '%s'\n", filename);
		return nullptr;
	}

	fseek(f, 0, SEEK_END);
	int len = ftell(f);
	fseek(f, 0, SEEK_SET);

	auto source = new GLchar[len + 1];
	fread(source, 1, len, f);
	fclose(f);

	source[len] = 0;
	return source;
}

GLuint LoadShaders(ShaderInfo * info, LoadType loadType)
{
	if (!info) { return 0; }

	GLuint program = glCreateProgram();

	auto entry = info;
	while (entry->type != GL_NONE) {
		entry->shader = glCreateShader(entry->type);
		if (loadType == LoadFromFile) {
			entry->source = ReadShader(entry->source);
			if (entry->source == nullptr) {
				for (entry = info; entry->type != GL_NONE; entry++) {
					glDeleteShader(entry->shader);
					entry->shader = 0;
				}
				return 0;
			}
		}

		glShaderSource(entry->shader, 1, reinterpret_cast<const GLchar* const*>(&entry->source), nullptr);
		if (loadType == LoadFromFile) {
			delete[] entry->source;
			entry->source = nullptr;
		}

		glCompileShader(entry->shader);

		GLint compiled;
		glGetShaderiv(entry->shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
#ifdef _DEBUG
			GLsizei len;
			glGetShaderiv(entry->shader, GL_INFO_LOG_LENGTH, &len);
			GLchar* log = new GLchar[len + 1];
			glGetShaderInfoLog(entry->shader, len, &len, log);
			fprintf(stderr, "Shader compilation failed: %s\n", log);
			delete[] log;
#endif
			return 0;
		}

		glAttachShader(program, entry->shader);
		++entry;
		
	}

	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked) {
#ifdef _DEBUG
		GLsizei len;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
		auto log = new GLchar[len + 1];
		glGetProgramInfoLog(program, len, &len, log);
		fprintf(stderr, "Shader linking filed: %s\n", log);
		delete[] log;
#endif

		for (entry = info; entry->type != GL_NONE; entry++) {
			glDeleteShader(entry->shader);
			entry->shader = 0;
		}

		return 0;
	}

	return program;
}


#ifdef __cplusplus
};
#endif
