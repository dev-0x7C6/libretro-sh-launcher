#ifndef __PLAYLIST_GENERATOR_H__
#define __PLAYLIST_GENERATOR_H__

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>

#if __GNUC__ == 6
#include <experimental/filesystem>
#else
#include <filesystem>
#endif

#include "utils.h"
#include "third-party/crc/crc.h"

namespace fs = std::experimental::filesystem;

class playlist_generator : public utils, public crc
{
	public:
		playlist_generator();
		~playlist_generator();
	
		void generate();
};

#endif //__PLAYLIST_GENERATOR_H__
