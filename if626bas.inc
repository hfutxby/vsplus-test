/*~-*/
/*~XSF_LANGUAGE: C/C++*/
/*~T*/
/*

              VV         VV     SSSSSS     PPPPPPPPP
              VV         VV   SSS    SSS   PP       PP
              VV         VV   SS      SS   PP       PPP
              VV         VV    SS          PP       PP
               VV       VV        SS       PPPPPPPPP
                VV     VV           SS     PP
                 VV   VV      SS      SS   PP
                  VV VV       SSS    SSS   PP
                   VVV          SSSSSS     PPLUS


              INCLUDE FILE INTERFACE BASIS zu VS-PLUS


       Filename       : if626bas.inc
       Version        : 6.2.6
       Datum          : 06.02.2014
       Programmierer  : P. Herren


       Inhalt :
       ~~~~~~~~
        In diesem File sind die geraetespezifischen Funktionsaufrufe vorhanden
*/
/*~I*/
#ifndef _if626bas
/*~T*/
#define _if626bas
/*~I*/
#ifdef _EMULATOR_
/*~T*/
#include "ovstgdek.h"
/*~-*/
#endif
/*~E*/
/*~I*/
#ifdef _SIEMENS_C900_
/*~T*/
#define VSP_NR         11  /* Siemens(WFi) Steuerverfahrens-Nummer      */
#define VSP_FEHLER_NR  99  /* Siemens(WFi) eigene Nummer fuer VSPLUS    */

/*~-*/
#endif
/*~E*/
/*~I*/
#ifndef OEV_MODUL
/*~T*/
#define OEV_MODUL    8
/*~-*/
#endif
/*~E*/
/*~A*/
/*~+:TIMERDEFINITIONEN (Timerbasis)*/
/*~T*/
#define VSP_T0      0u                           /* Basis 0 fuer Timer                     */
#define VSP_T1      VSP_T0+DETMAX                /* Basis 1 fuer Timer                     */
#define VSP_T2      VSP_T1+DETMAX                /* Basis 2 fuer Timer                     */
#define VSP_T3      VSP_T2+DETMAX                /* Basis 3 fuer Timer                     */
#define VSP_T4      VSP_T3+VSMAX                 /* Basis 4 fuer Timer                     */
#define VSP_T5      VSP_T4+VSMAX                 /* Basis 5 fuer Timer                     */
#define VSP_T6      VSP_T5+VSMAX                 /* Basis 6 fuer Timer                     */
#define VSP_T7      VSP_T6+VSMAX                 /* Basis 7 fuer Timer                     */
#define VSP_TE      VSP_T7+SPEZMAX               /* Letzter Timer (Basis 0-7)              */

/*~E*/
/*~A*/
/*~+:DATENTYPEN VS-PLUS*/
/*~K*/
/*~+:Variablentypen*/
/*~T*/
typedef         unsigned char  U_BYTE;  /*  8-Bit Variable  ohne V */
typedef         signed char    S_BYTE;  /*  8-Bit Variable  mit V  */

/*~I*/
#ifdef _EMULATOR_
/*~T*/
typedef         unsigned char  BYTE;    /* EMULATOR */

/*~-*/
#endif
/*~E*/
/*~T*/
#define US_WORD U_WORD                  /* 16-Bit Variable  ohne V */
#define SS_WORD S_WORD                  /* 16-Bit Variable  mit V  */

/*~T*/
typedef         unsigned short U_WORD;  /* 16-Bit Variable  ohne V */
typedef         signed short   S_WORD;  /* 16-Bit Variable  mit V  */

/*~T*/
#define UL_WORD U_WORD                  /* 16-Bit Variable  ohne V */
#define SL_WORD S_WORD                  /* 16-Bit Variable  mit V  */

/*~I*/
#ifdef _EMULATOR_
/*~T*/
#define WORD    U_WORD                  /* 16-Bit Variable  ohne V */
/* #define MAXWORD 0xFFFF */

/*~-*/
#endif
/*~E*/
/*~T*/
typedef         unsigned long  U_LONG;  /* 32-Bit Variable  ohne V */
typedef         signed long    S_LONG;  /* 32-Bit Variable  mit V  */

/*~I*/
#ifdef _EMULATOR_
/*~T*/
typedef         unsigned long  ULONG;   /* EMULATOR */

/*~-*/
#endif
/*~E*/
/*~K*/
/*~+:Logisch*/
/*~+:*/
/*~T*/
typedef enum {L_falsch, L_richtig} Logisch;

/*~K*/
/*~+:Zeit*/
/*~+:*/
/*~T*/
typedef US_WORD Zeit;
typedef SS_WORD DZeit;
typedef S_WORD  LZeit;
/*~E*/
/*~A*/
/*~+:PROTOTYPEN*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Prototypen für Funktionen in der Simulation*/
/*~T*/
extern void  Meldung (short int nr, short int par1, short int par2, short int par3, short int par4);
extern void  MeldungNET(unsigned char nr, unsigned short Anr, unsigned char par1, unsigned char par2, unsigned char par3, unsigned char par4, unsigned char par5);
extern void  ZwangsAB (int VS, int type, int tx, int prg);
extern int   OePNV_Ein_Aus(void);
extern void  OePNV_Ein_Aus_Ist (int OV_Bevorzugung);
extern int   IV_Ein_Aus(void);
extern void  IV_Ein_Aus_Ist (int IV_Bevorzugung);
extern int   Det_Aktiv(int KanalNummer);
extern int   Sg_Aktiv(int KanalNummer);
extern int   AktuelleZeit(int* Stunde, int* Minute, int* Sekunde);
extern int   AktuellesDatum(int* Jahr, int* Monat, int* Tag, int* Wochentag);
extern int   Get_OCITOutstationId(int* ZNr, int* FNr, int* Realknoten);
extern int   ProgrammWahlZentrale(void);
extern int   Neue_Befehle(void);
extern int   Oeffnen_VSP_Befehle(void);
extern int   Lesen_VSP_Befehle(char* data, int _sizeof);
extern void  Schliessen_VSP_Befehle(void);
extern int   Wunsch_VSPLUS(int Wunsch, int Teiknoten);
extern void* Allozieren_VSP_Speicher(int _sizeof, int ID);
extern void* Gib_VSP_Zeiger(int ID);
extern int   Oeffnen_VSP_Parameter(void);
extern int   Read_VSP_Parameter(char* data, int _sizeof);
extern void  Schliessen_VSP_Parameter(void);
extern int   Oeffnen_Sichern_Parameter(void);
extern void  Schreiben_Sichern_Parameter(char* data, int _sizeof);
extern void  Schliessen_Sichern_Parameter(void);
extern void  Freigeben_VSP_Speicher(int ID);

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergerät*/
/*~K*/
/*~+:Programmfunktionen*/
/*~T*/
short int ProgrammAktuell(void);
short int ProgrammWahl(void);
short int Zykluszeit(void);
short int Umlaufzeit(void);
int ProgrammWahlZentrale(void);

