#include "Parser.h"

//We reserve 26 as the value for the channel


//All values must be in format argument a b c

Parser::Parser()
{
	for (size_t i = 0; i < 27; i++)
		alphaVals[i] = 0.f;
}

void Parser::operator = (const Parser& P)
{
	values = P.values;
}

void Parser::updateLogic(std::string s)
{
	resetTable();
	std::replace(s.begin(), s.end(), '\n', ' ');
	if (s.size() < 2) { return; }
	std::stringstream ss(s);
	std::vector<std::string> vstrings{};
	vstrings.empty();
	std::string tmp;
	size_t pos = 0;
	while ((pos = s.find(" ")) != std::string::npos) {
		vstrings.push_back(s.substr(0, pos));
		s.erase(0, pos + 1);
	}
	
	if (vstrings.size() % 4 != 0)
		return;
		
	for (size_t i = 0; (vstrings.size() - i >= 4); i = i + 4)
	{
		bool type = false;
		bool found = false;
		int a, b, c, d = 0;
		float e;
		//op logic
		if(!(vstrings[i].size() > 2 && vstrings[i+1].size() > 0 && vstrings[i + 2].size() > 0 && vstrings[i + 3].size() > 0))
		{
			values.clear(); return;
		}
		for (int j = 0; (j < 13 && found != true); j++)
		{
			if (ops[j] == vstrings[i]){ a = j; found = true; }
		}
		if(found == false) { values.clear(); return; }

		//a1 logic
		if (std::regex_match(vstrings[i + 1], identRegex)) {
			b = (int)vstrings[i + 1].at(0) - (int)'a';
		}
		else if (vstrings[i + 1] == "@") {
			b = 26;
		}
		else { values.clear(); return; }

		//a2 logic
		if (std::regex_match(vstrings[i + 2], identRegex)) {
			c = (int)vstrings[i + 2].at(0) - (int)'a';
			e = 0.f;
			type = false;
		}
		else if (vstrings[i + 2].at(0) == '@') {
			c = 26;
			e = 0.f;
			type = false;
		}
		else if (std::regex_match(vstrings[i + 2], doubleRegex)) {
			c = -1;
			type = true;
			e = std::stof(vstrings[i + 2]);
		}
		else { values.clear(); return; }

		//rt
		if (std::regex_match(vstrings[i + 3], identRegex)) {
			d = (int)vstrings[i + 3].at(0) - (int)'a';
		}
		else if (vstrings[i + 3].at(0) == '@') {
			d = 26;
		}
		else if ((a == 11 || a == 12) && std::regex_match(vstrings[i + 3], intRegex))			//conditional result for jil and jig
		{
			d = std::stoi(vstrings[i + 3]);
		}
		else { values.clear(); return; }

		values.push_back(chunkVal(type, a, b, c, e, d));
	}
}

Parser::~Parser()
{
	values.empty();
}

