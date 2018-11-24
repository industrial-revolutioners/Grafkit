#include "stdafx.h"

#include <gtest/gtest.h>

#include "utils/asset/AssetFile.h"
#include "scene/scene.h"

#include "core/system.h"

#include "render/material.h"
#include "render/texture.h""

#include "resource/loaders/TextureLoader.h"

#include "application.h" 

// --- 

using namespace Grafkit;

#define ASSET_ROOT "tests/assets/"
#define TEXTURE_ROOT "textures/"

class TextureLoadTest : public testing::Test
{
public:
    TextureLoadTest() : m_app(nullptr)
    {
        m_assetFactory = new FileAssetFactory(ASSET_ROOT);
        m_app = new Testing::TestApplicationContext(m_render, m_assetFactory);
    }

    ~TextureLoadTest()
    {
        delete m_assetFactory;
        delete m_app;
    }

    void SetUp() override { 
    }

    void TearDown() override
    {
    }

protected:
    Testing::TestApplicationContext* m_app;

    Renderer m_render;
    IAssetFactory* m_assetFactory;
};

//
#define TEXTURE_2D_UUID "54484011-5c9e-4479-81f4-937888131e60"
#define TEXTURE_NAME "normap.jpg"
#define TEXTURE_PATH TEXTURE_ROOT TEXTURE_NAME 

TEST_F(TextureLoadTest, Load2DTexture)
{
    // given
    TextureBitmapParams params = { TEXTURE_PATH };
    Texture2DResRef resource = m_app->Load<Texture2DRes>(new TextureFromBitmap("texture", TEXTURE_2D_UUID, params));

    ASSERT_TRUE(resource);
    ASSERT_FALSE(*resource);

    // when
    m_app->DoPrecalc();

    // then
    ASSERT_TRUE(resource);
    ASSERT_TRUE(*resource);

    ASSERT_STREQ(TEXTURE_2D_UUID, resource->GetUuid().c_str());

    ASSERT_NE(nullptr, (*resource)->GetTexture2D());
    ASSERT_NE(nullptr, (*resource)->GetTextureResource());
    ASSERT_NE(nullptr, (*resource)->GetShaderResourceView());
}

namespace
{
    const char *cubemapNames[] = {
        "yoko_posx.jpg",
        "yoko_negx.jpg",
        "yoko_posy.jpg",
        "yoko_negy.jpg",
        "yoko_posz.jpg",
        "yoko_negz.jpg",
    };
}

#define TEXTURE_CUBE_UUID "af6404fa-a4f7-4b01-8c8b-1218faf6d35c"
//
TEST_F(TextureLoadTest, LoadCubeMap)
{
    // given
    TextureCubemapParams params;
    std::transform(cubemapNames, cubemapNames + 6, std::back_inserter(params.sourceNames), [](std::string s)->std::string {return std::string(TEXTURE_ROOT) + s; });

    TextureCubeResRef resource = m_app->Load<TextureCubeRes>(new TextureCubemapFromBitmap("texture", TEXTURE_CUBE_UUID, params));

    ASSERT_TRUE(resource);
    ASSERT_FALSE(*resource);

    // when
    m_app->DoPrecalc();

    // then
    ASSERT_TRUE(resource);
    ASSERT_TRUE(*resource);

    ASSERT_STREQ(TEXTURE_CUBE_UUID, resource->GetUuid().c_str());

    ASSERT_NE(nullptr, (*resource)->GetTexture2D());
    ASSERT_NE(nullptr, (*resource)->GetTextureResource());
    ASSERT_NE(nullptr, (*resource)->GetShaderResourceView());
}

#define TEXTURE_NOISE_UUID "91bde168-b40e-4c81-b974-2977014e5460"

//
TEST_F(TextureLoadTest, LoadNoiseMap)
{
    // given
    TextureNoiseParams params{ 256 };
    Texture2DResRef resource = m_app->Load<Texture2DRes>(new TextureNoiseMapBuilder("noiseMap", TEXTURE_NOISE_UUID, params));

    ASSERT_TRUE(resource);
    ASSERT_FALSE(*resource);

    // when
    m_app->DoPrecalc();

    // then
    ASSERT_TRUE(resource);
    ASSERT_TRUE(*resource);

    ASSERT_STREQ(TEXTURE_NOISE_UUID, resource->GetUuid().c_str());

    ASSERT_NE(nullptr, (*resource)->GetTexture2D());
    ASSERT_NE(nullptr, (*resource)->GetTextureResource());
    ASSERT_NE(nullptr, (*resource)->GetShaderResourceView());
}