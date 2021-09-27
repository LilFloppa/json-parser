#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "Tokenizer.h"
#include "JsonParser.h"

std::string ReadFile(std::string filename)
{
	std::ifstream file(filename);

	if (file.good())
	{
		file.seekg(0, std::ios_base::end);
		size_t size = file.tellg();
		file.seekg(0);

		std::string buffer;
		buffer.resize(size);
		file.read(&buffer[0], size);

		file.close();

		return buffer;
	}

	throw std::exception("Failed to open file");
}

std::vector<std::string> GetArgs(int argc, char** argv)
{
	std::vector<std::string> args;
	for (int i = 0; i < argc; i++)
		args.emplace_back(argv[i]);

	return args;
}

bool TryGetArgValue(const std::vector<std::string>& args, const std::string& key, std::string& value)
{
	auto iter = std::find(args.begin(), args.end(), key);
	if (iter != args.end() && ++iter != args.end())
	{
		value = *iter;
		return true;
	}

	return false;
}

bool ArgExists(const std::vector<std::string>& args, const std::string& key)
{
	return std::find(args.begin(), args.end(), key) != args.end();
}

const std::string keyInfo =
"Program args:\n\
-h: program description\n\
-f: filename\n\
-k: key name\n";

const std::string description =
"Program takes a file and the required key as input\n\
and searches for all values corresponding to this key in the file\n\n";

int main(int argc, char** argv)
{
	auto args = GetArgs(argc, argv);
	if (args.size() < 2) 
	{
		std::cout << keyInfo << std::endl;
		return -1;
	}

	if (ArgExists(args, "-h"))
	{
		std::cout << description << keyInfo << std::endl;
		return 0;
	}

	std::string filename;
	if (ArgExists(args, "-f"))
	{
		if (!TryGetArgValue(args, "-f", filename))
		{
			std::cout << "Failed to read -f arg's value" << std::endl;
			return -1;
		}
	}
	else
	{
		std::cout << "Filename is required (-f arg)" << std::endl;
		return -1;
	}

	std::string key;
	if (ArgExists(args, "-k"))
	{
		if (!TryGetArgValue(args, "-k", key))
		{
			std::cout << "Failed to read -k arg's value" << std::endl;
			return -1;
		}
	}
	else
	{
		std::cout << "Key name is required (-k arg)" << std::endl;
		return -1;
	}

	std::string content = ReadFile(filename);

	JsonParser parser(content);
	JsonElement* root = parser.Parse().get();

	auto result = root->FindValue(key);

	for (auto& pair : result)
		std::cout << pair.first << ": " << pair.second << std::endl;

	return 0;
}