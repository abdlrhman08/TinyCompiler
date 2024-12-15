use std::{
    collections::HashSet,
    io::{Read, Write},
    fs::File,
    ffi::{c_char, CStr, CString}
};

#[repr(C)]
#[derive(Clone, Debug)]
pub enum TokenType {
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
    ELSE,
}

#[repr(C)]
pub struct Token {
    token_type: TokenType,
    string_val: *const c_char,
    num_val: u64,
}

impl std::fmt::Display for Token {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "Token type {:?} ", self.token_type);
        if self.string_val != std::ptr::null() {
            unsafe { 
                write!(f, "String Value '{}'", CStr::from_ptr(self.string_val).to_str().unwrap()) 
            };
        } else {
            write!(f, "Number Value {}", self.num_val);
        }
        Ok(())
    }
}

pub struct Tokenizer {
    input: String,
    current_pos: usize,
    code_book: HashSet<CString>
}

#[derive(Debug)]
pub enum TokenizerError {
    InvalidIdentifier(String),
    UnknownToken(char),
}

impl Tokenizer {
    pub fn new(input: String) -> Self {
        let mut code_book = HashSet::new();
        code_book.reserve(input.split_whitespace().count());
        Tokenizer {
            input,
            current_pos: 0,
            code_book
        }
    }

    fn skip_whitespace(&mut self) {
        while self.current_pos < self.input.len() && 
              self.input.chars().nth(self.current_pos).unwrap().is_whitespace() {
            self.current_pos += 1;
        }
    }

    fn skip_comments(&mut self) {
        if self.current_pos < self.input.len() && 
            self.input.chars().nth(self.current_pos).unwrap() == '{'
        {
            while self.current_pos < self.input.len() && 
                  self.input.chars().nth(self.current_pos).unwrap() != '}' {
                self.current_pos += 1;
            }
            if self.current_pos < self.input.len() {
                self.current_pos += 1;
            }
        }
    }

    fn tokenize_number(&mut self) -> Result<Option<Token>, TokenizerError> {
        let start = self.current_pos;
        while self.current_pos < self.input.len() && 
              self.input.chars().nth(self.current_pos).unwrap().is_digit(10) {
            self.current_pos += 1;
        }
        
        if start != self.current_pos {    
            let num_str = &self.input[start..self.current_pos];
            return Ok(Some(Token {
                token_type: TokenType::NUMBER,
                string_val: std::ptr::null(),
                num_val: num_str.parse().unwrap_or(0),
            }));
        }
        Ok(None)
    }

    fn tokenize_identifier(&mut self) -> Result<Option<Token>, TokenizerError> {
        let start = self.current_pos;
        
        if self.current_pos < self.input.len() && 
           !self.input.chars().nth(self.current_pos).unwrap().is_alphabetic() {
            return Ok(None);
        }

        let mut end_pos = self.current_pos;
        while end_pos < self.input.len() {
            let ch = self.input.chars().nth(end_pos).unwrap();
            if !ch.is_alphabetic() {
                break;
            }
            end_pos += 1;
        }

        if start != end_pos {
            let id_str = &self.input[start..end_pos];
            
            let token_type = match id_str {
                "if" => TokenType::IF,
                "then" => TokenType::THEN,
                "else" => TokenType::ELSE,
                "end" => TokenType::END,
                "repeat" => TokenType::REPEAT,
                "until" => TokenType::UNTIL,
                "read" => TokenType::READ,
                "write" => TokenType::WRITE,
                _ => TokenType::IDENTIFIER,
            };
            
            let cstr_conversion = CString::new(id_str).unwrap();
            let val_address = if let None = self.code_book.get(&cstr_conversion) {
                let sorry_for_redundancy = cstr_conversion.clone();
                self.code_book.insert(cstr_conversion);
                self.code_book.get(&sorry_for_redundancy).unwrap().as_ptr() as *const c_char
            } else {
                self.code_book.get(&cstr_conversion).unwrap().as_ptr() as *const c_char
            };

            self.current_pos = end_pos;
            return Ok(Some(Token {
                token_type,
                string_val: val_address,
                num_val: 0,
            }));
        }
        Ok(None)
    }

    fn tokenize_symbol(&mut self) -> Result<Option<Token>, TokenizerError> {
        if self.current_pos >= self.input.len() {
            return Ok(None);
        }

        let start = self.current_pos;
        let ch = self.input.chars().nth(self.current_pos).unwrap();
        let token_type = match ch {
            ';' => TokenType::SEMICOLON,
            '<' => TokenType::LESSTHAN,
            '=' => TokenType::EQUAL,
            '+' => TokenType::PLUS,
            '-' => TokenType::MINUS,
            '*' => TokenType::MULT,
            '/' => TokenType::DIV,
            '(' => TokenType::OPENBRACKET,
            ')' => TokenType::CLOSEDBRACKET,
            ':' => {
                if self.current_pos + 1 < self.input.len() && 
                   self.input.chars().nth(self.current_pos + 1).unwrap() == '=' {
                    self.current_pos += 1;
                    TokenType::ASSIGN
                } else {
                    return Err(TokenizerError::UnknownToken(ch));
                }
            }
            _ => return Err(TokenizerError::UnknownToken(ch)),
        };
        self.current_pos += 1;
        
        let cstr_conversion = CString::new(&self.input[start..self.current_pos]).unwrap();
        let val_address = if let None = self.code_book.get(&cstr_conversion) {
            let sorry_for_redundancy = cstr_conversion.clone();
            self.code_book.insert(cstr_conversion);
            self.code_book.get(&sorry_for_redundancy).unwrap().as_ptr() as *const c_char
        } else {
            self.code_book.get(&cstr_conversion).unwrap().as_ptr() as *const c_char
        };

        Ok(Some(Token {
            token_type,
            string_val: val_address,
            num_val: 0,
        }))
    }

