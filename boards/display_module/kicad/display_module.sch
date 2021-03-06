EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:my_connectors
LIBS:my_transistors
LIBS:display_module-cache
EELAYER 25 0
EELAYER END
$Descr A2 23386 16535
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
L Crystal Y1
U 1 1 570A6642
P 3750 800
F 0 "Y1" H 3750 950 50  0000 C CNN
F 1 "16M" H 3750 650 50  0000 C CNN
F 2 "Crystals:Crystal_HC50_Vertical" H 3750 800 50  0001 C CNN
F 3 "" H 3750 800 50  0000 C CNN
	1    3750 800 
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 570A696B
P 4200 800
F 0 "#PWR01" H 4200 550 50  0001 C CNN
F 1 "GND" H 4200 650 50  0000 C CNN
F 2 "" H 4200 800 50  0000 C CNN
F 3 "" H 4200 800 50  0000 C CNN
	1    4200 800 
	0    -1   -1   0   
$EndComp
$Comp
L C C6
U 1 1 570A698E
P 4050 800
F 0 "C6" H 4075 900 50  0000 L CNN
F 1 "22p" H 4075 700 50  0000 L CNN
F 2 "Capacitors_ThroughHole:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 4088 650 50  0001 C CNN
F 3 "" H 4050 800 50  0000 C CNN
	1    4050 800 
	0    -1   -1   0   
$EndComp
$Comp
L C C5
U 1 1 570A6CE5
P 3450 800
F 0 "C5" H 3475 900 50  0000 L CNN
F 1 "22p" H 3475 700 50  0000 L CNN
F 2 "Capacitors_ThroughHole:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 3488 650 50  0001 C CNN
F 3 "" H 3450 800 50  0000 C CNN
	1    3450 800 
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR02
U 1 1 570A6D6F
P 3300 800
F 0 "#PWR02" H 3300 550 50  0001 C CNN
F 1 "GND" H 3300 650 50  0000 C CNN
F 2 "" H 3300 800 50  0000 C CNN
F 3 "" H 3300 800 50  0000 C CNN
	1    3300 800 
	0    1    1    0   
$EndComp
$Comp
L VCC #PWR03
U 1 1 570A714E
P 1300 1250
F 0 "#PWR03" H 1300 1100 50  0001 C CNN
F 1 "VCC" H 1300 1400 50  0000 C CNN
F 2 "" H 1300 1250 50  0000 C CNN
F 3 "" H 1300 1250 50  0000 C CNN
	1    1300 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 800  3600 2200
Wire Wire Line
	3900 800  3900 2300
Connection ~ 3900 800 
Connection ~ 3600 800 
Wire Wire Line
	3600 2200 3350 2200
Wire Wire Line
	3900 2300 3350 2300
Wire Wire Line
	1300 1250 1300 2800
Wire Wire Line
	1300 1600 1450 1600
Wire Wire Line
	1300 1900 1450 1900
Connection ~ 1300 1600
Wire Wire Line
	1300 2200 1450 2200
Connection ~ 1300 1900
$Comp
L GND #PWR04
U 1 1 570C40C4
P 1300 4050
F 0 "#PWR04" H 1300 3800 50  0001 C CNN
F 1 "GND" H 1300 3900 50  0000 C CNN
F 2 "" H 1300 4050 50  0000 C CNN
F 3 "" H 1300 4050 50  0000 C CNN
	1    1300 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 3900 1450 3900
Wire Wire Line
	1300 3100 1300 4050
Wire Wire Line
	1450 3800 1300 3800
Connection ~ 1300 3900
Wire Wire Line
	4100 1800 3350 1800
Wire Wire Line
	4200 2000 3350 2000
Wire Wire Line
	3350 2100 4250 2100
$Comp
L C C3
U 1 1 570CA704
P 1300 2950
F 0 "C3" H 1325 3050 50  0000 L CNN
F 1 "100n" H 1325 2850 50  0000 L CNN
F 2 "Capacitors_ThroughHole:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 1338 2800 50  0001 C CNN
F 3 "" H 1300 2950 50  0000 C CNN
	1    1300 2950
	1    0    0    -1  
$EndComp
Connection ~ 1300 2200
Connection ~ 1300 3800
$Comp
L CONN_01X02 P1
U 1 1 570CB3DC
P 2400 5250
F 0 "P1" H 2400 5400 50  0000 C CNN
F 1 "POWER" V 2500 5250 50  0000 C CNN
F 2 "Connect:bornier2" H 2400 5250 50  0001 C CNN
F 3 "" H 2400 5250 50  0000 C CNN
	1    2400 5250
	1    0    0    -1  
$EndComp
$Comp
L CP C4
U 1 1 570CB55C
P 1950 5250
F 0 "C4" H 1975 5350 50  0000 L CNN
F 1 "10uF" H 1975 5150 50  0000 L CNN
F 2 "Capacitors_ThroughHole:CP_Radial_D5.0mm_P2.50mm" H 1988 5100 50  0001 C CNN
F 3 "" H 1950 5250 50  0000 C CNN
	1    1950 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 5400 1950 5600
