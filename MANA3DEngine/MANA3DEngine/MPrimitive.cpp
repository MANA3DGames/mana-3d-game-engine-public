#include "MPrimitive.h"
#include "MString.h"
#include "SceneManager.h"
#include "MANA3DEngine.h"
using namespace MANA3D;
using namespace glm;

#include <algorithm>


MPrimitive::MPrimitive( const char* str )
{
	vector<MPrimitive*>& container = MANA3DEngine::GetIsEditorLoop() ? scenePrimitives : runtimePrimitives;
	MString name( "%s%i", str, (int)container.size() );
	gameObject = SceneManager::AddGameObject( name.GetCStr() );
	container.push_back( this );
}

MPrimitive::~MPrimitive()
{

}



void MPrimitive::AddMeshRenderer( MMesh* mesh )
{
	MeshRenderer* renderer = gameObject->AddComponent<MeshRenderer>();
	MMaterial* mat = MMaterial::Create( MShader::Get( "Diffuse" ), "NewMaterial" );
	renderer->SetMaterial( mat );
	renderer->SetMesh( mesh );
}


GameObject* MPrimitive::GetGameObject()
{
	return gameObject;
}



vector<MPrimitive*> MPrimitive::scenePrimitives;
vector<MPrimitive*> MPrimitive::runtimePrimitives;
MMesh* MPrimitive::rectangleMesh;
MMesh* MPrimitive::cubeMesh;
MMesh* MPrimitive::sphereMesh;


