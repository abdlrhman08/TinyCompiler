use std::{
    cell::LazyCell,
    collections::HashMap,
    io::Read,
    fs::File,
    ffi::{c_char, CStr}
};

#[repr(C)]
#[derive(Clone)]
enum TokenType {
    SEMICOLON,
    IF,
    THEN,
    END,
    REPEAT,
    UNTIL,
    IDENTIFIER,
    ASSIGN,
    READ,
    WRITE,
    LESSTHAN,
    EQUAL,
    PLUS,
    MINUS,
    MULT,
    DIV,
    OPENBRACKET,
    CLOSEDBRACKET,
    NUMBER,
}

#[repr(C)]
pub struct Token {
    token_type: TokenType,
    string_val: *const c_char,
    num_val: u64,
}

pub struct CompilationUnit {
    pub code_buffer: String,
    cursor: u64
}

impl CompilationUnit {
    fn open(file_name: &str) -> Self{
        let mut file = File::open(file_name).expect("Unable to open the file");
        let mut code_buffer = String::new();
        
        file.read_to_string(&mut code_buffer).expect("Unable to read file");
        CompilationUnit { code_buffer, cursor: 0 }
    }
}

static mut COMPILATION_UNIT: Option<CompilationUnit> = None;

static mut DFA: LazyCell<HashMap<u32, HashMap<char, u32>>> = LazyCell::new(|| {
    HashMap::from([
        (0, HashMap::from([
            (';', 1), ('<', 2), ('=', 3), ('+', 4), ('-', 5), ('*', 6), ('/', 7),
            ('(', 8), (')', 9)
        ])),
    ])
});
static mut ACCEPTING: LazyCell<HashMap<u32, TokenType>> = LazyCell::new(|| {
    HashMap::from([
        (1, TokenType::SEMICOLON), (2, TokenType::LESSTHAN), (3, TokenType::EQUAL), (4, TokenType::PLUS),
        (5, TokenType::MINUS), (6, TokenType::MULT), (7, TokenType::DIV), (8, TokenType::OPENBRACKET),
        (9, TokenType::CLOSEDBRACKET)
    ])
});

fn traverse(buffer: &str, cursor: &mut u64) -> TokenType {
    let mut state = 0;
    for (index, symbol) in buffer.chars().skip(*cursor as usize).enumerate() {
        unsafe { 
            state = DFA[&state][&symbol];
            
            if ACCEPTING.contains_key(&state) {
                *cursor = (index + 1) as u64;
                return ACCEPTING[&state].clone();
            }
        }
    }

    panic!("Invalid token");
}

#[no_mangle]
pub extern "C" fn open_compilation(file_name: *const c_char) {
    if file_name.is_null() {
        panic!("No file given");
    }
    unsafe { 
        let file_name = CStr::from_ptr(file_name).to_str().unwrap();
        if let None = COMPILATION_UNIT {
            COMPILATION_UNIT = Some(CompilationUnit::open(file_name));
        }
    };
}

#[no_mangle]
pub extern "C" fn next_token() -> Token {
    let compilation_unit = unsafe { 
        if let Some(ref mut compilation_unit) = COMPILATION_UNIT {
            compilation_unit
        } else { panic!("No file opened") }
    };

    Token { 
        token_type: traverse(&compilation_unit.code_buffer, &mut compilation_unit.cursor),
        string_val: std::ptr::null(),
        num_val: 12
    }
}