Wire Wire Line
	750  5450 2200 5450
Wire Wire Line
	2200 5450 2200 5300
$Comp
L GND #PWR05
U 1 1 570CC037
P 1950 5600
F 0 "#PWR05" H 1950 5350 50  0001 C CNN
F 1 "GND" H 1950 5450 50  0000 C CNN
F 2 "" H 1950 5600 50  0000 C CNN
F 3 "" H 1950 5600 50  0000 C CNN
	1    1950 5600
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR06
U 1 1 570CC510
P 1950 4900
F 0 "#PWR06" H 1950 4750 50  0001 C CNN
F 1 "VCC" H 1950 5050 50  0000 C CNN
F 2 "" H 1950 4900 50  0000 C CNN
F 3 "" H 1950 4900 50  0000 C CNN
	1    1950 4900
	1    0    0    -1  
$EndComp
Connection ~ 1950 5450
Wire Wire Line
	1950 4900 1950 5100
Wire Wire Line
	2200 5050 2200 5200
Wire Wire Line
	750  5050 2200 5050
Connection ~ 1950 5050
$Comp
L SW10 SPI_OUT1
U 1 1 570C2B9B
P 5900 800
F 0 "SPI_OUT1" H 5900 950 60  0000 C CNN
F 1 "SW10" H 5900 850 60  0000 C CNN
F 2 "my_footprints:SW10" H 5900 800 60  0001 C CNN
F 3 "" H 5900 800 60  0000 C CNN
	1    5900 800 
	0    -1   -1   0   
$EndComp
$Comp
L SW10 SPI_IN1
U 1 1 570C333B
P 7250 800
F 0 "SPI_IN1" H 7250 950 60  0000 C CNN
F 1 "SW10" H 7250 850 60  0000 C CNN
F 2 "my_footprints:SW10" H 7250 800 60  0001 C CNN
F 3 "" H 7250 800 60  0000 C CNN
	1    7250 800 
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5300 1450 5300 1200
Wire Wire Line
	4100 1800 4100 1500
Wire Wire Line
	4100 1500 6650 1500
Wire Wire Line
	6650 1200 6650 1750
Wire Wire Line
	3350 1900 4150 1900
Wire Wire Line
	4150 1900 4150 1550
Wire Wire Line
	5000 1550 5000 1200
Wire Wire Line
	6350 1550 6350 1200
Connection ~ 5000 1550
Wire Wire Line
	4200 2000 4200 1600
Wire Wire Line
	4200 1600 7550 1600
Wire Wire Line
	4900 1600 4900 1200
Wire Wire Line
	6250 1600 6250 1200
Connection ~ 4900 1600
Wire Wire Line
	4250 2100 4250 1650
Wire Wire Line
	4250 1650 7750 1650
Wire Wire Line
	4800 1650 4800 1200
Wire Wire Line
	6150 1650 6150 1200
Connection ~ 4800 1650
$Comp
L GND #PWR07
U 1 1 570C6148
P 5100 1200
F 0 "#PWR07" H 5100 950 50  0001 C CNN
F 1 "GND" H 5100 1050 50  0000 C CNN
F 2 "" H 5100 1200 50  0000 C CNN
F 3 "" H 5100 1200 50  0000 C CNN
	1    5100 1200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR08
U 1 1 570C6249
P 6450 1200
F 0 "#PWR08" H 6450 950 50  0001 C CNN
F 1 "GND" H 6450 1050 50  0000 C CNN
F 2 "" H 6450 1200 50  0000 C CNN
F 3 "" H 6450 1200 50  0000 C CNN
	1    6450 1200
	1    0    0    -1  
$EndComp
NoConn ~ 4700 1200
NoConn ~ 5200 1200
NoConn ~ 5400 1200
NoConn ~ 5500 1200
NoConn ~ 6050 1200
NoConn ~ 6550 1200
NoConn ~ 6750 1200
NoConn ~ 6850 1200
$Comp
L ATMEGA328-P IC1
U 1 1 570A55DB
P 2350 2700
F 0 "IC1" H 1600 3950 50  0000 L BNN
F 1 "ATMEGA328-P" H 2750 1300 50  0000 L BNN
F 2 "Housings_DIP:DIP-28_W7.62mm" H 2350 2700 50  0001 C CIN
F 3 "" H 2350 2700 50  0000 C CNN
	1    2350 2700
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 57171CF6
P 1000 5250
F 0 "C2" H 1025 5350 50  0000 L CNN
F 1 "100n" H 1025 5150 50  0000 L CNN
F 2 "Capacitors_ThroughHole:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 1038 5100 50  0001 C CNN
F 3 "" H 1000 5250 50  0000 C CNN
	1    1000 5250
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 57171DDF
P 750 5250
F 0 "C1" H 775 5350 50  0000 L CNN
F 1 "100n" H 775 5150 50  0000 L CNN
F 2 "Capacitors_ThroughHole:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 788 5100 50  0001 C CNN
F 3 "" H 750 5250 50  0000 C CNN
	1    750  5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	750  5450 750  5400
