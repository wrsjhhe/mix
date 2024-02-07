#pragma once

namespace mix {

    struct GeometryGroup {

        int start{};
        int count{};
        unsigned int materialIndex{};
    };

    struct UpdateRange {

        int offset{};
        int count{};
    };

    struct DrawRange {

        int start{};
        int count{};
    };

}
