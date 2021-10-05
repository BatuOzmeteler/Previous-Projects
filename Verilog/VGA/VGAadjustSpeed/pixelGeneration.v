`timescale 1ns / 1ps

module pixelGeneration(push, clk, rst, pixel_x, pixel_y, video_on, rgb);

parameter COUNTERSIZEMAX = 50000000;
parameter COUNTERSIZEMIN = 250000;
reg[31:0] counterSize, counterSizeNext;
reg[31:0] counter, counterNext;
reg[2:0] state, stateNext;
reg direction, directionNext;

reg[9:0] addx, addy, addxNext, addyNext;

input clk, rst;
input [1:0] push;
input [9:0] pixel_x, pixel_y;
input video_on;
output reg [2:0] rgb;
wire square_on;

assign square_on = ((pixel_x > addx) && (pixel_x < addx + 40)) && ((pixel_y > addy) && (pixel_y < addy + 40));

wire increase, decrease;
reg increase_reg, decrease_reg;

debounce dbIncrease(.in(push[1]), .clk(clk), .out(increase), .rst(rst));
debounce dbDecrease(.in(push[0]), .clk(clk), .out(decrease), .rst(rst));

assign posBtnIncrease = increase & ~increase_reg;
assign posBtnDecrease = decrease & ~decrease_reg;

always@(posedge clk)begin
	increase_reg <=#1 increase;
	decrease_reg <=#1 decrease;
	counterSize <=#1 counterSizeNext;
	counter <=#1 counterNext;
	state <=#1 stateNext;
	addx <=#1 addxNext;
	addy <=#1 addyNext;
	direction <=#1 directionNext;
end

always @(*)begin
	
	if(rst)begin
		directionNext = 1;
		counterNext = 0;
		counterSizeNext = 10000000;
		stateNext = 0;
		addxNext = 320;
		addyNext = 220;
	end
	else begin
		directionNext = direction;
		addyNext = addy;
		addxNext = addx;
		counterNext = counter + 1;
		counterSizeNext = counterSize;
		stateNext = state;

			if(posBtnIncrease)begin
				if(counterSize > COUNTERSIZEMIN)
					counterSizeNext = counterSize - 250000;
			end
			if(posBtnDecrease)begin
				if(counterSize < COUNTERSIZEMAX)
					counterSizeNext = counterSize + 250000;
			end
			if(counter > counterSize)begin
				case(state)
					0:begin
						if(addx < 600)begin
							addxNext = addx + 1;
							counterNext = 0;
						end else begin
							directionNext = 1;
							stateNext = 1;
						end
					end
					1:begin
						if(direction)begin
							if(addy < 440 & addx > 0)begin
								directionNext = 1;
								addyNext = addy + 1;
								addxNext = addx - 1;
								counterNext = 0;
							end else if(addx == 0)begin
								directionNext = 0;
								stateNext = 3;
							end else if(addy == 440)begin
								directionNext = 0;
								stateNext = 2;
							end
						end else begin
							if(addy > 10 & addx > 0)begin
								directionNext = 0;
								addyNext = addy - 1;
								addxNext = addx - 1;
								counterNext = 0;
							end else if(addx == 0)begin
								directionNext = 1;
								stateNext = 3;
							end else if(addy == 10)begin
								directionNext = 1;
								stateNext = 4;
							end
						end
					end
					2:begin
						if(direction)begin
							if(addx < 600 & addy > 10)begin
								directionNext = 1;
								addyNext = addy - 1;
								addxNext = addx + 1;
								counterNext = 0;
							end else if(addx == 600)begin
								directionNext = 0;
								stateNext = 1;
							end else if(addy == 10)begin
								directionNext = 0;
								stateNext = 4;
							end
						end else begin
							if(addy > 10 & addx > 0)begin
								directionNext = 0;
								addyNext = addy - 1;
								addxNext = addx - 1;
								counterNext = 0;
							end else if(addx == 0)begin
								directionNext = 1;
								stateNext = 3;
							end else if(addy == 10)begin
								directionNext = 1;
								stateNext = 4;
							end
						end
					end
					3:begin
						if(direction)begin
							if(addx < 600 & addy > 10)begin
								directionNext = 1;
								addyNext = addy - 1;
								addxNext = addx + 1;
								counterNext = 0;
							end else if(addx == 600)begin
								directionNext = 0;
								stateNext = 1;
							end else if(addy == 10)begin
								directionNext = 0;
								stateNext = 4;
							end
						end else begin
							if(addy < 440 & addx < 600)begin
								directionNext = 0;
								addyNext = addy + 1;
								addxNext = addx + 1;
								counterNext = 0;
							end else if(addx == 600)begin
								directionNext = 1;
								stateNext = 1;
							end else if(addy == 440)begin
								directionNext = 1;
								stateNext = 2;
							end
						end
					end
					4:begin
						if(direction)begin
							if(addx > 0 & addy < 440)begin
								directionNext = 1;
								addyNext = addy + 1;
								addxNext = addx - 1;
								counterNext = 0;
							end else if(addx == 0)begin
								directionNext = 0;
								stateNext = 3;
							end else if(addy == 440)begin
								directionNext = 0;
								stateNext = 2;
							end
						end else begin
							if(addy < 440 & addx < 600)begin
								directionNext = 0;
								addyNext = addy + 1;
								addxNext = addx + 1;
								counterNext = 0;
							end else if(addx == 600)begin
								directionNext = 1;
								stateNext = 1;
							end else if(addy == 440)begin
								directionNext = 1;
								stateNext = 2;
						   end
						end
					end
				endcase
			end
	end
end


always @(*) begin 
	rgb = 3'b010;
	if(video_on) begin
		if(square_on) rgb = 3'b001;
	else rgb = 3'b100;
	end 				
end
endmodule
