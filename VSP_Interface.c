//#include "if626bas.inc"
//#include "vsp626ste.inc"
//#include "if626max.inc"
#include <stdio.h>
#include "tsc.h" //debug()
#include <time.h>

#if 0
/* API implemented by vsplus, controller can call these to get some info
 */

/* tells the controller that a pending program switch has to be delayed. 
 * the controller has to delay the program switching until this function
 * allows it. only traffic-actuated vsplus programs have to be delayed.
 *
 * int m_Prog_Schaltung_erlaubt(void);
 * int vs_prog_swtich_allow(void);
 *
 * return: 1=allow, 0=not allow
 */

/* returns the value to the controller how PT on/off is set.
 *
 * l_VSP_X_Ein_Aus_Ist(0);
 * short vs_pt_state(void);
 *
 * return: 1=PT off, 2=PT on
 */

/* returns the value to the controller how IT on/off is set.
 *
 * l_VSP_X_Ein_Aus_Ist(1);
 * short vs_it_state(void);
 *
 * return: 1=IT off, 2=IT on
 */

/* returns the value to the controller of the active program modification
 * and the job id.
 *
 * m_ZSondereingriffVSPvn(vn);
 * unsigned char vs_prog_amod(unsigned long* jobID);
 *
 * return: number of program modification
 * jobID: corresponding job id
 */

/* the controller starts the initialization of the vsplus parameter are.
 *
 * f_Initial_VSP_Parameter()
 * int vs_init_parameter(void)
 *
 * return: 0=no error, -1=error
 */

/* the controller starts the parameter file checking by vsplus.
 *
 * f_Pruefen_VSP_Parameter()
 * int vs_chk_parameter(void)
 *
 * return: 0=no error, <0=error value with some meaning
 */

/* the controoler starts the parameter file reading by vsplus.
 *
 * f_Lesen_VSP_Parameter()
 * int vs_read_parameter(void)
 *
 * return: 0=no error, <0=error value whit some meaning
 */

/* the controller requests vsplus to give free the parameter area.
 *
 * f_Ende_VSP_Parameter()
 * void vs_free_parameter(void)
 */

/* returns the path name for transmitting an OCIT command(default "57.520")
 *
 * l_VABefehlPfad()
 * char* vs_ocit_path()
 *
 * return: OCIT path name for command file
 */

/* enables the controller to check if a program number belongs to a
 * vsplus program or not
 *
 * l_Prog_VSP(x)
 * short vs_prog_chk(short prg)
 *
 * return:	0 = not defined
 *			1 = fixed time with vsplus parameters
 *			2 = fixed time without vsplus parameters
 *			5 = vsplus with parameters
 *			6 = vsplus without parameters
 * prg: program number
 */

/* controller can find out what vsplus version is in use
 *
 * f_versions_txt(x,y)
 * short vs_version(char* text, int size)
 *
 * return:	1 = version information available
 *			0 = not available
 * text: vsplus version text
 * size: maximum number of characters for version text(min, 10)
 */

/* controller can read the process data from vsplus according to OCIT-I
 *
 * f_VSP_ProzessDaten(x,y)
 * unsigned short vs_read_process_data(void *px, void *py)
 */
#endif

/* funktion:1=read;3=clear;4=stop and clear;5=stop
 * timer: timer index
 */
short int timer(short int funktion, short int timer)
{
	short int ret = tsc_timer(funktion, timer, 0);
	debug(3, "funktion:%d, timer:%d, ret:%d\n", funktion, timer, ret);
	return ret;
}

/* load and start a timer
 * funktion=2;
 * timer: timer index
 * wert: initial counts
 */
short int timer_2(short int funktion, short int timer, short int wert)
{
	short int ret = tsc_timer(funktion, timer, wert);
	debug(3, "funktion:%d, timer:%d, wert:%d, ret:%d\n", funktion, timer, wert, ret);
	return ret;
}

/* VS-PLUS is told the actual program number
 * return value:   Actual program number 0 – 255
 */
short int ProgrammAktuell(void)
{
	short int ret = tsc_prog_actual();
	debug(3, "ret:%d\n", ret);
	return ret;
}

/* VS-PLUS is told a program change request. If there is no 
 * change request pending, the actual program is contained 
 * in the return value
 * return value:  Number of the new, not yet activated program 0 – 255
 */
