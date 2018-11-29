#include "pch.h"
#include <Kore/IO/FileReader.h>
#include <Kore/Graphics4/Graphics.h>
#include <Kore/Graphics4/PipelineState.h>
#include <Kore/Graphics4/Shader.h>
#include <Kore/Math/Matrix.h>
#include <Kore/System.h>
#include <Kore/Compute/Compute.h>
#include <limits>
#include <cmath>

using namespace Kore;

namespace {
	Graphics4::Shader* vertexShader;
	Graphics4::Shader* fragmentShader;
	Graphics4::PipelineState* pipeline;
	Graphics4::VertexBuffer* vertices;
	Graphics4::IndexBuffer* indices;
	Graphics4::Texture* texture;
	Graphics4::TextureUnit texunit;
	Graphics4::ConstantLocation offset;
	ComputeShader* computeShader;
	ComputeTextureUnit computeTexunit;
	ComputeConstantLocation computeLocation;
	float t = 0.0f;
	int time, limit;
	
	float width = 1024;
	float height = 768;

	void update() {
		Graphics4::begin();
		Graphics4::clear(Graphics4::ClearColorFlag | Graphics4::ClearDepthFlag);

		Kore::Compute::setShader(computeShader);
		Kore::Compute::setTexture(computeTexunit, texture, Compute::Write);
		t = time > 0 ? (float(time) / limit) * float(M_PI * 2) : 0.0f;
		Kore::Compute::setFloat(computeLocation, t);
		Kore::Compute::compute(texture->width, texture->height, 1);
		
		Graphics4::setPipeline(pipeline);
		Graphics4::setMatrix(offset, mat3::RotationZ(t));// (float)Kore::System::time()));
		Graphics4::setVertexBuffer(*vertices);
		Graphics4::setIndexBuffer(*indices);
		Graphics4::setTexture(texunit, texture);
		Graphics4::drawIndexedVertices();

		time = (time + 1) % limit;
		Graphics4::end();
		Graphics4::swapBuffers();
	}
}

int kore(int argc, char** argv) {
	Kore::System::init("Shader", 1024, 768);
	Kore::System::setCallback(update);

	//texture = new Texture("parrot.png");
	texture = new Graphics4::Texture(512, 512, Graphics4::Image::RGBA64, false);

	FileReader cs("test.comp");
	computeShader = new ComputeShader(cs.readAll(), cs.size());

	computeTexunit = computeShader->getTextureUnit("destTex");
	computeLocation = computeShader->getConstantLocation("roll");

	FileReader vs("shader.vert");
	FileReader fs("shader.frag");
	vertexShader = new Graphics4::Shader(vs.readAll(), vs.size(), Graphics4::VertexShader);
	fragmentShader = new Graphics4::Shader(fs.readAll(), fs.size(), Graphics4::FragmentShader);
	Graphics4::VertexStructure structure;
	structure.add("pos", Graphics4::Float3VertexData);
	structure.add("tex", Graphics4::Float2VertexData);
	pipeline = new Graphics4::PipelineState;
	pipeline->inputLayout[0] = &structure;
	pipeline->inputLayout[1] = nullptr;
	pipeline->vertexShader = vertexShader;
	pipeline->fragmentShader = fragmentShader;
	pipeline->compile();

	texunit = pipeline->getTextureUnit("texsampler");
	offset = pipeline->getConstantLocation("mvp");

	vertices = new Graphics4::VertexBuffer(4, structure);
	float* v = vertices->lock();
	v[0] = -2.0f; v[1] = -2.0f; v[2] = 0.5f; v[3] = 0.0f; v[4] = 1.0f;
	v[5] = 2.0f; v[6] = -2.0f; v[7] = 0.5f; v[8] = 1.0f; v[9] = 1.0f;
	v[10] = -2.0f; v[11] = 2.0f; v[12] = 0.5f; v[13] = 0.0f; v[14] = 0.0f;
	v[15] = 2.0; v[16] = 2.0; v[17] = 0.5; v[18] = 1.0; v[19] = 0.0;
	vertices->unlock();

	indices = new Graphics4::IndexBuffer(6);
	int* i = indices->lock();
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 1; i[4] = 3; i[5] = 2;
	indices->unlock();

	time = 0;
	limit = 1440;

	Kore::System::start();

	return 0;
}
