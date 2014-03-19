/*~-*/
/*~XSF_LANGUAGE: C/C++*/
/*~K*/
/*~+:*/
/*~+:              VV         VV     SSSSSS     PPPPPPPPP*/
/*~+:              VV         VV   SSS    SSS   PP       PP*/
/*~+:              VV         VV   SS      SS   PP       PPP*/
/*~+:              VV         VV    SS          PP       PP*/
/*~+:               VV       VV        SS       PPPPPPPPP*/
/*~+:                VV     VV           SS     PP*/
/*~+:                 VV   VV      SS      SS   PP*/
/*~+:                  VV VV       SSS    SSS   PP*/
/*~+:                   VVV          SSSSSS     PPLUS*/
/*~+:*/
/*~+:*/
/*~+:              INCLUDE FILE F‹R PROJEKTSPEZIFISCHE MAXIMALWERTE*/
/*~+:*/
/*~+:*/
/*~+:       Filename       : if626max.inc*/
/*~+:       Version        : 6.2.6*/
/*~+:       Datum          : 15.08.2012*/
/*~+:       Programmierer  : P. Herren*/
/*~+:*/
/*~+:*/
/*~+:       Inhalt :*/
/*~+:       ~~~~~~~~~~~~~~~~*/
/*~+:        if62xmax enth‰lt die Maximawerte f¸r alle Feldgrˆssen f¸r das Steuerger‰t*/
/*~+:*/
/*~+:*/
/*~I*/
#ifndef _if626max
#define _if626max

#define SGMAX 64u

/* øøø */
#define NUL         0     
#define MaxInt      32767 

#ifdef _VSPLUS_SIM_

#define zeit_fak       10  /* øøøø    (1/10s)   */
#define l_vsp_serien() 1   /* Aufrufe pro Sekunde     */
#define t_ea_kont()    1   /* Zeit seit letztem Schalten */
#define geraete_fak    1   /* Geraetekonstante        */

#else
#define zeit_fak       10  /* Zeitfaktor    (1/10s)   */
#define l_vsp_serien() 1   /* Aufrufe pro Sekunde     */
#define t_ea_kont()    1   /* Zeit seit letztem Schalten */
#define geraete_fak    1   /* Geraetekonstante        */

#endif

/* øøø */
#define PRG_MAX           33

/*~E*/
/*~A*/
/*~+:Datens‰tzeøø*/
/*~K*/
/*~+:Definitionen der Satzdimensionen*/
/*~+:*/
/*~K*/
/*~+:Anzahl Parameter-Gruppen und S‰tze f¸r Datenhaltung (PIS626)*/
/*~T*/
#define MAXSAETZE             41 
#define MAXPARAMETR           80 

/*~+:Anzahl DetektorparametersÑtze*/
/*øøøø*/
#define DP_MAX                 4
/*~+:Anzahl Verkehrsstromparameter*/
/*øøøøø*/
#define VP_MAX                 8
/*~K*/
/*~+:Anzahl DatensÑtze Bildparameter rangabhÑngig*/
/*~T*/
#define BPR_MAX                8
/*~K*/
/*~+:Anzahl DatensÑtze Bildparameter verkehrsstromabhÑngig*/
/*~T*/
#define BPV_MAX                8
/*~K*/
/*~+:Anzahl DatensÑtze Bildparameter verkehrsstromabhÑngig*/
/*~T*/
#define IF_MAX                 4

/*~K*/
/*~+:Anzahl definierte RahmensignalplÑne*/
/*~T*/
#define RSP_MAX                16

/*~K*/
/*~+:Anzahl Saetze f¸er Plausibiltaet*/
/*~T*/
#define PL_MAX                  4
/*~K*/
/*~+:Anzahl der spezial Timer*/
/*~T*/
#define SPEZMAX                16

/*~K*/
/*~+:Netzmodule*/
/*~T*/
#define NETCOMMAND_MAX            2

/*~E*/
/*~A*/
/*~+:DP Detektorparameter*/
/*~K*/
/*~+:Detektoren*/
/*~K*/
/*~+:Anzahl der parallelen EingÑngen*/
/*~T*/
#define DETMAX          340u

/*~K*/
/*~+:Gesammtzahl der verwendeten Detektoren und Meldepnkte*/
/*~T*/
#define EINGANGMAX_OEVSP      (OSp_VSMAX*OSp_MPMAX)
#define EINGANGMAX      (DETMAX+EINGANGMAX_OEVSP)