short int ProgrammWahl(void)
{
	short int ret = tsc_prog_select();
	debug(3, "ret:%d\n", ret);
	return ret;
}

/* The current cycle second within the active signal program
 * return value:  Current cycle second in units of 100ms, rounded to seconds
 */
short int Zykluszeit(void)
{
	short int ret = tsc_prog_tx();
	debug(3, "ret:%d\n", ret);
	return ret;
}

/* The cycle time of the active signal program (in units of 100ms)
 * return value:  Cycle time in units of 100ms
 */
short int Umlaufzeit(void)
{
	short int ret = tsc_prog_tu();
	debug(3, "ret:%d\n", ret);
	return ret;
}

/* FIXME:returns the program number source (who has sent current valid program
 * switch command
 * return:	1 = program number comes from central;
 *			0 = comes locally from controller
 */
int ProgrammWahlZentrale(void)
{
	int ret = tsc_prog_src();
	debug(3, "ret:%d\n", ret);
	return ret;
}

/* Number of rising slopes counted by the controller, until clearing by VS-PLUS
 * return value:  number of rising slopes since last clear
 * det:  detector channel number
 */ 
short int d_imp(short int det)
{
	short int ret = tsc_sum_rising(det);
	debug(3, "det:%d, ret:%d\n", det, ret);
	return ret;
}

/* The rising slope counter is cleared by VS-PLUS
 * return value:  none
 * det:  detector channel number
 */
void d_limp(short int det)
{
	debug(3, "det:%d\n", det);
	tsc_clr_rising(det);
}

/* The occupancy degree (in percent) indicates how long a 
 * detector has been occupied during the last second. (0 – 100%)
 * return value:  detector occupancy degree during the last second, in percent 
 * det:  detector channel number
 */
short int d_belga(short int det)
{
	short int ret = tsc_cur_hold(det);
	debug(3, "det:%d, ret:%d\n", det, ret);
	return ret;
}

/* Detector fault
 * return: 0:no fault; >0:fault
 * det: detctor channel number
 */
short int d_stoer(short int det)
{
	short int ret = tsc_det_fault(det);
	debug(3, "det:%d, ret:%d\n", det, ret);
	return ret;
}

/* The  smoothened  occupancy  degree  is  calculated  over  a  defined  interval.
 * return value:  smoothened occupancy degree, in percent (0 – 100%)
 * det:  detector channel number
 */
short int d_belgg(short int det)
{
	short int ret = tsc_sm_hold(det);
	debug(3, "det:%d, ret:%d\n", det, ret);
	return ret;
}

/* Actual occupancy state of the detector
 * return value:  0: not occupied; >0: occupied
 * det:  detector channel number
 */
short int d_blg(short int det)
{
	short int ret = tsc_hold_state(det);
	debug(3, "det:%d, ret:%d\n", det, ret);
	return ret;
}

/* the net time gap starts at the last falling slope
 * return: net time gap in units of 100ms
 * det: detector channel number
 */
short int d_ztlkn(short int det)
{
	short int ret = tsc_det_net(det);
	debug(3, "det:%d, ret:%d\n", det, ret);
	return ret;
}

/* the gross time gap starts at the last rising slope
 * return: gross time gap in units of 100ms
 * det: detector channel number
 */
short int d_zeitlb(short int det)
{
	short int ret = tsc_det_gross(det);
	debug(3, "det:%d, ret:%d\n", det, ret);
	return ret;
}

/* Actual occupancy time of the detector
 * return value:  detector occupancy time in units of 100ms
 * det:  detector channel number
 */
short int d_blgzt(short int det)
{
	short int ret = tsc_hold_time(det);
	debug(3, "det:%d, ret:%d\n", det ,ret);
	return ret;
}

/* Number of falling slopes counted by the controller, until clearing by VS-PLUS
 * return value:  Number of falling slopes since last clear
 * type:  1: read counter (number of falling slopes)
 *		  2: clear counter
 * det:  detector channel number
 */
short int d_impab(short int type, short int det)
{
	short int ret;
	if(type == 1)
		ret = tsc_sum_falling(det);
	else
		ret = tsc_clr_falling(det);
	debug(3, "type:%d, det:%d, ret:%d\n", type, det, ret);
	return ret;
}

/* FIXME: for future use, not implemented */
short int d_kvalue(short int det)
{
printf("%s(%d)\n", __func__, __LINE__);
return 0;
}

