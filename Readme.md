<div align="center">
  <p>
    <a href="https://github.com/Adam-Al-Rahman/netzwerx" target="_blank">
      <img width="100%" src="docs/assets/img/netzwerx.png"></a>
  </p>
<br/>

[Netzwerx](https://github.com/Adam-Al-Rahman/netzwerx) is a networking python library

</div>

## Using Python Environment

If you install this module in a python environment (conda, venv, pyenv, etc)

Activate your python environment. Then

**Linux Usage:**

```shell
sudo $(which python) myscript.py

```

## Contributions

### How to build && Run

```shell
bazel build //...
```

```shell
bazel run @hedron_compile_commands//:refresh_all --config msan
```

```shell
bazel run @hedron_compile_commands//:refresh_all --config asan
```

```shell
# Still in planning phase of development
bazel run //netzwerx:netzwerx
```

### How to Test

```markdown
⚠️ Automatic Test with github [on platfrom: (windows, ubuntu)] is on the way.
In Development Phase: 2 -- Test
```

C++ test is done using (gtest & gmock)

```shell
bazel test //...
```

Python test is done using (pytest)

```shell
# Still in planning phase of development
```

## The Team

Netzwerx is currently maintained by [Atiq Urrehaman](https://github.com/Adam-Al-Rahman/).

## License

Netzwerx has a Apache-style license, as found in the [Apache-2.0 License](./LICENSE) file.
