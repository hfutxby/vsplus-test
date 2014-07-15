/*~-*/
/*~XSF_LANGUAGE: C/C++*/
/*~T*/
/* VS-PLUS 6 Release 2 State 6  Edition 2014 */
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


              INCLUDE FILE INTERFACE BASIS for VS-PLUS


       File name      : if626bas.inc
       Version        : 6.2.6
       Date           : 15.08.2012
       Programmer     : P. Herren


       Conents :
       ~~~~~~~~
        This file contains the controller-specific function calls

        #define _VSPLUS_SIM_    Definitions for version with simulation
        #define _VSPLUS_STG_    Definitions for version with controller

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
#define VSP_NR         11  /* Siemens(WFi) control scheme number      */
#define VSP_FEHLER_NR  99  /* Siemens(WFi) own number for VS-PLUS    */

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
/*~+:TIMER DEFINITIONs (Timer base)*/
/*~T*/
#define VSP_T0      0u                           /* Base 0 for timer                     */
#define VSP_T1      VSP_T0+DETMAX                /* Base 1 for timer                     */
#define VSP_T2      VSP_T1+DETMAX                /* Base 2 for timer                     */
#define VSP_T3      VSP_T2+DETMAX                /* Base 3 for timer                     */
#define VSP_T4      VSP_T3+VSMAX                 /* Base 4 for timer                     */
#define VSP_T5      VSP_T4+VSMAX                 /* Base 5 for timer                     */
#define VSP_T6      VSP_T5+VSMAX                 /* Base 6 for timer                     */
#define VSP_T7      VSP_T6+VSMAX                 /* Base 7 for timer                     */
#define VSP_TE      VSP_T7+SPEZMAX               /* Last timer (Base 0-7)              */
#define MAXTIMER (VSP_TE+1)
//#define MAXTIMER 5

/*~E*/
/*~A*/
/*~+:DATA TYPES VS-PLUS*/
/*~K*/
/*~+:Variable types*/
/*~T*/
typedef         unsigned char  U_BYTE;  /*  8-Bit variable  without V  */
typedef         signed char    S_BYTE;  /*  8-Bit variable  with V  */

/*~I*/
#ifdef _EMULATOR_
/*~T*/
typedef         unsigned char  BYTE;    /* EMULATOR */

/*~-*/
#endif
/*~E*/
/*~T*/
#define US_WORD U_WORD                  /* 16-Bit variable  without V  */
#define SS_WORD S_WORD                  /* 16-Bit variable  with V  */

/*~T*/
typedef         unsigned short U_WORD;  /* 16-Bit variable  without V  */
typedef         signed short   S_WORD;  /* 16-Bit variable  with V  */

/*~T*/
#define UL_WORD U_WORD                  /* 16-Bit variable  without V  */
#define SL_WORD S_WORD                  /* 16-Bit variable  with V  */

/*~I*/
#ifdef _EMULATOR_
/*~T*/
#define WORD    U_WORD                  /* 16-Bit variable  without V  */
/* #define MAXWORD 0xFFFF */

/*~-*/
#endif
/*~E*/
/*~T*/
typedef         unsigned long  U_LONG;  /* 32-Bit variable  without V  */
typedef         signed long    S_LONG;  /* 32-Bit variable  with V  */

/*~I*/
#ifdef _EMULATOR_
/*~T*/
typedef         unsigned long  ULONG;   /* EMULATOR */

/*~-*/
#endif
/*~E*/
/*~K*/
/*~+:Logical*/
/*~+:*/
/*~T*/
typedef enum {L_falsch, L_richtig} Logisch;

/*~K*/
/*~+:Time*/
/*~+:*/
/*~T*/
typedef US_WORD Zeit;
typedef SS_WORD DZeit;
typedef S_WORD  LZeit;
/*~E*/
/*~A*/
/*~+:PROTOTYPES*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Prototypes for simulation functions*/
/*~T*/
extern void  Meldung (short int nr, short int par1, short int par2, short int par3, short int par4);
extern void  MeldungNET(unsigned char nr, unsigned short Anr, unsigned char par1, unsigned char par2, unsigned char par3, unsigned char par4, unsigned char par5);
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
/*~+:Controller*/
/*~K*/
/*~+:Program functions*/
/*~T*/
short int ProgrammAktuell(void);
short int ProgrammWahl(void);
short int Zykluszeit(void);
short int Umlaufzeit(void);
int ProgrammWahlZentrale(void);
short int timer(short int funktion, short int timer);
short int timer_2(short int funktion, short int timer, short int wert);

/*~K*/
/*~+:Detector functions*/
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
short d_zeitlb(short det);

/*~K*/
/*~+:Signal-group functions*/
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
short int s_stoeblink(short int sg);
unsigned short s_t_rot(short int sg);
unsigned short int s_t_gelb(short int sg);
unsigned short int s_t_min_rot(short int sg);
unsigned short int s_t_grun(short int sg);
unsigned short int s_t_vor(short int sg);
unsigned short int s_t_min_grun(short int sg);
unsigned short int s_HW_VspFreigegeben(short int sg);
short int s_zwi_zeit(short sgr, short sge);

