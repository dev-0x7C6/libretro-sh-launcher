#!/bin/sh

notify-send "Libretro Sh launcher:" "Updating playlist..." -t 2

lpl="$HOME/.config/retroarch/playlists/Bash - Sh Launcher - PC.lpl"
tumb="$HOME/.config/retroarch/thumbnails/Bash - Sh Launcher - PC"
path=$(awk -F'path=' '{print $2}' $HOME/.config/retroarch/sh-laucher.cfg | tr -d '\n')

if [ ! -d $path ]; then
	notify-send "Libretro Sh Launcher error:" "Path $path doesn't exist."
	notify-send "Libretro Sh Launcher error:" "Plese fix your config and reload retroarch."
	exit 0
fi

if [ ! -f "$lpl" ]; then
	touch "$lpl"
fi

if [ ! -d "$tumb" ]; then
	mkdir "$tumb"
	mkdir "$tumb/Named_Boxarts"
	mkdir "$tumb/Named_Snaps"
	mkdir "$tumb/Named_Titles"
fi

for file in $path/*; do
	NAME=$(awk -F'NAME=' '{print $2}' $file | tr -d '\n"')
	[ -z "$NAME" ] && NAME=$(basename "$file")
	
	if [ -z "$(grep $file "$lpl")" ]; then
		notify-send "Libretro Sh launcher:" "Found new file $(basename $file)" -t 1
		echo $file >> "$lpl"		
		echo $NAME >> "$lpl"
		echo "/usr/lib/x86_64-linux-gnu/libretro/sh_launcher_libretro.so" >> "$lpl"
		echo "Linux (Sh Launcher)" >> "$lpl"
		echo "$(crc32 $file)|crc" >> "$lpl"
		echo "Bash - Sh Launcher - PC.lpl" >> "$lpl"
	fi
	
	ART=$(awk -F'ART=' '{print $2}' $file | tr -d '\n"')
	
	if [ ! -z "$(echo $ART | grep '^-\?[0-9]*$')" ]; then # steam id
		wget http://cdn.edgecast.steamstatic.com/steam/apps/$ART/header.jpg -O /tmp/header.jpg
		convert /tmp/header.jpg /tmp/$(echo "header.jpg" | sed 's/jpg/png/g')
		
		md5cur="1"
		md5thb="1"
		
		if [ -f "$tumb/Named_Boxarts/$NAME.png" ]; then
			md5cur=$(md5sum "/tmp/header.png" | cut -d' ' -f1)
			md5thb=$(md5sum "$tumb/Named_Boxarts/$NAME.png" | cut -d' ' -f1)
		fi
		
		if [ "$md5cur" != "$md5thb" ]; then
			cp /tmp/header.png "$tumb/Named_Boxarts/$NAME.png"
			cp /tmp/header.png "$tumb/Named_Snaps/$NAME.png"
			cp /tmp/header.png "$tumb/Named_Titles/$NAME.png"
		fi
		
		rm /tmp/header.png
		rm /tmp/header.jpg
	elif [ ! -z "$ART" ] && [ -f "$ART" ]; then # file is exists
		
		md5cur="1"
		md5thb="1"
		
		if [ -f "$tumb/Named_Boxarts/$NAME.png" ]; then
			md5cur=$(md5sum "$ART" | cut -d' ' -f1)
			md5thb=$(md5sum "$tumb/Named_Boxarts/$NAME.png" | cut -d' ' -f1)
		fi
		
		if [ "$md5cur" != "$md5thb" ]; then
			notify-send "Libretro Sh launcher:" "Update art for $(basename $file)" -t 1
			ext=$(file "$ART" | cut -d':' -f2 | cut -d' ' -f2)
			a=$(basename "$ART")
			fixext=$(echo "$a" | sed 's/jp?g/png/g')
			
			if [ "$ext" != "PNG" ]; then
				cp "$ART" /tmp/
				convert /tmp/"$a" /tmp/$fixext
				ART="/tmp/$fixext"
			fi
			
			cp "$ART" "$tumb/Named_Boxarts/$NAME.png"
			cp "$ART" "$tumb/Named_Snaps/$NAME.png"
			cp "$ART" "$tumb/Named_Titles/$NAME.png"
			
			if [ -f "/tmp/$fixext" ]; then
				rm /tmp/$fixext
			fi
		fi
	else # generic art
		gen=$(awk -F'display_generic_art=' '{print $2}' $HOME/.config/retroarch/sh-laucher.cfg | tr -d '\n')
		if [ ! -z "$gen" ] || [ "$gen" = "true" ]; then
			cp "/usr/share/libretro/assets/xmb/monochrome/png/generic_art.png" "$tumb/Named_Boxarts/$NAME.png"
			cp "/usr/share/libretro/assets/xmb/monochrome/png/generic_art.png" "$tumb/Named_Snaps/$NAME.png"
			cp "/usr/share/libretro/assets/xmb/monochrome/png/generic_art.png" "$tumb/Named_Titles/$NAME.png"
		else
			continue
		fi
	fi
done
