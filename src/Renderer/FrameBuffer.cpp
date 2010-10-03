/*
 * FrameBuffer.cpp
 *
 *  Created on: Aug 31, 2010
 *      Author: bmonkey
 */

#include "FrameBuffer.h"
#include "Materials.h"
#include "SceneGraph.h"
#include "Camera.h"
#include "MeshFactory.h"

#include <sstream>

FrameBuffer::FrameBuffer(GLuint width, GLuint height) {
	glError("FrameBuffer",12);
	//Gen texture for fbo
    // create a texture object
	this->width = width;
	this->height = height;
	useFBO = false;

	//pass1Mat = new ShadowMap(width, height);
	pass1Mat = new FBOMaterial(width, height);


	/*
	Texture * pass1 = TextureFactory::Instance().texture(width, height, "RenderTexture");
	Texture * pass2 = TextureFactory::Instance().texture(width, height, "RenderTexture");

	pass1Mat = new FBOMaterial();
	pass1Mat->addTexture(pass1);
	pass1Mat->done();

	pass2Mat = new FBOMaterial();
	pass2Mat->addTexture(pass2);
	pass2Mat->done();
*/

    renderPlane = MeshFactory::Instance().plane();

	//pass1Mat = new TextureMaterial("bunny.png");
	glError("FrameBuffer",33);

    glGenFramebuffers(1, &fboId);
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);

    // create a renderbuffer object to store depth info
    // NOTE: A depth renderable image should be attached the FBO for depth test.
    // If we don't attach a depth renderable image to the FBO, then
    // the rendering output will be corrupted because of missing depth test.
    // If you also need stencil test for your rendering, then you must
    // attach additional image to the stencil attachement point, too.

    glGenRenderbuffers(1, &rboId);
    glBindRenderbuffer(GL_RENDERBUFFER, rboId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

/*
    GLuint stencilbuffer;                       // ID of Renderbuffer object
    glGenRenderbuffers(1, &stencilbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, stencilbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX1, TEXTURE_WIDTH, TEXTURE_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
*/
    Texture * fboTexture = pass1Mat->textures[0];

    // attach a texture to FBO color attachement point
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fboTexture->getHandler(), 0);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, pass2->getHandler(), 0);
    // attach a renderbuffer to depth attachment point
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboId);
    //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencilbuffer);

    //@ disable color buffer if you don't attach any color buffer image,
    //@ for example, rendering depth buffer only to a texture.
    //@ Otherwise, glCheckFramebufferStatusEXT will not be complete.
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    // check FBO status
    printFramebufferInfo();
    checkFramebufferStatus();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glError("FrameBuffer",74);

}

void FrameBuffer::bind() {
	if (!useFBO) return;
    // set the rendering destination to FBO
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);

    // Set the render target
    //glDrawBuffer(GL_COLOR_ATTACHMENT0);
    //glDrawBuffer(GL_DEPTH_ATTACHMENT);





    glError("FrameBuffer::bind", 105);


    //GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT };
	//glDrawBuffers(2, buffers);
}

void FrameBuffer::unBind() {
    // back to normal window-system-provided framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind
}