Wire Wire Line
	750  5100 750  5050
Wire Wire Line
	1000 5100 1000 5050
Connection ~ 1000 5050
Wire Wire Line
	1000 5400 1000 5450
Connection ~ 1000 5450
$Comp
L TLC5940NT U1
U 1 1 58C536C3
P 5500 8000
F 0 "U1" H 5000 8875 50  0000 L CNN
F 1 "TLC5940NT" H 6000 8875 50  0000 R CNN
F 2 "Housings_DIP:DIP-28_W7.62mm" H 5550 7025 50  0001 L CNN
F 3 "" H 5100 8700 50  0001 C CNN
	1    5500 8000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 1450 4050 1600
Wire Wire Line
	4050 1600 3350 1600
$Comp
L NDP6020P Q3
U 1 1 58C515E1
P 7100 5200
F 0 "Q3" H 7350 5275 50  0000 L CNN
F 1 "NDP6020P" H 7350 5200 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-220_Vertical" H 7350 5125 50  0001 L CIN
F 3 "" H 7100 5200 50  0001 L CNN
	1    7100 5200
	1    0    0    -1  
$EndComp
$Comp
L NDP6020P Q4
U 1 1 58C5183B
P 8000 5200
F 0 "Q4" H 8250 5275 50  0000 L CNN
F 1 "NDP6020P" H 8250 5200 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-220_Vertical" H 8250 5125 50  0001 L CIN
F 3 "" H 8000 5200 50  0001 L CNN
	1    8000 5200
	1    0    0    -1  
$EndComp
$Comp
L NDP6020P Q1
U 1 1 58C51AE9
P 5300 5200
F 0 "Q1" H 5550 5275 50  0000 L CNN
F 1 "NDP6020P" H 5550 5200 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-220_Vertical" H 5550 5125 50  0001 L CIN
F 3 "" H 5300 5200 50  0001 L CNN
	1    5300 5200
	1    0    0    -1  
$EndComp
$Comp
L NDP6020P Q2
U 1 1 58C51AEF
P 6200 5200
F 0 "Q2" H 6450 5275 50  0000 L CNN
F 1 "NDP6020P" H 6450 5200 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-220_Vertical" H 6450 5125 50  0001 L CIN
F 3 "" H 6200 5200 50  0001 L CNN
	1    6200 5200
	1    0    0    -1  
$EndComp
$Comp
L NDP6020P Q7
U 1 1 58C51E0D
P 10700 5200
F 0 "Q7" H 10950 5275 50  0000 L CNN
F 1 "NDP6020P" H 10950 5200 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-220_Vertical" H 10950 5125 50  0001 L CIN
F 3 "" H 10700 5200 50  0001 L CNN
	1    10700 5200
	1    0    0    -1  
$EndComp
$Comp
L NDP6020P Q8
U 1 1 58C51E13
P 11600 5200
F 0 "Q8" H 11850 5275 50  0000 L CNN
F 1 "NDP6020P" H 11850 5200 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-220_Vertical" H 11850 5125 50  0001 L CIN
F 3 "" H 11600 5200 50  0001 L CNN
	1    11600 5200
	1    0    0    -1  
$EndComp
$Comp
L NDP6020P Q5
U 1 1 58C51E19
P 8900 5200
F 0 "Q5" H 9150 5275 50  0000 L CNN
F 1 "NDP6020P" H 9150 5200 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-220_Vertical" H 9150 5125 50  0001 L CIN
F 3 "" H 8900 5200 50  0001 L CNN
	1    8900 5200
	1    0    0    -1  
$EndComp
$Comp
L NDP6020P Q6
U 1 1 58C51E1F
P 9800 5200
F 0 "Q6" H 10050 5275 50  0000 L CNN
F 1 "NDP6020P" H 10050 5200 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-220_Vertical" H 10050 5125 50  0001 L CIN
F 3 "" H 9800 5200 50  0001 L CNN
	1    9800 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 5000 12000 5000
Wire Wire Line
	12000 5000 12000 4800
$Comp
L VCC #PWR09
U 1 1 58C51F1D
P 12000 4800
F 0 "#PWR09" H 12000 4650 50  0001 C CNN
F 1 "VCC" H 12000 4950 50  0000 C CNN
F 2 "" H 12000 4800 50  0001 C CNN
F 3 "" H 12000 4800 50  0001 C CNN
	1    12000 4800
	1    0    0    -1  
$EndComp
$Comp
L LM23088A DISP1
U 1 1 58C523BA
P 7200 8050
F 0 "DISP1" H 7200 7075 60  0000 C CNN
F 1 "LM23088A" H 7200 6975 60  0000 C CNN
F 2 "my_footprints:LM23088A" H 7000 7950 60  0001 C CNN
F 3 "" H 7000 7950 60  0000 C CNN
	1    7200 8050
	1    0    0    -1  
