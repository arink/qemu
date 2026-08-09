_libtestdevice_bindings_inc_rs = rust.bindgen(
  args: bindgen_args_common + [
    '--allowlist-file', meson.project_source_root() / 'include/hw/misc/testdevice.h',
    '--blocklist-file',
      meson.project_source_root() /
      'include/(block\|chardev/|exec/|hw/core/|qemu/|qom/|system/).*',
  ],
  kwargs: bindgen_kwargs,
)

_libtestdevice_rs = static_library(
  'testdevice',
  structured_sources(
    [
      'src/lib.rs',
      'src/bindings.rs',
      'src/device.rs',
    ],
    {'.' : _libtestdevice_bindings_inc_rs},
  ),
  override_options: ['rust_std=2021', 'build.rust_std=2021'],
  rust_abi: 'rust',
  link_with: [
    _util_rs,
    _migration_rs,
    _bql_rs,
    _qom_rs,
    _system_rs,
    _hwcore_rs,
  ],
  dependencies: [common_rs],
)

rust_devices_ss.add(when: 'CONFIG_X_TESTDEVICE_RUST', if_true: [declare_dependency(
  link_whole: [_libtestdevice_rs],
  variables: {'crate': 'testdevice'},
)])


