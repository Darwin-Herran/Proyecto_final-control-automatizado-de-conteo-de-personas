/*
/////////////////// DATOS DEL PROGRAMA ////////////////////
//  TTITULO: Contador
//  MICRO:PIC16F15244
//  ESTUDIANTES: Drwin Herran y Jeisson betancourt 
//  Profesor: Harold F MURCIA
//  FECHA: 25 de noviembre de 2021*/



#include <xc.h>

#include <stdint.h>

#include <stdbool.h>

#include <conio.h>

#include <stdlib.h>

#include <stdio.h>

#include "flex_lcd.h"


#pragma config FEXTOSC = OFF // External Oscillator Mode Selection bits->Oscillator not enabled
#pragma config RSTOSC = HFINTOSC_1MHZ // Power-up Default Value for COSC bits->HFINTOSC (1 MHz)
#pragma config CLKOUTEN = OFF // Clock Out Enable bit->CLKOUT function is disabled; I/O function on RA4
#pragma config VDDAR = HI // VDD Range Analog Calibration Selection bit->Internal analog systems are calibrated for operation between VDD = 2.3V - 5.5V

// CONFIG2
#pragma config MCLRE = EXTMCLR // Master Clear Enable bit->If LVP = 0, MCLR pin is MCLR; If LVP = 1, RA3 pin function is MCLR
#pragma config PWRTS = PWRT_OFF // Power-up Timer Selection bits->PWRT is disabled
#pragma config WDTE = OFF // WDT Operating Mode bits->WDT disabled; SEN is ignored
#pragma config BOREN = ON // Brown-out Reset Enable bits->Brown-out Reset Enabled, SBOREN bit is ignored
#pragma config BORV = LO // Brown-out Reset Voltage Selection bit->Brown-out Reset Voltage (VBOR) set to 1.9V
#pragma config PPS1WAY = ON // PPSLOCKED One-Way Set Enable bit->The PPSLOCKED bit can be cleared and set only once in software
#pragma config STVREN = ON // Stack Overflow/Underflow Reset Enable bit->Stack Overflow or Underflow will cause a reset

// CONFIG4
#pragma config BBSIZE = BB512 // Boot Block Size Selection bits->512 words boot block size
#pragma config BBEN = OFF // Boot Block Enable bit->Boot Block is disabled
#pragma config SAFEN = OFF // SAF Enable bit->SAF is disabled
#pragma config WRTAPP = OFF // Application Block Write Protection bit->Application Block is not write-protected
#pragma config WRTB = OFF // Boot Block Write Protection bit->Boot Block is not write-protected
#pragma config WRTC = OFF // Configuration Registers Write Protection bit->Configuration Registers are not write-protected
#pragma config WRTSAF = OFF // Storage Area Flash (SAF) Write Protection bit->SAF is not write-protected
#pragma config LVP = ON // Low Voltage Programming Enable bit->Low Voltage programming enabled. MCLR/Vpp pin function is MCLR. MCLRE Configuration bit is ignored.

// CONFIG5
#pragma config CP = OFF // User Program Flash Memory Code Protection bit->User Program Flash Memory code protection is disabled

///////////// DEFINICIONES  //////////////////
#define _XTAL_FREQ 1000000
#define S1  PORTBbits.RB5//Sensor 1
#define S2  PORTBbits.RB7 //Sensor 2
#define SW1 PORTAbits.RA5 // ASCENDER 
#define SW2 PORTAbits.RA4// DESCENDER
#define VERDE        LATCbits.LATC3  //LED VERDE 
#define ROJO         LATAbits.LATA1   // LED ROJO
#define AMARILLO     LATCbits.LATC0 //LED AMARILLO 
#define SW3     PORTCbits.RC2    // SUMAR PARA ELEGIR NUMERO

#define ACQ_US_DELAY 10

