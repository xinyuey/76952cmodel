vlib work
vmap work work

vlog ./src/*.c test_c.sv
vsim test_c

add wave test_c/*
run -all
