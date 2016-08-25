#include "pch.h"
#include <Kore/IO/FileReader.h>
#include <Kore/Graphics/Graphics.h>
#include <Kore/Graphics/Shader.h>
#include <Kore/Math/Matrix.h>
#include <Kore/System.h>
#include <Kore/Compute/Compute.h>
#include <limits>

using namespace Kore;

namespace {
	Shader* vertexShader;
	Shader* fragmentShader;
	Program* program;
	VertexBuffer* vertices;
	IndexBuffer* indices;
	Texture* texture;
	TextureUnit texunit;
	ConstantLocation offset;
	ComputeShader* computeShader;
	ComputeTextureUnit computeTexunit;
	ComputeConstantLocation computeLocation;
	
	float width = 1024;
	float height = 768;

	void update() {
		Graphics::begin();
		Graphics::clear(Graphics::ClearColorFlag | Graphics::ClearDepthFlag);

		Kore::Compute::setShader(computeShader);
		Kore::Compute::setTexture(computeTexunit, texture);
		Kore::Compute::setFloat(computeLocation, 0);
		Kore::Compute::compute(texture->width, texture->height, 1);
		
		program->set();
		Graphics::setMatrix(offset, mat3::RotationZ(0));// (float)Kore::System::time()));
		Graphics::setVertexBuffer(*vertices);
		Graphics::setIndexBuffer(*indices);
		Graphics::setTexture(texunit, texture);
		Graphics::drawIndexedVertices();

		Graphics::end();
		Graphics::swapBuffers();
	}
}

int kore(int argc, char** argv) {
	Kore::System::setName("Compute");
	Kore::System::setup();
	Kore::WindowOptions options;
	options.title = "Compute";
	options.width = 1024;
	options.height = 768;
	options.x = 100;
	options.y = 100;
	options.targetDisplay = -1;
	options.mode = WindowModeWindow;
	options.rendererOptions.depthBufferBits = 16;
	options.rendererOptions.stencilBufferBits = 8;
	options.rendererOptions.textureFormat = 0;
	options.rendererOptions.antialiasing = 0;
	Kore::System::initWindow(options);
	Kore::System::setCallback(update);

	//texture = new Texture("parrot.png");
	texture = new Texture(256, 256, Image::Format::RGBA128, false);

	FileReader cs("test.comp");
	computeShader = new ComputeShader(cs.readAll(), cs.size());

	computeTexunit = computeShader->getTextureUnit("destTex");
	computeLocation = computeShader->getConstantLocation("roll");

	FileReader vs("shader.vert");
	FileReader fs("shader.frag");
	vertexShader = new Shader(vs.readAll(), vs.size(), VertexShader);
	fragmentShader = new Shader(fs.readAll(), fs.size(), FragmentShader);
	VertexStructure structure;
	structure.add("pos", Float3VertexData);
	structure.add("tex", Float2VertexData);
	program = new Program;
	program->setVertexShader(vertexShader);
	program->setFragmentShader(fragmentShader);
	program->link(structure);

	texunit = program->getTextureUnit("texsampler");
	offset = program->getConstantLocation("mvp");

	vertices = new VertexBuffer(3, structure);
	float* v = vertices->lock();
	v[0] = -1.0f; v[1] = -1.0f; v[2] = 0.5f; v[3] = 0.0f; v[4] = 1.0f;
	v[5] = 1.0f; v[6] = -1.0f; v[7] = 0.5f; v[8] = 1.0f; v[9] = 1.0f;
	v[10] = -1.0f; v[11] = 1.0f; v[12] = 0.5f; v[13] = 0.0f; v[14] = 0.0f;
	vertices->unlock();

	indices = new IndexBuffer(3);
	int* i = indices->lock();
	i[0] = 0; i[1] = 1; i[2] = 2;
	indices->unlock();

	Kore::System::start();

	return 0;
}
