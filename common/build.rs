fn main() {
    println!("cargo:rerun-if-changed=include/plugin.h");

    let bindings = bindgen::Builder::default()
        .header("include/plugin.h")
        .allowlist_file(".*plugin.h")
        .opaque_type("ecs.*")
        .generate()
        .expect("Unable to generate bindings");

    bindings
        .write_to_file("src/raw_bindings.rs")
        .expect("Couldn't write bindings!");
}