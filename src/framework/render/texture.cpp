#include "stdafx.h"

#include "Texture.h"

using namespace std;
using namespace Grafkit;
using namespace Grafkit;

using namespace FWdebugExceptions;

//using Grafkit::Matrix;

using namespace DirectX;
using namespace Grafkit;

//#define USE_SRGB_TEXTURE

namespace
{
    DXGI_FORMAT bitmapformats[] = {
        DXGI_FORMAT_UNKNOWN,
        DXGI_FORMAT_R8_UNORM,
        DXGI_FORMAT_R8G8_UNORM,
#ifdef USE_SRGB_TEXTURE
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
#else
        DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_FORMAT_R8G8B8A8_UNORM,
#endif
    };
}

// ========================================================================================================================
// a bit of badly hacked thing, might need to clean it up later on
// but for a while, it does well. 
// ========================================================================================================================

ATexture::ATexture() :
    m_pTexture(nullptr)
    , width(0)
    , height(0)
    , //m_mipSlices(0), m_mipLevels(0), 
    depth(0)
    , channels(0)
    , channelWidth(0)
    , m_pResourceView(nullptr)
    , m_pTargetView(nullptr) {
}

ATexture::~ATexture() { Shutdown(); }

void ATexture::Shutdown()
{
    if (m_pResourceView)
    {
        m_pResourceView->Release();
        m_pResourceView = nullptr;
    }

    if (m_pTargetView)
    {
        m_pTargetView->Release();
        m_pTargetView = nullptr;
    }

    if (m_pTexture)
    {
        m_pTexture->Release();
        m_pTexture = nullptr;
    }
}

void ATexture::UpdateTexture(Renderer& device, const void* data, size_t len, size_t subRes, size_t offset)
{
    if (!len || !data)
        return;

    D3D11_MAPPED_SUBRESOURCE res;
    device.GetDeviceContext()->Map(m_pTexture.Get(), subRes, D3D11_MAP_WRITE_DISCARD, NULL, &res);
    CopyMemory(static_cast<unsigned char*>(res.pData) + offset, data, len);
    device.GetDeviceContext()->Unmap(m_pTexture.Get(), subRes);
}

void ATexture::CopyChannel3To4(const void* in, void* out, size_t w, size_t h)
{
    uint8_t* dst = static_cast<uint8_t*>(out);
    const uint8_t* src = static_cast<const uint8_t*>(in);

    size_t x = w;
    size_t y = h;

    size_t stride_src = x * 3;
    size_t stride_dst = x * 4;

    while (y--)
    {
        while (x--)
        {
            dst[4 * x + 0] = src[3 * x + 0];
            dst[4 * x + 1] = src[3 * x + 1];
            dst[4 * x + 2] = src[3 * x + 2];
            dst[4 * x + 3] = 0xff;
        }
        src += stride_src;
        dst += stride_dst;
        x = w;
    }
}

void ATexture::Update(Renderer& device, const void* bitmap, size_t index)
{
    size_t len = 0;
    switch (GetDimension())
    {
    case 1:
        len = channels * channelWidth * width;
        break;
    case 2:
        len = channels * channelWidth * width * height;
        break;
    case 3:
        len = channels * channelWidth * width * height * depth;
        break;
    default:
        return;
    }

    UpdateTexture(device, bitmap, len, index);
}

void ATexture::Update(Renderer& device, const BitmapRef bitmap, size_t index)
{
    UINT w = bitmap->GetX(), h = bitmap->GetY(), ch = bitmap->GetCh();
    uint8_t* data = static_cast<uint8_t*>(bitmap->GetData());

    if (ch == 3)
    {
        D3D11_MAPPED_SUBRESOURCE subRes;
        subRes.pData = nullptr;
        subRes.DepthPitch = 0;
        subRes.RowPitch = 0;
        device.GetDeviceContext()->Map(m_pTexture.Get(), index, D3D11_MAP_WRITE_DISCARD, NULL, &subRes);

        CopyChannel3To4(data, subRes.pData, w, h);

        device.GetDeviceContext()->Unmap(m_pTexture.Get(), NULL);
    }
    else { UpdateTexture(device, data, w * h * ch * channelWidth, index); }
}

