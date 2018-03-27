#include <iostream>
#include <Imlib2.h>

#if __GNUC__ == 6
#include <experimental/filesystem>
#else
#include <filesystem>
#endif

#include "utils.h"

namespace fs = std::experimental::filesystem;

class art : public utils
{
	public:
		art();
		~art();
		
		void convert(const std::string &file, const std::string &name);
		void copyart(const std::string &file, const std::string &name);
		
	private:
		bool checkFormat(const std::string &file);
		
		std::string nb = getHomeDir() + "/.config/retroarch/thumbnails/Bash - Sh Launcher - PC/Named_Boxarts/";
		std::string ns = getHomeDir() + "/.config/retroarch/thumbnails/Bash - Sh Launcher - PC/Named_Snaps/";
		std::string nt = getHomeDir() + "/.config/retroarch/thumbnails/Bash - Sh Launcher - PC/Named_Titles/";
		
		Imlib_Image image;
};