/*~K*/
/*~+:Anzahl Detektoren fÅr IV Auswertung*/
/*~T*/
#define Det_IV_MAX      DETMAX

/*~K*/
/*~+:Anzahl Detektoren fÅr OeV Auswertung*/
/*~T*/
#define Det_OEV_MAX     230u

/*~K*/
/*~+:Anzahl Logische Verknuepfungen fuer Detektoren*/
/*~T*/
#define MAX_LOGI_DET    20
#define MAX_LOG_OBJ_DET 16
#define MAX_LOG_OBJ_1    8
#define MAX_LOG_OBJ_2    4
#define MAX_LOG_OBJ_3    2
#define MAX_LOG_OBJ_4    1

/*~E*/
/*~A*/
/*~+:VP Verkehrsstromparameter*/
/*~K*/
/*~+:Verkehrsstrîme*/
/*~T*/
#define VSMAX             64u

/*~K*/
/*~+:OeV Verkehrsstrîme*/
/*~T*/
#define VS_OEVMAX         45u

/*~K*/
/*~+:Anzahl definierte VorlÑufe*/
/*~T*/
#define VLMAX             20

/*~K*/
/*~+:Anzahl definierte NachlÑufe*/
/*~T*/
#define NLMAX             20

/*~K*/
/*~+:Anzahl passive VerlÑngerungen*/
/*~T*/
#define PVMAX             20

/*~K*/
/*~+:Weitergabe der Anmeldung*/
/*~T*/
#define WEITERMAX         20

/*~K*/
/*~+:Anzahl Verriegelungen zwischen VS*/
/*~T*/
#define VRMAX             50

/*~K*/
/*~+:öbergÑnge*/
/*~T*/
#define UeGMAX_MAL        (VSMAX*FEIMAX)
#define UeGMAX            (UeGMAX_MAL+1)

/*~K*/
/*~+:Setzbare Kriterien zur Priorisierung von ôV*/
/*~T*/
#define KrtMAX            8

/*~K*/
/*~+:Anzahl RÅckhalterelationen von ôV*/
/*~T*/
#define ZRMAX             30

/*~K*/
/*~+:Anzahl Logische Verkn¸pfungen fuer VS */
/*~T*/
#define MAX_LOGI_VS       20
#define MAX_LOG_OBJ_VS     4

/*~K*/
/*~+:Anzahl ZSondereingriff Verkehrsstroeme*/
/*~T*/
#define ZSONDERVSMAX      32u
#define ZSONDERINSTANZMAX  3

/*~E*/
/*~A*/
/*~+:BP Bildparameter*/
/*~K*/
/*~+:Hauptreihe*/
/*~T*/
#define HRMAX                        64
#define HR1MAX                       HRMAX
#define HR2MAX                       HRMAX
#define HR3MAX                       HRMAX

/*~K*/
/*~+:Grîsse des Wunschbildes*/
/*~T*/
#define WBLAENGE                    (VSMAX+PEMAX+1)

/*~K*/
/*~+:Anzahl der PrioritÑtselemente*/
/*~T*/
#define PEMAX                       6

/*~K*/
/*~+:Anzahl der PrioritÑtsklassen*/
/*~T*/
#define PKMAX                       3

/*~K*/
/*~+:Anzahl der Nebnestrîme mit Anmeldung*/
/*~T*/
#define NS_mit_MAX                  24

/*~K*/
/*~+:Anzahl der Nebnestrîme ohne Anmeldung*/
/*~T*/
#define NS_ohne_MAX                 20

/*~K*/
/*~+:Anzahl der obligatorischen Nebnestrîme*/
/*~T*/
#define NS_obli_MAX                 5

/*~E*/
/*~A*/
/*~+:IF Interfaceparameter*/
/*~K*/
/*~+:Anzahl definierte Anzeigeelemente*/
/*~T*/
#define ANZEMAX            96

/*~K*/
/*~+:Anzahl Diagonalpfeile*/
/*~T*/
#define PFEIMAX            20

/*~K*/
/*~+:Anzahl Anze-Verriegelungen   Anzeigeelemente VS*/
/*~T*/
#define ANZEVRMAX          VRMAX*UN_MAX*UN_MAX
#define VSVRMAX            ANZEVRMAX

/*~E*/
/*~A*/
/*~+:OeV Modul*/
/*~K*/
/*~+:OeV Speicher Dimensionen*/
/*~T*/
#define OEV_PARA_SAETZE        OEV_ZEIT_PARA_SAETZE

