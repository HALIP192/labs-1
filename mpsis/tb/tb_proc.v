`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 22.10.2021 14:33:45
// Design Name: 
// Module Name: tb_proc
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


module tb_proc();
    reg clk;
    reg reset;
    reg [7:0] sw;
    proc one_proc(
        .clk(clk),
        .reset(reset),
        .SW(sw)
    );
    
    initial
    begin
       clk <= 0;
       while (1)
        #5 clk <= ~clk;
    end
    
    initial
    begin
        sw <= 'b0000_0000;
        reset <= 0;
        #10;
        reset <= 1;
        #10;
        reset <= 0;
        #600;
        $finish();
    end
endmodule
