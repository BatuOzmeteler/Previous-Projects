`timescale 1ns / 1ps
module grocery_calculator(clk, rst, inp, add, out);
input clk, rst, add, sub;
input  [3:0] inp;
output reg [7:0] out;
wire addOut, subOut;

reg state, stateNext;
reg [3:0] num1, num1Next;

debouncer db1 (.clk(clk),.rst(rst),.noisy_in(add),.clean_out(addOut));
debouncer db2 (.clk(clk),.rst(rst),.noisy_in(sub),.clean_out(subOut));

always @(posedge clk) begin
	state         <= #1 stateNext;
	num1          <= #1 num1Next;
	out       	  <= #1 outNext;
end

always @(*) begin
	
	if(rst) begin
		stateNext    = 0;
		num1Next     = 0;
		outNext  	 = 0;
	end else begin
		stateNext    = state;
		num1Next     = num1;
		dataOutNext  = dataOut;
		case(state)
			0: begin
				if(addOut || subOut)begin
					stateNext   = 1;
					num1Next    = inp;
					outNext 	= inp;
				end
				
			end
			1: begin
				stateNext = 0;
				if(addOut)begin
					num1Next  = num1 + inp;
					outNext   = num1 + inp;
				end
				if(subOut)begin
					num1Next  = num1 - inp;
					outNext   = num1 - inp;
				end
			end
		endcase
	end
end
endmodule
