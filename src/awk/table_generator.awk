BEGIN {
    current_block = ""
    array = "[a-zA-Z0-9_]" 
    FS = "[:=,]"
    print "#include <string.h>"
    print "#include \"../include/opcode_tables.h\""
    print "    struct opcode opcode_table_one_byte[256];"
}

$1 ~ /^#/ {
    next    
}

$1 == "OPERANDS" {
    printf "// OPERANDS:\n"
    print "void init_operand_tables(void) {"
    next
}

# Operands table format
/^[a-zA-Z_0-9]+:+[0-9]+:+[0-9]+\s+*\{/ {
    print ""
    operand_block = $1
    sub(/\s.*/, "", operand_block)
    printf "// %s:\n", operand_block
    if ($3) {
        sub(/^*\s*{\s*/, "", $3)
        rows = $2
        cols = $3
        printf "    char %s[%s][%s];\n", tolower(operand_block), rows, cols
    } else {
        sub(/^*\s*{\s*/, "", $2)
        rows = $2
        printf "    char %s[%s];\n", tolower(operand_block), rows
    }
        n = 0
        size = $2
    
    next
}

/^\}end/ {
    operand_block = ""
    next
}

# Rest of the fields should be key value pairs of binary number to label
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

# If I see an l-val with $, treat it as a pointer to a new array
# Basically just means it isn't a terminal value
label ~ /^\s*\$/ {    
    sub(/^\s*\$*/, "", label)
    printf "%s, ", label
}

# Opcode table format
/^[a-zA-Z_0-9]+\s*+{+\s*/ {
    print ""
    opcode_block = $1
    sub(/\s.*/, "", opcode_block)
    printf "// %s:\n", opcode_block
    if (opcode_block ~ /^GRP+[0-9]*/ ) {
        printf "    struct opcode %s[8];\n", tolower(opcode_block)
        n=0
    }
    next
}

opcode_block ~ /^GRP+[0-9]*/ {
    if ($1 == "}") {
        opcode_block = ""
        next
    }
    table = tolower(opcode_block)
    mnemonic = tolower($1)    
   
    printf "    %s[%d].mnemonic = \"%s\";\n", table, n++, mnemonic
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
            printf "    %s[0x%s].mnemonic =%s;\n    %s[0x%s].flags = %s;\n", table, opcode, mnemonic, table, opcode, flags
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

