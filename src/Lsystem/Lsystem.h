#pragma once

#include <unordered_map>
#include <string>
#include <functional>

namespace CityGen
{
	class Mesh;

	class Lsystem
	{
	public:
		Lsystem(Mesh* owner) : m_owner(owner), m_rulesMap{} {}
		
		void AddRule(char var, std::string const& rule);
		std::string const& GetRule(char var) const;
		void SetFunction(char var, std::function<void()> func);

		void GenerateSequence(std::string& currentSeq, int depth);
		void ProcessSequence(std::string const& sequence);

	private:
		Mesh* m_owner;
		std::unordered_map<char, std::vector<std::string>> m_rulesMap;
		std::unordered_map<char, std::function<void()>> m_funcMap;
	};

}
