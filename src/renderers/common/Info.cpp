#include <renderers/common/Info.h>

using namespace mix;

Info::Info() {

}

Info::~Info() {

}

void Info::update() {

}

void Info::reset() {
	render.drawCalls = 0;
	render.triangles = 0;
	render.points = 0;
	render.lines = 0;

	timestamp.render = 0;
}