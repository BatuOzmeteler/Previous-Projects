`timescale 1ns / 1ps
module projectCPU(clk, rst, data_fromRAM, wrEn, addr_toRAM, data_toRAM, pCounter);

input clk, rst;
input [15:0] data_fromRAM;
output reg [15:0] data_toRAM;
output reg [12:0] addr_toRAM;
output reg [12:0] pCounter;
output reg wrEn;

// additional variables and signals

reg wrEnNext;
reg [ 2:0] opcode, opcodeNext;
reg [15:0] w, wNext;
reg [15:0] operand, operandNext, data_toRAMNext;
reg [12:0] pCounterNext, addr_toRAMNext, addressA, addressANext;
reg [ 3:0] state, stateNext; 

always @(posedge clk) begin
	state    			<= #1 stateNext;
	pCounter 			<= #1 pCounterNext;
	opcode   			<= #1 opcodeNext;
	addressA  			<= #1 addressANext;
	w 		   			<= #1 wNext;
	operand				<= #1 operandNext;
	addr_toRAMNext 	<= #1 addr_toRAM;
	data_toRAMNext 	<= #1 data_toRAM;
	wrEnNext					<= #1 wrEn;
end

always @* begin
	stateNext    = state;
	pCounterNext = pCounter;
	opcodeNext   = opcode;
	addressANext = addressA;
	operandNext  = operand;
	wNext 		 = w;
	addr_toRAM 	 = addr_toRAMNext;
	data_toRAM 	 = data_toRAMNext;
	wrEn = wrEnNext;
	if(rst) begin
		stateNext    = 0;
		pCounterNext = 0;
		opcodeNext   = 0;
		addressANext = 0;
		addr_toRAM   = 0;
		wrEn         = 0;
		data_toRAM   = 0;
		wNext 		 = 0;
		operandNext  = 0;
	end else begin
		case(state)                       
			0: begin				  // take instruction
				wrEn = 0;
				addr_toRAM  = pCounter;
				stateNext   = 1;
			end 
			1: begin                   // take *A
				opcodeNext  = data_fromRAM[15:13];
				addressANext = data_fromRAM[12:0]; //Address of *A
				wrEn = 0;
				if(data_fromRAM[12:0] == 0)begin //indirect addressing 
					addr_toRAM = 4;
					stateNext = 11; 
				end else begin // normal addressing
					addr_toRAM   = data_fromRAM[12:0];
					stateNext = 2;
				end
			end
			2: begin				// write *A or **4 into operandNext   
									// and jump to the matching opcode state
					operandNext = data_fromRAM;
					wrEn = 0;
					if(opcodeNext == 3'b000)//ADD
						stateNext = 3;
					else if (opcodeNext == 3'b001)//NAND
						stateNext = 4;
					else if (opcodeNext == 3'b010)//SRL
						stateNext = 5;
					else if (opcodeNext == 3'b011)//LT
						stateNext = 6;
					else if (opcodeNext == 3'b100)//BZ
						stateNext = 7;
					else if (opcodeNext == 3'b101)//CP2W
						stateNext = 8;
					else if (opcodeNext == 3'b110)//CPfW
						stateNext = 9;
					else if (opcodeNext == 3'b111)//MUL
						stateNext = 10;
			
			end
			3: begin //ADD
				wrEn = 0;            
				pCounterNext = pCounter + 1;
				wNext = w + operand;				
				stateNext = 0;
			end
			4: begin //NAND
				wrEn = 0;  
				pCounterNext = pCounter + 1;				
				wNext = ~(w & operand);				
				stateNext = 0;
			end
			5: begin //SRL
				if(operand < 17)begin
					wNext = w >> operand;
				end else begin
					wNext = w << (operand - 16);
				end
				wrEn = 0;  
				pCounterNext = pCounter + 1;							
				stateNext = 0;
			end
			6: begin //LT
				wrEn = 0;  
				pCounterNext = pCounter + 1;
				wNext = w < operand;
				stateNext = 0;
				
			end
			7: begin //BZ
				wrEn = 0;  
				if(w == 0)begin
					pCounterNext = operand;
					stateNext = 0;
				end else begin 
					pCounterNext = pCounter + 1;
					stateNext = 0;
				end
			end
			8: begin //CP2W
				wrEn = 0;  
				pCounterNext = pCounter + 1;
				wNext = operand;
				stateNext = 0;
			end
			9: begin //CPfW
				pCounterNext = pCounter + 1;
				wrEn = 1;
				addr_toRAM = addressA;
				data_toRAM = w;
				stateNext = 0;
			end
			10: begin // MUL
				wrEn = 0;  
				pCounterNext = pCounter + 1;
				wNext = w[7:0] * operand[7:0];
				stateNext = 0;
			end
			11: begin // to get **4 (indirect addressing)
				addr_toRAM  = data_fromRAM;
				addressANext = data_fromRAM;
				wrEn = 0;
				stateNext = 2;
			end
		endcase
	end
end
endmodule