$EndComp
$Comp
L TLC5940NT U2
U 1 1 58C52700
P 9600 8000
F 0 "U2" H 9100 8875 50  0000 L CNN
F 1 "TLC5940NT" H 10100 8875 50  0000 R CNN
F 2 "Housings_DIP:DIP-28_W7.62mm" H 9650 7025 50  0001 L CNN
F 3 "" H 9200 8700 50  0001 C CNN
	1    9600 8000
	1    0    0    -1  
$EndComp
$Comp
L LM23088A DISP2
U 1 1 58C52706
P 11300 8050
F 0 "DISP2" H 11300 7075 60  0000 C CNN
F 1 "LM23088A" H 11300 6975 60  0000 C CNN
F 2 "my_footprints:LM23088A" H 11100 7950 60  0001 C CNN
F 3 "" H 11100 7950 60  0000 C CNN
	1    11300 8050
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 6900 7600 6800
Wire Wire Line
	7600 6800 11700 6800
Wire Wire Line
	11700 5400 11700 6900
Connection ~ 11700 6800
Wire Wire Line
	11600 6700 11600 6900
Wire Wire Line
	7500 6700 11600 6700
Wire Wire Line
	7500 6700 7500 6900
Wire Wire Line
	7400 6900 7400 6600
Wire Wire Line
	7400 6600 11500 6600
Wire Wire Line
	11500 6600 11500 6900
Wire Wire Line
	11400 6500 11400 6900
Wire Wire Line
	7300 6500 11400 6500
Wire Wire Line
	7300 6500 7300 6900
Wire Wire Line
	7200 6900 7200 6400
Wire Wire Line
	7200 6400 11300 6400
Wire Wire Line
	11300 6400 11300 6900
Wire Wire Line
	11200 6300 11200 6900
Wire Wire Line
	7100 6300 11200 6300
Wire Wire Line
	7100 6300 7100 6900
Wire Wire Line
	7000 6900 7000 6200
Wire Wire Line
	6300 6200 11100 6200
Wire Wire Line
	11100 6200 11100 6900
Wire Wire Line
	11000 6100 11000 6900
Wire Wire Line
	5400 6100 11000 6100
Wire Wire Line
	6900 6100 6900 6900
Wire Wire Line
	10800 5400 10800 6700
Connection ~ 10800 6700
Wire Wire Line
	9900 5400 9900 6600
Connection ~ 9900 6600
Wire Wire Line
	9000 5400 9000 6500
Connection ~ 9000 6500
Wire Wire Line
	8100 5400 8100 6400
Connection ~ 8100 6400
Wire Wire Line
	7200 5400 7200 6300
Connection ~ 7200 6300
Wire Wire Line
	6300 5400 6300 6200
Connection ~ 7000 6200
Wire Wire Line
	5400 5400 5400 6100
Connection ~ 6900 6100
Wire Wire Line
	3350 3900 11400 3900
Wire Wire Line
	11400 3200 11400 5150
Wire Wire Line
	10500 3200 10500 5150
Wire Wire Line
	10500 3800 3350 3800
Wire Wire Line
	3350 3700 9600 3700
Wire Wire Line
	9600 3200 9600 5150
Wire Wire Line
	3350 3600 8700 3600
Wire Wire Line
	8700 3200 8700 5150
Wire Wire Line
	3350 3500 7800 3500
Wire Wire Line
	7800 3200 7800 5150
Wire Wire Line
	3350 3400 6900 3400
Wire Wire Line
	6900 3200 6900 5150
Wire Wire Line
	3350 3300 6000 3300
Wire Wire Line
	6000 3200 6000 5150
Wire Wire Line
	3350 3200 5100 3200
Wire Wire Line
	5100 3200 5100 5150
Wire Wire Line
	8900 8700 7900 8700
Wire Wire Line
	7900 8700 7900 9300
Wire Wire Line
	7900 9300 4800 9300
Wire Wire Line
	4800 9300 4800 8800
Wire Wire Line
	3350 2650 4600 2650
Wire Wire Line
	4600 2650 4600 9400
Wire Wire Line
	4600 7800 4800 7800
Wire Wire Line
	4600 9400 8700 9400
Wire Wire Line
	8700 9400 8700 7800
Wire Wire Line
	8700 7800 8900 7800
Connection ~ 4600 7800
Wire Wire Line
	3350 2750 4500 2750
Wire Wire Line
	4500 2750 4500 9500
Wire Wire Line
	4500 7900 4800 7900
Wire Wire Line
	4500 9500 8800 9500
Wire Wire Line
	8800 9500 8800 7900
Wire Wire Line
	8800 7900 8900 7900
Connection ~ 4500 7900
Wire Wire Line
	3350 2850 4400 2850
Wire Wire Line
	4400 2850 4400 9600
Wire Wire Line
	4400 9600 8600 9600
Wire Wire Line
	3350 2950 4300 2950
Wire Wire Line
	4300 2950 4300 8700
Wire Wire Line
	4300 8700 4800 8700
Wire Wire Line
	4800 8600 4400 8600
