#pragma once
#include <string>
#include <map>
enum class tagStarter {
	NONE = 0,
	CLOSE = 1,
	EXLAM = 2,
	QUEST = 3
};
//SVGs are just a type of XML files
class xmlTag {
public:
	xmlTag(std::string t, tagStarter s){
		type = t;
		status = s;
	}
	~xmlTag();
	tagStarter status;
	std::string type;
	std::map<std::string, std::string> attrib;
};