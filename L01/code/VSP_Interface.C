/*~-*/
/*~XSF_LANGUAGE: C/C++*/
/*~K*/
/*~+:*/
/*~+:              VV         VV     SSSSSS         PPPPPPPPP*/
/*~+:              VV         VV   SSS    SSS       PP       PP*/
/*~+:              VV         VV   SS      SS       PP       PPP*/
/*~+:              VV         VV    SS              PP       PP*/
/*~+:               VV       VV        SS      ###  PPPPPPPPP*/
/*~+:                VV     VV           SS         PP*/
/*~+:                 VV   VV      SS      SS       PP*/
/*~+:                  VV VV       SSS    SSS       PP*/
/*~+:                   VVV          SSSSSS         PPLUS*/
/*~+:*/
/*~+:                                  ON*/
/*~+:*/
/*~+:*/
/*~+:                2222      000000     7777777777     000000  */
/*~+:              222   2    00    00    7      77     00    00 */
/*~+:                   22   00      00         77     00      00*/
/*~+:                  22    00      00        77      00      00*/
/*~+:                 22     0        0      7777      0        0*/
/*~+:                22      00      00      77        00      00*/
/*~+:               22       00      00     77         00      00*/
/*~+:              22         00    00     77           00    00 */
/*~+:             22222222     000000     77             000000  */
/*~+:*/
/*~+:*/
/*~+:*/
/*~+:       -----------------------------------------------------------*/
/*~+:       ¦                                                         |*/
/*~+:       ¦                     VS-PLUS Interface                   |*/
/*~+:       ¦                                                         |*/
/*~+:       -----------------------------------------------------------*/
/*~+:*/
/*~+:       Filename       : VSP_Interface.c*/
/*~+:       Version        : 6*/
/*~+:       Date          : 06.02.2014*/
/*~+:       Programmer  : Herren Peter*/
/*~+:*/
/*~+:       History:*/
/*~+:       ~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:*/
/*~+:       Contents:*/
/*~+:       ~~~~~~~~~~~~~~*/
/*~+:        */
/*~+:        - Interface functions for VS-PLUS.*/
/*~+:*/
/*~+:*/
/*~A*/
/*~+:include*/
/*~T*/
#include "basis626.inc"
#include "vsp626ste.inc"
/*~E*/
/*~A*/
/*~+:Timer / Timer_2*/
/*~A*/
/*~+:FUNCTION Timer*/
/*~A*/
/*~+:FUNCTION-HEADER Timer*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : Timer*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Execute VS-PLUS Timer functions*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                   funktion: call type*/
/*~+:   ->                   timer   : timer index*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  time in units of 100ms*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE Timer*/
/*~F*/
short int timer(short int funktion, short int timer)
/*~-*/
{
   /*~I*/
   if (timer > 0 && timer <= MAXTIMER)
   /*~-*/
   {
      /*~C*/
      switch (funktion)
      /*~-*/
      {
         /*~K*/
         /*~+:clear*/
         /*~F*/
         case 3:
         /*~-*/
         {
            /*~T*/
            break;
         /*~-*/
         }
         /*~E*/
         /*~K*/
         /*~+:stop and clear*/
         /*~F*/
         case 4:
         /*~-*/
         {
            /*~T*/
            break;
         /*~-*/
         }
         /*~E*/
         /*~K*/
         /*~+:stop*/
         /*~F*/
         case 5:
         /*~-*/
         {
            /*~T*/
            break;
         /*~-*/
         }
         /*~E*/
      /*~-*/
      }
      /*~E*/
      /*~T*/
      return Zeit;
   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION Timer_2*/
/*~A*/
/*~+:FUNCTION-HEADER Timer_2*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : Timer_2*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Execute VS-PLUS Timer_2 functions*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                   funktion: call type*/
/*~+:   ->                   timer   : timer index*/
/*~+:   ->                   wert    : timer start value*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  time in units of 100ms*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE Timer_2*/
/*~F*/
short int timer_2(short int funktion, short int timer, short int wert)
/*~-*/
{
   /*~I*/
   if (timer > 0 && timer <= MAXTIMER)
   /*~-*/
   {
      /*~C*/
      switch (funktion)
      /*~-*/
      {
         /*~K*/
         /*~+:load and start with wert (k)*/
         /*~F*/
         case 2:
         /*~-*/
         {
            /*~T*/
            break;
         /*~-*/
         }
         /*~E*/
      /*~-*/
      }
      /*~E*/
      /*~T*/
      return Zeit;
   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:Program functions*/
/*~A*/
/*~+:FUNCTION ProgrammAktuell - Actual signal-program*/
/*~A*/
/*~+:FUNCTION-HEADER ProgrammAktuell - Actual signal program*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : ProgrammAktuell*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Readout current signal program*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                   : */
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  Actual program number 0 - 255*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE ProgrammAktuell - Actual signal program*/
/*~F*/
short int ProgrammAktuell(void)
/*~-*/
{
   /*~T*/
   short int returnwert = 1;
   /*~T*/
   return returnwert;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION ProgrammWahl - Selected signal-program*/
/*~A*/
/*~+:FUNCTION-HEADER ProgrammWahl - Selected signal program*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : ProgrammWahl*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Readout selected signal program*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                   : */
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  Number of the new, not yet activated program 0 - 255*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE ProgrammWahl - Selected signal program*/
/*~F*/
short int ProgrammWahl(void)
/*~-*/
{
   /*~T*/
   return ProgWahl;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION Zykluszeit - Current cycle second (TX)*/
/*~A*/
/*~+:FUNCTION-HEADER Zykluszeit - Current cycle second (TX)*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : Zykluszeit*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Read current cycle second TX*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                   : */
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  Current cycle second in units of 100ms, rounded to seconds*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE Zykluszeit - Current cycle second (TX)*/
/*~F*/
short int Zykluszeit(void)
/*~-*/
{
   /*~T*/
   return (TX / 10) * 10;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION Umlaufzeit - Cycle time (TU)*/
/*~A*/
/*~+:FUNCTION-HEADER Umlaufzeit - Cycle time (TU)*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : Umlaufzeit*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Read cycle time TU*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                   : */
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  Cycle time in units of 100ms*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE Umlaufzeit - Cycle time (TU)*/
/*~F*/
short int Umlaufzeit(void)
/*~-*/
{
   /*~T*/
   return TU;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION int ProgrammWahlZentrale - ProgramChoiceCentral*/
/*~K*/
/*~+:1 = program number comes from central;*/
/*~+:0 = program number comes locally from controller*/
/*~+:Currently always 1, because we don't have a central-controller*/
/*~F*/
int ProgrammWahlZentrale(void)
/*~-*/
{
   /*~T*/
   return 1;

/*~-*/
}
/*~E*/
/*~E*/
/*~T*/

/*~E*/
/*~A*/
/*~+:Detector functions*/
/*~A*/
/*~+:FUNCTION d_imp - Impulse storage sum*/
/*~A*/
/*~+:FUNCTION-HEADER d_imp - Impulse storage sum*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : d_imp*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Readout impulse storage*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                det: detector channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  Number of rising slopes since last clear*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE d_imp - Impulse storage sum*/
/*~F*/
short int d_imp(short int det)
/*~-*/
{
   /*~I*/
   if (det > 0 && det < MAXDETEKTOR)
   /*~-*/
   {
      /*~T*/
      return Impulsspeicher;
   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION d_limp - Clear impulse storage*/
/*~A*/
/*~+:FUNCTION-HEADER d_limp - Clear impulse storage*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : d_limp*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Reset impulse storage*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                det: detector channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE d_limp - Clear impulse storage*/
/*~F*/
void d_limp(short int det)
/*~-*/
{
   /*~I*/
   if (det > 0 && det < MAXDETEKTOR)
   /*~-*/
   {
      /*~T*/
      Impulsspeicher = 0;
   /*~-*/
   }
   /*~E*/
   /*~T*/
   return;
   /*~T*/

/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION d_belga - Current occupancy degree*/
/*~A*/
/*~+:FUNCTION-HEADER d_belga - Current occupancy degree*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : d_belga*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Get the current occupancy rate as a percentage*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                det: detector channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  detector occupancy degree during the last second, in percent*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE d_belga - Current occupancy degree*/
/*~F*/
short int d_belga(short int det)
/*~-*/
{
   /*~I*/
   if (det > 0 && det < MAXDETEKTOR)
   /*~-*/
   {
      /*~T*/
      return BelegungsGradAktuell;
   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION d_stoer - Detector fault*/
/*~A*/
/*~+:FUNCTION-HEADER d_stoer - Detector fault*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : d_stoer*/
/*~+: Date             : 22.11.2011*/
/*~+: Programmer     : Sergej Ahlert*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.201#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  This function tells VS-PLUS whether a detector fault has been detected by the controller*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                det: detector channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  0: no fault; >0: fault*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE d_stoer - Detector fault*/
/*~F*/
short int d_stoer(short int det)
/*~-*/
{
   /*~I*/
   if (det > 0 && det < MAXDETEKTOR7)
   /*~-*/
   {
      /*~T*/
      return Stoerung;
   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION d_belgg - Smoothened occupancy degree*/
/*~A*/
/*~+:FUNCTION-HEADER d_belgg - Smoothened occupancy degree*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : d_belgg*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Get the smoothened occupancy rate as a percentage */
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                det: detector channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  smoothened occupancy degree, in percent (0 - 100%)*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE d_belg - Smoothened occupancy degree*/
/*~F*/
short int d_belgg(short int det)
/*~-*/
{
   /*~I*/
   if (det > 0 && det < MAXDETEKTOR)
   /*~-*/
   {
      /*~T*/
      return BelegungsgradGeglaettet;
   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION d_bl - Occupancy state*/
/*~A*/
/*~+:FUNCTION-HEADER d_blg - Occupancy state*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : d_blg*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Current occupancy state of the detector*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                det: detector channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  0: not occupied; >0: occupied*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE d_blg - Occupancy state*/
/*~F*/
short int d_blg(short int det)
/*~-*/
{
   /*~I*/
   if (det > 0 && det < MAXDETEKTOR_V7)
   /*~-*/
   {
      /*~T*/
      return Belegungszustand;
   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION d_ztlkn- Net time gap*/
/*~A*/
/*~+:FUNCTION-HEADER d_ztlkn - Net time gap*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : d_ztlkn*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Get net time gap in 1/10 s*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                det: detector channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  net time gap in units of 100ms*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE d_ztlkn - Net time gap*/
/*~F*/
short int d_ztlkn(short int det)
/*~-*/
{
   /*~I*/
   if (det > 0 && det < MAXDETEKTOR)
   /*~-*/
   {
      /*~T*/
      return NettoZeitluecke;
   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION d_zeitlb - Gross time gap*/
/*~A*/
/*~+:FUNCTION-HEADER d_zeitlb - Gross time gap*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : d_zeitlb*/
/*~+: Date             : 12.3.2014*/
/*~+: Programmer     : juc*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Get gross time gap in 1/10 s*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                det: detector channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  gross time gap in units of 100ms*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE d_zeitlb - Gross time gap*/
/*~F*/
short int d_zeitlb(short int det)
/*~-*/
{
   /*~I*/
   if (det > 0 && det < MAXDETEKTOR)
   /*~-*/
   {
      /*~T*/
      return BruttoZeitluecke;
   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION d_blgzt - Occupancy time*/
/*~A*/
/*~+:FUNCTION-HEADER d_blgzt - Occupancy time*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : d_blgzt*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Get actual occupancy time in 1/10 s*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                det: detector channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  detector occupancy time in units of 100ms*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE d_blgzt - Occupancy time*/
/*~F*/
short int d_blgzt(short int det)
/*~-*/
{
   /*~I*/
   if (det > 0 && det < MAXDETEKTOR)
   /*~-*/
   {
      /*~T*/
      return Belegungsdauer;
   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION d_impab - Sum of falling slopes*/
/*~K*/
/*~+:Parameters:*/
/*~+:->    type:   1: read counter (number of falling slopes)*/
/*~+:->     det:   detector channel number*/
/*~+:Return:*/
/*~+:Number of falling slopes since last clear*/
/*~+:*/
/*~F*/
short int d_impab(short int type, short int det)
/*~-*/
{
   /*~I*/
   if (det > 0 && det < MAXDETEKTOR)
   /*~-*/
   {
      /*~C*/
      switch (type)
      /*~-*/
      {
         /*~F*/
         case 1:
         /*~-*/
         {
            /*~T*/
            return AbfallendeImpulssumme;
            /*~T*/
            break;
         /*~-*/
         }
         /*~E*/
         /*~F*/
         case 2:
         /*~-*/
         {
            /*~T*/
            AbfallendeImpulssumme = 0;
            return 0;
            /*~T*/
            break;
         /*~-*/
         }
         /*~E*/
      /*~-*/
      }
      /*~E*/
   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION d_kvalue - Load, derived from traffic situation*/
/*~K*/
/*~+:Parameters:*/
/*~+:->     det: virtual detector (segment) channel number*/
/*~+:Return:*/
/*~+:traffic load in percent (0 - 100%)*/
/*~+:    */
/*~F*/
short int d_kvalue(short int det)
/*~-*/
{
   /*~I*/
   if (det > 0 && det < MAXDETEKTOR)
   /*~-*/
   {
      /*~T*/

   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:Signal-group functions*/
/*~A*/
/*~+:FUNCTION min_rot - Minimum red time*/
/*~A*/
/*~+:FUNCTION-HEADER min_rot - Minimum red time*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : min_rot*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Get the minimum red time (closed) defined in the controller*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                 sg: signal group channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  minimum red time in units of 100ms*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE min_rot - Minimum red time*/
/*~F*/
short int min_rot(short int sg)
/*~-*/
{
   /*~I*/
   if (sg > 0 && sg <= MAXSIGNALGRUPPE)
   /*~-*/
   {
      /*~T*/
      return bas_MindestRot;
   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION u_rot_gelb - Preparation time*/
/*~A*/
/*~+:FUNCTION-HEADER u_rot_gelb - Preparation time*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : u_rot_gelb*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Get transition time (from closed to open) time defined in the controller*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                 sg: signal group channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  preparation time in units of 100ms*/
/*~+:  0 if no transition time is available*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE u_rot_gelb - Preparation time*/
/*~F*/
short int u_rot_gelb(short int sg)
/*~-*/
{
   /*~I*/
   if (sg > 0 && sg <= MAXSIGNALGRUPPE)
   /*~-*/
   {
      /*~T*/
      return bas_RotGelb;
   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION min_gruen - Minimum green time*/
/*~A*/
/*~+:FUNCTION-HEADER min_gruen - Minimum green time*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : min_gruen*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Get the minimum green time (open) defined in the controller*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                 sg: signal group channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  minimum green time in units of 100ms*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE min_gruen - Minimum green time*/
/*~F*/
short int min_gruen(short int sg)
/*~-*/
{
   /*~I*/
   if (sg > 0 && sg <= MAXSIGNALGRUPPE)
   /*~-*/
   {
      /*~T*/
      return bas_MindestGruen;

   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION u_gelb - Amber time*/
/*~A*/
/*~+:FUNCTION-HEADER u_gelb - Amber time*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : u_gelb*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Get the transition time from open to closed defined in the controller*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                 sg: signal group channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  amber time in units of 100ms*/
/*~+:  0 if no transition time is available*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE u_gelb - Amber time*/
/*~F*/
short int u_gelb(short int sg)
/*~-*/
{
   /*~I*/
   if (sg > 0 && sg <= MAXSIGNALGRUPPE)
   /*~-*/
   {
      /*~T*/
      return bas_Gelb;
   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION SG_ein - Switch signal-group to open*/
/*~A*/
/*~+:FUNCTION-HEADER SG_ein - Switch signal-group to open*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : SG_ein*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Switch signal-group on (open)*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                 sg: signal group channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE SG_ein - Switch signal-group to open*/
/*~F*/
void SG_ein(short int sg)
/*~-*/
{
   /*~I*/
   if (sg > 0 && sg <= MAXSIGNALGRUPPE)
   /*~-*/
   {
      /*~T*/

   /*~-*/
   }
   /*~E*/
   /*~T*/
   return;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION SG_aus - Switch signal-group to closed */
/*~A*/
/*~+:FUNCTION-HEADER SG_aus - Switch signal-group to closed */
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : SG_aus*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Switch signal-group off (closed)*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                 sg: signal group channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE SG_aus - Switch signal-group to closed */
/*~F*/
void SG_aus(short int sg)
/*~-*/
{
   /*~I*/
   if (sg > 0 && sg <= MAXSIGNALGRUPPE)
   /*~-*/
   {
      /*~T*/

   /*~-*/
   }
   /*~E*/
   /*~T*/
   return;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION Relais_ein - Switch digital output on*/
/*~A*/
/*~+:FUNCTION-HEADER Relais_ein - Switch digital output on*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : Relais_ein*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Switch non-supervised output to "on"*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                 sg: output channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE Relais_ein - Switch digital output on*/
/*~F*/
void Relais_ein(short int sg)
/*~-*/
{
   /*~T*/
   return;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION Relais_aus - Switch digital output off*/
/*~A*/
/*~+:FUNCTION-HEADER Relais_aus - Switch digital output off*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : Relais_aus*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Switch a non-supervised output to "off"*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                 sg: output channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE Relais_aus - Switch digital output off*/
/*~F*/
void Relais_aus(short int sg)
/*~-*/
{
   /*~T*/
   return;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION Blinker_ein - Blinker on*/
/*~A*/
/*~+:FUNCTION-HEADER Blinker_ein - Blinker on*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : Blinker_ein*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Switch a non-supervised blinker output (blinking digital output) to "on"*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                 sg: output channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE Blinker_ein - Blinker on*/
/*~F*/
void Blinker_ein(short int sg)
/*~-*/
{
   /*~T*/
   return ;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION Blinker_aus - Blinker off*/
/*~A*/
/*~+:FUNCTION-HEADER Blinker_aus - Blinker off*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : Blinker_aus*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Switch a non-supervised blinker output (blinking digital output) to "off"*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                 sg: output channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE Blinker_aus - Blinker off*/
/*~F*/
void Blinker_aus(short int sg)
/*~-*/
{
   /*~T*/
   return;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION s_rot - Signal-group red state*/
/*~A*/
/*~+:FUNCTION-HEADER s_rot - Signal-group red state*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : s_rot*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  check if a signal group shows red*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                 sg: signal group channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  1 = signal group shows red; 0 = not red*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE s_rot - Signal-group red state*/
/*~F*/
short int s_rot(short int sg)
/*~-*/
{
   /*~I*/
   if (sg > 0 && sg <= MAXSIGNALGRUPPE)
   /*~-*/
   {
      /*~T*/

   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION s_sr_aus - Digital output off state*/
/*~A*/
/*~+:FUNCTION-HEADER s_sr_aus - Digital output off state*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : s_sr_aus*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  check if a digital output is switched off*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                 re: output channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  1 = output is off; 0 = output is not off*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE s_sr_aus - Digital output off state*/
/*~F*/
short int s_sr_aus(short int re)
/*~-*/
{
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION s_sb_aus - Digital blinker off state*/
/*~A*/
/*~+:FUNCTION-HEADER s_sb_aus - Digital blinker off state*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : s_sb_aus*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  check if a digital blinker output is switched off*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                bli: output channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  1 = output is off; 0 = output is not off*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE s_sb_aus - Digital blinker off state*/
/*~F*/
short int s_sb_aus(short int bli)
/*~-*/
{
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION s_min_rot - Minimum red state*/
/*~A*/
/*~+:FUNCTION-HEADER s_min_rot - Minimum red state*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : s_min_rot*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  check if a signal group shows red and the elapsed red time is still within the minimum red time*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                 sg: signal group channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  1 = signal group red and min red not reached; 0 = not red or min red elapsed*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE s_min_rot - Minimum red state*/
/*~F*/
short int s_min_rot(short int sg)
/*~-*/
{
   /*~I*/
   if (sg > 0 && sg <= MAXSIGNALGRUPPE)
   /*~-*/
   {
      /*~T*/

   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION s_gelb - Amber state*/
/*~A*/
/*~+:FUNCTION-HEADER s_gelb - Amber state*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : s_gelb*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  check if a signal group shows amber or in general if the signal group is in the "open-closed" transition*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                 sg: signal group channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  1 = signal group is in transition "open-closed"; 0 = not in transition*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE s_gelb - Amber state*/
/*~F*/
short int s_gelb(short int sg)
/*~-*/
{
   /*~I*/
   if (sg > 0 && sg <= MAXSIGNALGRUPPE)
   /*~-*/
   {
      /*~T*/

   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION s_grun - Green state*/
/*~A*/
/*~+:FUNCTION-HEADER s_grun - Green state*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : s_grun*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  check if a signal group shows green*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                 sg: signal group channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  1 = signal group shows green; 0 = not green*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE s_grun - Green state*/
/*~F*/
short int s_grun(short int sg)
/*~-*/
{
   /*~I*/
   if (sg > 0 && sg <= MAXSIGNALGRUPPE)
   /*~-*/
   {
      /*~T*/

   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION s_sr_ein - Digital output "on" state*/
/*~A*/
/*~+:FUNCTION-HEADER s_sr_ein - Digital output "on" state*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : s_sr_ein*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  check if a digital output is switched on*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                 re: output channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  1 = output is on; 0 = output is not on*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE s_sr_ein - Digital output "on" state*/
/*~F*/
short int s_sr_ein(short int re)
/*~-*/
{
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION s_sb_ein - Digital blinker "on" state*/
/*~A*/
/*~+:FUNCTION-HEADER s_sb_ein - Digital blinker "on" state*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : s_sb_ein*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  check if a digital blinker output is switched on*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                bli: output channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  1 = output is on; 0 = output is not on*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE s_sb_ein - Digital blinker "on" state*/
/*~F*/
short int s_sb_ein(short int bli)
/*~-*/
{
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION s_min_grun - Minimum green state*/
/*~A*/
/*~+:FUNCTION-HEADER s_min_grun - Minimum green state*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : s_min_grun*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  check if a signal group shows green and the elapsed green time is still within the minimum green time*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                 sg: signal group channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  1 = signal group green and min green not reached; 0 = not green or min green elapsed*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE s_min_grun - Minimum green state*/
/*~F*/
short int s_min_grun(short int sg)
/*~-*/
{
   /*~I*/
   if (sg > 0 && sg <= MAXSIGNALGRUPPE)
   /*~-*/
   {
      /*~T*/

   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION s_vor - Preparation state*/
/*~A*/
/*~+:FUNCTION-HEADER s_vor - Preparation state*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : s_vor*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  check if a signal group shows red-amber or in general if the signal group is in the "closed-open" transition*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                 sg: signal group channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  1 = signal group is in transition "closed-open"; 0 = not in transition*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE s_vor - Preparation state*/
/*~F*/
short int s_vor(short int sg)
/*~-*/
{
   /*~I*/
   if (sg > 0 && sg <= MAXSIGNALGRUPPE)
   /*~-*/
   {
      /*~T*/

   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION s_stoeblink - Fault blinking state*/
/*~A*/
/*~+:FUNCTION-HEADER s_stoeblink - Fault blinking state*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : s_stoeblink*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  check if the signal group is in fault mode*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                 sg: signal group channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  1 = signal group in fault mode; 0 = not in fault mode*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE s_stoeblink - Fault blinking state*/
/*~F*/
short int s_stoeblink(short int sg)
/*~-*/
{
   /*~I*/
   if (sg > 0 && sg <= MAXSIGNALGRUPPE)
   /*~-*/
   {
      /*~T*/

   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION s_t_rot - Current red time*/
/*~A*/
/*~+:FUNCTION-HEADER s_t_rot - Current red time*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : s_t_rot*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Current red duration*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                 sg: signal group channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  current red time in units of 100ms*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE s_t_rot - Current red time*/
/*~F*/
unsigned short s_t_rot(short int sg)
/*~-*/
{
   /*~I*/
   if (sg > 0 && sg <= MAXSIGNALGRUPPE)
   /*~-*/
   {
      /*~T*/

   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION s_t_gelb - Current amber time*/
/*~A*/
/*~+:FUNCTION-HEADER s_t_gelb - Current amber time*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : s_t_gelb*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Current amber duration (transition state from open to closed)*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                 sg: signal group channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  current amber time in units of 100ms*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE s_t_gelb - Current amber time*/
/*~F*/
unsigned short int s_t_gelb(short int sg)
/*~-*/
{
   /*~I*/
   if (sg > 0 && sg <= MAXSIGNALGRUPPE)
   /*~-*/
   {
      /*~T*/

   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION s_t_min_rot - Current minimum red time*/
/*~A*/
/*~+:FUNCTION-HEADER s_t_min_rot - Current minimum red time*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : s_t_min_rot*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Current duration of minimum red state*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                 sg: signal group channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  current minimum red time in units of 100ms*/
/*~+:  0 after minimum red time elapsed*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE s_t_min_rot - Current minimum red time*/
/*~F*/
unsigned short int s_t_min_rot(short int sg)
/*~-*/
{
   /*~I*/
   if (sg > 0 && sg <= MAXSIGNALGRUPPE)
   /*~-*/
   {
      /*~T*/

   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION s_t_grun - Current green time*/
/*~A*/
/*~+:FUNCTION-HEADER s_t_grun - Current green time*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : s_t_grun*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Current green duration*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                 sg: signal group channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  current green time in units of 100ms*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE s_t_grun - Current green time*/
/*~F*/
unsigned short int s_t_grun(short int sg)
/*~-*/
{
   /*~I*/
   if (sg > 0 && sg <= MAXSIGNALGRUPPE)
   /*~-*/
   {
      /*~T*/

   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION s_t_vor - Current preparation time*/
/*~A*/
/*~+:FUNCTION-HEADER s_t_vor - Current preparation time*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : s_t_vor*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Current red-amber time (transition from closed to open)*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                 sg: signal group channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  current red-amber time in units of 100ms*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE s_t_vor - Current preparation time*/
/*~F*/
unsigned short int s_t_vor(short int sg)
/*~-*/
{
   /*~I*/
   if (sg > 0 && sg <= MAXSIGNALGRUPPE)
   /*~-*/
   {
      /*~T*/

   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION s_t_min_grun - Current minimum green time*/
/*~A*/
/*~+:FUNCTION-HEADER s_t_min_grun - Current minimum green time*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : s_t_min_grun*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Current duration of minimum green state*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                 sg: signal group channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  actual minimum green time in units of 100ms*/
/*~+:  0 after minimum green time elapsed*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE s_t_min_grun - Current minimum green time*/
/*~F*/
unsigned short int s_t_min_grun(short int sg)
/*~-*/
{
   /*~I*/
   if (sg > 0 && sg <= MAXSIGNALGRUPPE)
   /*~-*/
   {
      /*~T*/

   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION s_HW_VspFreigegeben - Enabled for VS-PLUS*/
/*~A*/
/*~+:FUNCTION-HEADER s_HW_VspFreigegeben - Enabled for VS-PLUS*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : s_HW_VspFreigegeben*/
/*~+: Date             : 10.05.2004*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  Check if a signal group is enabled for VS-PLUS (under VS-PLUS control)*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                 sg: signal group channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  1 = signal group is enabled; 0 = not enabled*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE s_HW_VspFreigegeben - Enabled for VS-PLUS*/
/*~F*/
unsigned short int s_HW_VspFreigegeben(short int sg)
/*~-*/
{
   /*~I*/
   if (sg > 0 && sg <= MAXSIGNALGRUPPE)
   /*~-*/
   {
      /*~T*/

   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 1;

/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION s_zwi_zeit - Controller intergreen*/
/*~A*/
/*~+:FUNCTION-HEADER s_zwi_zeit - Controller intergreen*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : s_zwi_zeit*/
/*~+: Date             : 10.05.2004*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  read controller intergreen*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                sgR: Clearing signal group channel number*/
/*~+:   ->                sgE: Entering signal group channel number*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  controller intergreen time in units of 100ms*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE s_zwi_zeit - Controller intergreen*/
/*~F*/
short s_zwi_zeit(short sgR, short sgE)
/*~-*/
{
   /*~T*/
   return 32767;      /* Nicht Feindlich                */

/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:Error messages*/
/*~A*/
/*~+:FUNCTION Meldung - VS-PLUS messages*/
/*~A*/
/*~+:FUNCTION-HEADER Meldung - VS-PLUS messages */
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : Meldung*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  write a predefined numerical message to the controller*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->             degree:  message criticality*/
/*~+:   ->                 nr:  message number*/
/*~+:   ->        par1 - par4:  message parameters*/
/*~+:: */
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE Meldung - VS-PLUS messages*/
/*~F*/
void Meldung(short int degree, short int nr, short int par1, short int par2, short int par3, short int par4)
/*~-*/
{
   /*~T*/
   char Zeile[200];

   /*~T*/
   sprintf(Zeile, "No: %i   p1: %i   p2: %i   p3: %i   p4: %i \r",nr, par1, par2, par3, par4);
   Log(0, Zeile);
   /*~T*/
   return;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION MeldungNET - VS-PLUS messages from network control */
/*~A*/
/*~+:FUNCTION-HEADER MeldungNET - VS-PLUS messages from network control  */
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : MeldungNET*/
/*~+: Date             : 3.11.2009*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.201#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  write a predefined numerical network control message to the controller*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->             degree:  message criticality*/
/*~+:   ->                 nr:  message number*/
/*~+:   ->                Anr:  mission number*/
/*~+:   ->        par1 - par5:  message parameters*/
/*~+:: */
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE MeldungNET - VS-PLUS messages from network control */
/*~F*/
void  MeldungNET(short int degree, unsigned char nr, unsigned short Anr, unsigned char par1, unsigned char par2, unsigned char par3, unsigned char par4, unsigned char par5)

/*~-*/
{
   /*~T*/
   char Zeile[200];

   /*~I*/
   if (nr >= 80 && nr <= 84)
   /*~-*/
   {
      /*~T*/
      sprintf(Zeile, "VSPnet Info Nr.: %d  Anr: %d  Par1=%d  Par2=%d  Par3=%d  Par4=%d  Par5=%d \r", nr, Anr, par1, par2, par3, par4, par5);
   /*~-*/
   }
   /*~O*/
   /*~-2*/
   else
   {
      /*~T*/
      sprintf(Zeile, "VSPnet Warnung Nr.: %d  Anr: %d  Par1=%d  Par2=%d  Par3=%d  Par4=%d  Par5=%d \r", nr, Anr, par1, par2, par3, par4, par5);
   /*~-*/
   }
   /*~E*/
   /*~T*/
   Log(0, Zeile);
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION U_Kontrolle - Cycle control*/
/*~A*/
/*~+:FUNCTION-HEADER U_Kontrolle - Cycle control*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : U_Kontrolle*/
/*~+: Date             : 29.10.1998*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  VS-PLUS tells the controller the number of the traffic stream with the largest waiting time*/
/*~+:*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->                 vs:   number of the traffic stream with the largest waiting time 0 - VSMAX*/
/*~+:   ->               zeit:   waiting time in units of 100ms*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE U_Kontrolle - Cycle control*/
/*~F*/
void U_Kontrolle(short int vs, short int zeit)
/*~-*/
{
   /*~I*/
   if (vs > 0 && vs < VSMAX && zeit > 2400)
   /*~-*/
   {
      /*~I*/
      if (vs != VSalt)
      /*~-*/
      {
         /*~T*/
         MELDUNG(1, (short)998, (short)vs, (short)zeit, (short)0, (short)0);

         /*~T*/
         VSalt = vs;
      /*~-*/
      }
      /*~E*/
   /*~-*/
   }
   /*~E*/
   /*~T*/
   return;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~T*/

/*~E*/
/*~A*/
/*~+:System functions*/
/*~A*/
/*~+:FUNCTION TelegrammVomGeraet - Serial telegram*/
/*~A*/
/*~+:FUNCTION-HEADER TelegrammVomGeraet - Serial telegram*/
/*~K*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~+: FUNCTION          : */
/*~+: Date             : 07.08.2000*/
/*~+: Programmer     : Herren Peter*/
/*~+:*/
/*~+:*/
/*~+: Change-log:*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:*/
/*~+:  Date            : ##.##.199#*/
/*~+:  Programmer    : <Programmer name>*/
/*~+:  Change-log       : <Changes made>*/
/*~+:*/
/*~+:*/
/*~+: Task :*/
/*~+: ~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:  Read serial PT telegrams that have been received by the controller*/
/*~+:*/
/*~+: Solution :*/
/*~+: ~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Parameters :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  <-                    : */
/*~+:   ->       oev_tele_poi:   pointer to a telegram (struct R09serialTelegram)*/
/*~+:  <->                   : */
/*~+:*/
/*~+:*/
/*~+: Global variables :*/
/*~+: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~+:  */
/*~+:*/
/*~+:*/
/*~+: Return:*/
/*~+: ~~~~~~~~~~~~~~*/
/*~+:  1 = telegram available; 0 = none available*/
/*~+:*/
/*~+:************************************************************************************************************/
/*~+:*/
/*~E*/
/*~A*/
/*~+:FUNCTION-SOURCE TelegrammVomGeraet - Serial telegram*/
/*~F*/
short TelegrammVomGeraet(void* oev_tele_poi)
/*~-*/
{
   /*~T*/
   short ret = 0;
   /*~T*/
   return ret;
/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION s_SteuerungNichtAktiv - Control not active*/
/*~K*/
/*~+:check if VS-PLUS control is not active*/
/*~+:Return:*/
/*~+:1 = Off or switching on or off; 0 = signal program active*/
/*~+:*/
/*~F*/
unsigned short int s_SteuerungNichtAktiv(void)

/*~-*/
{
   /*~T*/
   return 0;

/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:Data handling*/
/*~A*/
/*~+:FUNCTION Allozieren_VSP_Speicher - Memory allocation*/
/*~K*/
/*~+:Request memory from the controller in order to save VS-PLUS parameters*/
/*~+:Parameters:*/
/*~+:->      _sizeof: requested memory size*/
/*~+:->           id: memory area identification (1 - 3)*/
/*~+:Return:*/
/*~+:Pointer to memory area*/
/*~+:*/
/*~F*/
void* Allozieren_VSP_Speicher(int _sizeof, int id)
/*~-*/
{
   /*~T*/
   void* _VSPD;

   /*~C*/
   switch (id)
   /*~-*/
   {
      /*~F*/
      case PAR_STAT:
      /*~-*/
      {
         /*~I*/
         if (!mm_hdl_VSPLUS_Data_1)
         /*~-*/
         {
            /*~T*/
            mm_hdl_VSPLUS_Data_1 = malloc(_sizeof);

         /*~-*/
         }
         /*~E*/
         /*~T*/
         _VSPD = mm_hdl_VSPLUS_Data_1;

         /*~T*/
         break;
      /*~-*/
      }
      /*~E*/
      /*~F*/
      case PAR_MEM_1:
      /*~-*/
      {
         /*~I*/
         if (!mm_hdl_VSPLUS_Data_2)
         /*~-*/
         {
            /*~T*/
            mm_hdl_VSPLUS_Data_2 = malloc(_sizeof);

         /*~-*/
         }
         /*~E*/
         /*~T*/
         _VSPD = mm_hdl_VSPLUS_Data_2;

         /*~T*/
         break;
      /*~-*/
      }
      /*~E*/
      /*~F*/
      case PAR_MEM_2:
      /*~-*/
      {
         /*~I*/
         if (!mm_hdl_VSPLUS_Data_3)
         /*~-*/
         {
            /*~T*/
            mm_hdl_VSPLUS_Data_3 = malloc(_sizeof);

         /*~-*/
         }
         /*~E*/
         /*~T*/
         _VSPD = mm_hdl_VSPLUS_Data_3;

         /*~T*/
         break;
      /*~-*/
      }
      /*~E*/
      /*~F*/
      case OEV_MEM_1:
      /*~-*/
      {
         /*~I*/
         if (!mm_hdl_VSPLUS_Data_4)
         /*~-*/
         {
            /*~T*/
            mm_hdl_VSPLUS_Data_4 = malloc(_sizeof);

         /*~-*/
         }
         /*~E*/
         /*~T*/
         _VSPD = mm_hdl_VSPLUS_Data_4;

         /*~T*/
         break;
      /*~-*/
      }
      /*~E*/
      /*~F*/
      case OEV_MEM_2:
      /*~-*/
      {
         /*~I*/
         if (!mm_hdl_VSPLUS_Data_5)
         /*~-*/
         {
            /*~T*/
            mm_hdl_VSPLUS_Data_5 = malloc(_sizeof);

         /*~-*/
         }
         /*~E*/
         /*~T*/
         _VSPD = mm_hdl_VSPLUS_Data_5;

         /*~T*/
         break;
      /*~-*/
      }
      /*~E*/
      /*~O*/
      /*~-2*/
      default:
      {
         /*~T*/
         return NULL;

      /*~-*/
      }
   /*~-*/
   }
   /*~E*/
   /*~T*/
   return (void*)_VSPD;
/*~-*/
}
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION Gib_VSP_Zeiger - Pointer to memory area*/
/*~K*/
/*~+:Get the pointer to the memory area*/
/*~+:Parameter:*/
/*~+:->      id: memory area identification (1 - 3)*/
/*~+:Return:*/
/*~+:pointer to memory area*/
/*~+:*/
/*~F*/
void* Gib_VSP_Zeiger(int id)
/*~-*/
{
   /*~T*/
   void* _VSPD = NULL;

   /*~C*/
   switch (id)
   /*~-*/
   {
      /*~F*/
      case PAR_STAT:
      /*~-*/
      {
         /*~I*/
         if (mm_hdl_VSPLUS_Data_1)
         /*~-*/
         {
            /*~T*/
            _VSPD = mm_hdl_VSPLUS_Data_1;

         /*~-*/
         }
         /*~E*/
         /*~T*/
         break;
      /*~-*/
      }
      /*~E*/
      /*~F*/
      case PAR_MEM_1:
      /*~-*/
      {
         /*~I*/
         if (mm_hdl_VSPLUS_Data_2)
         /*~-*/
         {
            /*~T*/
            _VSPD = mm_hdl_VSPLUS_Data_2;

         /*~-*/
         }
         /*~E*/
         /*~T*/
         break;
      /*~-*/
      }
      /*~E*/
      /*~F*/
      case PAR_MEM_2:
      /*~-*/
      {
         /*~I*/
         if (mm_hdl_VSPLUS_Data_3)
         /*~-*/
         {
            /*~T*/
            _VSPD = mm_hdl_VSPLUS_Data_3;

         /*~-*/
         }
         /*~E*/
         /*~T*/
         break;
      /*~-*/
      }
      /*~E*/
      /*~F*/
      case OEV_MEM_1:
      /*~-*/
      {
         /*~I*/
         if (mm_hdl_VSPLUS_Data_4)
         /*~-*/
         {
            /*~T*/
            _VSPD = mm_hdl_VSPLUS_Data_4;

         /*~-*/
         }
         /*~E*/
         /*~T*/
         break;
      /*~-*/
      }
      /*~E*/
      /*~F*/
      case OEV_MEM_2:
      /*~-*/
      {
         /*~I*/
         if (mm_hdl_VSPLUS_Data_5)
         /*~-*/
         {
            /*~T*/
            _VSPD = mm_hdl_VSPLUS_Data_5;

         /*~-*/
         }
         /*~E*/
         /*~T*/
         break;
      /*~-*/
      }
      /*~E*/
   /*~-*/
   }
   /*~E*/
   /*~T*/
   return (void*)_VSPD;
/*~-*/
}
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION Freigeben_VSP_Speicher - Free allocated memory area*/
/*~K*/
/*~+:Free memory that has been requested from the controller*/
/*~+:Parameter:*/
/*~+:->     id: memory area identification*/
/*~+:*/
/*~F*/
void Freigeben_VSP_Speicher(int id)
/*~-*/
{
   /*~C*/
   switch (id)
   /*~-*/
   {
      /*~F*/
      case PAR_STAT:
      /*~-*/
      {
         /*~I*/
         if (mm_hdl_VSPLUS_Data_1)
         /*~-*/
         {
            /*~T*/
            free(mm_hdl_VSPLUS_Data_1);

         /*~-*/
         }
         /*~E*/
         /*~T*/
         break;
      /*~-*/
      }
      /*~E*/
      /*~F*/
      case PAR_MEM_1:
      /*~-*/
      {
         /*~I*/
         if (mm_hdl_VSPLUS_Data_2)
         /*~-*/
         {
            /*~T*/
            free(mm_hdl_VSPLUS_Data_2);

         /*~-*/
         }
         /*~E*/
         /*~T*/
         break;
      /*~-*/
      }
      /*~E*/
      /*~F*/
      case PAR_MEM_2:
      /*~-*/
      {
         /*~I*/
         if (mm_hdl_VSPLUS_Data_3)
         /*~-*/
         {
            /*~T*/
            free(mm_hdl_VSPLUS_Data_3);

         /*~-*/
         }
         /*~E*/
         /*~T*/
         break;
      /*~-*/
      }
      /*~E*/
      /*~F*/
      case OEV_MEM_1:
      /*~-*/
      {
         /*~I*/
         if (mm_hdl_VSPLUS_Data_4)
         /*~-*/
         {
            /*~T*/
            free(mm_hdl_VSPLUS_Data_4);

         /*~-*/
         }
         /*~E*/
         /*~T*/
         break;
      /*~-*/
      }
      /*~E*/
      /*~F*/
      case OEV_MEM_2:
      /*~-*/
      {
         /*~I*/
         if (mm_hdl_VSPLUS_Data_5)
         /*~-*/
         {
            /*~T*/
            free(mm_hdl_VSPLUS_Data_5);

         /*~-*/
         }
         /*~E*/
         /*~T*/
         break;
      /*~-*/
      }
      /*~E*/
   /*~-*/
   }
   /*~E*/
   /*~T*/

/*~-*/
}
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION Oeffnen_VSP_Parameter - Open supply file (VCB)*/
/*~K*/
/*~+:Open the VCB file that has been sent to the controller*/
/*~+:Return:*/
/*~+:1 = supply file was opened successfully, 0 = not opened*/
/*~+:*/
/*~F*/
int Oeffnen_VSP_Parameter(void)
/*~-*/
{
   /*~T*/
   Fvcb = fopen (VCBDateinamen, "rb");
   /*~I*/
   if (Fvcb == NULL)
   /*~-*/
   {
      /*~T*/
      return 0;
   /*~-*/
   }
   /*~O*/
   /*~-2*/
   else
   {
      /*~T*/
      return 1;
   /*~-*/
   }
   /*~E*/
/*~-*/
}
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION Read_VSP_Parameter - Read supply file*/
/*~K*/
/*~+:Read data sequentially from the opened VCB file*/
/*~+:Parameter:*/
/*~+:data:   VCB data*/
/*~+:sizeof: datasize to be read */
/*~+:Return:*/
/*~+:datasize read or error (-1)*/
/*~+:*/
/*~F*/
int Read_VSP_Parameter(char* data, int _sizeof)
/*~-*/
{
   /*~I*/
   if (Fvcb)
   /*~-*/
   {
      /*~T*/
      return fread((void*)data, sizeof(char), _sizeof, Fvcb);
   /*~-*/
   }
   /*~O*/
   /*~-2*/
   else
   {
      /*~T*/
      return -1;
   /*~-*/
   }
   /*~E*/
/*~-*/
}
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION Schliessen_VSP_Parameter - Close supply file*/
/*~K*/
/*~+:Close the supply file*/
/*~+:*/
/*~F*/
void Schliessen_VSP_Parameter(void)
/*~-*/
{
   /*~T*/
   fclose (Fvcb);

/*~-*/
}
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION Oeffnen_Sichern_Parameter - Open backup supply file (VCB)*/
/*~K*/
/*~+:Open the backup supply file that is stored on the controller*/
/*~+:Return:*/
/*~+:1 = supply file was opened successfully, 0 = not opened*/
/*~+:*/
/*~F*/
int Oeffnen_Sichern_Parameter(void)
/*~-*/
{
   /*~T*/
   char VCBDateiNamenSIC[1024];
   int a;
   /*~T*/
   strcpy(VCBDateiNamenSIC, VCBDateinamen);
   /*~T*/
   a = strlen(VCBDateiNamenSIC);
   /*~L*/
   while (VCBDateiNamenSIC[a] != '.')
   /*~-*/
   {
      /*~T*/
      a--;
   /*~-*/
   }
   /*~E*/
   /*~T*/
   a++;

   /*~T*/
   strcpy (&VCBDateiNamenSIC[a],  "bak.vcb");
   /*~T*/
   Fvcb = fopen (VCBDateiNamenSIC, "wb");
   /*~I*/
   if (Fvcb == NULL)
   /*~-*/
   {
      /*~T*/
      return 0;
   /*~-*/
   }
   /*~O*/
   /*~-2*/
   else
   {
      /*~T*/
      return 1;
   /*~-*/
   }
   /*~E*/
/*~-*/
}
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION Schreiben_Sichern_Parameter - Write backup supply file*/
/*~K*/
/*~+:Write  backup VCB supply file*/
/*~+:Parameter:*/
/*~+:->      data:   the value to be written*/
/*~+:->      sizeof: size of the value to be written*/
/*~+:*/
/*~F*/
void Schreiben_Sichern_Parameter(char* data, int _sizeof)
/*~-*/
{
   /*~I*/
   if (Fvcb)
   /*~-*/
   {
      /*~T*/
      fwrite((void*)data, sizeof(char), _sizeof, Fvcb);

   /*~-*/
   }
   /*~E*/
/*~-*/
}
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION Schliessen_Sichern_Parameter - Close backup supply file*/
/*~K*/
/*~+:Close the backup supply file*/
/*~F*/
void Schliessen_Sichern_Parameter(void)
/*~-*/
{
   /*~T*/
   fclose (Fvcb);

/*~-*/
}
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION Oeffnen_VSP_Befehle - Open command file (VCB)*/
/*~K*/
/*~+:Open the command file which has arrived on the controller*/
/*~+:Return:*/
/*~+:1 = command file was opened successfully*/
/*~+:0 = not opened*/
/*~+:*/
/*~F*/
int Oeffnen_VSP_Befehle(void)
/*~-*/
{
   /*~I*/
   if (VSPCommandState == 1)
   /*~-*/
   {
      /*~T*/
      Fbefvcb = fopen (VSPBefehleDateiNamen, "rb");
      /*~I*/
      if (Fbefvcb == NULL)
      /*~-*/
      {
         /*~T*/
               VSPCommandState = 0;

         /*~T*/
         return 0;
      /*~-*/
      }
      /*~O*/
      /*~-2*/
      else
      {
         /*~T*/
         VSPCommandState = 2;

         /*~T*/
         return 1;
      /*~-*/
      }
      /*~E*/
   /*~-*/
   }
   /*~O*/
   /*~-2*/
   else
   {
      /*~T*/
      return 0;
   /*~-*/
   }
   /*~E*/
/*~-*/
}
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION Lesen_VSP_Befehle - Read command file*/
/*~K*/
/*~+:Read data sequentially from the opened command file*/
/*~+:Parameter:*/
/*~+:->      data:   data read*/
/*~+:->      sizeof: datasize to be read*/
/*~+:Return:*/
/*~+:datasize read or error (-1)*/
/*~+:*/
/*~F*/
int Lesen_VSP_Befehle(char* data, int _sizeof)
/*~-*/
{
   /*~I*/
   if (Fbefvcb)
   /*~-*/
   {
      /*~T*/
      return fread((void*)data, sizeof(char), _sizeof, Fbefvcb);
   /*~-*/
   }
   /*~O*/
   /*~-2*/
   else
   {
      /*~T*/
      return -1;
   /*~-*/
   }
   /*~E*/
/*~-*/
}
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION Schliessen_VSP_Befehle - Close command file*/
/*~K*/
/*~+:Close command file*/
/*~F*/
void Schliessen_VSP_Befehle(void)
/*~-*/
{
   /*~T*/
   fclose (Fbefvcb);

   /*~T*/
   VSPCommandState = 3;

/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
/*~A*/
/*~+:OCIT functions*/
/*~A*/
/*~+:FUNCTION OePNV_Ein_Aus - OCIT PT on/off status */
/*~K*/
/*~+:Read OCIT PT on/off status*/
/*~+:Return:   */
/*~+:0 = not set*/
/*~+:1 = PT off*/
/*~+:2 = PT on*/
/*~+:*/
/*~F*/
int OePNV_Ein_Aus(void)
/*~-*/
{
   /*~T*/
   return _OePNV_Ein_Aus_;
/*~-*/
}
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION IV_Ein_Aus - OCIT IT on/off status*/
/*~K*/
/*~+:Read OCIT IT on/off status*/
/*~+:Return:*/
/*~+:0 = not set*/
/*~+:1 = IT off*/
/*~+:2 = IT on*/
/*~+:*/
/*~F*/
int IV_Ein_Aus(void)
/*~-*/
{
   /*~T*/
   return _IV_Ein_Aus_;
/*~-*/
}
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION Det_Aktiv - Detector exists*/
/*~K*/
/*~+:Check if a detector with a certain channel number is defined in the controller*/
/*~+:Parameter:*/
/*~+:->      KanalNummer:    detector channel number*/
/*~+:Return:*/
/*~+:1 = detector is defined in the controller; 0 = not defined*/
/*~+:*/
/*~F*/
int Det_Aktiv(int KanalNummer)
/*~-*/
{
   /*~I*/
   if (KanalNummer < 1 || KanalNummer >= MAXDETEKTOR)
   /*~-*/
   {
      /*~T*/
      return 0;
   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION Sg_Aktiv - Signal group exists*/
/*~K*/
/*~+:Check if a signal group with a certain channel number is defined in the controller*/
/*~+:Parameter:*/
/*~+:->      KanalNummer:    signal group channel number*/
/*~+:Return:*/
/*~+:1 = signal group is defined in the controller; 0 = not defined*/
/*~+:*/
/*~F*/
int Sg_Aktiv(int KanalNummer)
/*~-*/
{
   /*~I*/
   if (KanalNummer <= 0 || KanalNummer > max_Signalgruppen)
   /*~-*/
   {
      /*~T*/
      return 0;
   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION AktuelleZeit - Current time*/
/*~K*/
/*~+:Get current time*/
/*~+:Parameter:*/
/*~+:->      Stunde:         current hour (0 . 23)*/
/*~+:->      Minute:         current minute (0 . 59)*/
/*~+:->      Sekunde:        current second (0 . 59)*/
/*~+:Return:*/
/*~+:1 = time is set; 0 = not set*/
/*~+:*/
/*~F*/
int AktuelleZeit(int* Stunde, int* Minute, int* Sekunde)
/*~-*/
{
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION AktuellesDatum - Current date*/
/*~K*/
/*~+:Get the current date*/
/*~+:Parameter:*/
/*~+:->      Jahr:   current year (1970 .. 2999)*/
/*~+:->      Monat:  current month (1 .. 12)*/
/*~+:->      Tag:    current day (1 .. 31)*/
/*~+:->      Wochentag:      current weekday (1 = Monday; 2 = Tuesday; 3 = Wednesday; 4 = Thursday; 5 = Friday; 6 = Saturday; 7 = Sunday)*/
/*~+:Return:*/
/*~+:1 = date is set; 0 = not set*/
/*~+:*/
/*~F*/
int AktuellesDatum(int* Jahr, int* Monat, int* Tag, int* Wochentag)
/*~-*/
{
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION Get_OCITOutstationId - OCIT-O identification*/
/*~K*/
/*~+:Get OCIT Outstations node identification*/
/*~+:Parameter:*/
/*~+:->      ZNr:    area number*/
/*~+:->      FNr:    controller number*/
/*~+:->      Relknoten:      relative node number*/
/*~+:Return:*/
/*~+:1 = id is set; 0 = not set*/
/*~+:*/
/*~F*/
int Get_OCITOutstationId(int* ZNr, int* FNr, int* Realknoten)
/*~-*/
{
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION Wunsch_VSPLUS - Switch off VS-PLUS*/
/*~K*/
/*~+:Switch the controller to a different mode*/
/*~+:Parameter:*/
/*~+:->      Request:        0 = check if this function is supported by the controller */
/*~+:                        1 = switch into fix time mode*/
/*~+:                        2 = switch controller off*/
/*~+:                        3 = restart VS-PLUS (reset)*/
/*~+:                        4 = switch program according to program timer */
/*~+:                        5 = predefined program (emergency program)*/
/*~+:->      Partial node:   0 = whole intersection*/
/*~+:                        1 - 99 = partial node number*/
/*~+:                        1 - 32 = program number for emergency program*/
/*~+:Return:*/
/*~+:1 = request will be executed; 0 = function not supported*/
/*~+:*/
/*~F*/
int Wunsch_VSPLUS(int Wunsch, int Teiknoten)
/*~-*/
{
   /*~T*/
   char err_txt[1024];
   /*~C*/
   switch (Wunsch)
   /*~-*/
   {
      /*~F*/
      case 1:
      /*~-*/
      {
         /*~T*/
         sprintf (err_txt, "Wunsch von VS-PLUS %d Anlage in Festzeit schalten\n", Wunsch);

         /*~T*/
         Log(777, err_txt);

         /*~T*/
         break;
      /*~-*/
      }
      /*~E*/
      /*~F*/
      case 2:
      /*~-*/
      {
         /*~I*/
         if (Teiknoten)
         /*~-*/
         {
            /*~T*/
            sprintf (err_txt, "Wunsch von VS-PLUS %d Teilknoten %d ausschalten\n", Wunsch, Teiknoten);

         /*~-*/
         }
         /*~O*/
         /*~-2*/
         else
         {
            /*~T*/
            sprintf (err_txt, "Wunsch von VS-PLUS %d Gesammter Knoten ausschalten\n", Wunsch);

         /*~-*/
         }
         /*~E*/
         /*~T*/
         Log(777, err_txt);

         /*~T*/
         break;
      /*~-*/
      }
      /*~E*/
      /*~F*/
      case 3:
      /*~-*/
      {
         /*~T*/
         sprintf (err_txt, "Wunsch von VS-PLUS %d VS-PLUS neu starten\n", Wunsch);

         /*~T*/
         Log(777, err_txt);

         /*~T*/
         break;
      /*~-*/
      }
      /*~E*/
      /*~F*/
      case 4:
      /*~-*/
      {
         /*~T*/
         sprintf (err_txt, "Wunsch von VS-PLUS %d Programm nach JAUT schalten\n", Wunsch);

         /*~T*/
         Log(777, err_txt);

         /*~T*/
         break;
      /*~-*/
      }
      /*~E*/
      /*~F*/
      case 5:
      /*~-*/
      {
         /*~T*/
         sprintf (err_txt, "Wunsch von VS-PLUS %d Notprogramm Nr %d schalten\\n", Wunsch, Teiknoten);

         /*~T*/
         Log(777, err_txt);

         /*~T*/
         break;
      /*~-*/
      }
      /*~E*/
      /*~O*/
      /*~-2*/
      default:
      {
         /*~T*/
         sprintf (err_txt, "Wunsch von VS-PLUS %d Unbekannter Wunsch\n", Wunsch);

         /*~T*/
         Log(777, err_txt);

      /*~-*/
      }
   /*~-*/
   }
   /*~E*/
   /*~T*/
   return 0;
/*~-*/
}
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION Neue_Befehle - New command file available*/
/*~K*/
/*~+:Check if a new command file is available from the controller*/
/*~+:Return:*/
/*~+:1 = new command file is present*/
/*~+:0 = no file present*/
/*~+:*/
/*~F*/
int Neue_Befehle(void)
/*~-*/
{
   /*~T*/
   return 0;

/*~-*/
}
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION ZSondereingriff - OCIT special program modification*/
/*~K*/
/*~+:Read the actual command "ZSondereingriff" (special program modification) and the corresponding job ID*/
/*~+:Parameter:*/
/*~+:->      Sondereingriff: number of special program modification (1 - 254)*/
/*~+:->      EndZeitpunkt:   end time (UTC time)*/
/*~+:->      VorgangsNummer: job number*/
/*~+:*/
/*~F*/
void ZSondereingriffvn(unsigned char* Sondereingriff, unsigned long* EndZeitpunkt, unsigned long* VorgangsNummer)
/*~-*/
{
   /*~T*/

/*~-*/
}
/*~E*/
/*~E*/
/*~A*/
/*~+:FUNCTION UTCZeitstempel - UTC timestamp*/
/*~K*/
/*~+:Get UTC timestamp*/
/*~+:Return:*/
/*~+:Current UTC time*/
/*~+:*/
/*~F*/
unsigned long UTCZeitstempel(void)

/*~-*/
{
   /*~T*/

/*~-*/
}
/*~E*/
/*~E*/
/*~E*/
