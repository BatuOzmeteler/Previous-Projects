module keyBoardCore(clk, rst, rx_done_tick, rx_data, sevenOut);
input clk, rst;
input rx_done_tick;
input [7:0] rx_data;
output reg [19:0] sevenOut;
reg [19:0] sevenOutNext;
wire [4:0] binary_data;
reg [1:0] state, stateNext;

reg [1:0] cnt, cntNext;
parameter COUNTERSIZE = 1;
key2Binary key2Binary(.key_code(rx_data), .binary(binary_data));

always@(posedge clk) begin
	sevenOut <= sevenOutNext;
	state <= stateNext;
	cnt <= cntNext;
end

always@(*) begin
	if(rst) begin
		sevenOutNext = 0;
		stateNext = 0;
		cntNext = 0;
	end else begin
		stateNext = state;
		sevenOutNext = sevenOut;
		cntNext = cnt;
			case(state)
				0:begin
					if(rx_done_tick)begin
						cntNext = cnt+1;
						if(cnt > COUNTERSIZE)begin
							sevenOutNext = {15'b100001000010000,binary_data};
							stateNext = state + 1;
							cntNext = 0;
						end
					end
				end
				1:begin
					if(rx_done_tick)begin
						cntNext = cnt+1;
						if(cnt > COUNTERSIZE)begin
							sevenOutNext = {10'b1000010000,binary_data,sevenOut[4:0]};
							stateNext = state + 1;
							cntNext = 0;
						end
					end
				end
				2:begin
					if(rx_done_tick)begin
						cntNext = cnt+1;
						if(cnt > COUNTERSIZE)begin
							sevenOutNext = {5'b10000,binary_data,sevenOut[9:0]};
							stateNext = state + 1;
							cntNext = 0;
						end
					end
				end
				3:begin
					if(rx_done_tick)begin
						cntNext = cnt+1;
						if(cnt > COUNTERSIZE)begin
							sevenOutNext = {binary_data,sevenOut[14:0]};
							stateNext = 0;
							cntNext = 0;
						end
					end
				end
			endcase
		end	
	end
endmodule
