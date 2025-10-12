use common::{ecs_world_t, Plugin};
use std::ffi::CString;

use flecs_ecs::prelude::*;
use flecs_ecs::sys::{ecs_os_init};

#[unsafe(no_mangle)]
pub static VERSION: [u8; 2] = *common::VERSION;

#[derive(Debug, Component)]
pub struct Position {
    pub x: f32,
    pub y: f32,
}

unsafe extern "C" fn init(world: *mut ecs_world_t) {
    let world2 = World::new();

    println!("plugin raw = {:p}", world);
    let world_ref = unsafe {WorldRef::from_ptr(world)};
    world_ref.entity_named("Lol")
        .set(Position{x:10.0, y:20.0});
}

unsafe extern "C" fn do_work(world: *mut ecs_world_t) -> i32 {
    20
}

unsafe extern "C" fn shutdown(world: *mut ecs_world_t) {
    println!("Plugin shutting down!");
}

#[unsafe(no_mangle)]
pub extern "C" fn create_plugin() -> *mut Plugin {
    let plugin = Box::new(Plugin {
        name: CString::new("SamplePlugin").unwrap().into_raw(),
        init: Some(init),
        do_work: Some(do_work),
        shutdown: Some(shutdown),
    });
    Box::into_raw(plugin)
}