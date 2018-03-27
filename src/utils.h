#ifndef __UTILS_H__
#define __UTILS_H__

#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

class utils
{
	public:
		utils();
		~utils();
		
	protected:
		static std::string basename(const std::string &str);
		static std::string getHomeDir();
		static std::vector<std::string> split(const std::string &str, char delim);
		static bool is_number(const std::string &s);
};

#endif /* __UTILS_H__ */
