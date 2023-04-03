#pragma once

#include "D3DUtil.h"

namespace DPhoenix {
	class PrimitiveInstance {
	private:
		GeometryGenerator::MeshData* mMeshptr; 
	

		//vertex and index buffers for quad and sprite
		ID3D11Buffer* mPrimitiveVB;
		ID3D11Buffer* mPrimitiveIB; 

	public:
		XMFLOAT3 mPosition;		//position
		XMFLOAT3 mVelocity;		//velocity 
		XMFLOAT3 mRelativeVelocity; //velocity based on forward vector
		XMFLOAT3 mScale;		//scale
		XMFLOAT3 mRotation;		//rotation
		XMFLOAT3 mForwardVector; //forward vector
		XMFLOAT3 mRightVector;	//right vector 

		//constructor/destructor 
		PrimitiveInstance();
		~PrimitiveInstance();

		//update instance movement 
		void Update(float deltaTime, bool moveRelative);


	//load instance of mesh using position and colour data 
		void LoadColorInstance(ID3D11Device* md3dDevice, XMFLOAT4 color,
			GeometryGenerator::MeshData* meshPtr);

		//generate world matrix based on scale, translation and roation for rendering
		XMMATRIX CalculateTransforms();

		//render individual instance ro device context provided
		void Render(ID3D11DeviceContext* dc);
	};
}