void FrameBuffer::draw() {
	if (!useFBO) {
		SceneGraph::Instance().drawNodes();
		return;
	}
	glError("FrameBuffer::draw", 105);


    //Using the fixed pipeline to render to the depthbuffer
    //glUseProgram(0);

    // In the case we render the shadowmap to a higher resolution, the viewport must be modified accordingly.
    glViewport(0,0,width, height);

    // Clear previous frame values
    glClear( GL_DEPTH_BUFFER_BIT);

    //Disable color rendering, we only want to write to the Z-Buffer
    //glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	//make shadowmap
	// Culling switching, rendering only backface, this is done to avoid self-shadowing


	//glCullFace(GL_FRONT);
	SceneGraph::Instance().drawNodesLight();
	unBind();
/*
	// Now rendering from the camera POV, using the FBO to generate shadows
	glViewport(0,0,width,height);

	//Enabling color write (previously disabled for light POV z-buffer rendering)
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	//Using the shadow shader
	pass1Mat->activate();

	//setupMatrices(p_camera[0],p_camera[1],p_camera[2],l_camera[0],l_camera[1],l_camera[2]);
	//setupMatrices(p_light[0],p_light[1],p_light[2],l_light[0],l_light[1],l_light[2]);

	glCullFace(GL_BACK);
	RenderEngine::Instance().clear();

	SceneGraph::Instance().drawNodes(pass1Mat->getShaderProgram());
*/


	RenderEngine::Instance().clear();
	glBindTexture(GL_TEXTURE_2D, 0);

	glViewport(0,0,width, height);

	bindShaders(pass1Mat->getShaderProgram());

	pass1Mat->activate();

	renderPlane->draw();
	glError("FrameBuffer::draw", 171);

	/*
	bindShaders(pass2Mat->getShaderProgram());

	glBindTexture(GL_TEXTURE_2D, 0);
	pass2Mat->activate();

	renderPlane->draw();
	//Camera::Instance().perspective();
*/
}

void FrameBuffer::bindShaders(ShaderProgram * shaderProgram) {
	glError("FrameBuffer::bindShaders", 128);

	shaderProgram->use();
	glError("FrameBuffer::bindShaders", 188);
	/*
	QMatrix4x4 bias = {
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
	};

	QMatrix4x4 camViewToShadowMapMatrix =
			bias
			* SceneGraph::Instance().light->getProjection()
			* SceneGraph::Instance().light->getView()
			* Camera::Instance().getView().inverted();

	shaderProgram->setUniform(camViewToShadowMapMatrix, "camViewToShadowMapMatrix");
*/
	//bias*perspLight*viewLight*(viewCam⁻1)
	shaderProgram->setUniform(QMatrix4x4(), "MVPMatrix");
/*


	//invProjView
	QMatrix4x4 invProjView = QMatrix4x4();
	invProjView = Camera::Instance().getView() * invProjView;
	invProjView = Camera::Instance().getProjection() * invProjView;
	shaderProgram->setUniform(invProjView.inverted(), "invProjView");
*/
	glError("FrameBuffer::bindShaders", 216);
}

void FrameBuffer::toggle(){
	if(useFBO){
		cout << "FBO Rendering diabled" << "\n";
		useFBO = false;
	}else{
		useFBO = true;
		cout << "FBO Rendering enabled" << "\n";
	}
}

FrameBuffer::~FrameBuffer() {
	glDeleteFramebuffers(1, &fboId);
	glDeleteRenderbuffers(1, &rboId);
}