// ========================================================================================================================

void Texture1D::Initialize(Renderer& device, size_t w, size_t components)
{
    this->SetDimm(w, 0, 0, components, 4);

    DXGI_FORMAT fmt = DXGI_FORMAT_R32_FLOAT;
    switch (channels)
    {
    case 4:
    case 3:
        channels = 4;
        fmt = DXGI_FORMAT_R32G32B32A32_FLOAT;
        break;
    case 2:
        fmt = DXGI_FORMAT_R32G32_FLOAT;
        break;
    case 1:
    default:
        break;
    }

    this->CreateTexture(device, fmt, w, nullptr);
}

void Texture1D::Initialize(Renderer& device, size_t w, const float* data)
{
    this->SetDimm(w, 0, 0, 1, 4);
    this->CreateTexture(device, DXGI_FORMAT_R32_FLOAT, w, data);
}

void Texture1D::Initialize(Renderer& device, size_t w, const float4* data)
{
    this->SetDimm(w, 0, 0, 4, 4);
    this->CreateTexture(device, DXGI_FORMAT_R32G32B32A32_FLOAT, w, data);
}

void Texture1D::CreateTexture(Renderer& device, DXGI_FORMAT format, size_t w, const void* initaldata)
{
    HRESULT result = 0;

    D3D11_SRV_DIMENSION srvDimension = D3D11_SRV_DIMENSION_TEXTURE1D;

    // ... 
    D3D11_TEXTURE1D_DESC textureDesc;
    ZeroMemory(&textureDesc, sizeof(textureDesc));
    textureDesc.Width = width;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = format;
    textureDesc.Usage = D3D11_USAGE_DYNAMIC;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    textureDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA *pData = nullptr, subresData;
    // initialdata
    if (initaldata)
    {
        // ... 
        subresData.pSysMem = initaldata;
        subresData.SysMemPitch = width * channelWidth;
        subresData.SysMemSlicePitch = 0;
        pData = &subresData;
    }

    ID3D11Texture1D* ppTex = nullptr;
    result = device->CreateTexture1D(&textureDesc, pData, &ppTex);

    if (FAILED(result))
    THROW_EX_HRESULT(TextureCreateException, result);

    m_pTexture = ppTex;

    // SRV

    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
    ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    shaderResourceViewDesc.Format = format;
    shaderResourceViewDesc.ViewDimension = srvDimension;

    shaderResourceViewDesc.Texture1D.MostDetailedMip = 0;
    shaderResourceViewDesc.Texture1D.MipLevels = -1;

    result = device->CreateShaderResourceView(m_pTexture.Get(), &shaderResourceViewDesc, &m_pResourceView);

    if (FAILED(result))
    THROW_EX_HRESULT(ShaderResourceViewException, result);
}

// ========================================================================================================================

void Texture2D::Initialize(Renderer& device, BitmapRef bitmap)
{
    int ch = bitmap->GetCh();
    int chw = 1;

    const DXGI_FORMAT fmt = bitmapformats[ch % 5];

    void* data = bitmap->GetData();

    CreateTextureBitmap(device, fmt, ch, bitmap->GetX(), bitmap->GetY(), data);
}

void Texture2D::Initialize(Renderer& device, int w, int h)
{
#ifdef USE_SRGB_TEXTURE
    const DXGI_FORMAT  fmt = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
#else // USE_SRGB_TEXTURE
    const DXGI_FORMAT fmt = DXGI_FORMAT_R8G8B8A8_UNORM;
#endif // USE_SRGB_TEXTURE

    CreateTextureTarget(device, fmt, 4, 1, w, h);
}

void Texture2D::InitializeFloatRGBA(Renderer& device, int w, int h) { CreateTextureTarget(device, DXGI_FORMAT_R32G32B32A32_FLOAT, 4, 4, w, h); }