/*~K*/
/*~+:Detektorfunktionen*/
/*~T*/
short int d_imp(short int det);
void d_limp(short int det);
short int d_belga(short int det);
short int d_stoer(short int det);
short int d_belgg(short int det);
short int d_blg(short int det);
short int d_ztlkn(short int det);
short int d_blgzt(short int det);
short int d_impab(short int type, short int det);
short int d_kvalue(short int det);

/*~K*/
/*~+:Signalgruppenfunktionen*/
/*~T*/
short int min_rot(short int sg);
short int u_rot_gelb(short int sg);
short int min_gruen(short int sg);
short int u_gelb(short int sg);
void SG_ein(short int sg);
void SG_aus(short int sg);
void Relais_ein(short int sg);
void Relais_aus(short int sg);
void Blinker_ein(short int sg);
void Blinker_aus(short int sg);
short int s_rot(short int sg);
short int s_sr_aus(short int re);
short int s_sb_aus(short int bli);
short int s_min_rot(short int sg);
short int s_gelb(short int sg);
short int s_grun(short int sg);
short int s_sr_ein(short int re);
short int s_sb_ein(short int bli);
short int s_min_grun(short int sg);
short int s_vor(short int sg);
short int s_blink(short int sg);
unsigned short s_t_rot(short int sg);
unsigned short int s_t_gelb(short int sg);
unsigned short int s_t_min_rot(short int sg);
unsigned short int s_t_grun(short int sg);
unsigned short int s_t_vor(short int sg);
unsigned short int s_t_min_grun(short int sg);
unsigned short int s_HW_VspFreigegeben(short int sg);
short int s_zzV(short sgr, short sge);

/*~K*/
/*~+:Fehlermeldungen*/
/*~T*/
void Meldung(short int degree, short int nr, short int par1, short int par2, short int par3, short int par4);
void  MeldungNET(short int degree, unsigned char nr, unsigned short Anr, unsigned char par1, unsigned char par2, unsigned char par3, unsigned char par4, unsigned char par5);
void U_Kontrolle(short int vs, short int zeit);

/*~K*/
/*~+:Systemfunktionen*/
/*~T*/
short TelegrammVomGeraet(void* oev_tele_poi);
unsigned short int s_SteuerungNichtAktiv(void);

/*~K*/
/*~+:Datenhandling*/
/*~T*/
void* Allozieren_VSP_Speicher(int _sizeof, int id);
void* Gib_VSP_Zeiger(int id);
void Freigeben_VSP_Speicher(int id);
int Oeffnen_VSP_Parameter(void);
int Read_VSP_Parameter(char* data, int _sizeof);
void Schliessen_VSP_Parameter(void);
int Oeffnen_Sichern_Parameter(void);
void Schreiben_Sichern_Parameter(char* data, int _sizeof);
void Schliessen_Sichern_Parameter(void);
int Oeffnen_VSP_Befehle(void);
int Lesen_VSP_Befehle(char* data, int _sizeof);
void Schliessen_VSP_Befehle(void);

/*~K*/
/*~+:OCIT Funktionen*/
/*~T*/
int OePNV_Ein_Aus(void);
int IV_Ein_Aus(void);
int Det_Aktiv(int KanalNummer);
int Sg_Aktiv(int KanalNummer);
int AktuelleZeit(int* Stunde, int* Minute, int* Sekunde);
int AktuellesDatum(int* Jahr, int* Monat, int* Tag, int* Wochentag);
int Get_OCITOutstationId(int* ZNr, int* FNr, int* Realknoten);
int Wunsch_VSPLUS(int Wunsch, int Teiknoten);
int Neue_Befehle(void);
void ZSondereingriffvn(unsigned char* Sondereingriff, unsigned long* EndZeitpunkt, unsigned long* VorgangsNummer);
unsigned long UTCZeitstempel(void);

/*~E*/
/*~-*/
#endif
/*~E*/
/*~A*/
/*~+:Prototypen für Funktionen im VS-PLUS*/
/*~T*/
int           l_Det_Wert(int KanalNummer, int Typ);
int           m_Prog_Schaltung_erlaubt(void);
int           l_Prog_VSP(int ProgNummer);
unsigned long l_V_Build_Nr(void);
int           f_Initial_VSP_Parameter(void);
int           f_Pruefen_VSP_Parameter(void);
int           f_Lesen_VSP_Parameter(void);
void          f_Ende_VSP_Parameter(void);
int           l_VSP_X_Ein_Aus_Ist(int T);
char*         l_VABefehlPfad(void);

/*~E*/
/*~E*/
/*~A*/
/*~+:Herstellerspezifisch*/
/*~A*/
/*~+:Siemens C900*/
/*~I*/
#ifdef _SIEMENS_C900_
/*~K*/
/*~+:Beispiel für "typdef struct" (GNU-Compiler Siemens)*/
/*~+:"typedef _PACKED_V_ struct _PACKED_N_" --> "struct __attribute__((packed))"*/
/*~+:*/
/*~T*/
#define  _PACKED_V_     
#define  _PACKED_N_     __attribute__((packed))

/*~O*/
/*~-*/
#else
/*~T*/
#define  _PACKED_V_
#define  _PACKED_N_

/*~-*/
#endif
/*~E*/
/*~T*/

/*~E*/
/*~E*/
/*~A*/
/*~+:DEFINITIONEN OeV-Modul*/
/*~T*/
#define OMAXBYTE                 255  /* bei unsigned char          */
#define OMAXWORD              65535U  /* bei unsigned short (2Byte) */

/*~K*/
/*~+:Sprache fuer OeV-Modul*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~T*/
#define SYSTEMLAGUAGE    49
/*~O*/
/*~-*/
#else
/*~T*/
#define SYSTEMLAGUAGE    49
/*~-*/
#endif
/*~E*/
/*~T*/

