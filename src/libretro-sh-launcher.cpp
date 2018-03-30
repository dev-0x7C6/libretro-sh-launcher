#include <iostream>
#include <cstdarg>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "playlist_generator.h"
#include "libretro.h"

static struct libretro
{
   uint32_t *frame_buf;
   struct retro_log_callback logging;
   retro_log_printf_t log_cb;
   retro_video_refresh_t video_cb;
   retro_audio_sample_t audio_cb;
   retro_audio_sample_batch_t audio_batch_cb;
   retro_environment_t environ_cb;
   retro_input_poll_t input_poll_cb;
   retro_input_state_t input_state_cb;
} libretro;

static void fallback_log(enum retro_log_level level, const char *fmt, ...)
{
   (void)level;
   std::va_list va;
   va_start(va, fmt);
   std::vfprintf(stderr, fmt, va);
   va_end(va);
}

void retro_init(void)
{
    libretro.frame_buf = new uint32_t[320*240]();
   
	playlist_generator plst;
	plst.generate();
}

void retro_deinit(void)
{
   delete libretro.frame_buf;
}

unsigned retro_api_version(void)
{
   return RETRO_API_VERSION;
}

void retro_set_controller_port_device(unsigned port, unsigned device)
{
   libretro.log_cb(RETRO_LOG_INFO, "Plugging device %u into port %u.\n", device, port);
}

void retro_get_system_info(struct retro_system_info *info)
{
   memset(info, 0, sizeof(*info));
   info->library_name     = "Sh Launcher";
   info->library_version  = "1.0.5";
   info->need_fullpath    = true;
   info->valid_extensions = "sh|bsh|script";   
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{
   float aspect = 4.0f / 3.0f;
   float sampling_rate = 30000.0f;

   info->timing = (struct retro_system_timing) {
      .fps = 60.0,
      .sample_rate = sampling_rate,
   };

   info->geometry = (struct retro_game_geometry) {
      .base_width   = 320,
      .base_height  = 240,
      .max_width    = 320,
      .max_height   = 240,
      .aspect_ratio = aspect,
   };
}

void retro_set_environment(retro_environment_t cb)
{
   libretro.environ_cb = cb;

   bool no_content = true;
   cb(RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME, &no_content);

   if (cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &libretro.logging))
      libretro.log_cb = libretro.logging.log;
   else
      libretro.log_cb = fallback_log;
}

void retro_set_audio_sample(retro_audio_sample_t cb)
{
   libretro.audio_cb = cb;
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb)
{
   libretro.audio_batch_cb = cb;
}

void retro_set_input_poll(retro_input_poll_t cb)
{
   libretro.input_poll_cb = cb;
}

void retro_set_input_state(retro_input_state_t cb)
{
   libretro.input_state_cb = cb;
}

void retro_set_video_refresh(retro_video_refresh_t cb)
{
   libretro.video_cb = cb;
}

void retro_reset(void)
{
   // Nothing needs to happen when the game is reset.
}

/**
 * libretro callback; Called every game tick.
 *
 * Once the core has run, we will attempt to exit, since Bash is done.
 */
void retro_run(void)
{
   // Clear the display.
   unsigned stride = 320;
   libretro.video_cb(libretro.frame_buf, 320, 240, stride << 2);

   // Shutdown the environment now that Bash has loaded and quit.
   libretro.environ_cb(RETRO_ENVIRONMENT_SHUTDOWN, NULL);
}

bool retro_load_game(const struct retro_game_info *info)
{
	pid_t pid, w;
	int status, ret;

	// Check if there is content to load.
	if (info != NULL && info->path != NULL && info->path[0] != '\0') 
	{
		//sprintf(command, "%s \"%s\"", command, info->path);
		
		if ((pid = fork()) < 0)
		{
			perror("Can't fork process.\n");
			return -1;
		}
		else if (pid == 0)
		{
			if (execlp("/bin/sh", "/bin/sh", "-c", info->path, (char*)NULL) == -1)
			{
                libretro.log_cb(RETRO_LOG_INFO, "Failed to launch %s.\n", info->path);
				return -1;
			}
		}
	}
	
	do
	{
		ret = waitpid(pid, &status, 0);
		/*
		 * Let me explain something. Actually waitpid return -1 here, but retroarch doesn't launch if get -1.
		 * So after an hour of debugging I just don't add error handling. And everything works now.
		 * */
	} while (ret == pid);
}

void retro_unload_game(void)
{
   // Nothing needs to happen when the game unloads.
}

unsigned retro_get_region(void)
{
   return RETRO_REGION_NTSC;
}

bool retro_load_game_special(unsigned game_type, const struct retro_game_info *info, size_t num_info)
{
   return retro_load_game(info);
}

size_t retro_serialize_size(void)
{
   return 0;
}

bool retro_serialize(void *data_, size_t size)
{
   return true;
}

bool retro_unserialize(const void *data_, size_t size)
{
   return true;
}

void *retro_get_memory_data(unsigned id)
{
   (void)id;
   return NULL;
}

size_t retro_get_memory_size(unsigned id)
{
   (void)id;
   return 0;
}

void retro_cheat_reset(void)
{
   // Nothing.
}

void retro_cheat_set(unsigned index, bool enabled, const char *code)
{
   (void)index;
   (void)enabled;
   (void)code;
}
