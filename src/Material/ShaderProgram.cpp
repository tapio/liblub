/*
 * ShaderSystem.cpp
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#include "ShaderProgram.h"
#include <boost/foreach.hpp>


ShaderProgram::ShaderProgram() {
	attribCount = 0;
    /* Assign our program handle a "name" */
    program = glCreateProgram();
    cout << "Creating Program #" << program <<"\n";
}

ShaderProgram::~ShaderProgram() {
	cout << "Shutting down Shader System...";
    glUseProgram(0);

    while(shaders.size() > 0){
    	detachShader(shaders.back());
    }

	cout << "done.\n";
}

void ShaderProgram::printProgramInfoLog() {
	int infologLen = 0;
	int charsWritten = 0;
	GLchar *infoLog;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infologLen);
	//printOpenGLError();  // Check for OpenGL errors
	if (infologLen > 0) {
		infoLog = (GLchar*) malloc(infologLen);
		if (infoLog == NULL) {
			printf("ERROR: Could not allocate InfoLog buffer\n");
			exit(1);
		}
		glGetProgramInfoLog(program, infologLen, &charsWritten, infoLog);
		printf("Program Log:\n%s\n\n", infoLog);
		free(infoLog);
	} else {
		printf("Program Info Log: OK\n");
	}
	//printOpenGLError();  // Check for OpenGL errors
}

void ShaderProgram::attachShader(string fileName, GLenum type){
    /* Attach our shaders to our program */
	Shader * shader = new Shader(fileName, type);
	shaders.push_back(shader);
	glAttachShader(program, shader->getReference());
}

void ShaderProgram::detachShader(Shader *shader){
	glDetachShader(program, shader->getReference());
	shaders.remove(shader);
	delete shader;
}

void ShaderProgram::bindAttrib(unsigned position, string name){
	glBindAttribLocation(program, position, name.c_str());
}

void ShaderProgram::bindAttribIfUnbound(string name){
	//glBindAttribLocation(program, position, name.c_str());
	BOOST_FOREACH( string attrib, boundAttribs )
	    {
	    	if (attrib == name) return;
	    }
	cout << "Binding " <<name <<"\n";
	boundAttribs.push_back(name);
	bindAttrib(name);
}



void ShaderProgram::bindAttrib(string name){
	bindAttrib(attribCount, name);
	attribCount++;
}

void ShaderProgram::linkAndUse(){
    /* Link our program, and set it as being actively used */
    glLinkProgram(program);
    printProgramInfoLog();
    use();
}

void ShaderProgram::use(){
	glUseProgram(program);
}

void ShaderProgram::reload(){
    glUseProgram(0);
    program = glCreateProgram();

    BOOST_FOREACH( Shader* shader, shaders )
    {
    	/*

        delete shader;
        */
    	shader->reload();
    	//attachShader(shader->fileName,shader->type);
    }

    linkAndUse();
	//ShaderFactory::Instance().addUniforms();

}

void ShaderProgram::setNormalMatrix(GLfloat * modelmatrix){
    GLfloat normalMatrix[9] = {
                    modelmatrix[0], modelmatrix[1], modelmatrix[2],
                    modelmatrix[4], modelmatrix[5], modelmatrix[6],
                    modelmatrix[8], modelmatrix[9], modelmatrix[10]
    };

	glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"), 1, GL_FALSE, normalMatrix);
}

void ShaderProgram::setModelViewMatrix(GLfloat * mvMatrix){
	glUniformMatrix4fv(glGetUniformLocation(program, "MVMatrix"), 1, GL_FALSE, mvMatrix);
}

void ShaderProgram::setModelViewProjectionMatrix(GLfloat * mvpMatrix){
    /* Bind our modelmatrix variable to be a uniform called mvpmatrix in our shaderprogram */
	glUniformMatrix4fv(glGetUniformLocation(program, "MVPMatrix"), 1, GL_FALSE, mvpMatrix);
}

void ShaderProgram::setLightPosition(GLfloat x, GLfloat y, GLfloat z){
	glUniform3f(glGetUniformLocation(program, "LightPosition"), x, y, z);
}

GLuint ShaderProgram::getReference() const{
	return program;
}
