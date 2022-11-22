
#include <stdint.h>
#include "tm4c123gh6pm.h"


void ConfigUART0(void){
	
	//Se activa la señal de reloj del UART0
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART0;
	//Se activa la señal de reloj del GPIOA
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
	//Desactivamos el modulo UART0
	UART0_CTL_R &= ~UART_CTL_UARTEN;
	
	//CONFIGURACION DE PINES PA0 Y PA1 PARA EL RX Y TX
	
	//Se desactiva las funciones analogicas
	GPIO_PORTA_AMSEL_R &= ~(0x03);
	//Activamos funciones alternas en PA0 y PA1 para el uso de comunicacion serial
	GPIO_PORTA_AFSEL_R |= 0x03;
	//Se conecta el UART0 con los PA0 y PA1
	GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFFFFFF00)|0x00000011;
	//Se activan funciones digitales
	GPIO_PORTA_DEN_R |= 0x03;
	
	//CONFIGURACION DE VELOCIDAD
	
	//Se tiene una velocidad de 9600 bps y una frecuencia de 16Mz -> 16*10^6
	//BRD=(16*10^6)/((16/(2^0))*9600)
	//BRD=104.166
	//DVINT=104
	//DIVFRAC=(104.166*64) = 11 (aproximado al entero mas cercano)
	UART0_IBRD_R =(UART0_IBRD_R & 0xFFFF0000) | (104); 
	UART0_FBRD_R =(UART0_FBRD_R & 0xFFFFFFC0) | (11); 
	
	//CONFIGURACION DE TRAMA Y BUFFERS
	
	//8, N, 1
	//X1110X00
	//01110000 = 0x70
	UART0_LCRH_R = (UART0_LCRH_R & 0xFFFFFF00) | (0x70);
	
	//Se habilita el RX y TX
	//UART0_CTL_R |= UART_CTL_RXE | UART_CTL_TXE; (Ya esta por defecto)
	
	//FINALMENTE ACTIVAMOS EL UART
	
	UART0_CTL_R |= UART_CTL_UARTEN;
}
uint8_t rxUart(){
	uint8_t car;
	while ((UART0_FR_R & UART_FR_RXFE)!=0){}//mientras el fifo no este full
	car = UART0_DR_R & 0xFF;
	return car;
}
//Funcion que transmite un caracter por el UART0
void txUart(uint8_t car){
	
	while ((UART0_FR_R & UART_FR_TXFF)!=0){} //mientras el fifo este full
	UART0_DR_R = car & 0xFF;
}

