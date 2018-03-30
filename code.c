#include<reg51.h>
sbit rd = P2^5; 
sbit wr = P2^6;
sbit intr = P2^7;
sbit l = P2^0;
sbit s = P2^1;
void delay()
{
 TR0=1;
 {
 while(TF0==0);
 TF0=0;
 }
 TR0=0;

 }

void delay1()
{
unsigned int x;
for(x=0;x<6530;x++);
}

void adc()
{
  rd=1;
	wr=0;
	delay1();
	wr=1;
	while(intr==1);
	rd=0;
  }

void main()
{
unsigned int a=0,b=0,c=0,i,j,k;
	P0=0;P3=0;
  TMOD=0X01;
  TL0=0;
  TH0=0;
while(1)
{


for(i=0;i<24;i++)
    {
	    for(j=0;j<60;j++)
	    {
	    	for(k=0;k<60;k++)
	    	{
	    	if(a==9|a==25|a==41|a==57|a==73|a==89)a=a+6;
	    	delay();
	    	a++;	
	    	if(a==96)a=0;
      	if(c>=0&c<5)
	    	{	s=1;
	    		adc();
	    		if(P1<0xff)
	    		{l=0; }
	    		else
	    		{l=1;}
	    		}
			else if(c>18|c<6) {s=1;l=1;}
		  else s=0;
		   }
	  	if(b==9|b==25|b==41|b==57|b==73|b==89)b=b+6;
			
	    	b++;
	    	if(b==96)b=0;
	    	P3=b;
		    }
		
		if(c==9|b==25|b==41)c=c+6;
		
		c++;
		if(c==36)c=0;
		P0=c;
		}	 		   	


}							 
}