void MPrimitive::CreateAllMeshes()
{
	CreateRectangleMesh();
	CreateCubeMesh();
	CreateSphereMesh();
}
void MPrimitive::CreateRectangleMesh()
{
	//// Set up vertex data (and buffer(s)) and configure vertex attributes
	//float vertices[] = {
	//	// Positions          // Colors           // Texture Coords
	//	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
	//	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
	//	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	//};
	//unsigned int indices[] = {
	//	0, 1, 3, // First triangle
	//	1, 2, 3  // Second triangle
	//};

	/*int vertexCount = ( sizeof( vertices ) / sizeof( *vertices ) );
	int indexCount = ( sizeof( indices ) / sizeof( *indices ) );*/


	
	vector<MVertex> vertices;

	MVertex v0;
	v0.position = vec3( 0.5f,  0.5f, 0.0f );
	v0.normal = vec3( 0.0f, 0.0f, -1.0f );
	v0.texCoords = vec2( 1.0f, 0.0f );
	vertices.push_back( v0 );

	MVertex v1;
	v1.position = vec3( 0.5f, -0.5f, 0.0f );
	v1.normal = vec3( 0.0f, 0.0f, -1.0f );
	v1.texCoords = vec2( 1.0f, 1.0f );
	vertices.push_back( v1 );

	MVertex v2;
	v2.position = vec3( -0.5f, -0.5f, 0.0f );
	v2.normal = vec3( 0.0f, 0.0f, -1.0f );
	v2.texCoords = vec2( 0.0f, 1.0f );
	vertices.push_back( v2 );

	MVertex v3;
	v3.position = vec3( -0.5f, 0.5f, 0.0f );
	v3.normal = vec3( 0.0f, 0.0f, -1.0f );
	v3.texCoords = vec2( 0.0f, 0.0f );
	vertices.push_back( v3 );


	vector<unsigned int> indices;

	indices.push_back( 0 );
	indices.push_back( 1 );
	indices.push_back( 3 );

	indices.push_back( 1 );
	indices.push_back( 2 );
	indices.push_back( 3 );

	rectangleMesh = MMesh::Create( vertices, indices, "Rectangle" );
}
void MPrimitive::CreateCubeMesh()
{
	vector<MVertex> vertices;

	MVertex v0;
	v0.position = vec3( -0.5, -0.5, 0.5 );
	v0.normal = vec3( 0.0f, 0.0f, 1.0f );
	v0.texCoords = vec2( 0.0f, 1.0f );
	vertices.push_back( v0 );

	MVertex v1;
	v1.position = vec3( 0.5, -0.5, 0.5 );
	v1.normal = vec3( 0.0f, 0.0f, 1.0f );
	v1.texCoords = vec2( 1.0f, 1.0f );
	vertices.push_back( v1 );

	MVertex v2;
	v2.position = vec3( 0.5, 0.5, 0.5 );
	v2.normal = vec3( 0.0f, 0.0f, 1.0f );
	v2.texCoords = vec2( 1.0f, 0.0f );
	vertices.push_back( v2 );

	MVertex v3;
	v3.position = vec3( -0.5, 0.5, 0.5 );
	v3.normal = vec3( 0.0f, 0.0f, 1.0f );
	v3.texCoords = vec2( 0.0f, 0.0f );
	vertices.push_back( v3 );

	MVertex v4;
	v4.position = vec3( -0.5, -0.5, -0.5 );
	v4.normal = vec3( 0.0f, 0.0f, -1.0f );
	v4.texCoords = vec2( 0.0f, 1.0f );
	vertices.push_back( v4 );

	MVertex v5;
	v5.position = vec3( 0.5, -0.5, -0.5 );
	v5.normal = vec3( 0.0f, 0.0f, -1.0f );
	v5.texCoords = vec2( 1.0f, 1.0f );
	vertices.push_back( v5 );

	MVertex v6;
	v6.position = vec3( 0.5, 0.5, -0.5 );
	v6.normal = vec3( 0.0f, 0.0f, -1.0f );
	v6.texCoords = vec2( 1.0f, 0.0f );
	vertices.push_back( v6 );

	MVertex v7;
	v7.position = vec3( -0.5, 0.5, -0.5 );
	v7.normal = vec3( 0.0f, 0.0f, -1.0f );
	v7.texCoords = vec2( 0.0f, 0.0f );
	vertices.push_back( v7 );


	vector<unsigned int> indices;
	indices.push_back( 0 );
	indices.push_back( 1 );
	indices.push_back( 2 );
	
	indices.push_back( 2 );
	indices.push_back( 3 );
	indices.push_back( 0 );
	
	indices.push_back( 1 );
	indices.push_back( 5 );
	indices.push_back( 6 );

	indices.push_back( 6 );
	indices.push_back( 2 );
	indices.push_back( 1 );

	indices.push_back( 7 );
	indices.push_back( 6 );
	indices.push_back( 5 );

	indices.push_back( 5 );
	indices.push_back( 4 );
	indices.push_back( 7 );

	indices.push_back( 4 );
	indices.push_back( 0 );
	indices.push_back( 3 );

	indices.push_back( 3 );
	indices.push_back( 7 );
	indices.push_back( 4 );
		
	indices.push_back( 4 );
	indices.push_back( 5 );
	indices.push_back( 1 );

	indices.push_back( 1 );
	indices.push_back( 0 );
	indices.push_back( 4 );
		
	indices.push_back( 3 );
	indices.push_back( 2 );
	indices.push_back( 6 );

	indices.push_back( 6 );
	indices.push_back( 7 );
	indices.push_back( 3 );

	cubeMesh = MMesh::Create( vertices, indices, "Cube" );
}
void MPrimitive::CreateSphereMesh()
{
	//vector<MVertex> vertices;

	//float radius = 0.5f;
	//float sectorCount = 50;
	//float stackCount = 50;

	//float x, y, z, xy;                              // vertex position
	//float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
	//float s, t;                                     // vertex texCoord

	//float sectorStep = 2 * Math3D::PI / sectorCount;
	//float stackStep = Math3D::PI / stackCount;
	//float sectorAngle, stackAngle;

	//for(int i = 0; i <= stackCount; ++i)
	//{
	//	stackAngle = Math3D::PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
	//	xy = radius * cosf(stackAngle);             // r * cos(u)
	//	z = radius * sinf(stackAngle);              // r * sin(u)

	//	// add (sectorCount+1) vertices per stack
	//	// the first and last vertices have same position and normal, but different tex coords
	//	for(int j = 0; j <= sectorCount; ++j)
	//	{
	//		sectorAngle = j * sectorStep;           // starting from 0 to 2pi

	//		// vertex position (x, y, z)
	//		x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
	//		y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
	//		vertices.push_back(x);
	//		vertices.push_back(y);
	//		vertices.push_back(z);

	//		// normalized vertex normal (nx, ny, nz)
	//		nx = x * lengthInv;
	//		ny = y * lengthInv;
	//		nz = z * lengthInv;
	//		normals.push_back(nx);
	//		normals.push_back(ny);
	//		normals.push_back(nz);

	//		// vertex tex coord (s, t) range between [0, 1]
	//		s = (float)j / sectorCount;
	//		t = (float)i / stackCount;
	//		texCoords.push_back(s);
	//		texCoords.push_back(t);
	//	}
	//}
}



void MPrimitive::Destroy( MPrimitive *primitive )
{
	vector<MPrimitive*>& container = MANA3DEngine::GetIsEditorLoop() ? scenePrimitives : runtimePrimitives;
	container.erase( std::remove( container.begin(), container.end(), primitive ), container.end() );
	delete primitive;
}

void MPrimitive::DestroyAll()
{
	while ( !scenePrimitives.empty() )
	{
		MPrimitive *temp = *scenePrimitives.begin();
		scenePrimitives.erase( scenePrimitives.begin() );
		delete temp;
	}
	scenePrimitives.clear();

	while ( !runtimePrimitives.empty() )
	{
		MPrimitive *temp = *runtimePrimitives.begin();
		runtimePrimitives.erase( runtimePrimitives.begin() );
		delete temp;
	}
	runtimePrimitives.clear();
}