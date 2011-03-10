/*
 * Copyright © 2010 Lubosz Sarnecki
 * Texture.h
 *
 *  Created on: Mar 24, 2010
 */

#pragma once

#include "common/OpenGL.h"
#include <string>

using std::string;

class Texture {
 public:
	void activate();
	void bind();
	void uniform(GLuint program);
  GLuint getHandler() const;
	string name;
 protected:
	GLenum glId;
	GLuint texture;
	GLuint textureType;
	GLint glChannelOrder, texChannelOrder;

	virtual ~Texture();

#if USE_FREEIMAGE
	void readFreeImage(GLenum target, string path);
#else
	void readQImage(GLenum target, string path);
#endif
};
