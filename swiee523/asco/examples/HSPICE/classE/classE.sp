*Class-E power amplifier

.PARAM V_SUPPLY = '#V_SUPPLY#'
.PARAM INP_FREQ = '#INP_FREQ#'
.PARAM INP_PERIOD = '1/INP_FREQ'
.PARAM NO_PERIODS = '#NO_PERIODS#'
.PARAM TMEAS_START = '(NO_PERIODS-1)*INP_PERIOD'
.PARAM TMEAS_STOP = '(NO_PERIODS)*INP_PERIOD'
.PARAM T_PERC = 99
.PARAM TMEAS_AUX = '(NO_PERIODS-1)*INP_PERIOD \\
+                  + T_PERC/100*INP_PERIOD'

*** *** SUPPLY VOLTAGES *** ***
* Voltages and currents
VDD VDD 0 V_SUPPLY
VSS VSS 0 0

*** *** INPUT SIGNAL *** ***
VSIG G1 VSS PULSE V_SUPPLY 0 'INP_PERIOD/2' 'INP_PERIOD/1000'
+               'INP_PERIOD/1000' 'INP_PERIOD/2' 'INP_PERIOD'

*** *** INDUCTOR *** ***
.SUBCKT LBOND.SUB IN OUT L=1
RBOND IN  1   '0.135*(L/1n)' $ 0.135 Ohm/mm; gold
LBOND 1   OUT 'L'            $ 1 nH/mm
.ENDS LBOND.SUB

*** *** OUTPUT STAGE *** ***
* Diffusion length, MOSwidth, MOSlength and multiplier
.PARAM LDIFF='1.2u' WS='#TR1_W#' LS='#LMIN#' MS='1'
M1 D1 G1 VSS VSS NMOS W=WS L=LS M=MS AD='WS*LDIFF' PD='2*(LDIFF+WS)'
+                                    AS='WS*LDIFF' PS='2*(LDIFF+WS)'

XL1 VDD D1  LBOND.SUB L=#L1#
XL0 D1  N2  LBOND.SUB L=#L0#
C0  N2  OUT #C0#

.PARAM FILT_RES = #RL#
R   OUT VSS FILT_RES

*** *** ANALYSIS *** ***
.TRAN 'INP_PERIOD/1000' '(NO_PERIODS+1)*INP_PERIOD'
.PROBE TRAN V(G1)
.PROBE TRAN V(D1)
.PROBE TRAN V(OUT)
.OP
.OPTION POST PROBE ACCURATE
.INCLUDE n.typ
.END
