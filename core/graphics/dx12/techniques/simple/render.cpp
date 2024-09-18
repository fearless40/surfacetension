#include "render.hpp"
#include "D3D12MemAlloc.h"
using namespace dx12::SimpleRender

  Mesh
    Render::create_static_mesh(const std::span<VertexType> &vertexs,
                               const std::span<IndexType> &indexs) {
    
    
}
  void render(CommandList &list);

private:
  D3D12Driver &m_driver;
};
