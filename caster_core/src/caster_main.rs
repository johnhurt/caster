use caster_core_model::CasterNodeConfig;
use getopts::Options;

/**
 * This is a wrapper for a complete caster application.  This exists so that 
 * multiple caster applications can run within the same program
 */
 pub struct CasterMain {
 	 pub config: CasterNodeConfig,
 	 pub exit_code: i32
 }
 
 impl CasterMain {
 	
	 /**
	  * Parse the given vector of strings into a node config 
	  */
	 pub fn parse_command_line_args(args: Vec<String>) 
	 		-> Result<CasterNodeConfig,String> {
	 			
	 	let mut result = CasterNodeConfig::new();
	 	
	    let program = args[0].clone();
	 	
	 	let mut opts = Options::new();
	 	
	 	opts.optflag("h", "help", "Prints this info :)");
	 	opts.optflag("s", "seed", "Indicates this node is the seed of a cluster");
	 	opts.optflag("R", "remote-seed"
	 			, "Indicates this node is the seed cluster bonded to a remote \
	 					cluster");
	 	opts.optopt("c", "chatter-port"
	 			, "Port used for clustering chatter (default 3130)", "3130");
	 	opts.optopt("o", "owned-ports"
	 			, "Set of ports for general use (default 3131-3140)"
	 			, "3131-3140");
	 	opts.optopt("P", "peer-address"
	 			, "Any other hosts(:chatter_port) as a comma-separated \
	                    list in the cluster to be joined either as a \
	                    local or remote peer.  Only the first \
	                    entry is used"
	 			, "caster.host1.com:3130,caster.host2.com:3130");
	 	opts.optopt("i", "internal-interface"
	 			, "Network Interface to use for communication among \
	                    peer nodes"
	 			, "caster1.internal.com");
	 	opts.optopt("e", "external-interface"
	 			, "Network Interface to use for communication between \
	                    this node and external tools or applications.  \
	                    Defaults to the internal interface"
	 			, "caster1.external.com");
	 	opts.optopt("r", "remote-interface"
	 			, "Network Interface to use for communication between \
	                    this node and remote peers.  Defaults to the \
	                    internal interface"
	 			, "caster1.intra-cluster.com");
	 	opts.reqopt("d", "storage-dirs"
	 			, "Comma separated list of directories to use for storage"
	 			, "/data1/caster,/data2/caster");
	 	
	 	let matches = match opts.parse(&args[1..]) {
	 		Ok(m) => { Some(m) },
	 		Err(e) => { println!("Failed to produce command line. {}", e); None }
	 	};
	 	
	 	
	 	
	 	Ok(result)
	 }
 	
 	pub fn new(config: CasterNodeConfig) -> Self {
 		CasterMain {
 			config: config,
 			exit_code: 0
 		}
 	}
 }
 