#include "Application.h"

class Planets : public Application {

public:
	Planets(){
		width = 1920;
		height = 1200;
		programName = "Planets";
  }

	void scene(){
		SceneGraph::Instance().addNode("earth.obj",{2.0f,0.0f,-5.0f}, new PhongTextureMat());
		SceneGraph::Instance().addNode("earth.obj",{0.0f,0.0f,-5.0f}, new PlanetMat("Planets/Mars"));
		SceneGraph::Instance().addNode("earth.obj",{-2.0f,0.0f,-5.0f}, new BumpMaterial("Planets/Mars"));
		SceneGraph::Instance().addNode("earth.obj",{-2.0f,2.0f,-5.0f}, new TextureMaterial("Planets/Mars.jpg"));
		SceneGraph::Instance().addNode("earth.obj",{-2.0f,-2.0f,-5.0f}, new TextureMaterial("Planets/Mars-normal.png"));
	}
};

int main(int argc, char *argv[])
{
	Planets().run();

}