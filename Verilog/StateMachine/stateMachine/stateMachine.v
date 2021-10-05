module stateMachine(rst, clk, btn, ledOut);

input rst,clk,btn;
output reg[2:0] ledOut;
reg [2:0] ledOutNext;
reg btndeb_reg;
debounce bb (.in(btn),.clk(clk),.out(btndebounced),.rst(rst));

assign posBtn = ~btndeb_reg & btndebounced;  

always@(posedge clk)begin
	ledOut <=#1 ledOutNext;
	btndeb_reg <=#1 btndebounced;
end

always@*begin
	if(rst) begin
		ledOutNext = 3'b001;
	end
	else begin
		if(posBtn)begin
			if(ledOut != 3'b100)begin
			ledOutNext = ledOut + 1;
			end
			else begin
			ledOutNext = 3'b001;
			end
		end
		else begin
			ledOutNext = ledOut;
		end
	end
end
endmodule
