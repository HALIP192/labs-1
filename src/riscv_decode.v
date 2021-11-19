`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 22.10.2021 15:06:37
// Design Name: 
// Module Name: riscv_decode
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

 module riscv_decode(
	input  [31:0]          fetched_instr_i,
	output reg [1:0]       ex_op_a_sel_o,
	output reg [2:0]       ex_op_b_sel_o,
	output reg [4:0]       alu_op_o,
	output reg             mem_req_o,
	output reg             mem_we_o,
	output reg [2:0]       mem_size_o,
	output reg             gpr_we_a_o,
	output reg             wb_src_sel_o,
	output reg             illegal_instr_o,
	output reg             branch_o,
	output reg             jal_o,
	output reg             jalr_o
	);

	`define ALU_ADD     5'b00000
    `define ALU_SUB     5'b01000
    `define ALU_SLL     5'b00001
    `define ALU_LTS     5'b00010
    `define ALU_LTU     5'b00011   
    `define ALU_XOR     5'b00100
    `define ALU_SRL     5'b00101
    `define ALU_SRA     5'b01101    
    `define ALU_OR      5'b00110
    `define ALU_AND     5'b00111
    `define ALU_EQF     5'b11000
    `define ALU_NEF     5'b11001
    `define ALU_LTSF    5'b11100
    `define ALU_GESF    5'b11101
    `define ALU_LTUF    5'b11110
    `define ALU_GEUF    5'b11111

	always @(fetched_instr_i) begin
        ex_op_a_sel_o   <= 0; 
        ex_op_b_sel_o   <= 0;
        alu_op_o        <= 0;    
        mem_req_o		<= 0;     
        mem_we_o		<= 0;      
        mem_size_o		<= 0;    
        gpr_we_a_o		<= 0;    
        wb_src_sel_o	<= 0;   
        illegal_instr_o	<= 0;
        branch_o		<= 0;
        jal_o			<= 0;
        jalr_o			<= 0;
		case (fetched_instr_i[6:0])
			'b0000011:
			begin
				gpr_we_a_o <= 1;
				ex_op_b_sel_o <= 1;
				alu_op_o <= `ALU_ADD;
				mem_req_o <= 1;
				wb_src_sel_o <= 1;
				case (fetched_instr_i[14:12])
					'b000:	mem_size_o <= 'd0;
					'b001:	mem_size_o <= 'd1;
					'b010:	mem_size_o <= 'd2;
					'b100:	mem_size_o <= 'd4;
					'b101:	mem_size_o <= 'd5;
					default:	illegal_instr_o <= 1;
				endcase
			end
			'b0001111:
				illegal_instr_o <= 0;
			'b0010011:
			begin
				gpr_we_a_o <= 1;
				ex_op_b_sel_o <= 1;
				alu_op_o <= {2'b00, fetched_instr_i[14:12]};
				if (fetched_instr_i[14:12] == 'b001 || fetched_instr_i[14:12] == 'b101)
				begin
					if (fetched_instr_i[14:12] == 'b001 && 7'b0000000 != fetched_instr_i[31:25])
						illegal_instr_o <= 1;
					else if (fetched_instr_i[14:12] == 'b101)
					begin
						if (7'b0100000 == fetched_instr_i[31:25])
							alu_op_o <= alu_op_o + 'b01000;
						else if (7'b0000000 != fetched_instr_i[31:25])
							illegal_instr_o <= 1;
					end
				end
			end
			'b0010111:
			begin
				gpr_we_a_o <= 1;
				ex_op_a_sel_o <= 1;
				ex_op_b_sel_o <= 2;
				alu_op_o <= `ALU_ADD;
			end
			'b0100011:
			begin
				mem_we_o <= 1;
				mem_req_o <= 1;
				ex_op_b_sel_o <= 3;
				alu_op_o <= `ALU_ADD;
				case (fetched_instr_i[14:12])
					'b000: mem_size_o <= fetched_instr_i[14:12];
					'b001: mem_size_o <= fetched_instr_i[14:12];
					'b010: mem_size_o <= fetched_instr_i[14:12];
					default: illegal_instr_o <= 1;
				endcase
			end
			'b0110011:
			begin
				gpr_we_a_o <= 1;
				alu_op_o <= {'b00, fetched_instr_i[14:12]};
				if (7'b0000000 != fetched_instr_i[31:25])
				begin
					if (7'b0100000 == fetched_instr_i[31:25] && (fetched_instr_i[14:12] == 3'b000 || fetched_instr_i[14:12] == 3'b101))
						alu_op_o <= alu_op_o + 'b01000;
					else 
						illegal_instr_o <= 1;
				end
			end
			'b0110111:
			begin
				gpr_we_a_o <= 1;
				ex_op_a_sel_o <= 2;
				ex_op_b_sel_o <= 2;
				alu_op_o <= `ALU_ADD;
			end
			'b1100011:
			begin
				branch_o <= 1;
				alu_op_o <= {'b11, fetched_instr_i[14:12]};
				if (fetched_instr_i[14:12] == 'b010 || fetched_instr_i[14:12] == 'b011)
				begin
					illegal_instr_o <= 1;
					alu_op_o <= 'b11000;
				end
			end
			'b1100111:
			begin
				gpr_we_a_o <= 1;
				ex_op_a_sel_o <= 1;
				ex_op_b_sel_o <= 4;
				alu_op_o <= `ALU_ADD;
				jalr_o <= 1;
				if (fetched_instr_i[14:12] != 'd0)
					illegal_instr_o <= 1;
			end
			'b1101111:
			begin
				gpr_we_a_o <= 1;
				ex_op_a_sel_o <= 1;
				ex_op_b_sel_o <= 4;
				alu_op_o <= `ALU_ADD;
				jal_o <= 1;
			end
			'b1110011: illegal_instr_o <= 0;
			default: illegal_instr_o <= 1;    
		endcase
	end
endmodule
