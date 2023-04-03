#include "PrimitiveInstance.h"

//constructor - initialise members
DPhoenix::PrimitiveInstance::PrimitiveInstance() {
	//null pointers before use
	mMeshptr = NULL; 
	mPrimitiveVB = NULL; 
	mPrimitiveIB = NULL; 

	//default values for all properties (note scale must be 1, nothing below 1 will render)
	mPosition.x = 0.0f; mPosition.y = 0.0f; mPosition.z = 0.0f; 
	mVelocity.x = 0.0f; mVelocity.y = 0.0f; mVelocity.z = 0.0f; 
	mRelativeVelocity.x = 0.0f; mRelativeVelocity.y = 0.0f; mRelativeVelocity.z = 0.0f; 
	mScale.x = 1.0f; mScale.y = 1.0f; mScale.z = 1.0f; 
	mRotation.x = 0.0f; mRotation.y = 0.0f; mRotation.z = 0.0f; 
	mForwardVector.x = 0.0f; mForwardVector.y = 0.0f; mForwardVector.z = 1.0f; 
	mRightVector.x = 1.0f; mRightVector.y = 0.0f; mRightVector.z = 0.0f; 
};

//destructor - free memory
DPhoenix::PrimitiveInstance::~PrimitiveInstance() {
	//release index buffers
	ReleaseCOM(mPrimitiveVB);
	ReleaseCOM(mPrimitiveIB);
}; 

//update method, handle movement off velocity and time
void DPhoenix::PrimitiveInstance::Update(float deltaTime, bool moverelative) {

//if moving relative to objs forward vector 
	if (moverelative) {
		mPosition.x += mRelativeVelocity.x * mForwardVector.x *deltaTime; 
		mPosition.y += mRelativeVelocity.y * mForwardVector.y *deltaTime;
		mPosition.z += mRelativeVelocity.z * mForwardVector.z *deltaTime;
	}
	//otherwise movement is global in world space
	else {
		mPosition.x += mVelocity.x *deltaTime; 
		mPosition.y += mVelocity.y *deltaTime;
		mPosition.z += mVelocity.z *deltaTime;
	}
}; 

//load mesh instance with only vertex and color data
void DPhoenix::PrimitiveInstance::LoadColorInstance(ID3D11Device * md3dDevice, XMFLOAT4 color,
	GeometryGenerator::MeshData* meshPtr) {
	//assign pointer to meshdata
	mMeshptr = meshPtr; 

	//create new vector of vertices
	std::vector<Vertex::PosColor> vertices; 
	vertices.resize(mMeshptr->Vertices.size()); 

	//this loop extracts position data for each vertex and assigns to the new vector with color data

	for(size_t i=0; i< mMeshptr->Vertices.size(); ++i) {
		vertices[i].Pos = mMeshptr->Vertices[i].Position; 
		vertices[i].Color = color; 

	}

	//vertex buffer
	D3D11_BUFFER_DESC vbd;						//desc strut for vertex buffer
	vbd.Usage = D3D11_USAGE_IMMUTABLE;			//usage flags 
	vbd.ByteWidth = sizeof(Vertex::PosColor)	//multiply struct by num of vertices
		* mMeshptr->Vertices.size(); 
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//bind as vertex buffer
	vbd.CPUAccessFlags = 0;						//cpu flags
	vbd.MiscFlags = 0;							//misc flags
	vbd.StructureByteStride = 0;				//leave at 0

	//create subresource with buffer data
	D3D11_SUBRESOURCE_DATA vinitData; 
	//apply vertices array
	vinitData.pSysMem = &vertices[0];
	//create buffer from device (stored in mPrimitiveVB
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mPrimitiveVB));

	//create indices vector
	std::vector<UINT> indices; 
	//transfer data from MeshData to new vector
	indices.insert(indices.end(),mMeshptr->Indices.begin(), mMeshptr->Indices.end()); 

	//create index buffer subresource
	D3D11_BUFFER_DESC ibd;										//desc strut for vertex buffer
	ibd.Usage = D3D11_USAGE_IMMUTABLE;							//immutable as shape will be the same
	ibd.ByteWidth = sizeof(UINT) * mMeshptr->Indices.size();	//amount of indices * size per index
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;					//bind as index buffer
	ibd.CPUAccessFlags = 0;										//cpu access flags
	ibd.MiscFlags = 0;											//misc flags
	ibd.StructureByteStride = 0;								//leave at 0

	D3D11_SUBRESOURCE_DATA iinitData;
	//apply indices array
	iinitData.pSysMem = &indices[0];
	//create buffer from device (stored in mprimitiveIB
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mPrimitiveIB)); 
}
//apply transformations and output world matrix
//(to convert from object space to world space) 
XMMATRIX DPhoenix::PrimitiveInstance::CalculateTransforms()
{
	//initialise matrices with identity matrix where appropiate
	XMMATRIX Scale = XMMatrixScaling(mScale.x, mScale.y, mScale.z); 
	XMMATRIX Translation = XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z); 
	XMMATRIX RotationX = XMMatrixRotationX(mRotation.x); 
	XMMATRIX RotationY = XMMatrixRotationY(mRotation.y); 
	XMMATRIX RotationZ = XMMatrixRotationZ(mRotation.z); 

	//rotations must be concatenated correctly in this order

	XMMATRIX Rotation = XMMatrixMultiply(RotationZ, XMMatrixMultiply(RotationY, RotationX)); 

	//final trandformation must also be concatenated correctly in this order
	XMMATRIX World = XMMatrixMultiply(Rotation, XMMatrixMultiply(Scale, Translation)); 

	//return the matrix so it can be used with the shader
	return World; 
}

//render - takes in device context and draws the buffers

void DPhoenix::PrimitiveInstance::Render(ID3D11DeviceContext* dc) {
	//stride (size of each data points)
	UINT stride = sizeof(Vertex::PosColor); 
	//the offset is where the data is the begin - 0 is the beginning
	UINT offset = 0; 

	//this sets the vertex buffer t the input assembler
	dc->IASetVertexBuffers(
		0, 1, &mPrimitiveVB, &stride, &offset); 

	//draw the mesh using the indices
	dc->DrawIndexed(mMeshptr->Indices.size(), 0, 0);
}