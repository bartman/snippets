# cgroup-memory-usage

This script manages cgroup that restricts processes to some subset of memory.

# install

## dependencies

We need a daemon that watches for new PIDs and puts them into cgroups.
This tool is `cgrulesengd` and comes from `cgroup-tools` package.

```
    sudo apt install cgroup-tools
```

## install service

Unfortunately, `cgrulesengd` needs to run as root, and there is no way to
have per-user configuration.  The `cgroup-memory-usage` will use `sudo`
to gain root access.

```
    ./cgroup-memory-usage --install
```

The install step creates some sample configuration files and scripts...

* `/etc/cgconfig.conf`    - creates and configures cgroups (`cgconfigparser`)
* `/etc/cgrules.conf`     - matches processes, puts them into a cgroup automatically (`cgrulesengd`)
* `/etc/systemd/system/cgroups.service`    - runs `cgconfigparser` and `cgrulesengd`

## configuration

You should configure the cgrups and rules by running...

```
    ./cgroup-memory-usage --edit-config
```

Edit the `/etc/cgrules.conf` file to configure your classification
accordingly.  See `man cgrules.conf` for further information.

## starting the serivce

Finally you can enable and start the service...

```
    ./cgroup-memory-usage --service-enable
```

(see `--help` for other service management options)

# usage

## observing stats

single dump of counters
```
./cgroup-memory-usage --info
```

watch counters (like top)
```
./cgroup-memory-usage --watch
```

## updating config

When the service is started, the `/etc/cgconfig.conf` will be used to
create a new croupt.  It is posslbe change these values at runtime.

Update soft/hard runtime limits

```
./cgroup-memory-usage --update --soft-limit 1G --hard-limit 2G
```

(see `--help` for other options)

