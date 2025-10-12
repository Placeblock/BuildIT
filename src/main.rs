use libloading::{Library, Symbol};
use common::{CreatePluginFn, CREATE_PLUGIN_SYMBOL, VERSION};
use std::ffi::CStr;

use flecs_ecs::prelude::*;

fn main() {
    let lib = unsafe { Library::new("./target/debug/libsample_plugin.so") }
        .expect("Failed to load plugin");

    unsafe {
        let plugin_version: Symbol<&[u8; 2]> = lib.get("VERSION".as_bytes()).expect("Failed to load version");
        let string_version = str::from_utf8(*plugin_version).unwrap();
        let compiled_version = str::from_utf8(VERSION).unwrap();
        if string_version != compiled_version {
            panic!("INVALID PLUGIN VERSION {} FOR COMPILED VERSION {}", string_version, compiled_version)
        }

        let create_plugin: Symbol<CreatePluginFn> = lib
            .get(CREATE_PLUGIN_SYMBOL)
            .expect("Failed to load create_plugin");

        let plugin = *create_plugin.expect("Invalid function create_plugin")();
        let name = CStr::from_ptr(plugin.name).to_str().unwrap();

        let mut world = World::new();


        println!("Host raw = {:p}", world.ptr_mut());

        println!("Loaded plugin: {}", name);

        plugin.init.expect("Failed to load init")(world.ptr_mut());

        let comp = world.component("Position");
        world.obser

        plugin.do_work.expect("Failed to load do_work")(world.ptr_mut());
        plugin.shutdown.expect("Failed to load shutdown")(world.ptr_mut());

    }
}