Connection ~ 4400 8600
Wire Wire Line
	8600 9600 8600 8600
Wire Wire Line
	8600 8600 8900 8600
Wire Wire Line
	3350 1700 4050 1700
Wire Wire Line
	4050 1700 4050 9700
Wire Wire Line
	4050 7600 4800 7600
Wire Wire Line
	4050 9700 8500 9700
Wire Wire Line
	8500 9700 8500 7600
Wire Wire Line
	8500 7600 8900 7600
Connection ~ 4050 7600
$Comp
L R R1
U 1 1 58C54CD6
P 3650 7400
F 0 "R1" V 3730 7400 50  0000 C CNN
F 1 "380" V 3650 7400 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 3580 7400 50  0001 C CNN
F 3 "" H 3650 7400 50  0001 C CNN
	1    3650 7400
	0    1    1    0   
$EndComp
$Comp
L GND #PWR010
U 1 1 58C5512C
P 3500 7400
F 0 "#PWR010" H 3500 7150 50  0001 C CNN
F 1 "GND" H 3500 7250 50  0000 C CNN
F 2 "" H 3500 7400 50  0001 C CNN
F 3 "" H 3500 7400 50  0001 C CNN
	1    3500 7400
	0    1    1    0   
$EndComp
Wire Wire Line
	4800 7400 3800 7400
$Comp
L R R8
U 1 1 58C553BA
P 8150 7400
F 0 "R8" V 8230 7400 50  0000 C CNN
F 1 "380" V 8150 7400 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 8080 7400 50  0001 C CNN
F 3 "" H 8150 7400 50  0001 C CNN
	1    8150 7400
	0    1    1    0   
$EndComp
$Comp
L GND #PWR011
U 1 1 58C553C0
P 8000 7400
F 0 "#PWR011" H 8000 7150 50  0001 C CNN
F 1 "GND" H 8000 7250 50  0000 C CNN
F 2 "" H 8000 7400 50  0001 C CNN
F 3 "" H 8000 7400 50  0001 C CNN
	1    8000 7400
	0    1    1    0   
$EndComp
Wire Wire Line
	8900 7400 8300 7400
$Comp
L GND #PWR012
U 1 1 58C55A32
P 5500 9100
F 0 "#PWR012" H 5500 8850 50  0001 C CNN
F 1 "GND" H 5500 8950 50  0000 C CNN
F 2 "" H 5500 9100 50  0001 C CNN
F 3 "" H 5500 9100 50  0001 C CNN
	1    5500 9100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR013
U 1 1 58C55A8E
P 9600 9100
F 0 "#PWR013" H 9600 8850 50  0001 C CNN
F 1 "GND" H 9600 8950 50  0000 C CNN
F 2 "" H 9600 9100 50  0001 C CNN
F 3 "" H 9600 9100 50  0001 C CNN
	1    9600 9100
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR014
U 1 1 58C55B20
P 9600 7000
F 0 "#PWR014" H 9600 6850 50  0001 C CNN
F 1 "VCC" H 9600 7150 50  0000 C CNN
F 2 "" H 9600 7000 50  0001 C CNN
F 3 "" H 9600 7000 50  0001 C CNN
	1    9600 7000
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR015
U 1 1 58C55D53
P 5500 7000
F 0 "#PWR015" H 5500 6850 50  0001 C CNN
F 1 "VCC" H 5500 7150 50  0000 C CNN
F 2 "" H 5500 7000 50  0001 C CNN
F 3 "" H 5500 7000 50  0001 C CNN
	1    5500 7000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 7300 6450 7300
Wire Wire Line
	6200 7400 6450 7400
Wire Wire Line
	6200 7500 6450 7500
Wire Wire Line
	6200 7600 6450 7600
Wire Wire Line
	6200 7700 6450 7700
Wire Wire Line
	6200 7800 6450 7800
Wire Wire Line
	6200 7900 6450 7900
Wire Wire Line
	6200 8000 6450 8000
Wire Wire Line
	6200 8100 6450 8100
Wire Wire Line
	6200 8200 6450 8200
Wire Wire Line
	6200 8300 6450 8300
Wire Wire Line
	6200 8400 6450 8400
Wire Wire Line
	6200 8500 6450 8500
Wire Wire Line
	6200 8600 6450 8600
Wire Wire Line
	6200 8700 6450 8700
Wire Wire Line
	6200 8800 6450 8800
