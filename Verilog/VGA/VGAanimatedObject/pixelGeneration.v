`timescale 1ns / 1ps

module pixelGeneration(push, clk, rst, pixel_x, pixel_y, video_on, rgb);

reg[9:0] addx, addy, addxNext, addyNext;
input clk, rst;
input [3:0] push;
input [9:0] pixel_x, pixel_y;
input video_on;
output reg [2:0] rgb;
wire square_on;

wire up, down, left, right;
reg up_reg, down_reg, left_reg, right_reg;

assign square_on = ((pixel_x >= addx) && (pixel_x <= addx + 40)) && ((pixel_y >= addy) && (pixel_y <= addy + 40));

debounce dbUp(.in(push[3]), .clk(clk), .out(up), .rst(rst));
debounce dbDown(.in(push[2]), .clk(clk), .out(down), .rst(rst));
debounce dbLeft(.in(push[1]), .clk(clk), .out(left), .rst(rst));
debounce dbRight(.in(push[0]), .clk(clk), .out(right), .rst(rst));

assign posBtnUp = push[3] & ~up_reg;
assign posBtnDown = push[2] & ~down_reg;
assign posBtnLeft = push[1] & ~left_reg;
assign posBtnRight = push[0] & ~right_reg;

always@(posedge clk)begin
	addx <=#1 addxNext;
	addy <=#1 addyNext;
	up_reg <=#1 up;
	down_reg <=#1 down;
	left_reg <=#1 left;
	right_reg <=#1 right;
end

always @(*) begin
	rgb = 3'b100;
	addxNext = addx;
	addyNext = addy;
	if(rst)begin
		rgb = 3'b000;
		addxNext = 320;
		addyNext = 220;
	end else begin 
		if(video_on) begin
			if(square_on)
				rgb = 3'b001;
				case(push[3:0])
					1:begin
						if(posBtnRight)
							addxNext = addx + 10;
					end
					2:begin
						if(posBtnLeft)
							addxNext = addx - 10;
					end
					4:begin
						if(posBtnDown)
							addyNext = addy + 10;
					end
					8:begin
						if(posBtnUp)
							addyNext = addy - 10;
					end	
				endcase
			end else
				rgb = 3'b010;
		end
	end
endmodule