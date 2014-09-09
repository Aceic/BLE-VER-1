FILES = +incdir+./
#INC = +incdir+/home/eda/questa_sim/verilog_src/ovm-2.1.1/src 
#INC = +incdir+/home/acer/eda/questa103an/questasim/verilog_src/ovm-2.1.2/src
INC = +incdir+/home/acer/eda/ovm-2.1.2/src
work = work
VSIMOPT= -novopt -sva -sv_seed random -l s.log work.top
lib:
	vlib $(work)
	vmap work $(work)
VSIMBATCH1 = -c -do "coverage save -onexit -assert -directive -cvg -codeAll mem_cov1;run -all;exit"

comp0:
	vlog -work $(work) $(INC) /home/acer/eda/ovm-2.1.2/src/ovm_pkg.sv ovm_container.sv $(FILES) tx_agent_pkg.sv ble_pkg.sv ppi_interface.sv ble_top.sv

run_test: lib comp0
	vsim  $(VSIMBATCH1) -coverage -novopt -sva -sv_seed random -l test1_sim.log work.ble_top +OVM_OBJECTION_TRACE +OVM_TESTNAME=ex_test
run_gui: lib comp0
	vsim  -coverage -novopt -sva -sv_seed random -l test1_sim.log work.ble_top +OVM_OBJECTION_TRACE +OVM_TESTNAME=ex_test

clean: 
	rm -rf work modelsim.ini vsim.wlf test1_sim.log mem_cov1 

