#include <renderers/common/ClippingContext.h>
#include <math/Matrix3.h>
#include <renderers/common/Renderer.h>
#include <cameras/Camera.h>
#include <nodes/materials/NodeMaterial.h>

using namespace mix;

unsigned int _clippingContextVersion = 0;

static Plane _plane;
static Matrix3 _viewNormalMatrix;

ClippingContext::ClippingContext(): version(++_clippingContextVersion){

}

void ClippingContext::projectPlanes(const std::vector<Plane>& source,int offset) {
	size_t l = source.size();

	for (int i = 0; i < l; ++i) {

		_plane.copy(source[i]).applyMatrix4(viewMatrix, _viewNormalMatrix);

		auto& v = planes[offset + i];
		auto& normal = _plane.normal;

		v.x = -normal.x;
		v.y = -normal.y;
		v.z = -normal.z;
		v.w = _plane.constant;

	}
}

void ClippingContext::updateGlobal(Renderer* renderer, Camera* camera) {
	auto rendererClippingPlanes = renderer->clippingPlanes;
	viewMatrix = camera->matrixWorldInverse;

	_viewNormalMatrix.getNormalMatrix(viewMatrix);

	bool update = false;

	if (!rendererClippingPlanes.empty()) {

		size_t l = rendererClippingPlanes.size();

		if (l != globalClippingCount) {
			
			planes.clear();
			planes.resize(l);

			globalClippingCount = l;

			update = true;

		}

		projectPlanes(rendererClippingPlanes, 0);

	}
	else if (globalClippingCount != 0) {

		globalClippingCount = 0;
		planes.clear();
		update = true;

	}

	if (renderer->localClippingEnabled != localClippingEnabled) {

		localClippingEnabled = renderer->localClippingEnabled;
		update = true;

	}

	if (update) version = _clippingContextVersion++;
}

void ClippingContext::update(ClippingContext* parent, NodeMaterial* material) {
	bool update = false;

	if (this != parent && parent->version != parentVersion) {

		globalClippingCount = material->isShadowNodeMaterial ? 0 : parent->globalClippingCount;
		localClippingEnabled = parent->localClippingEnabled;
		planes = parent->planes;
		parentVersion = parent->version;
		viewMatrix = parent->viewMatrix;


		update = true;

	}

	if (localClippingEnabled) {

		auto localClippingPlanes = material->clippingPlanes;

		if (!localClippingPlanes.empty()) {

			size_t l = localClippingPlanes.size();
			unsigned int offset = globalClippingCount;

			if (update || l != localClippingCount) {
				planes.clear();
				planes.resize(offset + l);

				localClippingCount = l;
				update = true;

			}

			projectPlanes(localClippingPlanes, offset);


		}
		else if (localClippingCount != 0) {

			localClippingCount = 0;
			update = true;

		}

		if (localClipIntersection != material->clipIntersection) {

			localClipIntersection = material->clipIntersection;
			update = true;

		}

	}

	if (update) version = _clippingContextVersion++;
}