
#include <fstream>

#include "shader.h"

#include "../math/matrix.h"

using namespace std;
using namespace FWrender;

using namespace FWdebugExceptions;

using FWmath::Matrix;

// =============================================================================================================================

Shader::Shader() : 
	m_pShader(NULL),
	m_vShader(NULL)
{
}

FWrender::Shader::~Shader()
{
	this->Shutdown();
}

//void FWrender::Shader::LoadFromFile(ID3D11Device * device, LPCSTR vsEntry, LPCSTR fsEntry, LPCWCHAR vsFile, LPCWCHAR fsFile)
void Shader::LoadFromFile(ID3D11Device* device, LPCSTR entry, LPCWCHAR file, ShaderType_e type)
{
	HRESULT result;
	ID3D10Blob* errorMessage;

	ID3D10Blob* shaderBuffer;

	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	// input checking
	if (!entry) throw new EX(NullPointerException);
	if (!file) throw new EX(NullPointerException);


	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	shaderBuffer = 0;

	// Compile the vertex shader code.
	result = D3DCompileFromFile(file, NULL, NULL, entry, "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &shaderBuffer, &errorMessage);

	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			DispatchShaderErrorMessage(errorMessage, file, entry);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			throw new EX(MissingShaderExcepotion);
		}
	}

	// Compile the pixel shader code.

	//LPCWCHAR _fsFile = fsFile ? fsFile : vsFile;
	//result = D3DCompileFromFile(_fsFile, NULL, NULL, fsEntry, "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);

	//if (FAILED(result))
	//{
	//	// If the shader failed to compile it should have writen something to the error message.
	//	if (errorMessage)
	//	{
	//		DispatchShaderErrorMessage(errorMessage, _fsFile, fsEntry);
	//	}
	//	// If there was nothing in the error message then it simply could not find the shader file itself.
	//	else
	//	{
	//		throw new EX(MissingShaderExcepotion);
	//	}
	//}


	if (type == ST_Vertex) {
		// Create the vertex shader from the buffer.
		result = device->CreateVertexShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, &m_vShader);
		if (FAILED(result))
		{
			throw new EX(VSCrerateException);
		}
	}
	else if (type == ST_Pixel){
		// Create the pixel shader from the buffer.
		result = device->CreatePixelShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, &m_pShader);
		if (FAILED(result))
		{
			throw new EX(FSCrerateException);
		}
	}

	// create reflection interface 
	result = D3DReflect(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&m_pReflector);
	if (FAILED(result)) {
		/// @todo throw exception
	}

	this->BuildReflection(device);

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	shaderBuffer->Release();
	shaderBuffer = NULL;

	// --- ezek mennenek a meshbe, vagy a mesh factoryba 
#if 0
	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the Model and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		throw new EX(InputLayoutCreateException);
	}
#endif
	// --- egeszen eddig ---

	// --- ezek mennenek a parameter managerbe ---
#if 0
	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		throw new EX(ConstantBufferCreateException);
	}
#endif
	// --- egeszen eddig 
}

void FWrender::Shader::Shutdown()
{
	// why is this empty 
}

void FWrender::Shader::Render(ID3D11DeviceContext * deviceContext)
{
}

// =============================================================================================================================

//void FWrender::Shader::SetCameraMatrices(ID3D11DeviceContext * deviceContext, FWmath::Matrix & mat_projection, FWmath::Matrix & mat_world, FWmath::Matrix & mat_view)
//{
//# if 0
//	HRESULT result;
//	D3D11_MAPPED_SUBRESOURCE mappedResource;
//	MatrixBufferType* mMatrix;
//	unsigned int bufferNumber;
//
//	// Transpose the matrices to prepare them for the shader.
//	mat_projection.Transpose();
//	mat_world.Transpose();
//	mat_view.Transpose();
//
//	// Lock the constant buffer so it can be written to.
//	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//	if (FAILED(result))
//	{
//		throw new EX(ConstantBufferLocateException);
//	}
//
//	mMatrix = (MatrixBufferType*)mappedResource.pData;
//
//	mMatrix->world = mat_world;
//	mMatrix->view = mat_view;
//	mMatrix->projection = mat_projection;
//
//	deviceContext->Unmap(m_matrixBuffer, 0);
//	bufferNumber = 0;
//	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
//
//#endif
//}

