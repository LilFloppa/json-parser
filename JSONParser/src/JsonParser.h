#include <string>
#include <variant>
#include <map>
#include <vector>

#include "Tokenizer.h"

enum JsonTypes 
{
	String,
	Object,
	List
};

using JsonObject = std::map<std::string, JsonElement*>;
using JsonList = std::vector<JsonElement*>;

struct JsonElement
{
	JsonTypes Type;
	std::variant<JsonObject*, JsonList*, std::string*> Value;
};

class JsonParser
{
public:
	JsonParser(std::string json) : tokenizer(json) { }

	JsonElement Parse();
private:
	JSONTokenizer tokenizer;
	JsonElement* root;
	JsonElement* current;
};