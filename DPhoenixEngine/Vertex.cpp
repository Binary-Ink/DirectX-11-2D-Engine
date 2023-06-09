#include "Vertex.h"
#include "Effect.h"

//this is where we define the input layouts for the shader
#pragma region InputLayoutDesc

const D3D11_INPUT_ELEMENT_DESC DPhoenix::InputLayoutDesc::SpritePosTex[2] =
{
	//the first value (FLOAT3) is for the vertex POSITION
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//the second value (FLOAT2) is for the vrtex TEXCOORD
	//is 12 as that is the byte size for the previous input element
	//32 / 8 = 4 | 4 * 3 = 12   
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

//initialisation of input layouts
#pragma region InputLayouts

const D3D11_INPUT_ELEMENT_DESC DPhoenix::InputLayoutDesc::PosColor[2] = {
	//first value for float3 is vertex position
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 
	//second value float 4 is vertex color 
	//(12 = byte size for previous element 
	{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

//set input layouts to zero before initialisation
ID3D11InputLayout* DPhoenix::InputLayouts::SpritePosTex = 0;
ID3D11InputLayout* DPhoenix::InputLayouts::PosColor = 0; 

//initialise all input layouts
void DPhoenix::InputLayouts::InitAll(ID3D11Device* device)
{
	//shader 'pass' description truct
	D3DX11_PASS_DESC passDesc;

	//
	// SpritePosTex
	//
	//get the first pass and put in the pass description struct
	Effects::SpriteFX->SpriteTech->GetPassByIndex(0)->GetDesc(&passDesc);
	//create the input layout
	HR(device->CreateInputLayout(
		InputLayoutDesc::SpritePosTex,		//the desc for the input layout
		2,									//number of input data types
		passDesc.pIAInputSignature,			//pointer to the compiled shader
		passDesc.IAInputSignatureSize,		//size of compiled shader
		&SpritePosTex));					//output, pointer to input layout object

	//
	// PosColor
	//
	// get the first pass and put in the pass description strut 
	Effects::ColorFX->ColorTech->GetPassByIndex(0)->GetDesc(&passDesc);
	//create the input layout
	HR(device->CreateInputLayout(
		InputLayoutDesc::PosColor,		//desc for input layout
		2,								//number of input data types
		passDesc.pIAInputSignature,	//pointer to compiled shader
		passDesc.IAInputSignatureSize,	//size of compiled shader 
		&PosColor));				//output, pointer to input layer obj 
}

//release memory of input layouts
void DPhoenix::InputLayouts::DestroyAll()
{
	ReleaseCOM(SpritePosTex);
	ReleaseCOM(PosColor); 

}

#pragma endregion

