/*
 * Copyright © 2010 Lubosz Sarnecki
 * Application.h
 *
 *  Created on: Jul 20, 2010
 */

#pragma once

#include "Window/MediaLayer.h"
#include "Material/Materials.h"
#include "Mesh/MeshLoader.h"
#include "Scene/SceneLoader.h"
#include "Scene/SceneData.h"
#include "System/Config.h"
#include "System/GUI.h"

class Application {
 public:
	virtual ~Application() {}
	virtual void scene() = 0;
	virtual void renderFrame() = 0;

	SceneLoader * sceneLoader;

	void run() {
		Config::Instance().load("config.xml");
		MediaLayer::Instance();
		MediaLayer::Instance().init(SceneData::Instance().name);
		scene();
		GUI::Instance().init();
    while (!MediaLayer::Instance().quit) {
      MediaLayer::Instance().renderFrame();
      renderFrame();
     }
	}
};

