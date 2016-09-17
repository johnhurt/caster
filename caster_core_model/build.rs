
use std::fs;
use std::fs::File;
use std::io::prelude::*;
use std::process::Command;
use std::path::Path;
use std::path::PathBuf;

type Result<T> = std::result::Result<T, String>;

fn main() {
	
	let dir_entries = fs::read_dir("proto").unwrap();
	
	let mut lib_content: Vec<String> = Vec::new();
	
	lib_content.push(String::from("extern crate protobuf;\n"));
	
	let mut type_names: Vec<String> = Vec::new();
	
	for dir_entry in dir_entries {
		let path_buf = dir_entry.unwrap().path();
		let path = path_buf.as_path();
		let type_name = path.file_name().unwrap()
				.to_str().unwrap().split('.').next().unwrap();
		compile_proto_buf(path);
		let lib_content_lines = write_lib_lines(path);
		lib_content.push(lib_content_lines);
		type_names.push(String::from(type_name));
	}
	
	let type_names_slice: &Vec<String> = type_names.as_ref();
	
	for type_name in type_names_slice {
		fix_compiled_file_modules(&type_name, type_names_slice);
	}
	
	match write_lib_content(lib_content) {
		Err(e) => panic!("Failure: {}",e),
		Ok(()) => println!("Wrote Lib File"),
	}
}

/**
 * Compile the protobuf file at the path given to the src path
 */
fn compile_proto_buf(proto_path : &Path) {
	let output = Command::new("protoc")
			.arg("--proto_path")
			.arg("proto")
			.arg("--rust_out")
			.arg("src")
			.arg(proto_path)
			.output()
			.unwrap_or_else(|e| { panic!("failed to execute process: {}", e) });
			
	if 0 != output.status.code().unwrap() {
		panic!("Failed to compile proto {} got error {}"
			, proto_path.to_str().unwrap()
			, String::from_utf8_lossy(&output.stderr));
	}
	
	
}

fn write_lib_lines(proto_path : &Path) -> String {
	
	let type_name = proto_path.file_name().unwrap()
			.to_str().unwrap().split('.').next().unwrap();
	let mut result = String::new();
	
	result.push_str("\nmod ");
	result.push_str(type_name);
	result.push_str("_;\n");
	result.push_str("pub use self::");
	result.push_str(type_name);
	result.push_str("_::");
	result.push_str(type_name);
	result.push_str(";\n");
	
	result
}

fn fix_compiled_file_modules(type_name: &str, all_types: &Vec<String>) {
	
	let mut output_path : PathBuf = PathBuf::from("src");
	output_path.push(type_name);
	output_path.set_extension("rs");
	
	let mut fixed_path : PathBuf = PathBuf::from("src");
	fixed_path.push(format!("{}_",type_name));
	fixed_path.set_extension("rs");

    // Open and read the file entirely
    let mut src = File::open(&output_path).unwrap();
    let mut data = String::new();
    
    src.read_to_string(&mut data).unwrap();

	for replace_type in all_types {
		let to_replace = format!("{}::{}", replace_type, replace_type);
	
	    // Run replace operation in memory
	    data = data.replace(&*to_replace, &*replace_type);
	}
	
    // Recreate the file and dump the processed contents to it
    let mut dst = File::create(&output_path).unwrap();
    dst.write_all(data.as_ref()).unwrap();
	
	match fs::rename(output_path, fixed_path) {
		Err(why) => panic!("Move Failed: {}", why),
		Ok(()) => println!("Successfully updated: {}"
			, type_name),
	}
}

fn write_lib_content(lib_content: Vec<String>) -> std::io::Result<()> {
	
	let mut f = try!(File::create("src/lib.rs"));
	
	for section in lib_content {
		try!(f.write_all(section.as_ref()));
	}
	
	Ok(())
}