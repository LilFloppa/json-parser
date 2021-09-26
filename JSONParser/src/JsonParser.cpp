#include "JsonParser.h"

std::shared_ptr<JsonElement> JsonParser::Parse()
{
	if (tokenizer.HasMoreTokens())
	{
		tokenizer.GetToken();
		root = std::make_shared<JsonElement>(JsonType::Object, ParseObject());

		return root;
	}
	else
	{
		throw std::logic_error("Invalid json");
	}
}

std::shared_ptr<JsonObject> JsonParser::ParseObject()
{
	std::shared_ptr<JsonObject> object = std::make_shared<JsonObject>();

	bool objectEnd = false;

	while (!objectEnd)
	{
		if (tokenizer.HasMoreTokens())
		{
			Token token = tokenizer.GetToken();
			std::string key = token.Value;
			tokenizer.GetToken();

			token = tokenizer.GetToken();

			switch (token.Type)
			{
			case TokenType::String:
			case TokenType::Boolean:
			case TokenType::Number:
			case TokenType::Null:
				(*object)[key] = std::make_shared<JsonElement>(JsonType::String, ParseString(token));
				break;

			case TokenType::OpenCurlyBrace:
				(*object)[key] = std::make_shared<JsonElement>(JsonType::Object, ParseObject());
				break;
			
			case TokenType::OpenSquareBracket:
				(*object)[key] = std::make_shared<JsonElement>(JsonType::Array, ParseArray());
				break;
			default:
				throw std::logic_error("Invalid json (invalid token in object)");
			}


			if (!tokenizer.HasMoreTokens())
				throw std::logic_error("Invalid json (unexpected end of object)");

			token = tokenizer.GetToken();
			if (token.Type == TokenType::CloseCurlyBrace)
				objectEnd = true;
		}
		else
		{
			throw std::logic_error("Invalid json (unexpected end of object)");
		}
	}

	return object;
}

std::shared_ptr<JsonArray> JsonParser::ParseArray()
{
	auto arr = std::make_shared<JsonArray>();
	bool arrayEnd = false;

	while (!arrayEnd)
	{
		if (tokenizer.HasMoreTokens())
		{
			Token token = tokenizer.GetToken();

			switch (token.Type)
			{
			case TokenType::String:
			case TokenType::Boolean:
			case TokenType::Number:
			case TokenType::Null:
				arr->push_back(std::make_shared<JsonElement>(JsonType::String, ParseString(token)));
				break;

			case TokenType::OpenCurlyBrace:
				arr->push_back(std::make_shared<JsonElement>(JsonType::Object, ParseObject()));
				break;

			case TokenType::OpenSquareBracket:
				arr->push_back(std::make_shared<JsonElement>(JsonType::Array, ParseArray()));
				break;
			default:
				throw std::logic_error("Invalid json (invalid token in array)");
			}

			if (!tokenizer.HasMoreTokens())
				throw std::logic_error("Invalid json (unexpected end of array)");

			token = tokenizer.GetToken();
			if (token.Type == TokenType::CloseSquareBracket)
				arrayEnd = true;
		}
		else
		{
			throw std::logic_error("unexpected end of array");
		}
	}
	return arr;
}

std::shared_ptr<std::string> JsonParser::ParseString(Token token)
{
	return std::make_shared<std::string>(token.Value);
}