/*~E*/
/*~A*/
/*~+:DEFINITIONEN Teilknoten*/
/*~I*/
#ifdef _VSPLUS_SIM_ 
/*~T*/
#define GERAET_TEILKNOTEN_MAX       1
/*~O*/
/*~-*/
#else
/*~T*/
#define GERAET_TEILKNOTEN_MAX       4
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:TIMER*/
/*~A*/
/*~+:Detektorwartezeit*/
/*~K*/
/*~+:Detektorwartezeit*/
/*~I*/
#ifdef _VSPLUS_SIM_   
/*~A*/
/*~+:Simulation*/
/*~T*/
#define t_twdet(x)      timer(1,x+(VSP_T0),0) /* -lesen               */
#define m_stwdet(x,k)   timer(2,x+(VSP_T0),k) /* -laden+starten mit k */
#define m_ltwdet(x)     timer(3,x+(VSP_T0),0) /* -loeschen            */
#define m_altwdet(x)    timer(4,x+(VSP_T0),0) /* -anhalten,loeschen   */
#define m_atwdet(x)     timer(5,x+(VSP_T0),0) /* -anhalten            */

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define t_twdet(x)      timer(1,x+(VSP_T0))     /* -lesen               */
#define m_stwdet(x,k)   timer_2(2,x+(VSP_T0),k) /* -laden+starten mit k */
#define m_ltwdet(x)     timer(3,x+(VSP_T0))     /* -loeschen            */
#define m_altwdet(x)    timer(4,x+(VSP_T0))     /* -anhalten,loeschen   */
#define m_atwdet(x)     timer(5,x+(VSP_T0))     /* -anhalten            */

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Detektorwartezeit 1. Folgezug*/
/*~K*/
/*~+:Detektorwartezeit 1. Folgezug*/
/*~I*/
#ifdef _VSPLUS_SIM_   
/*~A*/
/*~+:Simulation*/
/*~T*/
#define t_twdet1(x)     timer(1,x+(VSP_T1),0) /* -lesen               */
#define m_stwdet1(x,k)  timer(2,x+(VSP_T1),k) /* -laden+starten mit k */
#define m_ltwdet1(x)    timer(3,x+(VSP_T1),0) /* -loeschen            */
#define m_altwdet1(x)   timer(4,x+(VSP_T1),0) /* -anhalten,loeschen   */
#define m_atwdet1(x)    timer(5,x+(VSP_T1),0) /* -anhalten            */

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define t_twdet1(x)     timer(1,x+(VSP_T1))     /* -lesen               */
#define m_stwdet1(x,k)  timer_2(2,x+(VSP_T1),k) /* -laden+starten mit k */
#define m_ltwdet1(x)    timer(3,x+(VSP_T1))     /* -loeschen            */
#define m_altwdet1(x)   timer(4,x+(VSP_T1))     /* -anhalten,loeschen   */
#define m_atwdet1(x)    timer(5,x+(VSP_T1))     /* -anhalten            */

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Detektorwartezeit 2. Folgezug*/
/*~K*/
/*~+:Detektorwartezeit 2. Folgezug*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
#define t_twdet2(x)     timer(1,x+(VSP_T2),0) /* -lesen               */
#define m_stwdet2(x,k)  timer(2,x+(VSP_T2),k) /* -laden+starten mit k */
#define m_ltwdet2(x)    timer(3,x+(VSP_T2),0) /* -loeschen            */
#define m_altwdet2(x)   timer(4,x+(VSP_T2),0) /* -anhalten,loeschen   */
#define m_atwdet2(x)    timer(5,x+(VSP_T2),0) /* -anhalten            */

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define t_twdet2(x)     timer(1,x+(VSP_T2))     /* -lesen               */
#define m_stwdet2(x,k)  timer_2(2,x+(VSP_T2),k) /* -laden+starten mit k */
#define m_ltwdet2(x)    timer(3,x+(VSP_T2))     /* -loeschen            */
#define m_altwdet2(x)   timer(4,x+(VSP_T2))     /* -anhalten,loeschen   */
#define m_atwdet2(x)    timer(5,x+(VSP_T2))     /* -anhalten            */

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Verkehrsstromwartezeit*/
/*~K*/
/*~+:Verkehrsstromwartezeit*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
#define t_twvs(y)       timer(1,y+(VSP_T3),0) /* -lesen               */
#define m_stwvs(y,k)    timer(2,y+(VSP_T3),k) /* -laden+starten mit k */
#define m_ltwvs(y)      timer(3,y+(VSP_T3),0) /* -loeschen            */
#define m_altwvs(y)     timer(4,y+(VSP_T3),0) /* -anhalten+loeschen   */
#define m_atwvs(y)      timer(5,y+(VSP_T3),0) /* -anhalten            */

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define t_twvs(y)       timer(1,y+(VSP_T3))     /* -lesen               */
#define m_stwvs(y,k)    timer_2(2,y+(VSP_T3),k) /* -laden+starten mit k */
#define m_ltwvs(y)      timer(3,y+(VSP_T3))     /* -loeschen            */
#define m_altwvs(y)     timer(4,y+(VSP_T3))     /* -anhalten,loeschen   */
#define m_atwvs(y)      timer(5,y+(VSP_T3))     /* -anhalten            */

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Farbenzeit Zaehler fuer ANZ ohne Zeiten aus der Basis*/
/*~K*/
/*~+:Farbenzeit Zaehler fuer ANZ ohne Zeiten aus der Basis*/
/*~I*/
#ifdef _VSPLUS_SIM_ 
/*~A*/
/*~+:Simulation*/
/*~T*/
#define t_anzt(y)       timer(1,y+(VSP_T4),0) /* -lesen               */
#define m_sanzt(y,k)    timer(2,y+(VSP_T4),k) /* -laden+starten mit k */
#define m_lanzt(y)      timer(3,y+(VSP_T4),0) /* -loeschen            */
#define m_alanzt(y)     timer(4,y+(VSP_T4),0) /* -anhalten+loeschen   */
#define m_aanzt(y)      timer(5,y+(VSP_T4),0) /* -anhalten            */

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define t_anzt(y)       timer(1,y+(VSP_T4))     /* -lesen               */
#define m_sanzt(y,k)    timer_2(2,y+(VSP_T4),k) /* -laden+starten mit k */
#define m_lanzt(y)      timer(3,y+(VSP_T4))     /* -loeschen            */
#define m_alanzt(y)     timer(4,y+(VSP_T4))     /* -anhalten,loeschen   */
#define m_aanzt(y)      timer(5,y+(VSP_T4))     /* -anhalten            */

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Signalzeitzaehler (Rot/Gruen) fuer VS*/
/*~K*/
/*~+:Signalzeitzaehler (Rot/Gruen) fuer VS*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
#define t_vst(y)        timer(1,y+(VSP_T5),0) /* -lesen               */
#define m_svst(y,k)     timer(2,y+(VSP_T5),k) /* -laden+starten mit k */
#define m_lvst(y)       timer(3,y+(VSP_T5),0) /* -loeschen            */
#define m_alvst(y)      timer(4,y+(VSP_T5),0) /* -anhalten+loeschen   */
#define m_avst(y)       timer(5,y+(VSP_T5),0) /* -anhalten            */

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define t_vst(y)        timer(1,y+(VSP_T5))     /* -lesen               */
#define m_svst(y,k)     timer_2(2,y+(VSP_T5),k) /* -laden+starten mit k */
#define m_lvst(y)       timer(3,y+(VSP_T5))     /* -loeschen            */
#define m_alvst(y)      timer(4,y+(VSP_T5))     /* -anhalten,loeschen   */
#define m_avst(y)       timer(5,y+(VSP_T5))     /* -anhalten            */

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Signalzeitzaehler (Vorbereitung/Gelb) fuer VS*/
/*~K*/
/*~+:Signalzeitzaehler (Vorbereitung/Gelb) fuer VS*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
#define t_vsvort(y)     timer(1,y+(VSP_T6),0) /* -lesen               */
#define m_svsvort(y,k)  timer(2,y+(VSP_T6),k) /* -laden+starten mit k */
#define m_lvsvort(y)    timer(3,y+(VSP_T6),0) /* -loeschen            */
#define m_alvsvort(y)   timer(4,y+(VSP_T6),0) /* -anhalten+loeschen   */
#define m_avsvort(y)    timer(5,y+(VSP_T6),0) /* -anhalten            */

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define t_vsvort(y)     timer(1,y+(VSP_T6))     /* -lesen               */
#define m_svsvort(y,k)  timer_2(2,y+(VSP_T6),k) /* -laden+starten mit k */
#define m_lvsvort(y)    timer(3,y+(VSP_T6))     /* -loeschen            */
#define m_alvsvort(y)   timer(4,y+(VSP_T6))     /* -anhalten,loeschen   */
#define m_avsvort(y)    timer(5,y+(VSP_T6))     /* -anhalten            */

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Spezial-Timer (f.Sonderfunktionen)*/
/*~K*/
/*~+:Spezial-Timer (f.Sonderfunktionen)*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
#define t_spezt(y)      timer(1,y+(VSP_T7),0) /* -lesen               */
#define m_sspezt(y,k)   timer(2,y+(VSP_T7),k) /* -laden+starten mit k */
#define m_lspezt(y)     timer(3,y+(VSP_T7),0) /* -loeschen            */
#define m_alspezt(y)    timer(4,y+(VSP_T7),0) /* -anhalten+loeschen   */
#define m_aspezt(y)     timer(5,y+(VSP_T7),0) /* -anhalten            */

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define t_spezt(y)      timer(1,y+(VSP_T7))      /* -lesen               */
#define m_sspezt(y,k)   timer_2(2,y+(VSP_T7),k)  /* -laden+starten mit k */
#define m_lspezt(y)     timer(3,y+(VSP_T7))      /* -loeschen            */
#define m_alspezt(y)    timer(4,y+(VSP_T7))      /* -anhalten,loeschen   */
#define m_aspezt(y)     timer(5,y+(VSP_T7))      /* -anhalten            */

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:BASISFUNKTIONEN*/
/*~K*/
/*~+:BASISFUNKTIONEN*/
/*~A*/
/*~+:Programmfunktionen*/
/*~K*/
/*~+:Programmfunktionen*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
/* aktuelle Signalprogramm NR        */
#define f_prg()             ProgrammAktuell()
/* angewaehlte Signalprogramm NR      */
#define f_prgwl()           ProgrammWahl()

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergerät*/
/*~T*/
/* aktuelle Signalprogramm NR        */
#define f_prg()             ProgrammAktuell()
/* angewaehlte Signalprogramm NR     */
#define f_prgwl()           ProgrammWahl()
 
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Umlaufkontrolle*/
/*~K*/
/*~+:Umlaufkontrolle*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
#define f_ur(vs,zeit)            U_Kontrolle(vs, zeit)
/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergerät*/
/*~T*/
#define f_ur(vs,zeit)            U_Kontrolle(vs,zeit)
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Rahmenplanfunktionen*/
/*~K*/
/*~+:Rahmenplanfunktionen*/
/*~I*/
#ifdef _VSPLUS_SIM_ 
/*~A*/
/*~+:Simulation*/
/*~T*/
/* aktuelle Zykluszeit    */
#define l_zytim()        Zykluszeit()
/* aktuelle Umlaufzeit    */
#define l_umzt()         Umlaufzeit()

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergerät*/
/*~T*/
/* aktuelle Zykluszeit    */
#define l_zytim()        Zykluszeit()
/* aktuelle Umlaufzeit    */
#define l_umzt()         Umlaufzeit()
 
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Detektorfunktionen*/
/*~A*/
/*~+:Detektorfunktionen*/
/*~K*/
/*~+:Detektorfunktionen*/
/*~I*/
#ifdef _VSPLUS_SIM_
/*~A*/
/*~+:Simulation*/
/*~T*/
/* Impulsspeicher (ZS-Wert) Summe */
#define l_imp(x)        d_imp(x)
/* Impulsspeicher (ZS-Wert) reset */
#define m_limp(x)       d_limp(x)
/* Impulsspeicher (SS-Wert) Summe */
#define l_impss(x)      (0)
/* Impulsspeicher (SS-Wert) reset */
#define m_limpss(x)     (0)
/* Abfallende Imp.Flanke Summe  */
#define l_impab(x)      d_impab(1,x)
/* Abfallende Imp.Flanke Summe Reset */
#define m_limpab(x)     d_impab(2,x)

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
/* Impulsspeicher (ZS-Wert) Summe */
#define l_imp(x)        d_imp(x)
/* Impulsspeicher (ZS-Wert) reset */
#define m_limp(x)       d_limp(x)
/* Impulsspeicher (SS-Wert) Summe */
#define l_impss(x)      0
/* Impulsspeicher (SS-Wert) reset */
#define m_limpss(x)     0
/* Abfallende Imp.Flanke Summe  */
#define l_impab(x)      d_impab(1,x)
/* Abfallende Imp.Flanke Summe Reset */
#define m_limpab(x)     d_impab(2,x)

