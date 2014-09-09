
#Makefile for OVM Testbench
#RTL = ../Interface/ahi_if.sv

OVC_HOME = ../BLE_OVC
VIP_HOME = ../VIP
CTRL_HOME = ../VIP/ble_controller


work = work


PKG = +incdir+../src/ovm-2.1.2/src/ +incdir+../src/uvm_reg-1.1/src +incdir+..tb/ ../src/ovm-2.1.2/src/ovm_pkg.sv ../src/uvm_reg-1.1/src/uvm_reg_pkg.sv ../tb/ovm_container.sv
INTERFACE = +incdir+../interface ../interface/ahb_if.sv ../interface/pp_intf.sv ../interface/interrupt_if.sv ../interface/channel_if.sv ../interface/channel_40.sv
RAL =   +incdir+../RAL  ../RAL/ble_reg_bank_pkg.sv # ../RAL/ble_ral_block.sv 
OVC = +incdir+$(OVC_HOME)/master_agent +incdir+$(OVC_HOME)/test +incdir+$(OVC_HOME)/tb   $(OVC_HOME)/test/ovc_pkg.sv
SLAVE = +incdir+$(VIP_HOME)/ahb_slave 
 INT  = +incdir+$(CTRL_HOME)/int_agent 
TX  =  +incdir+$(CTRL_HOME)/tx_agt 
RX  =  +incdir+$(CTRL_HOME)/rx_agt
CTRL =  +incdir+$(CTRL_HOME)  +incdir+$(CTRL_HOME)/tb  
VIP  = $(SLAVE) $(INT)  $(TX) $(RX)    $(CTRL) $(VIP_HOME)/ahb_slave/ahb_slv_pkg.sv $(CTRL_HOME)/tb/ctrl_pkg.sv  +incdir+$(VIP_HOME)/tb $(VIP_HOME)/tb/vip_pkg.sv #$(CTRL_HOME)/int_agent/intr_pkg.sv $(CTRL_HOME)/tx_agt/tx_agent_pkg.sv  $(CTRL_HOME)/rx_agt/rx_agent_pkg.sv 

MEM =  +incdir+../memory ../memory/vip_define.sv ../memory/define.sv  #../memory/ble_memory.sv
ENV =  +incdir+../tb  +incdir+../test ../test/ble_test_pkg.sv  ../tb/clk_agent.sv ../tb/top.sv 
  #../memory/ble_memory.sv
# PKG = ../src/ovm-2.1.2/src/ovm_pkg.sv
DPILIB_VSIM_OPT = -sv_lib ../src/uvm_reg-1.1/lib/uvm_dpi
DPILIB_TARGET = dpi_lib32

DPI =gcc -m32 -fPIC -DQUESTA -g -W -shared -x c -I../src/uvm_reg-1.1/include ../src/uvm_reg-1.1/src/dpi/uvm_dpi.cc -o ../src/uvm_reg-1.1/lib/uvm_dpi.so
REG = ../src/uvm_reg-1.1/src/uvm_reg_pkg.sv  ../RAL/ble_reg_bank_pkg.sv

VSIMOPT= -novopt -sva -sv_seed 1230096565 -l s.log work.top
VSIMBATCH = -c -do "run -all;exit"
VSIMBATCH0 = -c -do "sv_lib ../src/uvm_reg-1.1/lib/uvm_dpi; coverage save -onexit -assert -directive -cvg -codeAll mem_cov0;run -all;exit"


comp0:
	vlog -work $(work) $(PKG) $(RAL) $(INTERFACE) $(OVC) $(VIP) $(ENV)
	$(DPI)


html:
	firefox covhtmlreport/pages/__frametop.htm

lib: $(DPI_TARGET)
	vlib $(work)
	vmap work $(work)

sv_cmp: lib comp0
         
run_sim:
	vsim $(VSIMOPT) $(VSIMBATCH)    
	vcover report -html mem_cov1
run_test: lib comp0
	  vsim $(DPILIB_VSIM_OPT) $(VSIMBATCH0) -novopt -sva -sv_seed random -l test_sim.log work.top +OVM_OBJECTION_TRACE +OVM_TESTNAME=ahb_simple_test +OVM_VERBOSITY=OVM_HIGH 
		vcover report -html mem_cov0
run_test1: lib comp0
	  vsim $(DPILIB_VSIM_OPT) $(VSIMBATCH0) -novopt -sva -sv_seed 3528674878 -l test_sim.log work.top +OVM_OBJECTION_TRACE +OVM_TESTNAME=adv_scan_test +OVM_VERBOSITY=OVM_HIGH 
		vcover report -html mem_cov0

run_test2: lib comp0
	  vsim $(DPILIB_VSIM_OPT) $(VSIMBATCH0) -novopt -sva -sv_seed random -l test_sim.log work.top +OVM_OBJECTION_TRACE +OVM_TESTNAME=adv_conn_test +OVM_VERBOSITY=OVM_HIGH 
		vcover report -html mem_cov0

run_test3:lib comp0
	 vsim  $(VSIMBATCH1) -novopt -sva -sv_seed random -l test1_sim.log work.top +UVM_OBJECTION_TRACE +UVM_TESTNAME=half_duplex_test +UVM_VERBOSITY=UVM_HIGH 
	vcover report -html mem_cov1


gui:
	vsim  $(DPILIB_VSIM_OPT) $(VSIMOPT) -coverage +OVM_OBJECTION_TRACE +OVM_TESTNAME=ahb_simple_test +OVM_VERBOSITY=OVM_MEDIUM
	vcover report -html mem_cov1


run_gui:lib comp0 gui
run_gui1 :lib comp0 gui1


gui1:
	vsim $(DPILIB_VSIM_OPT) $(VSIMOPT) -coverage +OVM_OBJECTION_TRACE +OVM_TESTNAME=adv_scan_test +OVM_VERBOSITY=OVM_MEDIUM
	vcover report -html mem_cov1



clean:
	rm -rf modelsim.* transcript* *log* work vsim.wlf fcover* covhtml* mem_cov*
	clear


	
report:
	vcover merge mem_cover mem_cov0 mem_cov1 mem_cov2 mem_cov3 mem_cov4 mem_cov5 mem_cov6 mem_cov7 mem_cov8 mem_cov9 mem_cov10
	vcover report -html mem_cover

regress: clean run_test run_test1 run_test2 run_test3 run_test4 run_test5 run_test6 run_test7 run_test8 run_test9 run_test10 report html
