#ifndef SVG_READER
#define SVG_READER
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>
enum class tagStarter {
	NONE = 0,
	CLOSE = 1,
	EXLAM = 2,
	QUEST = 3
};
//SVGs are just a type of XML files
class xmlTag {
public:
	xmlTag(std::string t, tagStarter s) {
		type = t;
		status = s;
	}
	tagStarter status;
	std::string type;
	std::map<std::string, std::string> attrib;
};
struct shape {
	float x, y;
	virtual std::vector<glm::vec3> convert();
	virtual void readAttrib(xmlTag t);
};
struct rect : public shape {
	float width, height;
	virtual std::vector<glm::vec3> convert() override;
	void readAttrib(xmlTag t) override;
};
class svgReader {
public:
	std::vector<xmlTag> tags;
	std::vector<shape*> read(std::string file);
private:
	void parse(std::string input);
};
#endif