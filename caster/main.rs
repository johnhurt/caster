extern crate caster_core

use caster_core::CasterMain;
use caster_core_model::CasterNodeConfig;
use std::env;

fn main() {
	
	let args: Vec<String> = env::args().collect();	

    let mut config = CasterMain::parse_command_line(args);
    
}