/*~E*/
/*~-*/
#endif
/*~E*/
/*~I*/
#ifdef _VSPLUS_SIM_
/*~A*/
/*~+:Simulation*/
/*~T*/
/* Bel.grad aktuell    (in %!)   */
#define l_belga(x)      d_belga(x)
/* Bel.grad geglaettet (in %!)   */
#define l_belgg(x)      d_belgg(x)

/*~K*/
/*~+:KWert aus VL (GPS Daten in %)*/
/*~T*/
#define l_kvalue(x)     d_kvalue(x)
/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~K*/
/*~+:Bel.grad aktuell    (in %!)*/
/*~T*/
#define l_belga(x)      d_belga(x)
/* Bel.grad geglaettet (in %!) */
#define l_belgg(x)      d_belgg(x)
/*~K*/
/*~+:KWert aus VL (GPS Daten in %)*/
/*~T*/
#define l_kvalue(x)     d_kvalue(x)
/*~E*/
/*~-*/
#endif
/*~E*/
/*~I*/
#ifdef _VSPLUS_SIM_
/*~A*/
/*~+:Simulation*/
/*~T*/
/* Belegungszust. akt.          */
#define l_belza(x)      d_blg(x)               

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
/* Belegungszust. akt. */
#define l_belza(x)      d_blg(x)
/*~E*/
/*~-*/
#endif
/*~E*/
/*~I*/
#ifdef _VSPLUS_SIM_
/*~A*/
/*~+:Simulation*/
/*~T*/
/* Nettozeitlücke in 1/10 s     */
#define t_zeitln(x)     d_ztlkn(x)             
/* Bruttozeitlücke in 1/10 s    */
#define t_zeitlb(x)     (0)                    

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
/* Nettozeitluecke  in 1/10 s */
#define t_zeitln(x)     d_ztlkn(x)
/* Bruttozeitluecke in 1/10 s */
#define t_zeitlb(x)     d_zeitlb(x)