/*~K*/
/*~+:Error messages*/
/*~T*/
void Meldung(short int degree, short int nr, short int par1, short int par2, short int par3, short int par4);
void  MeldungNET(short int degree, unsigned char nr, unsigned short Anr, unsigned char par1, unsigned char par2, unsigned char par3, unsigned char par4, unsigned char par5);
void U_Kontrolle(short int vs, short int zeit);

/*~K*/
/*~+:System functions*/
/*~T*/
short TelegrammVomGeraet(void* oev_tele_poi);
unsigned short int s_SteuerungNichtAktiv(void);

/*~K*/
/*~+:Data handling*/
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
/*~+:OCIT functions*/
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
/*~+:Prototypes for functions within VS-PLUS*/
/*~T*/
int           l_Det_Wert(int KanalNummer, int Typ);
int           m_Prog_Schaltung_erlaubt(void);
int           l_Prog_VSP(int ProgNummer);
int           f_Initial_VSP_Parameter(void);
int           f_Pruefen_VSP_Parameter(void);
int           f_Lesen_VSP_Parameter(void);
void          f_Ende_VSP_Parameter(void);
int           l_VSP_X_Ein_Aus_Ist(int T);
char*         l_VABefehlPfad(void);
/*~E*/
/*~E*/
/*~A*/
/*~+:Specific to controller manufacturer*/
/*~A*/
/*~+:Siemens C900*/
/*~I*/
#ifdef _SIEMENS_C900_
/*~K*/
/*~+:Example for "typdef struct" (GNU Compiler Siemens)*/
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
/*~+:DEFINITIONS for PT module*/
/*~T*/
#define OMAXBYTE                 255  /* in case of unsigned char          */
#define OMAXWORD              65535U  /* in case of unsigned short (2byte)  */

