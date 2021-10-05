`timescale 1ns / 1ps

module wrLogic(clk, rst, enter, sw, seg0wr, seg1wr, seg2wr, seg3wr);
input clk, rst, enter;
input [3:0] sw;
output reg [4:0] seg0wr, seg1wr, seg2wr, seg3wr;

reg [1:0] state, stateNext;
reg [4:0] seg0wrNext, seg1wrNext, seg2wrNext, seg3wrNext;
reg [3:0] sw1, sw2, sw3, sw1Next, sw2Next, sw3Next;
reg enter_r;

always @(posedge clk) begin
	state <= #1 stateNext;
	enter_r <= #1 enter;
	seg0wr <= #1 seg0wrNext;
	seg1wr <= #1 seg1wrNext;
	seg2wr <= #1 seg2wrNext;
	seg3wr <= #1 seg3wrNext;
	sw1 <= #1 sw1Next;
	sw2 <= #1 sw2Next;
	sw3 <= #1 sw3Next;
end

assign posEnter = ~enter_r & enter;

always @(*) begin
	seg0wrNext = seg0wr;
	seg1wrNext = seg1wr;
	seg2wrNext = seg2wr;
	seg3wrNext = seg3wr;
	sw1Next = sw1;
	sw2Next = sw2;
	sw3Next = sw3;
	stateNext = state;
	if(rst)begin
		seg0wrNext = 16;
		seg1wrNext = 16;
		seg2wrNext = 16;
		seg3wrNext = 16;
		sw1Next = 0;
		sw2Next = 0;
		sw3Next = 0;
		stateNext = 0;
	end else begin
		if(posEnter)begin
			case(state)
			0:begin
				if(sw == 0) begin
					sw1Next = sw;
					stateNext = state + 1;
				end else begin
					stateNext = 0;
				end	
			end
			1:begin
				if(sw == 4) begin
					sw2Next = sw;
					stateNext = state + 1;
				end else begin
					stateNext = 0;
				end	
			end
			2:begin
				if(sw == 8)begin
					sw3Next = sw;
					stateNext = state + 1;
				end else begin
					stateNext = 0;
				end
			end
			3:begin
				if(sw == 5)begin
					seg3wrNext = sw1;
					seg2wrNext = sw2;
					seg1wrNext = sw3;
					seg0wrNext = sw;
				end else begin
					seg0wrNext = 15;
					seg1wrNext = 15;
					seg2wrNext = 15;
					seg3wrNext = 15;
				end
				stateNext = 0;
			end
			endcase
		end
	end
end
endmodule
