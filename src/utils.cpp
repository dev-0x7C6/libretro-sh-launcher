#include "utils.h"

utils::utils(){}
utils::~utils(){}

// https://stackoverflow.com/questions/8520560/get-a-file-name-from-a-path#8520871
struct MatchPathSeparator
{
	bool operator()( char ch ) const
	{
		return ch == '/';
	}
};

std::string utils::basename(const std::string &filename)
{
	return std::string( 
		std::find_if( filename.rbegin(), filename.rend(),
				MatchPathSeparator() ).base(),
			filename.end() );
}

// https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c#4654718
bool utils::is_number(const std::string &s)
{
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

std::string utils::getHomeDir()
{
	struct passwd *pw = getpwuid(getuid());
	const char *homedir = pw->pw_dir;
	
	return std::string(homedir);
}

std::vector<std::string> utils::split(const std::string &s, char delim) 
{
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> elems;
  
	while (std::getline(ss, item, delim))
		elems.push_back(item);
	
	return elems;
}
