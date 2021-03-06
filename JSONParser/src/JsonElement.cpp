#include "JsonElement.h"

std::map<std::string, std::string> JsonElement::FindValue(const std::string& key)
{
	std::map<std::string, std::string> map;
	FindValue(key, "/", map);

	return map;
}

void JsonElement::FindValue(const std::string& key, std::string path, std::map<std::string, std::string>& result)
{
	switch (Type)
	{
	case JsonType::Object:
	{
		auto obj = std::get<std::shared_ptr<JsonObject>>(Value);
		for (auto& pair : *obj.get())
		{
			pair.second->FindValue(key, path + pair.first + "/", result);

			if (pair.first == key)
				result[path + key] = pair.second->ToString(0, path + key + "/", result);
		}
	}
	break;
	case JsonType::Array:
	{
		auto arr = std::get<std::shared_ptr<JsonArray>>(Value);
		int i = 0;
		for (auto& item : *arr.get())
		{
			item->FindValue(key, path + std::to_string(i++) + "/", result);
		}
	}
	break;
	default:
		break;
	}
}

std::string JsonElement::ToString(int depth, std::string path, std::map<std::string, std::string>& map)
{
	std::string result;

	switch (Type)
	{
	case JsonType::String:
		result += "\"" + *std::get<std::shared_ptr<std::string>>(Value).get() + "\"";
		break;
	case JsonType::Object:
	{
		result += "{ ";
		auto obj = std::get<std::shared_ptr<JsonObject>>(Value);
		for (auto& pair : *obj.get())
		{
			result += pair.first;
			result += ": ";
			if (map.find(path + pair.first) != map.end())
				result += map[path + pair.first];
			else
				result += pair.second->ToString(depth + 1, path + pair.first + "/", map);
			result += ", ";
		}

		result.pop_back();
		result.pop_back();
		if (result[result.size() - 1] != '}' && result[result.size() - 1] != ']')
			result += " }";
		else
			result += "}";
	}
	break;
	case JsonType::Array:
	{
		result += "[ ";
		auto arr = std::get<std::shared_ptr<JsonArray>>(Value);
		int i = 0;
		for (auto& item : *arr.get())
		{
			result += item->ToString(0, path + std::to_string(i++) + "/", map);
			result += ", ";
		}

		result.pop_back();
		result.pop_back();
		result += " ]";
	}
	break;
	}

	return result;
}