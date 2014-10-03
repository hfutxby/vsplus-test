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
/*~+:              INCLUDE FILE Steuerung von VS-PLUS*/
/*~+:*/
/*~+:*/
/*~+:*/
/*~+:       Filename       : vsp626ste.inc*/
/*~+:       Version        : 6.2.6*/
/*~+:       Datum          : 15.0.2012*/
/*~+:       Programmierer  : P. Herren*/
/*~+:*/
/*~+:*/
/*~+:       Inhalt :*/
/*~+:       ~~~~~~~~~~~~~~~~*/
/*~+:        In diesem File sind die Definitionen für die VS-PLUS Ansteuerung für Steuergeräte enthalten*/
/*~+:*/
/*~+:*/
/*~I*/
#ifndef  _vsp626steuerung
/*~T*/
#define _vsp626steuerung
/*~A*/
/*~+:Steuerung VS-PLUS*/
/*~K*/
/*~+:VS-PLUS Soll Typ*/
/*~T*/
typedef enum {
              VSP_AUS            = 0,
              VSP_EIN            = 1,
              VSP_NEU            = 2,
              VSP_WUNSCH_AUS_UM  = 3,
              VSP_NEU_INI        = 4,
              VSP_ND             = 99
            } VSPSollTyp;
/*~K*/
/*~+:Wunschbild Type*/
/*~T*/
typedef   unsigned char    WBTyp;

/*~K*/
/*~+:Wunschbild Ready Typ*/
/*~T*/
typedef   unsigned char   WBReadyTyp;

/*~E*/
/*~A*/
/*~+:Speicherverwaltung  */
/*~K*/
/*~+:Definition ID gepufferter Speicher für Parameter und OEV-Modul*/
/*~T*/
#define         PAR_STAT        1
#define         PAR_MEM_1       2
#define         PAR_MEM_2       3
#define         OEV_MEM_1       4
#define         OEV_MEM_2       5

/*~T*/
typedef struct {
                int     Init_1;
                int     Init_2;
                int     ParameterGeprueft;
                int     NeueParameter;
                int     FehlerInit;
                int     FehlerPruefen;
                int     FehlerLesen;
                int     AktiveDaten;
                int     PasiveDaten;
                int     VSPLUSMainInit;
                int     AmVersorgen;
} StatusVSPLUSParameter;
/*~T*/

/*~E*/
/*~T*/

/*~-*/
#endif
/*~E*/
