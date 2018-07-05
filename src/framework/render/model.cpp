#include "stdafx.h"

#include "mesh.h"
#include "Material.h"
#include "shader.h"
#include "SceneGraph.h"

#include "Actor.h"

#include "model.h"


using namespace Grafkit;

// ==================================================================
PERSISTENT_IMPL(Grafkit::Model);

void Model::_serialize(Archive& ar)
{
    this->Entity3D::_serialize(ar);
    PERSIST_REFOBJECT(ar, m_mesh);
}

Model::Model() {
}

Model::Model(MeshRef mesh) :
    m_mesh(mesh) {
}

Model::Model(MeshRef mesh, MaterialRef material) :
    m_material(material)
    , m_mesh(mesh) {
}

Model::~Model() {
}

MeshRef Model::GetMesh() const { return this->m_mesh; }

void Model::SetMesh(MeshRef model) { this->m_mesh = model; }

MaterialRef Model::GetMaterial() const { return this->m_material; }

void Model::SetMaterial(MaterialRef material) { this->m_material = material; }

// ==================================================================

void Model::Render(Renderer& render, SceneGraph* const & scene)
{
    if (m_material)
    {
        // bullshit, does not work
        //if (m_material->GetLayer() != scene->GetLayerID())
        //return;
        this->m_material->Render(render, scene->GetVShader(), scene->GetPShader());
    }

    if (m_geometryShader.Valid() && m_geometryShader->Valid())
        m_geometryShader->Get()->Bind(render);

    if (m_mesh)
        this->m_mesh->RenderMesh(render);

    //if (m_geometryShader.Valid() && m_geometryShader->Valid())
    //	m_geometryShader->Get()->Bind(render);
}

void Model::Build(Renderer& deviceContext, SceneGraph* const & scene)
{
    if (m_mesh.Valid())
        m_mesh->Build(deviceContext, scene->GetVShader());
}

void Model::SetGeometryShader(ShaderResRef shader) { m_geometryShader = shader; }

ShaderResRef Model::GetGeometryShader() const { return m_geometryShader; }
