#pragma once

#include <string>
#include <vector>
#include <map>

class Saveable
{
private:
	std::map<std::string, std::string> ioVars;

	static void copy(std::vector<std::string> src, std::vector<float>& dst);
	static void copy(std::vector<std::string> src, std::vector<int>& dst);
	static void copy(std::vector<std::string> src, std::vector<std::string>& dst);
	static std::string to_string_ext(int i);
	static std::string to_string_ext(float f);
	static std::string to_string_ext(std::string s);

protected:
	void load(std::string name, int& var);
	void load(std::string name, float& var);
	void load(std::string name, std::string& var);
	template<typename T>
	void load(std::string name, std::vector<T>& vec);

	void save(std::string name, const int value);
	void save(std::string name, const float value);
	void save(std::string name, const std::string value);
	template<typename T>
	void save(std::string name, std::vector<T> vec);

	virtual void saveData() = 0;

public:
	Saveable();
	Saveable(std::string filePath);
	void save(std::string path);
};