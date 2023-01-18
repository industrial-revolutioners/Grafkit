#ifndef _Mesh_H_
#define _Mesh_H_

#include <vector>
#include <list>
#include <map>

#include "../utils/reference.h"
#include "../utils/exceptions.h"

#include "../utils/persistence/persistence.h"

#include "renderer.h"
#include "dxtypes.h"

namespace Grafkit
{
    /**
    Stores basic data for a piece of mesh
    */
    class Mesh : public virtual Referencable, public Persistent
    {
    public:
        Mesh();
        Mesh(const Mesh& mesh);
        virtual ~Mesh();

        virtual void UpdateMesh(double time);
        void RenderMesh(ID3D11DeviceContext* deviceContext);

        void AddPointer(std::string inputName, size_t length, const void* pointer);
        void SetIndices(size_t vertexCount, size_t indexCount, const int* const indices);
        void Build(ID3D11Device *const& device, ShaderRef &vertexShader);

        size_t GetIndexCount() const { return m_indexCount; }

    protected:
        void Shutdown();

        struct BufferStateDescriptor {
            ID3D11Buffer *buffer;
            UINT stride, offset;
            BufferStateDescriptor() : buffer(nullptr), stride(0), offset(0) {
            }
        };

        PERSISTENT_DECL(Grafkit::Mesh, 1);

    protected:
        void Serialize(Archive& ar) override { _Serialize(ar); }
        void _Serialize(Archive& ar);

    private:
        ID3D11Buffer * m_indexBuffer;
        BufferStateDescriptor m_buffer;
        size_t m_vertexCount, m_indexCount;
        UINT *m_indices;

        std::map<std::string, const void*> m_mapPtr;

        struct vertex_pointer_t {
            std::string name;
            uint32_t length;
            uint8_t * data;
        };

        std::list<vertex_pointer_t> m_vertexPointers;
    };

    typedef Ref<Mesh> MeshRef;

}

// --- excpetions 
DEFINE_EXCEPTION(CreateVertexBufferException, 1301, "Could not create vertex buffer")
DEFINE_EXCEPTION(CreateIndevBufferException, 1302, "Could not create index buffer")

#endif
