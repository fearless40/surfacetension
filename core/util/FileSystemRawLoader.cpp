
#include "../stdafx.h"
#include <optional>
#include <memory>
#include <fstream>
#include "FileSystemRawLoader.h"
#include "../util/Overload.hpp"

namespace windows {
	using namespace Engine::Resources;

	FileSystemLoader::FileSystemLoader(std::filesystem::path root_path) : mRootPath{ root_path } {}
	FileSystemLoader::~FileSystemLoader() {

	}

	std::optional<RawResource> FileSystemLoader::load_file(const std::filesystem::path & rel_path) {
		return read_item(rel_path);
	}

	std::optional<std::filesystem::path> extract_id(RawID id) {
		return Util::match(id, 
			[](const RawID_IntID & id) -> std::optional<std::filesystem::path>  { return std::nullopt; },
			[](const std::filesystem::path& path) -> std::optional<std::filesystem::path> { return path ; }
		);
	}

	std::optional<RawResource> FileSystemLoader::read_item(RawID id) const {
		if (auto fid = extract_id(id); fid)
		{
			auto file_path = mRootPath / fid.value();
			if (std::filesystem::exists(file_path)) {
				auto mem_size = std::filesystem::file_size(file_path);
				
				RawResource outpt{ 
					std::make_unique<std::byte[]>(mem_size),
					static_cast<std::size_t>(mem_size),
					file_path
				};

				std::ifstream reader{ file_path };
				reader.read((char*)outpt.file_data.get(), outpt.size);
				return std::optional<RawResource>{std::move(outpt)};
			}
		}
		return {};
	}

	bool FileSystemLoader::has_item(RawID id) const {
		if (auto fid = extract_id(id); fid) {
			auto file_path = mRootPath / fid.value();
			return std::filesystem::exists(file_path);
		}
		return false;
	}
}