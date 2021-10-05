`timescale 1ns / 1ps

module debounce(in,clk,out,rst
    );
input in,clk,rst;
output reg out;
reg in_r,infirst;
reg [12:0] cnt;
wire triger;

always @(posedge clk) begin
	if(rst)begin
		in_r <= 0;
		cnt <= 0;
	end
	else begin
	   in_r<=in;
		if(triger | cnt!=0)
			cnt<=cnt + 1;
		if(triger & cnt==0)
			infirst <= in;
	end
end

always @(*)begin
	if(cnt!=0)
		out = infirst;
	else
		out = in;

end

assign triger = in_r^in;

endmodule
