#include "SceneGraph.h"
#include "Camera.h"
#include <math.h>
#include <boost/foreach.hpp>

SceneGraph::SceneGraph()
{

}

/* Multiply 4x4 matrix m1 by 4x4 matrix m2 and store the result in m1 */
void SceneGraph::multiply4x4(GLfloat *m1, GLfloat *m2)
{
    GLfloat temp[16];

    int x,y;

    for (x=0; x < 4; x++)
    {
        for(y=0; y < 4; y++)
        {
            temp[y + (x*4)] = (m1[x*4] * m2[y]) +
                              (m1[(x*4)+1] * m2[y+4]) +
                              (m1[(x*4)+2] * m2[y+8]) +
                              (m1[(x*4)+3] * m2[y+12]);
        }
    }

    memcpy(m1, temp, sizeof(GLfloat) * 16);
}



/* Perform translation operations on a matrix */
void SceneGraph::translate(GLfloat *matrix, GLfloat x, GLfloat y, GLfloat z)
{
    GLfloat newmatrix[16] = IDENTITY_MATRIX4;

    newmatrix[12] = x;
    newmatrix[13] = y;
    newmatrix[14] = z;

    multiply4x4(matrix, newmatrix);
}

/* Rotate a matrix by an angle on a X, Y, or Z axis */
void SceneGraph::rotate(GLfloat *matrix, GLfloat angle, AXIS axis)
{
    const GLfloat d2r = 0.0174532925199; /* PI / 180 */
    const int cos1[3] = { 5, 0, 0 };
    const int cos2[3] = { 10, 10, 5 };
    const int sin1[3] = { 6, 2, 1 };
    const int sin2[3] = { 9, 8, 4 };
    GLfloat newmatrix[16] = IDENTITY_MATRIX4;

    newmatrix[cos1[axis]] = cos(d2r * angle);
    newmatrix[sin1[axis]] = -sin(d2r * angle);
    newmatrix[sin2[axis]] = -newmatrix[sin1[axis]];
    newmatrix[cos2[axis]] = newmatrix[cos1[axis]];

    multiply4x4(matrix, newmatrix);
}

void SceneGraph::transpose3x3(GLfloat *matrix)
{
   GLfloat temp;
   for(int i=0;i<3;i++)
       for(int j=i+1;j<3;j++)
      {
         temp=matrix[i + (j*3)];
         matrix[i + (j*3)]=matrix[j + (i*3)];
         matrix[j + (i*3)]=temp;
      }
}

void SceneGraph::animate(float frameCount){
    rotate(modelmatrix, (GLfloat) frameCount * -1.0, X_AXIS);
    rotate(modelmatrix, (GLfloat) frameCount * 1.0, Y_AXIS);
    rotate(modelmatrix, (GLfloat) frameCount * 0.5, Z_AXIS);
}

void SceneGraph::transform(){
    /* Load the identity matrix into modelmatrix. rotate the model, and move it back 5 */
    memcpy(modelmatrix, identitymatrix, sizeof(GLfloat) * 16);
    //animate(frameCount);
    //translate(modelmatrix, -5, -5, -5);
    rotate(modelmatrix, (GLfloat) Camera::Instance().yaw, X_AXIS);
    rotate(modelmatrix, (GLfloat) Camera::Instance().pitch, Y_AXIS);
    rotate(modelmatrix, (GLfloat) Camera::Instance().roll, Z_AXIS);
    //translate(modelmatrix, 0, 0, -2.5);
    //translate(modelmatrix, 5, 5, 5);
    translate(modelmatrix, Camera::Instance().x, Camera::Instance().y, Camera::Instance().z);

    //cout<<"Cam " << Camera::Instance().x<<" "<< Camera::Instance().y<<" "<< Camera::Instance().z<<"\n";


}

void SceneGraph::transform(float frameCount){
    /* Load the identity matrix into modelmatrix. rotate the model, and move it back 5 */
    memcpy(modelmatrix, identitymatrix, sizeof(GLfloat) * 16);
    //animate(frameCount);
    //translate(modelmatrix, -5, -5, -5);
    rotate(modelmatrix, (GLfloat) Camera::Instance().yaw, X_AXIS);
    rotate(modelmatrix, (GLfloat) Camera::Instance().pitch, Y_AXIS);
    rotate(modelmatrix, (GLfloat) Camera::Instance().roll, Z_AXIS);
    //translate(modelmatrix, 0, 0, -2.5);
    //translate(modelmatrix, 5, 5, 5);
    translate(modelmatrix, Camera::Instance().x, Camera::Instance().y, Camera::Instance().z);

    //cout<<"Cam " << Camera::Instance().x<<" "<< Camera::Instance().y<<" "<< Camera::Instance().z<<"\n";


}

void SceneGraph::translate(float x, float y, float z){
    translate(modelmatrix, x, y, z);
}

void SceneGraph::translate(vector<float> translation){
    translate(modelmatrix, translation.at(0), translation.at(1), translation.at(2));
}

void SceneGraph::bindShaders(ShaderProgram * shaderProgram){

    shaderProgram->setNormalMatrix(modelmatrix);
	shaderProgram->setModelViewMatrix(modelmatrix);
    /* multiply our modelmatrix and our projectionmatrix. Results are stored in modelmatrix */
    multiply4x4(modelmatrix, Camera::Instance().projectionmatrix);
    shaderProgram->setModelViewProjectionMatrix(modelmatrix);
    shaderProgram->setLightPosition(-2.0, 1.0, 2.0);
}


void SceneGraph::addNode(string name, vector<float> position, Mesh * mesh, Material * material){
	sceneNodes.push_back(Node(name, position, mesh, material));
	materials.push_back(material);
}

void SceneGraph::addNode(string name, vector<float> position, Mesh * mesh){
	sceneNodes.push_back(Node(name, position, mesh));
}

void SceneGraph::drawNodes(){
    BOOST_FOREACH( Node node, sceneNodes )
    {
        transform();
        translate(node.getPosition());
        bindShaders(node.getMaterial()->shaderProgram);
    	node.draw();
    }

}

void SceneGraph::initUniforms(){
    BOOST_FOREACH( Material* material, materials )
    {
        material->initUniforms();
    }

}