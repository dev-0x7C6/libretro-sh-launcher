#include "ftlip.h"

ftlip::ftlip(const std::string &f)
{
	this->file = f;
}
ftlip::~ftlip(){}

std::vector<std::string> ftlip::split(const std::string &s, char delim) 
{
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> elems;
  
	while (std::getline(ss, item, delim))
		elems.push_back(item);
	
	return elems;
}

const void ftlip::append(const std::string &field, const std::string &str)
{
	this->set(field, this->get(field) + str);
}

const void ftlip::set(const std::string &field, const std::string &str)
{
	std::ifstream file (this->file);
	std::string line = "";
	bool found = false;
	
	std::vector<std::string> filev;
	
	if (file.is_open())
	{
		while (std::getline(file, line))
			filev.push_back(line);
		
		file.close();
	}
	
	if ( std::find(filev.begin(), filev.end(), field + "=" + str) != filev.end() )
		return;
		
	std::ofstream ofile (this->file);
	
	for (auto i = 0; i < filev.size(); i++)
	{
		if (filev.at(i).find(field) != std::string::npos)
		{
			found = true;
			filev.at(i) = field + "=" + str;
		}
		
		ofile << filev.at(i) << std::endl;
	}
	
	if (!found)
		ofile << field + "=" + str << std::endl;
	
	ofile.close();
}

const std::string ftlip::get(const std::string &field)
{
	std::string s = "";
	std::vector<std::string> ss;
	std::string line = "";
	std::ifstream file (this->file);
	
	std::string tmp = "";
		
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			if (line.size() > 0 && line.at(0) == '#') continue;
			
			if((line.find(field)) != std::string::npos)
			{
				s = line;
				break;
			}
		}
		file.close();
	}
	
	if (!s.empty())
	{
		try
		{
			ss = split(s, '=');
			s = "";
			
			for (auto i = 1; i < ss.size(); i++)
			{
				if (i > 1 && i < ss.size())
					s += "=";
					
				s += ss.at(i);
			}
		}
		catch(...){s = "";}
	}
	
	return s;
}
