#Test Plan	4CF9T05AC_REL	1	05/31/2023	13:21:21	oper	Test Paln Converted From ICMS
#Library	measure/SKT_MEASURE	1	05/31/2023	13:12:34	oper	
#Library	tester/HP4062UX	SPECS_B.02.51	09/01/2000	09:00:00	specs	TSTR:Agilent 4062UX Tester Algorithms
#Library	prober/EG4080_HPSTD	1	05/25/2022	10:10:41	specs	PRBR:EG4080 Prober Algorithms (Enhanced)
#Library	utility/UTILITY4062	1	03/07/2023	15:44:37	specs	UTIL:Utility Algorithms
#Wafer	4CF9T05AC	1	05/31/2023	13:21:21	oper	Wafer spec for
#Die	4CF9T05AC	1	05/24/2023	16:54:22	specs	4CF9T01AC
#Probe	pin12	1	09/09/1991	10:13:39	specs	
#Test	4CF9T05AC	1	05/31/2023	13:17:10	oper	
#Job	SYSTEM	1	05/11/2023	16:30:56	specs	

$Type: Wafer
$Name: 4CF9T05AC
$Vers: 1
$Desc: Wafer spec for
$Date: 05/31/2023
$Time: 13:21:21
$User: oper
$--------- ------------------- ------------ - - -
 ATTRIBUTE                                       
           SIZE=REAL,"mm"      200.000000        
           STEPX=REAL,"um"     25600.000000      
           STEPY=REAL,"um"     14240.000000      
           FLAT=INTEGER,"deg"  270               
           ALIGNDIEX=INTEGER   1                 
           ALIGNDIEY=INTEGER   7                 
           ALIGNMODX=REAL,"um" -910.000000       
           ALIGNMODY=REAL,"um" -2230.000000      
           CENTERDIEX=INTEGER  4                 
           CENTERDIEY=INTEGER  7                 
           OFFSETDIEX=REAL     0.000000          
           OFFSETDIEY=REAL     0.000000          
           COORDINATE=INTEGER  1                 
           WAFERSHAPE=INTEGER  1                 
 BODY                                            
           `4CF9T05AC`         3,11              
           `4CF9T05AC`         3,9               
           `4CF9T05AC`         3,7               
           `4CF9T05AC`         3,5               
           `4CF9T05AC`         3,3               
$--------- ------------------- ------------ - - -

$Type: Die
$Name: 4CF9T05AC
$Vers: 1
$Desc: 4CF9T01AC
$Date: 05/24/2023
$Time: 16:54:22
$User: specs
$---- --------- ---------- - - -
 BODY                           
      `F9_2000` -910,-2230      
      `F9_2050` -910,-3380      
      `F9_2051` -910,-3430      
      `F9_2052` -910,-3480      
#     `F9_6000` -2625,3665      
      `F9_6000` -2625,6665      
$---- --------- ---------- - - -

$Type: Probe
$Name: pin12
$Vers: 1
$Desc: 
$Date: 09/09/1991
$Time: 10:13:39
$User: specs
$---- --- ----- - - -----------------
 BODY                                
      PAD 1,14      Pad #1  = Pin #14
      PAD 2,44      Pad #2  = Pin #44
      PAD 3,38      Pad #3  = Pin #38
      PAD 4,37      Pad #4  = Pin #37
      PAD 5,7       Pad #5  = Pin #7 
      PAD 6,5       Pad #6  = Pin #5 
      PAD 7,46      Pad #7  = Pin #46
      PAD 8,47      Pad #8  = Pin #47
      PAD 9,48      Pad #9  = Pin #48
      PAD 10,1      Pad #10 = Pin #1 
      PAD 11,2      Pad #11 = Pin #2 
      PAD 12,3      Pad #12 = Pin #3 
      PAD 36,36     Pad #36 = Pin #36
$---- --- ----- - - -----------------

$Type: Test
$Name: 4CF9T05AC
$Vers: 1
$Desc: 
$Date: 05/31/2023
$Time: 13:17:10
$User: oper
$------------ ------------------- ------------------------------------- ---------------------------------------------------------------------------------- ------------------------------------------ -
 BODY                                                                                                                                                                                                  
 `F9_6000`::  Rel_Pre_Post        Vgs1=0,Vgs2=1.2,Vd1=0,N=70            `vtAd`,`gm_ad`,`Iout`,`Vin`,`IoutGate`,`IoutDrain`,`IoutSource`,`IoutBulk`         Gate=1,Drain=2,Source=3                     
 `F9_6000`::  Rel_Stress          Vd1=0,Vstress=0,Vdrelax=1.8           `Ig`,`Id`,`IgS`,`IdS`,`Time`                                                       Gate=1,Drain=2,Source=3                     
 `F9_6000`::  Rel_Pre_Post        Vgs1=0,Vgs2=1.2,Vd1=0,N=70            `vtAd2`,`gm2_ad`,`Iout2`,`Vin2`,`IoutGate2`,`IoutDrain2`,`IoutSource2`,`IoutBulk2` Gate=1,Drain=2,Source=3                     
$------------ ------------------- ------------------------------------- ---------------------------------------------------------------------------------- ------------------------------------------ -

$Type: Job
$Name: SYSTEM
$Vers: 1
$Desc: 
$Date: 05/11/2023
$Time: 16:30:56
$User: specs
$---- ------------ ------------------------------------------------------------ --------- - -
 BODY                                                                                        
#     Warning:     Don't change any statements between BEGIN_NAVI and END_NAVI.              
#     BEGIN_NAVI                                                                             
#     SELECT_WAFER 4CF9T05AC                                                                 
#     SELECT_PROBE pin12                                                                     
#     SELECT_DIE   4CF9T05AC                                                                 
#     SELECT_TEST  4CF9T05AC                                                                 
#     DIE_TEST     4CF9T05AC                                                    4CF9T01AC    
#     END_NAVI                                                                               
      SELECT       WAFER,"4CF9T05AC"                                                         
      SELECT       PROBE,"pin12"                                                             
      TEST         "4CF9T05AC","4CF9T05AC"                                                   
      LOOP                                                                                   
$---- ------------ ------------------------------------------------------------ --------- - -

