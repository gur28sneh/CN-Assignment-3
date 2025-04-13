# CN-Assignment-3

---

## Ques 1

### Part A

Run the following commands:

```bash
chmod +x nat_topology.py
sudo ./nat_topology
```

This will open Mininet. Inside Mininet, run:

```bash
h3 ping -c 3 h1
h5 ping -c 3 h7
h8 ping -c 3 h2
```

### Part B

Update the switches with:

```bash
sh ovs-vsctl set bridge s1 stp_enable=true
sh ovs-vsctl set bridge s2 stp_enable=true  
sh ovs-vsctl set bridge s3 stp_enable=true  
sh ovs-vsctl set bridge s4 stp_enable=true 
```

Wait for 30 seconds, then again run the pings:

```bash
h3 ping -c 3 h1
h5 ping -c 3 h7
h8 ping -c 3 h2
```

---

## Ques 3

Save the files in the same folder, then compile and run using:

```bash
gcc dvr.c node0.c node1.c node2.c node3.c -o dvrouting
./dvrouting
```

When prompted, enter:

```bash
2
```
