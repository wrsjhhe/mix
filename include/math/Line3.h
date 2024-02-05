#pragma once

#include <math/Vector3.h>

namespace mix {

    class Line3 {

    public:
        explicit Line3(const Vector3& start = Vector3(), const Vector3& end = Vector3());

        [[nodiscard]] const Vector3& start() const;

        [[nodiscard]] const Vector3& end() const;

        Line3 set(const Vector3& start, const Vector3& end);

        Line3& copy(const Line3& line);

        void getCenter(Vector3& target) const;

        void delta(Vector3& target) const;

        [[nodiscard]] float distanceSq() const;

        [[nodiscard]] float distance() const;

        void at(float t, Vector3& target) const;

        float closestPointToPointParameter(const Vector3& point, bool clampToLine);

        void closestPointToPoint(const Vector3& point, bool clampToLine, Vector3& target);

        Line3& applyMatrix4(const Matrix4& matrix);

    private:
        Vector3 start_;
        Vector3 end_;
    };

}
