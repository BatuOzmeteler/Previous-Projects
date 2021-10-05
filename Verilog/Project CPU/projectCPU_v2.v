`timescale 1ns / 1ps
module projectCPU_v2(clk, rst, wrEn, data_fromRAM ,addr_toRAM, data_toRAM, pCounter);
	
	parameter SIZE = 13;

	input 						clk, rst;
	input wire [15:0] 		data_fromRAM;
	
	output reg 					wrEn;
	output reg [SIZE-1:0] 	addr_toRAM;
	output reg [15:0] 		data_toRAM;
	output reg [SIZE-1:0] 	pCounter;
	
	// Regular expressions and more variables.
	reg [ 2:0] 					opcode, opcodeNext;
	reg [12:0]					A, ANext;
	reg [15:0] 					W, WNext, AStarNext, AStar ;
	reg [ 3:0] 					state, stateNext;
	reg [SIZE-1:0] 			addr_toRAMReg, pCounterNext;
	reg [15:0] 					data_toRAMReg;

	always @(posedge clk)
		begin
			pCounter 		<=#1 pCounterNext;
			state 			<=#1 stateNext;
			opcode   		<=#1 opcodeNext;
			A					<=#1 ANext;
			AStar	   		<=#1 AStarNext;
			W     			<=#1 WNext;
			addr_toRAMReg 	<=#1 addr_toRAM;
			data_toRAMReg 	<=#1 data_toRAM;
		end

	always @*
		begin
			/*
			Latch Preventing block.
			Remind the previous values to the signals. It is essential to create registers.
			*/
			stateNext 		= state;
			pCounterNext 	= pCounter;
			opcodeNext 		= opcode;
			ANext 			= A;
			AStarNext 		= AStar;
			WNext 			= W;
			
			addr_toRAM 		= addr_toRAMReg;
			data_toRAM 		= data_toRAMReg;

			if(rst)
				begin
					pCounterNext 	= 0;
					stateNext 		= 0;
					opcodeNext 		= 0;
					wrEn 				= 0;
					AStarNext 		= 0;
					ANext 			= 0;
					WNext 			= 0;
					addr_toRAM 		= 0;
					data_toRAM 		= 0;

				end
			else if(state == 0)
				begin
					/*Send a request for RAM[pCounter] from RAM.
					* Since block RAM works only in posedges, the value of RAM[pCounter] will be received at the next clock cycle. 
					* Also, set wrEn to zero. It may remain 1 from the state 4.
					*/
					addr_toRAM = pCounter;
					wrEn = 0;
					stateNext = 1;
				end
				
			else if(state == 1)
				begin
				/* The instruction from the RAM[pCounter] is received. Now read the A and opcode.
				*/ 
					wrEn = 0;
					opcodeNext   = data_fromRAM[15:13];
					ANext = data_fromRAM[12:0];

					//Indirect addressing condition.
					if(ANext == 0)
						begin
							addr_toRAM = 4;
							stateNext = 4;
						end
					else
						begin
							addr_toRAM = ANext;				
							stateNext = 2;
						end
				end
			else if(state == 2)
				begin
					// Read the AStar from the RAM.
					wrEn = 0;
					AStarNext = data_fromRAM;
					stateNext = 3;
				end
			else if(state == 3)
				begin
					//All W write operations.
					case(opcode)
						0 : WNext = AStar + W;
						1 : WNext = ~(AStar & W);
						2 : WNext = AStar <= 16 ? W >> AStar : W << AStar - 16;
						3 : WNext = W < AStar;
						5 : WNext = AStar;
						6 : 	begin
									addr_toRAM = A;
									data_toRAM = W;
									wrEn = 1;
								end
						7 : WNext = W[7:0] * AStar[7:0];
						default: WNext = W;
					endcase
					//BZ Condition.
					if(opcode == 4)
						begin
							pCounterNext = (W == 0) ? AStar : pCounter + 1;
						end
					else
						begin
							pCounterNext = pCounter + 1;
						end
					if(opcode != 6)
						begin
							wrEn = 0;
						end
					stateNext = 0;
				end
					
			else if(state == 4)
				begin
					//Indirect addressing.
					wrEn = 0;
					addr_toRAM = data_fromRAM;
					ANext = data_fromRAM;
					stateNext = 2;
				end
			//Latch protection for wrEn signal.
			else
				wrEn = 0;

			end
			
endmodule

