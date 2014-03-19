//#include "if626bas.inc"
//#include "vsp626ste.inc"
//#include "if626max.inc"
#include <stdio.h>
#include "tsc.h" //debug()

/* funktion:1=read;3=clear;4=stop and clear;5=stop
 * timer: timer index
 */
short int timer(short int funktion, short int timer)
{
	debug(3, "==>\n");
	return _timer(funktion, timer, 0);
}

/* load and start a timer
 * funktion=2;
 * timer: timer index
 * wert: initial counts
 */
short int timer_2(short int funktion, short int timer, short int wert)
{
	debug(3, "==>\n");
	return _timer(funktion, timer, wert);
}

/* VS-PLUS is told the actual program number
 * return value:   Actual program number 0 – 255
 */
short int ProgrammAktuell(void)
{
	debug(3, "==>\n");
	return tsc_prog_actual();
}

/* VS-PLUS is told a program change request. If there is no 
 * change request pending, the actual program is contained 
 * in the return value
 * return value:  Number of the new, not yet activated program 0 – 255
 */
short int ProgrammWahl(void)
{
	debug(3, "==>\n");
	return tsc_prog_select();
}

/* The current cycle second within the active signal program
 * return value:  Current cycle second in units of 100ms, rounded to seconds
 */
short int Zykluszeit(void)
{
	debug(3, "==>\n");
	return tsc_prog_tx();
}

/* The cycle time of the active signal program (in units of 100ms)
 * return value:  Cycle time in units of 100ms
 */
short int Umlaufzeit(void)
{
	debug(3, "==>\n");
	return tsc_prog_tu();
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

/* requests memory from the controller in order to 
 * save the VS-PLUS parameters
 *
 * return value: pointer to memory area
 * _sizeof: size of memory to be allocated
 * id: memory area identification (1 – 3)
 */
void* Allozieren_VSP_Speicher(int _sizeof, int id)
{
	debug(3, "==>\n");
	return tsc_alloc_mem(_sizeof, id) ;
}

/* returns the pointer to the memory area
 * return value:  pointer to memory area
 * id:  memory area identification (1 – 3)
 */
void* Gib_VSP_Zeiger(int id)
{
	debug(3, "==>\n");
	return tsc_get_mem(id);
}

/* frees memory that has been requested from the controller
 * return value:  none
 * id: memory area identification(1-3)
 */
void Freigeben_VSP_Speicher(int id)
{
	debug(3, "==>\n");
	tsc_free_mem(id);
	return;
}

/* opens the VCB file that has been sent to the controller
 * return value: 1 = supply file was opened successfully, 
 *				 0 = not opened
 */
int Oeffnen_VSP_Parameter(void)
{
	debug(3, "==>\n");
	return tsc_open_vcb();
}

/* Data is read sequentially from the formerly opened VCB 
 * file. This function corresponds to a "fread” in C. 
 * return value:  size of data read; -1 = error
 * data:  data read
 * sizeof:  size of data to be read 
 */
int Read_VSP_Parameter(char* data, int _sizeof)
{
	debug(3, "==>\n");
	return tsc_read_vcb(data, _sizeof);
}

/* closes the supply file
 * return value:  none
 */
void Schliessen_VSP_Parameter(void)
{
	debug(3, "==>\n");
	tsc_close_vcb();
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