/*~E*/
/*~-*/
#endif
/*~E*/
/*~I*/
#ifdef _VSPLUS_SIM_
/*~A*/
/*~+:Simulation*/
/*~T*/
/* Belegungsdauer in 1/10 s     */
#define t_belzt(x)      d_blgzt(x)             

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
/* Belegungsdauer in 1/10 s     */
#define t_belzt(x)      d_blgzt(x)             

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Detektorstoerung der Hardware*/
/*~K*/
/*~+:Detektorstoerung der Hardware*/
/*~I*/
#ifdef _VSPLUS_SIM_
/*~A*/
/*~+:Simulation*/
/*~T*/
/* Detektorstoerung der Hardware */
#define l_stoer(x)      d_stoer(x)
/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergerät*/
/*~T*/
/* Detektorstoerung der Hardware */
#define l_stoer(x)      d_stoer(x)

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:Signalgruppenfunktionen*/
/*~A*/
/*~+:Basiswerte*/
/*~K*/
/*~+:Basiswerte*/
/*~I*/
#ifdef _VSPLUS_SIM_ 
/*~A*/
/*~+:Simulation*/
/*~T*/
/* Mindestrot:               */
#define t_min_rot(x)         min_rot(x)
/* Vorbereitungszeit:        */
#define t_vor(x)             u_rot_gelb(x)
/* Mindestgrün:              */
#define t_min_grun(x)        min_gruen(x)
/* Gelbzeit:                 */
#define t_gelb(x)            u_gelb(x)

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergerät*/
/*~T*/
/* Mindestrot:               */
#define t_min_rot(x)          min_rot(x)
/* Vorbereitungszeit:        */
#define t_vor(x)              u_rot_gelb(x)
/* Mindestgruen:              */
#define t_min_grun(x)         min_gruen(x)
/* Gelbzeit:                 */
#define t_gelb(x)             u_gelb(x)

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Schaltbefehle: Farbbetrieb*/
/*~K*/
/*~+:Schaltbefehle: Farbbetrieb*/
/*~I*/
#ifdef _VSPLUS_SIM_ 
/*~A*/
/*~+:Simulation*/
/*~T*/
/* Signal ein                            */
#define EIN_Signal(x)    SG_ein(x)         
/* Signal aus                            */
#define AUS_Signal(x)    SG_aus(x)         
/* AIS ein                               */
#define EIN_SState(x)    Relais_ein(x)         
/* AIS aus                               */
#define AUS_SState(x)    Relais_aus(x)         
/* Schutzblinker ein                     */
#define EIN_SBlink(x)    Blinker_ein(x)         
/* Schutzblinker aus                     */
#define AUS_SBlink(x)    Blinker_aus(x)         

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergerät*/
/*~T*/
/* Signal ein                            */
#define EIN_Signal(x)    SG_ein(x)
/* Signal aus                            */
#define AUS_Signal(x)    SG_aus(x)
/* AIS ein                               */
#define EIN_SState(x)    Relais_ein(x)
/* AIS aus                               */
#define AUS_SState(x)    Relais_aus(x)
/* Schutzblinker ein                     */
#define EIN_SBlink(x)    Blinker_ein(x)
/* Schutzblinker aus                     */
#define AUS_SBlink(x)    Blinker_aus(x)

/*~E*/
/*~-*/
#endif
/*~E*/
/*~A*/
/*~+:Schaltbefehle: Sonderbetrieb, Diese Einstellungen gelten sowohl fuer das Geraet und die Simulation*/
/*~T*/
#define Signal_Dunkel(x)     0                  /* Signalgruppe auf Dunkel schalten      */
#define Signal_Blinken(x)    0                  /* Signalgruppe auf Blinken schalten     */
#define Signal_FarbeRot(x)   0                  /* Signalgruppe auf neu Rot schalten     */
#define Signal_FarbeGruen(x) 0                  /* Signalgruppe auf neu Grün schalten    */

/*~E*/
/*~E*/
/*~A*/
/*~+:Aktuelle Zustaende*/
/*~K*/
/*~+:Aktuelle Zustaende*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
/* IST Zustand Rot bestimmen:            */
#define HW_rot(x)           s_rot(x)
/* IST Zustand Rot bestimmen:            */
#define HW_sr_aus(x)        s_sr_aus(x)
/* IST Zustand Rot bestimmen:            */
#define HW_sb_aus(x)        s_sb_aus(x)
/* Mindestrot abgelaufen:                 */
#define HW_min_rot(x)       s_min_rot(x)
/* Innerhalb Gelb:                       */
#define HW_gelb(x)          s_gelb(x)
/* IST Zustand Grün bestimmen:           */
#define HW_grun(x)          s_grun(x)
/* IST Zustand Grün bestimmen:           */
#define HW_sr_ein(x)        s_sr_ein(x)
/* IST Zustand Grün bestimmen:           */
#define HW_sb_ein(x)        s_sb_ein(x)
/* Mindestgrün abgelaufen:                 */
#define HW_min_grun(x)      s_min_grun(x)
/* Innerhalb Vorbereitung:               */
#define HW_vor(x)           s_vor(x)

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergerät*/
/*~T*/
/* IST Zustand Rot bestimmen:            */
#define HW_rot(x)           s_rot(x)
/* IST Zustand Rot bestimmen:            */
#define HW_sr_aus(x)        s_sr_aus(x)
/* IST Zustand Rot bestimmen:            */
#define HW_sb_aus(x)        s_sb_aus(x)
/* Mindestrot abgelaufen:                */
#define HW_min_rot(x)       s_min_rot(x)
/* Innerhalb Gelb:                       */
#define HW_gelb(x)          s_gelb(x)
/* IST Zustand Gruen bestimmen:          */
#define HW_grun(x)          s_grun(x)
/* IST Zustand Gruen bestimmen:          */
#define HW_sr_ein(x)        s_sr_ein(x)
/* IST Zustand Gruen bestimmen:          */
#define HW_sb_ein(x)        s_sb_ein(x)
/* Mindestgruen abgelaufen:              */
#define HW_min_grun(x)      s_min_grun(x)
/* Innerhalb Vorbereitung:               */
#define HW_vor(x)           s_vor(x)

/*~E*/
/*~-*/
#endif
/*~E*/
/*~A*/
/*~+:Abfragen der Schaltbefehle, diese Einstellungen gelten sowohl fuer das Geraet und die Simulation*/
/*~T*/
/* Bestimmen ob ROT Befehl ansteht:      */
#define HW_bef_rot(x)       1
/* Bestimmen ob GRUEN Befehl ansteht:    */
#define HW_bef_grun(x)      1

