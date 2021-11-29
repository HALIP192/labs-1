`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 25.09.2021 10:08:40
// Design Name: 
// Module Name: ram
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


module ram(
    input   [5:0] ad,
    output  [31:0] rd
    );
    
    reg [31:0]  RAM[0:63];
    
    assign  rd = RAM[ad[5:2]];
    
    initial begin
        $readmemh("program.txt", RAM);
    end
    
endmodule
