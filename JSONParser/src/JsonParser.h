#include "Tokenizer.h"
#include "JsonElement.h"



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