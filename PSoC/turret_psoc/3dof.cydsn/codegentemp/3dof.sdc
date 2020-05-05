# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\Users\pajoh\projects\Turret\PSoC\turret_psoc\3dof.cydsn\3dof.cyprj
# Date: Tue, 05 May 2020 01:08:10 GMT
#set_units -time ns
create_clock -name {ADC_DelSig_1_Ext_CP_Clk(routed)} -period 13.888888888888888 -waveform {0 6.94444444444444} [list [get_pins {ClockBlock/dclk_0}]]
create_clock -name {Clock_2(routed)} -period 100000 -waveform {0 50000} [list [get_pins {ClockBlock/dclk_3}]]
create_clock -name {Clock_3(routed)} -period 1000000 -waveform {0 500000} [list [get_pins {ClockBlock/dclk_4}]]
create_clock -name {CyILO} -period 1000000 -waveform {0 500000} [list [get_pins {ClockBlock/ilo}] [get_pins {ClockBlock/clk_100k}] [get_pins {ClockBlock/clk_1k}] [get_pins {ClockBlock/clk_32k}]]
create_clock -name {CyIMO} -period 333.33333333333331 -waveform {0 166.666666666667} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyPLL_OUT} -period 13.888888888888888 -waveform {0 6.94444444444444} [list [get_pins {ClockBlock/pllout}]]
create_clock -name {CyMASTER_CLK} -period 13.888888888888888 -waveform {0 6.94444444444444} [list [get_pins {ClockBlock/clk_sync}]]
create_generated_clock -name {CyBUS_CLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2 3} [list [get_pins {ClockBlock/clk_bus_glb}]]
create_clock -name {ADC_DelSig_1_Ext_CP_Clk} -period 13.888888888888888 -waveform {0 6.94444444444444} [list [get_pins {ClockBlock/dclk_glb_0}]]
create_generated_clock -name {Clock_1} -source [get_pins {ClockBlock/clk_sync}] -edges {1 73 145} [list [get_pins {ClockBlock/dclk_glb_1}]]
create_generated_clock -name {UART_1_IntClock} -source [get_pins {ClockBlock/clk_sync}] -edges {1 79 157} [list [get_pins {ClockBlock/dclk_glb_2}]]
create_generated_clock -name {ADC_DelSig_1_theACLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 113 227} [list [get_pins {ClockBlock/aclk_glb_0}]]
create_generated_clock -name {Clock_2} -source [get_pins {ClockBlock/clk_sync}] -edges {1 7201 14401} [list [get_pins {ClockBlock/dclk_glb_3}]]
create_generated_clock -name {Clock_3} -source [get_pins {ClockBlock/clk_sync}] -edges {1 72001 144001} [list [get_pins {ClockBlock/dclk_glb_4}]]

set_false_path -from [get_pins {__ONE__/q}]

# Component constraints for C:\Users\pajoh\projects\Turret\PSoC\turret_psoc\3dof.cydsn\TopDesign\TopDesign.cysch
# Project: C:\Users\pajoh\projects\Turret\PSoC\turret_psoc\3dof.cydsn\3dof.cyprj
# Date: Tue, 05 May 2020 01:08:03 GMT
