#include "../render/renderer.h"
#include "../render/texture.h"
#include "../render/Material.h"
#include "../render/model.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/material.h"
#include "assimp/mesh.h"
#include "assimp/matrix4x4.h"

#include "../math/matrix.h"

#include "assimploader.h"

using namespace FWrender;
using namespace FWdebugExceptions;

// ================================================================================================================================================================
// Assimp helpers
// ================================================================================================================================================================

// ezeket mashogy nem lehet megoldnai, mert a matkey is egy makro
#define assimpMaterialKey_2_float4(SRC, _AI_ENUM, OUT)\
{\
	aiColor3D ac; float4& fv = OUT; \
	SRC->Get(_AI_ENUM, ac); \
	fv.x = ac.r, fv.y = ac.g, fv.z = ac.b, fv.w = 1.0; \
}

#define assimpMaterialKey_2_float(SRC, _AI_ENUM, OUT) \
{\
	float &scalar = OUT;\
	SRC->Get(_AI_ENUM, scalar);\
}

FWmath::Matrix ai4x4MatrixToFWMatrix(void* _m)
{
	if (!_m) throw EX(NullPointerException);
	aiMatrix4x4 *m = (aiMatrix4x4 *)_m;
	return FWmath::Matrix(
		m->a1, m->a2, m->a3, m->a4,
		m->b1, m->b2, m->b3, m->b4,
		m->c1, m->c2, m->c3, m->c4,
		m->d1, m->d2, m->d3, m->d4
		);
}

//TextureRef assimpTexture(enum aiTextureType what, aiMaterial* material, int subnode, enum texture_type_e targetType) 
TextureRef assimpTexture(enum aiTextureType what, aiMaterial* material, int subnode, int targetType)
{
	aiString path;
	TextureRef texture = NULL;
	aiReturn result = material->GetTexture(what, subnode, &path);

	if (result == AI_SUCCESS && path.data[0]) {
		texture = new Texture();
		texture->SetName(path.C_Str());

		// texture lookup

		// ide jon a textura generator, vagy lookup, attol fuggoen, hogy mink van eppen
		// ++ a textura slotot is erdemes lenne berakni a helyere (targetType)

		/*
		texture->setMinFilter(FWrender::TF_LINEAR_MIPMAP_LINEAR);
		texture->setMagFilter(FWrender::TF_LINEAR);
		texture->setIsGenerateMipmap(1);
		texture->setIsUseAnisotropic(1);

		texture->setTextureType(targetType);
		*/

		// LOAD_TEXTURE(texture);
	}

	return texture;
}

// ================================================================================================================================================================
// Head
// ================================================================================================================================================================

FWmodel::AssimpLoader::AssimpLoader(FWassets::IResourceRef resource, FWassets::IRenderAssetManager * const & assman, FWrender::Scenegraph * const & scenegraph) : 
	IRenderAssetGenerator(assman), 
	m_scenegraph(scenegraph), m_resource(resource)
{

}

FWmodel::AssimpLoader::~AssimpLoader()
{
}

