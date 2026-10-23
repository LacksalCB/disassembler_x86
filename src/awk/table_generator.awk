BEGIN {
    current_block = ""
    array = "[a-zA-Z0-9_]" 
    FS = "[:=,]"
    print "#include <string.h>"
    print "#include \"../include/opcode_tables.h\"\n"
}

$1 ~ /^#/ {
    next    
}

$1 ~ /OPERANDS\s*/ {
    print "\n// OPERANDS:"
    for (i = 2; i < NF; i += 2) {
        sub(/\s*/, "", $i)
        printf "char %s[%s][4];\n", tolower($i), $(i+1) 
    }

    print "\nvoid init_operand_tables() {"
    next
}

$1 ~ /OPCODES\s*/ {
    print "}\n"
    print "\n//OPCODES:"
    for (i = 2; i < NF; i += 3) {
        name = tolower($i)
        sub(/\s*/, "", name)
        size = $(i+2)
        for (j = 1; j <= $(i+1); j++) {
            printf "struct opcode %s%s[%s];\n", name, j, size 
        }
    }

    print "\nvoid init_opcode_tables() {"
    next
}

# Operands table format
/^[a-zA-Z_0-9]+:+[0-9]+:+[0-9]+\s+*\{/ {
    print ""
    operand_block = $1
    sub(/\s.*/, "", operand_block)
    printf "// %s:\n", operand_block
    n = 0
    size = $2
    
    next
}

/^\}end/ {
    operand_block = ""
    next
}

operand_block ~ array {
    arr = tolower(operand_block)
    val = $1
    label = $2
    if (substr(label, 1, 1) == "$") {
        printf "    %s[0b%d] = %s;\n", arr, val, label
        next
    }

    printf "    strcpy(%s[0b%d], \"%s\");\n", arr, val, label
    next
}

label ~ /^\s*\$/ {    
    sub(/^\s*\$*/, "", label)
    printf "%s, ", label
}

# Opcode table format
/^[a-zA-Z_0-9]+:+[0-9]+\s*+{+\s*/ {
    print ""
    opcode_block = $1
    size = $2
    sub(/\s.*/, "", size)
    printf "// %s:\n", opcode_block
    n=0
    next
}

opcode_block {
    table = tolower(opcode_block)
    if ($1 == "") {
        next    
    }
    if ($1 ~ /^\[+[0-9]*+\]+\s*/) { 
        mode = $1
        opcode = $2
        flags = $4
        printf "    %s[0x%s].flags = %s;\n", table, opcode, flags
    } else { 
        if ($2 ~ /"+[a-zA-Z0-9]+"/) { 
            opcode = $1
            mnemonic = $2
            flags = $3
            operands = $4
            select = $5
            dest_table = $6
            sub(/\s*/, "", dest_table)
            printf "    %s[0x%s].mnemonic =%s;\n", table, opcode, mnemonic
            printf "    %s[0x%s].flags = %s;\n", table, opcode, flags
            printf "    memcpy(%s[0x%s].operands, (int[])%s, sizeof(%s[0x%s].operands));\n", table, opcode, operands, table, opcode
            printf "    %s[0x%s].sel = %s;\n", table, opcode, select
            printf "    %s[0x%s].next = %s;\n", table, opcode, dest_table
        } 
    }   
    next
}

/^}/ {
    opcode_block = ""
    next
}

END {
    print "}"
}