void TxCadena( uint8_t Cadena[]){
	uint8_t i;
	for( i = 0; Cadena[i] != '\0'; i++ )
	txUart(Cadena[i]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ConfigUART2(void){
	
	//Se activa la señal de reloj del UART0
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART2;
	//Se activa la señal de reloj del GPIOA
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;
	//Desactivamos el modulo UART0
	UART2_CTL_R &= ~UART_CTL_UARTEN;
	
	//CONFIGURACION DE PINES PA0 Y PA1 PARA EL RX Y TX
	
	//desbloqueo del puerto PD7
	GPIO_PORTD_LOCK_R = 0x4C4F434B;
	GPIO_PORTD_CR_R |= (0x80);
	
	//Se desactiva las funciones analogicas
	GPIO_PORTD_AMSEL_R &= ~(0xC0);
	//Activamos funciones alternas en PD6 y PD7 para el uso de comunicacion serial
	GPIO_PORTD_AFSEL_R |= 0xC0;
	//Se conecta el UART0 con los PA0 y PA1
	GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R&0x00FFFFFF)|0x11000000;
	//Se activan funciones digitales
	GPIO_PORTD_DEN_R |= 0xC0;
	
	//CONFIGURACION DE VELOCIDAD
	GPIO_PORTD_LOCK_R = 0x4C4F434B;
	GPIO_PORTD_CR_R |= (0x80);
	//Se tiene una velocidad de 9600 bps y una frecuencia de 16Mz -> 16*10^6
	//BRD=(16*10^6)/((16/(2^0))*9600)
	//BRD=104.166 /8.6805
	//DVINT=104  / 8
	//DIVFRAC=(104.166*64) = 11 (aproximado al entero mas cercano)/ 556
	UART2_IBRD_R =(UART2_IBRD_R & 0xFFFF0000) | (104); 
	UART2_FBRD_R =(UART2_FBRD_R & 0xFFFFFFC0) | (11); 
	//CONFIGURACION DE TRAMA Y BUFFERS
	
	//8, N, 1
	//X1110X00
	//01110000 = 0x70
	UART2_LCRH_R = (UART2_LCRH_R & 0xFFFFFF00) | (0x70);
	
	//Se habilita el RX y TX
	UART2_CTL_R |= UART_CTL_RXE | UART_CTL_TXE; //(Ya esta por defecto)
	
	//FINALMENTE ACTIVAMOS EL UART
	
	UART2_CTL_R |= UART_CTL_UARTEN;
}

//Funcion que recibe un caracter por el UART2
uint8_t rxUart2(){
	uint8_t car;
	while ((UART2_FR_R & UART_FR_RXFE)!=0){}//mientras el fifo no este full
	car = UART2_DR_R & 0xFF;
	return car;
}
//Funcion que transmite un caracter por el UART2
void txUart2(uint8_t car){
	
	while ((UART2_FR_R & UART_FR_TXFF)!=0){} //mientras el fifo este full
  	UART2_DR_R = car & 0xFF;
}

void TxCadena2( uint8_t Cadena[]){
	uint8_t i;
	for( i = 0; Cadena[i] != '\0'; i++ )
	txUart2(Cadena[i]);
}
///////////////////////////////////////////////////////////////////////////////////
void main(void){
	ConfigUART0();
	ConfigUART2();
	uint8_t car;
	uint8_t centinela=0;
	uint8_t mensaje1[]="Bienvenido\n\r";
	uint8_t mensaje2[]="\n\rSeleccione modo\n\r";
	uint8_t mensaje3[]="\n\r1.Control Manual del Carro\n\r";
	uint8_t mensaje4[]="\n\r2.Modo Seguidor de Linea\n\r";
	uint8_t mensaje5[]="\n\r3.Mostrar valores del Acelerometro\n\r";
	uint8_t mensaje6[]="\n\rOpcion Invalida\n\r";
	uint8_t mensaje7[]="\n\rIngreso a Modo Control Manual del Carro\n\r";
	uint8_t mensaje8[]="\n\r'w' para avanzar\n\r";
	uint8_t mensaje9[]="\n\r's' para retroceder\n\r";
	uint8_t mensaje10[]="\n\r'd' giro a la derecha\n\r";
	uint8_t mensaje11[]="\n\r'a' giro a la izquierda\n\r";
	uint8_t mensaje12[]="\n\r'p' para parar\n\r";
	uint8_t mensaje13[]="\n\r'q' para salir de este modo\n\r";
	uint8_t mensaje14[]="\n\rIngreso a Modo Control Manual del Carro\n\r";
	uint8_t mensaje15[]="\n\rSalio del Modo Control Manual del Carro\n\r";
	uint8_t mensaje16[]="\n\rColoque el carro en la posicion que desee\n\r";
	uint8_t mensaje17[]="\n\rPresione 'g' para empezar\n\r";
	uint8_t mensaje18[]="\n\rSalio del Modo Seguidor de Linea\n\r";
	TxCadena(mensaje1);
	centinela=0;
	while(1){
		if(centinela==0){
			TxCadena(mensaje2);
			TxCadena(mensaje3);
			TxCadena(mensaje4);
			TxCadena(mensaje5);
		}
		car=rxUart();
		txUart(car);
		
		if(((car!='1') && (car!='2') && (car!='3'))){
			TxCadena(mensaje6);
			centinela=1;
		}

		if(car=='1'){
			txUart2(car);
			TxCadena(mensaje7);
			TxCadena(mensaje8);
			TxCadena(mensaje9);
			TxCadena(mensaje10);
			TxCadena(mensaje11);
			car=' ';
			//Con la letra q se saldra del proceso
			while(car!='q'){
				car=rxUart();
				txUart(car);
				txUart2(car);
				txUart('\n');
				txUart('\r');
			}
			TxCadena(mensaje15);
			centinela=0;
		}else{
			if(car=='2'){
				txUart2(car);
				//comenzara el modo seguidos de linea
				TxCadena(mensaje16);
				car=' ';
				while(car!='q'){
					car=rxUart();
					txUart(car);
					txUart2(car);
					txUart('\n');
					txUart('\r');
				}
				TxCadena(mensaje18);
				centinela=0;
				
			}else{
				if(car=='3'){
					//se motrara cada 1 segundo las aceleraciones en ambos ejes 
					
				}
			}
		}
	}
}