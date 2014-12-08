# Circus_client_cpp

Circus commands:

 * add: Add a watcher (TODO)
 * decr: Decrement the number of processes in a watcher (TODO)
 * dstats: Get circusd stats (TODO)
 * get: Get the value of a watcher option (TODO)
 * globaloptions: Get the arbiter options (TODO)
 * incr: Increment the number of processes in a watcher (TODO)
 * list: Get list of watchers or processes in a watcher
 
 Processes in a watcher:
 ```cpp
 Client circus_client = Client(context, host, port, timeout);
 Error error;
 vector<string> procs = circus_client.list("<watcher_name>", error);
 ```
 
  List of watchers:
 ```cpp
 Client circus_client = Client(context, host, port, timeout);
 Error error;
 vector<string> watchers = circus_client.list(error);
 ```
 
 Command-line equivalent:
 ```bash
 $ circusctl list [<name>]
```

 * listen: Suscribe to a watcher event (TODO)
 * listsockets: Get the list of sockets (TODO)
 * numprocesses: Get the number of processes (TODO)
 * numwatchers: Get the number of watchers (TODO)
 * options: Get the value of a watcher option (TODO)
 * quit: Quit the arbiter immediately (TODO)
 * reload: Reload the arbiter or a watcher (TODO)
 * restart: Restart the arbiter or a watcher (TODO)
 * rm: Remove a watcher (TODO)
 * set: Set a watcher option (TODO)
 * signal: Send a signal (TODO)
 * start: Start the arbiter or a watcher (TODO)
 * stats: Get process infos (TODO)
 * status: Get the status of a watcher or all watchers (TODO)
 * stop: Stop the arbiter or a watcher (TODO)

