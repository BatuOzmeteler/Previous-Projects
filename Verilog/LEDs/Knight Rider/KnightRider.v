module KnightRider(rst, clk, dataOut);

parameter COUNTERSIZE = 25000000;

input rst,clk;

output reg[7:0] dataOut;

reg [7:0] dataOutHold;

reg[31:0] counter, counterNext;

reg movingDirection, movingDirectionHold;

always@(posedge clk)begin

	movingDirectionHold <=#1 movingDirection;
	counter <=#1 counterNext;
	dataOutHold <=#1 dataOut;
	
end

always@*begin
	
	if(rst) begin
		dataOut = 8'b00000001;
		movingDirection = 0;
		counterNext = 0;
	end
	else begin
		dataOut = dataOutHold;
		movingDirection = movingDirectionHold;
		counterNext = counter + 1;
	
		if (counter > COUNTERSIZE) begin // this refers to one second
			// movingDirection 1 for right shift, 0 for left shift
			if(dataOut[7] == 1 || dataOut[0] == 1) movingDirection = !movingDirectionHold;
			if (movingDirection) dataOut = dataOut << 1;
			else dataOut = dataOut >> 1;
			counterNext = 0;
		end
	end
end

endmodule