$Comp
L R R2
U 1 1 58C56A2D
P 5450 1900
F 0 "R2" V 5530 1900 50  0000 C CNN
F 1 "380" V 5450 1900 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 5380 1900 50  0001 C CNN
F 3 "" H 5450 1900 50  0001 C CNN
	1    5450 1900
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 58C56EAF
P 5650 1900
F 0 "R3" V 5730 1900 50  0000 C CNN
F 1 "190" V 5650 1900 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 5580 1900 50  0001 C CNN
F 3 "" H 5650 1900 50  0001 C CNN
	1    5650 1900
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR016
U 1 1 58C56FE8
P 5650 2050
F 0 "#PWR016" H 5650 1900 50  0001 C CNN
F 1 "VCC" H 5650 2200 50  0000 C CNN
F 2 "" H 5650 2050 50  0001 C CNN
F 3 "" H 5650 2050 50  0001 C CNN
	1    5650 2050
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR017
U 1 1 58C57104
P 5450 2050
F 0 "#PWR017" H 5450 1800 50  0001 C CNN
F 1 "GND" H 5450 1900 50  0000 C CNN
F 2 "" H 5450 2050 50  0000 C CNN
F 3 "" H 5450 2050 50  0000 C CNN
	1    5450 2050
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 58C57389
P 5900 1900
F 0 "R4" V 5980 1900 50  0000 C CNN
F 1 "380" V 5900 1900 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 5830 1900 50  0001 C CNN
F 3 "" H 5900 1900 50  0001 C CNN
	1    5900 1900
	1    0    0    -1  
$EndComp
$Comp
L R R5
U 1 1 58C5738F
P 6100 1900
F 0 "R5" V 6180 1900 50  0000 C CNN
F 1 "190" V 6100 1900 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 6030 1900 50  0001 C CNN
F 3 "" H 6100 1900 50  0001 C CNN
	1    6100 1900
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR018
U 1 1 58C57395
P 6100 2050
F 0 "#PWR018" H 6100 1900 50  0001 C CNN
F 1 "VCC" H 6100 2200 50  0000 C CNN
F 2 "" H 6100 2050 50  0001 C CNN
F 3 "" H 6100 2050 50  0001 C CNN
	1    6100 2050
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR019
U 1 1 58C5739B
P 5900 2050
F 0 "#PWR019" H 5900 1800 50  0001 C CNN
F 1 "GND" H 5900 1900 50  0000 C CNN
F 2 "" H 5900 2050 50  0000 C CNN
F 3 "" H 5900 2050 50  0000 C CNN
	1    5900 2050
	1    0    0    -1  
$EndComp
$Comp
L R R6
U 1 1 58C5744D
P 6450 1900
F 0 "R6" V 6530 1900 50  0000 C CNN
F 1 "380" V 6450 1900 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 6380 1900 50  0001 C CNN
F 3 "" H 6450 1900 50  0001 C CNN
	1    6450 1900
	1    0    0    -1  
$EndComp
$Comp
L R R7
U 1 1 58C57453
P 6650 1900
F 0 "R7" V 6730 1900 50  0000 C CNN
F 1 "190" V 6650 1900 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 6580 1900 50  0001 C CNN
F 3 "" H 6650 1900 50  0001 C CNN
	1    6650 1900
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR020
U 1 1 58C57459
P 6650 2050
F 0 "#PWR020" H 6650 1900 50  0001 C CNN
F 1 "VCC" H 6650 2200 50  0000 C CNN
F 2 "" H 6650 2050 50  0001 C CNN
F 3 "" H 6650 2050 50  0001 C CNN
	1    6650 2050
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR021
U 1 1 58C5745F
P 6450 2050
F 0 "#PWR021" H 6450 1800 50  0001 C CNN
F 1 "GND" H 6450 1900 50  0000 C CNN
F 2 "" H 6450 2050 50  0000 C CNN
F 3 "" H 6450 2050 50  0000 C CNN
	1    6450 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 1650 5450 1750
Wire Wire Line
	5650 1750 5650 1650
Wire Wire Line
	5900 1550 5900 1750
Wire Wire Line
	6100 1550 6100 1750
Connection ~ 6650 1500
Wire Wire Line
	6450 1750 6450 1500
Connection ~ 6450 1500
Wire Wire Line
	4050 1450 5300 1450
Connection ~ 6100 1550
Connection ~ 5900 1550
Connection ~ 5650 1650
Connection ~ 5450 1650
Wire Wire Line
	10300 7300 10550 7300
Wire Wire Line
	10300 7400 10550 7400
Wire Wire Line
	10300 7500 10550 7500
Wire Wire Line
	10300 7600 10550 7600
Wire Wire Line
	10300 7700 10550 7700
Wire Wire Line
	10300 7800 10550 7800
Wire Wire Line
	10300 7900 10550 7900
Wire Wire Line
	10300 8000 10550 8000
Wire Wire Line
	10300 8100 10550 8100
Wire Wire Line
	10300 8200 10550 8200
Wire Wire Line
	10300 8300 10550 8300
Wire Wire Line
	10300 8400 10550 8400
Wire Wire Line
	10300 8500 10550 8500
Wire Wire Line
	10300 8600 10550 8600
Wire Wire Line
	10300 8700 10550 8700
Wire Wire Line
	10300 8800 10550 8800
Wire Wire Line
	8900 7300 8850 7300
Wire Wire Line
	8850 7300 8850 9100
Wire Wire Line
	8850 7500 8900 7500
Connection ~ 8850 7500
Wire Wire Line
	8850 9100 9600 9100
Connection ~ 9600 9100
Connection ~ 5500 9100
Wire Wire Line
	4800 7300 4700 7300
