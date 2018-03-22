#!/bin/sh

lpl="$HOME/.config/retroarch/playlists/Bash - Sh Launcher - PC.lpl"
tumb="$HOME/.config/retroarch/thumbnails/Bash - Sh Launcher - PC"
path=$(awk -F'path=' '{print $2}' sh-laucher.cfg | tr -d '\n')

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
	[ -z $NAME ] && NAME=$(basename "$file")
	
	if [ -z "$(grep $file "$lpl")" ]; then
		echo $file >> "$lpl"		
		echo $NAME >> "$lpl"
		echo "/usr/lib/x86_64-linux-gnu/libretro/sh_launcher_libretro.so" >> "$lpl"
		echo "Linux (Sh Launcher)" >> "$lpl"
		echo "$(crc32 $file)|crc" >> "$lpl"
		echo "Bash - Sh Launcher - PC.lpl" >> "$lpl"
	fi
	
	ART=$(awk -F'ART=' '{print $2}' $file | tr -d '\n"')
	
	if [ ! -z "$ART" ] && [ -f "$ART" ]; then
		
		md5cur="1"
		md5thb="1"
		
		if [ -f "$tumb/Named_Boxarts/$NAME.png" ]; then
			md5cur=$(md5sum "$ART" | cut -d' ' -f1)
			md5thb=$(md5sum "$tumb/Named_Boxarts/$NAME.png" | cut -d' ' -f1)
		fi
		
		if [ "$md5cur" != "$md5thb" ]; then
			cp "$ART" "$tumb/Named_Boxarts/$NAME.png"
			cp "$ART" "$tumb/Named_Snaps/$NAME.png"
			cp "$ART" "$tumb/Named_Titles/$NAME.png"
		fi
	fi
done
