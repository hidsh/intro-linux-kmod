# intro-linux-kmod

[Linuxカーネルモジュール自作入門](https://booth.pm/ja/items/1056339)を写経

手打ちしたコードなので、間違いとかあるかも。

## tree
```
$ tree .
.
├── hello
│   ├── hello.c
│   ├── hello.ko
│   └── Makefile
├── hello_sysfs
│   ├── hello_sysfs.c
│   ├── hello_sysfs.ko
│   └── Makefile
└── jprobe
    ├── jpr.c
    ├── jpr.ko
    └── Makefile
```

## 必要なもの

```
sudo apt install build-essential
```

## hello

`Hello world!`と表示するだけのカーネルモジュールサンプル。

### ビルドと実行

```
cd hello
make

sudo insmod ./hello.ko
sudo rmmod hello
dmesg
```


## jprobe

デバッグ用のjprobeのサンプル。
`jprobe.ko`を`insmod`して`hello.ko`の挙動を確認する。

### ビルドと実行

```
cd jprobe
make
sudo insmod ./jpr.ko
sudo insmod ../hello/hello.ko
sudo rmmod hello
sudo rmmod jpr
dmesg
```

## hello_sysfs

カーネルモジュールから`sysfs`を使うサンプル。
`/sys/module/<module name>/`配下の読み書きを確認する。

### ビルドと実行

```
cd hello_sysfs
make
sudo insmod ./hello_sysfs.ko
ls -l /sys/module/hello_sysfs/fooattrs/
cat /sys/module/hello_sysfs/fooattrs/name
cat /sys/module/hello_sysfs/fooattrs/value
echo 42 | sudo dd of=/sys/module/hello_sysfs/fooattrs/value
cat /sys/module/hello_sysfs/fooattrs/value

sudo rmmod hello_sysfs
```

## テスト環境

- Distro: Ubuntu 16.04.7 LTS (GNU/Linux 4.4.0-210-generic x86_64)
- VMware Fusion: Player Version 12.2.1 (18811640)
- MacOS: 11.6.2 (Big Sur)