/*~E*/
/*~E*/
/*~A*/
/*~+:Aktuelle Zeiten*/
/*~K*/
/*~+:Aktuelle Zeiten*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
/* aktuelle Rotdauer:                     */
#define HW_t_rot(x)          s_t_rot(x)
/* aktuelle Gelbdauer:                    */
#define HW_t_gelb(x)         s_t_gelb(x)
/* aktuelle Mindestrotdauer:              */
#define HW_t_min_rot(x)      s_t_min_rot(x)
/* aktuelle Gründauer:                    */
#define HW_t_grun(x)         s_t_grun(x)
/* aktuelle Vorbereitungsdauer:           */
#define HW_t_vor(x)          s_t_vor(x)
/* aktuelle Mindestgründauer:             */
#define HW_t_min_grun(x)     s_t_min_grun(x)

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergerät*/
/*~T*/
/* aktuelle Rotdauer:                     */
#define HW_t_rot(x)          s_t_rot(x)
/* aktuelle Gelbdauer:                    */
#define HW_t_gelb(x)         s_t_gelb(x)
/* aktuelle Mindestrotdauer:              */
#define HW_t_min_rot(x)      s_t_min_rot(x)
/* aktuelle Gruendauer:                   */
#define HW_t_grun(x)         s_t_grun(x)
/* aktuelle Vorbereitungsdauer:           */
#define HW_t_vor(x)          s_t_vor(x)
/* aktuelle Mindestgruendauer:            */
#define HW_t_min_grun(x)     s_t_min_grun(x)

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Sonderzustaende*/
/*~K*/
/*~+:Sonderzustaende*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
/* IST Zustand Störungsblinken:    */
#define HW_blink(x)            0
/* aktuelle Störungsblinkdauer:    */
#define HW_t_blink(x)          _
/* IST Zustand Dunkel:             */
#define HW_Dunkel(x)           0
/* IST Zustand Blinken:            */
#define HW_Blinkend(x)         0
/* IST SG für VS-PLUS freigegeben  */
#define HW_VspFreigegeben(x)   s_HW_VspFreigegeben(x)

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergerät*/
/*~T*/
/* IST Zustand Stoerungsblinken:   */
#define HW_blink(x)            s_blink(x)
/* IST Zustand Dunkel:             */
#define HW_Dunkel(x)           0
/* IST Zustand Blinken:            */
#define HW_Blinkend(x)         0
/* IST SG fuer VS-PLUS freigegeben */
#define HW_VspFreigegeben(x)   s_HW_VspFreigegeben(x)

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Zugriff auf verkuerzte Zwischenzeit*/
/*~K*/
/*~+:Zugriff auf verkuerzte Zwischenzeit*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
/* Zwischenzeit lesen aus dem Gerät */
#define l_zzV(x,y)           pANZE_Zwizt_V[x-1].MElement[y-1]
/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergerät*/
/*~T*/
/* Zwischenzeit lesen aus dem Geraet */
#define l_zzV(x,y)           s_zzV(x,y)
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:Fehlermeldungen*/
/*~I*/
#ifdef _VSPLUS_SIM_
/*~A*/
/*~+:Simulation*/
/*~T*/
#define MELDUNG(degree,nr,par1,par2,par3,par4)                  Meldung(nr, par1, par2, par3, par4)

/*~T*/
#define MELDUNGnet(degree,nr,Anr,par1,par2,par3,par4,par5)      MeldungNET(nr, Anr, par1, par2, par3, par4, par5)

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define MELDUNG(degree,nr,par1,par2,par3,par4)                  Meldung(degree, nr, par1, par2, par3, par4)

/*~T*/
#define MELDUNGnet(degree,nr,Anr,par1,par2,par3,par4,par5)      MeldungNET(degree, nr, Anr,  par1, par2, par3, par4, par5)

/*~T*/


/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Systemfunktionen*/
/*~A*/
/*~+:Aus bzw. Stoerung abfragen*/
/*~K*/
/*~+:Aus bzw. Stoerung abfragen*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
/*
Die Funktion liefert 0 solange ein Signalplan abgearbeitet wird. Im Auszustand, oder waehrend der Ein- und Auschaltbilder muss diese Funktion 1 liefern.
*/
/*~T*/
#define SteuerungNichtAktiv()      s_SteuerungNichtAktiv()

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~K*/
/*~+:Die Funktion liefert 0 solange ein Signalplan abgearbeitet wird. Im Auszustand, oder waehrend der Ein- und Auschaltbilder muss diese Funktion 1 liefern*/
/*~+:*/
/*~T*/
#define SteuerungNichtAktiv()    s_SteuerungNichtAktiv()
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Empfangenes Telegramm uebergeben*/
/*~K*/
/*~+:Empfangenes Telegramm uebergeben*/
/*~I*/
#ifdef _VSPLUS_SIM_ 
/*~A*/
/*~+:Simulation*/
/*~T*/
#define GibTelegramm(x)            TelegrammVomGeraet(x)

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define GibTelegramm(x)        TelegrammVomGeraet(x)
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Ausschalten von VS-PLUS*/
/*~K*/
/*~+:Ausschalten von VS-PLUS*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
#define m_Wunsch_VSPLUS(x,y)            Wunsch_VSPLUS(x,y)

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define m_Wunsch_VSPLUS(x,y)            Wunsch_VSPLUS(x,y) 
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Programmschaltungen verzoegern (wird von Adaptivsteuererung "Vmod" gesetzt)*/
/*~K*/
/*~+:Programmschaltungen verzoegern (wird von Adaptivsteuererung "Vmod" gesetzt)*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
#define Prog_Schaltung_erlaubt()                m_Prog_Schaltung_erlaubt()

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define _#_()           m_Prog_Schaltung_erlaubt
 
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~T*/

/*~E*/
/*~E*/
/*~A*/
/*~+:OCIT FUNKTIONEN*/
/*~K*/
/*~+:OCIT FUNKTIONEN*/
/*~A*/
/*~+:OePNV IV Ein / Aus  und  OePNV IV Ein- / Aus-Ist */
/*~K*/
/*~+:OePNV IV Ein / Aus  und  OePNV IV Ein- / Aus-Ist */
/*~I*/
#ifdef _VSPLUS_SIM_ 
/*~A*/
/*~+:Simulation*/
/*~T*/
#define l_OePNV_Ein_Aus()       OePNV_Ein_Aus()
#define l_IV_Ein_Aus()          IV_Ein_Aus()

/*~T*/
#define OePNV_Ein_Aus_Ist()     (l_VSP_X_Ein_Aus_Ist(0))
#define IV_Ein_Aus_Ist()        (l_VSP_X_Ein_Aus_Ist(1))

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define l_OePNV_Ein_Aus()            OePNV_Ein_Aus()   /* OePNV_Ein_Aus */)
#define l_IV_Ein_Aus()               IV_Ein_Aus()   /* IV_Ein_Aus */

