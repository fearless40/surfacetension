
#pragma once

#include "ID.h"
#include "RawResource.h"

namespace Engine::Resources {
	
	
	template<typename ResourceType, typename Tag>
	class ResourceManager {
	public:
		using ID = Engine::ID<ResourceManager, uint32_t>;
		using type = ResourceType;
		using tag = Tag;

		struct ResourceMapItem {
			RawID		raw_id;
			ID			game_id;
		};
		
		using ResourceMap = std::vector<ResourceMapItem>;

		static const type & get(ID id) { return resources[id.id]; }

		// Resource map Functions, useful for games that know what they need to load 
		// and need dedicated values that are hardcoded
		// Also easy to use a file list

		// Clears any existing maps. Does not unload loaded data
		static void		setResourceMap(const ResourceMap & map) {
			std::copy(map.begin(), map.end(), resource_map.end());
		}
		

		// using TransFormFunctor = ResourceType 

		template< typename TransFormFunctor>
		static void		loadResourceMap(const ResourceMap & map, TransFormFunctor && func);


		template< typename TransFormFunctor>
		static ID		load(RawResourceID rid, TransFormFunctor && func);
		
		static ID		create(ResourceType && resource); //No RawID is created. It has no entry in resource_map

		static void		unload(ID id); // Frees the memory. If it needs to be reloaded from disk it is
		static void		unloadAll();
		static void		removalAll();


	private:
		static std::vector<ResourceType> resources;
		static std::vector<ResourceMapItem> resource_map; // not sorted
	};
	
}