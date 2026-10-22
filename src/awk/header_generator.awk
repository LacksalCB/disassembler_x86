BEGIN {
    current_block = ""
    array = "[a-zA-Z0-9_]" 
    FS = "[:=,]"
    print "#ifndef OPCODE_TABLES_H"
    print "#define OPCODE_TABLES_H"

    print "#include <string.h>"
    print "#include <stdint.h>"
}

$1 == "OPCODE_LENGTH" {
    printf "#define OPCODE_LENGTH %d  \n",$2
    next
}

$1 == "DEFAULT_BIT_WIDTH" {
    printf "#define DEFAULT_BIT_WIDTH %d \n", $2
    next
}

$1 == "OPERANDS" {
    print "\n// OPERANDS:"
    print "void init_operand_tables(void);"
    next
}

$1 == "OPCODES" {
    print "\n//OPCODES:"
    print "extern struct opcode opcode_table_one_byte[256];"
    next
}

/^[a-zA-Z0-9_]*+\s*+=+\s*+{+\s*/ {
    print ""
    current_block = $1
    sub(/\s.*/, "", current_block)
    printf "// %s:\n", current_block
    n = 0
    next
}

/^\}end/ {
    current_block = ""
    next
}

current_block == "FLAGS" { 
    flag = $1
    sub(/^\$/, "", flag)
    sub(/^[ \t]+|[ \t]+$/, "", flag)
    if (flag == "F_NONE") {
        printf "#define %s  0\n", flag
    } else {
        printf "#define %s  1<<%d\n", flag, n++
    }
    next    
}

current_block == "STRUCTURES" {
    print $0
}

END {
   print "\n#endif"
}

