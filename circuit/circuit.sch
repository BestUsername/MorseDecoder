EESchema Schematic File Version 4
LIBS:circuit-cache
EELAYER 26 0
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
L MCU_Module:Arduino_UNO_R3 A?
U 1 1 5D74977C
P 4350 3200
F 0 "A?" H 4350 4378 50  0000 C CNN
F 1 "Arduino_UNO_R3" H 4350 4287 50  0000 C CNN
F 2 "Module:Arduino_UNO_R3" H 4500 2150 50  0001 L CNN
F 3 "https://www.arduino.cc/en/Main/arduinoBoardUno" H 4150 4250 50  0001 C CNN
	1    4350 3200
	-1   0    0    -1  
$EndComp
$Comp
L Display_Character:WC1602A DS?
U 1 1 5D74997E
P 7700 3200
F 0 "DS?" H 7700 4178 50  0000 C CNN
F 1 "WC1602A" H 7700 4087 50  0000 C CNN
F 2 "Display:WC1602A" H 7700 2300 50  0001 C CIN
F 3 "http://www.wincomlcd.com/pdf/WC1602A-SFYLYHTC06.pdf" H 8400 3200 50  0001 C CNN
	1    7700 3200
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 5D749CEB
P 5250 3200
F 0 "SW?" V 5204 3348 50  0000 L CNN
F 1 "SW_Push" V 5295 3348 50  0000 L CNN
F 2 "" H 5250 3400 50  0001 C CNN
F 3 "" H 5250 3400 50  0001 C CNN
	1    5250 3200
	1    0    0    -1  
$EndComp
$Comp
L Device:Buzzer BZ?
U 1 1 5D749EBC
P 5900 4000
F 0 "BZ?" H 5906 3675 50  0000 C CNN
F 1 "Buzzer" H 5906 3766 50  0000 C CNN
F 2 "" V 5875 4100 50  0001 C CNN
F 3 "~" V 5875 4100 50  0001 C CNN
	1    5900 4000
	1    0    0    1   
$EndComp
Entry Wire Line
	4450 4400 4350 4300
Entry Wire Line
	5450 4300 5350 4400
Wire Wire Line
	4150 2200 4150 2050
Wire Wire Line
	7700 4300 7700 4000
Wire Wire Line
	7700 2400 7700 2050
Wire Wire Line
	7700 2050 4150 2050
Wire Wire Line
	8100 2050 7700 2050
Connection ~ 7700 2050
Entry Wire Line
	8000 4400 8100 4300
Wire Wire Line
	8100 2600 8200 2600
Wire Wire Line
	4850 3300 6650 3300
Entry Wire Line
	6900 4300 6800 4400
Wire Wire Line
	7300 2700 6900 2700
Entry Wire Line
	7700 4300 7600 4400
Wire Wire Line
	6400 2600 6400 3400
Wire Wire Line
	6400 3400 4850 3400
Wire Wire Line
	6650 2800 7300 2800
Wire Wire Line
	4850 3500 7300 3500
Wire Wire Line
	4850 3600 7300 3600
Wire Wire Line
	4850 3700 7300 3700
Wire Wire Line
	4850 3800 7300 3800
Wire Wire Line
	6400 2600 7300 2600
Wire Wire Line
	6650 2800 6650 3300
Wire Wire Line
	4850 3900 5800 3900
Wire Wire Line
	5800 4100 5450 4100
Connection ~ 5450 4100
Wire Wire Line
	5450 4100 5450 3200
Wire Wire Line
	5450 4100 5450 4300
Wire Wire Line
	4850 3200 5050 3200
$Comp
L Device:R R?
U 1 1 5D75301E
P 8200 2750
F 0 "R?" H 8270 2796 50  0000 L CNN
F 1 "2.2K" H 8270 2705 50  0000 L CNN
F 2 "" V 8130 2750 50  0001 C CNN
F 3 "~" H 8200 2750 50  0001 C CNN
	1    8200 2750
	1    0    0    -1  
$EndComp
Entry Wire Line
	8100 4400 8200 4300
Wire Wire Line
	8100 3000 8100 4300
Wire Wire Line
	8200 2900 8200 4300
Wire Wire Line
	6900 2700 6900 4300
Wire Wire Line
	8100 2050 8100 2600
Connection ~ 8100 2600
Wire Wire Line
	8100 2600 8100 2900
Wire Bus Line
	4450 4400 8100 4400
$EndSCHEMATC
