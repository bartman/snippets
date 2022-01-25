# cgroup-memory-usage

This script manages cgroup that restricts processes to some subset of memory.

# install

```
./cgroup-memory-usage --install
```

This creates some sample config/scripts...

* `/etc/cgconfig.conf`    - creates and configures cgroups (cgconfigparser)
* `/etc/cgrules.conf`     - matches processes, puts them into a cgroup automatically (cgrulesengd)
* `/etc/systemd/system/cgroups.service`    - runs cgconfigparser and cgrulesengd

Following the install step, the script needs to be enabled/ran:

```
   $SUDO systemctl enable --now cgroups.service
```

# observing stats

single dump of counters

```
./cgroup-memory-usage --info
```

watch counters
```
./cgroup-memory-usage --info
```

# updating config

update soft/hard limits

```
./cgroup-memory-usage --soft-limit 1G --hard-limit 2G
```