/* The minimum read time defined in the controller.
 * return value: minimum red time in units of 100ms.
 * sg: signal group channel number
 */
short int min_rot(short int sg)
{
	short int ret = tsc_min_red(sg);
	debug(3, "sg:%d, ret:%d\n", sg, ret);
	return ret;
}

/* preparation time
 */
short int u_rot_gelb(short int sg)
{
	short int ret = tsc_prep(sg);
	debug(3, "sg:%d, ret:%d\n", sg, ret);
	return ret;
}

/* the minimum green time defined in the controller
 * return: minimumu green time in units of 100ms
 * sg: signal group channel number
 */
short int min_gruen(short int sg)
{
	short int ret = tsc_min_green(sg);;
	debug(3, "sg:%d, ret:%d\n", sg, ret);
	return ret;
}

/* the amber time expresses the transition time from open to closed.
 * this function returns the value defined int the controller. the function
 * returns 0 if no transition time is available. if the transition consist
 * of several elements, the sum of transition times has to be returned.
 * return: amber time in units of 100ms
 * sg: signal group channel number
 */
short int u_gelb(short int sg)
{
	short int ret = tsc_amber(sg);
	debug(3, "sg:%d, ret:%d\n", sg, ret);
	return ret;
}

/* switch signal group to open
 * sg: signal group channel number
 */
void SG_ein(short int sg)
{
        time_t tt = time(NULL);
        struct tm *t = localtime(&tt);

        //debug(2, "sg:%d\n", sg);
        printf("%04d-%02d-%02d %02d:%02d:%02d\tsg:%d open\n", t->tm_year+1900,
                        t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec,sg);
        tsc_sg_open(sg);
}

/* switch signal group to closed
 */
void SG_aus(short int sg)
{
        time_t tt = time(NULL);
        struct tm *t = localtime(&tt);
        printf("%04d-%02d-%02d %02d:%02d:%02d\tsg:%d close\n", t->tm_year+1900,
                        t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec,sg);
        //debug(2, "sg:%d\n", sg);
        tsc_sg_close(sg);
}

/* FIXME:command for switching a non-supervised output to "on"
 * return: none
 * sg: output channel number
 */
void Relais_ein(short int sg)
{
	debug(3, "sg:%d\n", sg);
	tsc_digital_on(sg);
}

/* FIXME:command for switching a non-supervised output to "off"
 * return: none
 * sg: output channel number
 */
void Relais_aus(short int sg)
{
	debug(3, "sg:%d\n", sg);
	tsc_digital_off(sg);
}

/* FIXME:command for switching a non-supervised blinker output(blinking digital
 * output) to "on"
 * return: none
 * sg: output channel number
 */
void Blinker_ein(short int sg)
{
	debug(3, "sg:%d\n", sg);
	tsc_digital_blink_on(sg);
}

/* FIXME:command for switching a non-supervised blinker output(blinking digital
 * output) to "off"
 * return: none
 * sg: output channel number
 */
void Blinker_aus(short int sg)
{
	debug(3, "sg:%d\n", sg);
	tsc_digital_blink_off(sg);
}

/* This function enables VS-PLUS to check if a signal 
 * group shows red. The return value is 1 if this is 
 * the case. All other cases result in a return value of 0.
 * return value:  1 = signal group shows red; 0 = not red
 * sg:   signal group channel number
 */
short int s_rot(short int sg)
{
	short int ret = tsc_chk_red(sg);
	debug(3, "sg:%d, ret:%d\n", sg, ret);
	return ret;
}

/* FIXME:enables vsplus to check if a digital output is switched off
 * return:	1 = off, 0 = not off
 * re: output channel number
 */
short int s_sr_aus(short int re)
{
	short int ret = !tsc_digital_state(re);
	debug(3, "re:%d, ret:%d\n", re, ret);
	return ret;
}

/* FIXME:enables vsplus to check if a digital blinker output is switched off
 * return:  1 = off, 0 = not off
 * re: output channel number
 */
short int s_sb_aus(short int bli)
{
	short int ret = !tsc_digital_blink_state(bli);
	debug(3, "bli:%d, ret:%d\n", bli, ret);
	return ret;
}

/* This function enables VS-PLUS to check if a signal 
 * group shows red and the elapsed red time is still 
 * within the minimum red time.
 * return value:  1 = signal group red and min red not reached;
 *				  0 = not red or min red elapsed
 *  sg:   signal group channel number
 */
