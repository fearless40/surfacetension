#pragma once

namespace Engine::Resources {
	std::optional<RawResource> LoadFile(RawID raw_id);
	void		SetLoader(std::unique_ptr<RawLoader> loader, int priority);
}