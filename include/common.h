#include "config.h"

#pragma once

//TODO: Qt Include hack

#include <QtCore/qdatastream.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qtextstream.h>


#ifdef USE_GL3
#define GL3_PROTOTYPES 1
#define LIBLUB_GL_MAJOR_VERION 3
#define LIBLUB_GL_MINOR_VERION 3
#ifndef GLfloat
#include <GL3/gl3.h>
#include <GL3/glx.h>
#include <GL3/glxext.h>
#include <GL3/glext.h>

#endif
#else
#define GL_GLEXT_PROTOTYPES 1
#define LIBLUB_GL_MAJOR_VERION 2
#define LIBLUB_GL_MINOR_VERION 1
//#include <GL/glew.h>
#include <GL/gl.h>
//#include <GL3/glu.h>
#include <GL/glext.h>
#endif

#include <iostream>
using namespace std;

static void glError(string file, int line) {
	GLenum err(glGetError());

	while (err != GL_NO_ERROR) {
		std::string error;

		switch (err) {
		case GL_INVALID_OPERATION:
			error = "INVALID_OPERATION";
			break;
		case GL_INVALID_ENUM:
			error = "INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			error = "INVALID_VALUE";
			break;
		case GL_OUT_OF_MEMORY:
			error = "OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			error = "INVALID_FRAMEBUFFER_OPERATION";
			break;
		default:
			error = "Unknown error";
			break;
		}

		cerr << "\x1b[1;31m" << "GL_" << error << "\e[m" << " - " << file << ':' << line	 << "\n";
		err = glGetError();
	}
}
