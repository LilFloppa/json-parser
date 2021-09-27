#include <iostream>
#include <fstream>
#include <string>
#include <variant>

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

int main()
{
	std::string content = ReadFile("input/nested_key.json");

	JSONTokenizer tokenizer(content);

	while (tokenizer.HasMoreTokens())
	{
		Token token = tokenizer.GetToken();
		std::cout << "Type: (" << token.Type << ") Value: (" << token.Value << ")" << std::endl;
	}

	JsonParser parser(content);
	JsonElement* root = parser.Parse().get();

	auto result = root->FindValue("z");

	for (auto& pair : result)
	{
		std::cout << pair.first << ": " << pair.second << std::endl;
	}

	return 0;
}