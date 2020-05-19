//
// GPIO_Keypad : 3x3 keypad input and control LEDs (or Relays)
//
// EVB : Nu-LB-NUC140
// MCU : NUC140VE3CN

// PA0,1,2,3,4,5 connected to 3x3 Keypad
// PC12,13,14,15 connected to LEDs (or Relays)

#include <stdio.h>
#include <stdbool.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "Scankey.h"
#include "LCD.h"
#include "Seven_Segment.h"



#define			PIN_GETAR		PA11
#define 		PIR					PA10
#define 		LED					PC12
#define			TOMBOL			PB15



int timer, count, input, i=0;
bool detek, cek_pir, cekdata ;
uint32_t input_key = 0;
uint32_t pass_key[6];
int num_key = 0;
int count_pass = 0;
uint32_t password[6] = {1,4,7,7,4,1};
bool cek_pass[6] = {0,0,0,0,0,0};
bool boolpir[3] = {0,0,0};
bool owner_house = false;
int count_pir = 0;
char charma[100];
bool pir;




void Init_GPIO(void)
{
	  GPIO_SetMode(PC, BIT12, GPIO_MODE_OUTPUT);
	  GPIO_SetMode(PC, BIT13, GPIO_MODE_OUTPUT);
	  GPIO_SetMode(PC, BIT14, GPIO_MODE_OUTPUT);
	  GPIO_SetMode(PC, BIT15, GPIO_MODE_OUTPUT);
	  PC12=1; PC13=1; PC14=1; PC15=1;
	
		GPIO_SetMode(PB, BIT11, GPIO_MODE_OUTPUT); ///BUZZER
		GPIO_SetMode(PA, BIT10, GPIO_MODE_INPUT); //PIN PIR PA10
		GPIO_SetMode(PA, BIT11, GPIO_MODE_INPUT); //PIN SENSOR GETAR = PA1
		GPIO_SetMode(PB, BIT15, GPIO_PMD_OUTPUT); //PIN BUAT TOMBOL
		PB11=1; //BUZZER OFF
}

void Init_PWM0(void)
{
	PWM_ConfigOutputChannel(PWM0, PWM_CH0, 50,  3); // 0.544ms = ~3% duty_cycle
	PWM_EnableOutput(PWM0, PWM_CH_0_MASK);
	PWM_Start(PWM0, PWM_CH_0_MASK);
}

void Display_7seg(uint16_t value)
{
  uint8_t digit;
	digit = value / 1000;
	CloseSevenSegment();
	ShowSevenSegment(3,digit);
	CLK_SysTickDelay(5000);
			
	value = value - digit * 1000;
	digit = value / 100;
	CloseSevenSegment();
	ShowSevenSegment(2,digit);
	CLK_SysTickDelay(5000);

	value = value - digit * 100;
	digit = value / 10;
	CloseSevenSegment();
	ShowSevenSegment(1,digit);
	CLK_SysTickDelay(5000);

	value = value - digit * 10;
	digit = value;
	CloseSevenSegment();
	ShowSevenSegment(0,digit);
	CLK_SysTickDelay(5000);
}


void Buzz(int number)
{
	int i;
	for (i=0; i<number; i++) {
      PB11=0; // PB11 = 0 to turn on Buzzer
	  CLK_SysTickDelay(100000);	 // Delay 
	  PB11=1; // PB11 = 1 to turn off Buzzer	
	  CLK_SysTickDelay(100000);	 // Delay 
	}
}



void check_pass(void)
{
	for(i = 0; i<6; i++)
	{
		if(pass_key[i] == password[i])
		{
				cek_pass[i] = true;
		}
	}
	if(cek_pass[0] == 1 && cek_pass[1] == 1 && cek_pass[2] == 1 && cek_pass[3] == 1 && cek_pass[4] == 1 && cek_pass[5] == 1)
	{
		detek = false;
		Buzz(5);
		owner_house = true;
		//Servo_Turn(14);
		
	}
	for(i = 0; i<6; i++)
	{
			cek_pass[i] = 0;
			pass_key[i] = 0;
	}	
}


void print_pass(int num)
{
	switch(num)
	{
		case 0 : print_Line(3, "                "); break;
		case 1 : print_Line(3, "     *          "); break;
		case 2 : print_Line(3, "     **         "); break;
		case 3 : print_Line(3, "     ***        "); break;
		case 4 : print_Line(3, "     ****       "); break;
		case 5 : print_Line(3, "     *****      "); break;
		case 6 : print_Line(3, "     ******     "); break;
	}
}

