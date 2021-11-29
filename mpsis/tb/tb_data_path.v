`timescale 1ns / 1ps

module tb_data_path ();

	reg	clk;
	reg	reset;

	data_path dp_one(
			.clk(clk),
			.reset(reset)
		);

	initial
	begin
		clk = 0;
		while (1)
			#5 clk = !clk;
	end

	initial
	begin
		reset = 1;
		#20 reset = !reset;

	end

endmodule