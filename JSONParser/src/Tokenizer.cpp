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

void JSONTokenizer::ProcessNumber(Token& token)
{
	token.Type = TokenType::Number;
	char c = json[currentPos];
	while (std::isdigit(c) || c == '+' || c == '-' || c == '.' || c == 'E' || c == 'e')
	{
		token.Value += c;
		currentPos++;

		if (currentPos == jsonSize)
			throw std::logic_error("Invalid JSON file");

		c = json[currentPos];
	}

	currentPos--;
}

void JSONTokenizer::ProcessString(Token& token)
{
	token.Type = TokenType::String;
	char p = json[currentPos++];
	char c = json[currentPos];
	while (!(c == '"' && p != '\\'))
	{
		token.Value += c;
		currentPos++;

		if (currentPos == jsonSize)
			throw std::logic_error("Invalid JSON file");

		p = c;
		c = json[currentPos];
	}
}

void JSONTokenizer::ProcessBoolean(Token& token)
{
	token.Type = TokenType::Boolean;
	if (json[currentPos] == 't')
	{
		token.Value = json.substr(currentPos, 4);
		currentPos += 3;
	}
	else
	{
		token.Value = json.substr(currentPos, 5);
		currentPos += 4;
	}


}

void JSONTokenizer::ProcessNull(Token& token)
{
	token.Type = TokenType::Null;
	currentPos += 3;
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

	if (c == '"')
	{
		ProcessString(token);
	}
	else if (std::isdigit(c) || c == '-')
	{
		ProcessNumber(token);
	}
	else if (c == '[')
	{
		token.Type = TokenType::OpenSquareBracket;
	}
	else if (c == ']')
	{
		token.Type = TokenType::CloseSquareBracket;
	}
	else if (c == '{')
	{
		token.Type = TokenType::OpenCurlyBrace;
	}
	else if (c == '}')
	{
		token.Type = TokenType::CloseCurlyBrace;
	}
	else if (c == ',')
	{
		token.Type = TokenType::Comma;
	}
	else if (c == ':')
	{
		token.Type = TokenType::Colon;
	}
	else if (c == 't' || c == 'f')
	{
		ProcessBoolean(token);
	}
	else if (c == 'n')
	{
		ProcessNull(token);
	}

	currentPos++;

	return token;
}