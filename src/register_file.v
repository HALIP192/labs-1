`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 25.09.2021 09:27:05
// Design Name: 
// Module Name: register_file
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


module register_file(
    input           clk,
    input           reset,
    input   [4:0]   a1,
    input   [4:0]   a2,
    input   [4:0]   a3,
    input   [31:0]  wd3,
    input           we,
    output  [31:0]  rd1,
    output  [31:0]  rd2
    );
    
    reg  [31:0] reg_mem[0:31];
     
    assign  rd1 = reg_mem[a1];
    assign  rd2 = reg_mem[a2];
    
    initial begin
        $readmemh("start_mem.txt", reg_mem);
    end
    
    always @(posedge clk)
    begin
        if (reset)
        begin
            $readmemh("start_mem.txt", reg_mem);
        end 
        else if (we == 1)
        begin
            if (a3 != 0)
                reg_mem[a3] <= wd3;
        end 
    end
    
    
endmodule
