
#include <span>

namespace dx12
{
    /**
     * Can only render static geometry
     * forward based lighting only (max 8 lights)
     */
    namespace SimpleRender
    {
        struct Mesh
        {
        };

        struct Material
        {
        };

        struct VertexType
        {
            float x, y, z;
            float nx, ny, nz;
            float u, v;
        };

        using IndexType = unsigned int;

        struct Light
        {
            float x, y, z, f;
        };

        class Render
        {
            Mesh create_static_mesh(const std::span<VertextType> &vertexs, const std::span<IndexType> &indexs);
            void render(CommandList &list);
        };

        class CommandList
        {
        public:
            void set_world_projection(matrix m);
            void set_global_lighting(Light l);
            void add_spot_light();
            void add_diret_light();
            void add_other_light();
            void add_mesh(const Mesh &mesh, Matrix position, Material mat);
            
            /**
            * Just resets the internal vector of meshs
            */
            void reset();
        }
    }

}
}