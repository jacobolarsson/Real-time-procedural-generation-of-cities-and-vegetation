#include "Lsystem.h"
#include "../Mesh/Mesh.h"

#include <iostream>
#include <glm/gtc/random.hpp>

using namespace CityGen;

void Lsystem::AddRule(char var, std::string const& rule)
{
	if (m_rulesMap.find(var) != m_rulesMap.end()) {
		m_rulesMap.at(var).emplace_back(rule);
		return;
	}
	m_rulesMap.insert(std::make_pair(var, std::vector<std::string>{ rule }));
}

std::string const& CityGen::Lsystem::GetRule(char var) const
{
	if (m_rulesMap.find(var) == m_rulesMap.end()) {
		std::cerr << "Lsystem error: Could not find rule for the given var\n";
		return {};
	}
	if (m_rulesMap.at(var).size() == 1) {
		return m_rulesMap.at(var).at(0);
	}
	// Return a random rule from the var's list of possible rules
	int idx = glm::linearRand(0, static_cast<int>(m_rulesMap.at(var).size()) - 1);
	return m_rulesMap.at(var).at(idx);
}

void Lsystem::SetFunction(char var, std::function<void()> func)
{ 
	m_funcMap.insert(std::make_pair(var, func));
}

void Lsystem::GenerateSequence(std::string& currentSeq, int depth)
{
	if (depth <= 0) {
		return;
	}
	// Use the given set of rules to translate the variables from currentSeq
	for (int i = 0; i < currentSeq.size(); i++) {
		char c = currentSeq.at(i);
		if (m_rulesMap.find(c) != m_rulesMap.end()) {
			std::string const& rule = GetRule(c);
			currentSeq.replace(i, 1, rule);
			i += rule.length() - 1;
		}
	}
	return GenerateSequence(currentSeq, depth - 1);
}

void Lsystem::ProcessSequence(std::string const& sequence)
{
	for (char c : sequence) {
		if (m_funcMap.find(c) != m_funcMap.end()) {
			std::invoke(m_funcMap.at(c));
		}
	}
}