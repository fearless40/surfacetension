#include "../stdafx.h"
#include "RawResource.h"

namespace Engine::Resources {

	struct LoaderAndPosition {
		std::unique_ptr<RawLoader> loader;
		int priority;
		bool operator < (const LoaderAndPosition & lp) {
			return priority < lp.priority;
		}
	};

	using RawResourceLoaderList = std::vector<LoaderAndPosition>;

	RawResourceLoaderList loaders;


	std::optional<RawResource> LoadFile(RawID raw_id) {
		auto const position = std::find_if(loaders.begin(), loaders.end(), [&raw_id](const LoaderAndPosition & item) {
			return item.loader->has_item( raw_id );
		});
		if (position != loaders.end()) {
			return position->loader->read_item(raw_id);
		}
		return {}; //Not found
	}

	void SetLoader(std::unique_ptr<RawLoader> loader, int priority) {
		loaders.push_back({ std::move(loader), priority });
		std::sort(loaders.begin(), loaders.end());
	}

}