void PIN_MANAGER_Initialize(void) 
{
  // LATx registers
  LATA = 0x00;
  LATB = 0x00;
  LATC = 0x00;

  // TRISx registers
  TRISA = 0b00110000;
  TRISB = 0b10100000;
  TRISC = 0b00000100;

  // ANSELx registers
  ANSELC = 0;
  ANSELB = 0;
  ANSELA = 0;

  
 // ADCON0bits.CHS=5;
  //ADCON0bits.CHS=7;
  // WPUx registers/
 
  WPUC = 0x04;

  // ODx registers
  ODCONA = 0x04;
  ODCONB = 0x00;
  ODCONC = 0x00;

  // SLRCONx registers
  SLRCONA = 0x37;
  SLRCONB = 0xFF;
  SLRCONC = 0xFF;

  // INLVLx registers
  INLVLA = 0x3F;
  INLVLB = 0xFF;
  INLVLC = 0xFF;

}

uint8_t contador = 0;
uint8_t  MAX = 50;
char s[20];


void menu(void) {
 
  uint24_t variable = 0;
  bool operacion = false;
  int ok = 0;
  while (1) {
    
   //Modo seleccion de operacion
    if (SW3 == 0) {                // Presiono y espero a que deje de presionar
      __delay_ms(1000); 
      if (SW3!= 0) { 
        operacion = !operacion;   //cambio la operacion
        
        if (operacion) {          //Segun estado de operacion mostrar en LCD si es suma o resta
          Lcd_Cmd(LCD_CLEAR);
          sprintf(s, "AJUSTE ");
          Lcd_Out2(1, 0, s);
          sprintf(s, "LIMITE PERSONAS");
          Lcd_Out2(2, 0, s);
        } 
        else {
          __delay_ms(500);
        }
      }
    }
    //Modo personas
    
    while (SW3 == 0) {             //Mientras presiono boton
      variable++;
      if (variable == 34723) {    //Durante 5 segundos y activo configuracion
        variable = 0;
        Lcd_Cmd(LCD_CLEAR);       //Mostrar en lcd que debe cambiar el aforo maximo
        sprintf(s, "ELIJA");
        Lcd_Out2(1, 0, s);
        sprintf(s, "LIMITE PERSONAS");
        Lcd_Out2(2, 0, s);
       __delay_ms(2000);          //Delay para alcanzar a dejar de presionar el boton
        
        while (1) {
          
          if (SW3 == 0) {              // Presiono y espero a que deje de presionar
            __delay_ms(1000); 
            if (SW3 != 0) { 
              if (operacion == true) { //operacion(true)=suma
                  if(MAX==255){     //Si llego al numero maximo no se le puede sumar más
                      MAX=255;
                  }
                  else{
                      MAX=MAX+10; //Aumentar maximo de a 10
                  }
                
                Lcd_Cmd(LCD_CLEAR);
                sprintf(s, "Configuracion");
                Lcd_Out2(1, 0, s);
                sprintf(s, "Maximo: %d", MAX);
                Lcd_Out2(2, 0, s);
                
              } else {                  //operacion(false)=resta
                if (MAX == 0) {
                  MAX = 0;           //Si es el minimo no se le puede restar mas
                } else {
                  MAX = MAX - 10; //Restar maximo de a 10
                  
                }
                if(MAX==0){
                    MAX=50;
                }
                Lcd_Cmd(LCD_CLEAR);
                 sprintf(s, "Configuracion");
                Lcd_Out2(1, 0, s);
                sprintf(s, "Maximo: %d", MAX);
                Lcd_Out2(2, 0, s);
              }

            }
          }
          while (SW3 == 0) {          //Presiono boton y espero 5segundos consecutivos
            variable++;
            if (variable == 44643) {
             variable = 0;
             ok = 1;                 //Variable para salir de los loop
             Lcd_Cmd(LCD_CLEAR);
             break;
            }
          }
          variable=0;
          if (ok == 1) {             //Sale de while
            ok = 2;
            break;
          }
        }
      }
      if (ok == 2) {                 //Sale de while
        ok = 3;
        break;
      }
    }
    variable = 0;
    if (ok == 3) {                   //Sale de while
      ok = 4;
      Lcd_Cmd(LCD_CLEAR);
      sprintf(s, "Modo contador ");  //Vuelve al modo contador
      Lcd_Out2(1, 0, s);
      LATAbits.LATA2 = 1;           //Apaga led porque se cambio el maximo
      __delay_ms(1000);
      break;
     }

  }

}
 