short int s_min_rot(short int sg)
{
	short int ret = tsc_chk_min_red(sg);
	debug(3, "sg:%d, ret:%d\n", sg, ret);
	return ret;
}

/* to check if a signal group shows amber or in gerenal if the 
 * signal group is in the "open-closed" transition.
 */
short int s_gelb(short int sg)
{
	short int ret = tsc_chk_amber(sg);
	debug(3, "sg:%d, ret:%d\n", sg, ret);
	return ret;
}

/* to check if a signal group shows green
 */
short int s_grun(short int sg)
{
	short int ret = tsc_chk_green(sg);
	debug(3, "sg:%d, ret:%d\n", sg, ret);
	return ret;
}

/* FIXME:to check if a digital output is switched on
 */
short int s_sr_ein(short int re)
{
	short int ret = tsc_digital_state(re);
	debug(3, "re:%d, ret:%d\n", re, ret);
	return ret;
}

/* FIXME:to check if a digital blinker output is switched on
 */
short int s_sb_ein(short int bli)
{
	short int ret = tsc_digital_blink_state(bli);
	debug(3, "bli:%d, ret:%d\n", bli, ret);
	return ret;
}

/* to check if a signal group show green and the elapsed green time
 * is still within the minimum green time
 */
short int s_min_grun(short int sg)
{
	short int ret = tsc_chk_min_green(sg);
	debug(3, "sg:%d, ret:%d\n", sg, ret);
	return ret;
}

/* to check if a signal group show red-amber or in general if the signal
 * group is in the "closed-open" transition.
 */
short int s_vor(short int sg)
{
	short int ret = tsc_chk_prep(sg);
	debug(3, "sg:%d, ret:%d\n", sg, ret);
	return ret;
}

/* FIXME:indicates if the signal group is in fault blinking mode
 */
short int s_stoeblink(short int sg)
{
	short int ret = tsc_sg_fault(sg);
	debug(3, "sg:%d, ret:%d\n", sg, ret);
	return ret;
}

/* Tells how long the signal group is already red.
 * return value:  current red time in units of 100ms
 * sg:   signal group channel number
 */
unsigned short s_t_rot(short int sg)
{
	unsigned short int ret = tsc_red_time(sg);
	if(sg == 2)
		;//debug(2, "sg:%d, ret:%d\n", sg, ret);
	return ret;
}

/* Tells how long the signal group is already amber(transition
 * state from open to closed).
 * return value:  current amber time in units of 100ms
 * sg:   signal group channel number
 */
unsigned short int s_t_gelb(short int sg)
{
	unsigned short int ret = tsc_amber_time(sg);
	//debug(2, "sg:%d, ret:%d\n", sg, ret);
	return ret;
}

/* tells how long the signal group is already in minimum read state.
 * after the elasping of the minimum red time return value is 0.
 *
 */
unsigned short int s_t_min_rot(short int sg)
{
	unsigned short int ret = tsc_min_red_time(sg);
	//debug(2, "sg:%d, ret:%d\n", sg, ret);
	return ret;
}

/* tell how long the signal group is already green
 * return: current green time in units of 100ms
 * sg: signal group channel number
 */
unsigned short int s_t_grun(short int sg)
{
	unsigned short int ret = tsc_green_time(sg);
	debug(3, "sg:%d, ret:%d\n", sg, ret);
	return ret;
}

/* tell how long the signal group is already red-amber (transition from
 * closed to open)
 */
unsigned short int s_t_vor(short int sg)
{
	unsigned short int ret = tsc_prep_time(sg);
	debug(3, "sg:%d, ret:%d\n", sg, ret);
	return ret;
}

/* tell how long the signal group is already in miminum green state.
 * after the elapsing of the minimum green time the return value is 0.
 */
unsigned short int s_t_min_grun(short int sg)
{
	unsigned short int ret = tsc_min_green_time(sg);
	debug(3, "sg:%d, ret:%d\n", sg, ret);
	return ret;
}

/* indicates if a signal group is enabled for vsplus
 * return: 1 = sg is enabled; 0 = not enabled
 * sg:
 */
unsigned short int s_HW_VspFreigegeben(short int sg)
{
	unsigned short int ret = tsc_sg_enabled(sg);
	debug(3, "sg:%d, ret:%d\n", sg, ret);
	return ret;
}

