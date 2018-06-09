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
class xmlTag {	//XLM tag defnintion has status (open, close, none, question mark)
public:
	xmlTag(std::string t, tagStarter s) {
		type = t;
		status = s;
	}
	tagStarter status;
	std::string type;								//Track tag name
	std::map<std::string, std::string> attrib;		//Keeps track of attributes
};
struct shape {									//Shape (rectangles for now)
	float x, y, width, height;
	virtual std::vector<glm::vec3> convert();
	virtual void readAttrib(xmlTag t);
};
struct rect : public shape {					//Rectangle is a shape
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
	float getX() const;
	float getY() const;
	float getWidth() const;
	float getHeight() const;
private:
	std::vector<shape*> shapes;
	std::vector<shape*> read(std::string file);
	void parse(std::string input);
	std::vector<std::vector<glm::vec3>> verticies;
	float x, y, width, height;
};
#endif