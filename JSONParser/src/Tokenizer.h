#pragma once
#include <iostream>
#include <string>

enum class TokenType
{
	None,
	String,
	Number,
	Array,
	Boolean,
	Null,
	OpenCurlyBrace,
	CloseCurlyBrace,
	Colon,
	Comma,
	End
};

inline std::ostream& operator<<(std::ostream& stream, const TokenType& type)
{
	switch (type)
	{
	case TokenType::None:
		stream << "None";
		break;
	case TokenType::String:
		stream << "String";
		break;
	case TokenType::Number:
		stream << "Number";
		break;
	case TokenType::Array:
		stream << "Array";
		break;
	case TokenType::Boolean:
		stream << "Boolean";
		break;
	case TokenType::Null:
		stream << "Null";
		break;
	case TokenType::OpenCurlyBrace:
		stream << "{";
		break;
	case TokenType::CloseCurlyBrace:
		stream << "}";
		break;
	case TokenType::Colon:
		stream << ":";
		break;
	case TokenType::Comma:
		stream << ",";
		break;
	case TokenType::End:
		stream << "End";
		break;
	default:
		break;
	}

	return stream;
}

struct Token
{
	std::string Value;
	TokenType Type = TokenType::None;
};

// TODO: Process array properly
class JSONTokenizer
{
public:
	JSONTokenizer(std::string json);
	bool HasMoreTokens();
	Token GetToken();

private:
	void SkipWhitespace();
	void ProcessNumber(Token& token);
	void ProcessString(Token& token);
	void ProcessArray(Token& token);
	void ProcessBoolean(Token& token);
	void ProcessNull(Token& token);

private:
	std::string json;
	int currentPos = 0;
	int jsonSize;
};