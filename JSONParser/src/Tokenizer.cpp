#include "Tokenizer.h"
#include <stdexcept>

JSONTokenizer::JSONTokenizer(std::string json)
{
	this->json = json;
	jsonSize = json.size();
}

bool JSONTokenizer::HasMoreTokens()
{
	return currentPos != jsonSize;
}

void JSONTokenizer::SkipWhitespace()
{
	if (!HasMoreTokens())
		return;

	while (json[currentPos] == ' ' || json[currentPos] == '\n' || json[currentPos] == '\t')
	{
		currentPos++;

		if (currentPos == jsonSize)
			break;
	}
}



Token JSONTokenizer::GetToken()
{
	SkipWhitespace();

	Token token;
	if (currentPos == jsonSize)
	{
		token.Type = TokenType::End;
		return token;
	}

	char c = json[currentPos];

	if (c == '{')
	{
		token.Type = TokenType::OpenCurlyBrace;
	}
	else if (c == '}')
	{
		token.Type = TokenType::CloseCurlyBrace;
	}
	else if (c == '"')
	{
		token.Type = TokenType::String;
		currentPos++;

		while ((c = json[currentPos++]) != '"')
		{
			token.Value.push_back(c);

			if (currentPos == jsonSize)
				throw std::logic_error("Invalid JSON file");
		}
	}
	else if (std::isdigit(c) || c == '-')
	{
		token.Type == TokenType::Number;
	}

	return token;
}