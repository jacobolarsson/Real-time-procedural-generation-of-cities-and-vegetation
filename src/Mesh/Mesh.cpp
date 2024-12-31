#define TINYOBJLOADER_IMPLEMENTATION

#include "Mesh.h"
#include "../GameObject/GameObject.h"
#include "../Shapes/Shapes.h"

#include <tiny_obj_loader.h>
#include <iostream>
#include <numeric>
#include <glm/gtc/random.hpp>

using namespace CityGen;

std::unordered_map<std::string, std::vector<Vertex>> Mesh::models{};

void Mesh::LoadVtxData(std::string const& filename, std::string const& modelname)
{
	models.insert(std::make_pair(modelname, std::vector<Vertex>()));

	tinyobj::ObjReader reader;

	if (!reader.ParseFromFile(filename)) {
		if (!reader.Error().empty()) {
			std::cerr << "TinyObjReader: " + reader.Error() << std::endl;
		}
		exit(1);
	}
	if (!reader.Warning().empty()) {
		std::cerr << "TinyObjReader: " << reader.Warning() << std::endl;
	}

	tinyobj::attrib_t const& attrib = reader.GetAttrib();
	std::vector<tinyobj::shape_t> const& shapes = reader.GetShapes();

	models.at(modelname).reserve(shapes.at(0).mesh.indices.size());

	size_t indexOffset = 0;
	// Loop over faces(triangle)
	for (size_t i = 0; i < shapes.at(0).mesh.num_face_vertices.size(); i++) {
		// For each vertex in the face
		for (int j = 0; j < 3; j++) {
			tinyobj::index_t idx = shapes[0].mesh.indices[indexOffset + j];

			float vx = attrib.vertices[3 * static_cast<size_t>(idx.vertex_index)];
			float vy = attrib.vertices[3 * static_cast<size_t>(idx.vertex_index) + 1];
			float vz = attrib.vertices[3 * static_cast<size_t>(idx.vertex_index) + 2];

			float nx = attrib.normals[3 * static_cast<size_t>(idx.normal_index)];
			float ny = attrib.normals[3 * static_cast<size_t>(idx.normal_index) + 1];
			float nz = attrib.normals[3 * static_cast<size_t>(idx.normal_index) + 2];

			models.at(modelname).emplace_back(vec3(vx, vy, vz), vec3(nx, ny, nz));
		}
		indexOffset += 3;
	}
}

// Thomas Wang¡¯s 32 bit Mix Function
unsigned Mesh::Hash(int key) const
{
	key += ~(key << 15);
	key ^= (key >> 10);
	key += (key << 3);
	key ^= (key >> 6);
	key += ~(key << 11);
	key ^= (key >> 16);
	return key;}

