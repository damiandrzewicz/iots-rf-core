EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L moteino:MoteinoR6_1 U1
U 1 1 6185BA0B
P 3850 3500
F 0 "U1" H 4200 2450 50  0000 C CNN
F 1 "MoteinoR6" H 3550 2450 50  0000 C CNN
F 2 "moteino:moteino-r6" H 3600 3350 50  0001 C CNN
F 3 "" H 3600 3350 50  0001 C CNN
	1    3850 3500
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J1
U 1 1 6185DB0E
P 5650 4150
F 0 "J1" H 5730 4142 50  0000 L CNN
F 1 "CR2450 Battery Holder" H 5730 4051 50  0000 L CNN
F 2 "Battery:BatteryHolder_Keystone_107_1x23mm" H 5650 4150 50  0001 C CNN
F 3 "~" H 5650 4150 50  0001 C CNN
	1    5650 4150
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 6185E322
P 9650 3700
F 0 "R1" H 9720 3746 50  0000 L CNN
F 1 "680k" H 9720 3655 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 9580 3700 50  0001 C CNN
F 3 "~" H 9650 3700 50  0001 C CNN
	1    9650 3700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 6185E5F7
P 9650 4200
F 0 "R2" H 9720 4246 50  0000 L CNN
F 1 "330" H 9720 4155 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 9580 4200 50  0001 C CNN
F 3 "~" H 9650 4200 50  0001 C CNN
	1    9650 4200
	1    0    0    -1  
$EndComp
Text GLabel 9450 3800 1    50   Input ~ 0
BattADC
Wire Wire Line
	9650 3850 9650 3950
Wire Wire Line
	9650 3950 9450 3950
Connection ~ 9650 3950
Wire Wire Line
	9650 3950 9650 4050
$Comp
L power:+3V0 #PWR01
U 1 1 61860534
P 3750 2350
F 0 "#PWR01" H 3750 2200 50  0001 C CNN
F 1 "+3V0" H 3765 2523 50  0000 C CNN
F 2 "" H 3750 2350 50  0001 C CNN
F 3 "" H 3750 2350 50  0001 C CNN
	1    3750 2350
	1    0    0    -1  
$EndComp
$Comp
L power:+3V0 #PWR02
U 1 1 61860DF0
P 5350 4050
F 0 "#PWR02" H 5350 3900 50  0001 C CNN
F 1 "+3V0" H 5365 4223 50  0000 C CNN
F 2 "" H 5350 4050 50  0001 C CNN
F 3 "" H 5350 4050 50  0001 C CNN
	1    5350 4050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 618612A2
P 9650 4850
F 0 "#PWR05" H 9650 4600 50  0001 C CNN
F 1 "GND" H 9655 4677 50  0000 C CNN
F 2 "" H 9650 4850 50  0001 C CNN
F 3 "" H 9650 4850 50  0001 C CNN
	1    9650 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 4150 5350 4150
Wire Wire Line
	5350 4150 5350 4050
$Comp
L power:GND #PWR03
U 1 1 61862A91
P 5350 4350
F 0 "#PWR03" H 5350 4100 50  0001 C CNN
F 1 "GND" H 5355 4177 50  0000 C CNN
F 2 "" H 5350 4350 50  0001 C CNN
F 3 "" H 5350 4350 50  0001 C CNN
	1    5350 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 4250 5350 4250
Wire Wire Line
	5350 4250 5350 4350
NoConn ~ 4350 2950
NoConn ~ 4350 3050
NoConn ~ 4350 3150
NoConn ~ 4350 3350
NoConn ~ 4350 3450
NoConn ~ 4350 3550
NoConn ~ 4350 3650
NoConn ~ 4350 3750
NoConn ~ 4350 3850
NoConn ~ 4350 3950
NoConn ~ 4350 4050
NoConn ~ 4350 4150
NoConn ~ 3350 3400
NoConn ~ 3350 3500
NoConn ~ 3350 3600
NoConn ~ 3350 3900
NoConn ~ 3350 4000
Wire Notes Line
	10000 4750 10000 2900
Wire Notes Line
	10000 2900 9200 2900
Wire Notes Line
	5150 3700 5150 4600
Wire Notes Line
	5150 4600 6700 4600
Wire Notes Line
	6700 4600 6700 3700
