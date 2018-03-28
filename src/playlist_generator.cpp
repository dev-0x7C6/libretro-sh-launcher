#include "playlist_generator.h"
#include "art.h"

#include "third-party/ftlip/ftlip.h"
#include "third-party/https-downloader/HTTPDownloader.hpp"

playlist_generator::playlist_generator(){}
playlist_generator::~playlist_generator(){}

void playlist_generator::generate()
{
    const std::string HOME = getHomeDir();

    // where playlist must be
    const std::string &playlists = HOME + "/.config/retroarch/playlists/Bash - Sh Launcher - PC.lpl";
    // where config must be
    const std::string &config = HOME + "/.config/retroarch/sh-launcher.cfg";
	
	ftlip ini(config);
	const std::string &path = ini.get("path");
	
	std::ofstream oplst(playlists, std::ios_base::app);
	
	int crcnum = 0;
	
	const std::string &cached_s = ini.get("cached");
	std::vector<std::string> cached = split(cached_s, ' ');
	
	art a;
	HTTPDownloader dwnl;
	
	for (auto & p : fs::directory_iterator(path))
    {
        if(is_directory(p.path()))
            continue;

		ftlip sh(p.path());
		
		std::string NAME = sh.get("NAME");
		std::string ART = sh.get("ART");
		
		std::string fpath = p.path();
		fpath.substr(1, fpath.size()-2);
		
		std::string NAMEs = basename(fpath);
		if (NAME != "")
			NAMEs = NAME.substr(1, NAME.size()-2); // remove " symbols

        std::string current = fs::current_path();
        current.substr(1, current.size()-2);
		
		if (ART != "")
		{
			ART = ART.substr(1, ART.size()-2);
			
			if (!is_number(ART)) // doesn't steam id
			{
				a.convert(ART, NAMEs);
			}
			else // steam id
			{
                dwnl.download("http://cdn.edgecast.steamstatic.com/steam/apps/" + ART + "/header.jpg",\
                              "/tmp/header.jpg");
				a.convert("/tmp/header.jpg", NAMEs);
				
				if (fs::exists("/tmp/header.jpg"))
					fs::remove("/tmp/header.jpg");
			}
		}
		else
		{
            if (ini.get("display_generic_art") == "true")
            {
                std::string generic_art = "/xmb/monochrome/png/generic_art.png";

                if (fs::exists(HOME + "/.config/retroarch" + generic_art))
                    a.copyart(HOME + "/.config/retroarch" + generic_art, NAMEs);
                else if (fs::exists("/usr/share/libretro/assets" + generic_art))
                    a.copyart("/usr/share/libretro/assets" + generic_art, NAMEs);
            }
		}
		
		const char *name_cstr = NAME.c_str();
		crcnum = crc32b(name_cstr);
		
		if ( std::find(cached.begin(), cached.end(), std::to_string(crcnum)) != cached.end() )
			continue;

		oplst << fpath << std::endl;
		oplst << NAMEs << std::endl;
        oplst << current << "/sh_launcher_libretro.so" << std::endl;
		oplst << "Linux (Sh Launcher)" << std::endl;
		oplst << std::to_string(crcnum) + "|crc" << std::endl;
		oplst << "Bash - Sh Launcher - PC.lpl" << std::endl;

		ini.append("cached", " " + std::to_string(crcnum));
	}
	
	oplst.close();
}
