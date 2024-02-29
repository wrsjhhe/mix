#include <nodes/core/NodeBuilder.h>
#include <core/Object3D.h>
#include <scenes/Scene.h>
#include <materials/Material.h>
#include <nodes/core/NodeParser.h>
#include <renderers/common/Renderer.h>

#include <unordered_map>
#include <string>

using namespace mix;

std::unordered_map<uint32_t, std::string> typeFromLength = { 
	{2, "vec2"},
	{3, "vec3"},
	{4, "vec4"},
	{9, "mat3"},
	{16, "mat4"},
};


NodeBuilder::NodeBuilder(Object3D* object, Renderer* renderer, NodeParser* parser, Scene* scene, Material* material):
	object(object), renderer(renderer), parser(parser), scene(scene), material(material)
{
	if (this->material == nullptr) {
		this->material = object->material();
	}


}