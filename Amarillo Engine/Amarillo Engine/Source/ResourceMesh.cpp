#include "ResourceMesh.h"

ResourceMesh::ResourceMesh(std::string unique_id) : Resource(unique_id, ResourceType::RT_TEXTURE)
{

}

ResourceMesh::~ResourceMesh()
{
}

void ResourceMesh::CleanUp()
{
	UnloadFromMemory();

	id_vertices = 0;
	num_vertices = 0;
	RELEASE_ARRAY(vertices);

	// Indices
	id_indices = 0;
	num_indices = 0;
	RELEASE_ARRAY(indices);

	// UVs
	id_uv = 0;
	num_uvs = 0;
	RELEASE_ARRAY(uvs);
}

void ResourceMesh::SetFaces(float* _vertices, uint _num_vertices, uint* _indices, uint _num_indices)
{

	if (_num_vertices > 0)
	{
		// Vertices
		vertices = new float[_num_vertices * 3];
		memcpy(vertices, _vertices, sizeof(float) * _num_vertices * 3);
		num_vertices = _num_vertices;

		if (_num_indices > 0)
		{
			// Indices
			indices = new uint[_num_indices];
			memcpy(indices, _indices, sizeof(uint) * _num_indices);
			num_indices = _num_indices;
		}

		CalcMeshBBox();
	}
}

void ResourceMesh::SetUvs(float* _uvs, uint _num_uvs)
{
	if (_num_uvs > 0)
	{
		// UVs
		uvs = new float[_num_uvs * 3];
		memcpy(uvs, _uvs, sizeof(float) * _num_uvs * 3);
		num_uvs = _num_uvs;
	}
}

void ResourceMesh::SetTransform(float3 _pos, Quat _rotation, float3 _scale)
{
	position = _pos;
	rotation = _rotation;
	scale = _scale;

	CalcMeshBBox();
}

uint ResourceMesh::GetIdVertices()
{
	return id_vertices;
}

uint ResourceMesh::GetNumVertices()
{
	return num_vertices;
}

uint ResourceMesh::GetIdIndices()
{
	return id_indices;
}

uint ResourceMesh::GetNumIndices()
{
	return num_indices;
}

uint ResourceMesh::GetIdUV()
{
	return id_uv;
}

uint ResourceMesh::GetNumUVs()
{
	return num_uvs;
}

float* ResourceMesh::GetVertices()
{
	return vertices;
}

uint* ResourceMesh::GetIndices()
{
	return indices;
}

float* ResourceMesh::GetUVs()
{
	return uvs;
}

AABB ResourceMesh::GetBBox()
{
	return bbox;
}

float ResourceMesh::GetDiagonal()
{
	return bbox.Diagonal().Length();
}

float3 ResourceMesh::GetPosition()
{
	return position;
}

Quat ResourceMesh::GetRotation()
{
	return rotation;
}

float3 ResourceMesh::GetScale()
{
	return scale;
}
void ResourceMesh::LoadToMemory()
{
	if (id_vertices == 0 && vertices != nullptr)
		id_vertices = applic->renderer3D->LoadBuffer(vertices, num_vertices * 3);

	if (id_indices == 0 && indices != nullptr)
		id_indices = applic->renderer3D->LoadBuffer(indices, num_indices);

	if (id_uv == 0 && uvs != nullptr)
		id_uv = applic->renderer3D->LoadBuffer(uvs, num_uvs * 3);
}

void ResourceMesh::UnloadFromMemory()
{
	if (id_vertices != 0)
	{
		applic->renderer3D->UnloadBuffer(id_vertices, num_vertices * 3);
		id_vertices = 0;
	}

	if (id_indices != 0)
	{
		applic->renderer3D->UnloadBuffer(id_indices, num_indices);
		id_indices = 0;
	}

	if (id_uv != 0)
	{
		applic->renderer3D->UnloadBuffer(id_uv, num_uvs * 3);
		id_uv = 0;
	}
}

void ResourceMesh::CalcMeshBBox()
{
	bbox.SetNegativeInfinity();

	if (vertices != nullptr && num_vertices > 0)
	{
		bbox.Enclose((float3*)vertices, num_vertices);
	}
}