void inputPass(void)
{	
	input = ScanKey();
	if(input != 0)
	{
			input_key = 0;
			input_key = input;
			//Display_7seg(input_key);
			pass_key[num_key] = input_key;
			Buzz(1);
			num_key++;
			print_pass(num_key);
			if(num_key == 6) 
			{
				check_pass();
				Buzz(5);
				print_Line(3, "                ");
				num_key = 0;
				count_pass++;
				if(count_pass >=3)
				{
					detek = true;
				}
				//clear_LCD();
			}
		
		
		
			/*
			if(input_key != 0)
			{
				Buzz(1);
				//CLK_SysTickDelay(5000);
				num_key++;
			}
			*/
			
			input_key = 0;
	}
}


void waktu(void)
{
	timer++;
	if(timer > 300000) 
	//if(timer > 10000) 
	{
		timer = 0;
		Display_7seg(count);
		CLK_SysTickDelay(500);
		Buzz(1);
		count--;
	}			
}


void cek_getar(void)
{
	if(PIN_GETAR == 0)
	{
			detek = true;
	}
}


void Servo_Turn(float pos)
{
	PWM_ConfigOutputChannel(PWM0, PWM_CH0, 50, pos);
	CLK_SysTickDelay(200000); // latency for turning motor
}


void mainMenu(void)
{
	clear_LCD();
	print_Line(0, "MENU");
	while(1)
	{
		
	}
}





void cek_PIR(void)
{
	/*
	int buff = count%5;
	Display_7seg(count);
	if(buff == 0 && cek_pir == false) 
	{
		PC15 = 1;
		PC14 = 0;
		cek_pir = true;
	}
	else if(buff == 0 && cek_pir == true) 
	{
		Buzz(1);
		PC15 = 1;
		PC14 = 1;
		cek_pir = false;
		//count_pir = count_pir + 1;
		//Display_7seg(count_pir);
		//cekdata = true;
	}
	
	else PC15 = 0;
	*/
	
	/*
	if(cek_pir == true && cekdata == true)
	{
		
		if(PIR == 1)
		{
			boolpir[count_pir] = 1;
			cekdata = false;
		}
		else if(PIR == 0) boolpir[count_pir] = 0;
	}
	
	*/
	
	if(PIR == 0 && pir == false) cek_pir = true;
		
	if(cek_pir == true)
	{
		pir = true;
		if(PIR == 1 && cekdata == false) 
		{
			count_pir++;
			cekdata = true;
		}
		else if(cekdata == true && PIR == 0)
		{
			cek_pir = false;
		}
		
	}
	

}



int main(void)
{
	uint32_t i =0;
	
	
	SYS_Init();
	OpenKeyPad();
	Init_GPIO();
	init_LCD();
	Init_PWM0();
	OpenSevenSegment();
	


	mulai:
	
	clear_LCD();
	Servo_Turn(9);
	detek = false;
	owner_house = false;
	timer = 0;
	count = 60;
	Buzz(1);
	cek_pir = false;
	count_pir = 0;
	cekdata = false;
	pir = false;
	for(i=0; i<3; i++) boolpir[i] = 0;
	CLK_SysTickDelay(50000);
	

	
	
	//PROGRAM UTAMA
	while(1)
	{
		LED = false;
		count_pir = 0;
		if(PIR == 1)
		{
			//detek = true;
			Buzz(1);
			LED = true;
			print_Line(0, "     WELCOME    ");
			print_Line(2, " Input Password ");
			//clear_LCD();
			while(1)
			{
				waktu();
				cek_getar();
				inputPass();
				cek_PIR();

				if(count <= 0) detek = true;
				if(count_pir < 3 && count <=0) 
				{
					//PC13 = 0;
					detek = false;
					goto mulai;
					break;
				}
				
				if( detek == true || (detek == false && owner_house == true)) break;
			}
		}
		
		if(detek == true && owner_house == false)
		{
			PB11 = 0;
			while(1)
			{
				PB11 = 0;
				//Buzz(10);
				print_Line(0, "    	WELCOME    ");
				print_Line(2, "     MALING     ");
				inputPass();
				if(owner_house == true) 
				{
					print_Line(0, "      SUDAH     ");
					print_Line(1, "      AMAN      ");
					detek = false;
					LED		= TRUE; 				//MATIKAN LED
					CLK_SysTickDelay(20000000);
					clear_LCD();
					break;
				}
			}
			
		}
		
		else if(detek == false && owner_house == true)
		{
			Buzz(1);
			clear_LCD();
			print_Line(0, "    	WELCOME    ");
			print_Line(1, "       SIR      ");
			
			Servo_Turn(5);
			detek = false;
			owner_house = false;
			timer = 0;
			count = 19;
			CLK_SysTickDelay(5000);
			print_Line(3, "Press 5 for MENU");
			while(1)
			{
				waktu();
				input = ScanKey();
				if(input == 5)
				{
					mainMenu();
				}
				if(count<=0) break;
			}
			goto mulai;
		}
	}
}
