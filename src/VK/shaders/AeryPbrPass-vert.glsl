#version 450

//--------------------------------------------------------------------------------------
//  Include IO structures
//--------------------------------------------------------------------------------------

#include "GLTF_VS2PS_IO.glsl"

//--------------------------------------------------------------------------------------
// Constant buffers
//--------------------------------------------------------------------------------------
#include "perFrameStruct.h"

layout (std140, binding = ID_PER_FRAME) uniform _PerFrame 
{
    PerFrame myPerFrame;
};

layout (std140, binding = ID_PER_OBJECT) uniform perObject
{
    mat4 u_mCurrWorld;
    mat4 u_mPrevWorld;
} myPerObject;

mat4 GetWorldMatrix()
{
    return myPerObject.u_mCurrWorld;
}

mat4 GetCameraViewProj()
{
    return myPerFrame.u_mCameraCurrViewProj;
}

mat4 GetPrevWorldMatrix()
{
    return myPerObject.u_mPrevWorld;
}

mat4 GetPrevCameraViewProj()
{
    return myPerFrame.u_mCameraPrevViewProj;
}

#include "GLTFVertexFactory.glsl"

//--------------------------------------------------------------------------------------
// Main
//--------------------------------------------------------------------------------------
void main()
{
	gltfVertexFactory();
}