void FrameBuffer::printFramebufferInfo()
{
    cout << "\n***** FBO STATUS *****\n";

    // print max # of colorbuffers supported by FBO
    int colorBufferCount = 0;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &colorBufferCount);
    cout << "Max Number of Color Buffer Attachment Points: " << colorBufferCount << endl;

    int objectType;
    int objectId;

    // print info of the colorbuffer attachable image
    for(int i = 0; i < colorBufferCount; ++i){
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                              GL_COLOR_ATTACHMENT0+i,
                                              GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
                                              &objectType);
        if(objectType != GL_NONE){
            glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                                  GL_COLOR_ATTACHMENT0+i,
                                                  GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                                                  &objectId);

            string formatName;

            cout << "Color Attachment " << i << ": ";
            if(objectType == GL_TEXTURE)
                cout << "GL_TEXTURE, " << getTextureParameters(objectId) << endl;
            else if(objectType == GL_RENDERBUFFER)
                cout << "GL_RENDERBUFFER, " << getRenderbufferParameters(objectId) << endl;
        }
    }

    // print info of the depthbuffer attachable image
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                             GL_DEPTH_ATTACHMENT,
                                             GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
                                             &objectType);
    if(objectType != GL_NONE)
    {
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                                 GL_DEPTH_ATTACHMENT,
                                                 GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                                                 &objectId);

        cout << "Depth Attachment: ";
        switch(objectType)
        {
        case GL_TEXTURE:
            cout << "GL_TEXTURE, " << getTextureParameters(objectId) << endl;
            break;
        case GL_RENDERBUFFER:
            cout << "GL_RENDERBUFFER, " << getRenderbufferParameters(objectId) << endl;
            break;
        }
    }

    // print info of the stencilbuffer attachable image
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                             GL_STENCIL_ATTACHMENT,
                                             GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
                                             &objectType);
    if(objectType != GL_NONE)
    {
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                                 GL_STENCIL_ATTACHMENT,
                                                 GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                                                 &objectId);

        cout << "Stencil Attachment: ";
        switch(objectType)
        {
        case GL_TEXTURE:
            cout << "GL_TEXTURE, " << getTextureParameters(objectId) << endl;
            break;
        case GL_RENDERBUFFER:
            cout << "GL_RENDERBUFFER, " << getRenderbufferParameters(objectId) << endl;
            break;
        }
    }

    cout << endl;
}

string FrameBuffer::getTextureParameters(GLuint id)
{
    if(glIsTexture(id) == GL_FALSE)
        return "Not texture object";

    int width, height, format;
    string formatName;
    glBindTexture(GL_TEXTURE_2D, id);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);            // get texture width
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);          // get texture height
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &format); // get texture internal format
    glBindTexture(GL_TEXTURE_2D, 0);

    formatName = convertInternalFormatToString(format);

    stringstream ss;
    ss << width << "x" << height << ", " << formatName;
    return ss.str();
}

string FrameBuffer::getRenderbufferParameters(GLuint id)
{
    if(glIsRenderbuffer(id) == GL_FALSE)
        return "Not Renderbuffer object";

    int width, height, format;
    string formatName;
    glBindRenderbuffer(GL_RENDERBUFFER, id);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);    // get renderbuffer width
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);  // get renderbuffer height
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_INTERNAL_FORMAT, &format); // get renderbuffer internal format
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    formatName = convertInternalFormatToString(format);

    stringstream ss;
    ss << width << "x" << height << ", " << formatName;
    return ss.str();
}

