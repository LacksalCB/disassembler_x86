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

$1 ~ /OPERANDS\s*/ {
    print "\n// OPERANDS:"
    print "void init_operand_tables();"
    for (i = 2; i < NF; i += 2) {
        sub(/\s*/, "", $i)
        printf "extern char %s[%s][4];\n", tolower($i), $(i+1) 
    }

    next
}

$1 ~ /OPCODES\s*/ {
    print "\n//OPCODES:"
    print "void init_opcode_tables();"
    for (i = 2; i < NF; i += 3) {
        name = tolower($i)
        sub(/\s*/, "", name)
        size = $(i+2)
        for (j = 1; j <= $(i+1); j++) {
            printf "extern struct opcode %s%s[%s];\n", name, j, size 
        }
    }
    next
}

/^[a-zA-Z0-9_]*+\s*+=+\s*[a-zA-Z0-9_,]*+{+\s*/ {
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

current_block == "OPERANDS" {
    print "HERE"
}

END {
   print "\n#endif"
}

