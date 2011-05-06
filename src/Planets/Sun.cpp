/*
 * Sun.cpp
 *
 *  Created on: May 5, 2011
 *      Author: bmonkey
 */

#include "Sun.h"
#include "System/Timer.h"
#include "Scene/SceneData.h"
#include "Material/Materials.h"
#include "Mesh/Geometry.h"

Sun::Sun(float innerRadius, float outerRadius) {
  this->innerRadius = innerRadius;
  this->outerRadius = outerRadius;
}

Sun::~Sun() {
  // TODO Auto-generated destructor stub
}

void Sun::init(){
  QList<string> sunAttributes;
  sunAttributes.push_back("normal");
  sunAttributes.push_back("uv");
  Material * sunMaterial = new Template("perlin",sunAttributes);
  sunNode = new Node("sun", { 0,0,0 }, 1, Geometry::sphere(sunAttributes, innerRadius, 100, 50), sunMaterial);
  sunNode->setRotation(QVector3D(-90,0,180));
  perlinNoise = sunMaterial->getShaderProgram();
}
void Sun::draw(){
  float time = float(Timer::Instance().secoundsPassed) + float(Timer::Instance().nanosecoundsPassed)/1000000000.0;
  perlinNoise->use();
  perlinNoise->setUniform("time", time);
  sunNode->setView(SceneData::Instance().getCurrentCamera());
  sunNode->draw();
}