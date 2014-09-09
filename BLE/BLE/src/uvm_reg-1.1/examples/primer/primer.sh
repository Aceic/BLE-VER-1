rm -rf work
vlib work
vlog primer_out.sv
vsim -do "run -all; q" -c -sv_lib ../../../lib/ovm_dpi tb_top test
