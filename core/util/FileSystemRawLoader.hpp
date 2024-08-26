#pragma once

#include <filesystem>
#include "../engine/RawResource.h"


namespace windows {
	class FileSystemLoader : public Engine::Resources::RawLoader {
	public:
		FileSystemLoader(std::filesystem::path root_path);
		~FileSystemLoader();
		std::optional<Engine::Resources::RawResource> read_item(Engine::Resources::RawID id) const override;
		bool		has_item(Engine::Resources::RawID id) const override;

	private:

		std::optional<Engine::Resources::RawResource> load_file(const std::filesystem::path & rel_path);
		std::filesystem::path mRootPath;
	};
}