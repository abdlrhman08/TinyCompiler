use std::{
    io::Read,
    fs::File,
    ffi::{c_char, CStr}
};

#[repr(C)]
enum TokenType {
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
    NUMBER
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
        if let Some(ref compilation_unit) = COMPILATION_UNIT {
            compilation_unit
        } else { panic!("No file opened") }
    };

    let index  = compilation_unit.cursor as usize;
    for symbol in compilation_unit.code_buffer.chars().skip(index) {
        match symbol {
            s => println!("{s}"),
        }
    }

    // This is just random
    Token { 
        token_type: TokenType::WRITE,
        string_val: std::ptr::null(),
        num_val: 12
    }
}

