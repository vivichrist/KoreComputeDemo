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
	ComputeShader* computeShader;
	
	float width = 1024;
	float height = 768;

	void update() {
		Graphics::begin();
		Graphics::clear(Graphics::ClearColorFlag | Graphics::ClearDepthFlag);
		
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

	FileReader cs("test.comp");
	computeShader = new ComputeShader(cs.readAll(), cs.size());

	Kore::Compute::setShader(computeShader);
	Kore::Compute::compute(16, 16, 1);

	Kore::System::start();

	return 0;
}