/*~T*/
#define OSp_VSMAX              MAX_OEV_PARA_GRUPPEN
#define OSp_MPMAX              MAX_OEV_MP
#define OSp_MAX                (OSp_VSMAX*OSp_MPMAX)
#define OEVSP_MAX              OEV_PARA_SAETZE

/*~K*/
/*~+:Kennung f¸r OEV-Modul*/
/*~T*/
#define OEV_MODUL     8

/*~K*/
/*~+:Restliche Definitionen sind im OeV Modul 12.09.2006 / mtr*/
/*~E*/
/*~A*/
/*~+:Definitionen Vektorbreite*/
/*~K*/
/*~+:* ******************************************************************** **/
/*~+:*           ---- Definitionen fÅr Tabellen (fix) ----                  **/
/*~+:* ******************************************************************** **/
/*~+:*/
/*~K*/
/*~+:Anzahl definierte Detektoren pro VS*/
/*~T*/
#define DVSMAX                   14

/*~K*/
/*~+:Anzahl definierte feindliche VS*/
/*~T*/
#define FEIMAX                   50

/*~K*/
/*~+:Anzahl Anze unbedingt*/
/*~T*/
#define UN_MAX                   3

/*~K*/
/*~+:Anzahl Anze bedingt UND*/
/*~T*/
#define UND_MAX                  3

/*~K*/
/*~+:Anzahl Anze bedingt ODER*/
/*~T*/
#define ODER_MAX                 3

/*~K*/
/*~+:Freie Programmierung*/
/*~T*/
#define FP_PARA_MAX            16
#define FP_ITEM_MAX            16

/*~K*/
/*~+:Freie Programmierung Net*/
/*~T*/
#define FP_PARA_MAX_NET        16
#define FP_ITEM_MAX_NET        16

/*~E*/
/*~A*/
/*~+:Adaptiv 1 (Vmod)*/
/*~K*/
/*~+:Modifikation*/
/*~T*/
#define MODTIMER           1        /* Nummer des Timer f¸r Modifikation        */
#define MAX_MOD           40        /* Maximale Anzahl Modifikationen           */
#define MOD_HIST_ANZ       8        /* Anzahl f¸r History der Modifikationen    */
#define MOD_SPERR_ANZ      8        /* Anzahl f¸r Sperrn der Modifikationen     */

/*~T*/

/*~E*/
/*~A*/
/*~+:Plausibilitaet*/
/*~T*/
#define MAX_PL_STGDET   20
#define MAX_PL_VS       20
#define MAX_PL_VSPDET   20
#define MAX_PL_RES_STD  12

/*~T*/

/*~E*/
/*~A*/
/*~+:Spezielle Faelle*/
/*~K*/
/*~+:Timer fuer die VS-PLUS Laufzeit*/
/*~T*/
#define VSPLUSLAUFZEIT 2

/*~E*/
/*~A*/
/*~+:Zaehlen von Objekten*/
/*~K*/
/*~+:Grenzwerte*/
/*~T*/
#define KleinstesIntervall 3
#define GroesstesIntervall 12
#define AnzahlZaehlBloecke 15
#define MaximaleIntervallLaenge 18000

/*~E*/
/*~A*/
/*~+:EPICS*/
/*~K*/
/*~+:Maximale Anzahl der Phasenuebergaenge*/
/*~T*/
#define e_MAXPUE 30

/*~K*/
/*~+:Maximale Anzahl der Phasenuebergaenge (+1 ?)*/
/*~T*/
#define MAXPUE e_MAXPUE

/*~K*/
/*~+:Maximale Anzahl von Spuren in einer Zufahrt*/
/*~T*/
#define MAXSPUR 5

/*~K*/
/*~+:Maximale Anzahl Sub-queues bzw. Meldepunktpaare pro queue*/
/*~T*/
#define MAXSUBQ 4

/*~K*/
/*~+:Hoechster Signalprogrammindex (1..98)*/
/*~T*/
#define MAXPRGINDEX 6

/*~K*/
/*~+:Maximale Anzahl der Phasen*/
/*~T*/
#define e_MAXPHAS 20
/*~K*/
/*~+:Maximale Anzahl der Phasen  (+1 ?)*/
/*~T*/
#define MAXPHAS e_MAXPHAS

/*~K*/
/*~+:Maximale Anzahl EPICS Errors ueber PD*/
/*~T*/
#define MAXEPICSERRORSpd 6
/*~E*/
/*~A*/
/*~+:AmPhA*/
/*~T*/
#define         MAX_SG_AMPHA    30
/*~E*/
/*~-*/
#endif
/*~E*/
