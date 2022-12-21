#include "F28x_Project.h"
 void InitADC(void);
 void InitTimer0(void);
 void PIE(void);
 void GPIO(void);
interrupt void ISR(void);
 int a;
 int main(void)
 {
  EALLOW;
  DINT;
  InitADC();
  PIE();
  GPIO();
  InitTimer0();
  IER=0xffff;
  EDIS;
  EINT;

 while(1)
 {
 }
 return 0;
 }
 void InitADC(void)
 {
 AdcaRegs.ADCINTSEL1N2.bit.INT1E=0; //Disable the interrupt
 CpuSysRegs.PCLKCR13.bit.ADC_A=1; //enable the clock
 AdcaRegs.ADCCTL2.bit.PRESCALE=4; // select the pre scale value
 AdcaRegs.ADCCTL2.bit.RESOLUTION=0; //resolution
 AdcaRegs.ADCCTL2.bit.SIGNALMODE=0;
 AdcaRegs.ADCCTL1.bit.INTPULSEPOS=1;//interrupt pulse position
 AdcaRegs.ADCSOC0CTL.bit.TRIGSEL=01;// initialize the soc 05h ADCTRIG5 - ePWM1, ADCSOCAtrigger select
 AdcaRegs.ADCSOC0CTL.bit.CHSEL=2;//2h ADCIN2 channelselect
 AdcaRegs.ADCSOC0CTL.bit.ACQPS=14;//acquisition prescaler
 AdcaRegs.ADCINTSEL1N2.bit.INT1E=1;//Enable intterupt
 AdcaRegs.ADCCTL1.bit.ADCPWDNZ=1;//power down
 }

 void InitTimer0(void)
 {
 CpuTimer0Regs.TCR.bit.TSS=1; // stop timer 0
 CpuTimer0Regs.TCR.bit.TIF=1;//clear flag
 CpuTimer0Regs.PRD.all=100;//period=10kHz
 CpuTimer0Regs.TPR.bit.TDDR=120;//prescalar =120
  CpuTimer0Regs.TCR.bit.FREE=1;//run free
 CpuTimer0Regs.TCR.bit.TRB=1;//reload
 CpuTimer0Regs.TCR.bit.TIE=1;//enable interrupt
 CpuTimer0Regs.TCR.bit.TSS=0; //start the timer 0
 }

 void GPIO(void)
 {
 GpioCtrlRegs.GPAPUD.bit.GPIO31=1;//Pull up is disabled
 GpioCtrlRegs.GPADIR.bit.GPIO31=1;//output->GPIO
 GpioCtrlRegs.GPAINV.bit.GPIO31=0;//output is not inverted
 }


 void PIE(void)
 {

 PieCtrlRegs.PIECTRL.bit.ENPIE=1;//enable interrupt
 PieVectTable.TIMER0_INT = &ISR;//address for interrupt
  PieCtrlRegs.PIEIER1.bit.INTx7=1;//timer 0 interrupt is enabled
  PieCtrlRegs.PIEACK.bit.ACK1=1;//group 1 for timer 0
  }

 interrupt void ISR(void)
  {
     CpuTimer0Regs.TCR.bit.TIF=1;//clear flag
     a=AdcaResultRegs.ADCRESULT0;
     PieCtrlRegs.PIEACK.bit.ACK1=1;//group 1 for timer 0
  }
