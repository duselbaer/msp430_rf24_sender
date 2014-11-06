#include "msp430lib/usci/v1/MSP430_USCI_UART.hpp"
#include "msp430lib/usci/v1/MSP430_USCI_A0.hpp"

#include "msp430lib/utils/Output.hpp"

int main()
{
  // Disable Watchdog, Setup Clock Module
  WDTCTL  = WDTPW + WDTHOLD;  // Stop WDT
  BCSCTL1 = CALBC1_16MHZ;      // Set DCO
  DCOCTL  = CALDCO_16MHZ;

  typedef msp430lib::MSP430_USCI_UART<msp430lib::MSP430_USCI_A0> UART_HW;
  UART_HW uart;
  uart.init();

  // Output Stream
  msp430lib::Output<UART_HW> os(uart);

  // Initialize the NRF24L01 Device

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

  while (1)
  {
    os << "Press BTN_1 (P2.3) to send something!\n";

    while ((P2IN & BIT3) != 0);
  }
}