Wire Wire Line
	4700 7300 4700 9100
Wire Wire Line
	4700 9100 5500 9100
Wire Wire Line
	4700 7500 4800 7500
Connection ~ 4700 7500
NoConn ~ 5600 1200
NoConn ~ 6950 1200
Connection ~ 6300 5000
Connection ~ 7200 5000
Connection ~ 8100 5000
Connection ~ 9000 5000
Connection ~ 9900 5000
Connection ~ 10800 5000
Connection ~ 11700 5000
NoConn ~ 8900 8800
NoConn ~ 8900 8200
NoConn ~ 4800 8200
$Comp
L R R10
U 1 1 58E7BA6E
P 6000 3050
F 0 "R10" V 6080 3050 50  0000 C CNN
F 1 "10k" V 6000 3050 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 5930 3050 50  0001 C CNN
F 3 "" H 6000 3050 50  0001 C CNN
	1    6000 3050
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR022
U 1 1 58E7BBBB
P 6000 2900
F 0 "#PWR022" H 6000 2750 50  0001 C CNN
F 1 "VCC" H 6000 3050 50  0000 C CNN
F 2 "" H 6000 2900 50  0001 C CNN
F 3 "" H 6000 2900 50  0001 C CNN
	1    6000 2900
	1    0    0    -1  
$EndComp
Connection ~ 5100 3200
$Comp
L R R9
U 1 1 58E7BDC2
P 5100 3050
F 0 "R9" V 5180 3050 50  0000 C CNN
F 1 "10k" V 5100 3050 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 5030 3050 50  0001 C CNN
F 3 "" H 5100 3050 50  0001 C CNN
	1    5100 3050
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR023
U 1 1 58E7BDC8
P 5100 2900
F 0 "#PWR023" H 5100 2750 50  0001 C CNN
F 1 "VCC" H 5100 3050 50  0000 C CNN
F 2 "" H 5100 2900 50  0001 C CNN
F 3 "" H 5100 2900 50  0001 C CNN
	1    5100 2900
	1    0    0    -1  
$EndComp
$Comp
L R R11
U 1 1 58E7BEB3
P 6900 3050
F 0 "R11" V 6980 3050 50  0000 C CNN
F 1 "10k" V 6900 3050 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 6830 3050 50  0001 C CNN
F 3 "" H 6900 3050 50  0001 C CNN
	1    6900 3050
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR024
U 1 1 58E7BEB9
P 6900 2900
F 0 "#PWR024" H 6900 2750 50  0001 C CNN
F 1 "VCC" H 6900 3050 50  0000 C CNN
F 2 "" H 6900 2900 50  0001 C CNN
F 3 "" H 6900 2900 50  0001 C CNN
	1    6900 2900
	1    0    0    -1  
$EndComp
$Comp
L R R12
U 1 1 58E7C037
P 7800 3050
F 0 "R12" V 7880 3050 50  0000 C CNN
F 1 "10k" V 7800 3050 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 7730 3050 50  0001 C CNN
F 3 "" H 7800 3050 50  0001 C CNN
	1    7800 3050
	1    0    0    -1  
$EndComp
$Comp
L R R13
U 1 1 58E7C0D0
P 8700 3050
F 0 "R13" V 8780 3050 50  0000 C CNN
F 1 "10k" V 8700 3050 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 8630 3050 50  0001 C CNN
F 3 "" H 8700 3050 50  0001 C CNN
	1    8700 3050
	1    0    0    -1  
$EndComp
$Comp
L R R14
U 1 1 58E7C170
P 9600 3050
F 0 "R14" V 9680 3050 50  0000 C CNN
F 1 "10k" V 9600 3050 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 9530 3050 50  0001 C CNN
F 3 "" H 9600 3050 50  0001 C CNN
	1    9600 3050
	1    0    0    -1  
$EndComp
$Comp
L R R15
U 1 1 58E7C21B
P 10500 3050
F 0 "R15" V 10580 3050 50  0000 C CNN
F 1 "10k" V 10500 3050 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 10430 3050 50  0001 C CNN
F 3 "" H 10500 3050 50  0001 C CNN
	1    10500 3050
	1    0    0    -1  
