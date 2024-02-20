#pragma once

#include <filesystem>
#include <optional>

#include <textures/Image.h>

namespace mix {

	class ImageLoader {

	public:
		std::optional<Image> load(const std::filesystem::path& imagePath, int channels = 4, bool flipY = true);
		std::optional<Image> load(const std::vector<unsigned char>& data, int channels = 4, bool flipY = true);
	};

}
