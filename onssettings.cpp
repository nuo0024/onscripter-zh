

#include "onssettings.h"
#include <stdio.h>
#include <string.h>
#include <SDL.h>


#if defined(PSP)
#include <psppower.h>
#endif


#if defined(PDA)
#if defined(PDA_WIDTH)
int os_screen_width=PDA_WIDTH;
#else
int os_screen_width=360;
#endif
#endif


#if defined(PSP)
#define NUMBTN	14
SDLKey os_button_map[] = { SDLK_ESCAPE, /* TRIANGLE */
                           SDLK_RETURN, /* CIRCLE   */
                           SDLK_SPACE,  /* CROSS    */
                           SDLK_RCTRL,  /* SQUARE   */
                           SDLK_o,      /* LTRIGGER */
                           SDLK_s,      /* RTRIGGER */
                           SDLK_DOWN,   /* DOWN     */
                           SDLK_LEFT,   /* LEFT     */
                           SDLK_UP,     /* UP       */
                           SDLK_RIGHT,  /* RIGHT    */
                           SDLK_0,      /* SELECT   */
                           SDLK_a,      /* START    */
                           SDLK_UNKNOWN,/* HOME     */ /* kernel mode only */
                           SDLK_UNKNOWN,/* HOLD     */};
                           
static short psp_org_cpu_clock;
static short psp_org_bus_clock;

#elif defined(__PS3__)
#define NUMBTN	19
SDLKey os_button_map[] = {
        SDLK_0,      /* SELECT   */
        SDLK_UNKNOWN,/* L3       */
        SDLK_UNKNOWN,/* R3       */
        SDLK_a,      /* START    */
        SDLK_UP,     /* UP       */
        SDLK_RIGHT,  /* RIGHT    */
        SDLK_DOWN,   /* DOWN     */
        SDLK_LEFT,   /* LEFT     */
        SDLK_SPACE,  /* L2       */
        SDLK_RETURN, /* R2       */
        SDLK_o,      /* L1       */
        SDLK_s,      /* R1       */
        SDLK_ESCAPE, /* TRIANGLE */
        SDLK_RETURN, /* CIRCLE   */
        SDLK_SPACE,  /* CROSS    */
        SDLK_RCTRL,  /* SQUARE   */
        SDLK_UNKNOWN,/* PS       */
        SDLK_UNKNOWN,
        SDLK_UNKNOWN,
    };
#endif


#if defined(PSP) || defined(__PS3__)
int MapButton(SDLKey destkey, const char *key)
{	
	int keycode = atoi(key);
	if (keycode<1 || keycode>NUMBTN) return -1;
	
	os_button_map[keycode-1] = destkey;
	return 0;
}
#endif

#if defined(PSP)
void PSPInit()
{
	psp_org_cpu_clock = scePowerGetCpuClockFrequency();
	psp_org_bus_clock = scePowerGetBusClockFrequency();
}

void PSPFin()
{
	scePowerSetCpuClockFrequency(psp_org_cpu_clock);
	scePowerSetBusClockFrequency(psp_org_bus_clock);
}
#endif


int ReadSettings(ONScripterLabel *pOns, const char *file)
{
	FILE *fp;
	char key[256];
	char value[256];
	int rt;
	//FILE *fp2;

#if defined(PSP)
	int value_int;

	putenv("SDL_ASPECT_RATIO=4:3");
#endif

	fp = fopen(file,"rt");
	//fp2 = fopen("setting.log","wb");
	if (!fp) return -1;
	
	while (!feof(fp))
	{
		rt = fscanf(fp, "%[^=\n]=%[^=\n]\n", key, value);
		if (rt==0 || rt==EOF) break;
		//fprintf(fp2, "KEY=%s, VALUE=%s\n", key, value);

		if (!stricmp(key, "FONT"))
		{
			pOns->setFontFile(value);
		}
		else if (!stricmp(key, "registry"))
		{
			pOns->setRegistryFile(value);
		}
		else if (!stricmp(key, "dll"))
		{
			pOns->setDLLFile(value);
		}
		else if (!stricmp(key, "root"))
		{
			pOns->setArchivePath(value);
		}
		else if (!stricmp(key, "fullscreen"))
		{
			if (!stricmp(value, "yes"))
				pOns->setFullscreenMode();
			else if (!stricmp(value, "no"))
				pOns->setWindowMode();
		}
			
#if defined(PSP)
		if (!stricmp(key, "RESOLUTION"))
		{
			value_int = atoi(value);
			if (value_int > 0 && value_int <= 360) os_screen_width=value_int;
		}
		else if (!stricmp(key, "SCREENSIZE"))
		{
			if (!stricmp(value,"NORMAL"))
			{
				putenv("SDL_ASPECT_RATIO=4:3");
			}
			else if (!stricmp(value,"FULL"))
			{
				putenv("SDL_ASPECT_RATIO=");
			}
		}
		else if (!stricmp(key, "CPUCLOCK"))
		{
			value_int = atoi(value);
			if (value_int > 0 && value_int <= 333)
				scePowerSetCpuClockFrequency(value_int);
		}
		else if (!stricmp(key, "BUSCLOCK"))
		{
			value_int = atoi(value);
			if (value_int > 0 && value_int <= 167)
				scePowerSetBusClockFrequency(value_int);
		}
		
		else if (!stricmp(key, "LMB_ONCE"))
		{
			MapButton(SDLK_SPACE, value);
		}
		else if (!stricmp(key, "LMB"))
		{
			MapButton(SDLK_RETURN, value);
		}
		else if (!stricmp(key, "RMB"))
		{
			MapButton(SDLK_ESCAPE, value);
		}
		else if (!stricmp(key, "CURSOR_PREV"))
		{
			MapButton(SDLK_UP ,value);
		}
		else if (!stricmp(key, "CURSOR_NEXT"))
		{
			MapButton(SDLK_DOWN, value);
		}
		else if (!stricmp(key, "SKIP"))
		{
			MapButton(SDLK_s, value);
		}
		else if (!stricmp(key, "PAGEFLIP"))
		{
			MapButton(SDLK_o, value);
		}
		else if (!stricmp(key, "MEM_UP"))
		{
			MapButton(SDLK_LEFT, value);
		}
		else if (!stricmp(key, "MEM_DOWN"))
		{
			MapButton(SDLK_RIGHT, value);
		}
		else if (!stricmp(key, "SKIPMODE"))
		{
			MapButton(SDLK_s, value);
		}
		else if (!stricmp(key, "AUTOMODE"))
		{
			MapButton(SDLK_a, value);
		}
		else if (!stricmp(key, "SPEED"))
		{
			MapButton(SDLK_0, value);
		}
		else if (!stricmp(key, "NONE"))
		{
			MapButton(SDLK_UNKNOWN, value);
		}
#endif
	}
	
	fclose(fp);
	//fclose(fp2);
	return 0;
}