/*~K*/
/*~+:Language for PT module*/
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
/*~+:DEFINITIONS for partial intersections*/
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
/*~+:Detector waiting time*/
/*~K*/
/*~+:Detector waiting time*/
/*~I*/
#ifdef _VSPLUS_SIM_   
/*~A*/
/*~+:Simulation*/
/*~T*/
#define t_twdet(x)      timer(1,x+(VSP_T0),0) /* -read               */
#define m_stwdet(x,k)   timer(2,x+(VSP_T0),k) /* -load and start with k */
#define m_ltwdet(x)     timer(3,x+(VSP_T0),0) /* -clear            */
#define m_altwdet(x)    timer(4,x+(VSP_T0),0) /* -stop and clear   */
#define m_atwdet(x)     timer(5,x+(VSP_T0),0) /* -stop            */

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Controller*/
/*~T*/
#define t_twdet(x)      timer(1,x+(VSP_T0))     /* -read               */
#define m_stwdet(x,k)   timer_2(2,x+(VSP_T0),k) /* -load and start with k */
#define m_ltwdet(x)     timer(3,x+(VSP_T0))     /* -clear            */
#define m_altwdet(x)    timer(4,x+(VSP_T0))     /* -stop and clear   */
#define m_atwdet(x)     timer(5,x+(VSP_T0))     /* -stop            */

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Detector waiting time for 1st follow-up haul*/
/*~K*/
/*~+:Detector waiting time for 1st follow-up haul*/
/*~I*/
#ifdef _VSPLUS_SIM_   
/*~A*/
/*~+:Simulation*/
/*~T*/
#define t_twdet1(x)     timer(1,x+(VSP_T1),0) /* -read               */
#define m_stwdet1(x,k)  timer(2,x+(VSP_T1),k) /* -load and start with k */
#define m_ltwdet1(x)    timer(3,x+(VSP_T1),0) /* -clear            */
#define m_altwdet1(x)   timer(4,x+(VSP_T1),0) /* -stop and clear   */
#define m_atwdet1(x)    timer(5,x+(VSP_T1),0) /* -stop            */

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Controller*/
/*~T*/
#define t_twdet1(x)     timer(1,x+(VSP_T1))     /* -read               */
#define m_stwdet1(x,k)  timer_2(2,x+(VSP_T1),k) /* -load and start with k */
#define m_ltwdet1(x)    timer(3,x+(VSP_T1))     /* -clear            */
#define m_altwdet1(x)   timer(4,x+(VSP_T1))     /* -stop and clear   */
#define m_atwdet1(x)    timer(5,x+(VSP_T1))     /* -stop            */

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Detector waiting time for 2nd follow-up haul*/
/*~K*/
/*~+:Detector waiting time for 2nd follow-up haul*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
#define t_twdet2(x)     timer(1,x+(VSP_T2),0) /* -read               */
#define m_stwdet2(x,k)  timer(2,x+(VSP_T2),k) /* -load and start with k */
#define m_ltwdet2(x)    timer(3,x+(VSP_T2),0) /* -clear            */
#define m_altwdet2(x)   timer(4,x+(VSP_T2),0) /* -stop and clear  */
#define m_atwdet2(x)    timer(5,x+(VSP_T2),0) /* -stop            */

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Controller*/
/*~T*/
#define t_twdet2(x)     timer(1,x+(VSP_T2))     /* -read               */
#define m_stwdet2(x,k)  timer_2(2,x+(VSP_T2),k) /* -load and start with k */
#define m_ltwdet2(x)    timer(3,x+(VSP_T2))     /* -clear            */
#define m_altwdet2(x)   timer(4,x+(VSP_T2))     /* -stop and clear   */
#define m_atwdet2(x)    timer(5,x+(VSP_T2))     /* -stop            */

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Traffic stream waiting time*/
/*~K*/
/*~+:Traffic stream waiting time*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
#define t_twvs(y)       timer(1,y+(VSP_T3),0) /* -read               */
#define m_stwvs(y,k)    timer(2,y+(VSP_T3),k) /* -load and start with k */
#define m_ltwvs(y)      timer(3,y+(VSP_T3),0) /* -clear            */
#define m_altwvs(y)     timer(4,y+(VSP_T3),0) /* -stop and clear   */
#define m_atwvs(y)      timer(5,y+(VSP_T3),0) /* -stop            */

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Controller*/
/*~T*/
#define t_twvs(y)       timer(1,y+(VSP_T3))     /* -read               */
#define m_stwvs(y,k)    timer_2(2,y+(VSP_T3),k) /* -load and start k */
#define m_ltwvs(y)      timer(3,y+(VSP_T3))     /* -clear            */
#define m_altwvs(y)     timer(4,y+(VSP_T3))     /* -stop and clear   */
#define m_atwvs(y)      timer(5,y+(VSP_T3))     /* -stop           */

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Digital output switch-on - switch-off time*/
/*~K*/
/*~+:Digital output switch-on - switch-off time*/
/*~I*/
#ifdef _VSPLUS_SIM_ 
/*~A*/
/*~+:Simulation*/
/*~T*/
#define t_anzt(y)       timer(1,y+(VSP_T4),0) /* -read               */
#define m_sanzt(y,k)    timer(2,y+(VSP_T4),k) /* -load and start with k */
#define m_lanzt(y)      timer(3,y+(VSP_T4),0) /* -clear            */
#define m_alanzt(y)     timer(4,y+(VSP_T4),0) /* -stop and clear   */
#define m_aanzt(y)      timer(5,y+(VSP_T4),0) /* -stop            */

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Controller*/
/*~T*/
#define t_anzt(y)       timer(1,y+(VSP_T4))     /* -read               */
#define m_sanzt(y,k)    timer_2(2,y+(VSP_T4),k) /* -load and start with k */
#define m_lanzt(y)      timer(3,y+(VSP_T4))     /* -clear            */
#define m_alanzt(y)     timer(4,y+(VSP_T4))     /* -stop and clear   */
#define m_aanzt(y)      timer(5,y+(VSP_T4))     /* -stop            */

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Signal time counter (red green) for traffic stream*/
/*~K*/
/*~+:Signal time counter (red green) for traffic stream*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
#define t_vst(y)        timer(1,y+(VSP_T5),0) /* -read               */
#define m_svst(y,k)     timer(2,y+(VSP_T5),k) /* -load and start with k */
#define m_lvst(y)       timer(3,y+(VSP_T5),0) /* -clear            */
#define m_alvst(y)      timer(4,y+(VSP_T5),0) /* -stop and clear   */
#define m_avst(y)       timer(5,y+(VSP_T5),0) /* -stop            */

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Controller*/
/*~T*/
#define t_vst(y)        timer(1,y+(VSP_T5))     /* -read               */
#define m_svst(y,k)     timer_2(2,y+(VSP_T5),k) /* -load and start with k */
#define m_lvst(y)       timer(3,y+(VSP_T5))     /* -clear            */
#define m_alvst(y)      timer(4,y+(VSP_T5))     /* -stop and clear   */
#define m_avst(y)       timer(5,y+(VSP_T5))     /* -stop            */

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Signal time counter (prepraration/amber) for traffic stream*/
/*~K*/
/*~+:Signal time counter (prepraration/amber) for traffic stream*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
#define t_vsvort(y)     timer(1,y+(VSP_T6),0) /* -read               */
#define m_svsvort(y,k)  timer(2,y+(VSP_T6),k) /* -load and start with k */
#define m_lvsvort(y)    timer(3,y+(VSP_T6),0) /* -clear            */
#define m_alvsvort(y)   timer(4,y+(VSP_T6),0) /* -stop and clear   */
#define m_avsvort(y)    timer(5,y+(VSP_T6),0) /* -stop            */

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Controller*/
/*~T*/
#define t_vsvort(y)     timer(1,y+(VSP_T6))     /* -read               */
#define m_svsvort(y,k)  timer_2(2,y+(VSP_T6),k) /* -load and start with k */
#define m_lvsvort(y)    timer(3,y+(VSP_T6))     /* -clear            */
#define m_alvsvort(y)   timer(4,y+(VSP_T6))     /* -stop and clear   */
#define m_avsvort(y)    timer(5,y+(VSP_T6))     /* -stop            */

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Special timer (for special functions)*/
/*~K*/
/*~+:Special timer (for special functions)*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
#define t_spezt(y)      timer(1,y+(VSP_T7),0) /* -read               */
#define m_sspezt(y,k)   timer(2,y+(VSP_T7),k) /* -load and start with k */
#define m_lspezt(y)     timer(3,y+(VSP_T7),0) /* -clear            */
#define m_alspezt(y)    timer(4,y+(VSP_T7),0) /* -stop and clear   */
#define m_aspezt(y)     timer(5,y+(VSP_T7),0) /* -stop            */

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Controller*/
/*~T*/
#define t_spezt(y)      timer(1,y+(VSP_T7))      /* -read               */
#define m_sspezt(y,k)   timer_2(2,y+(VSP_T7),k)  /* -load and start with k */
#define m_lspezt(y)     timer(3,y+(VSP_T7))      /* -clear            */
#define m_alspezt(y)    timer(4,y+(VSP_T7))      /* -stop and clear   */
#define m_aspezt(y)     timer(5,y+(VSP_T7))      /* -stop            */

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:BASE FUNCTIONS*/
/*~K*/
/*~+:BASE FUNCTIONS*/
/*~A*/
/*~+:Program functions*/
/*~K*/
/*~+:Program functions*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
/* Actual signal-program        */
#define f_prg()             ProgrammAktuell()
/* Selected signal-program      */
#define f_prgwl()           ProgrammWahl()

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Controller*/
/*~T*/
/* Actual signal-program        */
#define f_prg()             ProgrammAktuell()
/* Selected signal-program     */
#define f_prgwl()           ProgrammWahl()
 
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Cycle control*/
/*~K*/
/*~+:Cycle control*/
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
/*~+:Controller*/
/*~T*/
#define f_ur(vs,zeit)            U_Kontrolle(vs,zeit)
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Framework signal-plan functions*/
/*~K*/
/*~+:Framework signal-plan functions*/
/*~I*/
#ifdef _VSPLUS_SIM_ 
/*~A*/
/*~+:Simulation*/
/*~T*/
/* Current cycle second (TX)    */
#define l_zytim()        Zykluszeit()
/* Cycle time (TU)    */
#define l_umzt()         Umlaufzeit()

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Controller*/
/*~T*/
/* Current cycle second (TX)    */
#define l_zytim()        Zykluszeit()
/* Cycle time (TU)    */
#define l_umzt()         Umlaufzeit()
 
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Detector functions*/
/*~A*/
/*~+:Detector functions*/
/*~K*/
/*~+:Detector functions*/
/*~I*/
#ifdef _VSPLUS_SIM_
/*~A*/
/*~+:Simulation*/
/*~T*/
/* Sum impulse storage (ZS-Value) */
#define l_imp(x)        d_imp(x)
/* Reset impulse storage (ZS-Value) */
#define m_limp(x)       d_limp(x)
/* Sum impulse storage (SS-Value) */
#define l_impss(x)      (0)
/* Reset impulse storage (SS-Value) */
#define m_limpss(x)     (0)
/* Sum impulse falling slopes  */
#define l_impab(x)      d_impab(1,x)
/* Reset sum of impulse falling slopes */
#define m_limpab(x)     d_impab(2,x)

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Controller*/
/*~T*/
/* Sum impulse storage (ZS-Value) */
#define l_imp(x)        d_imp(x)
/* Reset impulse storage (ZS-Value) */
#define m_limp(x)       d_limp(x)
/* Sum impulse storage (SS-Value) */
#define l_impss(x)      0
/* Reset impulse storage (SS-Value) */
#define m_limpss(x)     0
/* Sum impulse falling slopes   */
#define l_impab(x)      d_impab(1,x)
/* Reset sum of impulse falling slopes  */
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
/* Current percentage of occupancy   */
#define l_belga(x)      d_belga(x)
/* Smoothened occupancy degree (percentage)   */
#define l_belgg(x)      d_belgg(x)

