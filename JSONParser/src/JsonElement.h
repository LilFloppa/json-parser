#pragma once
#include <string>
#include <variant>
#include <map>
#include <vector>
#include <memory>

enum class JsonType
{
	None,
	String,
	Object,
	Array
};

using JsonObject = std::map<std::string, std::shared_ptr<class JsonElement>>;
using JsonArray = std::vector<std::shared_ptr<class JsonElement>>;

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

	std::map<std::string, std::string> FindValue(const std::string& key);

private:
	void FindValue(const std::string& key, std::string path, std::map<std::string, std::string>& result);
	std::string ToString(int depth, std::string path, std::map<std::string, std::string>& map);
};