#include <string>
#include <variant>
#include <map>
#include <vector>

#include "Tokenizer.h"
#include <optional>

enum class JsonType
{
	None,
	String,
	Object,
	Array
};

using JsonObject = std::map<std::string, std::shared_ptr<class JsonElement>>;
using JsonArray = std::vector< std::shared_ptr<class JsonElement>>;

class JsonElement
{
public:
	JsonType Type = JsonType::None;
	std::variant<
		std::shared_ptr<JsonObject>, 
		std::shared_ptr<JsonArray>, 
		std::shared_ptr<std::string>> Value;

	JsonElement(JsonType type, std::shared_ptr<JsonObject> obj) : Type(type), Value(obj) {}
	JsonElement(JsonType type, std::shared_ptr<JsonArray> arr) : Type(type), Value(arr) {}
	JsonElement(JsonType type, std::shared_ptr<std::string> str) : Type(type), Value(str) {}

	std::string ToString()
	{
		return ToString(0);
	}

private:
	std::string ToString(int depth)
	{
		std::string result;

		switch (Type)
		{
		case JsonType::String:
			result += "\"" + *std::get<std::shared_ptr<std::string>>(Value).get() + "\"";
			break;
		case JsonType::Object:
		{
			result += "{\n";
			auto obj = std::get<std::shared_ptr<JsonObject>>(Value);
			for (auto& pair : *obj.get())
			{
				AddTabs(result, depth + 1);
				result += pair.first;
				result += ": ";
				result += pair.second->ToString(depth + 1);
				result += ",\n";
			}

			result.pop_back();
			result.pop_back();
			result += "\n";
			AddTabs(result, depth);
			result += "}";
		}
		break;
		case JsonType::Array:
		{
			result += "[";
			auto arr = std::get<std::shared_ptr<JsonArray>>(Value);
			for (auto& item : *arr.get())
			{
				result += item->ToString();
				result += ", ";
			}
			
			result.pop_back();
			result.pop_back();
			result += "]";
		}
		break;
		}

		return result;
	}

	void AddTabs(std::string& str, int depth)
	{
		for (int i = 0; i < depth; i++)
			str += "  ";
	}
};

class JsonParser
{
public:
	JsonParser(std::string json) : tokenizer(json) { }
	std::shared_ptr<JsonElement> Parse();

private:
	std::shared_ptr<JsonObject> ParseObject();
	std::shared_ptr<JsonArray> ParseArray();
	std::shared_ptr<std::string> ParseString(Token token);

private:
	JSONTokenizer tokenizer;
	std::shared_ptr<JsonElement> root = nullptr;
	std::shared_ptr<JsonElement> current = nullptr;
};