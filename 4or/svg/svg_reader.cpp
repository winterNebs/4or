#include "svg_reader.h"
svgReader::svgReader(std::string file) {			//Read the file and set shapes
	svgReader::shapes = svgReader::read(file);
}
std::vector<shape*> svgReader::getShapes() const{
	return svgReader::shapes;
}
svgReader::~svgReader() {

}
std::vector<glm::vec3> shape::convert() {
	std::vector<glm::vec3> shapes;
	return shapes;
}
void shape::readAttrib(xmlTag t) {}

std::vector<glm::vec3> rect::convert() {
	std::vector<glm::vec3> points;
	points.push_back(glm::vec3(x, y, 0.0f));
	points.push_back(glm::vec3(x + width, y, 0.0f));
	points.push_back(glm::vec3(x + width, y + height, 0.0f));
	points.push_back(glm::vec3(x, y + height, 0.0f));
	return points;
}
void rect::readAttrib(xmlTag t) {		//Read attributes (currently x,y,width,height) from tags
	for (std::map<std::string, std::string>::const_iterator it = t.attrib.begin(); it != t.attrib.end(); ++it) {
		if (it->first == "x") {
			x = std::stof(it->second);
		}
		else if (it->first == "y"){
			y = std::stof(it->second);
		}
		else if (it->first == "width") {
			width = std::stof(it->second);
		}
		else if (it->first == "height") {
			height = std::stof(it->second);
		}
	}
}

std::vector<shape*> svgReader::read(std::string file) {	//Svg file reader
	std::string line;
	std::ifstream svg(file, std::ios::binary);
	std::string total = "";
	if (svg.is_open()) {
		while (getline(svg, line)) { 
			total += line;
		}
		svg.close();
	}
	else {
		std::cout << "FAILED TO READ SVG" << std::endl;
	}
	parse(total);
	std::vector<shape*> shapes;
	for (auto i : tags) {			//Take all tags and convert them into shapes
		if (i.type == "rect") {
			rect* newRect = new rect();
			newRect->readAttrib(i);
			shapes.push_back(newRect);
		}
		else if (i.type == "svg") {
			rect newRect;
			newRect.readAttrib(i);
			width = newRect.width;
			height = newRect.height;
			x = newRect.x;
			y = newRect.y;
		}
	}
	return shapes;
}

void svgReader::parse(std::string input) {			//Logical parsing
	bool openBrack = false;							//Basically reads each character individually, tries to construct tags based on open bracket
	tagStarter tagStart = tagStarter::NONE;			//Sequences, easier to see in action than explain
	bool space = false;
	bool attrib = false;
	std::string temp = "";
	std::string tempAttrib = "";
	for (int i = 0; i < input.size(); ++i) {
		switch (input.at(i)) {
		case '<':
			openBrack = true;
			space = false;
			break;
		case '>':
			openBrack = false;
			if (!space) {
				tags.push_back(xmlTag(temp, tagStart));
				temp = "";
			}
			space = false;
			break;
		case '!':
			tagStart = tagStarter::EXLAM;
			break;
		case '?':
			tagStart = tagStarter::QUEST;
			break;
		case '/':
			tagStart = tagStarter::CLOSE;
			break;
		case ' ':
			if (!space) {
				tags.push_back(xmlTag(temp, tagStart));
				temp = "";
				space = true;
			}
			if (attrib) {
				temp += input.at(i);
			}
			break;
		case '=':
			tempAttrib = temp;
			temp = "";
			break;
		case '\"':
			if (attrib) {
				tags.at(tags.size() - 1).attrib.insert(std::pair<std::string, std::string>(tempAttrib, temp));
				temp = "";
				tempAttrib = "";
				attrib = false;
			}
			else {
				attrib = true;
			}
			break;
		case '\n':
			break;
		default:
			if (openBrack) {
				temp += input.at(i);
			}
		}
	}
}
std::vector<std::vector<glm::vec3>> svgReader::getVerticies() const {
	return svgReader::verticies;
}
float svgReader::getX() const {
	return svgReader::x;
}
float svgReader::getY() const {
	return svgReader::y;
}
float svgReader::getWidth() const {
	return svgReader::width;
}
float svgReader::getHeight() const {
	return svgReader::height;
}