//#include "if626bas.inc"
//#include "vsp626ste.inc"
//#include "if626max.inc"
#include <stdio.h>

short int timer(short int funktion, short int timer)
{
	printf("%s(%d)\n", __func__, __LINE__);
	return 0;
}

short int timer_2(short int funktion, short int timer, short int wert)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

short int ProgrammAktuell(void)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
   }

short int ProgrammWahl(void)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
   }

short int Zykluszeit(void)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
 }

short int Umlaufzeit(void)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;

}

int ProgrammWahlZentrale(void)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
  
}

short int d_imp(short int det)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
   
}

void d_limp(short int det)
{
printf("%s(%d)\n", __func__, __LINE__);
return ;
   
}

short int d_belga(short int det)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
   
}

short int d_stoer(short int det)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
  
}

short int d_belgg(short int det)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
   
}

short int d_blg(short int det)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
   
}

short int d_ztlkn(short int det)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
   
}

short int d_zeitlb(short int det)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

short int d_blgzt(short int det)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

short int d_impab(short int type, short int det)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

short int d_kvalue(short int det)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

short int min_rot(short int sg)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

short int u_rot_gelb(short int sg)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

short int min_gruen(short int sg)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

short int u_gelb(short int sg)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

void SG_ein(short int sg)
{
printf("%s(%d)\n", __func__, __LINE__);
return;
}

void SG_aus(short int sg)
{
printf("%s(%d)\n", __func__, __LINE__);
return;
}

void Relais_ein(short int sg)
{
printf("%s(%d)\n", __func__, __LINE__);
}

void Relais_aus(short int sg)
{
printf("%s(%d)\n", __func__, __LINE__);
   return;
}

void Blinker_ein(short int sg)
{
printf("%s(%d)\n", __func__, __LINE__);
   return ;
}

void Blinker_aus(short int sg)
{
printf("%s(%d)\n", __func__, __LINE__);
   return;
}

short int s_rot(short int sg)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

short int s_sr_aus(short int re)
{
printf("%s(%d)\n", __func__, __LINE__);
   return 0;
}
short int s_sb_aus(short int bli)
{
printf("%s(%d)\n", __func__, __LINE__);
   return 0;
}

short int s_min_rot(short int sg)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

short int s_gelb(short int sg)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

short int s_grun(short int sg)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

short int s_sr_ein(short int re)
{
printf("%s(%d)\n", __func__, __LINE__);
   return 0;
}

short int s_sb_ein(short int bli)
{
printf("%s(%d)\n", __func__, __LINE__);
   return 0;
}
short int s_min_grun(short int sg)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

short int s_vor(short int sg)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

short int s_stoeblink(short int sg)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

unsigned short s_t_rot(short int sg)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

unsigned short int s_t_gelb(short int sg)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

unsigned short int s_t_min_rot(short int sg)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

unsigned short int s_t_grun(short int sg)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

unsigned short int s_t_vor(short int sg)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

unsigned short int s_t_min_grun(short int sg)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

unsigned short int s_HW_VspFreigegeben(short int sg)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

short s_zwi_zeit(short sgR, short sgE)
{
printf("%s(%d)\n", __func__, __LINE__);
   return 32767;      /* Nicht Feindlich                */
}

void Meldung(short int degree, short int nr, short int par1, short int par2, short int par3, short int par4)
{
printf("%s(%d)\n", __func__, __LINE__);
return ;
}

void  MeldungNET(short int degree, unsigned char nr, unsigned short Anr, unsigned char par1, unsigned char par2, unsigned char par3, unsigned char par4, unsigned char par5)
{
printf("%s(%d)\n", __func__, __LINE__);
return ;
}

void U_Kontrolle(short int vs, short int zeit)
{
printf("%s(%d)\n", __func__, __LINE__);
return ;
}

short TelegrammVomGeraet(void* oev_tele_poi)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

unsigned short int s_SteuerungNichtAktiv(void)
{
printf("%s(%d)\n", __func__, __LINE__);
   return 0;
}

void* Allozieren_VSP_Speicher(int _sizeof, int id)
{
printf("%s(%d)\n", __func__, __LINE__);
return NULL ;
}

void* Gib_VSP_Zeiger(int id)
{
printf("%s(%d)\n", __func__, __LINE__);
return NULL;
}

void Freigeben_VSP_Speicher(int id)
{
printf("%s(%d)\n", __func__, __LINE__);
return ;
}

int Oeffnen_VSP_Parameter(void)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

int Read_VSP_Parameter(char* data, int _sizeof)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

void Schliessen_VSP_Parameter(void)
{
printf("%s(%d)\n", __func__, __LINE__);
return;
}

int Oeffnen_Sichern_Parameter(void)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

void Schreiben_Sichern_Parameter(char* data, int _sizeof)
{
printf("%s(%d)\n", __func__, __LINE__);
return ;
}

void Schliessen_Sichern_Parameter(void)
{
printf("%s(%d)\n", __func__, __LINE__);
return;
}

int Oeffnen_VSP_Befehle(void)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

int Lesen_VSP_Befehle(char* data, int _sizeof)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

void Schliessen_VSP_Befehle(void)
{
printf("%s(%d)\n", __func__, __LINE__);
return ;
}

int OePNV_Ein_Aus(void)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

int IV_Ein_Aus(void)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

int Det_Aktiv(int KanalNummer)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

int Sg_Aktiv(int KanalNummer)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

int AktuelleZeit(int* Stunde, int* Minute, int* Sekunde)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

int AktuellesDatum(int* Jahr, int* Monat, int* Tag, int* Wochentag)
{
printf("%s(%d)\n", __func__, __LINE__);
   return 0;
}

int Get_OCITOutstationId(int* ZNr, int* FNr, int* Realknoten)
{
printf("%s(%d)\n", __func__, __LINE__);
   return 0;
}

int Wunsch_VSPLUS(int Wunsch, int Teiknoten)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

int Neue_Befehle(void)
{
printf("%s(%d)\n", __func__, __LINE__);
   return 0;
}

void ZSondereingriffvn(unsigned char* Sondereingriff, unsigned long* EndZeitpunkt, unsigned long* VorgangsNummer)
{
printf("%s(%d)\n", __func__, __LINE__);
return;
}

unsigned long UTCZeitstempel(void)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}