/*~K*/
/*~+:Load, derived from traffic situation (GPS data, in %)*/
/*~T*/
#define l_kvalue(x)     d_kvalue(x)
/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Controller*/
/*~T*/
/* Current percentage of occupancy */
#define l_belga(x)      d_belga(x)
/* Smoothened occupancy degree (percentage) */
#define l_belgg(x)      d_belgg(x)
/*~K*/
/*~+:Load, derived from traffic situation (GPS data, in %)*/
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
/* Current state of occupancy */
#define l_belza(x)      d_blg(x)               

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Controller*/
/*~T*/
/* Current state of occupancy */
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
/* Net time gap in 1/10 s    */
#define t_zeitln(x)     d_ztlkn(x)             
/* Gross time gap in 1/10 s    */
#define t_zeitlb(x)     (0)                    

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Controller*/
/*~T*/
/* Net time gap in 1/10 s */
#define t_zeitln(x)     d_ztlkn(x)
/* Gross time gap in 1/10 s */
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
/* Occupancy time in 1/10 s     */
#define t_belzt(x)      d_blgzt(x)             

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Controller*/
/*~T*/
/* Occupancy time in 1/10 s     */
#define t_belzt(x)      d_blgzt(x)             

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Hardware detector fault*/
/*~K*/
/*~+:Hardware detector fault*/
/*~I*/
#ifdef _VSPLUS_SIM_
/*~A*/
/*~+:Simulation*/
/*~T*/
/* Hardware detector fault */
#define l_stoer(x)      d_stoer(x)
/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Controller*/
/*~T*/
/* Hardware detector fault */
#define l_stoer(x)      d_stoer(x)

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:Signal-group functions*/
/*~A*/
/*~+:Base values*/
/*~K*/
/*~+:Base values*/
/*~I*/
#ifdef _VSPLUS_SIM_ 
/*~A*/
/*~+:Simulation*/
/*~T*/
/* minimum red time          */
#define t_min_rot(x)         min_rot(x)
/* preparation time          */
#define t_vor(x)             u_rot_gelb(x)
/* minimum green time        */
#define t_min_grun(x)        min_gruen(x)
/* amber time                */
#define t_gelb(x)            u_gelb(x)

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Controller*/
/*~T*/
/* minimum red time           */
#define t_min_rot(x)          min_rot(x)
/* preparation time           */
#define t_vor(x)              u_rot_gelb(x)
/* minimum green time         */
#define t_min_grun(x)         min_gruen(x)
/* amber time                 */
#define t_gelb(x)             u_gelb(x)

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Switching commands: using signal colors*/
/*~K*/
/*~+:Switching commands: using signal colors*/
/*~I*/
#ifdef _VSPLUS_SIM_ 
/*~A*/
/*~+:Simulation*/
/*~T*/
/* signal group on       */
#define EIN_Signal(x)    SG_ein(x)
/* signal group off      */
#define AUS_Signal(x)    SG_aus(x)
/* digital output on              */
#define EIN_SState(x)    Relais_ein(x)
/* digital output off             */
#define AUS_SState(x)    Relais_aus(x)
/* digital blinker on       */
#define EIN_SBlink(x)    Blinker_ein(x)
/* digital blinker off      */
#define AUS_SBlink(x)    Blinker_aus(x)

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Controller*/
/*~T*/
/* signal group on       */
#define EIN_Signal(x)    SG_ein(x)
/* signal group off      */
#define AUS_Signal(x)    SG_aus(x)
/* digitl output on              */
#define EIN_SState(x)    Relais_ein(x)
/* digital output off             */
#define AUS_SState(x)    Relais_aus(x)
/* digital blinker on       */
#define EIN_SBlink(x)    Blinker_ein(x)
/* digital blinker off      */
#define AUS_SBlink(x)    Blinker_aus(x)

