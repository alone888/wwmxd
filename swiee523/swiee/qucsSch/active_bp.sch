<Swiee Schematic 0.0.18>
<Properties>
  <View=0,-180,900,740,1,0,120>
  <Grid=10,10,1>
  <DataSet=active_bp.dat>
  <DataDisplay=active_bp.dpl>
  <OpenDisplay=1>
  <Script=active_bp.m>
  <RunScript=0>
  <showFrame=0>
  <FrameText0=头衔>
  <FrameText1=作者:>
  <FrameText2=日期:>
  <FrameText3=修订:>
</Properties>
<Symbol>
</Symbol>
<Components>
  <OpAmp OP1 1 370 160 -26 42 0 0 "1e6" 1 "15 V" 0>
  <GND * 1 300 180 0 0 0 0>
  <GND * 1 180 260 0 0 0 0>
  <GND * 1 60 260 0 0 0 0>
  <Vac V1 1 60 230 18 -26 0 1 "1 V" 1 "1 GHz" 0 "0" 0 "0" 0>
  <R R3 1 130 140 -26 15 0 0 "390" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <R R2 1 180 230 15 -26 0 1 "5.6k" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <C C1 1 230 140 -26 17 0 0 "47n" 1 "" 0 "neutral" 0>
  <C C2 1 180 90 17 -26 0 1 "47n" 1 "" 0 "neutral" 0>
  <R R1 1 300 90 15 -26 0 1 "3.9k" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <.AC AC1 1 460 110 0 33 0 0 "log" 1 "0.1 kHz" 1 "100 kHz" 1 "200" 1 "no" 0>
  <.DC DC1 1 140 330 0 33 0 0 "26.85" 0 "0.001" 0 "1 pA" 0 "1 uV" 0 "no" 0 "150" 0 "no" 0 "none" 0 "CroutLU" 0>
  <Amp X1 1 240 50 -26 27 0 0 "10" 1 "50 Ohm" 0 "50 Ohm" 0 "0 dB" 0>
</Components>
<Wires>
  <160 140 180 140 "" 0 0 0 "">
  <180 140 200 140 "" 0 0 0 "">
  <180 120 180 140 "" 0 0 0 "">
  <180 140 180 200 "" 0 0 0 "">
  <260 140 300 140 "" 0 0 0 "">
  <300 140 340 140 "" 0 0 0 "">
  <300 120 300 140 "" 0 0 0 "">
  <300 180 340 180 "" 0 0 0 "">
  <60 140 100 140 "" 0 0 0 "">
  <60 140 60 200 "" 0 0 0 "">
  <300 50 410 50 "" 0 0 0 "">
  <300 50 300 60 "" 0 0 0 "">
  <410 50 410 160 "Output" 450 50 30 "">
  <270 50 300 50 "" 0 0 0 "">
  <180 50 180 60 "" 0 0 0 "">
  <180 50 210 50 "" 0 0 0 "">
</Wires>
<Diagrams>
  <Rect 620 320 240 160 3 #c0c0c0 1 00 1 0 0.2 1 1 -0.1 0.5 1.1 1 -0.1 0.5 1.1 315 0 225 "" "" "">
	<"active_lp:acfrequency" #0000ff 0 3 0 0 0>
  </Rect>
  <Rect 600 0 240 160 3 #c0c0c0 1 00 1 0 0.2 1 1 -0.1 0.5 1.1 1 -0.1 0.5 1.1 315 0 225 "" "" "">
	<"active_lp:Phase" #0000ff 0 3 0 0 0>
  </Rect>
</Diagrams>
<Paintings>
  <Text 280 270 14 #000000 0 "active bandpass filter">
</Paintings>
