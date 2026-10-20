BEGIN {
    current_block = ""
    array = "[a-zA-Z0-9_]" 
    FS = "[:=,]"
    print "#include <string.h>"
    print "#include \"../include/opcode_tables.h\""


    print "    struct opcode opcode_table[256];"
}

$1 == "OPERANDS" {
    printf "// OPERANDS:\n"
    print "void init_operand_tables(void) {"
    next
}


$1 ~ /^#/ {
    next    
}

/^[a-zA-Z_0-9]+:?+[0-9]*+:?+[0-9]*+\s+*\{/ {
    print ""
    current_block = $1
    sub(/\s.*/, "", current_block)
    printf "// %s:\n", current_block
    if (current_block == "OPCODE_TABLE") {
        n=0;
    } else {
    if ($3) {
        sub(/^*\s*{\s*/, "", $3)
        rows = $2
        cols = $3
        printf "    char %s[%s][%s];\n", tolower(current_block), rows, cols
    } else {
        sub(/^*\s*{\s*/, "", $2)
        rows = $2
        printf "    char %s[%s];\n", tolower(current_block), rows
    }
        n = 0
        size = $2
    }
    next
}

/^\}/ {
    current_block = ""
    next
}

current_block == "OPCODE_TABLE" {
    if ($1 == "") {
        next    
    }
    opcode = $1
    mnemonic = $2
    flags = $3

    printf "        opcode_table[0x%s].mnemonic = %s;\nopcode_table[0x%s].flags = %s;\n", opcode, mnemonic, opcode, flags
    next
}

# Rest of the fields should be key value pairs of binary number to label
current_block ~ array {
    arr = tolower(current_block)
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

END {
    print "}"
}

