module progLogic(clk, rst, switch, enter, dataWr, addrWr, wrEn);

input clk, rst, enter;
input [7:0] switch;

output reg [15:0] dataWr;
reg [15:0] dataWrNext;


output reg [7:0] addrWr;
reg [7:0] addrWrNext, addrWrReg, dataWrReg;

reg state, stateNext;
output reg wrEn;

reg enter_reg;
assign posBtn = ~enter_reg & enter;

always@(posedge clk) begin
	state <=#1 stateNext; 
	addrWrReg <=#1 addrWrNext;
	dataWrReg <=#1 dataWrNext;
	enter_reg <=#1 enter;
end

always@(*)begin
	addrWrNext = addrWrReg;
	addrWr = addrWrReg;
	stateNext = state;
	wrEn = 0;
	dataWr = dataWrReg;
	dataWrNext = dataWrReg;
	if(rst)begin
		stateNext = 0;
		addrWrNext = 0;
		wrEn = 0;
	end else begin
			case(state)
			0:begin
				if(posBtn)begin
					dataWrNext[7:0] = switch;
					stateNext = state + 1;
				end
			end
			1:begin
				if(posBtn)begin
					dataWr = {dataWrReg,switch};
					wrEn = 1;
					addrWrNext = addrWrReg + 1;
					stateNext = 0;
				end
			end
			endcase
	end
end
endmodule
