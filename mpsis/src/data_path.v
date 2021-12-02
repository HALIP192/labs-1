`timescale 1ns / 1ps

module miriscv_core (
	input           clk,
	input           reset,

	input	[31:0]	instr_rdata_i, 
	output	[31:0]	instr_addr_o,

	input	[31:0]	data_rdata_i,
	output			data_req_o,
	output			data_we_o,
	output	[3:0]	data_be_o,
	output	[31:0]	data_addr_o, 
	output	[31:0]	data_wdata_o
	);

    reg		[31:0]	PC;
    wire			stall;
	wire	[31:0]	command;
	wire	[31:0]	data[0:3];
	wire			comp;
	wire    [31:0]  mul[0:5];
	wire	[31:0]	imm_I;
	wire	[31:0]	imm_S;
	wire	[31:0]	imm_J;
	wire	[31:0]	imm_B;

	wire	[1:0]	ex_op_a_sel_o;
	wire	[2:0]	ex_op_b_sel_o;  
	wire	[4:0]	alu_op_o;  
	wire			mem_req_o;      
	wire			mem_we_o;      
	wire	[2:0]	mem_size_o;     
	wire			gpr_we_a_o;     
	wire			wb_src_sel_o;   
	wire			illegal_instr_o;
	wire			branch_o;
	wire			jal_o;       
	wire			jalr_o;         

	riscv_decode	dec_one(
		.fetched_instr_i(command),
		.ex_op_a_sel_o(ex_op_a_sel_o),
		.ex_op_b_sel_o(ex_op_b_sel_o),
		.alu_op_o(alu_op_o),  
		.mem_req_o(mem_req_o),
		.mem_we_o(mem_we_o),
		.mem_size_o(mem_size_o),
		.gpr_we_a_o(gpr_we_a_o),
		.wb_src_sel_o(wb_src_sel_o),
		.illegal_instr_o(illegal_instr_o),
		.branch_o(branch_o),
		.jal_o(jal_o),
		.jalr_o(jalr_o)
	);

	/*ram ram_one(
			.ad(PC),
			.rd(command)
		);*/
	
	register_file   regfile_one(
			.clk(clk),
			.reset(reset),
			.a1(command[19:15]),
			.a2(command[24:20]),
			.a3(command[11:7]),
			.wd3(mul[2]),
			.we(gpr_we_a_o),
			.rd1(data[0]),
			.rd2(data[1])
		);  
	
	miriscv_alu mir_one(
			.operator_i(alu_op_o),
			.operand_a_i(mul[0]),
			.operand_b_i(mul[1]),
			.result_o(data[2]),
			.comparison_result_o(comp)
		);

	/*data_memory	dmem_one(
			.clk(clk),
			.we(mem_we_o),
			.i({mem_req_o, mem_size_o}),
			.wd(data[1]),
			.ad(data[2]),
			.rd(data[3])
		);*/

	miriscv_lsu	mlsu_one(
			.clk_i(clk),
			.resetn_i(reset),
			
			.lsu_addr_i(data[2]),
			.lsu_we_i(mem_we_o),
			.lsu_size_i(mem_size_o),
			.lsu_data_i(data[1]),
			.lsu_req_i(mem_we_o),
			.lsu_stall_req_o(stall),
			.lsu_data_o(data[3]),

			.data_rdata_i(data_rdata_i),
			.data_req_o(data_req_o),
			.data_we_o(data_we_o),
			.data_be_o(data_be_o),
			.data_addr_o(data_addr_o),
			.data_wdata_o(data_wdata_o)
		);

	always @(posedge clk)
	begin
		if (reset)
			PC <= 0;
		else if (!stall)
			PC <= mul[5];
	end
	
	assign	command = instr_rdata_i;
	assign	instr_addr_o = PC;

	assign	mul[0] = ex_op_a_sel_o == 0 ? data[0] : (ex_op_a_sel_o == 1 ? PC : 0);
	assign	mul[1] = ex_op_b_sel_o == 0 ? data[1] : (ex_op_b_sel_o == 1 ? imm_I : 
					(ex_op_b_sel_o == 2 ? {command[31:12], 12'd0} : 
					(ex_op_b_sel_o == 3 ? imm_S : 'd4)));
	assign  mul[2] = wb_src_sel_o ? data[3] : data[2];
	assign  mul[4] = branch_o ? imm_B : imm_J;
	assign  mul[3] = (jal_o | (comp & branch_o)) ? mul[4] : 'd4;
	assign  mul[5] = jalr_o ? data[0] : PC + mul[3];

    assign  imm_I = {{20{command[31]}}, command[31:20]};
    assign  imm_S = {{20{command[31]}}, command[31:25], command[11:7]};
    assign  imm_J = {{12{command[31]}}, command[31], command[19:12], command[20], command[30:21], 1'b0};
    assign  imm_B = {{20{command[31]}}, command[31], command[7], command[30:25], command[11:8], 1'b0};

endmodule