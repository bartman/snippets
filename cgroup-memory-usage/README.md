# cgroup-memory-usage

This script manages cgroup that restricts processes to some subset of memory.

# install

We need a daemon that watches for new PIDs and puts them into cgroups.
This tool is `cgrulesengd` and comes from `cgroup-tools` package.

```
    sudo apt install cgroup-tools
```

Unfortunately, `cgrulesengd` needs to run as root, and there is no way to
have per-user configuration.

```
    sudo ./cgroup-memory-usage --install
```

This creates some sample config/scripts...

* `/etc/cgconfig.conf`    - creates and configures cgroups (`cgconfigparser`)
* `/etc/cgrules.conf`     - matches processes, puts them into a cgroup automatically (`cgrulesengd`)
* `/etc/systemd/system/cgroups.service`    - runs `cgconfigparser` and `cgrulesengd`

Following the install step, the script needs to be enabled/ran:

```
   sudo systemctl enable --now cgroups.service
```

# configuration

Edit the `/etc/cgrules.conf` file to configure your classification
accordingly.  See `man cgrules.conf` for further information.

# observing stats

single dump of counters
```
./cgroup-memory-usage --info
```

watch counters (like top)
```
./cgroup-memory-usage --watch
```

# updating config

update soft/hard runtime limits

```
./cgroup-memory-usage --update --soft-limit 1G --hard-limit 2G
```