    pub fn tokenize(&mut self) -> Result<Vec<Token>, TokenizerError> {
        let mut tokens = Vec::new();

        while self.current_pos < self.input.len() {
            self.skip_whitespace();
            self.skip_comments();
            self.skip_whitespace();

            if self.current_pos >= self.input.len() {
                break;
            }

            if let Some(token) = self.tokenize_number()? {
                tokens.push(token);
            } else if let Some(token) = self.tokenize_identifier()? {
                tokens.push(token);
            } else if let Some(token) = self.tokenize_symbol()? {
                tokens.push(token);
            } else {
                let ch = self.input.chars().nth(self.current_pos).unwrap();
                return Err(TokenizerError::UnknownToken(ch));
            }
        }

        Ok(tokens)
    }
}

pub struct CompilationUnit {
    identified_tokens: usize,
    tokenizer: Option<Tokenizer>,
}

impl CompilationUnit {
    fn open(file_name: &str) -> Result<Self, &str> {
        let file = File::open(file_name);
        let mut code_buffer = String::new();
        if let Ok(mut file) = file {
            if let Err(_) = file.read_to_string(&mut code_buffer) {
                return Err("Error reading the file")
            }
            if !code_buffer.is_ascii() {
                return Err("Compilation unit has non-ascii characters, can not compile");
            }
        } else {
            return Err("Error opening the file");
        }

        Ok(CompilationUnit { 
            identified_tokens: 0,
            tokenizer: Some(Tokenizer::new(code_buffer)),
        })
    }

    fn tokenize(&mut self) {
        let parsed_out = File::create("parsed.txt");
        let tokens = self.tokenizer
            .as_mut()
            .expect("Tokenizer not initialized")
            .tokenize();
        
        match tokens {
            Ok(tokens) => {
                self.identified_tokens = tokens.len();
                if let Ok(mut out_file) = parsed_out {
                    println!("Writing to output file");
                    for token in tokens.iter() {
                        writeln!(out_file, "{}", token);
                    }
                } else {
                    println!("Could not open a file to write, skipping..");
                }
                unsafe { TOKENS = Some(tokens.into_boxed_slice())};
            },
            Err(e) => {
                unsafe {
                    ERROR_STRING = Some(
                        CString::new(format!("Error during tokenization: {:?}", e)).unwrap()
                    )
                }
            }
        }
    }
}

static mut COMPILATION_UNIT: Option<CompilationUnit> = None;
static mut ERROR_STRING: Option<CString> = None;
static mut TOKENS: Option<Box<[Token]>> = None;

#[no_mangle]
pub extern "C" fn open_compilation(file_name: *const c_char) -> u8 {
    if file_name.is_null() {
        panic!("No file given");
    }

    unsafe {
        let file_name = CStr::from_ptr(file_name).to_str().unwrap();
        match CompilationUnit::open(file_name) {
            Err(e) => {
                ERROR_STRING = Some(CString::new(e).unwrap());
                1
            }
            Ok(c) => {
                COMPILATION_UNIT = Some(c);
                0 
            }
        }
    }
}

#[no_mangle]
pub extern "C" fn tokenize() -> *const Token {
    unsafe {
        let compilation_unit = COMPILATION_UNIT.as_mut().expect("No file opened");
        compilation_unit.tokenize();
        if let Some(ref tokens) = TOKENS {
            return tokens.as_ptr();
        }
    }
    std::ptr::null()
}

#[no_mangle]
pub extern "C" fn get_token_count() -> usize {
    unsafe {
        COMPILATION_UNIT
            .as_ref()
            .expect("No file opened")
            .identified_tokens
    }
}

#[no_mangle]
pub extern "C" fn print_token(token: *mut Token) {
    unsafe { println!("{}", *token) };
}

#[no_mangle]
pub extern "C" fn get_error_string() -> *const c_char {
    unsafe {
        match ERROR_STRING {
            Some(ref err) => err.as_ptr(),
            None => std::ptr::null()
        }
    }
}

#[no_mangle]
pub extern "C" fn print_error() {
    unsafe {
        match ERROR_STRING {
            Some(ref err) => println!("Error: {}", err.to_str().unwrap()),
            None => println!("No error occured")
        }
    }
}