/*~T*/
#define _#_()                (l_VSP_X_Ein_Aus_Ist(0)    /* OePNV_Ein_Aus_Ist */)
#define _#_()                (l_VSP_X_Ein_Aus_Ist(1))   /* IV_Ein_Aus_Ist */

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Detektor vorhanden*/
/*~K*/
/*~+:Detektor vorhanden*/
/*~I*/
#ifdef _VSPLUS_SIM_ 
/*~A*/
/*~+:Simulation*/
/*~T*/
#define l_Det_Aktiv(x)          Det_Aktiv(x)

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define l_Det_Aktiv(x)                  Det_Aktiv(x) 
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Signalgruppe vorhanden*/
/*~K*/
/*~+:Signalgruppe vorhanden*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
#define l_Sg_Aktiv(x)           Sg_Aktiv(x)

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define l_Sg_Aktiv(x)           Sg_Aktiv(x) 
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Abfrage der Uhrzeit*/
/*~K*/
/*~+:Abfrage der Uhrzeit*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
#define l_AktuelleZeit(x,y,z)   AktuelleZeit(x,y,z)

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define l_AktuelleZeit(x,y,z)           AktuelleZeit(x,y,z) 
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Abfragen des Datums*/
/*~K*/
/*~+:Abfragen des Datums*/
/*~I*/
#ifdef _VSPLUS_SIM_ 
/*~A*/
/*~+:Simulation*/
/*~T*/
#define l_AktuellesDatum(w,x,y,z)               AktuellesDatum(w,x,y,z)

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define l_AktuellesDatum(w,x,y,z)               AktuellesDatum(w,x,y,z)
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Abfragen der Knotennummer*/
/*~K*/
/*~+:Abfragen der Knotennummer*/
/*~I*/
#ifdef _VSPLUS_SIM_ 
/*~A*/
/*~+:Simulation*/
/*~T*/
#define l_Get_OCITOutstationId(x,y,z)           Get_OCITOutstationId(x,y,z)

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define l_Get_OCITOutstationId(x,y,z)           Get_OCITOutstationId(x,y,z) 
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Abfragen ob Programmwahl von Zentrale*/
/*~K*/
/*~+:Abfragen ob Programmwahl von Zentrale*/
/*~I*/
#ifdef _VSPLUS_SIM_ 
/*~A*/
/*~+:Simulation*/
/*~T*/
#define l_ProgrammWahlZentrale()           ProgrammWahlZentrale()

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define l_ProgrammWahlZentrale()           ProgrammWahlZentrale() 
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:ZSondereingriff*/
/*~K*/
/*~+:ZSondereingriff*/
/*~I*/
#ifdef _VSPLUS_SIM_ 
/*~A*/
/*~+:Simulation*/
/*~K*/
/*~+:Lesen des Sondereingriffs*/
/*~T*/
#define l_ZSondereingriffvn(S, E, vn)         ZSondereingriffvn(S, E, vn)

/*~K*/
/*~+:Lesen des UTC Zeitstempels*/
/*~T*/
#define l_UTCZeitstempel()              UTCZeitstempel()

/*~K*/
/*~+:Schreiben des Sondereingriffs*/
/*~+:Version mit Vorgangsnummer*/
/*~+:*/
/*~T*/
#define ZSondereingriffVSP(vn)            m_ZSondereingriffVSPvn(vn)

/*~T*/

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~K*/
/*~+:Lesen des Sondereingriffs*/
/*~+:Version mit Vorgangsnummer*/
/*~T*/
#define l_ZSondereingriffvn(S, E, vn)         ZSondereingriffvn(S, E, vn)

/*~K*/
/*~+:Lesen des UTC Zeitstempels*/
/*~T*/
#define l_UTCZeitstempel()              UTCZeitstempel()

/*~K*/
/*~+:Schreiben des Sondereingriffs*/
/*~+:Version mit Vorgangsnummer*/
/*~+:*/
/*~T*/
#define _#_(vn)                         m_ZSondereingriffVSPvn(vn)

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNKTIONEN PARAMETERVERSORGUNG*/
/*~K*/
/*~+:FUNKTIONEN PARAMETERVERSORGUNG*/
/*~A*/
/*~+:Speicherplatz anfordern*/
/*~K*/
/*~+:Speicherplatz anfordern*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
#define f_Alloziere_VSP_Speicher(x,y)           Allozieren_VSP_Speicher(x,y)

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define f_Alloziere_VSP_Speicher(x,y)           Allozieren_VSP_Speicher(x,y)
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Freigeben VS-PLUS Parameter Speicher*/
/*~K*/
/*~+:Freigeben VS-PLUS Parameter Speicher*/
/*~I*/
#ifdef _VSPLUS_SIM_ 
/*~A*/
/*~+:Simulation*/
/*~T*/
#define l_Freigeben_VSP_Speicher(x)           Freigeben_VSP_Speicher(x)

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define l_Freigeben_VSP_Speicher(x)           Freigeben_VSP_Speicher(x) 
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Zeiger auf VS-PLUS Parameter anfordern*/
/*~K*/
/*~+:Zeiger auf VS-PLUS Parameter anfordern*/
/*~I*/
#ifdef _VSPLUS_SIM_ 
/*~A*/
/*~+:Simulation*/
/*~T*/
#define l_Gib_VSP_Zeiger(x)           Gib_VSP_Zeiger(x)

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define l_Gib_VSP_Zeiger(x)           Gib_VSP_Zeiger(x)
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Versorgung vom Geraet lesen*/
/*~K*/
/*~+:Versorgung vom Geraet lesen*/
/*~I*/
#ifdef _VSPLUS_SIM_ 
/*~A*/
/*~+:Simulation*/
/*~T*/
#define l_Oeffnen_VSP_Parameter()       Oeffnen_VSP_Parameter()
#define l_Lesen_VSP_Parameter(x,y)      Read_VSP_Parameter(x,y)
#define l_Schliessen_VSP_Parameter()    Schliessen_VSP_Parameter()

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define l_Oeffnen_VSP_Parameter()       Oeffnen_VSP_Parameter()
#define l_Lesen_VSP_Parameter(x,y)      Read_VSP_Parameter(x,y)
#define l_Schliessen_VSP_Parameter()    Schliessen_VSP_Parameter()

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Versorgung beim Geraet sichern*/
/*~K*/
/*~+:Versorgung beim Geraet sichern*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
#define m_Oeffnen_Sichern_Parameter()       Oeffnen_Sichern_Parameter()
#define m_Schreiben_Sichern_Parameter(x,y)  Schreiben_Sichern_Parameter(x,y)
#define m_Schliessen_Sichern_Parameter()    Schliessen_Sichern_Parameter()

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define m_Oeffnen_Sichern_Parameter()       Oeffnen_Sichern_Parameter()
#define m_Schreiben_Sichern_Parameter(x,y)  Schreiben_Sichern_Parameter(x,y)
#define m_Schliessen_Sichern_Parameter()    Schliessen_Sichern_Parameter()

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Initialisierung Versorgung*/
/*~K*/
/*~+:Initialisierung Versorgung*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
#define Initial_VSP_Parameter()          f_Initial_VSP_Parameter()

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define _#_()           f_Initial_VSP_Parameter()

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Pruefen einer Versorgungsdatei*/
/*~K*/
/*~+:Pruefen einer Versorgungsdatei*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
#define Pruefen_VSP_Parameter()          f_Pruefen_VSP_Parameter()

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define _#_()           f_Pruefen_VSP_Parameter()

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Lesen einer Versorgungsdatei*/
/*~K*/
/*~+:Lesen einer Versorgungsdatei*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
#define Lesen_VSP_Parameter()          f_Lesen_VSP_Parameter()

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define _#_()           f_Lesen_VSP_Parameter()

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Ende VSP Parameter*/
/*~K*/
/*~+:Ende VSP Parameter*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
#define Ende_VSP_Parameter()          f_Ende_VSP_Parameter()

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define _#_()           f_Ende_VSP_Parameter()
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNKTIONEN FUER VS-PLUS BEFEHLE*/
/*~K*/
/*~+:FUNKTIONEN FUER VS-PLUS BEFEHLE*/
/*~A*/
/*~+:Sind neue Daten da*/
/*~K*/
/*~+:Sind neue Daten da*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
#define l_Neue_Befehle()                Neue_Befehle()

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define l_Neue_Befehle()                Neue_Befehle() 

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Zugreiffen auf die Befehlsdatei*/
/*~K*/
/*~+:Zugreiffen auf die Befehlsdatei*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
#define l_Oeffnen_VSP_Befehle()         Oeffnen_VSP_Befehle()
#define l_Lesen_VSP_Befehle(x,y)        Lesen_VSP_Befehle(x,y)
#define l_Schliessen_VSP_Befehle()      Schliessen_VSP_Befehle()

/*~T*/
#define VABefehlPfad()                  l_VABefehlPfad()

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define l_Oeffnen_VSP_Befehle()         Oeffnen_VSP_Befehle()
#define l_Lesen_VSP_Befehle(x,y)        Lesen_VSP_Befehle(x,y)
#define l_Schliessen_VSP_Befehle()      Schliessen_VSP_Befehle()

