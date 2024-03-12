#include <nodes/shadernode/ShaderNode.h>
#include <nodes/core/StackNode.h>

using namespace mix;

static StackNode* currentStack = nullptr;

void mix::setCurrentStack(StackNode* stack) {
	if (currentStack == stack) {

		//throw new Error( 'Stack already defined.' );

	}

	currentStack = stack;
}