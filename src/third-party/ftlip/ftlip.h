#ifndef __FTLIP_H__
#define __FTLIP_H__

#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <vector>

class ftlip final
{
		public:
			ftlip(const std::string &f);
			~ftlip();
			
			const std::string get(const std::string &field);
			const void set(const std::string &field, const std::string &str);
			const void append(const std::string &field, const std::string &str);
			
		private:
			std::string file;
			static std::vector<std::string> split(const std::string &s, char delim);
};

#endif /* __FTLIP_H__ */
