#include <stdio.h>
#include <wiringPiI2C.h>

#define HELP \
        "\n  \033[1mDS3231 Alarm by blog.everpi.net\n\n" \
        "\tUsage: ds3231 [hhmm]\n\n" \
        "\t\t [hhmm] Set hour and minutes\n" \
	"\t\t [hhmmM] Set only when minutes match, example, 1559M will alarm every 59 minutes of whatever hours\n\n" \


int main(int argc, char *argv[]){
	
	int fd = 0;
	int data = 0;	
	int bcd_alarm[2] = {0};
	char *alarm = argv[1];	

	if(argc < 2){
		printf("%s",HELP);
		return 0;
	}
	
	while(data!=2){

		bcd_alarm[data] = ((*alarm-'0')<<4)+(*(alarm+1)-'0');
		alarm+=2;
		data++;
	}	

	fd = wiringPiI2CSetup(0x68) ;
	
	if(argv[1][4] == 'M'){
		bcd_alarm[0] = (bcd_alarm[0] | (1<<7));
		printf("match\n");	
	}	

	printf("Hour:%x Min:%x\n",bcd_alarm[0],bcd_alarm[1]);
	wiringPiI2CWriteReg8(fd,0x0b,bcd_alarm[1]); // min
	wiringPiI2CWriteReg8(fd,0x0c,bcd_alarm[0]); // hr
	wiringPiI2CWriteReg8(fd,0x0d,1<<7);	
	wiringPiI2CWriteReg8(fd,0x0e,6);
	wiringPiI2CWriteReg8(fd,0x0f,1<<3);
	
	return 0;
}
