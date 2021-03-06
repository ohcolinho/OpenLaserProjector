# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\Users\Colin\Dropbox\Grad School\Spring 2014\ME 235\Code\USB_UART01\Final_PSoC_Code\USB_UART01-000.Bundle01\USB_UART01.cydsn\USB_UART01.cyprj
# Date: Thu, 15 May 2014 19:21:48 GMT
#set_units -time ns
create_clock -name {CyIMO} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyPLL_OUT} -period 15.625 -waveform {0 7.8125} [list [get_pins {ClockBlock/pllout}]]
create_clock -name {CyILO} -period 10000 -waveform {0 5000} [list [get_pins {ClockBlock/ilo}] [get_pins {ClockBlock/clk_100k}] [get_pins {ClockBlock/clk_1k}] [get_pins {ClockBlock/clk_32k}]]
create_clock -name {CyMASTER_CLK} -period 15.625 -waveform {0 7.8125} [list [get_pins {ClockBlock/clk_sync}]]
create_generated_clock -name {Clock_1} -source [get_pins {ClockBlock/clk_sync}] -edges {1 41 81} [list [get_pins {ClockBlock/dclk_glb_0}]]
create_generated_clock -name {CyBUS_CLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2 3} [list [get_pins {ClockBlock/clk_bus_glb}]]
create_clock -name {CyBUS_CLK(fixed-function)} -period 15.625 -waveform {0 7.8125} [get_pins {ClockBlock/clk_bus_glb_ff}]

set_false_path -from [get_pins {__ONE__/q}]

# Component constraints for C:\Users\Colin\Dropbox\Grad School\Spring 2014\ME 235\Code\USB_UART01\Final_PSoC_Code\USB_UART01-000.Bundle01\USB_UART01.cydsn\TopDesign\TopDesign.cysch
# Project: C:\Users\Colin\Dropbox\Grad School\Spring 2014\ME 235\Code\USB_UART01\Final_PSoC_Code\USB_UART01-000.Bundle01\USB_UART01.cydsn\USB_UART01.cyprj
# Date: Thu, 15 May 2014 19:21:41 GMT
