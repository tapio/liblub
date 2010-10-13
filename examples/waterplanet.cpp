#include "Application.h"
#include "MengerSponge.h"

class Sponge : public Application {

public:
	Sponge(){
		programName = "WaterPlanet";
  }

	void scene(){
		SceneGraph::Instance().addNode(new Node("Planet",{0,0,-10}, 10, MeshFactory::load("earth.obj"),new OceanMat()));
	}
};

int main(int argc, char *argv[])
{
	Sponge().run();

}