/*~E*/
/*~-*/
#endif
/*~E*/
/*~A*/
/*~+:Switching commands: special operations, these settings are valid for both contoller and simulation*/
/*~T*/
#define Signal_Dunkel(x)     0                  /* Switch signal group to dark           */
#define Signal_Blinken(x)    0                  /* Switch signal group to blinking       */
#define Signal_FarbeRot(x)   0                  /* Switch signal group to red after dark or blinking     */
#define Signal_FarbeGruen(x) 0                  /* Switch signal group to green after dark of blinking   */

/*~E*/
/*~E*/
/*~A*/
/*~+:Current signal state*/
/*~K*/
/*~+:Current state*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
/* Get current state whether signal-group shows red            */
#define HW_rot(x)           s_rot(x)
/* Get current state whether digital output is off             */
#define HW_sr_aus(x)        s_sr_aus(x)
/* Get current state whether digital blinker is off            */
#define HW_sb_aus(x)        s_sb_aus(x)
/* Get current state whether minimal red time elapsed          */
#define HW_min_rot(x)       s_min_rot(x)
/* Get current state whether signal-group shows amber          */
#define HW_gelb(x)          s_gelb(x)
/* Get current state whether signal-group shows green          */
#define HW_grun(x)          s_grun(x)
/* Get current state whether digital output is on              */
#define HW_sr_ein(x)        s_sr_ein(x)
/* Get current state whether digital blinker is on             */
#define HW_sb_ein(x)        s_sb_ein(x)
/* Get current state whether minimal green time elapsed        */
#define HW_min_grun(x)      s_min_grun(x)
/* Get current state whether signal-group is in transition open-closed */
#define HW_vor(x)           s_vor(x)

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Controller*/
/*~T*/
/* Get current state whether signal-group shows red           */
#define HW_rot(x)           s_rot(x)
/* Get current state whether digital output is off            */
#define HW_sr_aus(x)        s_sr_aus(x)
/* Get current state whether digital blinker is off           */
#define HW_sb_aus(x)        s_sb_aus(x)
/* Get current state whether minimal red time elapsed         */
#define HW_min_rot(x)       s_min_rot(x)
/* Get current state whether signal-group shows amber         */
#define HW_gelb(x)          s_gelb(x)
/*  Get current state whether signal-group shows green        */
#define HW_grun(x)          s_grun(x)
/* Get current state whether digital output is on             */
#define HW_sr_ein(x)        s_sr_ein(x)
/* Get current state whether digital blinker is on            */
#define HW_sb_ein(x)        s_sb_ein(x)
/* Get current state whether minimal green time elapsed       */
#define HW_min_grun(x)      s_min_grun(x)
/* Get current state whether signal-group is in transition open-closed  */
#define HW_vor(x)           s_vor(x)

