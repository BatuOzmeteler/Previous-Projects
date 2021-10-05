module mouse
(
	input wire clk, rst,
	inout wire ps2d, ps2c,
	output reg [9:0] mouse_x, mouse_y,
	output wire [2:0] btnm
);

localparam STRM=8'hf4; 
localparam [2: 0]
	init1 = 3'b000,
	init2 = 3'b001,
	init3 = 3'b010,
	pack1 = 3'b011,
	pack2 = 3'b100,
	pack3 = 3'b101,
	done = 3'b110 ;
	
reg [2:0] state_reg , state_next;
wire [7:0] rx_data;
reg wr_ps2;
reg m_done_tick;
wire rx_done_tick , tx_done_tick;
wire [8:0] xm, ym;
reg [8:0] x_reg , y_reg , x_next , y_next;
reg [2:0] btn_reg , btn_next ;
reg [9:0] mouse_xNext, mouse_yNext;
reg batu, batuNext;

ps2_rxtx ps2_unit
	(.clk(clk), .rst(rst), .wr_ps2(wr_ps2) ,
	.din(STRM), .dout(rx_data), .ps2d(ps2d), .ps2c(ps2c),
	.rx_done_tick(rx_done_tick),
	.tx_done_tick(tx_done_tick));

always@(posedge clk , posedge rst) begin
	if(rst)begin
		mouse_x <= 2;
		mouse_y <= 30;
	end else begin 
		batu <= batuNext;
		mouse_x <= mouse_xNext;
		mouse_y <= mouse_yNext;
	end
end

always @*begin
	if(m_done_tick)begin
		if(mouse_x > 600 && mouse_x < 750)begin
			mouse_xNext = 600;
			mouse_yNext = mouse_y;
		end else if(mouse_y > 440 && mouse_y < 590)begin
			mouse_xNext = mouse_x;
			mouse_yNext = 440;
		end else if(mouse_x > 750)begin
			mouse_xNext = 5;
			mouse_yNext = mouse_y;
		end else if(mouse_y > 590)begin
			mouse_xNext = mouse_x;
			mouse_yNext = 5;
		end else begin
			mouse_xNext = mouse_x + {xm[8] , xm};
			mouse_yNext = mouse_y - {ym[8] , ym};
		end
	end else begin
			mouse_xNext = mouse_x;
			mouse_yNext = mouse_y;
	end
end

	
//assign mouse_xNext = (~m_done_tick)? mouse_x : // no activity
					//mouse_x + {xm[8] , xm}; // x movement
//assign mouse_yNext = (~m_done_tick)? mouse_y : // no activity
					//mouse_y - {ym[8] , ym}; // y movement
					
				
///////////////////////////////////////////////////////////////////////////////



always @(posedge clk , posedge rst)begin
	if(rst)
		begin
			state_reg <= init1 ;
			x_reg <= 0 ;
			y_reg <= 0 ;
			btn_reg <= 0 ;
		end
	else
		begin
			state_reg <= state_next;
			x_reg <= x_next;
			y_reg <= y_next;
			btn_reg <= btn_next;
		end
end
// FSMD next-state logic
always @*
	begin
		state_next = state_reg;
		wr_ps2 = 1'b0; 
		m_done_tick = 1'b0;
		x_next = x_reg;
		y_next = y_reg;
		btn_next = btn_reg;
		case(state_reg)
			init1:
				begin
					wr_ps2 = 1'b1;
					state_next=init2;
				end
			init2 : // wait for send to complete
				if (tx_done_tick)
				state_next=init3 ;
			init3: // wait for acknowledge packet
				if (rx_done_tick)
				state_next=pack1;
			pack1: // wait for 1st data packet
				if(rx_done_tick)
				begin
					state_next=pack2;
					y_next [8]=rx_data[5];
					x_next [8]=rx_data[4];
					btn_next=rx_data[2:0];
				end
			pack2: // wait for 2nd data packet
				if (rx_done_tick)
				begin
					state_next=pack3;
					x_next [7:0] = rx_data;
				end
			pack3: // wait for 3rd data packet
				if(rx_done_tick)
				begin
					state_next=done;
					y_next [7:0] = rx_data;
				end
		done :
			begin
				m_done_tick = 1'b1;
				state_next = pack1;
			end
		endcase
	end
// output
assign xm = x_reg;
assign ym = y_reg;
assign btnm = btn_reg;
endmodule
