#pragma once
#include "utils/ResourcePreloader.h"
#include "utils/InitializeSerializer.h"
#include "schema.h"

namespace GkDemo
{
    class SchemaBuilder;
    class Demo;

    class Context : public Grafkit::ResourcePreloader, Grafkit::ClonableInitializer
    {
    public:

        explicit Context(Grafkit::Renderer &render, Grafkit::IAssetFactory* assetFactory);
        ~Context() override;

        void Relocate(std::string path);
        
        void SaveSchema();
        void LoadScema();

        void Intitialize();

        Demo* GetDemo() const { return m_demo; }
        void SetDemo(Demo* const demo) { m_demo = demo; }

        Grafkit::Renderer& GetDeviceContext() override { return m_render; }
        Grafkit::IAssetFactory* GetAssetFactory() override { return m_assetFactory; }
        void CreateTestStuff();

    protected:
        Grafkit::Renderer& m_render;
        
        Demo* m_demo;

        Grafkit::IAssetFactory* m_assetFactory;

        SchemaBuilder m_builder;
        //SchemaExport m_exporter;

    };


}