string FrameBuffer::convertInternalFormatToString(GLenum format)
{
    std::string formatName;

    switch(format)
    {
    case GL_STENCIL_INDEX:
        formatName = "GL_STENCIL_INDEX";
        break;
    case GL_DEPTH_COMPONENT:
        formatName = "GL_DEPTH_COMPONENT";
        break;
    case GL_ALPHA:
        formatName = "GL_ALPHA";
        break;
    case GL_RGB:
        formatName = "GL_RGB";
        break;
    case GL_RGBA:
        formatName = "GL_RGBA";
        break;
        /*
    case GL_LUMINANCE:
        formatName = "GL_LUMINANCE";
        break;
    case GL_LUMINANCE_ALPHA:
        formatName = "GL_LUMINANCE_ALPHA";
        break;
         */
    case GL_ALPHA4_EXT:
        formatName = "GL_ALPHA4";
        break;
    case GL_ALPHA8_EXT:
        formatName = "GL_ALPHA8";
        break;
    case GL_ALPHA12_EXT:
        formatName = "GL_ALPHA12";
        break;
    case GL_ALPHA16_EXT:
        formatName = "GL_ALPHA16";
        break;
    case GL_LUMINANCE4_EXT:
        formatName = "GL_LUMINANCE4";
        break;
    case GL_LUMINANCE8_EXT:
        formatName = "GL_LUMINANCE8";
        break;
    case GL_LUMINANCE12_EXT:
        formatName = "GL_LUMINANCE12";
        break;
    case GL_LUMINANCE16_EXT:
        formatName = "GL_LUMINANCE16";
        break;
    case GL_LUMINANCE4_ALPHA4_EXT:
        formatName = "GL_LUMINANCE4_ALPHA4";
        break;
    case GL_LUMINANCE6_ALPHA2_EXT:
        formatName = "GL_LUMINANCE6_ALPHA2";
        break;
    case GL_LUMINANCE8_ALPHA8_EXT:
        formatName = "GL_LUMINANCE8_ALPHA8";
        break;
    case GL_LUMINANCE12_ALPHA4_EXT:
        formatName = "GL_LUMINANCE12_ALPHA4";
        break;
    case GL_LUMINANCE12_ALPHA12_EXT:
        formatName = "GL_LUMINANCE12_ALPHA12";
        break;
    case GL_LUMINANCE16_ALPHA16_EXT:
        formatName = "GL_LUMINANCE16_ALPHA16";
        break;
    case GL_INTENSITY_EXT:
        formatName = "GL_INTENSITY";
        break;
    case GL_INTENSITY4_EXT:
        formatName = "GL_INTENSITY4";
        break;
    case GL_INTENSITY8_EXT:
        formatName = "GL_INTENSITY8";
        break;
    case GL_INTENSITY12_EXT:
        formatName = "GL_INTENSITY12";
        break;
    case GL_INTENSITY16_EXT:
        formatName = "GL_INTENSITY16";
        break;
    case GL_R3_G3_B2:
        formatName = "GL_R3_G3_B2";
        break;
    case GL_RGB4:
        formatName = "GL_RGB4";
        break;
    case GL_RGB5:
        formatName = "GL_RGB4";
        break;
    case GL_RGB8:
        formatName = "GL_RGB8";
        break;
    case GL_RGB10:
        formatName = "GL_RGB10";
        break;
    case GL_RGB12:
        formatName = "GL_RGB12";
        break;
    case GL_RGB16:
        formatName = "GL_RGB16";
        break;
    case GL_RGBA2:
        formatName = "GL_RGBA2";
        break;
    case GL_RGBA4:
        formatName = "GL_RGBA4";
        break;
    case GL_RGB5_A1:
        formatName = "GL_RGB5_A1";
        break;
    case GL_RGBA8:
        formatName = "GL_RGBA8";
        break;
    case GL_RGB10_A2:
        formatName = "GL_RGB10_A2";
        break;
    case GL_RGBA12:
        formatName = "GL_RGBA12";
        break;
    case GL_RGBA16:
        formatName = "GL_RGBA16";
        break;
    default:
        formatName = "Unknown Format";
    }

    return formatName;
}

///////////////////////////////////////////////////////////////////////////////
// check FBO completeness
///////////////////////////////////////////////////////////////////////////////
bool FrameBuffer::checkFramebufferStatus()
{
    // check FBO status
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch(status)
    {
    case GL_FRAMEBUFFER_COMPLETE:
        std::cout << "Framebuffer complete." << std::endl;
        return true;

    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        std::cout << "[ERROR] Framebuffer incomplete: Attachment is NOT complete." << std::endl;
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        std::cout << "[ERROR] Framebuffer incomplete: No image is attached to FBO." << std::endl;
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
        std::cout << "[ERROR] Framebuffer incomplete: Attached images have different dimensions." << std::endl;
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
        std::cout << "[ERROR] Framebuffer incomplete: Color attached images have different internal formats." << std::endl;
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        std::cout << "[ERROR] Framebuffer incomplete: Draw buffer." << std::endl;
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        std::cout << "[ERROR] Framebuffer incomplete: Read buffer." << std::endl;
        return false;

    case GL_FRAMEBUFFER_UNSUPPORTED:
        std::cout << "[ERROR] Unsupported by FBO implementation." << std::endl;
        return false;

    default:
        std::cout << "[ERROR] Unknow error." << std::endl;
        return false;
    }
}