//void FWrender::Shader::SetTexture(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView * texture)
//{
//}

// =============================================================================================================================

FWrender::Shader::ShaderVariable FWrender::Shader::operator[](const char * name)
{
	// fuckings 

	HRESULT result = 0;
	ID3D11ShaderReflectionConstantBuffer * pConstBuffer = this->m_pReflector->GetConstantBufferByName(name);
	D3D11_SHADER_BUFFER_DESC pDesc;
	// --- fuckings bele 
	//if (pConstBuffer->GetDesc(&pDesc) != S_OK)
	//	return FWrender::Shader::ShaderVariable(NULL);
	
	pConstBuffer->GetDesc(&pDesc);
	return FWrender::Shader::ShaderVariable(pDesc);
}

void FWrender::Shader::DispatchShaderErrorMessage(ID3D10Blob* errorMessage, LPCWCHAR file, LPCSTR entry)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	std::wstring error_string();

	compileErrors = (char*)(errorMessage->GetBufferPointer());
	bufferSize = errorMessage->GetBufferSize();
	fout.open("shader-error.txt");

	for (i = 0; i<bufferSize; i++)
	{
		if (compileErrors[i])
			// fuckings ebbe bele most keresztbe
			fout << compileErrors[i];
			// error_string += compileErrors[i];
	}

	fout.close();

	errorMessage->Release();
	errorMessage = 0;

	// @todo add compile errors text 
	throw new EX_DETAILS(ShaderException, L"See shader-error.txt");
}

void FWrender::Shader::BuildReflection(ID3D11Device* device)
{
	HRESULT result = 0;
	D3D11_SHADER_DESC desc;
	this->m_pReflector->GetDesc(&desc);

	// fetch input descriptors
	// input layouts? 
	for (size_t i = 0; i < desc.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC input_desc;
		this->m_pReflector->GetInputParameterDesc(i, &input_desc);

		// ... 
	}

	// fetch output desctiptor
	/// @todo tudumm 
	for (size_t i = 0; i < desc.OutputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC out_desc;
		this->m_pReflector->GetOutputParameterDesc(i, &out_desc);

		// ... 
	}

	// fetch constant buffers
	for (size_t i = 0; i < desc.ConstantBuffers; i++)
	{
		struct ConstantBufferLayout cbLayout;
		ID3D11ShaderReflectionConstantBuffer * pConstBuffer = this->m_pReflector->GetConstantBufferByIndex(i);
		result = pConstBuffer->GetDesc(&cbLayout.Description);

		for (size_t j = 0; j < cbLayout.Description.Variables; j++) 
		{
			// Get the variable description and store it
			ID3D11ShaderReflectionVariable* pVariable = pConstBuffer->GetVariableByIndex(j);
			D3D11_SHADER_VARIABLE_DESC var_desc;
			pVariable->GetDesc(&var_desc);
			cbLayout.Variables.push_back(var_desc);
			
			// Get the variable type description and store it
			ID3D11ShaderReflectionType* pType = pVariable->GetType();
			D3D11_SHADER_TYPE_DESC type_desc;
			pType->GetDesc(&type_desc);
			cbLayout.Types.push_back(type_desc);

			// create constant buffer for each variable
			ID3D11Buffer* buffer = NULL;
			D3D11_BUFFER_DESC bufferDesc;
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.ByteWidth = var_desc.Size;
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;

			result = device->CreateBuffer(&bufferDesc, NULL, &buffer);
			if (FAILED(result)) {
				throw EX(ConstantBufferCreateException);
			}

			cbLayout.Buffers.push_back(buffer);

		}

		this->m_constantBuffers.push_back(cbLayout);
	}
	
}

// =============================================================================================================================

void FWrender::Shader::ShaderVariable::operator=(float v)
{
	// ((float*)(((unsigned char*)&pFullscreenQuadConstants) + pDesc.StartOffset))[0] = v;
}
