/*
==============================================================================================================================================================

Author: Stephane Baudoux
Attribution-Noncommercial-Share Alike 3.0 Unported

You are free:
to Share - to copy, distribute and transmit the work
to Remix - to adapt the work

Under the following conditions:
Attribution - You must attribute the work in the manner specified by the author or licensor (but not in any way that suggests that they endorse you or your use of the work).
Noncommercial - You may not use this work for commercial purposes.
Share Alike - If you alter, transform, or build upon this work, you may distribute the resulting work only under the same or similar license to this one.

With the understanding that:
Waiver - Any of the above conditions can be waived if you get permission from the copyright holder.
Public Domain - Where the work or any of its elements is in the public domain under applicable law, that status is in no way affected by the license.
Other Rights - In no way are any of the following rights affected by the license:
Your fair dealing or fair use rights, or other applicable copyright exceptions and limitations;
The author's moral rights;
Rights other persons may have either in the work itself or in how the work is used, such as publicity or privacy rights.
Notice - For any reuse or distribution, you must make clear to others the license terms of this work. The best way to do this is with a link to this web page.

==============================================================================================================================================================
*/

#include <Windows.h>
#include <gl/GL.h>
#include <fstream>
#include "../../Core/Utility/CFile.h"
#include "../../Core/Logger/ILogger.h"
#include "../../Core/Memory/CExceptionLoad.h"
#include "../../Medias/IShaderBase.h"
#include "../../Renderers/OGL/COGLRenderer.h"
#include "../../Renderers/OGL/COGLShader.h"
#include "../../Renderers/OGL/COGLShadersLoader.h"

namespace DE
{
	COGLShadersLoader::COGLShadersLoader() {}

	COGLShadersLoader::~COGLShadersLoader() {}

	IShaderBase * COGLShadersLoader::LoadFromFile(const std::string & filename)
	{
		CFile file(filename);
		if(!file.FileExists())
			throw CExceptionLoad(filename);

		unsigned int shader;
		
		// Create the correct shader.
		if(file.GetExtension() == "vsh")
			shader = COGLRenderer::glCreateShader(GL_VERTEX_SHADER);
		else
			shader = COGLRenderer::glCreateShader(GL_FRAGMENT_SHADER);

		// Shader creation error
		if(shader == 0)
		{
			ILogger::Log() << "CE:COGLShadersLoader Error: Unable to create the shader.\n";
			ILogger::Kill();
			throw CExceptionLoad(filename);
		}

		// Load the source code.
		char * src = LoadSource(filename);
		if(src == nullptr)
		{
			COGLRenderer::glDeleteShader(shader);
			ILogger::Log() << "CE:COGLShadersLoader Error: The loaded source is empty.\n";
			ILogger::Kill();
			throw CExceptionLoad(filename);
		}

		// Assign the code to the shader.
		COGLRenderer::glShaderSource(shader, 1, (const GLchar**)&src, nullptr);
    
		// Compile the shader.
		COGLRenderer::glCompileShader(shader);

		// Compilation verification.
		GLint compile_status = GL_TRUE;
		COGLRenderer::glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
		if(compile_status != GL_TRUE)
		{
			// Compilation error.
			int logsize;
			COGLRenderer::glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logsize);

			// Get the error.
			char * log = new char[logsize + 1];
			memset(log, '\0', logsize + 1);
			COGLRenderer::glGetShaderInfoLog(shader, logsize, &logsize, log);

			// Log the error.
			ILogger::Log() << "CE:COGLShadersLoader Error: " << log << ".\n";
			ILogger::Kill();

			// Free the shader and the log buffer.
			delete[] log;
			COGLRenderer::glDeleteShader(shader);

			// Exception.
			throw CExceptionLoad(filename);
		}
    
		// Free the data
		delete[] src;

		return new COGLShader(filename, shader);
	}

	char * COGLShadersLoader::LoadSource(const std::string & filename) const
	{
		std::ifstream stream;
		stream.open(filename, std::ios::binary);

		// Get the file size
		stream.seekg(0, std::ios::end);
		const int size = (int)stream.tellg();

		// Get the data
		stream.seekg (0, std::ios::beg);
		char * data = new char[size + 1];
		stream.read(data, size);
		data[size] = '\0';

		stream.close();

		return data;
	}
}
