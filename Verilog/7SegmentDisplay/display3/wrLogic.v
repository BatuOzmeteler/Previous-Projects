`timescale 1ns / 1ps

module wrLogic(clk, rst, enter, sw, seg0wr, seg1wr, seg2wr, seg3wr);
input clk, rst, enter;
input [3:0] sw;
output reg [4:0] seg0wr, seg1wr, seg2wr, seg3wr;

reg [1:0] state, stateNext;
reg [4:0] seg0wrNext, seg1wrNext, seg2wrNext, seg3wrNext;
reg enter_r;

always @(posedge clk) begin
	state <= #1 stateNext;
	enter_r <= #1 enter;
	seg0wr <= #1 seg0wrNext;
	seg1wr <= #1 seg1wrNext;
	seg2wr <= #1 seg2wrNext;
	seg3wr <= #1 seg3wrNext;
end

assign posEnter = ~enter_r & enter;

always @(*) begin
	seg0wrNext = seg0wr;
	seg1wrNext = seg1wr;
	seg2wrNext = seg2wr;
	seg3wrNext = seg3wr;
	stateNext = state;
	if(rst)begin
		seg0wrNext = 0;
		seg1wrNext = 0;
		seg2wrNext = 0;
		seg3wrNext = 0;
		stateNext = 0;
	end else begin
		if(posEnter)begin
			case(state)
			0:begin
				seg0wrNext = sw;
			end
			1:begin
				seg1wrNext = sw;
			end
			2:begin
				seg2wrNext = sw;
			end
			3:begin
				seg3wrNext = sw;
			end
			endcase
		stateNext = state + 1;
		end
	end
end

endmodule
