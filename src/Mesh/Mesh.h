#pragma once
#include "../Common.h"
#include "../Lsystem/Lsystem.h"

#include <glm/gtc/matrix_transform.hpp> 
#include <vector>
#include <unordered_map>
#include <stack>
#include <random>

#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/partition_2.h>
#include <CGAL/Partition_traits_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/extrude.h>

namespace CityGen
{
	struct Vertex
	{
		Vertex(vec3 p, vec3 n) : pos(p), normal(n) {}

		vec3 pos;
		vec3 normal;
	};

	class GameObject;

	class Mesh
	{
	public:
		Mesh(GameObject* owner, GridCoords pos, int seed)
			: m_owner(owner)
			, m_vertices{}
			, m_vao(0u)
			, m_vbo(0u)
			, m_randEngine(Hash(pos.x^ Hash(pos.z ^ seed)))
		{}
		virtual ~Mesh()
		{
			glDeleteVertexArrays(1, &m_vao);
			glDeleteBuffers(1, &m_vbo);
		}

		virtual void Create() = 0;
		void FillGlVtxData();
		static void LoadVtxData(std::string const& filename, std::string const& modelname);
		unsigned Hash(int key) const;

		GLuint GetVao() const { return m_vao; }
		GLuint GetVbo() const { return m_vbo; }
		size_t GetVtxCount() const { return m_vertices.size(); }

		static std::unordered_map<std::string, std::vector<Vertex>> models;
	protected:
		GameObject* m_owner;
		std::vector<Vertex> m_vertices;
		GLuint m_vao;
		GLuint m_vbo;
		std::mt19937 m_randEngine;
	};

	class Cube : public Mesh
	{
	public:
		Cube(GameObject* owner, GridCoords pos, int seed) : Mesh(owner, pos, seed) { Create(); }
		void Create() override;
	};

	class Tree : public Mesh
	{
	public:
		Tree(GameObject* owner, GridCoords pos, int seed, vec3 posInCell)
			: Mesh(owner, pos, seed)
			, m_lsystem(this)
			, m_branchStatusStack()
			, m_posInCell(posInCell)
		{
			Create();
		}

		void Create() override;

	private:
		Lsystem m_lsystem;
		std::stack<std::pair<int, vec3>> m_branchStatusStack;
		vec3 m_posInCell;
	};

	class Plane : public Mesh
	{
	public:
		Plane(GameObject* owner, GridCoords pos, int seed) : Mesh(owner, pos, seed) { Create(); }
		void Create() override;
	};

	class Building : public Mesh
	{
		using Kernel = CGAL::Exact_predicates_inexact_constructions_kernel;
		using Point_2 = Kernel::Point_2;
		using Point_3 = Kernel::Point_3;
		using Vector_3 = Kernel::Vector_3;
		using Traits = CGAL::Partition_traits_2<Kernel>;
		using Polygon_2 = CGAL::Polygon_2<Kernel>;
		using Polygon_with_holes_2 = CGAL::Polygon_with_holes_2<Kernel>;
		using Triangulation = CGAL::Constrained_Delaunay_triangulation_2<Kernel>;
		using SurfaceMesh = CGAL::Surface_mesh<Point_3>;

	public:
		Building(GameObject* owner, GridCoords pos, int seed) : Mesh(owner, pos, seed) { Create(); }
		void Create() override;
		void CreateRandomPoly(Polygon_2& poly, vec3 offset = { 0.0f, 0.0f, 0.0f });
		void CreateFloorMesh(Polygon_2 const& poly, SurfaceMesh& mesh) const;
	};
}