char CONT[20];
int contado=0;
uint24_t  S;
uint24_t selec=0;
const  int LDR=950; 
bool S_1=0;
bool S_2=0;
int SU=0;

// PRINCIPAL 
void main(void)
{
    PIN_MANAGER_Initialize();
    
    LATAbits.LATA1=0;
   
  
    Lcd_Init();
    Lcd_Cmd(LCD_CLEAR);
    Lcd_Cmd(LCD_CURSOR_OFF);
    
    __delay_ms(600);
      Lcd_Out(2,2,"BuenDia");
      __delay_ms(500);
    while(1){
    
      //AUMENTAR  LASER
      if(S1==0){
         contado++;
        }
      sprintf(CONT, "NumPersonas %0d",contado);
        Lcd_Out2(1,2,CONT);
        sprintf(CONT, "LIMITE %d",MAX);
       Lcd_Out2(2,1,CONT);
        __delay_ms(100);
        
        
        //BAJAR CONTEO 
        
     if(S2==0){
         contado--;
        }
      sprintf(CONT, "NumPersonas %0d",contado);
        Lcd_Out2(1,2,CONT);
        sprintf(CONT, "LIMITE %d",MAX);
       Lcd_Out2(2,1,CONT);
        __delay_ms(100);
    
        
        
        if(PORTCbits.RC2==0){
            __delay_ms(500);
            if(PORTCbits.RC2!=0){
                sprintf(CONT,"CONTAR",contado);
                Lcd_Out2(1,0,CONT);
            }
        }
    
    while(PORTCbits.RC2==0){
        selec++;
        if(selec==3000){
            selec=0;
            Lcd_Cmd(LCD_CLEAR);
            sprintf(CONT,"AJUSTAR");
            Lcd_Out2(1, 0, CONT);
            menu();
            
        }    
    }
    selec=0;
   
    // ASCENDER Y DESCENDER CON LOS BOTONES 
   /**/
    if(SW2==0){
    __delay_ms(20);
    }
    if(SW2==1){
        contado++;
         sprintf(CONT, "NumPersonas %0d",contado);
        Lcd_Out2(1,2,CONT);
        sprintf(CONT, "LIMITE %0d",MAX);
       Lcd_Out2(2,1,CONT);
        __delay_ms(500);
        
    }
    if(SW1==1){
        contado--;
    }  
    sprintf(CONT, "NumPersonas %0d",contado);
        Lcd_Out2(1,2,CONT);
        sprintf(CONT, "LIMITE %0d",MAX);
        Lcd_Out2(2,1,CONT);
        __delay_ms(500);
        
        
    // LEDS     
         if(contado> MAX){
     LATCbits.LATC0=1;}
        else{                
             LATCbits.LATC0=0;
        }
        if(contado < MAX){
     LATAbits.LATA1=1;}
        else{                
             LATAbits.LATA1=0;
        }
        if(contado== MAX){
     LATCbits.LATC1=1;}
        else{                
        LATCbits.LATC1=0;   
        }
        
     
       /*
if(S1<LDR &&S2>LDR && S_1==0 && S_2==0)
            S_1=0;
            if(S1>LDR && S2<LDR && S_1==1 && S_2==0)
            {
                while(S2<LDR)
                {}
            
                SU++;
                 S_1=1;
                 S_2=1;
            }
         sprintf(CONT,"NumPersonas %d",SU);
        Lcd_Out2(1,2    ,CONT);
            sprintf(CONT, "LIMITE %d",MAX);
        Lcd_Out2(2,1,CONT);
            __delay_ms(500);
         
        //DISMINUIR
            if(S1>LDR && S2<LDR && S_1==0 && S_2==0 )
            S_2=0; 
        if(S1<LDR && S2>LDR  && S_1==0 && S_2==1){
            while(S1<LDR)
            {}
            SU--;           
         
            }
           sprintf(CONT,"NumPersonas %d",SU);
            Lcd_Out2(1,2  ,CONT);
              sprintf(CONT, "LIMITE %d",MAX);
        Lcd_Out2(2,1,CONT); 
            __delay_ms(500);
    */
         }
}