/* a controller intergreen value
 * return: controller intergreen time in units of 100ms
 * sgR: clearing signal group channel number
 * sgE: entering signal group channel number
 */
short s_zwi_zeit(short sgR, short sgE)
{
	short int ret = tsc_inter_green(sgR, sgE);
	debug(3, "sgR:%d, sgE:%d, ret:%d\n", sgR, sgE, ret);
	return ret;
}

void Meldung(short int degree, short int nr, short int par1, short int par2, short int par3, short int par4)
{
	//debug(3, "degree:%d, nr:%d, par1-4:%d,%d,%d,%d\n", degree, nr, par1, par2, par3, par4);
	char str[256] = {0};
	sprintf(str, "%s(%d): degree:%d, nr:%d, par1-4:%d,%d,%d,%d\n", __func__, __LINE__, degree, nr, par1, par2, par3, par4);
	printf("%s", str);
	drv_add_message(str);
}

void  MeldungNET(short int degree, unsigned char nr, unsigned short Anr, unsigned char par1, unsigned char par2, unsigned char par3, unsigned char par4, unsigned char par5)
{
	//debug(3, "degree:%d, nr:%d, Anr:%d, par1-5:%d,%d,%d,%d,%d\n", degree, nr, Anr, par1, par2, par3, par4, par5);
	char str[256] = {0};
	sprintf(str, "%s(%d):degree:%d, nr:%d, Anr:%d, par1-5:%d,%d,%d,%d,%d\n", __func__, __LINE__, degree, nr, Anr, par1, par2, par3, par4, par5);
	printf("%s", str);
	drv_add_message(str);
}

/* VS-PLUS tells the controller the number of the traffic 
 * stream with the largest waiting time. 
 * return value:  none
 * vs:  number of traffic stream with largest waiting time 0 – VSMAX
 * zeit:  waiting time in units of 100ms
 */
void U_Kontrolle(short int vs, short int zeit)
{
	debug(3, "vs:%d, zeit:%d\n", vs, zeit);
	tsc_stream_waiting(vs, zeit);
}

#include <string.h>
/* FIXME:VS-PLUS reads the serial PT telegrams that have been
 * received by the controller.
 * oev_tele_poi:   pointe on a telegram
 */
short TelegrammVomGeraet(void* oev_tele_poi)
{

#if 0
	typedef struct{
		int MP;// (call point);
		int Linie;// (line);
		int Kurs;// (course);
		int Route;// (route);
		int Prioritaet;// (priority);
		int Laenge;// (vehicle length);
		int RichtungVonHand;// (direction by hand);
		int FahrplanAbweichnung;// (difference to schedule);
	}PTMSG;
#endif
#if 0
	PTMSG pt = {0};
	short int ret = tsc_read_pt(&pt);
	memcpy(oev_tele_poi, &pt, sizeof(PTMSG));
	if(ret){
		debug(2, "call point:%d\n", pt.MP);
		debug(2, "line:%d\n", pt.Linie);
		debug(2, "course:%d\n", pt.Kurs);
		debug(2, "route:%d\n", pt.Route);
		debug(2, "priority:%d\n", pt.Prioritaet);
		debug(2, "vehicle length:%d\n", pt.Laenge);
		debug(2, "direction by hand:%d\n", pt.RichtungVonHand);
		debug(2, "difference to schedule:%d\n", pt.FahrplanAbweichnung);
		int i;
		for(i = 0; i < sizeof(PTMSG); ++i){
			printf("0x%02X ", *((char*)oev_tele_poi+i));
		}
		printf("\n");
	}

#endif
	short int ret = tsc_read_pt(oev_tele_poi);
	//debug(2, "ptr:%p, ret:%d\n", oev_tele_poi, ret);

#if 10
	PTMSG *ptr = (PTMSG*)oev_tele_poi;

	//if(ret){
	if(ptr->MP){
		debug(2, "call point:%d\n", ptr->MP);
		debug(2, "line:%d\n", ptr->Linie);
		debug(2, "course:%d\n", ptr->Kurs);
		debug(2, "route:%d\n", ptr->Route);
		debug(2, "priority:%d\n", ptr->Prioritaet);
		debug(2, "vehicle length:%d\n", ptr->Laenge);
		debug(2, "direction by hand:%d\n", ptr->RichtungVonHand);
		debug(2, "difference to schedule:%d\n", ptr->FahrplanAbweichnung);

		//int i;
		//for(i = 0; i < sizeof(PTMSG); ++i){
		//	printf("0x%02X ", *((char*)oev_tele_poi+i));
		//}
		printf("\n");
	}
#endif

	return ret;
}

