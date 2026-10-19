BEGIN {
    current_block = ""
    array = "[a-zA-Z0-9_]" 
    FS = "[:=,]"
    print "#include <string.h>"

    printf "\nstruct opcodes {\n    const char* mnemonic; \n    int flags; \n};\n\n"
}

$1 == "COUNT" {
    printf "#define COUNT %d  \n",$2
    next
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
    printf "\n// OPERANDS:\n"
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
    if (current_block == "FLAGS") {
        n = 0
    } else if (current_block == "FIELDS") { # not sure i'm gonna do this
        n = 0 
    } else if (current_block == "OPCODES") {
        print "}"
        printf "const struct opcodes opcode_table[256] = {\n"
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

# Special case, handled as macros 
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

# For declaration order
current_block == "FIELDS" {
    field = tolower($1)
    size = $2
    sub(/^\$/, "", field)

    printf "char %s[%s];\n", field, size
    next
}

current_block == "OPCODES" {
    if ($1 == "") {
        next    
    }
    opcode = $1
    mnemonic = $2
    flags = $3

    printf "    [0x%s] = { .mnemonic = %s, .flags = %s},\n", opcode, mnemonic, flags
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
   print "};"
}

