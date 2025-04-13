# CN-Assignment-3

#Ques 1
In part a, run the code using commands:
'''bash
chmod +x nat_topology.py 
sudo ./nat_topology
'''
This will open the mininet then run:
'''nash
h3 ping -c 3 h1
h5 ping -c 3 h7
h8 ping -c 3 h2
'''
For part b, update
'''bash
sh ovs-vsctl set bridge s1 stp_enable=true 
sh ovs-vsctl set bridge s2 stp_enable=true  
sh ovs-vsctl set bridge s3 stp_enable=true  
sh ovs-vsctl set bridge s4 stp_enable=true 
'''
wait for 30 sec and then run those ping commands again
---
#Ques 3
Save the files in folder and then run
'''bash
gcc dvr.c node0.c node1.c node2.c node3.c -o dvrouting
./dvrouting
'''
Then enter the trace value = 2
