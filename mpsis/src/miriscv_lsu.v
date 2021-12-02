module miriscv_lsu
(
	input			clk_i,
	input			resetn_i,

	input	[31:0]	lsu_addr_i,			//
	input			lsu_we_i,			//
	input	[2:0]	lsu_size_i,			
	input	[31:0]	lsu_data_i,			//
	input			lsu_req_i,			//
	output			lsu_stall_req_o,	
	output	[31:0]	lsu_data_o,			

	input	[31:0]	data_rdata_i,		
	output			data_req_o,			//
	output			data_we_o,			//
	output	[3:0]	data_be_o,			
	output	[31:0]	data_addr_o,		//
	output	[31:0]	data_wdata_o		//
);
	
	

	assign	data_addr_o		= lsu_addr_i;
	assign	data_wdata_o	= data_wdata;
	assign	data_req_o		= lsu_req_i;
	assign	data_we_o		= lsu_we_i;
	assign 	lsu_data_o		= data;
	assign	data_be_o		= data_be;
	assign	lsu_stall_req_o	= stall;
	assign	offset			= lsu_addr_i[1:0];

	reg		[31:0]	data_wdata;
	reg				stall;
	reg		[31:0]	data;
	reg		[3:0]	data_be;
	wire	[1:0]	offset;


	always @(posedge clk_i) begin
		if (lsu_req_i && !stall)
			stall <= 1;
		else
			stall <= 0;
	end

	always @(*) begin
		case(lsu_size_i)
        'd0: begin 
                case (offset)
                    'b00: begin 
                               data <= {{24{data_rdata_i[7]}}, data_rdata_i[7:0]};
                               data_be <= 'b0001;
                          end
                    'b01: begin
                                data <= {{24{data_rdata_i[15]}}, data_rdata_i[15:8]};
                                data_be <= 'b0010;
                          end
                    'b10: begin
                                data <= {{24{data_rdata_i[23]}}, data_rdata_i[23:16]};
                                data_be <= 'b0100;
                          end
                    'b11: begin
                                data <= {{24{data_rdata_i[31]}}, data_rdata_i[31:24]};     
                                data_be <= 'b1000;
                          end
                endcase
                data_wdata <= {4{lsu_data_i[7:0]}};
             end
        'd1: begin 
                case (offset)
                    'b00: begin
                                data <= {{16{data_rdata_i[15]}}, data_rdata_i[15:0]};
                                data_be <= 'b0011;
                          end
                    'b10: begin
                                data <= {{16{data_rdata_i[31]}}, data_rdata_i[31:16]};
                                data_be <= 'b1100;
                          end
                endcase
                data_wdata <= {2{lsu_data_i[15:0]}};
             end 
        'd2: begin
                data <= data_rdata_i;
                data_be <= 'b1111;
                data_wdata <= lsu_data_i;
             end
        'd3: begin 
                case (offset)
                    'b00: data <= {24'b0, data_rdata_i[7:0]};
                    'b01: data <= {24'b0, data_rdata_i[15:8]};
                    'b10: data <= {24'b0, data_rdata_i[23:16]};
                    'b11: data <= {24'b0, data_rdata_i[31:24]};     
                endcase
             end 
        'd4: begin 
                case (offset)
                    'b00: data <= {16'b0, data_rdata_i[15:0]};
                    'b10: data <= {16'b0, data_rdata_i[31:16]};
                endcase
             end  
         default: begin
                    data_be <= 'b0001;
                    data <= 'd0;
                    data_wdata <= 'd0;
             end
    endcase
	end


endmodule