void Texture2D::InitializeFloat(Renderer& device, int w, int h) { CreateTextureTarget(device, DXGI_FORMAT_R32_FLOAT, 4, 1, w, h); }

void Texture2D::InitializeDepth(Renderer& device, int w, int h) { CreateTextureTarget(device, DXGI_FORMAT_D32_FLOAT, 4, 1, w, h); }

void Texture2D::CreateTextureBitmap(Renderer& device, DXGI_FORMAT format, int channels, int w, int h, void* initialData)
{
    HRESULT result;

    this->SetDimm(w, h, 0, channels, 1);

    if (!width && !height)
    {
        int sw = 0, sh = 0;
        device.GetScreenSize(sw, sh);
        width = sw , height = sh;
    }
    else if (!width || !height || false
        //width != height
    )
    {
        DebugBreak();
        return;
    }

    int miplevels = floor(log2f(width));

    D3D11_TEXTURE2D_DESC textureDesc;
    ZeroMemory(&textureDesc, sizeof(textureDesc));
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = miplevels;
    textureDesc.ArraySize = 1;
    textureDesc.Format = format;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

    D3D11_SUBRESOURCE_DATA pData[16];

    // Initial data
    if (initialData)
    {
        pData[0].pSysMem = initialData;
        pData[0].SysMemSlicePitch = 0;
        if (channels == 3)
        {
            channels = 4;
            unsigned char* data = new unsigned char[width * height * channels * channelWidth];
            CopyChannel3To4(initialData, data, width, height);
            pData[0].pSysMem = data;
        }
        pData[0].SysMemPitch = width * channels;

        size_t s = width * height * channels * channelWidth;
        size_t ww = width;

        for (int i = 1; i < miplevels; i++)
        {
            s = s / 2;
            ww = ww / 2;
            unsigned char* data = new unsigned char[s];
            pData[i].pSysMem = data;
            pData[i].SysMemPitch = ww * channelWidth;
            pData[i].SysMemSlicePitch = 0;
        }
    }
    else { DebugBreak(); }

    ID3D11Texture2D* ppTex = nullptr;
    result = device->CreateTexture2D(&textureDesc, pData, &ppTex);

    if (FAILED(result)) THROW_EX(TextureCreateException);

    m_pTexture = ppTex;

    // SRV, RTV
    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
    ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    shaderResourceViewDesc.Format = format;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
    shaderResourceViewDesc.Texture2D.MipLevels = -1;

    result = device->CreateShaderResourceView(m_pTexture.Get(), &shaderResourceViewDesc, &m_pResourceView);

    if (FAILED(result))
    THROW_EX_HRESULT(ShaderResourceViewException, result);

    device.GetDeviceContext()->GenerateMips(m_pResourceView.Get());

    //if (pData[0].pSysMem != initialData)
    //	delete pData[0].pSysMem;
}

void Texture2D::CreateTextureTarget(Renderer& device, const DXGI_FORMAT format, const int channels, const int channelWidth, const int w, const int h)
{
    HRESULT result;

    this->SetDimm(w, h, 0, channels, 1);

    if (!width && !height)
    {
        int sw = 0, sh = 0;
        device.GetScreenSize(sw, sh);
        width = sw , height = sh;
    }
    else
    if (!width || !height) { return; }

    D3D11_SRV_DIMENSION srvDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    D3D11_RTV_DIMENSION rtvDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

    D3D11_TEXTURE2D_DESC textureDesc;
    ZeroMemory(&textureDesc, sizeof(textureDesc));
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = format;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA *pData = nullptr, subresData;

    ID3D11Texture2D* ppTex = nullptr;
    result = device->CreateTexture2D(&textureDesc, nullptr, &ppTex);

    if (FAILED(result))
    THROW_EX_HRESULT(TextureCreateException, result);

    m_pTexture = ppTex;

    // SRV
    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
    ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
    shaderResourceViewDesc.Format = format;
    shaderResourceViewDesc.ViewDimension = srvDimension;

    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
    shaderResourceViewDesc.Texture2D.MipLevels = -1;

    result = device->CreateShaderResourceView(m_pTexture.Get(), &shaderResourceViewDesc, &m_pResourceView);

    if (FAILED(result))
    THROW_EX_HRESULT(ShaderResourceViewException, result);

    // RTV
    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
    renderTargetViewDesc.Format = format;
    renderTargetViewDesc.ViewDimension = rtvDimension;

    renderTargetViewDesc.Texture2D.MipSlice = 0;

    result = device->CreateRenderTargetView(m_pTexture.Get(), &renderTargetViewDesc, &m_pTargetView);

    if (FAILED(result))
    THROW_EX(RenderTargetViewException);
}


