#include "Saveable.h"
#include <fstream>
#include <type_traits>
#include <iostream>

template<typename T>
void Saveable::load(std::string name, std::vector<T>& vec)
{
	if (ioVars.find(name) == ioVars.end())
		throw std::invalid_argument(name + " does not exist in save file");

	std::string src = ioVars.at(name);
	std::vector<std::string> srcSplit;
	int i;
	while ((i = src.find(',')) != src.npos)
	{
		srcSplit.push_back(src.substr(0, i));
		src.erase(0, i + 1);
	}
	srcSplit.push_back(src);

	vec.clear();
	copy(srcSplit, vec);
}

template void Saveable::load<int>(std::string name, std::vector<int>& vector);
template void Saveable::load<float>(std::string name, std::vector<float>& vector);
template void Saveable::load<std::string>(std::string name, std::vector<std::string>& vector);


Saveable::Saveable()
{
	//dummy when not loading class
}

void Saveable::copy(std::vector<std::string> src, std::vector<float>& dst)
{
	for (std::string el : src)
		dst.push_back(std::stof(el));
}

void Saveable::copy(std::vector<std::string> src, std::vector<int>& dst)
{
	for (std::string el : src)
		dst.push_back(std::stoi(el));
}

void Saveable::copy(std::vector<std::string> src, std::vector<std::string>& dst)
{
	dst = src;
}

std::string Saveable::to_string_ext(int i)
{
	return std::to_string(i);
}

std::string Saveable::to_string_ext(float f)
{
	return std::to_string(f);
}

std::string Saveable::to_string_ext(std::string s)
{
	return s;
}

void Saveable::load(std::string name, int& var)
{
	if (ioVars.find(name) == ioVars.end())
		throw std::invalid_argument(name + " does not exist in save file");

	var = std::stoi(ioVars.at(name));
}

void Saveable::load(std::string name, float& var)
{
	if (ioVars.find(name) == ioVars.end())
		throw std::invalid_argument(name + " does not exist in save file");

	var = std::stof(ioVars.at(name));
}

void Saveable::load(std::string name, std::string& var)
{
	if (ioVars.find(name) == ioVars.end())
		throw std::invalid_argument(name + " does not exist in save file");

	var = ioVars.at(name);
}

void Saveable::save(std::string name, const int value)
{
	ioVars[name] = std::to_string(value);
}

void Saveable::save(std::string name, const float value)
{
	ioVars[name] = std::to_string(value);
}

void Saveable::save(std::string name, const std::string value)
{
	ioVars[name] = value;
}

template<typename T>
void Saveable::save(std::string name, std::vector<T> vec)
{
	std::string val = "";
	for (T elem : vec)
	{
		val += to_string_ext(elem);
		val.append(",");
	}
	val.erase(val.end() - 1);
	ioVars[name] = val;
}

template void Saveable::save<int>(std::string name, std::vector<int> vec);
template void Saveable::save<float>(std::string name, std::vector<float> vec);
template void Saveable::save<std::string>(std::string name, std::vector<std::string> vec);


Saveable::Saveable(std::string filePath)
{
	std::ifstream in(filePath);
	if (!in.is_open())
		throw std::invalid_argument("Save file not found: " + filePath);

	//klasa se sprema u obliku:
	//linija n    : ----naziv_varijable=
	//linije n + k: vrijednost varijable  - ako se radi o vektoru odvojen je zarezima: a,b,c...
	//ioVars ne parsira vrijednosti nego ih ucitava u obliku stringa

	std::string line;
	int readChar = 0;
	while (std::getline(in, line))
	{
		std::string name;
		if (line.substr(0, 4) == "----")
			name = line.substr(4, line.find('=') - 4);
		else
			throw std::runtime_error("Save file is corrupted or bad format");

		std::string value = "";
		while (std::getline(in, line) && line.substr(0, 4) != "----")
		{
			value.append(line);
			readChar = in.tellg();
		}
		in.seekg(readChar);

		ioVars.insert({ name, value });
	}

	in.close();
}

void Saveable::save(std::string path)
{
	try
	{
		saveData();
	}
	catch (std::exception e)
	{
		//ako pokusamo spremiti config (nije implementirano) jer se config moze samo ucitavati
		std::cout << "Saving error: " << e.what() << std::endl << std::endl;
		return;
	}

	std::ofstream out(path, std::ios::out | std::ios::trunc);
	if(!out.is_open())
		throw std::invalid_argument("Error opening file: " + path);

	for (std::pair<std::string, std::string> entry : ioVars)
	{
		out << "----" << entry.first << "=\n";
		out << entry.second << std::endl;
	}

	out.close();
}