/* FIXME:returns 0 as long as a fixed time signal  plan
 * is being processed. When off or during switch on
 * or switch off, this function returns 1
 * return value:  1 = Off or switching on or off; 0 = signal program active
 */
unsigned short int s_SteuerungNichtAktiv(void)
{
	unsigned short int ret = tsc_ctl_active();
	debug(3, "ret:%d\n", ret);
	return ret;
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
	void *ptr = tsc_alloc_mem(_sizeof, id) ;
	debug(3, "_sizeof:%d, id:%d, ret:%p\n", _sizeof, id, ptr);
	return ptr;
}

/* returns the pointer to the memory area
 * return value:  pointer to memory area
 * id:  memory area identification (1 – 3)
 */
void* Gib_VSP_Zeiger(int id)
{
	void *ptr = tsc_get_mem(id);
	debug(3, "id:%d, ret:%p\n", id, ptr);
	return ptr;
}

/* frees memory that has been requested from the controller
 * return value:  none
 * id: memory area identification(1-3)
 */
void Freigeben_VSP_Speicher(int id)
{
	debug(3, "id:%d\n", id);
	tsc_free_mem(id);
}

/* opens the VCB file that has been sent to the controller
 * return value: 1 = supply file was opened successfully, 
 *				 0 = not opened
 */
int Oeffnen_VSP_Parameter(void)
{
	int ret = tsc_open_vcb();
	debug(3, "ret:%d\n", ret);
	return ret;
}

#include <string.h>
/* Data is read sequentially from the formerly opened VCB 
 * file. This function corresponds to a "fread” in C. 
 * return value:  size of data read; -1 = error
 * data:  data read
 * sizeof:  size of data to be read 
 */
int Read_VSP_Parameter(char* data, int _sizeof)
{
	int ret = tsc_read_vcb(data, _sizeof);
#if 0
	debug(3, "size=%d, ret=%d\n", _sizeof, ret);
	int i;
	for(i = 0; i < ret; i++){
		printf("%#x ", *(data+i)&0xff);
	}
	printf("\n");
#endif
	return ret;
}

/* closes the supply file
 * return value:  none
 */
void Schliessen_VSP_Parameter(void)
{
	debug(3, "==>\n");
	tsc_close_vcb();
}

/* open the backup supply file that is stored on the controller
 * return value:  1 = supply file was opened successfully, 0 = not opened
 */
int Oeffnen_Sichern_Parameter(void)
{
	int ret = tsc_open_back();
	debug(3, "ret:%d\n", ret);
	return ret;
}

/* write backup supply file
 */
void Schreiben_Sichern_Parameter(char* data, int _sizeof)
{
	tsc_write_back(data, _sizeof);
#if 0
	debug(3, "size=%d\n", _sizeof);
	int i;
	for(i = 0; i < _sizeof; i++){
		printf("%x ", *(data+i)&0xff);
	}
	printf("\n");
#endif
}

/* close the backup supply file
 */
void Schliessen_Sichern_Parameter(void)
{
	debug(3, "==>\n");
	tsc_close_back();
}

//Open command file
int Oeffnen_VSP_Befehle(void)
{
	int ret = tsc_open_cmd_vcb();
	debug(3, "ret:%d\n", ret);

	return ret;
}

//Read command file
int Lesen_VSP_Befehle(char* data, int _sizeof)
{
	int ret = tsc_read_cmd_vcb(data, _sizeof);
	debug(3, "ret:%d\n", ret);

	return ret;
}

//Close command file
void Schliessen_VSP_Befehle(void)
{
	int ret = tsc_close_cmd_vcb();
	debug(3, "ret:%d\n", ret);

	return;
}

/* FIXME:This function returns the value to the controller 
 * how PT on / off is set.
 * return value: 0 = PT not set 1 = PT off 2 = PT on
 */
int OePNV_Ein_Aus(void)
{
	int ret = 2;//PT not set
	debug(3, "ret:%d\n", ret);
	return ret;
}

/* FIXME:This function returns the value to the controller how 
 * IT on / off is set.
 * return value: 0 = IT not set 1 = IT off 2 = IT on
 */