void Mesh::FillGlVtxData()
{
	glGenBuffers(1, &m_vbo);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

	// Vertex positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glEnableVertexAttribArray(0);

	// Vertex normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void Cube::Create()
{
	m_vertices.reserve(12 * 3);

	m_vertices.emplace_back(vec3(-0.5f, -0.5f, -0.5f), vec3( 0.0f, -1.0f, 0.0f)); //0
	m_vertices.emplace_back(vec3(0.5f, -0.5f, -0.5f), vec3( 0.0f, -1.0f, 0.0f)); //1
	m_vertices.emplace_back(vec3(0.5f, -0.5f,  0.5f), vec3( 0.0f, -1.0f, 0.0f)); //3

	m_vertices.emplace_back(vec3(-0.5f, -0.5f, -0.5f), vec3( 0.0f, -1.0f, 0.0f)); //0
	m_vertices.emplace_back(vec3(0.5f, -0.5f,  0.5f), vec3( 0.0f, -1.0f, 0.0f)); //3
	m_vertices.emplace_back(vec3(-0.5f, -0.5f,  0.5f), vec3( 0.0f, -1.0f, 0.0f)); //2

	m_vertices.emplace_back(vec3(-0.5f, -0.5f, -0.5f), vec3( 0.0f, 0.0f, -1.0f)); //0
	m_vertices.emplace_back(vec3(-0.5f,  0.5f, -0.5f), vec3( 0.0f, 0.0f, -1.0f)); //4
	m_vertices.emplace_back(vec3(0.5f, -0.5f, -0.5f), vec3( 0.0f, 0.0f, -1.0f)); //1

	m_vertices.emplace_back(vec3(0.5f, -0.5f, -0.5f), vec3( 0.0f, 0.0f, -1.0f)); //1
	m_vertices.emplace_back(vec3(-0.5f,  0.5f, -0.5f), vec3( 0.0f, 0.0f, -1.0f)); //4
	m_vertices.emplace_back(vec3(0.5f,  0.5f, -0.5f), vec3( 0.0f, 0.0f, -1.0f)); //5

	m_vertices.emplace_back(vec3(-0.5f,  0.5f, -0.5f), vec3( 0.0f, 1.0f, 0.0f)); //4
	m_vertices.emplace_back(vec3(-0.5f,  0.5f,  0.5f), vec3( 0.0f, 1.0f, 0.0f)); //6
	m_vertices.emplace_back(vec3(0.5f,  0.5f, -0.5f), vec3( 0.0f, 1.0f, 0.0f)); //5

	m_vertices.emplace_back(vec3(0.5f,  0.5f, -0.5f), vec3( 0.0f, 1.0f, 0.0f)); //5
	m_vertices.emplace_back(vec3(-0.5f,  0.5f,  0.5f), vec3( 0.0f, 1.0f, 0.0f)); //6
	m_vertices.emplace_back(vec3(0.5f,  0.5f,  0.5f), vec3( 0.0f, 1.0f, 0.0f)); //7

	m_vertices.emplace_back(vec3(-0.5f, -0.5f,  0.5f), vec3( 0.0f, 0.0f, 1.0f)); //2
	m_vertices.emplace_back(vec3(0.5f,  0.5f,  0.5f), vec3( 0.0f, 0.0f, 1.0f)); //7
	m_vertices.emplace_back(vec3(-0.5f,  0.5f,  0.5f), vec3( 0.0f, 0.0f, 1.0f)); //6

	m_vertices.emplace_back(vec3(-0.5f, -0.5f,  0.5f), vec3( 0.0f, 0.0f, 1.0f)); //2
	m_vertices.emplace_back(vec3(0.5f, -0.5f,  0.5f), vec3( 0.0f, 0.0f, 1.0f)); //3
	m_vertices.emplace_back(vec3(0.5f,  0.5f,  0.5f), vec3( 0.0f, 0.0f, 1.0f)); //7

	m_vertices.emplace_back(vec3(-0.5f, -0.5f, -0.5f), vec3( -1.0f, 0.0f, 0.0f)); //0
	m_vertices.emplace_back(vec3(-0.5f,  0.5f,  0.5f), vec3( -1.0f, 0.0f, 0.0f)); //6
	m_vertices.emplace_back(vec3(-0.5f,  0.5f, -0.5f), vec3( -1.0f, 0.0f, 0.0f)); //4

	m_vertices.emplace_back(vec3(-0.5f, -0.5f, -0.5f), vec3( -1.0f, 0.0f, 0.0f)); //0
	m_vertices.emplace_back(vec3(-0.5f, -0.5f,  0.5f), vec3( -1.0f, 0.0f, 0.0f)); //2
	m_vertices.emplace_back(vec3(-0.5f,  0.5f,  0.5f), vec3( -1.0f, 0.0f, 0.0f)); //6

	m_vertices.emplace_back(vec3(0.5f, -0.5f,  0.5f), vec3( 1.0f, 0.0f, 0.0f)); //3
	m_vertices.emplace_back(vec3(0.5f,  0.5f, -0.5f), vec3( 1.0f, 0.0f, 0.0f)); //5
	m_vertices.emplace_back(vec3(0.5f,  0.5f,  0.5f), vec3( 1.0f, 0.0f, 0.0f)); //7

	m_vertices.emplace_back(vec3(0.5f, -0.5f,  0.5f), vec3( 1.0f, 0.0f, 0.0f)); //3
	m_vertices.emplace_back(vec3(0.5f, -0.5f, -0.5f), vec3( 1.0f, 0.0f, 0.0f)); //1
	m_vertices.emplace_back(vec3(0.5f,  0.5f, -0.5f), vec3( 1.0f, 0.0f, 0.0f)); //5

	vec3 ownerPos(static_cast<float>(m_owner->GetPos().x) + 0.5f, 0.5f, static_cast<float>(m_owner->GetPos().z) + 0.5f);
	mat4 m2wMtx = glm::translate(mat4(1.0f), ownerPos);
	mat4 m2wNormMtx = glm::transpose(glm::inverse(m2wMtx));

	for (Vertex& vtx : m_vertices) {
		vtx.pos = m2wMtx * vec4(vtx.pos, 1.0f);
		vtx.normal = m2wNormMtx * vec4(vtx.normal, 0.0f);
	}

	FillGlVtxData();
}

void CreateInteriorBranch(int* depth, std::vector<Vertex>* vertices, vec3* pos, std::mt19937* randEngine)
{
	mat4 translate = glm::translate(mat4(1.0f), *pos);
	float angle = 0.0f;
	vec3 rotVec(1.0f);
	vec3 scale = vec3(0.0003f, 0.0005f, 0.0003f);

	std::uniform_real_distribution angleDist(0.5f, glm::half_pi<float>() * 0.75f);
	std::uniform_real_distribution rotVecDist(-1.0f, 1.0f);
	std::uniform_real_distribution branchXZDist(0.5f, 0.7f);
	std::uniform_real_distribution branchYDist(0.6f, 0.8f);

	// If the branch is not the tree trunk, rotate it
	if (*depth > 0) {
		angle = angleDist(*randEngine);
		rotVec.x = rotVecDist(*randEngine);
		rotVec.y = rotVecDist(*randEngine);
		rotVec.z = rotVecDist(*randEngine);
		scale *= vec3(glm::pow(branchXZDist(*randEngine), static_cast<float>(*depth)),
					  glm::pow(branchYDist(*randEngine), static_cast<float>(*depth)),
					  glm::pow(branchXZDist(*randEngine), static_cast<float>(*depth)));
	}

	mat4 rtMtx = glm::rotate(translate, angle, rotVec);
	mat4 m2wMtx = rtMtx * glm::scale(mat4(1.0f), scale);
	mat4 m2wNormMtx = glm::transpose(glm::inverse(rtMtx));

	std::vector<Vertex> const& trunkModel = Mesh::models.at("tree_trunk");
	float maxYPos = std::numeric_limits<float>::min();
	float minYPos = std::numeric_limits<float>::max();
	int maxYIdx = -1;
	int minYIdx = -1;
	for (int i = 0; i < trunkModel.size(); i++) {
		if (trunkModel.at(i).pos.y > maxYPos) {
			maxYPos = trunkModel.at(i).pos.y;
			maxYIdx = i;
		}
		else if (trunkModel.at(i).pos.y < minYPos) {
			minYPos = trunkModel.at(i).pos.y;
			minYIdx = i;
		}
		vec3 p = m2wMtx * vec4(trunkModel.at(i).pos, 1.0f);
		vec3 norm = m2wNormMtx * vec4(trunkModel.at(i).normal, 0.0f);
		vertices->emplace_back(p, norm);
	}
	// Update the current branch depth and the next branch's origin position
	*pos = m2wMtx * vec4(trunkModel.at(maxYIdx).pos + trunkModel.at(minYIdx).pos, 1.0f);
	(*depth)++;
}

void CreateLeafBranch(int* depth, std::vector<Vertex>* vertices, vec3* pos, std::mt19937* randEngine)
{
	CreateInteriorBranch(depth, vertices, pos, randEngine);

	std::uniform_real_distribution sphereSizeDist(0.008f, 0.01f);
	vec3 scale = vec3(sphereSizeDist(*randEngine));
	mat4 translate = glm::translate(mat4(1.0f), *pos + vec3(0.0f, 0.01f, 0.0f));
	mat4 m2wMtx = glm::scale(translate, scale);
	mat4 m2wNormMtx = glm::transpose(glm::inverse(translate));

	std::vector<Vertex> const& sphereModel = Mesh::models.at("sphere");
	for (int i = 0; i < sphereModel.size(); i++) {
		vec3 p = m2wMtx * vec4(sphereModel.at(i).pos, 1.0f);
		vec3 norm = m2wNormMtx * vec4(sphereModel.at(i).normal, 0.0f);
		vertices->emplace_back(p, norm);
	}
}

void PushBranchStatus(int* depth, vec3* pos, std::stack<std::pair<int, vec3>>* stack)
{
	stack->emplace(*depth, *pos);
}

void PopBranchStatus(int* depth, vec3* pos, std::stack<std::pair<int, vec3>>* stack)
{
	std::pair<int, vec3> pair = stack->top();
	stack->pop();
	*depth = pair.first;
	*pos = pair.second;
}

void Tree::Create()
{
	m_lsystem.AddRule('0', "1[0][0]");
	m_lsystem.AddRule('0', "1[0][0][0]");
	m_lsystem.AddRule('0', "1[0][0][0][0]");

	int depth = 0;
	vec3 pos(static_cast<float>(m_owner->GetPos().x) + m_posInCell.x,
			 0.0f,
			 static_cast<float>(m_owner->GetPos().z) + m_posInCell.z);

	m_lsystem.SetFunction('0', std::bind(CreateLeafBranch, &depth, &m_vertices, &pos, &m_randEngine));
	m_lsystem.SetFunction('1', std::bind(CreateInteriorBranch, &depth, &m_vertices, &pos, &m_randEngine));
	m_lsystem.SetFunction('[', std::bind(PushBranchStatus, &depth, &pos, &m_branchStatusStack));
	m_lsystem.SetFunction(']', std::bind(PopBranchStatus, &depth, &pos, &m_branchStatusStack));

	std::string sequence("0");
	std::uniform_int_distribution depthDist(2, 3);
	m_lsystem.GenerateSequence(sequence, depthDist(m_randEngine));
	m_lsystem.ProcessSequence(sequence);

	FillGlVtxData();
}

void Plane::Create()
{
	m_vertices.reserve(6);
	m_vertices.emplace_back(vec3(-0.5f, 0.0f,  0.5f), vec3(0.0f, 1.0f, 0.0f));
	m_vertices.emplace_back(vec3( 0.5f, 0.0f,  0.5f), vec3(0.0f, 1.0f, 0.0f));
	m_vertices.emplace_back(vec3( 0.5f, 0.0f, -0.5f), vec3(0.0f, 1.0f, 0.0f));

	m_vertices.emplace_back(vec3(-0.5f, 0.0f,  0.5f), vec3(0.0f, 1.0f, 0.0f));
	m_vertices.emplace_back(vec3( 0.5f, 0.0f, -0.5f), vec3(0.0f, 1.0f, 0.0f));
	m_vertices.emplace_back(vec3(-0.5f, 0.0f, -0.5f), vec3(0.0f, 1.0f, 0.0f));

	vec3 ownerPos(static_cast<float>(m_owner->GetPos().x) + 0.5f, 0.0f, static_cast<float>(m_owner->GetPos().z) + 0.5f);
	mat4 m2wMtx = glm::translate(mat4(1.0f), ownerPos);

	for (Vertex& vtx : m_vertices) {
		vtx.pos = m2wMtx * vec4(vtx.pos, 1.0f);
	}

	FillGlVtxData();
}

void Building::Create()
{
	// Get a random shapes and copy them into CGAL polygons

	Polygon_2 poly1;
	CreateRandomPoly(poly1);

	Polygon_2 poly2;
	std::uniform_int_distribution idxDist1(static_cast<size_t>(0), poly1.size() - 1);
	size_t randVtxIdx = idxDist1(m_randEngine);
	vec3 offset = vec3(poly1.vertex(randVtxIdx).x(), 0.0f, poly1.vertex(randVtxIdx).y());
	CreateRandomPoly(poly2, offset);

	// Create a polygon that is the result of the union of poly1 and poly2
	Polygon_with_holes_2 unionRes1;

	try {
		CGAL::join(poly1, poly2, unionRes1);
	}
	catch (const CGAL::Precondition_exception& e) {
		std::cerr << e.what() << std::endl;
		return;
	}

	std::uniform_real_distribution heightDist1(3.5, 6.5);
	double height = heightDist1(m_randEngine);

	SurfaceMesh floorMesh1;
	CreateFloorMesh(unionRes1.outer_boundary(), floorMesh1);

	// Create main building step
	SurfaceMesh buildingMeshes[5];
	CGAL::Polygon_mesh_processing::extrude_mesh(floorMesh1, buildingMeshes[0], Vector_3(0.0, height, 0.0));
	
	// Create the building's ceiling
	SurfaceMesh ceiling = floorMesh1;
	for (auto& vtx : ceiling.points()) {
		vec3 vec3Vtx(vtx.x(), height, vtx.z());
		vec3Vtx = glm::scale(mat4(1.0f), vec3(0.8f, 1.0f, 0.8f)) * vec4(vec3Vtx, 1.0f);
		vtx = Point_3(vec3Vtx.x, vec3Vtx.y, vec3Vtx.z);
	}
	CGAL::Polygon_mesh_processing::extrude_mesh(ceiling, buildingMeshes[1], Vector_3(0.0, 0.1, 0.0));

	Polygon_2 poly3;

	std::uniform_int_distribution idxDist2(static_cast<size_t>(0), unionRes1.outer_boundary().size() - 1);
	randVtxIdx = idxDist2(m_randEngine);
	offset = vec3(unionRes1.outer_boundary().vertex(randVtxIdx).x(),
				  0.0f,
				  unionRes1.outer_boundary().vertex(randVtxIdx).y());
	CreateRandomPoly(poly3, offset);

	SurfaceMesh floorMesh2;
	CreateFloorMesh(poly3, floorMesh2);

	// Create second building step

	std::uniform_real_distribution heightDist2(height * 0.5, height * 0.7);
	height = heightDist2(m_randEngine);
	CGAL::Polygon_mesh_processing::extrude_mesh(floorMesh2, buildingMeshes[2], Vector_3(0.0, height, 0.0));

	Polygon_2 poly4;

	randVtxIdx = idxDist2(m_randEngine);
	offset = vec3(unionRes1.outer_boundary().vertex(randVtxIdx).x(),
				  0.0f,
				  unionRes1.outer_boundary().vertex(randVtxIdx).y());
	CreateRandomPoly(poly4, offset);

	SurfaceMesh floorMesh3;
	CreateFloorMesh(poly4, floorMesh3);
	
	// Create thrid building step
	std::uniform_real_distribution heightDist3(height * 0.5, height * 0.7);
	height = heightDist3(m_randEngine);
	CGAL::Polygon_mesh_processing::extrude_mesh(floorMesh3, buildingMeshes[3], Vector_3(0.0, height, 0.0));

	// Store all the building steps' mesh data
	for (int i = 0; i < 4; i++) {
		for (auto const& faceIdx : buildingMeshes[i].faces()) {
			auto vertices = CGAL::vertices_around_face(buildingMeshes[i].halfedge(faceIdx), buildingMeshes[i]);
			std::vector<vec3> points;
			points.reserve(3);
			for (auto const& vtxIdx : vertices) {
				auto const& vtx = buildingMeshes[i].point(vtxIdx);
				points.emplace_back(vtx.x(), vtx.y(), vtx.z());
			}
			vec3 norm = glm::normalize(glm::cross(points[1] - points[0], points[2] - points[0]));
			m_vertices.emplace_back(points[0], norm);
			m_vertices.emplace_back(points[1], norm);
			m_vertices.emplace_back(points[2], norm);
		}
	}

	vec3 ownerPos(static_cast<float>(m_owner->GetPos().x) + 0.5f, 0.0f, static_cast<float>(m_owner->GetPos().z) + 0.5f);
	mat4 m2wMtx = glm::translate(mat4(1.0f), ownerPos) * glm::scale(mat4(1.0f), vec3(0.25f));

	for (Vertex& vtx : m_vertices) {
		vtx.pos = m2wMtx * vec4(vtx.pos, 1.0f);
	}

	FillGlVtxData();
}

void Building::CreateRandomPoly(Polygon_2& poly, vec3 offset /*= { 0.0f, 0.0f, 0.0f }*/)
{
	std::array<std::vector<vec3>, 4> const& shapes = ShapeUtils::GetShapes();
	int maxIdx = static_cast<int>(shapes.size() - 1);

	std::uniform_int_distribution idxDist(0, maxIdx);
	int idx = idxDist(m_randEngine);
	std::uniform_int_distribution polyIdxDist(0, static_cast<int>(shapes[idx].size() - 1));
	int polyIdx = polyIdxDist(m_randEngine);
	std::uniform_real_distribution randAngleDist(0.0f, glm::pi<float>());
	float randAngle = randAngleDist(m_randEngine);

	mat4 mtx = glm::translate(mat4(1.0f), offset) *
			   glm::rotate(mat4(1.0f), randAngle, vec3(0.0f, 1.0f, 0.0f));

	for (int i = 0; i < shapes[idx].size(); i++) {
		vec3 pos = mtx * vec4(shapes[idx][i], 1.0f);
		poly.push_back(Point_2(pos.x, pos.z));
	}
}

void Building::CreateFloorMesh(Polygon_2 const& poly, SurfaceMesh& mesh) const
{
	std::list<Traits::Polygon_2> convexPolygons;
	// If the union is concave, divide it into convex polygons
	CGAL::approx_convex_partition_2(poly.vertices_begin(),
									poly.vertices_end(),
									std::back_inserter(convexPolygons));
	// Triangulate the polygon(s)
	std::list<Triangulation> trians;
	for (auto const& convexPoly : convexPolygons) {
		trians.push_back(Triangulation());
		trians.back().insert_constraint(convexPoly.vertices_begin(), convexPoly.vertices_end(), true);
	}
	// Store all the triangles of the polygon(s)
	for (auto const& trian : trians) {
		for (auto it = trian.faces_begin(); it != trian.faces_end(); it++) {
			auto vtx1 = it->vertex(0)->point();
			auto vtx2 = it->vertex(1)->point();
			auto vtx3 = it->vertex(2)->point();

			auto idx1 = mesh.add_vertex(Point_3(vtx1.x(), 0.0, vtx1.y()));
			auto idx2 = mesh.add_vertex(Point_3(vtx2.x(), 0.0, vtx2.y()));
			auto idx3 = mesh.add_vertex(Point_3(vtx3.x(), 0.0, vtx3.y()));
			mesh.add_face(idx1, idx2, idx3);
		}
	}
}
