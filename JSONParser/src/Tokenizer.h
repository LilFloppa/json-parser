#pragma once
#include <string>

enum class TokenType
{
	String,
	OpenCurlyBrace,
	CloseCurlyBrace,
	Colon,
	Number,
	Array,
	End
};

struct Token
{
	std::string Value;
	TokenType Type;
};

class JSONTokenizer
{
public:
	JSONTokenizer(std::string json);
	bool HasMoreTokens();
	Token GetToken();

private:
	void SkipWhitespace();
	void ProcessNumber();
	void ProcessString();

private:
	std::string json;
	int currentPos = 0;
	int jsonSize;
};