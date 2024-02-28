#include <nodes/lighting/LightNode.h>
#include <lights/Light.h>

using namespace mix;

std::string LightNode::TARGET_DIRECTION = "targetDirection";

LightNode::LightNode(const std::string& scope, Light* light):scope(scope),light(light){

}

void LightNode::setup() {

}