Wire Notes Line
	6700 3700 5150 3700
Text Notes 2900 2100 0    50   ~ 0
Moteino R6 with RFM69HCW
Text Notes 9250 3100 0    50   ~ 0
Battery Level \nMeasure
Text Notes 5700 3800 0    50   ~ 0
CR2450 Holder
NoConn ~ 3350 2850
$Comp
L power:+3V0 #PWR04
U 1 1 61889CE5
P 9650 3450
F 0 "#PWR04" H 9650 3300 50  0001 C CNN
F 1 "+3V0" H 9665 3623 50  0000 C CNN
F 2 "" H 9650 3450 50  0001 C CNN
F 3 "" H 9650 3450 50  0001 C CNN
	1    9650 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C1
U 1 1 61894DB0
P 6300 3050
F 0 "C1" H 6418 3096 50  0000 L CNN
F 1 "47u" H 6418 3005 50  0000 L CNN
F 2 "w_capacitors:CP_5x11mm_hor" H 6338 2900 50  0001 C CNN
F 3 "~" H 6300 3050 50  0001 C CNN
	1    6300 3050
	1    0    0    -1  
$EndComp
$Comp
L power:+3V0 #PWR06
U 1 1 61895242
P 6300 2750
F 0 "#PWR06" H 6300 2600 50  0001 C CNN
F 1 "+3V0" H 6315 2923 50  0000 C CNN
F 2 "" H 6300 2750 50  0001 C CNN
F 3 "" H 6300 2750 50  0001 C CNN
	1    6300 2750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR07
U 1 1 61895545
P 6300 3400
F 0 "#PWR07" H 6300 3150 50  0001 C CNN
F 1 "GND" H 6305 3227 50  0000 C CNN
F 2 "" H 6300 3400 50  0001 C CNN
F 3 "" H 6300 3400 50  0001 C CNN
	1    6300 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 2900 6300 2750
Wire Wire Line
	6300 3200 6300 3400
Wire Notes Line
	6000 2400 6000 3650
Wire Notes Line
	6000 3650 6700 3650
Wire Notes Line
	6700 3650 6700 2400
Wire Notes Line
	6700 2400 6000 2400
Text Notes 6050 2500 0    50   ~ 0
Power Booster
NoConn ~ 3350 4100
NoConn ~ 3350 4200
$Comp
L power:GND #PWR0101
U 1 1 618B077A
P 3850 4600
F 0 "#PWR0101" H 3850 4350 50  0001 C CNN
F 1 "GND" H 3855 4427 50  0000 C CNN
F 2 "" H 3850 4600 50  0001 C CNN
F 3 "" H 3850 4600 50  0001 C CNN
	1    3850 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 4600 3850 4500
Wire Wire Line
	3750 2500 3750 2350
NoConn ~ 3900 2500
$Comp
L Transistor_BJT:BC547 Q1
U 1 1 618D5FA3
P 9550 4600
F 0 "Q1" H 9741 4646 50  0000 L CNN
F 1 "BC547" H 9741 4555 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 9750 4525 50  0001 L CIN
F 3 "http://www.fairchildsemi.com/ds/BC/BC547.pdf" H 9550 4600 50  0001 L CNN
	1    9550 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	9650 4400 9650 4350
Wire Wire Line
	9650 4800 9650 4850
Wire Wire Line
	9650 3450 9650 3550
$Comp
L Device:R R3
U 1 1 618E379C
P 9250 4400
F 0 "R3" H 9320 4446 50  0000 L CNN
F 1 "2.2k" H 9320 4355 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 9180 4400 50  0001 C CNN
F 3 "~" H 9250 4400 50  0001 C CNN
	1    9250 4400
	1    0    0    -1  
$EndComp
Text GLabel 9250 4100 1    50   Input ~ 0
BattEN
Wire Wire Line
	9450 3950 9450 3800
Wire Wire Line
	9250 4100 9250 4250
Wire Wire Line
	9250 4550 9250 4600
Wire Wire Line
	9250 4600 9350 4600
NoConn ~ 3350 2950
NoConn ~ 3350 3050
NoConn ~ 4350 3250
NoConn ~ 3350 3300
NoConn ~ 3350 3700
NoConn ~ 3350 3800
$EndSCHEMATC