/*~E*/
/*~-*/
#endif
/*~E*/
/*~A*/
/*~+:Check switching commands: special operations, these settings are valid for both contoller and simulation*/
/*~T*/
/* Check if a red command is pending      */
#define HW_bef_rot(x)       1
/* Check if a green command is pending    */
#define HW_bef_grun(x)      1

/*~E*/
/*~E*/
/*~A*/
/*~+:Current signal time*/
/*~K*/
/*~+:Current signal time*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
/* current red duration                   */
#define HW_t_rot(x)          s_t_rot(x)
/* current amber duration                 */
#define HW_t_gelb(x)         s_t_gelb(x)
/* current minimum red duration           */
#define HW_t_min_rot(x)      s_t_min_rot(x)
/* current green duration                 */
#define HW_t_grun(x)         s_t_grun(x)
/* current preparation time               */
#define HW_t_vor(x)          s_t_vor(x)
/* current minimum green duration         */
#define HW_t_min_grun(x)     s_t_min_grun(x)

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Controller*/
/*~T*/
/* current red duration                   */
#define HW_t_rot(x)          s_t_rot(x)
/* current amber duration                 */
#define HW_t_gelb(x)         s_t_gelb(x)
/* current minimum red duration           */
#define HW_t_min_rot(x)      s_t_min_rot(x)
/* current green duration                 */
#define HW_t_grun(x)         s_t_grun(x)
/* current preparation time               */
#define HW_t_vor(x)          s_t_vor(x)
/* current minimum green duration         */
#define HW_t_min_grun(x)     s_t_min_grun(x)

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Special (extraordinary) state*/
/*~K*/
/*~+:Special (extraordinary) state*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
/* Get current state: signal-group is in fault mode        */
#define HW_blink(x)            0
/* Get current state: fault duration                       */
#define HW_t_blink(x)          0
/* Get current state: signal-group is dark                 */
#define HW_Dunkel(x)           0
/* Get current state: signal-group is blinking             */
#define HW_Blinkend(x)         0
/* Get current state: signal-group is enabled for VS-PLUS  */
#define HW_VspFreigegeben(x)   s_HW_VspFreigegeben(x)

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Controller*/
/*~T*/
/* Get current state: signal-group is in fault mode       */
#define HW_blink(x)            s_stoeblink(x)
/* Get current state: signal-group is dark                */
#define HW_Dunkel(x)           0
/* Get current state: signal-group is blinking            */
#define HW_Blinkend(x)         0
/* Get current state: signal-group is enabled for VS-Plus */
#define HW_VspFreigegeben(x)   s_HW_VspFreigegeben(x)

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Access to shortened intergreens*/
/*~K*/
/*~+:Access to shortened intergreens*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
/* Read intergreen from controller t */
#define l_zzV(x,y)           pANZE_Zwizt_V[x-1].MElement[y-1]
/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Controller*/
/*~T*/
/* Read intergreen from controller  */
#define l_zzV(x,y)           s_zwi_zeit(x,y)
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:Error messages*/
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
/*~+:Controller*/
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
/*~+:System functions*/
/*~A*/
/*~+:Check for controller off and for controller fault*/
/*~K*/
/*~+:Check for controller off and for controller fault*/
/*~I*/
#ifdef _VSPLUS_SIM_  
/*~A*/
/*~+:Simulation*/
/*~T*/
/*
The function returns 0 as long a a signal plan is being processed. 
When the controller is off or during switch-on or switch-off, this function must return 1
*/
/*~T*/
#define SteuerungNichtAktiv()      s_SteuerungNichtAktiv()

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Controller*/
/*~T*/
/*
The function returns 0 as long a a signal plan is being processed. 
When the controller is off or during switch-on or switch-off, this function must return 1
*/

/*~T*/
#define SteuerungNichtAktiv()    s_SteuerungNichtAktiv()
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Hand over received telegram*/
/*~K*/
/*~+:Hand over received telegram*/
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
/*~+:Controller*/
/*~T*/
#define GibTelegramm(x)        TelegrammVomGeraet(x)
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Switch off VS-PLUS*/
/*~K*/
/*~+:Switch off VS-PLUS*/
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
/*~+:Controller*/
/*~T*/
#define m_Wunsch_VSPLUS(x,y)            Wunsch_VSPLUS(x,y) 
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Delay program switch (set by adaptive control type "Vmod")*/
/*~K*/
/*~+:Delay program switch (set by adaptive control type "Vmod")*/
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
/*~+:Controller*/
/*~T*/
//#define _#_()           m_Prog_Schaltung_erlaubt
#define vs_prog_swtich_allow               m_Prog_Schaltung_erlaubt()
 
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~T*/

/*~E*/
/*~E*/
/*~A*/
/*~+:OCIT FUNCTIONS*/
/*~K*/
/*~+:OCIT FUNCTIONS*/
/*~A*/
/*~+:PT, IT on/off (preset in Controller, OCIT central-controller) and PT, IT on-/off-state (current in VSPlus) */
/*~K*/
/*~+:PT, IT on/off (preset Controller, OCIT Center) and PT, IT on-/off-state (current in VSPlus)*/
/*~I*/
#ifdef _VSPLUS_SIM_ 
/*~A*/
/*~+:Simulation*/
/*~T*/
#define l_OePNV_Ein_Aus()       OePNV_Ein_Aus()
#define l_IV_Ein_Aus()          IV_Ein_Aus()

