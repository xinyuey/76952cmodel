`timescale 1ns/1ps
module test_c;

import "DPI-C" function void BQ76952(
	input shortint unsigned VC1,
	input shortint unsigned VC2,
	input shortint unsigned VC3,
	input shortint unsigned VC4,
	input shortint unsigned VC5,
	input shortint unsigned VC6,
	input shortint unsigned VC7,
	input shortint unsigned VC8,
	input shortint unsigned VC9,
	input shortint unsigned VC10,
	input shortint unsigned VC11,
	input shortint unsigned VC12,
	input shortint unsigned VC13,
	input shortint unsigned VC14,
	input shortint unsigned VC15,
	input shortint unsigned VC16,
	input shortint current,
	input bit charger,
	input bit LD,
	input int BAT_Pin,
	input int PACK_Pin,
	input int LD_Pin,
	input shortint TS1,
	input shortint TS2,
	input shortint TS3,
	input bit RST_SHUT,
	output bit CHG_on,
	output bit DSG_on,
	output bit PCHG_on,
	output bit PDSG_on,
	inout bit Fuse,
	output bit Alert);

	shortint unsigned VC1;
	shortint unsigned VC2;
	shortint unsigned VC3;
	shortint unsigned VC4;
	shortint unsigned VC5;
	shortint unsigned VC6;
	shortint unsigned VC7;
	shortint unsigned VC8;
	shortint unsigned VC9;
	shortint unsigned VC10;
	shortint unsigned VC11;
	shortint unsigned VC12;
	shortint unsigned VC13;
	shortint unsigned VC14;
	shortint unsigned VC15;
	shortint unsigned VC16;
	shortint current;
	bit charger,LD;
	int BAT_Pin;
	int PACK_Pin;
	int LD_Pin;
	shortint TS1;
	shortint TS2;
	shortint TS3;
	bit RST_SHUT;
	bit CHG_on,DSG_on,PCHG_on,PDSG_on,Fuse,Alert;

	bit clk = 1 ;
	always #5 clk = ~clk;

	always@(posedge clk)begin
		$display($time);
		BQ76952(VC1,VC2,VC3,VC4,VC5,VC6,VC7,VC8,VC9,VC10,VC11,VC12,VC13,VC14,VC15,VC16,current,charger,LD,BAT_Pin,PACK_Pin,LD_Pin,TS1,TS2,TS3,RST_SHUT,CHG_on,DSG_on,PCHG_on,PDSG_on,Fuse,Alert);
	end

	initial begin
		VC1 = 16'd4000;
		VC2 = 16'd4000;
		VC3 = 16'd4000;
		VC4 = 16'd4000;
		VC5 = 16'd4000;
		VC6 = 16'd4000;
		VC7 = 16'd4000;
		VC8 = 16'd4000;
		VC9 = 16'd4000;
		VC10 = 16'd4000;
		VC11 = 16'd4000;
		VC12 = 16'd4000;
		VC13 = 16'd4000;
		VC14 = 16'd4000;
		VC15 = 16'd4000;
		VC16 = 16'd4000;
		current = 16'd9;
		charger = 1'b0;
		LD = 1'b1;
		BAT_Pin = 32'd64000;
		PACK_Pin = 32'd64000;
		LD_Pin = 32'd64000;
		TS1 = 16'd30;
		TS2 = 16'd1200;
		TS3 = 16'd30;
		RST_SHUT = 1'b0;
		#300 $stop;
	end


//COV CUV COVL 
	initial begin
	#5 
	VC1 = 16'd5500;
	#50
	VC1 = 16'd4000;
	VC2 = 16'd2500;
	#20
	VC2 = 16'd4000;
	end

//SOV SUV
	initial begin
	#5
	VC3 = 16'd6100;
	#50
	VC3 = 16'd4000;
	VC4 = 16'd1900;
	#20
	VC4 = 16'd4000;
	end
/*
//OCC SCD OCD1 OCD2 OCD3 SCDL OCDL
	initial begin
	#5 current = 16'd150;//OCC
	#55 current = 16'd10;
	#55 current = -16'd150;//SCD OCD1
	#55 current = -16'd250;//OCD2
	#55 current = -16'd850;//OCD3
	#55 current = 16'd10;
	end
*/
/*
//OTC UTC OTD UTD
	initial begin
	#5 
	TS1 = 16'd90;
	TS3 = 16'd90;
	#40
	TS1 = -16'd10;
	TS3 = 16'd30;
	#40
	TS1 = 16'd30;
	end
*/
/*
//2LVL
	initial begin
	#25 Fuse = 1'b1;
	end
*/
/*
//PCHG_on PTOS
	initial begin
	#100 VC2 = 16'd0;//PCHG_on=1,PTO_alert=1
	#20 current = 16'd51;//PTO_counter++ -> PTOS_error=1,PTO_counter=0
	#1500 VC2 = 16'd4000;
	end
*/
endmodule
