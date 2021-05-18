#include <wiringPi.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <lcd.h>
#include <string.h>
const int RS = 3;
const int EN = 14;
const int D0 = 4;
const int D1 = 12;
const int D2 = 13;
const int D3 = 6;


void provera(double tmp,int td,int tg)
{
	if(tmp < td || tmp > tg)
		{
			
				digitalWrite(27,HIGH);
				delay(100);
				digitalWrite(27,LOW);
			
		}

};

double temperatura(void) //očitavanje temperature
{
FILE *ft;
char tekst[100];
ft=fopen("/sys/bus/w1/devices/28-00000a401e35/w1_slave","r");
if(ft==NULL) return 0;
int i=0;
for(i=0;i<22;i++) //samo temperatura
fscanf(ft,"%s", tekst);        
fclose(ft);
for(i=0;i<10;i++) tekst[i]=tekst[i+2]; 
int temp=atoi(tekst); 
double tem=(double)temp/1000; 
return tem;
};

void zapis(double temp) //log metod
{
FILE * log;
log=fopen("/home/pi/log.txt","a+");
if(log==NULL) return;
	char buff[20];
    struct tm *sTm;

    time_t now = time (0);
    sTm = gmtime (&now);

    strftime (buff, sizeof(buff), "%H:%M:%S", sTm);
  
  
fprintf(log, "%s %.3f\n",buff,temp);
fclose(log);
};


int main() 
{
int taster_pritisnut = 0;
int taster_pritisnut_1 = 0;
int r_value_d,r_value_g;
int td = 0;
int tg = 0;
if (wiringPiSetup() == -1)
exit(1);

FILE * log;
log=fopen("/home/pi/log.txt","w");
if(log==NULL) return 0;
fprintf(log, "Vreme Temperatura\n");
fclose(log);
printf("\n\nPočetna Temp = %.3f \xC2\xB0 C\n", temperatura());

int lcd_h;
lcd_h = lcdInit(2,16,4,RS,EN,D0,D1,D2,D3,D0,D1,D2,D3);
pinMode(21, INPUT);
pinMode(25, OUTPUT); 
pinMode(26, OUTPUT);
pinMode(22,INPUT); //KEY1
pinMode(27,OUTPUT);
digitalWrite(25, HIGH);
digitalWrite(26, HIGH);


//taster_pritisnut = 0;
//taster_pritisnut_1 = 0;

while (1) 
{
		r_value_d = digitalRead(21);  // procitaj key0
		r_value_g = digitalRead(22);  // procitaj key1
		digitalWrite(26,r_value_d);		
		digitalWrite(25,r_value_g);
		if(!r_value_d && !taster_pritisnut)
		{
			taster_pritisnut = 1;
			td += 1;
			lcdPosition(lcd_h,0,1);
			lcdPrintf(lcd_h,"Donji opseg: %d",td);
			delay(200);
			lcdClear(lcd_h);
			fflush(stdout);
		}
		if(r_value_d) taster_pritisnut = 0;

		if(!r_value_g && !taster_pritisnut_1)
		{
			taster_pritisnut_1 = 1;
			tg += 5;
			lcdPosition(lcd_h,0,1);
            lcdPrintf(lcd_h,"Gornji opseg:%d",tg);
			delay(200);
			lcdClear(lcd_h);
			fflush(stdout);			
		}
		if(r_value_g) taster_pritisnut_1 = 0;
		double tempera = temperatura();
		zapis(tempera);
		lcdPosition(lcd_h,0,0);
		lcdPrintf(lcd_h,"Temperatura:%.1f",tempera);
		//printf("Trenutna temperatura : %.1f\n",tempera);
		provera(tempera,td,tg);
	
}
return 0;
}
