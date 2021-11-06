# VM creation tutorial

Based on https://gitlab.com/adrian.stratulat/vm-scripts/

## Setting up the environment

Tested on Ubuntu 18.04.
Any reasonable recent Linux distribution should work.

```
sudo apt-get install vagrant virtualbox virtualbox-qt python-pip python-setuptools
pip install ansible
vagrant plugin install vagrant-disksize
vagrant plugin install vagrant-reload
```

## Building a VM

```
cd vagrantfile-programare
vagrant up
```

Running the `vagrant up` command will power on the VM.
When it is first run, it will create and configure the VM.

## Accessing the VM

To access the VM via `ssh` run the `vagrant ssh` command.

To accces the VM from the CLI using the `ssh` command, you first need
to dump the ssh configs into a file and the instruct the `ssh` command
to load the configs from the file.
The config file will tell the `ssh` command where to find the ssh key
used to auth.

```
vagrant ssh-config > ssh-conf
ssh -F ssh-conf vagrant@default
```

The same applies to `scp`. Just add the `-F ssh-conf` argument to your scp command.


## Exporting the VM

Use the `vagrantfile-programare/export.sh` script to get an `.ova` image.
