#include "NRF24L01/v1/NRF24L01.hpp"
#include "NRF24L01/v1/driver/MSP430.hpp"

#include "msp430lib/sys/v1/MSP430_System.hpp"

#include "msp430lib/usci/v1/MSP430_USCI_UART.hpp"
#include "msp430lib/usci/v1/MSP430_USCI_A0.hpp"

#include "msp430lib/utils/Output.hpp"

int main()
{
  // Disable Watchdog, Setup Clock Module
  WDTCTL  = WDTPW + WDTHOLD;  // Stop WDT
  BCSCTL1 = CALBC1_16MHZ;      // Set DCO
  DCOCTL  = CALDCO_16MHZ;

  // MSP430_System --> Clock functions
  msp430lib::MSP430_System sys;
  sys.init();

  typedef msp430lib::MSP430_USCI_UART<msp430lib::MSP430_USCI_A0> UART_HW;
  UART_HW uart;
  uart.init();

  // Output Stream
  msp430lib::Output<UART_HW> os(uart);
  os << "\n\n\n==========================\n";
  os << "Debug stream initialized\n";

  // enable interrupts
  __eint();

  // Initialize the NRF24L01 Device
  typedef NRF24L01::driver::MSP430 NRF_COMM;
  typedef NRF24L01::NRF24L01<NRF_COMM> NRF_HW;

  os << "Initializing NRF_COMM\n";
  NRF_COMM nrf_comm(sys);

  os << "Initializing NRF_HW\n";
  NRF_HW nrf(nrf_comm);
  nrf.init();

  os << "Open writing pipe\n";
  nrf.setPayloadSize(0, 0);
  nrf.openWritingPipe(0x0102030405);
  nrf.standBy();
  os << "Ready for operation\n";

  // Initialize the button
  P2DIR &= ~BIT3;
  P2REN |= BIT3;
  P2OUT |= BIT3;

  // MAIN
  os << "\n\n"
     << "=========================\n"
     << "NRF24L01+ Sender (MSP430)\n"
     << "=========================\n"
     << "\n\n";

  nrf.dumpStatus(os);
  os << "\n\n";

  os << "Press BTN_1 (P2.3) to send something!\n";
  while (1)
  {

    while ((P2IN & BIT3) != 0);

    nrf.write("Hello World\n", 12);
  }
}
