module oneDigitUpDown(rst, clk, upDown, sevenSegment, anode);

    input rst,clk,upDown;
    
	 parameter COUNTERSIZE = 50000000;
	
    output wire[7:0] sevenSegment;
	 output reg [3:0] anode;
	 
	 reg [3:0] number, numberNext;	 
    reg [31:0] counter,counterNext;
	
	 SevenSegOneDig bb(.in(number),.sevenSeg(sevenSegment));
	
    always @ (posedge clk)begin
      counter <=#1 counterNext; 
	   number <=#1 numberNext;
    end

always@*begin
	 anode = 4'b0001;
	 numberNext = number;
		if(rst)begin
		numberNext = 0;
		counterNext = 0;
		end
		else begin
		counterNext = counter + 1;
	 
			if (counter > COUNTERSIZE) begin // this refers to one second
				if(upDown) begin
				numberNext = number + 1;
				end
				else begin
				numberNext = number - 1;
				end
			counterNext = 0;	
			end	
	 
	end
end
endmodule