/*~T*/
#define OePNV_Ein_Aus_Ist()     OePNV_Ein_Aus()
#define IV_Ein_Aus_Ist()        IV_Ein_Aus()

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Controller*/
/*~T*/
/* l_OePNV_Ein_Aus():
 *
 */
#define l_OePNV_Ein_Aus()            OePNV_Ein_Aus()   /* Read PT on/off */
#define l_IV_Ein_Aus()               IV_Ein_Aus()   /* Read IT on/off */

/*~T*/
//#define _#_()                (l_VSP_X_Ein_Aus_Ist(0)    /* Read PT on/off state of VSP */
//#define _#_()                (l_VSP_X_Ein_Aus_Ist(1))   /* Read IT on/off state of VSP */
#define vs_pt_state()               (l_VSP_X_Ein_Aus_Ist(0)    /* Read PT on/off state of VSP */
#define vs_it_state()                (l_VSP_X_Ein_Aus_Ist(1))   /* Read IT on/off state of VSP */

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Detector exists*/
/*~K*/
/*~+:Detector exists*/
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
/*~+:Controller*/
/*~T*/
#define l_Det_Aktiv(x)                  Det_Aktiv(x) 
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Signal-group exists*/
/*~K*/
/*~+:Signal-group exists*/
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
/*~+:Controller*/
/*~T*/
#define l_Sg_Aktiv(x)           Sg_Aktiv(x) 
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Get current time*/
/*~K*/
/*~+:Get current time*/
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
/*~+:Controller*/
/*~T*/
#define l_AktuelleZeit(x,y,z)           AktuelleZeit(x,y,z) 
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Get current date*/
/*~K*/
/*~+:Get current date*/
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
/*~+:Controller*/
/*~T*/
#define l_AktuellesDatum(w,x,y,z)               AktuellesDatum(w,x,y,z)
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Get unit-number (intersection): OCIT outstationID*/
/*~K*/
/*~+:Get unit-number (intersection): OCIT outstationID*/
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
/*~+:Controller*/
/*~T*/
#define l_Get_OCITOutstationId(x,y,z)           Get_OCITOutstationId(x,y,z) 
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Get central-controller signal program selection*/
/*~K*/
/*~+:Get central-controller signal program selection*/
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
/*~+:Controller*/
/*~T*/
#define l_ProgrammWahlZentrale()           ProgrammWahlZentrale() 
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Central-controller special intervention*/
/*~K*/
/*~+:Central-controller special intervention*/
/*~I*/
#ifdef _VSPLUS_SIM_ 
/*~A*/
/*~+:Simulation*/
/*~K*/
/*~+:Read special intervention*/
/*~+:Version includes job-number (vn)*/
/*~+:*/
/*~T*/
#define l_ZSondereingriffvn(S, E, vn)         ZSondereingriffvn(S, E, vn)

/*~K*/
/*~+:Get UTC time-stamp*/
/*~T*/
#define l_UTCZeitstempel()              UTCZeitstempel()

/*~K*/
/*~+:Write special intervention*/
/*~+:Version includes job-number (vn)*/
/*~+:*/
/*~T*/
#define ZSondereingriffVSP(vn)            m_ZSondereingriffVSPvn(vn)

/*~T*/

/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Controller*/
/*~K*/
/*~+:Read special intervention*/
/*~+:Version includes job-number (vn)*/
/*~+:*/
/*~T*/
#define l_ZSondereingriffvn(S, E, vn)         ZSondereingriffvn(S, E, vn)

/*~K*/
/*~+:Get UTC time-stamp*/
/*~T*/
#define l_UTCZeitstempel()              UTCZeitstempel()

