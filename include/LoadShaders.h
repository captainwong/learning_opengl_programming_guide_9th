#pragma once

#include <GL/GL.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

	struct ShaderInfo {
		GLenum type;		
		const char* source;
		GLuint shader;
	};

	enum LoadType {
		LoadFromFile,
		LoadFromString,
	};

	/**
	* @brief load shader and return gl program
	* 
	*/
	GLuint LoadShaders(ShaderInfo* info, LoadType loadType = LoadFromString);

#ifdef __cplusplus
};
#endif // __cplusplus
