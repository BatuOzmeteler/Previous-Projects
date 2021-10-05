module SevenSegFourDig (clk, rst, dataIn, sevenSegment, anode);

input clk,rst;
input [15:0] dataIn;

wire[7:0] sevenSeg1;
output wire[7:0] sevenSegment;

reg[3:0] number, numberNext;

output reg[3:0] anode;

reg[15:0] counter,counterNext;

SevenSegOneDig ssod(.in(number), .sevenSeg(sevenSeg1));
assign sevenSegment = sevenSeg1;

always @ (posedge clk)begin
	counter <=#1 counterNext;
	number <=#1 numberNext;	
end

always@*begin
		if(rst)begin
		anode = 4'b1111;
		counterNext = 0;
		numberNext = 0;
		end
		else begin
		counterNext = counter + 1;
		case(counter[15:14])
		
			0: begin
			anode = 4'b0111;
			numberNext = dataIn[3:0];
			end
			1: begin
			anode = 4'b1011;
			numberNext = dataIn[7:4];
			end
			2: begin
			anode = 4'b1101;
			numberNext = dataIn[11:8];
			end
			3: begin
			anode = 4'b1110;
			numberNext = dataIn[15:12];
			end
		
		endcase
		
		end
end
endmodule
