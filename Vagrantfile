# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure(2) do |config|
  # Used for vboxfs support.
  config.vm.box = "bento/debian-8.2"

  # Shared the root of the repo to /vagrant_data using vboxfs.
  config.vm.synced_folder ".", "/vagrant_data"

  # Install mingw and setup env for building.
  config.vm.provision "shell", inline: <<-SHELL
     sudo apt-get update -qq
     sudo apt-get install -yqq mingw-w64
     cd /vagrant_data
     make CC=i686-w64-mingw32-g++-win32 WINDRES=i686-w64-mingw32-windres
  SHELL
end
