#include "svg.h"
std::vector<xmlTag> tags;
svg::svg() {
}
svg::~svg() {
	
}

void svg::parse(std::string input) {
	bool openBrack = false;
	tagStarter tagStart = tagStarter::NONE;
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
				tags.at(tags.size()-1).attrib.insert(std::pair<std::string, std::string>(tempAttrib,temp));
				temp = "";
				tempAttrib = "";
				attrib = false;
			}
			else{
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
	for (auto i : tags) {
		std::cout << i.type << " " << std::endl;
		for (std::map<std::string, std::string>::const_iterator it = i.attrib.begin(); it != i.attrib.end(); ++it)	{
			std::cout << "\t" << it->first << " " << it->second << "\n";
		}
	}
}