int IV_Ein_Aus(void)
{
	int ret = 2;//IT not set
	debug(3, "ret:%d\n", ret);
	return ret;
}

/* Function to check if a detector with a certain channel number is defined in the controller
 * return value:  1 = detector is defined in the controller; 0 = not defined
 * KanalNummer:   detector channel number
 */
int Det_Aktiv(int KanalNummer)
{
	int ret = tsc_det_exist(KanalNummer);
	debug(3, "KanalNummer:%d, ret:%d\n", KanalNummer, ret);
	return ret;
}

/* Function to check if a signal group with a certain channel number is defined in the controller
 * return value:  1 = signal group is defined in the controller; 0 = not defined
 *  KanalNummer:   signal group channel number
 */
int Sg_Aktiv(int KanalNummer)
{
	int ret = tsc_sg_exist(KanalNummer);
	debug(3, "KanalNummer:%d, ret:%d\n", KanalNummer, ret);
	return ret;
}

/* Read current time
 * return value:  1 = time is set; 0 = not set
 * Stunde:   current hour (0 … 23)
 * Minute:  current minute (0 … 59)
 * Sekunde:  current second (0 … 59)
 */
int AktuelleZeit(int* Stunde, int* Minute, int* Sekunde)
{
	int ret = tsc_get_time(Stunde, Minute, Sekunde);
	debug(3, "Stunde:%d, Minute:%d, Sekunde:%d, ret:%d\n", *Stunde, *Minute, *Sekunde, ret);
	return ret;
}

/* Read current date
 * return value:  1 = date is set; 0 = not set
 * Jahr:   current year (1970 .. 2999)
 * Monat:  current month (1 .. 12)
 * Tag:  current day (1 .. 31)
 * Wochentag:  current weekday (1 = Monday; 2 = Tuesday; 3  = Wednesday; 4 
= Thursday; 5 = Friday; 6 = Saturday; 7 = Sunday)
 */
int AktuellesDatum(int* Jahr, int* Monat, int* Tag, int* Wochentag)
{
	int ret = tsc_get_date(Jahr, Monat, Tag, Wochentag);
	debug(3, "Jahr:%d, Monat:%d, Tag:%d, Wochentag:%d, ret:%d\n", *Jahr, *Monat, *Tag, *Wochentag, ret);
	return ret;
}

/* Returns the OCIT Outstations node identification
 * return value:  1 = id is set; 0 = not set
 *  ZNr:   area number
 * FNr:  controller number
 * Relknoten:  relative node number
 */
int Get_OCITOutstationId(int* ZNr, int* FNr, int* Realknoten)
{
	int ret = 1;
	ret = drv_get_ocitid(ZNr, FNr, Realknoten);
	debug(3, "ZNr:%d, FNr:%d, Realknoten:%d, ret:%d\n", *ZNr, *FNr, *Realknoten, ret);
	return ret;
}

/* FIXME:Through this function VS-PLUS can ask the controller 
 * to be switched in a different mode.
 */
int Wunsch_VSPLUS(int Wunsch, int Teiknoten)
{
	int ret = 0;
	debug(2, "Wunsch:%d, Teiknoten:%d, ret:%d\n", Wunsch, Teiknoten, ret);
	return ret;
}

/* VS-PLUS checks whether a new command file is available from the controller
 * return value: 	1 = new command file is present 
 * 					0 = no file present 
 */
int Neue_Befehle(void)
{
	int ret = tsc_chk_cmd_vcb();
	debug(3, "ret:%d\n", ret);
	return ret;
}

/* FIXME:VS-PLUS  reads  the  actual  command  "ZSondereingriff” 
 * (special  program  modification)  and the corresponding job ID.
 */
void ZSondereingriffvn(unsigned char* Sondereingriff, unsigned long* EndZeitpunkt, unsigned long* VorgangsNummer)
{
	*Sondereingriff = 0;
	*EndZeitpunkt = 0;
	*VorgangsNummer = 0;
	debug(3,"Sondereingriff:%s, EndZeitpunkt:%ld, VorgangsNummer:%ld\n", Sondereingriff, *EndZeitpunkt, *VorgangsNummer);
}

/* FIXME: for future use
 */
unsigned long UTCZeitstempel(void)
{
	int ret = 32767;
	debug(3, "ret:%d\n", ret);
	return ret;
}
