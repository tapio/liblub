/*
 * config.h
 *
 *  Created on: Jun 8, 2010
 *      Author: bmonkey
 */
#include <string>

using namespace std;

#pragma once
#define PROGRAM_NAME "liblub"
const string shaderDir = "media/shaders/";
const string textureDir = "media/textures/";
static const string suffixes[6] = {"_RT", "_LF", "_DN","_UP", "_FR", "_BK"};
#define DEBUG 0
#define USE_GL3