$EndComp
$Comp
L R R16
U 1 1 58E7C2C5
P 11400 3050
F 0 "R16" V 11480 3050 50  0000 C CNN
F 1 "10k" V 11400 3050 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 11330 3050 50  0001 C CNN
F 3 "" H 11400 3050 50  0001 C CNN
	1    11400 3050
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR025
U 1 1 58E7C364
P 7800 2900
F 0 "#PWR025" H 7800 2750 50  0001 C CNN
F 1 "VCC" H 7800 3050 50  0000 C CNN
F 2 "" H 7800 2900 50  0001 C CNN
F 3 "" H 7800 2900 50  0001 C CNN
	1    7800 2900
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR026
U 1 1 58E7C3E4
P 8700 2900
F 0 "#PWR026" H 8700 2750 50  0001 C CNN
F 1 "VCC" H 8700 3050 50  0000 C CNN
F 2 "" H 8700 2900 50  0001 C CNN
F 3 "" H 8700 2900 50  0001 C CNN
	1    8700 2900
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR027
U 1 1 58E7C464
P 9600 2900
F 0 "#PWR027" H 9600 2750 50  0001 C CNN
F 1 "VCC" H 9600 3050 50  0000 C CNN
F 2 "" H 9600 2900 50  0001 C CNN
F 3 "" H 9600 2900 50  0001 C CNN
	1    9600 2900
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR028
U 1 1 58E7C4E4
P 10500 2900
F 0 "#PWR028" H 10500 2750 50  0001 C CNN
F 1 "VCC" H 10500 3050 50  0000 C CNN
F 2 "" H 10500 2900 50  0001 C CNN
F 3 "" H 10500 2900 50  0001 C CNN
	1    10500 2900
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR029
U 1 1 58E7C564
P 11400 2900
F 0 "#PWR029" H 11400 2750 50  0001 C CNN
F 1 "VCC" H 11400 3050 50  0000 C CNN
F 2 "" H 11400 2900 50  0001 C CNN
F 3 "" H 11400 2900 50  0001 C CNN
	1    11400 2900
	1    0    0    -1  
$EndComp
Connection ~ 6000 3300
Connection ~ 6900 3400
Connection ~ 7800 3500
Connection ~ 8700 3600
Connection ~ 9600 3700
Connection ~ 10500 3800
Connection ~ 11400 3900
Connection ~ 6250 1600
Connection ~ 6150 1650
Connection ~ 6350 1550
Wire Wire Line
	3350 3050 4200 3050
Wire Wire Line
	4200 3050 4200 2150
Wire Wire Line
	4200 2150 4300 2150
Wire Wire Line
	4300 2150 4300 1700
Wire Wire Line
	4300 1700 7950 1700
Wire Wire Line
	4150 1550 7850 1550
$Comp
L ICSP U3
U 1 1 58E8012C
P 7750 1000
F 0 "U3" H 8050 1300 60  0000 C CNN
F 1 "ICSP" H 7600 1300 60  0000 C CNN
F 2 "my_footprints:PINS3x2" H 7620 860 60  0001 C CNN
F 3 "" H 7620 860 60  0001 C CNN
	1    7750 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	7850 1550 7850 1100
Wire Wire Line
	7550 1600 7550 1100
Wire Wire Line
	7750 1650 7750 1100
Wire Wire Line
	7950 1700 7950 1100
$Comp
L VCC #PWR030
U 1 1 58E8060B
P 7650 1100
F 0 "#PWR030" H 7650 950 50  0001 C CNN
F 1 "VCC" H 7650 1250 50  0000 C CNN
F 2 "" H 7650 1100 50  0001 C CNN
F 3 "" H 7650 1100 50  0001 C CNN
	1    7650 1100
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR031
U 1 1 58E8068E
P 8050 1100
F 0 "#PWR031" H 8050 850 50  0001 C CNN
F 1 "GND" H 8050 950 50  0000 C CNN
F 2 "" H 8050 1100 50  0000 C CNN
F 3 "" H 8050 1100 50  0000 C CNN
	1    8050 1100
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X02 J1
U 1 1 58E80D92
P 4550 2000
F 0 "J1" H 4550 2150 50  0000 C CNN
F 1 "SELFTEST" V 4650 2000 50  0000 C CNN
F 2 "my_footprints:PINS1x2" H 4550 2000 50  0001 C CNN
F 3 "" H 4550 2000 50  0001 C CNN
	1    4550 2000
	0    -1   -1   0   
$EndComp
$Comp
L CONN_01X02 J2
U 1 1 58E80E8C
P 5050 2000
F 0 "J2" H 5050 2150 50  0000 C CNN
F 1 "AUX" V 5150 2000 50  0000 C CNN
F 2 "my_footprints:PINS1x2" H 5050 2000 50  0001 C CNN
F 3 "" H 5050 2000 50  0001 C CNN
	1    5050 2000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3350 2450 4500 2450
Wire Wire Line
	4500 2450 4500 2200
Wire Wire Line
	3350 2550 5000 2550
Wire Wire Line
	5000 2550 5000 2200
$Comp
L GND #PWR032
U 1 1 58E812EF
P 4600 2200
F 0 "#PWR032" H 4600 1950 50  0001 C CNN
F 1 "GND" H 4600 2050 50  0000 C CNN
F 2 "" H 4600 2200 50  0000 C CNN
F 3 "" H 4600 2200 50  0000 C CNN
	1    4600 2200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR033
U 1 1 58E81378
P 5100 2200
F 0 "#PWR033" H 5100 1950 50  0001 C CNN
F 1 "GND" H 5100 2050 50  0000 C CNN
F 2 "" H 5100 2200 50  0000 C CNN
F 3 "" H 5100 2200 50  0000 C CNN
	1    5100 2200
	1    0    0    -1  
$EndComp
$EndSCHEMATC
