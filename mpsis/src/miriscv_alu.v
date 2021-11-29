`timescale 1ns / 1ps

module miriscv_alu(
    input       [4:0]   operator_i,
    input       [31:0]  operand_a_i,
    input       [31:0]  operand_b_i,
    output      [31:0]  result_o,
    output              comparison_result_o
    );
    
    reg [31:0]  res;
    reg         cres;
    
    assign  result_o = res;
    assign  comparison_result_o = cres;
    
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
    
    
    always @(*)
    begin
        case (operator_i)
            `ALU_ADD : 
                begin
                    res = operand_a_i + operand_b_i;
                    cres = 0;
                end
            `ALU_SUB :
                begin
                    res = operand_a_i - operand_b_i;
                    cres = 0;
                end
            `ALU_XOR :
                begin
                    res = operand_a_i ^ operand_b_i;
                    cres = 0;
                end
            `ALU_OR  :
                begin
                    res = operand_a_i | operand_b_i;
                    cres = 0;
                end
            `ALU_AND :
                begin
                    res = operand_a_i & operand_b_i;
                    cres = 0;
                end
            `ALU_SRA :
                begin
                    res = $signed(operand_a_i) >>> operand_b_i;
                    cres = 0;
                end
            `ALU_SRL :
                begin
                    res = operand_a_i >> operand_b_i;
                    cres = 0;
                end
            `ALU_SLL :
                begin
                    res = operand_a_i << operand_b_i;
                    cres = 0;
                end
            `ALU_LTS :
                begin
                    res = $signed(operand_a_i) < $signed(operand_b_i);
                    cres = 0;
                end
            `ALU_LTU :
                begin
                    res = operand_a_i < operand_b_i;
                    cres = 0;
                end
            `ALU_LTSF :
                begin
                    res = 0;
                    cres = $signed(operand_a_i) < $signed(operand_b_i);
                end
            `ALU_LTUF :
                begin
                    res = 0;
                    cres = operand_a_i < operand_b_i;
                end
            `ALU_GESF :
                begin
                    res = 0;
                    cres = $signed(operand_a_i) >= $signed(operand_b_i);
                end
            `ALU_GEUF :
                begin
                    res = 0;
                    cres = operand_a_i >= operand_b_i;
                end
            `ALU_EQF  :
                begin
                    res = 0;
                    cres = operand_a_i == operand_b_i;
                end
            `ALU_NEF  :  
                begin
                    res = 0;
                    cres = operand_a_i != operand_b_i;
                end         
        endcase
    end
    
endmodule
