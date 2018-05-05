#include "svgReader.h"

void svgReader::read() {
	std::string line;
	svg test = svg();
	//std::ifstream svg("test.txt");
	std::ifstream svg("2rect.svg", std::ios::binary);
	std::string total = "";
	if (svg.is_open()) {
		while (getline(svg, line)) { 
			total += line;
			//std::cout << line << '\n';
		}
		svg.close();
	}
	else {
		std::cout << "FAILED TO READ SVG" << std::endl;
	}
	test.parse(total);

}