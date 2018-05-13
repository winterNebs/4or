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
	float x, y, width, height;
	virtual std::vector<glm::vec3> convert();
	virtual void readAttrib(xmlTag t);
};
struct rect : public shape {
	virtual std::vector<glm::vec3> convert() override;
	void readAttrib(xmlTag t) override;
};
class svgReader {
public:
	svgReader(){}
	svgReader(std::string file);
	~svgReader();
	std::vector<xmlTag> tags;
	std::vector<std::vector<glm::vec3>> getVerticies() const;
	std::vector<shape*> getShapes() const;
	int getX() const;
	int getY() const;
	int getWidth() const;
	int getHeight() const;
private:
	std::vector<shape*> shapes;
	std::vector<shape*> read(std::string file);
	void parse(std::string input);
	std::vector<std::vector<glm::vec3>> verticies;
	int x, y, width, height;
};
#endif