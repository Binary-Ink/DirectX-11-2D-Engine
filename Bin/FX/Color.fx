//colour shader - basic fx 

//constant buffer - per object
// - world view projection combined matrix 
// - this is to transform the vertices 
cbuffer cbperObject {
	float4x4 gWorldProj;
};

//struct for vertex input 
struct VertexIn {
	float3 Pos : POSITION; //xyz position in world space
	float4 Color : COLOR; //colour of each vertex rgba
};

//struct for vertex output/pixel input
struct VertexOut {
	//SV is a SYSTEM VALUE - used for final outputs of 'processed' vertices
	float4 PosH : SV_POSITION; 
	float4 Color : COLOR; //colour in rgba
};

//vertex shader 
VertexOut VS(VertexIn vin) {
	VertexOut vout; 

	//transforms homogenous clip space (what can be seen from camera) 
	//multiplies vertex by matrix 
	//float 4 adds 4th dimention with value of 1.0f
	//ro ensure multiplication with vertices is valid, this is standard and won't change 
	vout.PosH = mul(float4(vin.Pos, 1.0f), gWorldProj);

	//pass vertex color into pixel shader
	vout.Color = vin.Color; 

	//this returns Vertexout struct so it can be used by pixel shader

	return vout; 
}

//Pixel Shader
float4 PS(VertexOut pin) : SV_TARGET{
	//return colour for given vertex
	return pin.Color;
};

//name of technique for effects class
technique11 ColorTech {
	//only one pass, multiple can be used for complex effects
	pass P0 {
		//vertex shaders and pixel shaders set to above functions
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};