/*~K*/
/*~+:Write special intervention*/
/*~+:Version includes job-number (vn)*/
/*~+:*/
/*~T*/
//#define _#_(vn)                         m_ZSondereingriffVSPvn(vn)
#define vs_prog_amod(vn)                         m_ZSondereingriffVSPvn(vn)

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTIONS FOR SUPPLY PARAMETER FILES*/
/*~K*/
/*~+:FUNCTIONS FOR SUPPLY PARAMETER FILES*/
/*~A*/
/*~+:Request memory*/
/*~K*/
/*~+:Request memory*/
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
/*~+:Controller*/
/*~T*/
#define f_Alloziere_VSP_Speicher(x,y)           Allozieren_VSP_Speicher(x,y)
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Free VS-PLUS supply parameter memory*/
/*~K*/
/*~+:Free VS-PLUS supply parameter memory*/
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
/*~+:Controller*/
/*~T*/
#define l_Freigeben_VSP_Speicher(x)           Freigeben_VSP_Speicher(x) 
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Request VS-PLUS supply parameter memory pointer*/
/*~K*/
/*~+:Request VS-PLUS supply parameter memory pointer*/
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
/*~+:Controller*/
/*~T*/
#define l_Gib_VSP_Zeiger(x)           Gib_VSP_Zeiger(x)
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Read supply parameters from controller*/
/*~K*/
/*~+:Read supply parameters from controller*/
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
/*~+:Controller*/
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
/*~+:Save supply parameters to controller*/
/*~K*/
/*~+:Save supply parameters to controller*/
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
/*~+:Controller*/
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
/*~+:Initialize supply process*/
/*~K*/
/*~+:Initialize supply process*/
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
/*~+:Controller*/
/*~T*/
//#define _#_()           f_Initial_VSP_Parameter()
#define vs_init_parameter()          f_Initial_VSP_Parameter()

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Check supply parameter file*/
/*~K*/
/*~+:Check supply parameter file*/
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
/*~+:Controller*/
/*~T*/
//#define _#_()           f_Pruefen_VSP_Parameter()
#define vs_chk_parameter()           f_Pruefen_VSP_Parameter()

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Read supply parameter file*/
/*~K*/
/*~+:Read supply parameter file*/
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
/*~+:Controller*/
/*~T*/
//#define _#_()           f_Lesen_VSP_Parameter()
#define vs_read_parameter()           f_Lesen_VSP_Parameter()

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:End VS-PLUS parameters - Release memory on controlled shut-down*/
/*~K*/
/*~+:End VS-PLUS parameters - Release memory on controlled shut-down*/
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
/*~+:Controller*/
/*~T*/
//#define _#_()           f_Ende_VSP_Parameter()
#define vs_free_parameter()           f_Ende_VSP_Parameter()
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTIONS FOR VS-PLUS COMMANDS*/
/*~K*/
/*~+:FUNCTIONS FOR VS-PLUS COMMANDS*/
/*~A*/
/*~+:New command file?*/
/*~K*/
/*~+:New command file?*/
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
/*~+:Controller*/
/*~T*/
#define l_Neue_Befehle()                Neue_Befehle() 

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Accessing the command file*/
/*~K*/
/*~+:Accessing the command file*/
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
/*~+:Controller*/
/*~T*/
#define l_Oeffnen_VSP_Befehle()         Oeffnen_VSP_Befehle()
#define l_Lesen_VSP_Befehle(x,y)        Lesen_VSP_Befehle(x,y)
#define l_Schliessen_VSP_Befehle()      Schliessen_VSP_Befehle()

/*~T*/
//#define _#_()                           l_VABefehlPfad()
#define vs_ocit_path()                           l_VABefehlPfad()

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:ACCESSING PROCESS DATA*/
/*~K*/
/*~+:ACCESSING PROCESS DATA*/
/*~A*/
/*~+:Is there a program that is parameterized for VS-PLUS?*/
/*~K*/
/*~+:Is there a program that is parameterized for VS-PLUS?*/
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
/*~+:Controller*/
/*~T*/
//#define  _#_(x)         l_Prog_VSP(x)
#define  vs_prog_chk(x)         l_Prog_VSP(x)

/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Controller asks for VS-PLUS version text*/
/*~K*/
/*~+:Controller asks for VS-PLUS version text*/
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
/*~+:Controller*/
/*~K*/
/*~+:short f_versions_txt(char* text, int size)*/
/*~+:*/
/*~T*/
//#define _#_()                   f_versions_txt(x,y)
#define vs_version(x,y)                   f_versions_txt(x,y)
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:Asking for PD values*/
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
/*~+:Controller*/
/*~K*/
/*~+:U_WORD f_VSP_ProzessDaten(void *px, void *py)*/
/*~T*/
//#define _#_()                   f_VSP_ProzessDaten(x,y)
#define vs_read_process_data(x,y)                   f_VSP_ProzessDaten(x,y)
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:READ PT MEMORY*/
/*~I*/
#ifdef _VSPLUS_SIM_
/*~A*/
/*~+:Simulation (inactive)*/
/*~T*/
#define l_OEVSpeicherLesenEin()         OEVSpeicherLesenEin()
/*~T*/
#define f_OEVSpeicherAusgabe(x,y)       OEVSpeicherAusgabe(x,y)
/*~E*/
/*~O*/
/*~-*/
#else
/*~A*/
/*~+:Controller*/
/*~K*/
/*~+:Controller defines if reading of PT memory (AMLi telegram) is requires*/
/*~+:0 = don't read, 1 = read*/
/*~+:*/
/*~T*/
#define l_OEVSpeicherLesenEin()         0
/*~K*/
/*~+:AMLi telegram*/
/*~+:void f_OEVSpeicherAusgabe(char* oev_daten, short typ)*/
/*~+:type 1 = AMLi telegram header (single output only)*/
/*~+:     0 = AMLi telegram data*/
/*~+:Format: NR  TT.MO HH:MM:SS  MPN  LLLKK RRR PZH FAHRP  TX SP PH-UE TWF RTE GNE*/
/*~+:*/
/*~T*/
#define f_OEVSpeicherAusgabe(x,y)       0
/*~E*/
/*~-*/
#endif
/*~E*/
/*~E*/
/*~A*/
/*~+:SWAP BYTES*/
/*~K*/
/*~+:SWAP BYTES*/
/*~I*/
#ifdef _PROZESSOR_INTEL_  
/*~A*/
/*~+:Intel processor*/
/*~T*/
/*
Call m_BC in order to swap bytes
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
/*~+:Not an Intel processor*/
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
