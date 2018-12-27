#include<reg51.h>

#define MID_NIGHT 0 					//12 A.M
#define MORNING 5					//5	 A.M
#define	EVENING	(18 + 6)				//18 = 6 P.M, IN BCD 18 IS (0001 1000) I.E = 24 
#define TRAFFIC_THRESOLD 0x0F				//TRAFFIC THRESHOLD LEVEL

#define SET 1
#define CLEAR 0


sbit adc_rd = P2^5;					// READ PIN CONNECTED TO ADC
sbit adc_wr = P2^6;					// WRITE PIN CONNECTED TO ADC
sbit intr = P2^7;					// ADC INTERRUPT PIN
sbit main_latch = P2^1;					// PIN CONNECTED TO MAIN LATCH TURNNING LIGHTS ON/OFF AS PER TIME
sbit volt_latch = P2^0;					// PIN CONNECTED TO VOLTAGE SWITCHING LATCH HIGH/LOW



char count,sec,min,hr;

void init()						//funtion for initialization of the systems
{	
	TMOD=0X01;					//timer 0 mode 1, 16 bits				
	TH0=15574>>8;					
	TL0=15574;
	IE=0x82;					//Interrupt Enable
	TR0=1;						//Timer0 Starts now
}

void mlatch_controller()				//Main Latch controlling function
{
	if(hr >= EVENING && hr < MORNING)
	{
			main_latch=SET;			//Lights ON 
	}
	else
	{
			main_latch=CLEAR;		//Lights OFF
	}	
}

void time_7seg_bcd()					//setting relation between sec,min,hr and converting hr:min:sec in BCD format
{
	if(sec==10|sec==26|sec==42|sec==58|sec==74)	//when seconds past = 10,20,30,40,50 
		sec+=6;
	else if(sec==90)														//when seconds past = 60
	{
		sec=0;
		min++;
	}
	
	if(min==10|min==26|min==42|min==58|min==74|min==90)	//when mins past = 10,20,30,40,50
		min+=6;
	else if(min==95)			//when mins past = 60
	{
		min=0;
		hr++;
		mlatch_controller();
	}
	
	if(hr==10|hr==26)			//when hr past is 10,20
		hr=hr+6;
	else if(hr==35)				//when hr past is 24
		hr=0;

}

void pulse()
{
	unsigned int x=65535;
	while(x--);
	
}

void timmer0_isr() interrupt 1			 //Timer0 Interrupt invoked after every 50000us ie 50ms
{															
		
		count++;										
		if(count%20==0)			//when Interrupt has been invoked 20 time i.e when 50ms*20 = 1000ms has been passed  
		{
			sec++;		
			count=0;
			time_7seg_bcd();	//calling fun to process and display time in bcd after each and every second
		}
/*Calculation 35566 - 50000 + 32 + 2 + 2 + 2 = 15574*/
	TH0=15574<<8;								 								TL0=15574;		
}

char adc_conv()					//ADC converter function
{
	unsigned char temp;
	adc_rd=1;
	adc_wr=0;
	pulse();
	adc_wr=1;				//converstion strats here
	while(intr==1);		
	adc_rd=0;				//value loaded in ADC_data lines
	temp=P1;				//storing u.c pin_data		
	
	return temp;				//returing the value	
}

void vlatch_controller(unsigned char dat)	//fun. to control the rellay
{								
		if(dat> TRAFFIC_THRESOLD)																
		{
			volt_latch=SET;		//glow street lights at high voltage
		}
		else 
		{
			volt_latch=CLEAR;	//glow street lights at low voltage
		}
}



void main()
{
	unsigned char adc_data;

	init();

	while(1)
	{
		P0=hr;
		P3=min;
		adc_data=adc_conv();
		if(main_latch==SET)				//if street lights are on, call voltage controller
			vlatch_controller(adc_data);
	}		
}
