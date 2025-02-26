#
# 5 Dec 2016  reduce delay from 60 to 30 in "set_mdelay(30)"-->Monitor loop for 10 modules takes ~8 sec
#
# 15 Mar 2015
#boot device          : dc
#processor number     : 0 
#host name            : sbs2
#file name            : /site/coda/kern/5.3.1/vx2306_IVG3
#inet on ethernet (e) : 129.57.36.152
#host inet (h)        : 129.57.37.50
#gateway inet (g)     : 129.57.36.1
#user (u)             : adaq
#flags (f)            : 0x20 
#target name (tn)     : sbscdet
#startup script (s)   : ~/hv/caen_hv/sbscdet.boot

#cd "caen_hv/v288"

#v288addr=0xe0600000;
#craten=12;


cd "~/hv/caen_hv/"
# load library for v288
ld < sy527/sy527.o
v288addr=0xe0600000;
#v288addr=0xe0700000;

# debug output level
set_dbg(0);
# system clock tiks to 120Hz (default is 60Hz)
sysClkRateSet(120);
# crate semaphore for mutex
sem_init() 
# set monitor task delay (in ticks, 1 tick=1/60 sec) between boards scans
set_mdelay(30);	   

# monitor( num_of_crates, crate_num1, crate_num2, crate_num3, crate_num2 )
#taskSpawn "caenmon",150,spTaskOptions,10000, monitor, 2, 12, 9,0,0

# start monitor for caen crate #12 
taskSpawn "caenmon12",150,spTaskOptions,10000, monitor1, 12

# start monitor for caen crate #9
#taskSpawn "caenmon9",150,spTaskOptions,10000, monitor1, 9


#ld < v288/v288.o
#init_288()
#menu(0xe0600000,0xc)

#ld < caen_server/vmetcpserver.o
#taskSpawn "tcaensrv1",120,spTaskOptions,10000, main, 6002

## load and start two servers
ld < server/server.o
taskSpawn "c_srv1",120,spTaskOptions,10000, start, 2012, 12
# 1 Nov 2016: Crate failure: DAC
#taskSpawn "c_srv2",120,spTaskOptions,10000, start, 2009, 9