/*~T*/
#define _#_()                           l_VABefehlPfad()

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:ABFRAGEN VON PROZESSDATEN*/
/*~K*/
/*~+:ABFRAGEN VON PROZESSDATEN*/
/*~A*/
/*~+:Ist ein Programm fuer VS-PLUS parametriert*/
/*~K*/
/*~+:Ist ein Programm fuer VS-PLUS parametriert*/
/*~I*/
#ifdef _VSPLUS_SIM_ 
/*~A*/
/*~+:Simulation*/
/*~T*/
#define Prog_VSP(x)             l_Prog_VSP(x)

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~T*/
#define  _#_(x)         l_Prog_VSP(x)

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Abfrage des VS-PLUS Versions-Text durch Steuergeraet*/
/*~K*/
/*~+:Abfrage des VS-PLUS Versions-Text durch Steuergeraet*/
/*~I*/
#ifdef _VSPLUS_SIM_ 
/*~A*/
/*~+:Simulation*/
/*~T*/
#define         f_versions_txt(x,y)             f_versions_txt(x,y)
/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~K*/
/*~+:short f_versions_txt(char* text, int size)*/
/*~+:*/
/*~T*/
#define _#_()                   f_versions_txt(x,y)
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Abfragen von PD Werten*/
/*~I*/
#ifdef _VSPLUS_SIM_ 
/*~A*/
/*~+:Simulation*/
/*~T*/
#define         f_VSP_ProzessDaten(x,y)             f_VSP_ProzessDaten(x,y)
/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergeraet*/
/*~K*/
/*~+:U_WORD f_VSP_ProzessDaten(void *px, void *py)*/
/*~T*/
#define _#_()                   f_VSP_ProzessDaten(x,y)
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:OEV-SPEICHER LESEN*/
/*~I*/
#ifdef _VSPLUS_SIM_
/*~A*/
/*~+:Simulation (inaktiv)*/
/*~T*/
#define l_OEVSpeicherLesenEin()         OEVSpeicherLesenEin()
/*~T*/
#define f_OEVSpeicherAusgabe(x,y)       OEVSpeicherAusgabe(x,y)
/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Steuergerät*/
/*~K*/
/*~+:STG definiert ob es den OeVSpeicher (AMLi-Telegramm) auslesen will oder nicht*/
/*~+:0 = nicht auslesen, 1 = auslesen*/
/*~T*/
#define _#_()           l_OEVSpeicherLesenEin()
/*~K*/
/*~+:AMLi-Telegramm*/
/*~+:void f_OEVSpeicherAusgabe(char* oev_daten, short typ)*/
/*~+:typ 1 = Header der AMLi-Telegramme ( wird nur einmal ausgegeben)*/
/*~+:    0 = Daten des AMLi-Telegramms*/
/*~+:Format: NR  TT.MO HH:MM:SS  MPN  LLLKK RRR PZH FAHRP  TX SP PH-UE TWF RTE GNE*/
/*~T*/
#define _#_()           f_OEVSpeicherAusgabe(x,y)
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:BYTE DREHEN*/
/*~K*/
/*~+:BYTE DREHEN*/
/*~I*/
#ifdef _PROZESSOR_INTEL_  
/*~A*/
/*~+:Intelprozessor*/
/*~T*/
/*
Bytesdrehen, man muss nur m_BC aufrufen
 */
#define m_BCS(a) (signed short) (((((unsigned short)(a))<<8)|(((unsigned short)(a))>>8)))
#define m_BCL(a) \
(signed long) (( ((unsigned long)(a)) <<24 ) | \
( ( ( ((unsigned long)(a)) >>8  )<<24 )>>8  )| \
( ( ( ((unsigned long)(a)) >>16 )<<24 )>>16 )| \
( ((unsigned long)(a)) >> 24))
#define m_BC(a) (  (sizeof (a) == 2) ? (m_BCS (a)) : (m_BCL (a)) )

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:kein Intelprozessor*/
/*~T*/
#define m_BC(a)             a
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:HELPER_MACROS*/
/*~K*/
/*~+:HELPER_MACROS*/
/*~I*/
#ifdef _HELPER_MACROS_
/*~T*/
#define LOBYTE(w)           ((unsigned char)(w))
#define HIBYTE(w)           ((unsigned char)((unsigned int)(w) >> 8))
#define LOWORD(l)           ((unsigned short)(l))
#define HIWORD(l)           ((unsigned short)((unsigned long)(l) >> 16))
/*~-*/
#endif
/*~E*/
/*~E*/
/*~-*/
#endif
/*~E*/
