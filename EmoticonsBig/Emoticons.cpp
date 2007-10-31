// Emoticons.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <stdio.h>
#include "../Wippien/Buffer.h"
#include "MMSystem.h"

#define TOTAL_EMOTICONS 252
static char *m_NamesText[TOTAL_EMOTICONS] = {":mellow:",":huh:","^_^",":o",";)",":p",":o:",":lol:","8)",":roll:","-_-","<_<",":)",":wub:",":x:",":(",":unsure:",":wacko:",":blink:",":ph34r:",":huggles:",":sweating:",":thumbsup:",":-",":zorro:",":santa:",":shaun:",":ninja:",":sorcerer:",":teehee:","=]",":pirat:",":gum:",":homestar:",":geek:",":phone:",":cat:",":luck:",":bom:",":evil:",":ermm:","o:)",":p:",">_<",":alien:",":((",":shifty:",":scooter:","(w00t)",":!:",":cat2:",":>",":band:",":beach:",":bear:",":cheer:",":cheers:",":clap:",":doh:",":drinks:",":dwarf:",":fool:",":good:",":headspin:",":hi:",":huglove:",":jester:",":king:",":lazy:",":idea:",":lol2:",":nep:",":offtopic:",":oops:",":orc:",":partytime:",":pilot:",":rock:",":santa2:",":shipwrecked:",":shout:",":spiteful:",":tease:",":unknw:",":vampire:",":vava:",":victory:",":whack:",":yahoo:",":yawn:",":yes:",":crazy:",":pokey:",":ok:",":scare:",":soc:",":flowers:",":rtfm:",":kiss:",":agresiv:",":think:",":beee:",":rot:",":swoon:",":wardellipse:",":ro:",":d",":x",":skull:",":aranjare:",":stars:",":stink:",":stinker:",":stretcher:",":suicide:",":superman:",":superstition:",":swordfight:",":example:",":taunt:",":phone:",":this:",":senile:",":order:",":tomato:",":umnik:",":vinsent:",":aikido:",":airkiss:",":alcoholic:",":goalie:",":bataita:",":boss:",":biker:",":book:",":boredom:",":butcher:",":bye:",":clapping:",":comando:",":cow:",":dance2:",":dance3:",":dance4:",":doctor:",":download:",":elf:",":feminist:",":fingers:",":fish:",":flirt:",":focus:",":friends:",":frusty:",":imao:",":juggle:",":moil:",":nea:",":neo:",":nhl:",":noti:",":paladin:",":party:",":pope:",":rabbi:",":read:",":russian:",":salut:",":pioneer:",":plane:",":queen:",":rap:",":scout:",":search:",":secret:",":threaten:",":wizard:",":firstmove:",":giggle:",":gdevil:",":gwerewolf:",":giveheart:",":giverose:",":help:",":hunter:",":ill:",":kisses:",":mamba:",":inlove:",":yam:",":patsak:",":phiu:",":pleasantry:",":sclerosis:",":acute:",":banned:",":drag:",":haha:",":king2:",
					":pash:",":brunette:",":close:",":crazypilot:",":sorry:",":angry:",":bb:",":cool:",":dance:",":derisive:",":pardon:",":yu:",":triniti:",":mda:",":no:",":meeting:",":protest:",":resent:",":scenic:",":kick:",":snooks:",":stop:",":pogranichnik:",":artist:",":baby:",":bag:",":balloon:",":bann:",":beer:",":blow:",":boxing:",":chef:",":clown:",":cowboy:",":donatello:",":drunk:",":eat:",":eek:",":flex:",":gathering:",":notworthy:",":pepsi:",":photo:",":pimp:",":pizza:",":serenade:",":smartass:",":spike:",":spam:",":surrender:",":tooth:",":))",":tongue:"};