// ================================================================================================================================================================
// It does the trick
// ================================================================================================================================================================
void FWmodel::AssimpLoader::operator()()
{
	Assimp::Importer importer;
	const aiScene *scene = (importer.ReadFileFromMemory(this->m_resource->GetData(), this->m_resource->GetSize(), aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_GenNormals));

	if (!scene) {
		throw EX_DETAILS(AssimpParseException, importer.GetErrorString());
	}

	int i = 0, j = 0, k = 0, l = 0;

	// build texture -- material LUT
	struct matrerial_texture_t {
		MaterialRef material;
		std::vector<TextureRef> textures;
	};
	std::vector< struct matrerial_texture_t> matrerial_texture_table;

	// -- load materials
	if (scene->HasMaterials()) {
		for (i = 0; i<scene->mNumMaterials; i++) {
			matrerial_texture_t mtable_elem;

			aiString path, name;

			MaterialRef material = new MaterialWrapper();
			aiMaterial *curr_mat = scene->mMaterials[i];

			this->m_assman->AddObject(material.Get());

			if (curr_mat->Get(AI_MATKEY_NAME, name) == AI_SUCCESS) material->SetName(name.C_Str());

			j = 0;
			aiReturn texFound = AI_FAILURE;

			mtable_elem.material = material;
			std::vector<TextureRef> &texture_table = mtable_elem.textures;

			// -- -- load textures
			// textura-> material 
			do
			{
				/// @todo ezt egy for ciklussal is meg lehetne oldani majd, sot. 
				TextureRef diffuse = assimpTexture(aiTextureType_DIFFUSE, scene->mMaterials[i], j,   0 /*TextureMapType_map0    */);	///@todo ezeket a slotok miatt hagytam meg
				TextureRef normal = assimpTexture(aiTextureType_NORMALS, scene->mMaterials[i], j,    0 /*TextureMapType_normal  */);
				TextureRef specular = assimpTexture(aiTextureType_SPECULAR, scene->mMaterials[i], j, 0 /*TextureMapType_specular*/);

				texFound = AI_FAILURE;

				if (diffuse) {
					texture_table.push_back(diffuse);
					texFound = AI_SUCCESS;
				}

				if (specular) {
					texture_table.push_back(specular);
					texFound = AI_SUCCESS;
				}

				if (normal) {
					texture_table.push_back(normal);
					texFound = AI_SUCCESS;
				}
				j++;
			} while (texFound == AI_SUCCESS);

			assimpMaterialKey_2_float4(curr_mat, AI_MATKEY_COLOR_DIFFUSE, material->GetDiffuse());
			assimpMaterialKey_2_float4(curr_mat, AI_MATKEY_COLOR_AMBIENT, material->GetAmbient());
			assimpMaterialKey_2_float4(curr_mat, AI_MATKEY_COLOR_SPECULAR, material->GetSpecular());
			
			assimpMaterialKey_2_float(curr_mat, AI_MATKEY_SHININESS, material->GetShininess());
			assimpMaterialKey_2_float(curr_mat, AI_MATKEY_SHININESS_STRENGTH, material->GetSpecularLevel());

			matrerial_texture_table.push_back(mtable_elem);
			
		}
	}
	

	// scene loading
	if (scene->HasMeshes()) {
		for (i = 0; i<scene->mNumMeshes; i++) {
			// -- meshes
			
			Model *model = new Model();
			aiMesh* curr_mesh = scene->mMeshes[i];

			aiString name;
			const char* mesh_name = curr_mesh->mName.C_Str(); //for dbg purposes
			///@todo model->SetName(mesh_name);

			///@todo 
			//model->pushVert(reinterpret_cast<vec3float*>(curr_mesh->mVertices), curr_mesh->mNumVertices);
			//model->pushTextureUV(reinterpret_cast<vec3float*>(curr_mesh->mTextureCoords[0]), curr_mesh->mNumVertices);	///@todo tobbfele texuv is lehet
			//model->pushNormals(reinterpret_cast<vec3float*>(curr_mesh->mNormals), curr_mesh->mNumVertices);
			//model->pushTangents(reinterpret_cast<vec3float*>(curr_mesh->mTangents), curr_mesh->mNumVertices);	//ha ilyen is van, akkor eljunk vele

			// -- faces
			j = curr_mesh->mNumFaces;

			while (j--) {
				aiFace *curr_face = &curr_mesh->mFaces[j];
				///*offset = */ model->pushMesh(curr_face->mIndices, curr_face->mNumIndices /*, offset */);
			}

			// -- luukup materials
			int mat_index = curr_mesh->mMaterialIndex;
			if (materials.size()>mat_index)
			{
				//Material *mat = materials[mat_index];
				model->setMaterial(material_table[mat_index].material);
				if (material_table[mat_index].textures) {
					k = material_table[mat_index].textures->size();
					while (k--) {
						model->SetTexture(k, (*material_table[mat_index].textures)[k]);
					}
					delete material_table[mat_index].textures;
					material_table[mat_index].textures = NULL;
				}
			}

			//FWmath::Matrix modelview = model->getModelviewMatrix();
			//this->models.push(model);
		}
	}

	// load cameras

	// load lights

	// build scenegraph
}

