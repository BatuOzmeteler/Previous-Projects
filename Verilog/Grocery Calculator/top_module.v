`timescale 1ns / 1ps

module top_module                  (clk, rst, 
                                    add, 
									inp,
									LED7, LED6, LED5, LED4, LED3, LED2, LED1, LED0
							        );
	
	input clk, rst, add;	
	input [3:0] inp;
	
	output LED7, LED6, LED5, LED4, LED3, LED2, LED1, LED0;

	wire divClk;
	wire addout;
	
	clk_divider clock_scaler (.clk_in(clk), .rst(rst), .divided_clk(divClk));
	
	debouncer 	input_filter (.clk(divClk), .rst(rst), .noisy_in(add), .clean_out(addout));
	
	grocery_calculator calc          (.clk(divClk), .rst(rst), 
	                                  .add(addout),
									  .inp(inp), 
							          .LED7(LED7), .LED6(LED6), .LED5(LED5), .LED4(LED4), .LED3(LED3), .LED2(LED2), .LED1(LED1), .LED0(LED0)
							         );
	
endmodule