static char *m_NamesHtml[TOTAL_EMOTICONS] = {"","","","","","","","","","","","&lt;_&lt;","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","&gt;_&lt;","","","","","","","",":&gt;","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",
"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""};


int m_Index[TOTAL_EMOTICONS+1];
_Buffer m_Path;
HANDLE m_hInstance;

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			{
				m_hInstance = hModule;

				for (int i=0;i<TOTAL_EMOTICONS;i++)
				{
					m_Index[i] = i;
				}

				// now sort..
				for (i=0;i<TOTAL_EMOTICONS-1;i++)
				{
					for (int o=i+1;o<TOTAL_EMOTICONS;o++)
					{
						if (strlen(m_NamesText[m_Index[i]])<strlen(m_NamesText[m_Index[o]]))
						{
							int j = m_Index[i];
							m_Index[i] = m_Index[o];
							m_Index[o] = j;
						}
					}
				}

				// set up path
				m_Path.Append("res://");
				char buff[MAX_PATH];
				memset(buff, 0, sizeof(buff));
				GetModuleFileName((HINSTANCE)hModule, buff, MAX_PATH);
				m_Path.Append(buff);
				m_Path.Append("/\0",2);
				m_Path.ConsumeEnd(1);
			}
			break;

		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


extern "C"
{

char * memstr (const char * str1,const char * str2,int size)
{
		char *cp = (char *) str1;
        char *s1, *s2;

        if ( !*str2 )
            return((char *)str1);

		int sz1 = size;

        while (sz1)
        {
                s1 = cp;
                s2 = (char *) str2;
				int sz2 = sz1;

                while ( /**s1*/ sz2 && *s2 && tolower(*s1)==(*s2) )
                        s1++, s2++, sz2--;

                if (!*s2)
                        return(cp);

                cp++;
				sz1--;
        }

        return(NULL);
}


#define MIN(a, b)		((a)<(b)?(a):(b))
BOOL Replace(_Buffer *inb, /*_Buffer *outb, */char *from, int index)
{
	char *a = memstr(inb->m_buf, from, inb->Len());
	if (a)
	{
		_Buffer b1;
		b1.Append(inb->m_buf, a - inb->m_buf);
		char buff[1024];
		sprintf(buff, "<img src=\"%s%d\">", m_Path.Ptr(), index);
//		sprintf(buff, "<img src=\"%s%d\">", m_Path.Ptr(), 999);
		b1.Append(buff);
		b1.Append(a + strlen(from), inb->Len() - (a-inb->m_buf) - strlen(from));
		inb->Clear();
		inb->Append(b1.Ptr(), b1.Len());
		return TRUE;
	}
	
	return FALSE;
}

int __stdcall Transform(_Buffer *inb, /*_Buffer *outb, */BOOL IsHtml, BOOL CanPlay)
{
	BOOL didplayemoticonsound = FALSE;
	for (int i=0;i<TOTAL_EMOTICONS;i++)
	{
		while (Replace(inb, /*outb, */*m_NamesHtml[m_Index[i]]?m_NamesHtml[m_Index[i]]:m_NamesText[m_Index[i]], 200+m_Index[i]))
		{
/*			_Buffer b;
			if (Transform(outb, &b, IsHtml))
			{
				inb->Clear();
				inb->Append(outb->Ptr(), outb->Len());
				outb->Clear();
				outb->Append(b.Ptr(), b.Len());
			}
*/
			// let's see if we have sound for this too...
			if (!didplayemoticonsound && CanPlay)
			{
				HRSRC  h = FindResource((HMODULE)m_hInstance, MAKEINTRESOURCE(5200+m_Index[i]), "WAVE");
				if (h)
				{
					// let's play 
					int pl = PlaySound(MAKEINTRESOURCE(5200+m_Index[i]), (HMODULE)m_hInstance ,SND_RESOURCE | SND_ASYNC | SND_NODEFAULT | SND_NOWAIT);
					pl = pl;
					didplayemoticonsound = TRUE;
				}
			}
		}
	}
	return didplayemoticonsound;
}

int __stdcall Rollback(_Buffer *inb,/* _Buffer *outb, */BOOL IsHtml)
{
//	outb->Clear();
//	outb->Append(inb->Ptr(), inb->Len());
	return TRUE;
}

int __stdcall GetCount(void)
{
	return TOTAL_EMOTICONS;
}

int __stdcall GetItem(int Index, char **ResourceType, char **ImageType)
{
	if (Index<0 || Index>=TOTAL_EMOTICONS)
		return 0;

	if (ResourceType)
		*ResourceType = MAKEINTRESOURCE(2110);//"2110";

	if (ImageType)
		*ImageType = "GIF";

	return Index+200;
}
void __stdcall GetText(int Index, char **Text)
{
	if (Index<0 || Index>=TOTAL_EMOTICONS)
	{
		if (Text)
			*Text = NULL;
		return;
	}

	if (Text)
		*Text = m_NamesText[Index];
}
void __stdcall GetHtml(int Index, char **Text)
{
	if (Index<0 || Index>=TOTAL_EMOTICONS)
	{
		if (Text)
			*Text = NULL;
		return;
	}

	if (Text)
		*Text = *m_NamesHtml[Index]?m_NamesHtml[Index]:m_NamesText[Index];
}

}