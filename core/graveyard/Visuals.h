
#include <span>
#include <memory>
#include "ID.h"
#include "Math.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "Lights.h"
#include "../util/MultiIterator.hpp"


namespace Engine::Visuals {
	
	namespace Basic {
		struct basic_tag {};
		using Visual = ID<basic_tag, std::uint64_t>;

		struct VisualState {
			Engine::Matrix world;
			Visual id;
		};

		struct RenderState{
			const Engine::Camera & cam;
			Graphics::Generic::RGBA GlobalAmbientLight{ .1f,.1f,.1f,0.f };
			std::vector<VisualState> states;
			std::vector<Engine::Lights::AnyLight> lights;
		};

		struct ScreenSize {
			float width{ 0 };
			float height{ 0 };
			float aspect_ratio() const noexcept { return width / height; }
		};

		using input = RenderState;
		//using output = Visual;


		Visual Create(Engine::MeshView mesh, Engine::Material mat);
		void Render(RenderState && state);
		ScreenSize Get_ScreenSize();

		/*
		Visual CopyMesh(Visual src, Engine::Material mat);
		Visual CopyMaterial(Visual src, Engine::MeshView mesh);
		void	Remove(Visual src);
		void	SetTag(Visual src, uint32_t tag);
		void	SetName(Visual src, std::string name);
		*/
	}
	
	struct Custom {};
	struct ParticleSystem {};
	struct MeshAtlas {};

	/*using Visual = ID<Basic, std::uint64_t>;


	

	template<typename VisualIDType>
	struct VisualStates {
		std::unique_ptr<VisualIDType> visual_data;
		std::unique_ptr<Engine::Matrix> matrix_data;
		std::size_t visual_count;
		Engine::Matrix	view;

		using iterator = Util::MultiIterator<VisualIDType, Engine::Matrix>;

		auto begin() { return iterator{0, visual_data.get(), matrix_data.get()}; }
		auto end() { return iterator::EndIterator{ visual_count }; }
	};

	
	void RenderBasicID(VisualStates<Visual> && data);
	

	Visual	Create(Engine::MeshView mesh, Engine::Material mat);
	*/
	
	//Render is a async function call 
	// Everything within Render is synchronous
	/*template <typename VisualType>
	void Render(VisualStates<VisualType> && state) {} //No body for unknown VisualTypes

	
	template <>
	void Render<Visual>(VisualStates<Visual> && state) {
		RenderBasicID(std::forward< VisualStates<Visual> >(state));
	}*/
}