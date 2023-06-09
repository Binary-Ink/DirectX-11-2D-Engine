#pragma once

#include "D3DUtil.h"

namespace DPhoenix {
	class GeometryGenerator {
	public:
		//main struct for storing each vertex generated 
		//creates a full range of data that could be used (not all of it initially)
		//this is different to out input layer vertices as we may only want parts of the information
		struct Vertex {
			Vertex(){}
			Vertex(const XMFLOAT3& p, const XMFLOAT3& n, const XMFLOAT3& t, const XMFLOAT2& uv)
				: Position(p), Normal(n), TangentU(t), TexC(uv) {}
			Vertex(
				float px, float py, float pz,		//position
				float nx, float ny, float nz,		//normal (where facing)
				float tx, float ty, float tz,		//tangent (for lighting)
				float u, float v)					//texture coordinates
				: Position(px, py, pz), Normal(nx, ny, nz), TangentU(tx, ty, tz), TexC(u, v) {}

			XMFLOAT3 Position; 
			XMFLOAT3 Normal;
			XMFLOAT3 TangentU; 
			XMFLOAT2 TexC; 
		};

		//vectors to hold vertices for indices for rendering 
		struct MeshData {
			std::vector<Vertex> Vertices; 
			std::vector<UINT> Indices; 
		};

		///<summary> 
		///creates a box centered at the origin with the given dimentions
		///</summary> 
		void CreateBox(float width, float height, float depth, MeshData& meshData);

		///<summary>
		///creates a sphere at the origin. Slices and stacks control degree of tesselation
		///</summary> 

		void CreateSphere(float radius, UINT sliceCount, UINT stackCount, MeshData& meshData);

		///<summary>
		///creates a mxn grid in zz plane with m as rows and n as columns 
		///</summary> 

		void CreateGrid(float width, float depth, UINT m, UINT n, MeshData& meshData);

	};
}
