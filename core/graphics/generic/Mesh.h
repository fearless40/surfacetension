#pragma once

#include "../Graphics/Generic/Vertexs.h"
#include "../Graphics/Generic/IndexSizes.h"

namespace Engine {

	struct Mesh {
		using vertex_type = Graphics::Generic::VertexTypes::PositionNormalUV;
		using index_definition = Graphics::Generic::IndexSize::Size16;
		using index_type = index_definition::type;
		
		std::vector<vertex_type> vertexs;
		std::vector<index_type> indicies;
	};

	struct MeshView {
		std::span<const std::byte> vertex_data;
		std::span<const std::byte> index_data;
	};

	template <typename Vertexs, typename Indexs> 
	MeshView make_meshview(const Vertexs & vxs, const Indexs & ixs) {
		std::span<const std::byte> vertex_view((std::byte*) vxs.data(), vxs.size() * sizeof(Vertexs::value_type));
		std::span<const std::byte> index_view((std::byte*) ixs.data(), ixs.size() * sizeof(Indexs::value_type));
		return { vertex_view, index_view };
	}
}