// ========================================================================================================================

void TextureCube::Initialize(Renderer& device, CubemapRef cubemap)
{
    HRESULT result;

    BitmapRef bitmap = cubemap->GetPosX(); //assume that everything is equal there
    int ch = bitmap->GetCh();
    DXGI_FORMAT format = bitmapformats[ch % 5];

    this->SetDimm(cubemap->GetPosX()->GetX(), cubemap->GetPosX()->GetY(), 0, (channels == 3) ? 4 : ch, 1);

    D3D11_TEXTURE2D_DESC textureDesc;
    ZeroMemory(&textureDesc, sizeof(textureDesc));
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 6;
    textureDesc.Format = format;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE | D3D11_RESOURCE_MISC_GENERATE_MIPS;

    // --- setup inital data 
    D3D11_SUBRESOURCE_DATA pData[6];

    for (int i = 0; i < 6; i++)
    {
        void* inData = cubemap->GetBitmap(i)->GetData();
        void* outData = inData;
        if (cubemap->GetBitmap(i)->GetCh() == 3)
        {
            outData = new unsigned int[width * height];
            CopyChannel3To4(inData, outData, width, height);
        }
        pData[i].pSysMem = outData;
        pData[i].SysMemPitch = width * 4;
        pData[i].SysMemSlicePitch = 0;
    }

    ID3D11Texture2D* ppTex = nullptr;
    result = device->CreateTexture2D(&textureDesc, pData, &ppTex);

    // cleanup converted data 
    for (int i = 0; i < 6; i++)
        if (pData[i].pSysMem != cubemap->GetBitmap(i)->GetData())
            delete pData[i].pSysMem;

    if (FAILED(result))
    THROW_EX_HRESULT(TextureCreateException, result);

    m_pTexture = ppTex;

    // --- srv
    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
    ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    shaderResourceViewDesc.Format = format;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;

    shaderResourceViewDesc.TextureCube.MostDetailedMip = 0;
    shaderResourceViewDesc.TextureCube.MipLevels = -1;

    result = device->CreateShaderResourceView(m_pTexture.Get(), &shaderResourceViewDesc, &m_pResourceView);

    if (FAILED(result)) THROW_EX_HRESULT(TextureCreateException, result);

    device.GetDeviceContext()->GenerateMips(m_pResourceView.Get());
}

// ========================================================================================================================

TextureSampler::TextureSampler()
    : m_pSamplerState(nullptr) {
}

TextureSampler::~TextureSampler() { this->Shutdown(); }


void TextureSampler::Initialize(Renderer& device, D3D11_TEXTURE_ADDRESS_MODE mode)
//void Grafkit::TextureSampler::Initialize(Renderer & device)
{
    HRESULT result = 0;

    // Create a texture sampler state description.
    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory(&samplerDesc, sizeof(samplerDesc));

    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    //samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;

    samplerDesc.AddressU = mode;
    samplerDesc.AddressV = mode;
    samplerDesc.AddressW = mode;

    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    // Create the texture sampler state.
    result = device->CreateSamplerState(&samplerDesc, &m_pSamplerState);
    if (FAILED(result))
    {
        THROW_EX(SamplerStateCreateException);
    }
}

void TextureSampler::Shutdown()
{
    if (m_pSamplerState)
    {
        m_pSamplerState->Release();
        m_pSamplerState = nullptr;
    }
}
